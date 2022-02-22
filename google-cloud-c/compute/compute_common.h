#ifndef PP_COMPUTE_COMMON_H
#define PP_COMPUTE_COMMON_H

#include <stdbool.h>
#include "../gcloud_export.h"

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

extern GCLOUD_EXPORT const struct GoogleCloudOperation EMPTY_GOOGLE_CLOUD_OPERATION;

extern GCLOUD_EXPORT struct OptionalGoogleCloudOperation google_cloud_operation_from_json(const JSON_Object *);

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

#endif /* PP_COMPUTE_COMMON_H */
