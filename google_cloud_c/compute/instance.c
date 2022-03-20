#include "instance.h"
#include "compute_common.h"

struct InstanceContext INSTANCE_CONTEXT = {NULL, NULL, NULL};

const struct Instance EMPTY_INSTANCE = {NULL,
                                        NULL,
                                        NULL,
                                        NULL,
                                        NULL,
                                        NULL,

                                        /*NetworkInterface*/ NULL,
                                        /*struct Disk*/ NULL,
                                        /*Metadata*/ NULL,
                                        /* selfLink*/ NULL,
                                        NULL,
                                        NULL,
                                        NULL,
                                        false,
                                        false,
                                        NULL,
                                        NULL,
                                        NULL,
                                        NULL,
                                        NULL};

const struct Instances EMPTY_INSTANCES = {NULL, 0};

struct OptionalInstance instance_from_json(const JSON_Object *jsonObject) {
  assert(!json_object_has_value(jsonObject, "operationType"));
  struct NetworkInterface *networkInterfaces;

  if (!json_object_has_value(jsonObject, "name")) {
    const struct OptionalInstance optionalInstance = {false, EMPTY_INSTANCE};
    return optionalInstance;
  }

  {
    const JSON_Array *network_json_items =
        json_object_get_array(jsonObject, "networkInterfaces");
    const size_t network_json_items_n =
        json_array_get_count(network_json_items);
    size_t i;
    networkInterfaces = (struct NetworkInterface *)malloc(
        network_json_items_n * sizeof(struct NetworkInterface));
    for (i = 0; i < network_json_items_n; i++) {
      const JSON_Object *network_json =
          json_array_get_object(network_json_items, i);
      struct AccessConfigs *accessConfigs;
      {
        const JSON_Array *ac_json_items =
            json_object_get_array(network_json, "accessConfigs");
        const size_t ac_json_items_n = json_array_get_count(ac_json_items);
        size_t j;
        accessConfigs = (struct AccessConfigs *)malloc(
            ac_json_items_n * sizeof(struct AccessConfigs));
        for (j = 0; j < ac_json_items_n; j++) {
          const JSON_Object *ac_json = json_array_get_object(ac_json_items, i);
          const struct AccessConfigs ac = {
              json_object_get_string(ac_json, "type"),
              json_object_get_string(ac_json, "name"),
              json_object_get_string(ac_json, "natIP"),
              json_object_get_string(ac_json, "networkTier"),
              json_object_get_string(ac_json, "kind")};

          accessConfigs[j] = ac;
        }
      }

      const struct NetworkInterface networkInterface = {
          json_object_get_string(network_json, "network"),
          json_object_get_string(network_json, "subnetwork"),
          json_object_get_string(network_json, "networkIP"),
          json_object_get_string(network_json, "name"), accessConfigs};
      networkInterfaces[i] = networkInterface;
    }
  }

  struct Metadata metadata = {NULL /*std::vector<struct Item>()*/, ""};

  struct Scheduling scheduling = {"", true, true};

  struct ShieldedInstanceConfig shieldedInstanceConfig = {false, true, true};

  struct ShieldedInstanceIntegrityPolicy shieldedInstanceIntegrityPolicy = {
      true};

  const struct Instance instance = {
      json_object_get_string(jsonObject, "id"),
      json_object_get_string(jsonObject, "creationTimestamp"),
      json_object_get_string(jsonObject, "name"),
      json_object_get_string(jsonObject, "machineType"),
      json_object_get_string(jsonObject, "status"),
      json_object_get_string(jsonObject, "zone"),

      networkInterfaces,

      NULL /*std::vector<struct Disk>()*/,

      &metadata,

      json_object_get_string(jsonObject, "selfLink"),

      &scheduling,

      json_object_get_string(jsonObject, "cpuPlatform"),
      json_object_get_string(jsonObject, "labelFingerprint"),
      (bool)json_object_get_boolean(jsonObject, "startRestricted"),
      (bool)json_object_get_boolean(jsonObject, "deletionProtection"),
      &shieldedInstanceConfig,
      &shieldedInstanceIntegrityPolicy,
      json_object_get_string(jsonObject, "fingerprint"),
      json_object_get_string(jsonObject, "lastStartTimestamp"),
      json_object_get_string(jsonObject, "kind")};
  const struct OptionalInstance optionalInstance = {true, instance};
  return optionalInstance;
}

bool instance_exists(const char *instance_name) {
  /* CHECK IF INSTANCE EXISTS */
  /* https://cloud.google.com/compute/docs/reference/rest/v1/instances/get
   * GET
   * https://compute.googleapis.com/compute/v1/projects/{project}/zones/{zone}/instances/{resourceId}
   */

  bool instance_exists = false;
  {
    char *path;
    asprintf(&path, "/v1/projects/%s/zones/%s/instances/%s",
             AUTH_CONTEXT.project_id, INSTANCE_CONTEXT.zone, instance_name);
    struct ServerResponse response = gcloud_get(NULL, path, NULL);
    DEBUG_SERVER_RESPONSE("instance_exists");
    instance_exists = response.status_code == 200;
  }
  return instance_exists;
}

struct Instances instances_list() {
  /* https://cloud.google.com/compute/docs/reference/rest/v1/instances/list
   * GET
   * https://compute.googleapis.com/compute/v1/projects/{project}/zones/{zone}/instances
   */
  char *path;
  asprintf(&path, "/v1/projects/%s/zones/%s/instances", AUTH_CONTEXT.project_id,
           INSTANCE_CONTEXT.zone);
  struct ServerResponse response = gcloud_get(NULL, path, NULL);
  DEBUG_SERVER_RESPONSE("instances_list");
  if (response.status_code == 404)
    return EMPTY_INSTANCES;
  assert(response.status_code == 200 && strlen(response.body) > 0);

  const JSON_Value *json_item =
      if_error_exit(json_parse_string(response.body), false);
  const JSON_Array *json_items =
      json_object_get_array(json_value_get_object(json_item), "items");
  const size_t json_items_n = json_array_get_count(json_items);

  struct Instance *instances =
      (struct Instance *)malloc(json_items_n * sizeof(struct Instance));
  size_t i;
  for (i = 0; i < json_items_n; i++) {
    const JSON_Object *json_obj = json_array_get_object(json_items, i);
    const struct OptionalInstance optionalInstance =
        instance_from_json(json_obj);
    assert(optionalInstance.set == true);
    instances[i] = optionalInstance.instance;
  }

  const struct Instances _instances = {instances, json_items_n};
  return _instances;
}

struct OptionalInstance instance_get(const char *instance_name) {
  /* https://cloud.google.com/compute/docs/reference/rest/v1/instances/get
   * GET
   * https://compute.googleapis.com/compute/v1/projects/{project}/zones/{zone}/instances/{resourceId}*/

  char *path;
  asprintf(&path, "/v1/projects/%s/zones/%s/instances/%s",
           AUTH_CONTEXT.project_id, INSTANCE_CONTEXT.zone, instance_name);
  struct ServerResponse response = gcloud_get(NULL, path, NULL);
  DEBUG_SERVER_RESPONSE("instance_get");
  if (response.status_code == 404) {
    const struct OptionalInstance optionalInstance = {false, EMPTY_INSTANCE};
    return optionalInstance;
  }
  assert(response.status_code == 200 && strlen(response.body) > 0);

  const JSON_Value *json_item =
      if_error_exit(json_parse_string(response.body), false);
  const JSON_Object *json_object = json_value_get_object(json_item);

  return instance_from_json(json_object);
}

const char *instance_to_json(const struct InstanceIncomplete *instance) {
  char *instance_json_str;
  asprintf(&instance_json_str,
           "{\n"
           "  \"name\": \"%s\",\n"
           "  \"zone\": \"%s\",\n"
           "  \"machineType\": \"%s\",\n"
           "  \"cpuPlatform\": \"%s\",\n"
           "  \"hostname\": \"%s\",\n"
           "  \"kind\": \"%s\"\n"
           "}",
           instance->name, instance->zone, instance->machineType,
           instance->cpuPlatform, instance->hostname, instance->kind);
  return instance_json_str;
}

struct OptionalInstance
instance_insert(const struct InstanceIncomplete *instance,
                const char *network_name) {
  /* CREATE THE INSTANCE (with startup script) */
  /* https://cloud.google.com/compute/docs/reference/rest/v1/instances/insert
   * POST
   * https://compute.googleapis.com/compute/v1/projects/{project}/zones/{zone}/instances
   */

  const char *shell_script = "#!/bin/bash\\n\\n"
                             "apt-get update\\n"
                             "apt-get install -y apache2\\n"
                             "echo gscripppt > /var/www/html/index.html\\n";

  char *path, *body;
  asprintf(&path, "/v1/projects/%s/zones/%s/instances", AUTH_CONTEXT.project_id,
           INSTANCE_CONTEXT.zone);
  asprintf(&body,
           "{\n"
           "  \"name\": \"%s\",\n"
           "  \"zone\": \"%s\",\n"
           "  \"machineType\": \"zones/%s/machineTypes/%s\",\n"
           "  \"disks\": [\n"
           "    {\n"
           "      \"autoDelete\": true,\n"
           "      \"boot\": true,\n"
           "      \"initializeParams\": {\n"
           "        \"sourceImage\": "
           "\"projects/debian-cloud/global/images/family/debian-10\"\n"
           "      },\n"
           "      \"mode\": \"READ_WRITE\",\n"
           "      \"type\": \"PERSISTENT\"\n"
           "    }\n"
           "  ],\n"
           /*"  \"cpuPlatform\": \"" << instance.cpuPlatform <<
           "\""
           "  ,"
           "\"hostname\": \"" << instance.hostname <<
           "\""
           "  ,"
           "  \"kind\": \"" << instance.kind <<
           "\""
           "  ,"*/
           "  \"networkInterfaces\": [\n"
           "    {\n"
           "      \"accessConfigs\": [\n"
           "          {\n"
           "            \"name\": \"external-nat\",\n"
           "            \"type\": \"ONE_TO_ONE_NAT\"\n"
           "          }\n"
           "      ],\n"
           "      \"network\": "
           "\"https://compute.googleapis.com/compute/v1/projects/%s/global/"
           "networks/%s\"\n"
           /*",      \"subnetwork\":"
           "\"regions/" << instance.zone.substr(0, instance.zone.rfind('-')) <<
           "/subnetworks/" << network_name << "\"" */
           "    }\n"
           "  ],\n"
           "  \"metadata\": {\n"
           "    \"items\": [\n"
           "      {\n"
           "        \"key\": \"startup-script\",\n"
           "        \"value\": \"%s\"\n"
           "      }\n"
           "    ]\n"
           "  }\n"
           "}",
           instance->name, instance->zone, INSTANCE_CONTEXT.zone,
           instance->machineType, AUTH_CONTEXT.project_id, network_name,
           shell_script);

  struct ServerResponse response = gcloud_post(NULL, path, body, NULL);
  DEBUG_SERVER_RESPONSE("instance_insert");

  if (response.status_code == 404) {
    const struct OptionalInstance optionalInstance = {false, EMPTY_INSTANCE};
    return optionalInstance;
  }

  assert((response.status_code == 200 || response.status_code == 201) &&
         strlen(response.body) > 0);

  const JSON_Value *json_item =
      if_error_exit(json_parse_string(response.body), false);
  const JSON_Object *json_object = json_value_get_object(json_item);

  if (json_object_has_value(json_object, "machineType"))
    return instance_from_json(json_object);
  else if (json_object_has_value(json_object, "items")) {
    const JSON_Array *_json_items = json_object_get_array(json_object, "items");
    const JSON_Object *_json_object = json_array_get_object(_json_items, 0);
    if (json_object_has_value(_json_object, "machineType"))
      return instance_from_json(_json_object);
  } else if (json_object_has_value(json_object, "kind") &&
             strcmp(json_object_get_string(json_object, "kind"),
                    "compute#operation") == 0) {
    const struct OptionalGoogleCloudOperation optionalGoogleCloudOperation =
        google_cloud_operation_from_json(json_object);
    if (optionalGoogleCloudOperation.set) {
      const struct GoogleCloudOperation googleCloudOperation =
          optionalGoogleCloudOperation.googleCloudOperation;
      if (strcmp(googleCloudOperation.status, "RUNNING") == 0)
        return instance_get(instance->name);
      else {
        const struct OptionalInstance optionalInstance = {false,
                                                          EMPTY_INSTANCE};
        fprintf(stderr, "instance_insert operation not RUNNING. Got:\n%s\n",
                json_serialize_to_string_pretty(json_item));
        return optionalInstance;
      }
    } else {
      const struct OptionalInstance optionalInstance = {false, EMPTY_INSTANCE};
      fprintf(
          stderr,
          "instance_insert JSON object missing required attributes. Got:\n%s\n",
          json_serialize_to_string_pretty(json_item));
      return optionalInstance;
    }
  }

  const struct OptionalInstance optionalInstance = {false, EMPTY_INSTANCE};
  fprintf(stderr, "instance_insert final else block on %s\n",
          json_serialize_to_string_pretty(json_item));
  return optionalInstance;
}

struct OptionalInstance
instance_incomplete_create_all(const struct InstanceIncomplete *instance,
                               const char *network_name,
                               const char *firewall_name) {
  /* Creates network, firewall, and instance */

  /* TODO: Proper request/response handling with structs and all for network and
   * firewall rules */

  char *_network_name, *_firewall_name;
  if (network_name == NULL) {
    asprintf(&_network_name, "%s-net", instance->name);
    network_name = _network_name;
  }
  if (firewall_name == NULL) {
    asprintf(&_firewall_name, "%s-https-http-8080-fw", instance->name);
    firewall_name = _firewall_name;
  }

  struct OptionalNetwork optionalNetwork = network_get(network_name);

  bool network_existent = optionalNetwork.set, firewall_existent = false,
       instance_existent = false;

  puts("instance_incomplete_create_all dance");

  if (!network_existent)
  create_network : {
    struct OptionalNetwork found_network = network_create(network_name);
    if (!found_network.set)
      found_network.network.name = network_name;
    printf("Creating the network \"%s\"\n", found_network.network.name);

    /* Wait 30 seconds: TODO check status continually until it's ready */
#ifdef _WIN32
    Sleep(30000);
#else
    sleep(30);
#endif

    do {
      printf("Waiting for network \"%s\" to be created.\n", network_name);
#ifdef _WIN32
      Sleep(10000);
#else
      sleep(100);
#endif
      found_network = network_get(network_name);
    } while (!found_network.set);
  }

    firewall_existent = firewall_exists(firewall_name);

  if (!firewall_existent) {
    struct OptionalFirewall optionalFirewall =
        firewall_create(network_name, firewall_name);
    printf("Firewall created: \"%s\"\n", optionalFirewall.firewall.name);

    /* Wait 30 seconds: TODO check status continually until it's ready */
#ifdef _WIN32
    Sleep(30000);
#else
    sleep(30);
#endif

    unsigned short max_attempts = 4;
    do {
      printf("Waiting for firewall \"%s\" to be created.\n", firewall_name);
#ifdef _WIN32
      Sleep(10000);
#else
      sleep(100);
#endif
      optionalFirewall = firewall_get(network_name);
      if (--max_attempts == 0 && !optionalFirewall.set) {
        max_attempts = 4;
        goto create_network;
      }
    } while (!optionalFirewall.set);
  }

  instance_existent = instance_exists(instance->name);

  struct OptionalInstance optionalInstance;
  if (!instance_existent) {
    optionalInstance = instance_insert(instance, network_name);
    if (optionalInstance.instance.name == NULL)
      optionalInstance.instance.name = instance->name;
    printf("Creating instance: \"%s\"\n", optionalInstance.instance.name);
    if (strcmp(optionalInstance.instance.status, "RUNNING") == 0)
      return optionalInstance;

      /* Wait 30 seconds: TODO check status continually until it's ready */
#ifdef _WIN32
    Sleep(30000);
#else
    sleep(30);
#endif

    unsigned short max_attempts = 4;
    do {
      if (--max_attempts == 0) {
        max_attempts = 4;
        goto create_network;
      }
      printf("Waiting for instance \"%s\" to be created.\n",
             optionalInstance.instance.name);
#ifdef _WIN32
      Sleep(10000);
#else
      sleep(100);
#endif
      optionalInstance = instance_get(optionalInstance.instance.name);
    } while (!optionalInstance.set);
  } else
    optionalInstance = instance_get((*instance).name);

  return optionalInstance;
}
