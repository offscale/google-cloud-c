#include "project.h"

bool project_exists(const char *project_id) {
  /* CHECK IF PROJECT EXISTS */
  /* https://cloud.google.com/resource-manager/reference/rest/v1/projects/get
   * GET https://cloudresourcemanager.googleapis.com/v1/projects/{projectId} */

  bool project_exists = false;
  {
    char *path;
    asprintf(&path,
             "https://cloudresourcemanager.googleapis.com/v1/projects/%s",
             project_id);
    struct ServerResponse response = gcloud_get(NULL, path, NULL);
    DEBUG_SERVER_RESPONSE("projects_get_response");
    project_exists = response.status_code == 200;
  }
  return project_exists;
}

const JSON_Object *get_project(const char *google_access_token) {
  /* https://cloud.google.com/resource-manager/reference/rest/v1/projects/list
   * GET https://cloudresourcemanager.googleapis.com/v1/projects
   * */
  CURLU *urlp = curl_url();
  CURLUcode rc = CURLUE_OK;
  struct curl_slist *headers = NULL;
  char *auth_header;
  struct ServerResponse response = {CURLE_OK, NULL, 100, NULL};

  rc = curl_url_set(urlp, CURLUPART_SCHEME, "https", 0);
  rc = curl_url_set(urlp, CURLUPART_HOST, "cloudresourcemanager.googleapis.com",
                    0);
  rc = curl_url_set(urlp, CURLUPART_PATH, "/v1/projects", 0);
  if (rc != CURLUE_OK)
    return NULL;

  asprintf(&auth_header, "Authorization: Bearer %s", google_access_token);
  headers = curl_slist_append(headers, auth_header);
  response = https_json_get(urlp, headers);
  free(auth_header);
  DEBUG_SERVER_RESPONSE("get_project");
  if (response.status_code > 299 || response.body == NULL ||
      response.code != CURLE_OK) {
    fprintf(stderr, "request failed to get projects\n\n%s\n", response.body);
    exit(EXIT_FAILURE);
  }
  if_bad_status_exit(&response);

  const JSON_Value *json_value = json_parse_string(response.body);
  const JSON_Array *projects_items_json =
      json_object_get_array(json_value_get_object(json_value), "projects");
  const size_t projects_items_n = json_array_get_count(projects_items_json);
  if (projects_items_n == 0)
    no_projects_error();
  size_t i;
  for (i = 0; i < projects_items_n; i++) {
    const JSON_Object *project_obj =
        json_array_get_object(projects_items_json, i);
    if (strcmp(json_object_get_string(project_obj, "lifecycleState"),
               "ACTIVE") == 0) {
      const char *projectId = json_object_get_string(project_obj, "projectId");
      const size_t projectId_n = strlen(projectId);
      const bool is_quickstart =
          (projectId_n > 11 /* len("quickstart-") */
           && projectId[0] == 'q' && projectId[1] == 'u' &&
           projectId[2] == 'i' && projectId[3] == 'c' && projectId[4] == 'k' &&
           projectId[5] == 's' && projectId[6] == 't' && projectId[7] == 'a' &&
           projectId[8] == 'r' && projectId[9] == 't' && projectId[10] == '-');
      if (!is_quickstart)
        return project_obj;
    }
  }
  return NULL;
}
