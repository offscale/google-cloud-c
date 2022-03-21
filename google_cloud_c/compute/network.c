#include <compute/network.h>
#include <json_common.h>

const struct RoutingConfig routingConfigNull = {""};

const struct Network EMPTY_NETWORK = {NULL, NULL, NULL, NULL,
                                      true, NULL, NULL, NULL};

bool network_exists(const char *network) {
  /* CHECK IF NETWORK EXISTS */
  /* https://cloud.google.com/compute/docs/reference/rest/v1/networks/get
   * GET
   * https://compute.googleapis.com/compute/v1/projects/{project}/global/networks/{resourceId}
   */

  bool network_exists = false;
  {
    char *path;
    asprintf(&path, "/v1/projects/%s/global/networks/%s",
             AUTH_CONTEXT.project_id, network);
    struct ServerResponse response = gcloud_get(NULL, path, NULL);
    DEBUG_SERVER_RESPONSE("network_exists");
    network_exists = response.status_code == 200;
  }
  return network_exists;
}

struct OptionalNetwork network_create(const char *network_name) {
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
  struct ServerResponse response = gcloud_post(NULL, path, body, NULL);
  DEBUG_SERVER_RESPONSE("network_create");
  if (response.status_code == 200 && strlen(response.body) > 0) {
    const JSON_Value *created_json_value =
        if_error_exit(json_parse_string(response.body), false);
    const JSON_Object *created_json_object =
        json_value_get_object(created_json_value);

    if (!json_object_has_value(created_json_object, "operationType")) {
      const struct Network network = {
          json_object_get_string(created_json_object, "id"),
          json_object_get_string(created_json_object, "creationTimestamp"),
          json_object_get_string(created_json_object, "name"),
          json_object_get_string(created_json_object, "selfLink"),
          (bool)json_object_get_boolean(created_json_object,
                                        "autoCreateSubnetworks"),
          NULL /*json_object_get_string(created_json_object, "subnetworks")*/,
          NULL /*json_object_get_string(created_json_object, "routingMode")*/,
          json_object_get_string(created_json_object, "kind")};
      const struct OptionalNetwork optional_network = {false, network};
      return optional_network;
    }
  }
  const struct OptionalNetwork optional_network = {false, EMPTY_NETWORK};
  return optional_network;
}

struct OptionalNetwork network_get(const char *network_name) {
  char *path;
  asprintf(&path, "/v1/projects/%s/global/networks/%s", AUTH_CONTEXT.project_id,
           network_name);
  struct ServerResponse response = gcloud_get(NULL, path, NULL);
  DEBUG_SERVER_RESPONSE("network_get");
  if (response.status_code == 200 && strlen(response.body) > 0) {
    /* const char *_body = strstr(response.c_str, "\r\n\r\n"); */
    const JSON_Value *created_json_value =
        if_error_exit(json_parse_string(response.body), false);
    const JSON_Object *created_json_object =
        json_value_get_object(created_json_value);

    if (!json_object_has_value(created_json_object, "operationType")) {
      const struct Network network = {
          json_object_get_string(created_json_object, "id"),
          json_object_get_string(created_json_object, "creationTimestamp"),
          json_object_get_string(created_json_object, "name"),
          json_object_get_string(created_json_object, "selfLink"),
          (bool)json_object_get_boolean(created_json_object,
                                        "autoCreateSubnetworks"),
          NULL /*json_object_get_string(created_json_object, "subnetworks")*/,
          NULL /*json_object_get_string(created_json_object, "routingMode")*/,
          json_object_get_string(created_json_object, "kind")};
      const struct OptionalNetwork optional_network = {true, network};
      return optional_network;
    }
  }
  const struct OptionalNetwork optional_network = {false, EMPTY_NETWORK};
  return optional_network;
}
