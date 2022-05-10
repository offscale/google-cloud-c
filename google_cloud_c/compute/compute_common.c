#include <parson.h>

#include <google_cloud_c/compute/compute_common.h>

const struct GoogleCloudOperation EMPTY_GOOGLE_CLOUD_OPERATION = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL};

struct OptionalGoogleCloudOperation
google_cloud_operation_from_json(const JSON_Object *json_object) {
  struct OptionalGoogleCloudOperation optionalGoogleCloudOperation;
  if (!json_object_has_value(json_object, "kind") ||
      !json_object_has_value(json_object, "targetId") ||
      !json_object_has_value(json_object, "status") ||
      !json_object_has_value(json_object, "id")) {
    optionalGoogleCloudOperation.set = false,
    optionalGoogleCloudOperation.googleCloudOperation =
        EMPTY_GOOGLE_CLOUD_OPERATION;
    return optionalGoogleCloudOperation;
  } else {
    struct GoogleCloudOperation googleCloudOperation;
    googleCloudOperation.id = json_object_get_string(json_object, "id");
    googleCloudOperation.name = json_object_get_string(json_object, "name");
    googleCloudOperation.zone = json_object_get_string(json_object, "zone");
    googleCloudOperation.operationType =
        json_object_get_string(json_object, "operationType");
    googleCloudOperation.targetLink =
        json_object_get_string(json_object, "targetLink");
    googleCloudOperation.targetId =
        json_object_get_string(json_object, "targetId");
    googleCloudOperation.status = json_object_get_string(json_object, "status");
    googleCloudOperation.user = json_object_get_string(json_object, "user");
    googleCloudOperation.progress =
        json_object_get_string(json_object, "progress");
    googleCloudOperation.insertTime =
        json_object_get_string(json_object, "insertTime");
    googleCloudOperation.startTime =
        json_object_get_string(json_object, "startTime");
    googleCloudOperation.selfLink =
        json_object_get_string(json_object, "selfLink");
    googleCloudOperation.kind = json_object_get_string(json_object, "kind");

    optionalGoogleCloudOperation.set = true;
    optionalGoogleCloudOperation.googleCloudOperation = googleCloudOperation;
    return optionalGoogleCloudOperation;
  }
}
