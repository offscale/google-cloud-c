#include <parson.h>

#include <c89stringutils_string_extras.h>

#include <google_cloud_c/project/project.h>

const struct Project projectNull = {NULL, NULL, LIFECYCLE_STATE_UNSPECIFIED,
                                    NULL, NULL, NULL};

bool project_exists(const char *const project_id) {
  /* CHECK IF PROJECT EXISTS */
  /* https://cloud.google.com/resource-manager/reference/rest/v1/projects/get
   * GET https://cloudresourcemanager.googleapis.com/v1/projects/{projectId} */

  bool exists = false;
  {
    char *path;
    {
      asprintf(&path, "/v1/projects/%s", project_id);
      {
        const struct ServerResponse response =
            gcloud_cloud_resource_get(NULL, path, NULL);
        DEBUG_SERVER_RESPONSE("projects_get_response");
        exists = response.status_code == 200;
      }
    }
  }
  return exists;
}

struct OptionalProject project_get(const char *const project_id) {
  /* https://cloud.google.com/resource-manager/reference/rest/v1/projects/get
   * GET https://cloudresourcemanager.googleapis.com/v1/projects/{projectId} */
  char *path;
  asprintf(&path, "/v1/projects/%s", project_id);
  {
    const struct ServerResponse response =
        gcloud_cloud_resource_get(NULL, path, NULL);
    struct OptionalProject optionalProject;
    DEBUG_SERVER_RESPONSE("projects_get_response");

    if (response.status_code == 200)
      optionalProject.set = true,
      optionalProject.project = project_from_json(
          json_value_get_object(json_parse_string(response.body)));
    else
      optionalProject.set = false, optionalProject.project = projectNull;
    return optionalProject;
  }
}

const struct Project *project_list(void) {
  /* https://cloud.google.com/resource-manager/reference/rest/v1/projects/list
   * GET https://cloudresourcemanager.googleapis.com/v1/projects
   * */
  const struct ServerResponse response =
      gcloud_cloud_resource_get(NULL, "/v1/projects", NULL);

  DEBUG_SERVER_RESPONSE("project_list");

  if (response.status_code == 200) {
    const JSON_Value *const json_value = json_parse_string(response.body);
    const JSON_Array *projects_items_json =
        json_object_get_array(json_value_get_object(json_value), "projects");
    const size_t projects_items_n = json_array_get_count(projects_items_json);
    /* if (projects_items_n == 0) return NULL; */
    if (projects_items_n > 0) {
      struct Project *projects =
          (struct Project *)malloc(projects_items_n * sizeof(struct Project));
      size_t i;
      for (i = 0; i < projects_items_n; i++) {
        const struct Project project =
            project_from_json(json_array_get_object(projects_items_json, i));
        projects[i] = project;
      }
      return projects;
    }
  }
  return NULL;
}

/* Utility functions */

enum LifecycleState str_to_LifecycleState(const char *const state) {
  if (strcmp(state, "LIFECYCLE_STATE_UNSPECIFIED") == 0)
    return LIFECYCLE_STATE_UNSPECIFIED;
  else if (strcmp(state, "ACTIVE") == 0)
    return ACTIVE;
  else if (strcmp(state, "DELETE_REQUESTED") == 0)
    return DELETE_REQUESTED;
  return DELETE_IN_PROGRESS /* unused */;
}

struct Project project_from_json(const JSON_Object *const jsonObject) {
  struct Project project;
  if (json_object_has_value(jsonObject, "parent")) {
    const JSON_Object *const parent_obj =
        json_object_get_object(jsonObject, "parent");
    struct ResourceId parent;
    parent.type = json_object_get_string(parent_obj, "type");
    parent.id = json_object_get_string(parent_obj, "id");
    project.parent = &parent;
  } else
    project.parent = NULL;

  project.projectNumber = json_object_get_string(jsonObject, "projectNumber");
  project.projectId = json_object_get_string(jsonObject, "projectId");
  if (json_object_has_value(jsonObject, "lifecycleState"))
    project.lifecycleState = str_to_LifecycleState(
        json_object_get_string(jsonObject, "lifecycleState"));
  else
    project.lifecycleState = LIFECYCLE_STATE_UNSPECIFIED;
  project.name = json_object_get_string(jsonObject, "name");
  project.createTime = json_object_get_string(jsonObject, "createTime");

  return project;
}
