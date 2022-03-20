#ifndef GOOGLE_CLOUD_C_GOOGLE_CLOUD_C_PROJECT_PROJECT_H
#define GOOGLE_CLOUD_C_GOOGLE_CLOUD_C_PROJECT_PROJECT_H

#include "cloud_auth.h"
#include <c89stringutils_string_extras.h>

#ifdef __cplusplus
#include <cstdbool>
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c_stdbool.h>
#endif /* __cplusplus */

struct ResourceId {
  const char *type;
  const char *id;
};

enum LifecycleState {
  LIFECYCLE_STATE_UNSPECIFIED,
  ACTIVE,
  DELETE_REQUESTED,
  DELETE_IN_PROGRESS /* unused */
};

struct Project {
  const char *projectNumber;
  const char *projectId;
  enum LifecycleState lifecycleState;
  const char *name;
  const char *createTime;
  /*"labels": {*/ /* Label values must be between 0 and 63 characters long
                   * and must conform to the regular expression
                   * [a-z0-9_-]{0,63}. A label value can be empty. */
  struct ResourceId *parent;
};

extern GOOGLE_CLOUD_C_EXPORT bool project_exists(const char *project_id);

extern GOOGLE_CLOUD_C_EXPORT struct Project project_get(const char *project_id);

extern GOOGLE_CLOUD_C_EXPORT const struct Project *project_list();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_GOOGLE_CLOUD_C_PROJECT_PROJECT_H */
