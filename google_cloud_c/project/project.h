#ifndef GOOGLE_CLOUD_C_PROJECT_H
#define GOOGLE_CLOUD_C_PROJECT_H

#ifdef __cplusplus
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __cplusplus */

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/project/google_cloud_c_project_export.h>

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

/* Check if project exists.
 * https://cloud.google.com/resource-manager/reference/rest/v1/projects/get */
extern GOOGLE_CLOUD_C_PROJECT_EXPORT bool
project_exists(const char *project_id);

/* Retrieves the Project identified by the specified `projectId`
 * https://cloud.google.com/resource-manager/reference/rest/v1/projects/get */
extern GOOGLE_CLOUD_C_PROJECT_EXPORT struct Project *
project_get(const char *project_id);

/* Lists `Project`s that the caller has the `resourcemanager.projects.get`
 * permission on.
 * https://cloud.google.com/resource-manager/reference/rest/v1/projects/list */
extern GOOGLE_CLOUD_C_PROJECT_EXPORT struct Project **project_list(void);

/* Utility functions */

extern GOOGLE_CLOUD_C_PROJECT_EXPORT enum LifecycleState
str_to_LifecycleState(const char *);

extern GOOGLE_CLOUD_C_PROJECT_EXPORT struct Project *
project_from_json(const JSON_Object *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_PROJECT_H */
