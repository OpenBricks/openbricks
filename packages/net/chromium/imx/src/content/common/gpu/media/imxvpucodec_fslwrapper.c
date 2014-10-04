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


#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <vpu_wrapper.h>
#include "imxvpucodec.h"
#include "imxvpucodec_platform.h"




/***********************************************/
/******* COMMON STRUCTURES AND FUNCTIONS *******/
/***********************************************/


#ifndef NULL
#define NULL ((void*)0)
#endif


#ifndef TRUE
#define TRUE (1)
#endif


#ifndef FALSE
#define FALSE (0)
#endif


#ifndef BOOL
#define BOOL int
#endif


static ImxVpuMJpegFormat convert_from_wrapper_mjpg_format(int format)
{
	return (ImxVpuMJpegFormat)format;
}


static ImxVpuPicType convert_from_wrapper_pic_type(VpuPicType type)
{
	switch (type)
	{
		case VPU_I_PIC:    return IMX_VPU_PIC_TYPE_I;
		case VPU_P_PIC:    return IMX_VPU_PIC_TYPE_P;
		case VPU_B_PIC:    return IMX_VPU_PIC_TYPE_B;
		case VPU_IDR_PIC:  return IMX_VPU_PIC_TYPE_IDR;
		case VPU_BI_PIC:   return IMX_VPU_PIC_TYPE_BI;
		case VPU_SKIP_PIC: return IMX_VPU_PIC_TYPE_SKIP;
		default: return IMX_VPU_PIC_TYPE_UNKNOWN;
	}
}


static VpuCodStd convert_to_wrapper_codec_std(ImxVpuCodecFormats format)
{
	switch (format)
	{
		case IMX_VPU_CODEC_FORMAT_MPEG4:    return VPU_V_MPEG4;
		case IMX_VPU_CODEC_FORMAT_H263:     return VPU_V_H263;
		case IMX_VPU_CODEC_FORMAT_H264:     return VPU_V_AVC;
		case IMX_VPU_CODEC_FORMAT_H264_MVC: return VPU_V_AVC_MVC;
		case IMX_VPU_CODEC_FORMAT_WMV3:     return VPU_V_VC1;
		case IMX_VPU_CODEC_FORMAT_WVC1:     return VPU_V_VC1_AP;
		case IMX_VPU_CODEC_FORMAT_MPEG2:    return VPU_V_MPEG2;
		case IMX_VPU_CODEC_FORMAT_MJPEG:    return VPU_V_MJPG;
		case IMX_VPU_CODEC_FORMAT_VP8:      return VPU_V_VP8;
		default: assert(FALSE);
	}

	return VPU_V_MPEG2; /* should never be reached */
}


static void convert_from_wrapper_mem_desc(VpuMemDesc *mem_desc, ImxVpuMemBlock *mem_block)
{
	mem_block->size = mem_desc->nSize;
	mem_block->virtual_address_unaligned = (void*)(mem_desc->nVirtAddr);
	mem_block->physical_address_unaligned = mem_desc->nPhyAddr;
	mem_block->cpu_address = mem_desc->nCpuAddr;
}


static void convert_to_wrapper_mem_desc(ImxVpuMemBlock *mem_block, VpuMemDesc *mem_desc)
{
	mem_desc->nSize = mem_block->size;
	mem_desc->nVirtAddr = (unsigned long)(mem_block->virtual_address_unaligned);
	mem_desc->nPhyAddr = mem_block->physical_address_unaligned;
	mem_desc->nCpuAddr = mem_block->cpu_address;
}


static void convert_to_wrapper_framebuffer(ImxVpuFramebuffer *fb, VpuFrameBuffer *wrapper_fb)
{
	memset(wrapper_fb, 0, sizeof(VpuFrameBuffer));

	wrapper_fb->nStrideY = fb->y_stride;
	wrapper_fb->nStrideC = fb->cbcr_stride;

	wrapper_fb->pbufY = (unsigned char*)(fb->physical_address + fb->y_offset);
	wrapper_fb->pbufCb = (unsigned char*)(fb->physical_address + fb->cb_offset);
	wrapper_fb->pbufCr = (unsigned char*)(fb->physical_address + fb->cr_offset);
	wrapper_fb->pbufMvCol = (unsigned char*)(fb->physical_address + fb->mvcol_offset);
}




/************************************************/
/******* DECODER STRUCTURES AND FUNCTIONS *******/
/************************************************/


#define MIN_NUM_FREE_FB_REQUIRED 6
#define FRAME_ALIGN 16


struct _ImxVpuDecoder
{
	VpuDecHandle handle;

	void *virt_mem_sub_block;
	size_t virt_mem_sub_block_size;

	ImxVpuCodecFormats codec_format;

	unsigned int num_framebuffers;
	VpuFrameBuffer **wrapper_framebuffers;
	ImxVpuFramebuffer *framebuffers;
	void **user_data_for_frames;
	void *pending_user_data;
	void *dropped_frame_user_data;
	int num_user_data;
	BOOL delay_pending_user_data;
	void *last_pending_user_data;

	BOOL consumption_info_available;
	BOOL flush_vpu_upon_reset;

	BOOL recalculate_num_avail_framebuffers;
	int num_available_framebuffers;
	int num_times_counter_decremented;
	int num_framebuffers_in_use;
};


static ImxVpuDecReturnCodes dec_convert_retcode(VpuDecRetCode code)
{
	switch (code)
	{
		case VPU_DEC_RET_SUCCESS:                    return IMX_VPU_DEC_RETURN_CODE_OK;
		case VPU_DEC_RET_FAILURE:                    return IMX_VPU_DEC_RETURN_CODE_ERROR;
		case VPU_DEC_RET_INVALID_PARAM:              return IMX_VPU_DEC_RETURN_CODE_INVALID_PARAMS;
		case VPU_DEC_RET_INVALID_HANDLE:             return IMX_VPU_DEC_RETURN_CODE_INVALID_HANDLE;
		case VPU_DEC_RET_INVALID_FRAME_BUFFER:       return IMX_VPU_DEC_RETURN_CODE_INVALID_FRAMEBUFFER;
		case VPU_DEC_RET_INSUFFICIENT_FRAME_BUFFERS: return IMX_VPU_DEC_RETURN_CODE_INSUFFICIENT_FRAMEBUFFERS;
		case VPU_DEC_RET_INVALID_STRIDE:             return IMX_VPU_DEC_RETURN_CODE_INVALID_STRIDE;
		case VPU_DEC_RET_WRONG_CALL_SEQUENCE:        return IMX_VPU_DEC_RETURN_CODE_WRONG_CALL_SEQUENCE;
		case VPU_DEC_RET_FAILURE_TIMEOUT:            return IMX_VPU_DEC_RETURN_CODE_TIMEOUT;

		default: return IMX_VPU_DEC_RETURN_CODE_ERROR;
	}
}


static unsigned int dec_convert_outcode(VpuDecBufRetCode code)
{
	/* TODO: REPEAT? SKIP? */
	unsigned int out = 0;
	if (code & VPU_DEC_INPUT_USED)         out |= IMX_VPU_DEC_OUTPUT_CODE_INPUT_USED;
	if (code & VPU_DEC_OUTPUT_EOS)         out |= IMX_VPU_DEC_OUTPUT_CODE_EOS;
	if (code & VPU_DEC_OUTPUT_DIS)         out |= IMX_VPU_DEC_OUTPUT_CODE_FRAME_OUTPUT;
	if (code & VPU_DEC_OUTPUT_NODIS)       out |= IMX_VPU_DEC_OUTPUT_CODE_NO_FRAME_OUTPUT;
	if (code & VPU_DEC_OUTPUT_DROPPED)     out |= IMX_VPU_DEC_OUTPUT_CODE_DROPPED;
	if (code & VPU_DEC_OUTPUT_MOSAIC_DIS)  out |= IMX_VPU_DEC_OUTPUT_CODE_DROPPED; /* mosaic frames are dropped */
	if (code & VPU_DEC_NO_ENOUGH_BUF)      out |= IMX_VPU_DEC_OUTPUT_CODE_NOT_ENOUGH_OUTPUT_FRAMES;
	if (code & VPU_DEC_NO_ENOUGH_INBUF)    out |= IMX_VPU_DEC_OUTPUT_CODE_NOT_ENOUGH_INPUT_DATA;
	if (code & VPU_DEC_INIT_OK)            out |= IMX_VPU_DEC_OUTPUT_CODE_INITIAL_INFO_AVAILABLE;
	if (code & VPU_DEC_RESOLUTION_CHANGED) out |= IMX_VPU_DEC_OUTPUT_CODE_RESOLUTION_CHANGED;
	return out;
}


static void dec_convert_to_wrapper_open_param(ImxVpuDecOpenParams *open_params, VpuDecOpenParam *wrapper_open_param)
{
	memset(wrapper_open_param, 0, sizeof(VpuDecOpenParam));
	
	wrapper_open_param->CodecFormat    = convert_to_wrapper_codec_std(open_params->codec_format);
	wrapper_open_param->nReorderEnable = open_params->enable_frame_reordering;
	wrapper_open_param->nPicWidth      = open_params->frame_width;
	wrapper_open_param->nPicHeight     = open_params->frame_height;
}


static void dec_convert_from_wrapper_initial_info(VpuDecInitInfo *wrapper_info, ImxVpuDecInitialInfo *info)
{
	info->frame_width             = wrapper_info->nPicWidth;
	info->frame_height            = wrapper_info->nPicHeight;
	info->frame_rate_numerator    = wrapper_info->nFrameRateRes;
	info->frame_rate_denominator  = wrapper_info->nFrameRateDiv;

	info->min_num_required_framebuffers = wrapper_info->nMinFrameBufferCount + MIN_NUM_FREE_FB_REQUIRED;
	info->mjpeg_source_format           = convert_from_wrapper_mjpg_format(wrapper_info->nMjpgSourceFormat);

	info->interlacing = wrapper_info->nInterlace;

	info->width_height_ratio = wrapper_info->nQ16ShiftWidthDivHeightRatio;

	info->framebuffer_alignment = wrapper_info->nAddressAlignment;
}


static int dec_get_wrapper_framebuffer_index(ImxVpuDecoder *decoder, VpuFrameBuffer *wrapper_fb)
{
	unsigned int i;

	// TODO: do something faster, like a hash table
	for (i = 0; i < decoder->num_framebuffers; ++i)
	{
		if (wrapper_fb == decoder->wrapper_framebuffers[i])
			return (int)i;
	}
	return -1;
}


char const * imx_vpu_dec_error_string(ImxVpuDecReturnCodes code)
{
	switch (code)
	{
		case IMX_VPU_DEC_RETURN_CODE_OK:                        return "ok";
		case IMX_VPU_DEC_RETURN_CODE_ERROR:                     return "unspecified error";
		case IMX_VPU_DEC_RETURN_CODE_INVALID_PARAMS:            return "invalid params";
		case IMX_VPU_DEC_RETURN_CODE_INVALID_HANDLE:            return "invalid handle";
		case IMX_VPU_DEC_RETURN_CODE_INVALID_FRAMEBUFFER:       return "invalid framebuffer";
		case IMX_VPU_DEC_RETURN_CODE_INSUFFICIENT_FRAMEBUFFERS: return "insufficient_framebuffers";
		case IMX_VPU_DEC_RETURN_CODE_INVALID_STRIDE:            return "invalid stride";
		case IMX_VPU_DEC_RETURN_CODE_WRONG_CALL_SEQUENCE:       return "wrong call sequence";
		case IMX_VPU_DEC_RETURN_CODE_TIMEOUT:                   return "timeout";
		default: return "<unknown>";
	}
}


static unsigned long vpu_load_inst_counter = 0;


ImxVpuDecReturnCodes imx_vpu_dec_load(void)
{
	IMX_VPU_TRACE("VPU load instance counter: %lu", vpu_load_inst_counter);
	if (vpu_load_inst_counter != 0)
		return IMX_VPU_DEC_RETURN_CODE_OK;

	ImxVpuDecReturnCodes ret = dec_convert_retcode(VPU_DecLoad());
	if (ret != IMX_VPU_DEC_RETURN_CODE_OK)
		IMX_VPU_ERROR("loading decoder failed: %s", imx_vpu_dec_error_string(ret));
	else
	{
		IMX_VPU_TRACE("loaded decoder");
		++vpu_load_inst_counter;
	}

	return ret;
}


ImxVpuDecReturnCodes imx_vpu_dec_unload(void)
{
	IMX_VPU_TRACE("VPU load instance counter: %lu", vpu_load_inst_counter);
	if (vpu_load_inst_counter == 0)
		return IMX_VPU_DEC_RETURN_CODE_OK;

	ImxVpuDecReturnCodes ret = dec_convert_retcode(VPU_DecUnLoad());
	if (ret != IMX_VPU_DEC_RETURN_CODE_OK)
		IMX_VPU_ERROR("unloading decoder failed: %s", imx_vpu_dec_error_string(ret));
	else
	{
		IMX_VPU_TRACE("unloaded decoder");
		--vpu_load_inst_counter;
	}

	return ret;
}


ImxVpuDecReturnCodes imx_vpu_dec_allocate_memory(ImxVpuMemBlock *mem_block)
{
	VpuDecRetCode ret;
	VpuMemDesc mem_desc;

	if (mem_block->alignment == 0)
		mem_block->alignment = 1;

	mem_desc.nSize = mem_block->size + mem_block->alignment;

	if ((ret = VPU_DecGetMem(&mem_desc)) != VPU_DEC_RET_SUCCESS)
	{
		IMX_VPU_ERROR("allocating %d bytes of physical memory failed: %s", mem_block->size, imx_vpu_dec_error_string(dec_convert_retcode(ret)));
		return dec_convert_retcode(ret);
	}
	else
		IMX_VPU_TRACE("allocated %d bytes of physical memory", mem_block->size);

	convert_from_wrapper_mem_desc(&mem_desc, mem_block);

	mem_block->virtual_address = (void *)IMX_VPU_ALIGN_VAL_TO(mem_block->virtual_address_unaligned, mem_block->alignment);
	mem_block->physical_address = (imx_vpu_phys_addr_t)IMX_VPU_ALIGN_VAL_TO(mem_block->physical_address_unaligned, mem_block->alignment);

	return IMX_VPU_DEC_RETURN_CODE_OK;
}


ImxVpuDecReturnCodes imx_vpu_dec_deallocate_memory(ImxVpuMemBlock *mem_block)
{
	ImxVpuDecReturnCodes ret;
	VpuMemDesc mem_desc;

	convert_to_wrapper_mem_desc(mem_block, &mem_desc);

	ret = dec_convert_retcode(VPU_DecFreeMem(&mem_desc));
	if (ret != IMX_VPU_DEC_RETURN_CODE_OK)
		IMX_VPU_ERROR("deallocating %d bytes of physical memory failed: %s", mem_block->size, imx_vpu_dec_error_string(ret));
	else
		IMX_VPU_TRACE("deallocated %d bytes of physical memory", mem_block->size);

	return ret;
}


void imx_vpu_dec_get_bitstream_buffer_info(unsigned int *alignment, size_t *size)
{
	int i;
	VpuMemInfo mem_info;

	VPU_DecQueryMem(&mem_info);

	/* only two sub blocks are ever present - get the VPU_MEM_PHY one */

	for (i = 0; i < mem_info.nSubBlockNum; ++i)
	{
		if (mem_info.MemSubBlock[i].MemType == VPU_MEM_PHY)
		{
			*alignment = mem_info.MemSubBlock[i].nAlignment;
			*size = mem_info.MemSubBlock[i].nSize;
			IMX_VPU_TRACE("determined alignment %d and size %d for the physical memory for the bitstream buffer", *alignment, *size);
			break;
		}
	}

	/* virtual memory block is allocated internally inside imx_vpu_dec_open() */
}


ImxVpuDecReturnCodes imx_vpu_dec_open(ImxVpuDecoder **decoder, ImxVpuDecOpenParams *open_params, void *bitstream_buffer_virtual_address, imx_vpu_phys_addr_t bitstream_buffer_physical_addres)
{
	int config_param;
	VpuDecRetCode ret;
	VpuMemInfo mem_info;
	VpuDecOpenParam open_param;

	*decoder = IMX_VPU_ALLOC(sizeof(ImxVpuDecoder));
	if ((*decoder) == NULL)
	{
		IMX_VPU_ERROR("allocating memory for decoder object failed");
		return IMX_VPU_DEC_RETURN_CODE_ERROR;
	}

	memset(*decoder, 0, sizeof(ImxVpuDecoder));

	{
		int i;

		VPU_DecQueryMem(&mem_info);

		IMX_VPU_INFO("about to allocate %d memory sub blocks", mem_info.nSubBlockNum);
		for (i = 0; i < mem_info.nSubBlockNum; ++i)
		{
			char const *type_str = "<unknown>";
			VpuMemSubBlockInfo *sub_block = &(mem_info.MemSubBlock[i]);

			switch (sub_block->MemType)
			{
				case VPU_MEM_VIRT:
					type_str = "virtual";

					(*decoder)->virt_mem_sub_block_size = sub_block->nSize + sub_block->nAlignment;
					(*decoder)->virt_mem_sub_block = IMX_VPU_ALLOC((*decoder)->virt_mem_sub_block_size);
					if ((*decoder)->virt_mem_sub_block == NULL)
					{
						IMX_VPU_ERROR("allocating memory for sub block failed");
						return IMX_VPU_DEC_RETURN_CODE_ERROR;
					}

					sub_block->pVirtAddr = (unsigned char *)IMX_VPU_ALIGN_VAL_TO((*decoder)->virt_mem_sub_block, sub_block->nAlignment);
					sub_block->pPhyAddr = 0;
					break;

				case VPU_MEM_PHY:
					type_str = "physical";

					sub_block->pVirtAddr = (unsigned char *)(bitstream_buffer_virtual_address);
					sub_block->pPhyAddr = (unsigned char *)(bitstream_buffer_physical_addres);
					break;
				default:
					break;
			}

			IMX_VPU_INFO("allocated memory sub block #%d:  type: %s  size: %d  alignment: %d  virtual address: %p  physical address: %p", i, type_str, sub_block->nSize, sub_block->nAlignment, sub_block->pVirtAddr, sub_block->pPhyAddr);
		}
	}

	dec_convert_to_wrapper_open_param(open_params, &open_param);

	IMX_VPU_TRACE("opening decoder");

	switch (open_params->codec_format)
	{
		case IMX_VPU_CODEC_FORMAT_H264:
		case IMX_VPU_CODEC_FORMAT_H264_MVC:
		case IMX_VPU_CODEC_FORMAT_MPEG2:
		case IMX_VPU_CODEC_FORMAT_MPEG4:
			(*decoder)->consumption_info_available = TRUE;
			(*decoder)->flush_vpu_upon_reset = TRUE;
			break;
		case IMX_VPU_CODEC_FORMAT_H263:
		case IMX_VPU_CODEC_FORMAT_WMV3:
		case IMX_VPU_CODEC_FORMAT_WVC1:
			(*decoder)->consumption_info_available = FALSE;
			(*decoder)->flush_vpu_upon_reset = FALSE;
			break;
		case IMX_VPU_CODEC_FORMAT_MJPEG:
		case IMX_VPU_CODEC_FORMAT_VP8:
			(*decoder)->consumption_info_available = FALSE;
			(*decoder)->flush_vpu_upon_reset = TRUE;
			break;
		default:
			break;
	}

	ret = VPU_DecOpen(&((*decoder)->handle), &open_param, &mem_info);
	if (ret != VPU_DEC_RET_SUCCESS)
	{
		IMX_VPU_ERROR("opening decoder failed: %s", imx_vpu_dec_error_string(dec_convert_retcode(ret)));
		goto cleanup;
	}

	IMX_VPU_TRACE("setting configuration");

	config_param = VPU_DEC_SKIPNONE;
	ret = VPU_DecConfig((*decoder)->handle, VPU_DEC_CONF_SKIPMODE, &config_param);
	if (ret != VPU_DEC_RET_SUCCESS)
	{
		IMX_VPU_ERROR("setting skipmode to NONE failed: %s", imx_vpu_dec_error_string(dec_convert_retcode(ret)));
		goto cleanup;
	}

	config_param = 0;
	ret = VPU_DecConfig((*decoder)->handle, VPU_DEC_CONF_BUFDELAY, &config_param);
	if (ret != VPU_DEC_RET_SUCCESS)
	{
		IMX_VPU_ERROR("setting bufdelay to 0 failed: %s", imx_vpu_dec_error_string(dec_convert_retcode(ret)));
		goto cleanup;
	}

	config_param = VPU_DEC_IN_NORMAL;
	ret = VPU_DecConfig((*decoder)->handle, VPU_DEC_CONF_INPUTTYPE, &config_param);
	if (ret != VPU_DEC_RET_SUCCESS)
	{
		IMX_VPU_ERROR("setting input type to \"normal\" failed: %s", imx_vpu_dec_error_string(dec_convert_retcode(ret)));
		goto cleanup;
	}

	(*decoder)->codec_format = open_params->codec_format;

finish:
	if (ret == VPU_DEC_RET_SUCCESS)
		IMX_VPU_TRACE("successfully opened decoder");

	return dec_convert_retcode(ret);

cleanup:
	if ((*decoder)->virt_mem_sub_block != NULL)
		IMX_VPU_FREE((*decoder)->virt_mem_sub_block, (*decoder)->virt_mem_sub_block_size);
	IMX_VPU_FREE(*decoder, sizeof(ImxVpuDecoder));
	*decoder = NULL;

	goto finish;
}


ImxVpuDecReturnCodes imx_vpu_dec_close(ImxVpuDecoder *decoder)
{
	VpuDecRetCode ret;

	IMX_VPU_TRACE("closing decoder");

	ret = VPU_DecFlushAll(decoder->handle);
	if (ret == VPU_DEC_RET_FAILURE_TIMEOUT)
	{
		IMX_VPU_WARNING("resetting decoder after a timeout occurred");
		ret = VPU_DecReset(decoder->handle);
		if (ret != VPU_DEC_RET_SUCCESS)
			IMX_VPU_ERROR("resetting decoder failed: %s", imx_vpu_dec_error_string(dec_convert_retcode(ret)));
	}
	else if (ret != VPU_DEC_RET_SUCCESS)
		IMX_VPU_ERROR("flushing decoder failed: %s", imx_vpu_dec_error_string(dec_convert_retcode(ret)));

	ret = VPU_DecClose(decoder->handle);
	if (ret != VPU_DEC_RET_SUCCESS)
		IMX_VPU_ERROR("closing decoder failed: %s", imx_vpu_dec_error_string(dec_convert_retcode(ret)));

	if (decoder->user_data_for_frames != NULL)
		IMX_VPU_FREE(decoder->user_data_for_frames, sizeof(void*) * decoder->num_framebuffers);
	if (decoder->wrapper_framebuffers != NULL)
		IMX_VPU_FREE(decoder->wrapper_framebuffers, sizeof(VpuFrameBuffer*) * decoder->num_framebuffers);
	if (decoder->virt_mem_sub_block != NULL)
		IMX_VPU_FREE(decoder->virt_mem_sub_block, decoder->virt_mem_sub_block_size);
	IMX_VPU_FREE(decoder, sizeof(ImxVpuDecoder));

	IMX_VPU_TRACE("closed decoder");

	return dec_convert_retcode(ret);
}


ImxVpuDecReturnCodes imx_vpu_dec_set_drain_mode(ImxVpuDecoder *decoder, int enabled)
{
	int config_param;
	VpuDecRetCode ret;

	config_param = enabled ? VPU_DEC_IN_DRAIN : VPU_DEC_IN_NORMAL;
	ret = VPU_DecConfig(decoder->handle, VPU_DEC_CONF_INPUTTYPE, &config_param);

	if (ret != VPU_DEC_RET_SUCCESS)
		IMX_VPU_ERROR("setting decoder drain mode failed: %s", imx_vpu_dec_error_string(dec_convert_retcode(ret)));
	else
		IMX_VPU_INFO("set decoder drain mode to %d", enabled);

	return dec_convert_retcode(ret);
}


ImxVpuDecReturnCodes imx_vpu_dec_flush(ImxVpuDecoder *decoder)
{
	VpuDecRetCode ret = VPU_DEC_RET_SUCCESS;

	decoder->delay_pending_user_data = FALSE;

	if (decoder->flush_vpu_upon_reset)
	{
		ret = VPU_DecFlushAll(decoder->handle);
		if (ret == VPU_DEC_RET_FAILURE_TIMEOUT)
		{
			IMX_VPU_WARNING("resetting decoder after a timeout occurred");
			ret = VPU_DecReset(decoder->handle);
			if (ret != VPU_DEC_RET_SUCCESS)
				IMX_VPU_ERROR("resetting decoder failed: %s", imx_vpu_dec_error_string(dec_convert_retcode(ret)));
		}
		else if (ret != VPU_DEC_RET_SUCCESS)
			IMX_VPU_ERROR("flushing decoder failed: %s", imx_vpu_dec_error_string(dec_convert_retcode(ret)));
		else
			IMX_VPU_INFO("flushed decoder");

		decoder->recalculate_num_avail_framebuffers = TRUE;
	}
	else
		IMX_VPU_INFO("decoder not flushed, because it is unnecessary for this codec format");

	if (decoder->user_data_for_frames != NULL)
		memset(decoder->user_data_for_frames, 0, sizeof(void*) * decoder->num_framebuffers);
	decoder->num_user_data = 0;

	return dec_convert_retcode(ret);
}


ImxVpuDecReturnCodes imx_vpu_dec_register_framebuffers(ImxVpuDecoder *decoder, ImxVpuFramebuffer *framebuffers, unsigned int num_framebuffers)
{
	unsigned int i;
	VpuDecRetCode ret;
	VpuFrameBuffer *temp_fbs;

	IMX_VPU_TRACE("attempting to register %u framebuffers", num_framebuffers);

	decoder->wrapper_framebuffers = NULL;

	temp_fbs = IMX_VPU_ALLOC(sizeof(VpuFrameBuffer) * num_framebuffers);
	if (temp_fbs == NULL)
	{
		IMX_VPU_ERROR("allocating memory for framebuffers failed");
		return IMX_VPU_DEC_RETURN_CODE_ERROR;
	}

	for (i = 0; i < num_framebuffers; ++i)
		convert_to_wrapper_framebuffer(&framebuffers[i], &(temp_fbs[i]));

	ret = VPU_DecRegisterFrameBuffer(decoder->handle, temp_fbs, num_framebuffers);

	IMX_VPU_FREE(temp_fbs, sizeof(VpuFrameBuffer) * num_framebuffers);

	if (ret != VPU_DEC_RET_SUCCESS)
	{
		ImxVpuDecReturnCodes imxret = dec_convert_retcode(ret);
		IMX_VPU_ERROR("registering framebuffers failed: %s", imx_vpu_dec_error_string(imxret));
		return ret;
	}

	decoder->wrapper_framebuffers = IMX_VPU_ALLOC(sizeof(VpuFrameBuffer*) * num_framebuffers);
	{
		int out_num;
		VPU_DecAllRegFrameInfo(decoder->handle, decoder->wrapper_framebuffers, &out_num);
		IMX_VPU_LOG("out_num: %d  num_framebuffers: %u", out_num, num_framebuffers);
	}

	decoder->framebuffers = framebuffers;
	decoder->num_framebuffers = num_framebuffers;
	decoder->num_available_framebuffers = num_framebuffers;

	decoder->user_data_for_frames = IMX_VPU_ALLOC(sizeof(void*) * num_framebuffers);
	if (decoder->user_data_for_frames == NULL)
	{
		IMX_VPU_ERROR("allocating memory for user data failed");
		IMX_VPU_FREE(decoder->wrapper_framebuffers, sizeof(VpuFrameBuffer*) * num_framebuffers);
		decoder->wrapper_framebuffers = NULL;
		return IMX_VPU_DEC_RETURN_CODE_ERROR;
	}

	memset(decoder->user_data_for_frames, 0, sizeof(void*) * num_framebuffers);
	decoder->num_user_data = 0;

	return IMX_VPU_DEC_RETURN_CODE_OK;
}


void imx_vpu_dec_calc_framebuffer_sizes(ImxVpuDecInitialInfo *initial_info, unsigned int *frame_width, unsigned int *frame_height, unsigned int *y_stride, unsigned int *uv_stride, unsigned int *y_size, unsigned int *uv_size, unsigned int *mvcol_size, unsigned int *total_size)
{
	int alignment;

	*frame_width = IMX_VPU_ALIGN_VAL_TO(*frame_width, FRAME_ALIGN);
	if (initial_info->interlacing)
		*frame_height = IMX_VPU_ALIGN_VAL_TO(*frame_height, (2 * FRAME_ALIGN));
	else
		*frame_height = IMX_VPU_ALIGN_VAL_TO(*frame_height, FRAME_ALIGN);

	*y_stride = *frame_width;
	*y_size = (*y_stride) * (*frame_height);

	switch (initial_info->mjpeg_source_format)
	{
		case IMX_VPU_MJPEG_FORMAT_YUV420:
			*uv_stride = *y_stride / 2;
			*uv_size = *mvcol_size = *y_size / 4;
			break;
		case IMX_VPU_MJPEG_FORMAT_YUV422_HORIZONTAL:
			*uv_stride = *y_stride / 2;
			*uv_size = *mvcol_size = *y_size / 2;
			break;
		case IMX_VPU_MJPEG_FORMAT_YUV444:
			*uv_stride = *y_stride;
			*uv_size = *mvcol_size = *y_size;
			break;
		case IMX_VPU_MJPEG_FORMAT_YUV400:
			/* TODO: check if this is OK */
			*uv_stride = 0;
			*uv_size = *mvcol_size = 0;
			break;
		default:
			assert(FALSE);
	}

	alignment = initial_info->framebuffer_alignment;
	if (alignment > 1)
	{
		*y_size = IMX_VPU_ALIGN_VAL_TO(*y_size, alignment);
		*uv_size = IMX_VPU_ALIGN_VAL_TO(*uv_size, alignment);
		*mvcol_size = IMX_VPU_ALIGN_VAL_TO(*mvcol_size, alignment);
	}

	*total_size = *y_size + *uv_size + *uv_size + *mvcol_size + alignment;
}


ImxVpuDecReturnCodes imx_vpu_dec_get_initial_info(ImxVpuDecoder *decoder, ImxVpuDecInitialInfo *info)
{
	VpuDecRetCode ret;
	VpuDecInitInfo init_info;

	ret = VPU_DecGetInitialInfo(decoder->handle, &init_info);
	IMX_VPU_LOG("VPU_DecGetInitialInfo: min num framebuffers required: %d", init_info.nMinFrameBufferCount);
	dec_convert_from_wrapper_initial_info(&init_info, info);
	return dec_convert_retcode(ret);
}


ImxVpuDecReturnCodes imx_vpu_dec_decode_frame(ImxVpuDecoder *decoder, ImxVpuEncodedFrame *encoded_frame, unsigned int *output_code)
{
	VpuDecRetCode ret;
	VpuBufferNode node;
	int buf_ret_code;

	node.pVirAddr = encoded_frame->virtual_address;
	node.pPhyAddr = 0; /* encoded data is always read from a regular memory block, not a DMA buffer */
	node.nSize = encoded_frame->data_size;

	node.sCodecData.pData = encoded_frame->codec_data;
	node.sCodecData.nSize = encoded_frame->codec_data_size;

	decoder->pending_user_data = encoded_frame->user_data;

	ret = VPU_DecDecodeBuf(decoder->handle, &node, &buf_ret_code);
	IMX_VPU_LOG("VPU_DecDecodeBuf buf ret code: 0x%x", buf_ret_code);

	*output_code = dec_convert_outcode(buf_ret_code);

	if (ret != VPU_DEC_RET_SUCCESS)
	{
		IMX_VPU_ERROR("decoding frame failed: %s", imx_vpu_dec_error_string(dec_convert_retcode(ret)));
		return dec_convert_retcode(ret);
	}

	if (decoder->recalculate_num_avail_framebuffers)
	{
		decoder->num_available_framebuffers = decoder->num_framebuffers - decoder->num_framebuffers_in_use;
		IMX_VPU_LOG("recalculated number of available framebuffers to %d", decoder->num_available_framebuffers);
		decoder->recalculate_num_avail_framebuffers = FALSE;
	}

	if (buf_ret_code & VPU_DEC_INIT_OK)
	{
		decoder->delay_pending_user_data = TRUE;
		decoder->last_pending_user_data = decoder->pending_user_data;
	}

	if (buf_ret_code & VPU_DEC_FLUSH)
	{
		IMX_VPU_INFO("VPU requested a decoder flush");
		ret = VPU_DecFlushAll(decoder->handle);
		if (ret == VPU_DEC_RET_FAILURE_TIMEOUT)
		{
			IMX_VPU_WARNING("timeout detected, resetting decoder");

			ret = VPU_DecReset(decoder->handle);
			if (ret != VPU_DEC_RET_SUCCESS)
			{
				ImxVpuDecReturnCodes imxret = dec_convert_retcode(ret);
				IMX_VPU_ERROR("resetting decoder failed: %s", imx_vpu_dec_error_string(imxret));
				return imxret;
			}
			else
				*output_code |= IMX_VPU_DEC_OUTPUT_CODE_INTERNAL_RESET;
		}
		else if (ret != VPU_DEC_RET_SUCCESS)
		{
			ImxVpuDecReturnCodes imxret = dec_convert_retcode(ret);
			IMX_VPU_ERROR("flushing decoder failed: %s", imx_vpu_dec_error_string(imxret));
			return imxret;
		}
		else
			IMX_VPU_INFO("flushed decoder");
	}

	if (buf_ret_code & VPU_DEC_RESOLUTION_CHANGED)
	{
		IMX_VPU_INFO("resolution changed - resetting internal states");

		*output_code |= IMX_VPU_DEC_OUTPUT_CODE_INITIAL_INFO_AVAILABLE;

		decoder->delay_pending_user_data = TRUE;
		decoder->recalculate_num_avail_framebuffers = FALSE;

		decoder->num_user_data = 0;

		if (decoder->user_data_for_frames != NULL)
			IMX_VPU_FREE(decoder->user_data_for_frames, sizeof(void*) * decoder->num_framebuffers);
		if (decoder->wrapper_framebuffers != NULL)
			IMX_VPU_FREE(decoder->wrapper_framebuffers, sizeof(VpuFrameBuffer*) * decoder->num_framebuffers);

		decoder->user_data_for_frames = NULL;
		decoder->wrapper_framebuffers = NULL;
	}

	if (buf_ret_code & VPU_DEC_NO_ENOUGH_INBUF)
	{
		/* Not dropping frame here on purpose; the next input frame may
		 * complete the input */
	}

	{
		void *user_data = decoder->delay_pending_user_data ? decoder->last_pending_user_data : decoder->pending_user_data;

		/* The first time this location is reached, VPU_DEC_INIT_OK will be set in the output_code.
		 * This implies that the framebuffers have not been allocated and registered yet,
		 * so no user data can be stored yet.
		 * With codec formats that produce consumption info, this is not a problem, because
		 * VPU_DEC_ONE_FRM_CONSUMED will be returned only when framebuffers are present.
		 * But with other formats, an explicit decoder->framebuffers != NULL check is necessary
		 * (see below). The user_data pointer does not get lost; it is stored in last_pending_user_data. */
		if ((buf_ret_code & VPU_DEC_ONE_FRM_CONSUMED) && !(buf_ret_code & VPU_DEC_OUTPUT_DROPPED))
		{
			int fb_index;

			VpuDecFrameLengthInfo consumed_frame_info;
			ret = VPU_DecGetConsumedFrameInfo(decoder->handle, &consumed_frame_info);
			if (ret != VPU_DEC_RET_SUCCESS)
			{
				ImxVpuDecReturnCodes imxret = dec_convert_retcode(ret);
				IMX_VPU_ERROR("getting consumed frame info failed: %s", imx_vpu_dec_error_string(imxret));
				return imxret;
			}

			fb_index = dec_get_wrapper_framebuffer_index(decoder, consumed_frame_info.pFrame);

			if (consumed_frame_info.pFrame != NULL)
			{
				if ((fb_index >= 0) && (fb_index < (int)(decoder->num_framebuffers)))
				{
					IMX_VPU_LOG("framebuffer index %d for framebuffer %p user data %p", fb_index, (void *)(consumed_frame_info.pFrame), user_data);
					decoder->user_data_for_frames[fb_index] = user_data;
				}
				else
					IMX_VPU_ERROR("framebuffer index %d for framebuffer %p user data %p out of bounds", fb_index, (void *)(consumed_frame_info.pFrame), user_data);
			}
			else
				IMX_VPU_WARNING("consumed frame info contains a NULL frame");
		}
		else if (!(decoder->consumption_info_available) && (decoder->framebuffers != NULL))
		{
			if (decoder->num_user_data < (int)(decoder->num_framebuffers))
			{
				decoder->user_data_for_frames[decoder->num_user_data] = user_data;
				decoder->num_user_data++;

				IMX_VPU_LOG("user data %p stored as newest", user_data);

				IMX_VPU_TRACE("incremented number of userdata pointers to %d", decoder->num_user_data);
			}
			else
				IMX_VPU_WARNING("too many user data pointers in memory - cannot store current one");
		}

		decoder->last_pending_user_data = decoder->pending_user_data;
		decoder->pending_user_data = NULL;
	}

	if ((buf_ret_code & VPU_DEC_ONE_FRM_CONSUMED) && !(buf_ret_code & VPU_DEC_OUTPUT_DROPPED))
	{
		decoder->num_available_framebuffers--;
		decoder->num_times_counter_decremented++;
		IMX_VPU_LOG("decremented number of available framebuffers to %d (with consumed frame info); number of times decremented is now %d", decoder->num_available_framebuffers, decoder->num_times_counter_decremented);
	}

	if (buf_ret_code & VPU_DEC_OUTPUT_NODIS)
	{
		if ((encoded_frame->virtual_address != NULL) && (decoder->codec_format == IMX_VPU_CODEC_FORMAT_VP8))
			*output_code |= IMX_VPU_DEC_OUTPUT_CODE_DECODE_ONLY;
	}

	/* VPU_DEC_NO_ENOUGH_BUF handled by caller - should be treated as an error */

	if ((buf_ret_code & VPU_DEC_OUTPUT_DIS) && !(decoder->consumption_info_available))
	{
		decoder->num_available_framebuffers--;
		decoder->num_times_counter_decremented++;
		IMX_VPU_LOG("decremented number of available framebuffers to %d (no consumed frame info); number of times decremented is now %d", decoder->num_available_framebuffers, decoder->num_times_counter_decremented);
	}
	else if (buf_ret_code & VPU_DEC_OUTPUT_MOSAIC_DIS)
	{
		IMX_VPU_TRACE("dropping mosaic frame");

		/* mosaic frames do not seem to be useful for anything, so they are just dropped here */

		ImxVpuDecReturnCodes imxret;
		ImxVpuDecodedFrame decoded_frame;

		if ((imxret = imx_vpu_dec_get_decoded_frame(decoder, &decoded_frame)) != IMX_VPU_DEC_RETURN_CODE_OK)
		{
			IMX_VPU_ERROR("error getting output mosaic frame: %s", imx_vpu_dec_error_string(imxret));
			return imxret;
		}

		if ((imxret = imx_vpu_dec_mark_framebuffer_as_displayed(decoder, decoded_frame.framebuffer)) != IMX_VPU_DEC_RETURN_CODE_OK)
		{
			IMX_VPU_ERROR("error marking mosaic frame as displayed: %s", imx_vpu_dec_error_string(imxret));
			return imxret;
		}

		decoder->dropped_frame_user_data = decoded_frame.user_data;

		*output_code |= IMX_VPU_DEC_OUTPUT_CODE_DROPPED;
	}
	else if (buf_ret_code & VPU_DEC_OUTPUT_DROPPED)
	{
		// TODO make this work for formats with consumption info
		if (decoder->num_user_data > 0)
		{
			decoder->dropped_frame_user_data = decoder->user_data_for_frames[0];
			decoder->user_data_for_frames[0] = NULL;
			memmove(decoder->user_data_for_frames, decoder->user_data_for_frames + 1, sizeof(void*) * (decoder->num_user_data - 1));
			decoder->num_user_data--;
		}
		else
			decoder->dropped_frame_user_data = NULL;
	}

	/* In case the VPU didn't use the input and no consumed frame info is available,
	 * drop the input frame to make sure timestamps are okay
	 * (If consumed frame info is present it is still possible it might be used for input-output frame
	 * associations; unlikely to occur thought) */
	if ((encoded_frame->virtual_address != NULL) && !(buf_ret_code & (VPU_DEC_ONE_FRM_CONSUMED | VPU_DEC_INPUT_USED | VPU_DEC_RESOLUTION_CHANGED)))
	{
		decoder->dropped_frame_user_data = encoded_frame->user_data;
		*output_code |= IMX_VPU_DEC_OUTPUT_CODE_DROPPED;
	}

	return IMX_VPU_DEC_RETURN_CODE_OK;
}


ImxVpuDecReturnCodes imx_vpu_dec_get_decoded_frame(ImxVpuDecoder *decoder, ImxVpuDecodedFrame *decoded_frame)
{
	VpuDecRetCode ret;
	VpuDecOutFrameInfo out_frame_info;
	int fb_index;
	void *user_data;

	ret = VPU_DecGetOutputFrame(decoder->handle, &out_frame_info);
	if (ret != VPU_DEC_RET_SUCCESS)
	{
		ImxVpuDecReturnCodes imxret = dec_convert_retcode(ret);
		IMX_VPU_ERROR("error getting decoded output frame: %s", imx_vpu_dec_error_string(imxret));
		return imxret;
	}

	fb_index = dec_get_wrapper_framebuffer_index(decoder, out_frame_info.pDisplayFrameBuf);

	user_data = NULL;
	if (decoder->consumption_info_available)
	{
		if ((fb_index >= 0) && (fb_index < (int)(decoder->num_framebuffers)))
		{
			user_data = decoder->user_data_for_frames[fb_index];
			IMX_VPU_LOG("framebuffer index %d for framebuffer %p and user data %p", fb_index, (void *)(out_frame_info.pDisplayFrameBuf), user_data);
			decoder->user_data_for_frames[fb_index] = NULL;
		}
		else
			IMX_VPU_ERROR("framebuffer index %d for framebuffer %p and user data %p out of bounds", fb_index, (void *)(out_frame_info.pDisplayFrameBuf), user_data);
	}
	else
	{
		if (decoder->num_user_data > 0)
		{
			user_data = decoder->user_data_for_frames[0];
			decoder->user_data_for_frames[0] = NULL;
			IMX_VPU_LOG("framebuffer index %d user data %p retrieved as oldest", fb_index, user_data);
			memmove(decoder->user_data_for_frames, decoder->user_data_for_frames + 1, sizeof(void*) * (decoder->num_user_data - 1));
			decoder->num_user_data--;
		}
	}

	decoded_frame->pic_type = convert_from_wrapper_pic_type(out_frame_info.ePicType);
	decoded_frame->user_data = user_data;

	/* XXX
	 * This association assumes that the order of internal framebuffer entries
	 * inside the VPU wrapper is the same as the order of the framebuffers here.
	 * So, decoder->framebuffers[1] equals internal framebuffer entry with index 1 etc.
	 */
	decoded_frame->framebuffer = &(decoder->framebuffers[fb_index]);
	/* This is used in imx_vpu_dec_mark_framebuffer_as_displayed() to be able
	 * to mark the vpuwrapper framebuffer as displayed */
	decoded_frame->framebuffer->internal = out_frame_info.pDisplayFrameBuf;

	decoder->num_framebuffers_in_use++;

	return IMX_VPU_DEC_RETURN_CODE_OK;
}


void* imx_vpu_dec_get_dropped_frame_user_data(ImxVpuDecoder *decoder)
{
	return decoder->dropped_frame_user_data;
}


int imx_vpu_dec_get_num_free_framebuffers(ImxVpuDecoder *decoder)
{
	return decoder->num_available_framebuffers;
}


int imx_vpu_dec_get_min_num_free_required(ImxVpuDecoder *decoder)
{
	IMXVPUCODEC_UNUSED_PARAM(decoder);
	return MIN_NUM_FREE_FB_REQUIRED;
}


ImxVpuDecReturnCodes imx_vpu_dec_mark_framebuffer_as_displayed(ImxVpuDecoder *decoder, ImxVpuFramebuffer *framebuffer)
{
	VpuDecRetCode ret;
	VpuFrameBuffer *wrapper_fb = (VpuFrameBuffer *)(framebuffer->internal);

	ret = VPU_DecOutFrameDisplayed(decoder->handle, wrapper_fb);
	if (ret != VPU_DEC_RET_SUCCESS)
	{
		ImxVpuDecReturnCodes imxret = dec_convert_retcode(ret);
		IMX_VPU_ERROR("error marking output frame as displayed: %s", imx_vpu_dec_error_string(imxret));
		return imxret;
	}

	IMX_VPU_LOG("marked framebuffer %p with physical address 0x%x as displayed", (void *)framebuffer, framebuffer->physical_address);

	if (decoder->num_times_counter_decremented > 0)
	{
		decoder->num_available_framebuffers++;
		decoder->num_times_counter_decremented--;
		decoder->num_framebuffers_in_use--;

		IMX_VPU_LOG("num_available_framebuffers %d  num_times_counter_decremented %d  num_framebuffers_in_use %d", decoder->num_available_framebuffers, decoder->num_times_counter_decremented, decoder->num_framebuffers_in_use);
	}

	return IMX_VPU_DEC_RETURN_CODE_OK;
}

