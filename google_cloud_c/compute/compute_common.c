#include <parson.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#define C89STRINGUTILS_IMPLEMENTATION
#endif /* defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) */

#include <google_cloud_c/compute/compute_common.h>

const struct GoogleCloudOperation googleCloudOperationNull = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL};

struct OptionalGoogleCloudOperation
google_cloud_operation_from_json(const JSON_Object *const jsonObject) {
  struct OptionalGoogleCloudOperation optionalGoogleCloudOperation;
  if (!json_object_has_value(jsonObject, "kind") ||
      !json_object_has_value(jsonObject, "targetId") ||
      !json_object_has_value(jsonObject, "status") ||
      !json_object_has_value(jsonObject, "id")) {
    optionalGoogleCloudOperation.set = false,
    optionalGoogleCloudOperation.googleCloudOperation =
        googleCloudOperationNull;
    return optionalGoogleCloudOperation;
  } else {
    struct GoogleCloudOperation googleCloudOperation;
    googleCloudOperation.id = json_object_get_string(jsonObject, "id");
    googleCloudOperation.name = json_object_get_string(jsonObject, "name");
    googleCloudOperation.zone = json_object_get_string(jsonObject, "zone");
    googleCloudOperation.operationType =
        json_object_get_string(jsonObject, "operationType");
    googleCloudOperation.targetLink =
        json_object_get_string(jsonObject, "targetLink");
    googleCloudOperation.targetId =
        json_object_get_string(jsonObject, "targetId");
    googleCloudOperation.status = json_object_get_string(jsonObject, "status");
    googleCloudOperation.user = json_object_get_string(jsonObject, "user");
    googleCloudOperation.progress =
        json_object_get_string(jsonObject, "progress");
    googleCloudOperation.insertTime =
        json_object_get_string(jsonObject, "insertTime");
    googleCloudOperation.startTime =
        json_object_get_string(jsonObject, "startTime");
    googleCloudOperation.selfLink =
        json_object_get_string(jsonObject, "selfLink");
    googleCloudOperation.kind = json_object_get_string(jsonObject, "kind");

    optionalGoogleCloudOperation.set = true,
    optionalGoogleCloudOperation.googleCloudOperation = googleCloudOperation;
    return optionalGoogleCloudOperation;
  }
}
