#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "FreeRTOS.h"

#ifdef NDEBUG
#define debug(M, ...)
#else
//#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n\r", __FILE__, __LINE__, ##__VA_ARGS__)
#define debug(M, ...) \
{\
		portDISABLE_INTERRUPTS();\
		fprintf(stderr, "DEBUG %s:%d: " M "\n\r", __FILE__, __LINE__, ##__VA_ARGS__);\
		portENABLE_INTERRUPTS();\
}
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

//#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n\r", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
//
//#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n\r", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
//
//#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n\r", __FILE__, __LINE__, ##__VA_ARGS__)

#define log_err(M, ...)\
{\
		portDISABLE_INTERRUPTS();\
		fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n\r", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__);\
		portENABLE_INTERRUPTS();\
}

#define log_warn(M, ...) \
{\
		portDISABLE_INTERRUPTS();\
		fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n\r", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__);\
		portENABLE_INTERRUPTS();\
}

#define log_info(M, ...) \
{\
		portDISABLE_INTERRUPTS();\
		fprintf(stderr, "[INFO] (%s:%d) " M "\n\r", __FILE__, __LINE__, ##__VA_ARGS__);\
		portENABLE_INTERRUPTS();\
}

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.\n\r")

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif
