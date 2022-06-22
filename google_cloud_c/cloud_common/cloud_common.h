#ifndef GOOGLE_CLOUD_C_CLIENT_CLOUD_COMMON_H
#define GOOGLE_CLOUD_C_CLIENT_CLOUD_COMMON_H

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

#include <google_cloud_c/cloud_common/google_cloud_c_cloud_common_export.h>

/* =================
 * |   Operation   |
 * ================ */

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
  const char *status;
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
}

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

/* utility functions */

extern GOOGLE_CLOUD_C_CLOUD_COMMON_EXPORT struct Operation *
operation_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_CLOUD_COMMON_EXPORT const char *
operation_to_json(const struct Operation *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_CLIENT_CLOUD_COMMON_H */
