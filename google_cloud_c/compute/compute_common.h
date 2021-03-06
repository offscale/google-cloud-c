#ifndef GOOGLE_CLOUD_C_COMPUTE_COMMON_H
#define GOOGLE_CLOUD_C_COMPUTE_COMMON_H

#ifdef __cplusplus
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __cplusplus */

#include <parson.h>

#include <google_cloud_c/compute/google_cloud_c_compute_export.h>

struct GoogleCloudOperation {
  const char *id, *name, *zone, *operationType, *targetLink, *targetId, *status,
      *user, *progress, *insertTime, *startTime, *selfLink, *kind;
};

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct GoogleCloudOperation *
google_cloud_operation_from_json(const JSON_Object *);

/*
#include <string>

namespace Compute {
    struct Deprecated {
        //enum
        std::string state;

        std::string replacement;
        std::string deprecated;
        std::string obsolete;
        std::string deleted;
    };
};
*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_COMPUTE_COMMON_H */
