#ifndef GOOGLE_CLOUD_C_COMPUTE_COMMON_H
#define GOOGLE_CLOUD_C_COMPUTE_COMMON_H

#include <acquire_stdbool.h>
#include <google_cloud_c_export.h>

struct GoogleCloudOperation {
    const char *id,
               *name,
               *zone,
               *operationType,
               *targetLink,
               *targetId,
               *status,
               *user,
               *progress,
               *insertTime,
               *startTime,
               *selfLink,
               *kind;
};

struct OptionalGoogleCloudOperation {
    bool set;
    struct GoogleCloudOperation googleCloudOperation;
};

extern GOOGLE_CLOUD_C_EXPORT const struct GoogleCloudOperation EMPTY_GOOGLE_CLOUD_OPERATION;

extern GOOGLE_CLOUD_C_EXPORT struct OptionalGoogleCloudOperation google_cloud_operation_from_json(const JSON_Object *);

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

#endif /* !GOOGLE_CLOUD_C_COMPUTE_COMMON_H */
