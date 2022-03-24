#include <google_cloud_c/project/project.h>
#include <json_common.h>
#include <parson.h>

enum LifecycleState str_to_LifecycleState(const char *);
struct Project project_parse(const JSON_Object *);

bool project_exists(const char *project_id) {
  /* CHECK IF PROJECT EXISTS */
  /* https://cloud.google.com/resource-manager/reference/rest/v1/projects/get
   * GET https://cloudresourcemanager.googleapis.com/v1/projects/{projectId} */

  bool project_exists = false;
  {
    char *path;
    {
      asprintf(&path, "/v1/projects/%s", project_id);
      {
        struct ServerResponse response =
            gcloud_cloud_resource_get(NULL, path, NULL);
        DEBUG_SERVER_RESPONSE("projects_get_response");
        project_exists = response.status_code == 200;
      }
    }
  }
  return project_exists;
}

struct Project project_get(const char *project_id) {
  /* https://cloud.google.com/resource-manager/reference/rest/v1/projects/get
   * GET https://cloudresourcemanager.googleapis.com/v1/projects/{projectId} */
  char *path;
  asprintf(&path, "/v1/projects/%s", project_id);
  {
    struct ServerResponse response =
        gcloud_cloud_resource_get(NULL, path, NULL);
    DEBUG_SERVER_RESPONSE("projects_get_response");

    if_bad_status_exit(&response);

    return project_parse(
        json_value_get_object(json_parse_string(response.body)));
  }
}

const struct Project *project_list() {
  /* https://cloud.google.com/resource-manager/reference/rest/v1/projects/list
   * GET https://cloudresourcemanager.googleapis.com/v1/projects
   * */
  struct ServerResponse response =
      gcloud_cloud_resource_get(NULL, "/v1/projects", NULL);

  DEBUG_SERVER_RESPONSE("project_list");
  if_bad_status_exit(&response);

  const JSON_Value *json_value = json_parse_string(response.body);
  const JSON_Array *projects_items_json =
      json_object_get_array(json_value_get_object(json_value), "projects");
  const size_t projects_items_n = json_array_get_count(projects_items_json);
  /* if (projects_items_n == 0) return NULL; */
  struct Project *projects =
      (struct Project *)malloc(projects_items_n * sizeof(struct Project));
  size_t i;
  for (i = 0; i < projects_items_n; i++) {
    const struct Project project =
        project_parse(json_array_get_object(projects_items_json, i));
    projects[i] = project;
  }
  return NULL;
}

/* Utility functions */

enum LifecycleState str_to_LifecycleState(const char *state) {
  if (strcmp(state, "LIFECYCLE_STATE_UNSPECIFIED") == 0)
    return LIFECYCLE_STATE_UNSPECIFIED;
  else if (strcmp(state, "ACTIVE") == 0)
    return ACTIVE;
  else if (strcmp(state, "DELETE_REQUESTED") == 0)
    return DELETE_REQUESTED;
  return DELETE_IN_PROGRESS /* unused */;
}

struct Project project_parse(const JSON_Object *project_obj) {
  struct Project project;
  if (json_object_has_value(project_obj, "parent")) {
    JSON_Object *parent_obj = json_object_get_object(project_obj, "parent");
    struct ResourceId parent;
    parent.type = json_object_get_string(parent_obj, "type");
    parent.id = json_object_get_string(parent_obj, "id");
    project.parent = &parent;
  }

  project.projectNumber = json_object_get_string(project_obj, "projectNumber");
  project.projectId = json_object_get_string(project_obj, "projectId");
  project.lifecycleState = str_to_LifecycleState(
      json_object_get_string(project_obj, "lifecycleState"));
  project.name = json_object_get_string(project_obj, "name");
  project.createTime = json_object_get_string(project_obj, "createTime");

  return project;
}
