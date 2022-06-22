#include <c89stringutils_string_extras.h>

#include <google_cloud_c/compute/compute_common.h>
#include <google_cloud_c/compute/instance.h>

struct InstanceContext INSTANCE_CONTEXT = {NULL, NULL, NULL};

const struct Instances instancesNull = {NULL, 0};

bool instance_exists(const char *const instance_name) {
  /* CHECK IF INSTANCE EXISTS */
  /* https://cloud.google.com/compute/docs/reference/rest/v1/instances/get
   * GET
   * https://compute.googleapis.com/compute/v1/projects/{project}/zones/{zone}/instances/{resourceId}
   */

  char *path;
  asprintf(&path, "/v1/projects/%s/zones/%s/instances/%s",
           AUTH_CONTEXT.project_id, INSTANCE_CONTEXT.zone, instance_name);
  {
    const struct ServerResponse response = gcloud_get(NULL, path, NULL);
    DEBUG_SERVER_RESPONSE("instance_exists");
    return response.status_code == 200;
  }
}

struct Instances instances_list(void) {
  /* https://cloud.google.com/compute/docs/reference/rest/v1/instances/list
   * GET
   * https://compute.googleapis.com/compute/v1/projects/{project}/zones/{zone}/instances
   */
  char *path;
  asprintf(&path, "/v1/projects/%s/zones/%s/instances", AUTH_CONTEXT.project_id,
           INSTANCE_CONTEXT.zone);
  {
    const struct ServerResponse response = gcloud_get(NULL, path, NULL);
    struct Instances _instances = instancesNull;
    DEBUG_SERVER_RESPONSE("instances_list");
    if (response.status_code == 200 && response.body != NULL &&
        response.body[0] != '\0') {
      const JSON_Value *const json_item = json_parse_string(response.body);
      const JSON_Array *const json_items =
          json_object_get_array(json_value_get_object(json_item), "items");
      const size_t json_items_n = json_array_get_count(json_items);

      if (json_items_n > 0) {
        struct Instance **instances = malloc(json_items_n * sizeof **instances);
        size_t i;
        for (i = 0; i < json_items_n; i++) {
          const JSON_Object *const json_obj =
              json_array_get_object(json_items, i);
          struct Instance *const instance =
              optional_instance_from_json(json_obj);
          assert(instance != NULL);
          instances[i] = instance;
        }
        _instances.arr = instances, _instances.size = json_items_n;
      }
    }
    return _instances;
  }
}

struct Instance *instance_get(const char *const instance_name) {
  /* Returns the specified Instance resource.
   * https://cloud.google.com/compute/docs/reference/rest/v1/instances/get
   * GET
   * https://compute.googleapis.com/compute/v1/projects/{project}/zones/{zone}/instances/{resourceId}*/

  char *path;
  asprintf(&path, "/v1/projects/%s/zones/%s/instances/%s",
           AUTH_CONTEXT.project_id, INSTANCE_CONTEXT.zone, instance_name);
  {
    const struct ServerResponse response = gcloud_get(NULL, path, NULL);
    struct Instance *optionalInstance = NULL;
    DEBUG_SERVER_RESPONSE("instance_get");
    if (response.status_code == 200 && response.body != NULL &&
        response.body[0] != '\0')
      optionalInstance = optional_instance_from_json(
          json_value_get_object(json_parse_string(response.body)));
    else if (response.status_code != 404)
      fputs(response.body, stderr);
    return optionalInstance;
  }
}

struct Instance *instance_insert(const struct InstanceIncomplete *instance,
                                 const char *network_name,
                                 const char *shell_script) {
  /* CREATE THE INSTANCE (with startup script) */
  /* https://cloud.google.com/compute/docs/reference/rest/v1/instances/insert
   * POST
   * https://compute.googleapis.com/compute/v1/projects/{project}/zones/{zone}/instances
   */
  char *path, *body;

  shell_script = shell_script == NULL
                     ? "#!/bin/bash\\n\\n"
                       "apt-get update\\n"
                       "apt-get install -y apache2\\n"
                       "echo gscripppt > /var/www/html/index.html\\n"
                     : shell_script;

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

  {
    const struct ServerResponse response = gcloud_post(NULL, path, body, NULL);
    struct Instance *optionalInstance = NULL;
    DEBUG_SERVER_RESPONSE("instance_insert");

    if ((response.status_code == 200 || response.status_code == 201) &&
        response.body != NULL && response.body[0] != '\0') {
      const JSON_Value *const json_item = json_parse_string(response.body);
      const JSON_Object *const json_object = json_value_get_object(json_item);

      if (json_object_has_value(json_object, "machineType"))
        optionalInstance = optional_instance_from_json(json_object);
      else if (json_object_has_value(json_object, "items")) {
        const JSON_Array *const _json_items =
            json_object_get_array(json_object, "items");
        if (json_array_get_count(_json_items) > 0) {
          const JSON_Object *const _json_object =
              json_array_get_object(_json_items, 0);
          if (json_object_has_value(_json_object, "machineType"))
            optionalInstance = optional_instance_from_json(_json_object);
        }
      } else if (json_object_has_value(json_object, "kind") &&
                 strcmp(json_object_get_string(json_object, "kind"),
                        "compute#operation") == 0) {
        const struct GoogleCloudOperation *const optionalGoogleCloudOperation =
            google_cloud_operation_from_json(json_object);
        if (optionalGoogleCloudOperation != NULL) {
          if (strcmp(optionalGoogleCloudOperation->status, "RUNNING") == 0)
            optionalInstance = instance_get(instance->name);
          else
            fprintf(stderr, "instance_insert operation not RUNNING. Got:\n%s\n",
                    json_serialize_to_string_pretty(json_item));
        } else
          fprintf(stderr,
                  "instance_insert JSON object missing required attributes. "
                  "Got:\n%s\n",
                  json_serialize_to_string_pretty(json_item));
      }
    } else if (response.status_code != 404)
      fputs(response.body, stderr);
    return optionalInstance;
  }
}

struct Instance *instance_incomplete_create_all(
    const struct InstanceIncomplete *instance, const char *network_name,
    const char *firewall_name, const char *const shell_script) {
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

  {
    struct Network *optionalNetwork = network_get(network_name);

    bool network_existent = optionalNetwork != NULL, firewall_existent = false,
         instance_existent = false;

    puts("instance_incomplete_create_all dance");

    if (!network_existent) {
      struct Operation *network_operation = network_create(network_name);
      const char *const network_targetId = strdup(network_operation->targetId);
      printf("[%s] Creating the network \"%s\"\n",
             OperationStatus_to_str(network_operation->status), network_name);

      while (network_operation->status != DONE) {
        printf("[%s] Waiting for network \"%s\" to be created.\n", OperationStatus_to_str(network_operation->status), network_name);
#ifdef _WIN32
        Sleep(10000);
#else
        sleep(100);
#endif
        network_operation =
            global_operation_get(AUTH_CONTEXT.project_id, network_targetId);
      }
    }

    firewall_existent = firewall_exists(firewall_name);

    if (!firewall_existent) {
      struct Operation *firewall_operation =
          firewall_create(network_name, firewall_name);
      const char *const firewall_targetId =
          strdup(firewall_operation->targetId);
      printf("[%s] Creating the firewall \"%s\"\n",
             OperationStatus_to_str(firewall_operation->status), firewall_name);

      /* Wait 30 seconds: TODO check status continually until it's ready */
#ifdef _WIN32
      Sleep(30000);
#else
      sleep(30);
#endif

      while (firewall_operation->status != DONE) {
        printf("[%s] Waiting for firewall \"%s\" to be created.\n", OperationStatus_to_str(firewall_operation->status), firewall_name);
#ifdef _WIN32
        Sleep(10000);
#else
        sleep(100);
#endif
        firewall_operation =
            global_operation_get(AUTH_CONTEXT.project_id, firewall_targetId);
      }
    }

    instance_existent = instance_exists(instance->name);

    {
      struct Instance *instance_created;
      if (!instance_existent) {
        instance_created =
            instance_insert(instance, network_name, shell_script);
        printf("[%s] Creating instance: \"%s\"\n",
               InstanceStatus_to_str(instance_created->status),
               instance_created->name);
        while (instance_created->status != INSTANCE_RUNNING) {
          /* Wait 30 seconds: TODO check status continually until it's ready */
#ifdef _WIN32
          Sleep(30000);
#else
          sleep(30);
#endif

          printf("[%s] Waiting for instance \"%s\" to be created and start "
                 "running.\n",
                 InstanceStatus_to_str(instance_created->status),
                 instance->name);
#ifdef _WIN32
          Sleep(10000);
#else
          sleep(100);
#endif
          instance_created = instance_get(instance_created->name);
        }
      } else
        instance_created = instance_get((*instance).name);

      return instance_created;
    }
  }
}

/* Utility functions */

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

struct Instance *
optional_instance_from_json(const JSON_Object *const jsonObject) {
  struct Instance *optionalInstance = NULL;
  if (json_object_has_value(jsonObject, "operationType") &&
      json_object_has_value(jsonObject, "name")) {
    const JSON_Array *const network_json_items =
        json_object_get_array(jsonObject, "networkInterfaces");
    const size_t network_json_items_n =
        json_array_get_count(network_json_items) + 1;

    struct NetworkInterface **networkInterfaces = NULL;
    if (network_json_items_n > 1) {
      size_t i;
      networkInterfaces =
          malloc(network_json_items_n * sizeof **networkInterfaces);
      for (i = 0; i < network_json_items_n - 1; i++) {
        const JSON_Object *const network_json =
            json_array_get_object(network_json_items, i);
        struct AccessConfigs **accessConfigs = NULL;
        const JSON_Array *const ac_json_items =
            json_object_get_array(network_json, "accessConfigs");
        const size_t ac_json_items_n = json_array_get_count(ac_json_items);
        if (ac_json_items_n > 0) {
          size_t j;
          accessConfigs = malloc(ac_json_items_n * sizeof **accessConfigs);
          for (j = 0; j < ac_json_items_n; j++)
            accessConfigs[j] = AccessConfigs_from_json(
                json_array_get_object(ac_json_items, i));
        }

        {
          struct NetworkInterface *networkInterface =
              NetworkInterface_from_json(network_json);
          networkInterface->accessConfigs = accessConfigs;
          networkInterfaces[i] = networkInterface;
        }
      }
      networkInterfaces[network_json_items_n - 1] = NULL;
    }

    optionalInstance = instance_from_json(jsonObject);

    optionalInstance->networkInterfaces = networkInterfaces;

    {
      struct Metadata *metadata = NULL;
      optionalInstance->metadata = metadata;
    }

    {
      struct Scheduling *scheduling = malloc(sizeof *scheduling);
      scheduling->onHostMaintenance = NULL, scheduling->automaticRestart = true,
      scheduling->preemptible = true;
      optionalInstance->scheduling = scheduling;
    }

    {
      struct ShieldedInstanceConfig *shieldedInstanceConfig =
          malloc(sizeof *shieldedInstanceConfig);
      shieldedInstanceConfig->enableSecureBoot = false,
      shieldedInstanceConfig->enableVtpm = true,
      shieldedInstanceConfig->enableIntegrityMonitoring = true;
      optionalInstance->shieldedInstanceConfig = shieldedInstanceConfig;
    }

    {
      struct ShieldedInstanceIntegrityPolicy *shieldedInstanceIntegrityPolicy =
          malloc(sizeof *shieldedInstanceIntegrityPolicy);
      shieldedInstanceIntegrityPolicy->updateAutoLearnPolicy = true;

      optionalInstance->shieldedInstanceIntegrityPolicy =
          shieldedInstanceIntegrityPolicy;
    }
  }
  return optionalInstance;
}

/* Utility functions */

struct NetworkInterface *
NetworkInterface_from_json(const JSON_Object *const jsonObject) {
  struct NetworkInterface *networkInterface = malloc(sizeof *networkInterface);
  networkInterface->network = json_object_get_string(jsonObject, "network");
  networkInterface->subnetwork =
      json_object_get_string(jsonObject, "subnetwork");
  networkInterface->networkIP = json_object_get_string(jsonObject, "networkIP");
  networkInterface->name = json_object_get_string(jsonObject, "name");
  return networkInterface;
}

struct AccessConfigs *
AccessConfigs_from_json(const JSON_Object *const jsonObject) {
  struct AccessConfigs *accessConfigs = malloc(sizeof *accessConfigs);
  accessConfigs->type = json_object_get_string(jsonObject, "type");
  accessConfigs->name = json_object_get_string(jsonObject, "name");
  accessConfigs->natIP = json_object_get_string(jsonObject, "natIP");
  accessConfigs->networkTier =
      json_object_get_string(jsonObject, "networkTier");
  accessConfigs->kind = json_object_get_string(jsonObject, "kind");
  return accessConfigs;
}

struct Instance *instance_from_json(const JSON_Object *const jsonObject) {
  struct Instance *instance = malloc(sizeof *instance);
  instance->id = json_object_get_string(jsonObject, "id");
  instance->creationTimestamp =
      json_object_get_string(jsonObject, "creationTimestamp");
  instance->name = json_object_get_string(jsonObject, "name");
  instance->machineType = json_object_get_string(jsonObject, "machineType");
  if (json_object_has_value_of_type(jsonObject, "status", JSONString))
    instance->status =
        str_to_InstanceStatus(json_object_get_string(jsonObject, "status"));
  instance->zone = json_object_get_string(jsonObject, "zone");
  instance->disks = NULL /*std::vector<struct Disk>()*/;
  instance->selfLink = json_object_get_string(jsonObject, "selfLink");
  instance->cpuPlatform = json_object_get_string(jsonObject, "cpuPlatform");
  instance->labelFingerprint =
      json_object_get_string(jsonObject, "labelFingerprint");
  if (json_object_has_value_of_type(jsonObject, "startRestricted", JSONBoolean))
    instance->startRestricted =
        (bool)json_object_get_boolean(jsonObject, "startRestricted");
  else
    instance->startRestricted = false;
  if (json_object_has_value_of_type(jsonObject, "deletionProtection",
                                    JSONBoolean))
    instance->deletionProtection =
        (bool)json_object_get_boolean(jsonObject, "deletionProtection");
  else
    instance->startRestricted = false;
  instance->fingerprint = json_object_get_string(jsonObject, "fingerprint");
  instance->lastStartTimestamp =
      json_object_get_string(jsonObject, "lastStartTimestamp");
  instance->kind = json_object_get_string(jsonObject, "kind");
  return instance;
}

enum InstanceStatus str_to_InstanceStatus(const char *const status) {
  if (strcmp(status, "PROVISIONING") == 0)
    return PROVISIONING;
  else if (strcmp(status, "RUNNING") == 0)
    return INSTANCE_RUNNING;
  else if (strcmp(status, "STOPPING") == 0)
    return STOPPING;
  else if (strcmp(status, "SUSPENDING") == 0)
    return SUSPENDING;
  else if (strcmp(status, "SUSPENDED") == 0)
    return SUSPENDED;
  else if (strcmp(status, "REPAIRING") == 0)
    return REPAIRING;
  else if (strcmp(status, "TERMINATED") == 0)
    return TERMINATED;
  /*else if (strcmp(status, "STAGING") == 0)*/
  return STAGING;
}

const char *InstanceStatus_to_str(enum InstanceStatus status) {
  switch (status) {
  case PROVISIONING:
    return "PROVISIONING";
  case INSTANCE_RUNNING:
    return "RUNNING";
  case STOPPING:
    return "STOPPING";
  case SUSPENDING:
    return "SUSPENDING";
  case SUSPENDED:
    return "SUSPENDED";
  case REPAIRING:
    return "REPAIRING";
  case TERMINATED:
    return "TERMINATED";
  case STAGING:
  default:
    return "STAGING";
  }
}
