#include <parson.h>

#include <google_cloud_c/storage/service_account.h>
#include <google_cloud_c/client/cloud_auth.h>

const struct ProjectServiceAccount projectServiceAccountNull = {NULL, NULL};

/* GET
 * https://storage.googleapis.com/storage/v1/projects/projectIdentifier/serviceAccount
 * https://cloud.google.com/storage/docs/json_api/v1/projects/serviceAccount/get
 */
struct OptionalProjectServiceAccount
get_service_account(const char *projectIdentifier) {
  char *path;
  asprintf(&path, "/storage/v1/projects/%s/serviceAccount", projectIdentifier);
  {
    struct ServerResponse response = gcloud_storage_get(NULL, path, NULL);
    struct OptionalProjectServiceAccount optionalProjectServiceAccount;
    DEBUG_SERVER_RESPONSE("get_service_account");
    if (response.status_code == 200)
      optionalProjectServiceAccount.set = true,
      optionalProjectServiceAccount.project_service_account =
          project_service_account_from_json(
              json_value_get_object(json_parse_string(response.body)));
    else
      optionalProjectServiceAccount.set = false,
      optionalProjectServiceAccount.project_service_account =
          projectServiceAccountNull;
    return optionalProjectServiceAccount;
  }
}

/* utility functions */

struct ProjectServiceAccount
project_service_account_from_json(const JSON_Object *jsonObject) {
  struct ProjectServiceAccount project_service_account;
  project_service_account.email_address =
      json_object_get_string(jsonObject, "email_address");
  project_service_account.kind = json_object_get_string(jsonObject, "kind");
  return project_service_account;
}
