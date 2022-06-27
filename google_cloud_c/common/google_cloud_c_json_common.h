#ifndef GOOGLE_CLOUD_C_JSON_COMMON_H
#define GOOGLE_CLOUD_C_JSON_COMMON_H

#ifdef __cplusplus
#include <cstdlib>
extern "C" {
#else
#include <stdlib.h>
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __STDC_VERSION__ >= 199901L */
#endif /* __cplusplus */

#ifdef _MSC_VER
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
#include <sys/types.h>
#endif

#include <google_cloud_c/common/google_cloud_c_common_export.h>

#include <parson.h>

extern GOOGLE_CLOUD_C_COMMON_EXPORT const char **
json_array_to_cstr_array(const JSON_Array *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_JSON_COMMON_H */
