#include <c89stringutils_string_extras.h>

#include <google_cloud_c/compute/firewall.h>

/* CHECK IF FIREWALL EXISTS */
/* https://cloud.google.com/compute/docs/reference/rest/v1/firewalls/get
 * GET
 * https://compute.googleapis.com/compute/v1/projects/{project}/global/firewalls/{resourceId}
 */
bool firewall_exists(const char *const firewall) {
  char *path;
  asprintf(&path, "/v1/projects/%s/global/firewalls/%s",
           AUTH_CONTEXT.project_id, firewall);
  {
    const struct ServerResponse response = gcloud_get(NULL, path, NULL);
    DEBUG_SERVER_RESPONSE("firewall_get_response");
    return response.status_code == 200;
  }
}

/* Creates a firewall rule in the specified project using the data included in
 * the request.
 * https://cloud.google.com/compute/docs/reference/rest/v1/firewalls/insert
 * POST
 * https://compute.googleapis.com/compute/v1/projects/{project}/global/firewalls
 */
struct Operation *firewall_create(const char *const network_name,
                                  const char *const firewall_name) {
  char *path, *body;
  asprintf(&path, "/v1/projects/%s/global/firewalls", AUTH_CONTEXT.project_id);
  asprintf(&body,
           "{\n"
           "  \"allowed\": [\n"
           "    {\n"
           "      \"IPProtocol\": \"tcp\",\n"
           "      \"ports\": [\n"
           "        \"22\"\n"
           "      ]\n"
           "    },\n"
           "    {\n"
           "      \"IPProtocol\": \"tcp\",\n"
           "      \"ports\": [\n"
           "        \"80\"\n"
           "      ]\n"
           "    },\n"
           "    {\n"
           "      \"IPProtocol\": \"tcp\",\n"
           "      \"ports\": [\n"
           "        \"8080\"\n"
           "      ]\n"
           "    },\n"
           "    {\n"
           "      \"IPProtocol\": \"tcp\",\n"
           "      \"ports\": [\n"
           "        \"443\"\n"
           "      ]\n"
           "    },\n"
           "    {\n"
           "      \"IPProtocol\": \"icmp\"\n"
           "    }\n"
           "  ],\n"
           "  \"direction\": \"INGRESS\",\n"
           "  \"name\": \"%s\",\n"
           "  \"network\": "
           "\"https://compute.googleapis.com/compute/v1/projects/%s/global/"
           "networks/%s\"\n"
           "}",
           firewall_name, AUTH_CONTEXT.project_id, network_name);
  {
    const struct ServerResponse response = gcloud_post(NULL, path, body, NULL);
    DEBUG_SERVER_RESPONSE("firewall_create_response");
    return response.status_code == 200
               ? operation_from_json(
                     json_value_get_object(json_parse_string(response.body)))
               : NULL;
  }
}

struct Firewall *firewall_get(const char *const firewall_name) {
  char *path;
  asprintf(&path, "/v1/projects/%s/global/firewalls", AUTH_CONTEXT.project_id);
  {
    const struct ServerResponse response = gcloud_get(NULL, path, NULL);
    DEBUG_SERVER_RESPONSE("firewall_get");
    return response.status_code == 200 ? Firewall_from_name(firewall_name)
                                       : NULL;
  }
}

/* Utility functions */

struct Firewall *Firewall_from_name(const char *const firewall_name) {
  struct Firewall *firewall = malloc(sizeof *firewall);
  firewall->name = firewall_name;
  return firewall;
}
