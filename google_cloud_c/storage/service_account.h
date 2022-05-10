#ifndef GOOGLE_CLOUD_C_STORAGE_SERVICE_ACCOUNT_H
#define GOOGLE_CLOUD_C_STORAGE_SERVICE_ACCOUNT_H

#ifdef __cplusplus
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __cplusplus */

#include <google_cloud_c/storage/google_cloud_c_storage_export.h>

/* https://cloud.google.com/storage/docs/json_api/v1/projects/serviceAccount#resource
 */
struct ProjectServiceAccount {
  const char *email_address, *kind;
};

struct OptionalProjectServiceAccount {
  bool set;
  struct ProjectServiceAccount project_service_account;
};

extern GOOGLE_CLOUD_C_STORAGE_EXPORT const struct ProjectServiceAccount
    projectServiceAccountNull;

/* Get the email address of this project's Cloud Storage service account, also
 * known as the service agent.
 * https://cloud.google.com/storage/docs/json_api/v1/projects/serviceAccount/get
 */
extern GOOGLE_CLOUD_C_STORAGE_EXPORT struct OptionalProjectServiceAccount
get_service_account(const char *);

/* utility functions */

extern GOOGLE_CLOUD_C_STORAGE_EXPORT struct ProjectServiceAccount
project_service_account_from_json(const JSON_Object *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_STORAGE_SERVICE_ACCOUNT_H */
