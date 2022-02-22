#include <parson.h>
#include "compute_common.h"

const struct GoogleCloudOperation EMPTY_GOOGLE_CLOUD_OPERATION = {
        NULL,NULL,NULL,NULL,
        NULL,NULL,NULL,NULL,
        NULL, NULL,NULL, NULL,
        NULL
};

struct OptionalGoogleCloudOperation google_cloud_operation_from_json(const JSON_Object *json_object) {
    if (!json_object_has_value(json_object, "kind")
    || !json_object_has_value(json_object, "targetId")
    || !json_object_has_value(json_object, "status")
    || !json_object_has_value(json_object, "id")) {
        const struct OptionalGoogleCloudOperation optionalGoogleCloudOperation = {
                false,
                EMPTY_GOOGLE_CLOUD_OPERATION
        };
        return optionalGoogleCloudOperation;
    }
    const struct GoogleCloudOperation googleCloudOperation = {
            json_object_get_string(json_object, "id"),
            json_object_get_string(json_object, "name"),
            json_object_get_string(json_object, "zone"),
            json_object_get_string(json_object, "operationType"),
            json_object_get_string(json_object, "targetLink"),
            json_object_get_string(json_object, "targetId"),
            json_object_get_string(json_object, "status"),
            json_object_get_string(json_object, "user"),
            json_object_get_string(json_object, "progress"),
            json_object_get_string(json_object, "insertTime"),
            json_object_get_string(json_object, "startTime"),
            json_object_get_string(json_object, "selfLink"),
            json_object_get_string(json_object, "kind"),
    };
    const struct OptionalGoogleCloudOperation optionalGoogleCloudOperation = {
            true,
            googleCloudOperation
    };
    return optionalGoogleCloudOperation;
}
