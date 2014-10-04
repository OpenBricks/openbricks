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


#ifndef IMXVPUCODEC_PLATFORM_CHROMIUM_H
#define IMXVPUCODEC_PLATFORM_CHROMIUM_H


#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif


#define IMX_VPU_ALLOC(SIZE) malloc(SIZE)
#define IMX_VPU_FREE(PTR, SIZE) free(PTR)


#define IMX_VPU_ERROR(...)   imx_vpu_log(IMX_VPU_LOG_LEVEL_ERROR,   __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define IMX_VPU_WARNING(...) imx_vpu_log(IMX_VPU_LOG_LEVEL_WARNING, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define IMX_VPU_INFO(...)    imx_vpu_log(IMX_VPU_LOG_LEVEL_INFO,    __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define IMX_VPU_DEBUG(...)   imx_vpu_log(IMX_VPU_LOG_LEVEL_DEBUG,   __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define IMX_VPU_LOG(...)     imx_vpu_log(IMX_VPU_LOG_LEVEL_LOG,     __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define IMX_VPU_TRACE(...)   imx_vpu_log(IMX_VPU_LOG_LEVEL_TRACE,   __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)


typedef enum
{
	IMX_VPU_LOG_LEVEL_ERROR = 0,
	IMX_VPU_LOG_LEVEL_WARNING,
	IMX_VPU_LOG_LEVEL_INFO,
	IMX_VPU_LOG_LEVEL_DEBUG,
	IMX_VPU_LOG_LEVEL_LOG,
	IMX_VPU_LOG_LEVEL_TRACE
}
ImxVpuLogLevel;


void imx_vpu_log(ImxVpuLogLevel level, char const *file, int const line, char const *fn, const char * format, ...);


#ifdef __cplusplus
}
#endif


#endif


