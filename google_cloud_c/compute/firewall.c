#include "firewall.h"

const struct Firewall EMPTY_FIREWALL = {NULL};

bool firewall_exists(const char *firewall) {
  /* CHECK IF FIREWALL EXISTS */
  /* https://cloud.google.com/compute/docs/reference/rest/v1/firewalls/get
   * GET
   * https://compute.googleapis.com/compute/v1/projects/{project}/global/firewalls/{resourceId}
   */

  bool firewall_exists = false;
  {
    char *path;
    asprintf(&path, "/v1/projects/%s/global/firewalls/%s",
             AUTH_CONTEXT.project_id, firewall);
    struct ServerResponse response = gcloud_get(NULL, path, NULL);
    DEBUG_SERVER_RESPONSE("firewall_get_response");
    firewall_exists = response.status_code == 200;
  }
  return firewall_exists;
}

struct OptionalFirewall firewall_create(const char *network_name,
                                        const char *firewall_name) {
  /* CREATE THE FIREWALL + RULES */
  /* https://cloud.google.com/compute/docs/reference/rest/v1/firewalls/insert
   * POST
   * https://compute.googleapis.com/compute/v1/projects/{project}/global/firewalls
   */

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
  struct ServerResponse response = gcloud_post(NULL, path, body, NULL);
  DEBUG_SERVER_RESPONSE("firewall_create_response");
  const struct Firewall firewall = {firewall_name};
  const struct OptionalFirewall optionalFirewall = {response.status_code == 200,
                                                    firewall};
  return optionalFirewall;
}

struct OptionalFirewall firewall_get(const char *firewall_name) {
  char *path;
  asprintf(&path, "/v1/projects/%s/global/firewalls", AUTH_CONTEXT.project_id);

  struct ServerResponse response = gcloud_get(NULL, path, NULL);
  DEBUG_SERVER_RESPONSE("firewall_get");
  const struct Firewall firewall = {firewall_name};
  const struct OptionalFirewall optionalFirewall = {response.status_code == 200,
                                                    firewall};
  return optionalFirewall;
}
