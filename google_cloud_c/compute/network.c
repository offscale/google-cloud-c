#include <google_cloud_c/compute/network.h>

const struct RoutingConfig routingConfigNull = {""};

const struct Network networkNull = {NULL, NULL, NULL, NULL,
                                    true, NULL, NULL, NULL};

bool network_exists(const char *const network) {
  /* CHECK IF NETWORK EXISTS */
  /* https://cloud.google.com/compute/docs/reference/rest/v1/networks/get
   * GET
   * https://compute.googleapis.com/compute/v1/projects/{project}/global/networks/{resourceId}
   */

  bool exists = false;
  {
    char *path;
    asprintf(&path, "/v1/projects/%s/global/networks/%s",
             AUTH_CONTEXT.project_id, network);
    {
      const struct ServerResponse response = gcloud_get(NULL, path, NULL);
      DEBUG_SERVER_RESPONSE("exists");
      exists = response.status_code == 200;
    }
  }
  return exists;
}

struct OptionalNetwork network_create(const char *const network_name) {
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
    struct OptionalNetwork optional_network;
    optional_network.set = false, optional_network.network = networkNull;
    DEBUG_SERVER_RESPONSE("network_create");
    if (response.body != NULL && response.body[0] != '\0') {
      const JSON_Value *const res_json_value = json_parse_string(response.body);
      const JSON_Object *const res_json_object =
          json_value_get_object(res_json_value);

      if (json_object_has_value(res_json_object, "error"))
        fputs(json_object_get_string(res_json_object, "error"), stderr);
      else if (!json_object_has_value(res_json_object, "autoCreateSubnetworks"))
        fputs(response.body, stderr);
      else
        optional_network.set = true,
        optional_network.network = network_from_json(res_json_object);
    } else
      fputs("Empty response.body", stderr);
    return optional_network;
  }
}

struct OptionalNetwork network_get(const char *const network_name) {
  char *path;
  asprintf(&path, "/v1/projects/%s/global/networks/%s", AUTH_CONTEXT.project_id,
           network_name);
  {
    const struct ServerResponse response = gcloud_get(NULL, path, NULL);
    struct OptionalNetwork optional_network;
    DEBUG_SERVER_RESPONSE("network_get");
    optional_network.set = false, optional_network.network = networkNull;
    if (response.body != NULL && response.body[0] != '\0') {
      /* const char *const _body = strstr(response.c_str, "\r\n\r\n"); */
      const JSON_Value *const res_json_value = json_parse_string(response.body);
      const JSON_Object *const res_json_object =
          json_value_get_object(res_json_value);

      if (json_object_has_value(res_json_object, "error"))
        fputs(json_object_get_string(res_json_object, "error"), stderr);
      else if (!json_object_has_value(res_json_object, "autoCreateSubnetworks"))
        fputs(response.body, stderr);
      else
        optional_network.set = true,
        optional_network.network = network_from_json(res_json_object);
    } else
      fputs("Empty response.body", stderr);
    return optional_network;
  }
}

/* utility functions */

struct Network network_from_json(const JSON_Object *const jsonObject) {
  struct Network network;
  network.id = json_object_get_string(jsonObject, "id");
  network.creationTimestamp =
      json_object_get_string(jsonObject, "creationTimestamp");
  network.name = json_object_get_string(jsonObject, "name");
  network.selfLink = json_object_get_string(jsonObject, "selfLink");
  if (json_object_has_value_of_type(jsonObject, "autoCreateSubnetworks",
                                    JSONBoolean))
    network.autoCreateSubnetworks =
        (bool)json_object_get_boolean(jsonObject, "autoCreateSubnetworks");
  else
    network.autoCreateSubnetworks = false;
  network.subnetworks =
      NULL /*json_object_get_string(res_json_object, "subnetworks")*/;
  network.routingConfig =
      NULL /*json_object_get_string(res_json_object, "routingMode")*/;
  network.kind = json_object_get_string(jsonObject, "kind");
  return network;
}
