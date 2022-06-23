#include <parson.h>

#include <google_cloud_c/compute/compute_common.h>

struct GoogleCloudOperation *
google_cloud_operation_from_json(const JSON_Object *const jsonObject) {
  if (!json_object_has_value(jsonObject, "kind") ||
      !json_object_has_value(jsonObject, "targetId") ||
      !json_object_has_value(jsonObject, "status") ||
      !json_object_has_value(jsonObject, "id")) {
    return NULL;
  } else {
    struct GoogleCloudOperation *googleCloudOperation =
        malloc(sizeof *googleCloudOperation);
    googleCloudOperation->id = json_object_get_string(jsonObject, "id");
    googleCloudOperation->name = json_object_get_string(jsonObject, "name");
    googleCloudOperation->zone = json_object_get_string(jsonObject, "zone");
    googleCloudOperation->operationType =
        json_object_get_string(jsonObject, "operationType");
    googleCloudOperation->targetLink =
        json_object_get_string(jsonObject, "targetLink");
    googleCloudOperation->targetId =
        json_object_get_string(jsonObject, "targetId");
    googleCloudOperation->status = json_object_get_string(jsonObject, "status");
    googleCloudOperation->user = json_object_get_string(jsonObject, "user");
    googleCloudOperation->progress =
        json_object_get_string(jsonObject, "progress");
    googleCloudOperation->insertTime =
        json_object_get_string(jsonObject, "insertTime");
    googleCloudOperation->startTime =
        json_object_get_string(jsonObject, "startTime");
    googleCloudOperation->selfLink =
        json_object_get_string(jsonObject, "selfLink");
    googleCloudOperation->kind = json_object_get_string(jsonObject, "kind");

    return googleCloudOperation;
  }
}
