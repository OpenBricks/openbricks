#include "imxvpucodec_platform_chromium.h"
#include "base/logging.h"

#include <stdio.h>
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif


void imx_vpu_log(ImxVpuLogLevel level, char const *file, int const line, char const *fn, const char * format, ...)
{
	va_list args;
	char buf[500];

	va_start(args, format);
	vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);

	#define DO_LOG(severity) do { LOG(severity) << file << ":" << line << " (" << fn << ")  " << buf; } while(0)
	#define DO_VLOG(severity) do { VLOG(severity) << file << ":" << line << " (" << fn << ")  " << buf; } while(0)

	switch (level)
	{
		case IMX_VPU_LOG_LEVEL_ERROR: DO_LOG(ERROR); break;
		case IMX_VPU_LOG_LEVEL_WARNING: DO_LOG(WARNING); break;
		case IMX_VPU_LOG_LEVEL_INFO: DO_LOG(INFO); break;
		case IMX_VPU_LOG_LEVEL_DEBUG: DO_VLOG(0); break;
		case IMX_VPU_LOG_LEVEL_LOG: DO_VLOG(1); break;
		case IMX_VPU_LOG_LEVEL_TRACE: DO_VLOG(2); break;
		default: break;
	}
}


#ifdef __cplusplus
}
#endif
