#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#define C89STRINGUTILS_IMPLEMENTATION
#endif /* defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) */
#include <c89stringutils_string_extras.h>

#include <google_cloud_c/cloud_common/cloud_common.h>

struct Operation *operation_from_json(const JSON_Object *const json_object) {
  struct Operation *operation = malloc(sizeof *operation);

  operation->kind = json_object_get_string(json_object, "kind");
  operation->id = json_object_get_string(json_object, "id");
  operation->creationTimestamp =
      json_object_get_string(json_object, "creationTimestamp");
  operation->name = json_object_get_string(json_object, "name");
  operation->zone = json_object_get_string(json_object, "zone");
  operation->clientOperationId =
      json_object_get_string(json_object, "clientOperationId");
  operation->operationType =
      json_object_get_string(json_object, "operationType");
  operation->targetLink = json_object_get_string(json_object, "targetLink");
  operation->targetId = json_object_get_string(json_object, "targetId");
  operation->status = json_object_get_string(json_object, "status");
  operation->statusMessage =
      json_object_get_string(json_object, "statusMessage");
  operation->user = json_object_get_string(json_object, "user");
  if (json_object_has_value_of_type(json_object, "progress", JSONNumber))
    operation->progress = (int)json_object_get_number(json_object, "progress");
  operation->insertTime = json_object_get_string(json_object, "insertTime");
  operation->startTime = json_object_get_string(json_object, "startTime");
  operation->endTime = json_object_get_string(json_object, "endTime");
  /*operation->error = json_object_get_string(json_object, "error");
  operation->warnings = json_object_get_string(json_object, "warnings");*/
  if (json_object_has_value_of_type(json_object, "httpErrorStatusCode",
                                    JSONNumber))
    operation->httpErrorStatusCode =
        (int)json_object_get_number(json_object, "httpErrorStatusCode");
  operation->httpErrorMessage =
      json_object_get_string(json_object, "httpErrorMessage");
  operation->selfLink = json_object_get_string(json_object, "selfLink");
  operation->region = json_object_get_string(json_object, "region");
  operation->description = json_object_get_string(json_object, "description");
  operation->operationGroupId =
      json_object_get_string(json_object, "operationGroupId");

  return operation;
}

extern GOOGLE_CLOUD_C_CLOUD_COMMON_EXPORT const char *
operation_to_json(const struct Operation *operation) {
  char *s = NULL;
  jasprintf(&s, "{");

  if (operation->kind != NULL && operation->kind[0] != '\0')
    jasprintf(&s, "  \"kind\": \"%s\",", operation->kind);

  if (operation->id != NULL && operation->id[0] != '\0')
    jasprintf(&s, "  \"id\": \"%s\",", operation->id);

  if (operation->creationTimestamp != NULL &&
      operation->creationTimestamp[0] != '\0')
    jasprintf(&s, "  \"creationTimestamp\": \"%s\",",
              operation->creationTimestamp);

  if (operation->name != NULL && operation->name[0] != '\0')
    jasprintf(&s, "  \"name\": \"%s\",", operation->name);

  if (operation->zone != NULL && operation->zone[0] != '\0')
    jasprintf(&s, "  \"zone\": \"%s\",", operation->zone);

  if (operation->clientOperationId != NULL &&
      operation->clientOperationId[0] != '\0')
    jasprintf(&s, "  \"clientOperationId\": \"%s\",",
              operation->clientOperationId);

  if (operation->operationType != NULL && operation->operationType[0] != '\0')
    jasprintf(&s, "  \"operationType\": \"%s\",", operation->operationType);

  if (operation->targetLink != NULL && operation->targetLink[0] != '\0')
    jasprintf(&s, "  \"targetLink\": \"%s\",", operation->targetLink);

  if (operation->targetId != NULL && operation->targetId[0] != '\0')
    jasprintf(&s, "  \"targetId\": \"%s\",", operation->targetId);

  if (operation->status != NULL && operation->status[0] != '\0')
    jasprintf(&s, "  \"status\": \"%s\",", operation->status);

  if (operation->statusMessage != NULL && operation->statusMessage[0] != '\0')
    jasprintf(&s, "  \"statusMessage\": \"%s\",", operation->statusMessage);

  if (operation->user != NULL && operation->user[0] != '\0')
    jasprintf(&s, "  \"user\": \"%s\",", operation->user);

  if (operation->progress)
    jasprintf(&s, "  \"progress\": %d,", operation->progress);

  if (operation->insertTime != NULL && operation->insertTime[0] != '\0')
    jasprintf(&s, "  \"insertTime\": \"%s\",", operation->insertTime);

  if (operation->startTime != NULL && operation->startTime[0] != '\0')
    jasprintf(&s, "  \"startTime\": \"%s\",", operation->startTime);

  if (operation->endTime != NULL && operation->endTime[0] != '\0')
    jasprintf(&s, "  \"endTime\": \"%s\",", operation->endTime);
  /* operation->errors ; operation->warnings */

  if (operation->httpErrorStatusCode)
    jasprintf(&s, "  \"httpErrorStatusCode\": %d,",
              operation->httpErrorStatusCode);

  if (operation->httpErrorMessage != NULL &&
      operation->httpErrorMessage[0] != '\0')
    jasprintf(&s, "  \"httpErrorMessage\": \"%s\",",
              operation->httpErrorMessage);

  if (operation->selfLink != NULL && operation->selfLink[0] != '\0')
    jasprintf(&s, "  \"selfLink\": \"%s\",", operation->selfLink);

  if (operation->region != NULL && operation->region[0] != '\0')
    jasprintf(&s, "  \"region\": \"%s\",", operation->region);

  if (operation->description != NULL && operation->description[0] != '\0')
    jasprintf(&s, "  \"description\": \"%s\",", operation->description);

  if (operation->operationGroupId != NULL &&
      operation->operationGroupId[0] != '\0')
    jasprintf(&s, "  \"operationGroupId\": \"%s\",",
              operation->operationGroupId);

  jasprintf(&s, "\0");
  s[strlen(s) - 1] = '}';
  return s;
}
