#include <c89stringutils_string_extras.h>

#include <google_cloud_c/compute/network.h>

bool network_exists(const char *const network) {
  /* CHECK IF NETWORK EXISTS */
  /* https://cloud.google.com/compute/docs/reference/rest/v1/networks/get
   * GET
   * https://compute.googleapis.com/compute/v1/projects/{project}/global/networks/{resourceId}
   */

  char *path;
  asprintf(&path, "/v1/projects/%s/global/networks/%s", AUTH_CONTEXT.project_id,
           network);
  {
    const struct ServerResponse response = gcloud_get(NULL, path, NULL);
    DEBUG_SERVER_RESPONSE("exists");
    return response.status_code == 200;
  }
}

struct Network *network_create(const char *const network_name) {
  /* CREATE THE NETWORK */
  /* https://cloud.google.com/compute/docs/reference/rest/v1/networks/insert
   * POST
   * https://compute.googleapis.com/compute/v1/projects/{project}/global/networks
   */
  char *body, *path;
  asprintf(&body,
           "{\n"
           "  \"name\": \"%s\",\n"
           "  \"autoCreateSubnetworks\": true,\n"
           "  \"routingConfig\": {\n"
           "    \"routingMode\": \"REGIONAL\"\n"
           "  }\n"
           "}",
           network_name);
  asprintf(&path, "/v1/projects/%s/global/networks", AUTH_CONTEXT.project_id);
  {
    const struct ServerResponse response = gcloud_post(NULL, path, body, NULL);
    struct Network *optional_network = NULL;
    DEBUG_SERVER_RESPONSE("network_create");
    if (response.body != NULL && response.body[0] != '\0') {
      const JSON_Value *const res_json_value = json_parse_string(response.body);
      const JSON_Object *const res_json_object =
          json_value_get_object(res_json_value);

      if (json_object_has_value(res_json_object, "error")) {
        const JSON_Object *err =
            json_object_get_object(res_json_object, "error");
        fprintf(stderr, "[code=%u] %s",
                (unsigned)json_object_get_number(err, "code"),
                json_object_get_string(err, "message"));
      } else if (!json_object_has_value(res_json_object,
                                        "autoCreateSubnetworks"))
        fputs(response.body, stderr);
      else
        optional_network = network_from_json(res_json_object);
    } else
      fputs("Empty response.body", stderr);
    return optional_network;
  }
}

struct Network *network_get(const char *const network_name) {
  char *path;
  asprintf(&path, "/v1/projects/%s/global/networks/%s", AUTH_CONTEXT.project_id,
           network_name);
  {
    const struct ServerResponse response = gcloud_get(NULL, path, NULL);
    struct Network *optional_network = NULL;
    DEBUG_SERVER_RESPONSE("network_get");
    if (response.body != NULL && response.body[0] != '\0') {
      /* const char *const _body = strstr(response.c_str, "\r\n\r\n"); */
      const JSON_Value *const res_json_value = json_parse_string(response.body);
      const JSON_Object *const res_json_object =
          json_value_get_object(res_json_value);

      if (json_object_has_value(res_json_object, "error")) {
        const JSON_Object *err =
            json_object_get_object(res_json_object, "error");
        fprintf(stderr, "[code=%u] %s",
                (unsigned)json_object_get_number(err, "code"),
                json_object_get_string(err, "message"));
      } else if (!json_object_has_value(res_json_object,
                                        "autoCreateSubnetworks"))
        fputs(response.body, stderr);
      else
        optional_network = network_from_json(res_json_object);
    } else
      fputs("Empty response.body", stderr);
    return optional_network;
  }
}

/* utility functions */

struct Network *network_from_json(const JSON_Object *const jsonObject) {
  struct Network *network = malloc(sizeof *network);
  network->id = json_object_get_string(jsonObject, "id");
  network->creationTimestamp =
      json_object_get_string(jsonObject, "creationTimestamp");
  network->name = json_object_get_string(jsonObject, "name");
  network->selfLink = json_object_get_string(jsonObject, "selfLink");
  if (json_object_has_value_of_type(jsonObject, "autoCreateSubnetworks",
                                    JSONBoolean))
    network->autoCreateSubnetworks =
        (bool)json_object_get_boolean(jsonObject, "autoCreateSubnetworks");
  else
    network->autoCreateSubnetworks = false;
  network->subnetworks =
      NULL /*json_object_get_string(res_json_object, "subnetworks")*/;
  network->routingConfig =
      NULL /*json_object_get_string(res_json_object, "routingMode")*/;
  network->kind = json_object_get_string(jsonObject, "kind");
  return network;
}
