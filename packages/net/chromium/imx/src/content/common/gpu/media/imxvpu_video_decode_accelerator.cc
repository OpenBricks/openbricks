#include "media/base/limits.h"
#include "base/bind.h"
#include "base/memory/singleton.h"
#include "content/common/gpu/media/imxvpu_video_decode_accelerator.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_implementation.h"


namespace content
{


namespace
{


class ImxVpuLoadSingleton
{
public:
	static ImxVpuLoadSingleton* GetInstance()
	{
		return Singleton < ImxVpuLoadSingleton > ::get();
	}

	bool Load()
	{
		base::AutoLock auto_lock(lock_);

		ImxVpuDecReturnCodes ret;

		if ((ret = imx_vpu_dec_load()) != IMX_VPU_DEC_RETURN_CODE_OK)
		{
			LOG(ERROR) << "Could not load VPU: " << imx_vpu_dec_error_string(ret);
			return false;
		}
		else
			return true;
	}

	bool Unload()
	{
		base::AutoLock auto_lock(lock_);

		ImxVpuDecReturnCodes ret;

		if ((ret = imx_vpu_dec_unload()) != IMX_VPU_DEC_RETURN_CODE_OK)
		{
			LOG(ERROR) << "Could not unload VPU: " << imx_vpu_dec_error_string(ret);
			return false;
		}
		else
			return true;
	}

private:
	ImxVpuLoadSingleton()
	{
	}

	friend struct DefaultSingletonTraits < ImxVpuLoadSingleton >;

	DISALLOW_COPY_AND_ASSIGN(ImxVpuLoadSingleton);

	base::Lock lock_;
};


} // unnamed namespace end


ImxVpuVideoDecodeAccelerator::ImxVpuVideoDecodeAccelerator(base::WeakPtr < gpu::gles2::GLES2Decoder > const gles2_decoder, base::Callback < bool(void) > const &make_context_current)
	: gles2_decoder_(gles2_decoder)
	, make_context_current_(make_context_current)
	, vpu_decoder_(NULL)
	, initial_info_received_(false)
	, message_loop_(base::MessageLoop::current())
{
	vpu_bitstream_buffer_block_.virtual_address = NULL;
}


ImxVpuVideoDecodeAccelerator::~ImxVpuVideoDecodeAccelerator()
{
	DCHECK_EQ(message_loop_, base::MessageLoop::current());
}


bool ImxVpuVideoDecodeAccelerator::Initialize(media::VideoCodecProfile profile, Client *client)
{
	gfx::GLContext *context;

	DCHECK_EQ(message_loop_, base::MessageLoop::current());

	client_ptr_factory_.reset(new base::WeakPtrFactory < Client > (client));
	client_ = client_ptr_factory_->GetWeakPtr();

	base::AutoLock auto_lock(lock_);

	LOG(INFO) << "Initializing i.MX VPU decoder";

	if (!make_context_current_.Run())
	{
		LOG(ERROR) << "Failed to make this decoder's GL context current.";
		return false;
	}

	if ((context = gles2_decoder_->GetGLContext()) == NULL)
	{
		LOG(ERROR) << "GLES2 context is NULL";
		return false;
	}

	if (!init_viv_direct_texture(*context, direct_texture_procs_))
	{
		LOG(ERROR) << "Initializing the direct texture extension failed";
		return false;
	}

	if ((profile >= media::H264PROFILE_MIN) && (profile <= media::H264PROFILE_MAX))
	{
		codec_format_ = IMX_VPU_CODEC_FORMAT_H264;
		VLOG(1) << "Setting h.264 as codec format";
	}
	else if ((profile >= media::VP8PROFILE_MIN) && (profile <= media::VP8PROFILE_MAX))
	{
		codec_format_ = IMX_VPU_CODEC_FORMAT_VP8;
		VLOG(1) << "Setting VP8 as codec format";
	}
	else
	{
		VLOG(1) << "Unsupported profile";
		return false;
	}

	VLOG(1) << "Loading VPU";
	if (!(ImxVpuLoadSingleton::GetInstance()->Load()))
		return false;

	if (!AllocateVpuBitstreamBuffer())
	{
		ImxVpuLoadSingleton::GetInstance()->Unload();
		return false;
	}

	VLOG(1) << "Opening decoder";
	if (!OpenDecoder())
	{
		ImxVpuLoadSingleton::GetInstance()->Unload();
		return false;
	}

	VLOG(1) << "Initialization done";
#ifdef IMXVPU_NEEDS_NOTIFY_INIT
	message_loop_->PostTask(FROM_HERE, base::Bind(&Client::NotifyInitializeDone, client_));
#endif

	return true;
}


void ImxVpuVideoDecodeAccelerator::Decode(media::BitstreamBuffer const &bitstream_buffer)
{
	VLOG(3) << "Decoding bitstream buffer";

	base::AutoLock auto_lock(lock_);

	input_queue_.push(bitstream_buffer);
	ProcessQueuedInput();
}


void ImxVpuVideoDecodeAccelerator::AssignPictureBuffers(std::vector < media::PictureBuffer > const &buffers)
{
	DCHECK(output_picture_buffers_.empty());

	base::AutoLock auto_lock(lock_);

	VLOG(1) << buffers.size() << " picture buffers are being provided by the client";

	// TODO: is the size of "buffers" always exactly the same size as the one
	// requested in ProvidePictureBuffers? In other words, is
	// buffers.size() always the same as the size of the vpu_framebuffers_ array?
	for (size_t i = 0; i < buffers.size(); ++i)
	{
		int32 id = buffers[i].id();

		output_picture_buffers_.insert(std::make_pair(id, buffers[i]));

		ImxVpuFramebuffer &framebuffer = vpu_framebuffers_[i];
		framebuffer.user_data = reinterpret_cast < void* > (id);

		VLOG(2) << "Associating picture buffer " << i << "/" << buffers.size() << " ID " << id << " with framebuffer";
	}

	ProcessQueuedInput();
}


void ImxVpuVideoDecodeAccelerator::ReusePictureBuffer(int32 picture_buffer_id)
{
	base::AutoLock auto_lock(lock_);

	VLOG(3) << "Reusing picture buffer with ID " << picture_buffer_id;

	for (size_t i = 0; i < vpu_framebuffers_.size(); ++i)
	{
		ImxVpuFramebuffer &framebuffer = vpu_framebuffers_[i];
		int32 id = reinterpret_cast < int32 > (framebuffer.user_data);
		if (id == picture_buffer_id)
		{
			ImxVpuDecReturnCodes ret;

			if ((ret = imx_vpu_dec_mark_framebuffer_as_displayed(vpu_decoder_, &framebuffer)) != IMX_VPU_DEC_RETURN_CODE_OK)
			{
				LOG(ERROR) << "Marking framebuffer for picture buffer with ID " << picture_buffer_id << "as displayed failed : " << imx_vpu_dec_error_string(ret);
			}
			else
				ProcessQueuedInput();

			return;
		}
	}

	LOG(WARNING) << "Picture buffer ID " << picture_buffer_id << " could not be associated with a framebuffer";
}


void ImxVpuVideoDecodeAccelerator::Flush()
{
	base::AutoLock auto_lock(lock_);

	VLOG(2) << "Flush: processing all currently queued input bitstream buffers";
	ProcessQueuedInput();

	VLOG(2) << "Flush: draining VPU decoder";
	imx_vpu_dec_set_drain_mode(vpu_decoder_, 1);
	while (true)
	{
		ProcessRetval pretval = ProcessInput(NULL);
		if (pretval != ProcessOK)
			break;
		// TODO: handle ProcessFail (ProcessEOS should just cause the loop to terminate)
	}
	imx_vpu_dec_set_drain_mode(vpu_decoder_, 0);

	VLOG(2) << "Flush: done";
	base::MessageLoop::current()->PostTask(
		FROM_HERE,
		base::Bind(
			&Client::NotifyFlushDone,
			client_
		)
	);

}


void ImxVpuVideoDecodeAccelerator::Reset()
{
	base::AutoLock auto_lock(lock_);

	VLOG(2) << "Reset: flushing decoder";
	imx_vpu_dec_flush(vpu_decoder_);

	VLOG(2) << "Reset: ending all queued bitstream buffers";
	while (!input_queue_.empty())
	{
		int32 bitstream_buffer_id = input_queue_.front().id();
		input_queue_.pop();

		if (bitstream_buffer_id != -1)
		{
			base::MessageLoop::current()->PostTask(
				FROM_HERE,
				base::Bind(
					&Client::NotifyEndOfBitstreamBuffer,
					client_, bitstream_buffer_id
				)
			);
		}
	}

	BitstreamBufferQueue empty_queue;
	std::swap(input_queue_, empty_queue);

	VLOG(2) << "Reset: done";
	base::MessageLoop::current()->PostTask(
		FROM_HERE,
		base::Bind(
			&Client::NotifyResetDone,
			client_
		)
	);
}


void ImxVpuVideoDecodeAccelerator::Destroy()
{
	DCHECK_EQ(message_loop_, base::MessageLoop::current());
	Cleanup();
	delete this;
}


#ifndef IMXVPU_WITH_VDA_IMPL
bool ImxVpuVideoDecodeAccelerator::CanDecodeOnIOThread()
{
	return false;
}
#endif


void ImxVpuVideoDecodeAccelerator::Cleanup()
{
	DCHECK_EQ(message_loop_, base::MessageLoop::current());

	base::AutoLock auto_lock(lock_);
	client_ptr_factory_.reset();

	CloseDecoder();

	DeallocateVpuBitstreamBuffer();

	ImxVpuLoadSingleton::GetInstance()->Unload();
}


bool ImxVpuVideoDecodeAccelerator::OpenDecoder()
{
	lock_.AssertAcquired();

	ImxVpuDecOpenParams open_params;

	open_params.codec_format = codec_format_;

	open_params.enable_frame_reordering = (codec_format_ == IMX_VPU_CODEC_FORMAT_H264) ? 1 : 0;

	// frame width & height are read from the bitstream
	open_params.frame_width = 0;
	open_params.frame_height = 0;

	if (imx_vpu_dec_open(&(vpu_decoder_), &open_params, vpu_bitstream_buffer_block_.virtual_address, vpu_bitstream_buffer_block_.physical_address) != IMX_VPU_DEC_RETURN_CODE_OK)
		return false;

	return true;
}


void ImxVpuVideoDecodeAccelerator::CloseDecoder()
{
	lock_.AssertAcquired();

	if (vpu_decoder_ == NULL)
		return;

	imx_vpu_dec_close(vpu_decoder_);
	DeallocateVpuFramebuffers();

	vpu_decoder_ = NULL;
}


bool ImxVpuVideoDecodeAccelerator::AllocateVpuBitstreamBuffer()
{
	lock_.AssertAcquired();

	ImxVpuDecReturnCodes ret;

	imx_vpu_dec_get_bitstream_buffer_info(&(vpu_bitstream_buffer_block_.alignment), &(vpu_bitstream_buffer_block_.size));
	if ((ret = imx_vpu_dec_allocate_memory(&vpu_bitstream_buffer_block_)) != IMX_VPU_DEC_RETURN_CODE_OK)
	{
		LOG(ERROR) << "Allocating VPU bitstream buffer failed: " << imx_vpu_dec_error_string(ret);
		return false;
	}
	else
		return true;
}


bool ImxVpuVideoDecodeAccelerator::DeallocateVpuBitstreamBuffer()
{
	lock_.AssertAcquired();

	ImxVpuDecReturnCodes ret;

	if (vpu_bitstream_buffer_block_.virtual_address == NULL)
		return true;

	if ((ret = imx_vpu_dec_deallocate_memory(&vpu_bitstream_buffer_block_)) != IMX_VPU_DEC_RETURN_CODE_OK)
	{
		LOG(ERROR) << "Deallocating VPU bitstream buffer failed: " << imx_vpu_dec_error_string(ret);
		return false;
	}
	else
		return true;
}


bool ImxVpuVideoDecodeAccelerator::AllocateAndRegisterVPUFramebuffers()
{
	lock_.AssertAcquired();

	ImxVpuDecReturnCodes ret;
	unsigned int y_stride, cbcr_stride, y_size, cbcr_size, mvcol_size, total_size;

	aligned_width_ = vpu_dec_initial_info_.frame_width;
	aligned_height_ = vpu_dec_initial_info_.frame_height;

	imx_vpu_dec_calc_framebuffer_sizes(
		&vpu_dec_initial_info_,
		&aligned_width_, &aligned_height_,
		&y_stride, &cbcr_stride,
		&y_size, &cbcr_size,
		&mvcol_size,
		&total_size
	);

	vpu_framebuffers_.resize(vpu_dec_initial_info_.min_num_required_framebuffers + media::limits::kMaxVideoFrames);
	vpu_framebuffer_mem_blocks_.resize(vpu_framebuffers_.size());
	memset(&(vpu_framebuffers_[0]), 0, sizeof(ImxVpuFramebuffer) * vpu_framebuffers_.size());
	memset(&(vpu_framebuffer_mem_blocks_[0]), 0, sizeof(ImxVpuMemBlock) * vpu_framebuffers_.size());

	for (unsigned int i = 0; i < vpu_framebuffers_.size(); ++i)
	{
		ImxVpuFramebuffer &framebuffer = vpu_framebuffers_[i];
		ImxVpuMemBlock &memblock = vpu_framebuffer_mem_blocks_[i];

		memblock.size = total_size;
		memblock.alignment = vpu_dec_initial_info_.framebuffer_alignment;
		if (imx_vpu_dec_allocate_memory(&memblock) != IMX_VPU_DEC_RETURN_CODE_OK)
		{
			LOG(ERROR) << "Could not allocate framebuffer memory for framebuffer " << i << "/" << vpu_framebuffers_.size();
			memblock.physical_address = 0;
			return false;
		}

		framebuffer.y_stride = y_stride;
		framebuffer.cbcr_stride = cbcr_stride;
		framebuffer.virtual_address = memblock.virtual_address;
		framebuffer.physical_address = memblock.physical_address;
		framebuffer.y_offset = 0;
		framebuffer.cb_offset = y_size;
		framebuffer.cr_offset = y_size + cbcr_size;
		framebuffer.mvcol_offset = y_size + cbcr_size * 2;
	}

	if ((ret = imx_vpu_dec_register_framebuffers(vpu_decoder_, &(vpu_framebuffers_[0]), vpu_framebuffers_.size())) != IMX_VPU_DEC_RETURN_CODE_OK)
	{
		LOG(ERROR) << "Registering framebuffers failed: " << imx_vpu_dec_error_string(ret);
		return false;
	}
	else
		return true;
}


bool ImxVpuVideoDecodeAccelerator::DeallocateVpuFramebuffers()
{
	lock_.AssertAcquired();

	for (unsigned int i = 0; i < vpu_framebuffer_mem_blocks_.size(); ++i)
	{
		ImxVpuMemBlock &memblock = vpu_framebuffer_mem_blocks_[i];
		if (memblock.physical_address != 0)
		{
			if (imx_vpu_dec_deallocate_memory(&memblock) != IMX_VPU_DEC_RETURN_CODE_OK)
			{
				LOG(ERROR) << "Deallocating memory block of framebuffer " << i << "/" << vpu_framebuffer_mem_blocks_.size() << " failed";
			}
		}
	}
	vpu_framebuffer_mem_blocks_.clear();

	return true;
}


void ImxVpuVideoDecodeAccelerator::ProcessQueuedInput()
{
	lock_.AssertAcquired();

	VLOG(1) << "Input queue size: " << input_queue_.size();

	{
		if (input_queue_.empty())
		{
			VLOG(1) << "Input queue empty - nothing to process";
			return;
		}

		if (initial_info_received_ && output_picture_buffers_.empty())
		{
			VLOG(1) << "No picture buffers have been provided yet - will try again later";
			return;
		}

		if (initial_info_received_ && (imx_vpu_dec_get_num_free_framebuffers(vpu_decoder_) < imx_vpu_dec_get_min_num_free_required(vpu_decoder_)))
		{
			VLOG(1) << "Not enough free framebuffers available - will try again later";
			return;
		}

		media::BitstreamBuffer const &queued_bitstream_buffer = input_queue_.front();
		ProcessRetval ret = ProcessInput(&queued_bitstream_buffer);
		input_queue_.pop();

		if (ret != ProcessOK)
			return;
	}
}


ImxVpuVideoDecodeAccelerator::ProcessRetval ImxVpuVideoDecodeAccelerator::ProcessInput(media::BitstreamBuffer const *input_bitstream_buffer)
{
	ImxVpuDecReturnCodes ret;

	lock_.AssertAcquired();


	ImxVpuEncodedFrame encoded_frame;
	scoped_ptr < base::SharedMemory > shm;

	// bitstream_buffer is NULL while draining
	if (input_bitstream_buffer != NULL)
	{
		VLOG(3) << "Processing input bitstream buffer with ID " << input_bitstream_buffer->id();

		shm.reset(new base::SharedMemory(input_bitstream_buffer->handle(), true));
		// The shared memory block is automatically unmapped by the destructor
		shm->Map(input_bitstream_buffer->size());

		// The stored bitstream IDs are incremented to make debug output clearer
		// (otherwise, the imxvpucodec debug printout will print (nil) for the ID 0,
		// 0x1 for the ID 1 etc.)
		encoded_frame.virtual_address = shm->memory();
		encoded_frame.data_size = input_bitstream_buffer->size();
		encoded_frame.user_data = reinterpret_cast < void* > (input_bitstream_buffer->id() + 1);

		VLOG(3) << "Creating encoded frame (data address " << encoded_frame.virtual_address << " size " << encoded_frame.data_size << " id " << input_bitstream_buffer->id();
	}
	else
	{
		VLOG(2) << "Setting input data to NULL";

		encoded_frame.virtual_address = NULL;
		encoded_frame.data_size = 0;
		encoded_frame.user_data = NULL;
	}

	encoded_frame.codec_data = NULL;
	encoded_frame.codec_data_size = 0;


	unsigned int output_code;
	if ((ret = imx_vpu_dec_decode_frame(vpu_decoder_, &encoded_frame, &output_code)) != IMX_VPU_DEC_RETURN_CODE_OK)
	{
		LOG(ERROR) << "Decoding frame failed: " << imx_vpu_dec_error_string(ret);
		return ProcessFailed;
	}

	VLOG(1) << "Output code of decoded frame: 0x" << std::hex << output_code;

	if (input_bitstream_buffer != NULL)
	{
		base::MessageLoop::current()->PostTask(
			FROM_HERE,
			base::Bind(
				&Client::NotifyEndOfBitstreamBuffer,
				client_,
				input_bitstream_buffer->id()
			)
		);
	}

	if (output_code & (IMX_VPU_DEC_OUTPUT_CODE_INITIAL_INFO_AVAILABLE | IMX_VPU_DEC_OUTPUT_CODE_RESOLUTION_CHANGED))
	{
		VLOG(1) << "Initial information is available - retrieving";

		if ((ret = imx_vpu_dec_get_initial_info(vpu_decoder_, &vpu_dec_initial_info_)) != IMX_VPU_DEC_RETURN_CODE_OK)
		{
			LOG(ERROR) << "Retrieving initial info failed: " << imx_vpu_dec_error_string(ret);
			return ProcessFailed;
		}

		VLOG(1) << "Initial info: frame size " << vpu_dec_initial_info_.frame_width << "x" << vpu_dec_initial_info_.frame_height << "  min num required framebuffers: " << vpu_dec_initial_info_.min_num_required_framebuffers;

		if (
			!DeallocateVpuFramebuffers() ||
			!AllocateAndRegisterVPUFramebuffers()
		)
			return ProcessFailed;

		initial_info_received_ = true;

		base::MessageLoop::current()->PostTask(
			FROM_HERE,
			base::Bind(
				&Client::ProvidePictureBuffers,
				client_,
				vpu_framebuffers_.size(),
				gfx::Size(aligned_width_, aligned_height_),
				GL_TEXTURE_2D
			)
		);
	}

	ProcessRetval pretval = (output_code & IMX_VPU_DEC_OUTPUT_CODE_EOS) ? ProcessEOS : ProcessOK;

	int32 bitstream_buffer_id = 0;
	if (output_code & IMX_VPU_DEC_OUTPUT_CODE_FRAME_OUTPUT)
	{
		ImxVpuDecodedFrame decoded_frame;
		if ((ret = imx_vpu_dec_get_decoded_frame(vpu_decoder_, &decoded_frame)) != IMX_VPU_DEC_RETURN_CODE_OK)
		{
			LOG(ERROR) << "Retrieving decoded frame failed: " << imx_vpu_dec_error_string(ret);
			return ProcessFailed;
		}

		bitstream_buffer_id = reinterpret_cast < int32 > (decoded_frame.user_data) - 1;

		VLOG(3) << "Decoded frame was retrieved, bitstream buffer id " << bitstream_buffer_id;

		if (decoded_frame.framebuffer == NULL)
		{
			LOG(ERROR) << "Framebuffer of decoded frame is NULL";
			pretval = ProcessFailed;
		}
		else
		{
			if (!ProcessOutput(*(decoded_frame.framebuffer), bitstream_buffer_id))
			{
				// if ProcessOutput returns false, then no picture buffer has
				// been sent to the client, so the decoded frame must be returned
				// to the VPU pool here
				VLOG(1) << "ProcessOutput failed -> returning decoded frame to internal VPU pool";
				imx_vpu_dec_mark_framebuffer_as_displayed(vpu_decoder_, decoded_frame.framebuffer);
				pretval = ProcessFailed;
			}

			// if processing the output was successful, the framebuffer is
			// _not_ marked as displayed here; this is done in ReusePictureBuffer(),
			// because only then it is certain that the client is done with that frame
		}
	}
	else if (output_code & IMX_VPU_DEC_OUTPUT_CODE_DROPPED)
	{
		void *user_data = imx_vpu_dec_get_dropped_frame_user_data(vpu_decoder_);
		bitstream_buffer_id = reinterpret_cast < int32 > (user_data) - 1;
		VLOG(2) << "Frame was dropped, bitstream buffer id " << bitstream_buffer_id;
	}

	return pretval;
}


bool ImxVpuVideoDecodeAccelerator::ProcessOutput(ImxVpuFramebuffer const &output_framebuffer, int32 input_bitstream_buffer_id)
{
	lock_.AssertAcquired();

	int32 picture_buffer_id = reinterpret_cast < int32 > (output_framebuffer.user_data);
	OutputBufferMap::const_iterator iter = output_picture_buffers_.find(picture_buffer_id);
	if (iter == output_picture_buffers_.end())
	{
		LOG(ERROR) << "No picture buffer with ID " << picture_buffer_id << " found";
		return false;
	}
	GLuint picture_buffer_texture_id = iter->second.texture_id();

	make_context_current_.Run();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, picture_buffer_texture_id);

	GLvoid *virt_addr = output_framebuffer.virtual_address;
	GLuint phys_addr = reinterpret_cast < GLuint > (output_framebuffer.physical_address);

	direct_texture_procs_.TexDirectVIVMap(
		GL_TEXTURE_2D,
		aligned_width_, aligned_height_,
		GL_VIV_I420,
		&virt_addr, &phys_addr
	);
	direct_texture_procs_.TexDirectInvalidateVIV(GL_TEXTURE_2D);

	gles2_decoder_->RestoreTextureUnitBindings(0);
	gles2_decoder_->RestoreActiveTexture();

	base::MessageLoop::current()->PostTask(
		FROM_HERE,
		base::Bind(
			&Client::PictureReady,
			client_,
			media::Picture(picture_buffer_id, input_bitstream_buffer_id)
		)
	);

	return true;
}


}  // namespace content
