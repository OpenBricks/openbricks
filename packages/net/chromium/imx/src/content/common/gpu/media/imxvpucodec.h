/*
 * imxvpucodec - i.MX6 VPU hardware codec engine API library
 * Copyright (c) 2014 Carlos Rafael Giani
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute
 * it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must
 *    not claim that you wrote the original software. If you use this
 *    software in a product, an acknowledgment in the product
 *    documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must
 *    not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */


#ifndef IMXVPUCODEC_H
#define IMXVPUCODEC_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif



/* This library provides a high-level interface for controlling the Freescale
 * i.MX VPU en/decoder.
 * Other libraries do not provide a way of associating frames with user defined
 * information, and lack calls to check the number of currently free framebuffers
 * (when decoding). The former is required by many media frameworks such as
 * GStreamer, FFmpeg/libav, the Chromium media codebase etc. The latter is
 * necessary when framebuffer display and decoding can happen in different
 * threads (the counter makes it possible to use synchronization primitives
 * like thread condition variables to wait until enough frames are free).
 *
 * Note that the functions are _not_ thread safe. If they may be called from
 * different threads, you must make sure they are surrounded by a mutex lock.
 * It is recommended to use one global mutex for the imx_vpu_*_load()/unload()
 * functions, and another de/encoder instance specific mutex for all of the other
 * calls.
 *
 * How to use the decoder (error handling omitted for clarity):
 * 1. Call imx_vpu_dec_load()
 * 2. Call imx_vpu_dec_get_bitstream_buffer_info(), and allocate a DMA buffer
 *    with the given size and alignment.
 * 3. Fill an instance of ImxVpuDecOpenParams with the values specific to the
 *    input data. In most cases, one wants to set enable_frame_reordering to 1
 *    with h.264 data here.
 * 4. Call imx_vpu_dec_open(), passing in a pointer to the filled ImxVpuDecOpenParams
 *    instance, and the virtual and physical addresses of the bitstream DMA buffer
 *    which was allocated in step 2.
 * 5. Call imx_vpu_dec_decode_frame() with the first encoded frame.
 *    If the output_code bitmask contains IMX_VPU_DEC_OUTPUT_CODE_INITIAL_INFO_AVAILABLE,
 *    proceed, otherwise continue feeding in data.
 * 6. Once IMX_VPU_DEC_OUTPUT_CODE_INITIAL_INFO_AVAILABLE has been set in the output code,
 *    call imx_vpu_dec_get_initial_info() with a pointer to an ImxVpuDecInitialInfo
 *    instance.
 * 7. (Optional) Perform the necessary size and alignment calculations by calling
 *    imx_vpu_dec_calc_framebuffer_sizes().
 * 8. Create an array of at least as many ImxVpuFramebuffer instances as specified in
 *    min_num_required_framebuffers. Each instance must point to a DMA buffer that is big
 *    enough to hold a frame. If step 7 was performed, allocating as many bytes as indicated
 *    by total_size is enough. Make sure the Y,Cb,Cr,MvCol offsets in each ImxVpuFramebuffer
 *    instance are valid.
 * 9. Call imx_vpu_dec_register_framebuffers() and pass in the ImxVpuFramebuffer array
 *    and the number of ImxVpuFramebuffer instances.
 * 10. Continue calling imx_vpu_dec_decode_frame(). The virtual address in encoded_frame
 *     must not be NULL.
 *     If the IMX_VPU_DEC_OUTPUT_CODE_FRAME_OUTPUT flag is set in the output code,
 *     call imx_vpu_dec_get_decoded_frame() with a pointer to an ImxVpuDecodedFrame instance
 *     which gets filled with information about the decoded frame. Once the decoded frame
 *     has been processed by the user, imx_vpu_dec_mark_framebuffer_as_displayed() must be
 *     called to let the decoder know that the framebuffer is available for storing new
 *     decoded frames again.
 *     If IMX_VPU_DEC_OUTPUT_CODE_DROPPED is set, you can call
 *     imx_vpu_dec_get_dropped_frame_user_data() to retrieve the user_data field
 *     of the dropped frame. If IMX_VPU_DEC_OUTPUT_CODE_EOS is set, stop playback and close
 *     the decoder.
 * 11. In case a flush/reset is desired (typically after seeking), call imx_vpu_dec_flush().
 *     Note that any internal user_data pointers from the en/decoded frames will be
 *     set to NULL after this call (this is the only exception where the library modifies
 *     the user_data fields).
 * 12. When there is no more incoming data, and pending decoded frames need to be retrieved
 *     from the decoder, call imx_vpu_dec_set_drain_mode(). This is typically necessary when
 *     the data source reached its end, playback is finishing, and there is a delay
 *     of N frames at the beginning.
 *     After this call, continue calling imx_vpu_dec_decode_frame() to retrieve the pending
 *     decoded frames, but the virtual address of encoded_frame must be NULL.
 *     As in step 10, if IMX_VPU_DEC_OUTPUT_CODE_EOS is set, stop playback, close the decoder.
 * 13. After playback is finished, close the decoder with imx_vpu_dec_close().
 * 14. Deallocate framebuffer memory blocks and the bitstream buffer memory block.
 * 15. Call imx_vpu_dec_unload().
 *
 * Step 15 should only be called if no more playback sessions will occur.
 *
 * As mentioned before, in situations where decoding and display of decoded frames happen in
 * different thread, it is necessary to let the decoder wait until enough framebuffers
 * are free (= available for the VPU to decode into). This is typically done by such a check
 * (in pseudo code):
 *
 *   mutex_lock(&mutex);
 *
 *   while (imx_vpu_dec_get_num_free_framebuffers(decoder) < imx_vpu_dec_get_min_num_free_required(decoder))
 *     condition_wait(&condition_variable, &mutex);
 *
 *   imx_vpu_dec_decode_frame(decoder, encoded_frame, &output_code);
 *   ...
 *
 *   mutex_unlock(&mutex);
 */



/***********************************************/
/******* COMMON STRUCTURES AND FUNCTIONS *******/
/***********************************************/


#define IMX_VPU_ALIGN_VAL_TO(LENGTH, ALIGN_SIZE)  ( ((uintptr_t)(((uint8_t*)(LENGTH)) + (ALIGN_SIZE) - 1) / (ALIGN_SIZE)) * (ALIGN_SIZE) )


typedef uint32_t imx_vpu_phys_addr_t;
typedef uint32_t imx_vpu_cpu_addr_t; /* used only in allocators so far */


typedef enum
{
	IMX_VPU_PIC_TYPE_UNKNOWN = 0,
	IMX_VPU_PIC_TYPE_I,
	IMX_VPU_PIC_TYPE_P,
	IMX_VPU_PIC_TYPE_B,
	IMX_VPU_PIC_TYPE_IDR,
	IMX_VPU_PIC_TYPE_BI,
	IMX_VPU_PIC_TYPE_SKIP
}
ImxVpuPicType;


typedef enum
{
	IMX_VPU_CODEC_FORMAT_MPEG2 = 0, /* includes MPEG1 */
	IMX_VPU_CODEC_FORMAT_MPEG4,
	IMX_VPU_CODEC_FORMAT_H263,
	IMX_VPU_CODEC_FORMAT_H264,
	IMX_VPU_CODEC_FORMAT_H264_MVC,
	IMX_VPU_CODEC_FORMAT_WMV3,
	IMX_VPU_CODEC_FORMAT_WVC1,
	IMX_VPU_CODEC_FORMAT_MJPEG,
	IMX_VPU_CODEC_FORMAT_VP8
	/* XXX others will be added when the firmware supports them */
}
ImxVpuCodecFormats;


typedef enum
{
	IMX_VPU_MJPEG_FORMAT_YUV420            = 0, /* also known as I420 */
	IMX_VPU_MJPEG_FORMAT_YUV422_HORIZONTAL = 1,
	IMX_VPU_MJPEG_FORMAT_YUV422_VERTICAL   = 2, /* 4:2:2 vertical, actually 2:2:4 (according to the VPU docs) */
	IMX_VPU_MJPEG_FORMAT_YUV444            = 3,
	IMX_VPU_MJPEG_FORMAT_YUV400            = 4  /* 8-bit grayscale */
}
ImxVpuMJpegFormat;


typedef struct
{
	/* Stride of the Y and of the Cb&Cr components.
	 * Specified in bytes. */
	unsigned int y_stride, cbcr_stride;

	/* The virtual address of is actually not used by the VPU,
	 * and only of interest to the user. It can be NULL for cases
	 * where only a physical address exists or where a virtual
	 * address is not necessary. */
	void *virtual_address;
	/* The physical address must always be valid. */
	imx_vpu_phys_addr_t physical_address;

	/* These define the starting offsets of each component
	 * relative to the start of the buffer. Specified in bytes.
	 *
	 * mvcol is the "co-located motion vector" data. */
	size_t
		y_offset,
		cb_offset,
		cr_offset,
		mvcol_offset;

	/* User-defined pointer. The library does not touch this value.
	 * This can be used to identify framebuffers for example.
	 * Not to be confused with the user_data fields of ImxVpuEncodedFrame
	 * and ImxVpuDecodedFrame. */
	void *user_data;

	/* Internal, implementation-defined data. Do not modify. */
	void *internal;
}
ImxVpuFramebuffer;


typedef struct
{
	/* Virtual and physical addresses pointing to the encoded data.
	 * The virtual address must always be valid. The physical address
	 * is only required for encoding. */
	void *virtual_address;
	imx_vpu_phys_addr_t physical_address;

	/* Size of the encoded data, in bytes. */
	unsigned int data_size;

	/* Pointer to out-of-band codec/header data. If such data exists,
	 * specify the pointer to the memory block containing the data,
	 * as well as the size of the memory block (in bytes).
	 * Set pointer and size for every encoded frame when decoding.
	 * If no such data exists or is required, set pointer to NULL and
	 * size to 0. */
	void *codec_data;
	unsigned int codec_data_size;

	/* User-defined pointer. The library does not touch this value.
	 * This pointer and the one from the corresponding
	 * decoded frame will have the same value. The library will
	 * pass then through.
	 * It can be used to identify frames and associated corresponding
	 * en- and decoded frames for example. */
	void *user_data;
}
ImxVpuEncodedFrame;


typedef struct
{
	/* When decoding: pointer to the framebuffer containing the decoded frame.
	 * When encoding: pointer to the framebuffer containing the frame to be encoded.
	 * Must always be valid. */
	ImxVpuFramebuffer *framebuffer;

	/* picture type (I, P, B, ..) */
	ImxVpuPicType pic_type;

	/* User-defined pointer. The library does not touch this value.
	 * This pointer and the one from the corresponding
	 * encoded frame will have the same value. The library will
	 * pass then through.
	 * It can be used to identify frames and associated corresponding
	 * en- and decoded frames for example. */
	void *user_data;
}
ImxVpuDecodedFrame;


/* This structure is only used by the allocate/deallocate calls below,
 * which in turn are convenience calls that wrap VPU-provided DMA buffer
 * allocators. If a different DMA buffer allocator is used (like ION),
 * this structure does not have to be used. */
typedef struct
{
	size_t size;
	unsigned int alignment;

	void*               virtual_address;
	imx_vpu_phys_addr_t physical_address;
	imx_vpu_cpu_addr_t  cpu_address;

	void*               virtual_address_unaligned;
	imx_vpu_phys_addr_t physical_address_unaligned;
}
ImxVpuMemBlock;




/************************************************/
/******* DECODER STRUCTURES AND FUNCTIONS *******/
/************************************************/


typedef struct _ImxVpuDecoder ImxVpuDecoder;


typedef enum
{
	IMX_VPU_DEC_RETURN_CODE_OK = 0,
	IMX_VPU_DEC_RETURN_CODE_ERROR,
	IMX_VPU_DEC_RETURN_CODE_INVALID_PARAMS,
	IMX_VPU_DEC_RETURN_CODE_INVALID_HANDLE,
	IMX_VPU_DEC_RETURN_CODE_INVALID_FRAMEBUFFER,
	IMX_VPU_DEC_RETURN_CODE_INSUFFICIENT_FRAMEBUFFERS,
	IMX_VPU_DEC_RETURN_CODE_INVALID_STRIDE,
	IMX_VPU_DEC_RETURN_CODE_WRONG_CALL_SEQUENCE,
	IMX_VPU_DEC_RETURN_CODE_TIMEOUT
}
ImxVpuDecReturnCodes;


typedef enum
{
	IMX_VPU_DEC_OUTPUT_CODE_INPUT_USED               = (1UL << 0),
	IMX_VPU_DEC_OUTPUT_CODE_EOS                      = (1UL << 1),
	IMX_VPU_DEC_OUTPUT_CODE_FRAME_OUTPUT             = (1UL << 2),
	IMX_VPU_DEC_OUTPUT_CODE_NO_FRAME_OUTPUT          = (1UL << 3),
	IMX_VPU_DEC_OUTPUT_CODE_DROPPED                  = (1UL << 4),
	IMX_VPU_DEC_OUTPUT_CODE_NOT_ENOUGH_OUTPUT_FRAMES = (1UL << 5),
	IMX_VPU_DEC_OUTPUT_CODE_NOT_ENOUGH_INPUT_DATA    = (1UL << 6),
	IMX_VPU_DEC_OUTPUT_CODE_INITIAL_INFO_AVAILABLE   = (1UL << 7),
	IMX_VPU_DEC_OUTPUT_CODE_RESOLUTION_CHANGED       = (1UL << 8),
	IMX_VPU_DEC_OUTPUT_CODE_DECODE_ONLY              = (1UL << 9),
	IMX_VPU_DEC_OUTPUT_CODE_INTERNAL_RESET           = (1UL << 10)
}
ImxVpuDecOutputCodes;


typedef struct
{
	ImxVpuCodecFormats codec_format;

	int enable_frame_reordering;
	unsigned int frame_width, frame_height;
}
ImxVpuDecOpenParams;


typedef struct
{
	/* Width of height of frames, in pixels. */
	unsigned int frame_width, frame_height;
	/* Frame rate ratio. */
	unsigned int frame_rate_numerator, frame_rate_denominator;

	/* Caller must register at least this many framebuffers
	 * with the decoder. */
	unsigned int min_num_required_framebuffers;

	/* Pixel format of the decoded frames. For codec formats
	 * other than motion JPEG, this value will always be
	 * IMX_VPU_MJPEG_FORMAT_YUV420. */
	ImxVpuMJpegFormat mjpeg_source_format;

	/* 0 = no interlacing, 1 = interlacing. */
	int interlacing;

	/* Fixed point, shifted by 16.
	 * Example: 1.0 -> floor(1.0*(1<<16)) = 0x10000
	 *          0.5 -> floor(0.5*(1<<16)) = 0x8000 */
	unsigned int width_height_ratio;

	/* Physical framebuffer addresses must be aligned to this value. */
	unsigned int framebuffer_alignment;
}
ImxVpuDecInitialInfo;


/* Returns a human-readable description of the error code.
 * Useful for logging. */
char const * imx_vpu_dec_error_string(ImxVpuDecReturnCodes code);

/* These two functions load/unload the decoder. Thanks to an internal reference
 * counter, it is safe to call these functions more than once. However, the
 * number of unload() calls must match the number of load() calls.
 *
 * The decoder must be loaded before doing anything else with the decoder.
 * Similarly, the decoder must not be unloaded before all decoder activities
 * have been finished. This includes opening/decoding decoder instances. */
ImxVpuDecReturnCodes imx_vpu_dec_load(void);
ImxVpuDecReturnCodes imx_vpu_dec_unload(void);

/* Convenience allocator for allocating DMA buffers. */
ImxVpuDecReturnCodes imx_vpu_dec_allocate_memory(ImxVpuMemBlock *mem_block);
ImxVpuDecReturnCodes imx_vpu_dec_deallocate_memory(ImxVpuMemBlock *mem_block);

/* Called before imx_vpu_dec_open(), it returns the alignment and size for the
 * physical memory block necessary for the decoder's bitstream buffer. The user
 * must allocate a DMA buffer of at least this size, and its physical address
 * must be aligned according to the alignment value. */
void imx_vpu_dec_get_bitstream_buffer_info(unsigned int *alignment, size_t *size);

ImxVpuDecReturnCodes imx_vpu_dec_open(ImxVpuDecoder **decoder, ImxVpuDecOpenParams *open_params, void *bitstream_buffer_virtual_address, imx_vpu_phys_addr_t bitstream_buffer_physical_address);
ImxVpuDecReturnCodes imx_vpu_dec_close(ImxVpuDecoder *decoder);

ImxVpuDecReturnCodes imx_vpu_dec_set_drain_mode(ImxVpuDecoder *decoder, int enabled);
ImxVpuDecReturnCodes imx_vpu_dec_flush(ImxVpuDecoder *decoder);

ImxVpuDecReturnCodes imx_vpu_dec_register_framebuffers(ImxVpuDecoder *decoder, ImxVpuFramebuffer *framebuffers, unsigned int num_framebuffers);
void imx_vpu_dec_calc_framebuffer_sizes(ImxVpuDecInitialInfo *initial_info, unsigned int *frame_width, unsigned int *frame_height, unsigned int *y_stride, unsigned int *cbcr_stride, unsigned int *y_size, unsigned int *cbcr_size, unsigned int *mvcol_size, unsigned int *total_size);

ImxVpuDecReturnCodes imx_vpu_dec_get_initial_info(ImxVpuDecoder *decoder, ImxVpuDecInitialInfo *info);

ImxVpuDecReturnCodes imx_vpu_dec_decode_frame(ImxVpuDecoder *decoder, ImxVpuEncodedFrame *encoded_frame, unsigned int *output_code);
ImxVpuDecReturnCodes imx_vpu_dec_get_decoded_frame(ImxVpuDecoder *decoder, ImxVpuDecodedFrame *decoded_frame);
void* imx_vpu_dec_get_dropped_frame_user_data(ImxVpuDecoder *decoder);
int imx_vpu_dec_get_num_free_framebuffers(ImxVpuDecoder *decoder);
int imx_vpu_dec_get_min_num_free_required(ImxVpuDecoder *decoder);
ImxVpuDecReturnCodes imx_vpu_dec_mark_framebuffer_as_displayed(ImxVpuDecoder *decoder, ImxVpuFramebuffer *framebuffer);




#ifdef __cplusplus
}
#endif


#endif

