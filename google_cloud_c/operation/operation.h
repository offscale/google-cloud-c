#ifndef GOOGLE_CLOUD_C_CLIENT_OPERATION_H
#define GOOGLE_CLOUD_C_CLIENT_OPERATION_H

#ifdef __cplusplus
#include <cassert>
#include <cstdlib>
#include <cstring>
extern "C" {
#else
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#endif /* __cplusplus */

#include <parson.h>

#include <google_cloud_c/operation/google_cloud_c_operation_export.h>

/* =================
 * |   Operation   |
 * ================ */

enum OperationStatus { PENDING, RUNNING, DONE };

struct Operation {
  const char *kind;
  const char *id;
  const char *creationTimestamp;
  const char *name;
  const char *zone;
  const char *clientOperationId;
  const char *operationType;
  const char *targetLink;
  const char *targetId;
  enum OperationStatus status;
  const char *statusMessage;
  const char *user;
  int progress;
  const char *insertTime;
  const char *startTime;
  const char *endTime;
  struct OperationErrors *errors;
  struct OperationWarning **warnings;
  int httpErrorStatusCode;
  const char *httpErrorMessage;
  const char *selfLink;
  const char *region;
  const char *description;
  const char *operationGroupId;
};

struct OperationError {
  const char *code;
  const char *location;
  const char *message;
};

struct OperationErrors {
  struct OperationError **errors;
};

struct OperationWarning {
  const char *code;
  const char *message;
  const char *data;
  /* "data": [
        {
          "key": string,
          "value": string
        }
      ] */
};

/* Retrieves the specified Operations resource.
 * https://cloud.google.com/compute/docs/reference/rest/v1/globalOperations/get
 */
extern GOOGLE_CLOUD_C_OPERATION_EXPORT struct Operation *
global_operation_get(const char *project_id, const char *resourceId);

/* Retrieves the specified zone-specific Operations resource.
 * https://cloud.google.com/compute/docs/reference/rest/v1/zoneOperations/get */
struct Operation *zonal_operation_get(const char *project_id, const char *zone,
                                      const char *resourceId);

/* Retrieves the specified region-specific Operations resource.
 * https://cloud.google.com/compute/docs/reference/rest/v1/regionOperations/get
 */
struct Operation *region_operation_get(const char *project_id, const char *zone,
                                       const char *resourceId);

/* utility functions */

extern GOOGLE_CLOUD_C_OPERATION_EXPORT enum OperationStatus
str_to_OperationStatus(const char *);

extern GOOGLE_CLOUD_C_OPERATION_EXPORT const char *
    OperationStatus_to_str(enum OperationStatus);

extern GOOGLE_CLOUD_C_OPERATION_EXPORT struct Operation *
operation_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_OPERATION_EXPORT const char *
operation_to_json(const struct Operation *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_CLIENT_OPERATION_H */
