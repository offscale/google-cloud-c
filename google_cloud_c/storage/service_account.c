#include <parson.h>

#include <c89stringutils_string_extras.h>

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/storage/service_account.h>

/* GET
 * https://storage.googleapis.com/storage/v1/projects/projectIdentifier/serviceAccount
 * https://cloud.google.com/storage/docs/json_api/v1/projects/serviceAccount/get
 */
struct ProjectServiceAccount *
get_service_account(const char *const projectIdentifier) {
  char *path;
  asprintf(&path, "/storage/v1/projects/%s/serviceAccount", projectIdentifier);
  {
    const struct ServerResponse response = gcloud_storage_get(NULL, path, NULL);
    DEBUG_SERVER_RESPONSE("get_service_account");
    return response.status_code == 200
               ? project_service_account_from_json(
                     json_value_get_object(json_parse_string(response.body)))
               : NULL;
  }
}

/* utility functions */

struct ProjectServiceAccount *
project_service_account_from_json(const JSON_Object *const jsonObject) {
  struct ProjectServiceAccount *project_service_account =
      malloc(sizeof(struct ProjectServiceAccount));
  project_service_account->email_address =
      json_object_get_string(jsonObject, "email_address");
  project_service_account->kind = json_object_get_string(jsonObject, "kind");
  return project_service_account;
}
