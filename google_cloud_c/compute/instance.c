#include <c89stringutils_string_extras.h>

#include <google_cloud_c/common/google_cloud_c_json_common.h>
#include <google_cloud_c/compute/compute_common.h>
#include <google_cloud_c/compute/instance.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#define strdup _strdup
#endif /* defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) */

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
          struct Instance *const instance = instance_from_json(json_obj);
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
      optionalInstance = instance_from_json(
          json_value_get_object(json_parse_string(response.body)));
    else if (response.status_code != 404)
      fputs(response.body, stderr);
    return optionalInstance;
  }
}

struct Instance *
instance_insert(const struct InstanceIncomplete *const instance,
                const char *const network_name, const char *shell_script) {
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
        optionalInstance = instance_from_json(json_object);
      else if (json_object_has_value(json_object, "items")) {
        const JSON_Array *const _json_items =
            json_object_get_array(json_object, "items");
        if (json_array_get_count(_json_items) > 0) {
          const JSON_Object *const _json_object =
              json_array_get_object(_json_items, 0);
          if (json_object_has_value(_json_object, "machineType"))
            optionalInstance = instance_from_json(_json_object);
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
    const struct InstanceIncomplete *const instance, const char *network_name,
    const char *firewall_name, const char *const shell_script) {
  /* Creates network, firewall, and instance */

  /* TODO: Proper request/response handling with structs and all for network and
   * firewall rules */
  char *_network_name, *_firewall_name;
  struct Network *optionalNetwork;

  bool network_existent, firewall_existent = false, instance_existent = false;

  struct Instance *instance_found;

  if (network_name == NULL) {
    asprintf(&_network_name, "%s-net", instance->name);
    network_name = _network_name;
  }
  if (firewall_name == NULL) {
    asprintf(&_firewall_name, "%s-https-http-8080-fw", instance->name);
    firewall_name = _firewall_name;
  }

  optionalNetwork = network_get(network_name);
  network_existent = optionalNetwork != NULL;

  puts("instance_incomplete_create_all dance");

  if (!network_existent) {
    struct Operation *network_operation = network_create(network_name);
    const char *const network_op_name = strdup(network_operation->name);
    printf("[%s] Creating the network \"%s\"\n",
           OperationStatus_to_str(network_operation->status), network_name);

    while (network_operation->status != DONE) {
      printf("[%s] Waiting for network \"%s\" to be created.\n",
             OperationStatus_to_str(network_operation->status), network_name);
#ifdef _WIN32
      Sleep(10000);
#else
      sleep(100);
#endif
      network_operation =
          global_operation_get(AUTH_CONTEXT.project_id, network_op_name);
    }
  }

  firewall_existent = firewall_exists(firewall_name);

  if (!firewall_existent) {
    struct Operation *firewall_operation =
        firewall_create(network_name, firewall_name);
    const char *const firewall_op_name = strdup(firewall_operation->name);
    printf("[%s] Creating the firewall \"%s\"\n",
           OperationStatus_to_str(firewall_operation->status), firewall_name);

    /* Wait 30 seconds: TODO check status continually until it's ready */
#ifdef _WIN32
    Sleep(30000);
#else
    sleep(30);
#endif

    while (firewall_operation->status != DONE) {
      printf("[%s] Waiting for firewall \"%s\" to be created.\n",
             OperationStatus_to_str(firewall_operation->status), firewall_name);
#ifdef _WIN32
      Sleep(10000);
#else
      sleep(100);
#endif
      firewall_operation =
          global_operation_get(AUTH_CONTEXT.project_id, firewall_op_name);
    }
  }

  instance_existent = instance_exists(instance->name);

  if (!instance_existent) {
    instance_found = instance_insert(instance, network_name, shell_script);
    printf("[%s] Creating instance: \"%s\"\n",
           InstanceStatus_to_str(instance_found->status), instance_found->name);
    while (instance_found->status != INSTANCE_RUNNING) {
      /* Wait 30 seconds: TODO check status continually until it's ready */
#ifdef _WIN32
      Sleep(30000);
#else
      sleep(30);
#endif

      printf("[%s] Waiting for instance \"%s\" to be created and start "
             "running.\n",
             InstanceStatus_to_str(instance_found->status), instance->name);
#ifdef _WIN32
      Sleep(10000);
#else
      sleep(100);
#endif
      instance_found = instance_get(instance_found->name);
    }
  } else
    instance_found = instance_get(instance->name);

  return instance_found;
}

/* Utility functions */

const char *instance_to_json(const struct InstanceIncomplete *const instance) {
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

const char *instance_complete_to_json(const struct Instance *const instance) {
  char *s = NULL;
  jasprintf(&s, "{");
  if (instance->kind != NULL && instance->kind[0] != '\0')
    jasprintf(&s, "  \"kind\": \"%s\",", instance->kind);

  if (instance->id != NULL && instance->id[0] != '\0')
    jasprintf(&s, "  \"id\": \"%s\",", instance->id);

  if (instance->creationTimestamp != NULL &&
      instance->creationTimestamp[0] != '\0')
    jasprintf(&s, "  \"creationTimestamp\": \"%s\",",
              instance->creationTimestamp);

  if (instance->name != NULL && instance->name[0] != '\0')
    jasprintf(&s, "  \"name\": \"%s\",", instance->name);

  /*if (instance->tags != NULL && instance->tags[0] != '\0')
    jasprintf(&s, "  \"tags\": \"%s\",", instance->tags);*/

  if (instance->machineType != NULL && instance->machineType[0] != '\0')
    jasprintf(&s, "  \"machineType\": \"%s\",", instance->machineType);

  jasprintf(&s, "  \"status\": \"%s\",",
            InstanceStatus_to_str(instance->status));

  if (instance->zone != NULL && instance->zone[0] != '\0')
    jasprintf(&s, "  \"zone\": \"%s\",", instance->zone);

  /*if (instance->networkInterfaces != NULL && instance->networkInterfaces[0] !=
    '\0') jasprintf(&s, "  \"networkInterfaces\": \"%s\",",
    instance->networkInterfaces);*/

  /*if (instance->disks != NULL && instance->disks[0] != '\0')
    jasprintf(&s, "  \"disks\": \"%s\",", instance->disks);

  if (instance->metadata != NULL && instance->metadata[0] != '\0')
    jasprintf(&s, "  \"metadata\": \"%s\",", instance->metadata);*/

  if (instance->selfLink != NULL && instance->selfLink[0] != '\0')
    jasprintf(&s, "  \"selfLink\": \"%s\",", instance->selfLink);

  /*if (instance->scheduling != NULL && instance->scheduling[0] != '\0')
    jasprintf(&s, "  \"scheduling\": \"%s\",", instance->scheduling);*/

  if (instance->cpuPlatform != NULL && instance->cpuPlatform[0] != '\0')
    jasprintf(&s, "  \"cpuPlatform\": \"%s\",", instance->cpuPlatform);

  if (instance->labelFingerprint != NULL &&
      instance->labelFingerprint[0] != '\0')
    jasprintf(&s, "  \"labelFingerprint\": \"%s\",",
              instance->labelFingerprint);

  /*if (instance->startRestricted != NULL && instance->startRestricted[0] !=
    '\0') jasprintf(&s, "  \"startRestricted\": \"%s\",",
    instance->startRestricted);*/

  /*if (instance->deletionProtection != NULL && instance->deletionProtection[0]
    != '\0') jasprintf(&s, "  \"deletionProtection\": \"%s\",",
    instance->deletionProtection);*/

  /*if (instance->shieldedInstanceConfig != NULL &&
      instance->shieldedInstanceConfig[0] != '\0')
    jasprintf(&s, "  \"shieldedInstanceConfig\": \"%s\",",
              instance->shieldedInstanceConfig);*/

  /*if (instance->shieldedInstanceIntegrityPolicy != NULL &&
      instance->shieldedInstanceIntegrityPolicy[0] != '\0')
    jasprintf(&s, "  \"shieldedInstanceIntegrityPolicy\": \"%s\",",
              instance->shieldedInstanceIntegrityPolicy);*/

  if (instance->fingerprint != NULL && instance->fingerprint[0] != '\0')
    jasprintf(&s, "  \"fingerprint\": \"%s\",", instance->fingerprint);

  jasprintf(&s, "\0");
  s[strlen(s) - 1] = '}';
  return s;
}

/* Utility functions */

struct Tags *Tags_from_json(const JSON_Object *const jsonObject) {
  struct Tags *tags = malloc(sizeof *tags);
  if (json_object_has_value_of_type(jsonObject, "fingerprint", JSONString))
    tags->fingerprint = json_object_get_string(jsonObject, "fingerprint");
  return tags;
}

struct GuestOsFeatures **
GuestOsFeatures_arr_from_json(const JSON_Array *const json_items) {
  struct GuestOsFeatures **guest_os_features_arr = NULL;
  const size_t json_items_n = json_array_get_count(json_items) + 1;
  size_t i;

  if (json_items_n > 1) {
    guest_os_features_arr =
        malloc(json_items_n * sizeof **guest_os_features_arr);
    for (i = 0; i < json_items_n - 1; i++)
      guest_os_features_arr[i] =
          GuestOsFeatures_from_json(json_array_get_object(json_items, i));
    guest_os_features_arr[i] = NULL;
  }
  return guest_os_features_arr;
}

struct GuestOsFeatures *
GuestOsFeatures_from_json(const JSON_Object *const jsonObject) {
  struct GuestOsFeatures *guest_os_features = malloc(sizeof *guest_os_features);
  if (json_object_has_value_of_type(jsonObject, "type", JSONString)) {
    guest_os_features = malloc(sizeof *guest_os_features);
    guest_os_features->type = json_object_get_string(jsonObject, "type");
  }
  return guest_os_features;
}

struct Disk **Disks_from_json(const JSON_Array *const json_items) {
  struct Disk **disks = NULL;
  const size_t json_items_n = json_array_get_count(json_items) + 1;
  size_t i;

  if (json_items_n > 1) {
    disks = malloc(json_items_n * sizeof **disks);
    for (i = 0; i < json_items_n - 1; i++)
      disks[i] = Disk_from_json(json_array_get_object(json_items, i));
    disks[i] = NULL;
  }
  return disks;
}

struct Disk *Disk_from_json(const JSON_Object *const jsonObject) {
  struct Disk *disk = malloc(sizeof *disk);
  disk->kind = json_object_get_string(jsonObject, "kind");
  disk->type = json_object_get_string(jsonObject, "type");
  disk->mode = json_object_get_string(jsonObject, "mode");
  disk->source = json_object_get_string(jsonObject, "source");
  disk->deviceName = json_object_get_string(jsonObject, "deviceName");
  if (json_object_has_value_of_type(jsonObject, "index", JSONNumber))
    disk->index = (int)json_object_get_number(jsonObject, "index");
  if (json_object_has_value_of_type(jsonObject, "boot", JSONBoolean))
    disk->boot = (bool)json_object_get_boolean(jsonObject, "boot");
  if (json_object_has_value_of_type(jsonObject, "autoDelete", JSONBoolean))
    disk->autoDelete = (bool)json_object_get_boolean(jsonObject, "autoDelete");
  if (json_object_has_value_of_type(jsonObject, "licenses", JSONArray))
    disk->licenses =
        json_array_to_cstr_array(json_object_get_array(jsonObject, "licenses"));
  disk->interface = json_object_get_string(jsonObject, "interface");
  if (json_object_has_value_of_type(jsonObject, "guestOsFeatures", JSONArray))
    disk->guestOsFeatures = GuestOsFeatures_arr_from_json(
        json_object_get_array(jsonObject, "guestOsFeatures"));
  disk->diskSizeGb = json_object_get_string(jsonObject, "diskSizeGb");
  return disk;
}

struct NetworkInterface *
NetworkInterface_from_json(const JSON_Object *const jsonObject) {
  struct NetworkInterface *networkInterface = malloc(sizeof *networkInterface);
  networkInterface->kind = json_object_get_string(jsonObject, "kind");
  networkInterface->network = json_object_get_string(jsonObject, "network");
  networkInterface->subnetwork =
      json_object_get_string(jsonObject, "subnetwork");
  networkInterface->networkIP = json_object_get_string(jsonObject, "networkIP");
  networkInterface->name = json_object_get_string(jsonObject, "name");
  networkInterface->fingerprint =
      json_object_get_string(jsonObject, "fingerprint");
  networkInterface->stackType = json_object_get_string(jsonObject, "stackType");
  if (json_object_has_value_of_type(jsonObject, "accessConfigs", JSONArray)) {
    const JSON_Array *const json_items =
        json_object_get_array(jsonObject, "accessConfigs");
    const size_t json_items_n = json_array_get_count(json_items) + 1;
    size_t i;

    if (json_items_n > 1) {
      networkInterface->accessConfigs =
          malloc(json_items_n * sizeof networkInterface->accessConfigs);
      for (i = 0; i < json_items_n - 1; i++)
        networkInterface->accessConfigs[i] =
            AccessConfigs_from_json(json_array_get_object(json_items, i));
      networkInterface->accessConfigs[i] = NULL;
    }
  }
  return networkInterface;
}

const char *NetworkInterface_to_json(
    const struct NetworkInterface *const networkInterface) {
  char *s = NULL;
  jasprintf(&s,
            "    {\n"
            "      \"kind\": \"%s\",\n"
            "      \"network\": \"%s\",\n"
            "      \"subnetwork\": \"%s\",\n"
            "      \"networkIP\": \"%s\",\n"
            "      \"name\": \"%s\",\n"
            "      \"accessConfigs\": [\n",
            networkInterface->kind, networkInterface->network,
            networkInterface->subnetwork, networkInterface->networkIP,
            networkInterface->name);
  {
    struct AccessConfigs **accessConfig;
    for (accessConfig = networkInterface->accessConfigs; *accessConfig;
         accessConfig++)
      jasprintf(&s, AccessConfigs_to_json(*accessConfig));
    jasprintf(&s, "      ],\n");
  }
  jasprintf(&s,
            "      \"fingerprint\": \"%s\",\n"
            "      \"stackType\": \"%s\"\n"
            "    }\n",
            networkInterface->fingerprint, networkInterface->stackType);
  return s;
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

const char *
AccessConfigs_to_json(const struct AccessConfigs *const accessConfigs) {
  char *s;
  asprintf(&s,
           "        {\n"
           "          \"kind\": \"%s\",\n"
           "          \"type\": \"%s\",\n"
           "          \"name\": \"%s\",\n"
           "          \"natIP\": \"%s\",\n"
           "          \"networkTier\": \"%s\"\n"
           "        }\n",
           accessConfigs->kind, accessConfigs->type, accessConfigs->name,
           accessConfigs->natIP, accessConfigs->networkTier);
  return s;
}

struct Item *Item_from_json(const JSON_Object *const jsonObject) {
  struct Item *item = malloc(sizeof *item);
  item->key = json_object_get_string(jsonObject, "key");
  item->value = json_object_get_string(jsonObject, "value");
  return item;
}

struct Item **items_from_json(const JSON_Array *const json_items) {
  struct Item **items = NULL;
  const size_t json_items_n = json_array_get_count(json_items) + 1;
  size_t i;

  if (json_items_n > 1) {
    items = malloc(json_items_n * sizeof **items);
    for (i = 0; i < json_items_n - 1; i++)
      items[i] = Item_from_json(json_array_get_object(json_items, i));
    items[i] = NULL;
  }
  return items;
}

struct Metadata *Metadata_from_json(const JSON_Object *const jsonObject) {
  struct Metadata *metadata = malloc(sizeof *metadata);
  metadata->kind = json_object_get_string(jsonObject, "kind");
  metadata->fingerprint = json_object_get_string(jsonObject, "fingerprint");
  if (json_object_has_value_of_type(jsonObject, "items", JSONArray))
    metadata->items =
        items_from_json(json_object_get_array(jsonObject, "items"));
  return metadata;
}

struct Scheduling *Scheduling_from_json(const JSON_Object *const jsonObject) {
  struct Scheduling *scheduling = malloc(sizeof *scheduling);
  scheduling->onHostMaintenance =
      json_object_get_string(jsonObject, "onHostMaintenance");
  if (json_object_has_value_of_type(jsonObject, "automaticRestart",
                                    JSONBoolean))
    scheduling->automaticRestart =
        json_object_get_boolean(jsonObject, "automaticRestart");
  if (json_object_has_value_of_type(jsonObject, "preemptible", JSONBoolean))
    scheduling->preemptible =
        json_object_get_boolean(jsonObject, "preemptible");
  scheduling->provisioningModel =
      json_object_get_string(jsonObject, "provisioningModel");
  return scheduling;
}

struct ShieldedInstanceConfig *
ShieldedInstanceConfig_from_json(const JSON_Object *const jsonObject) {
  struct ShieldedInstanceConfig *shielded_instance_config =
      malloc(sizeof *shielded_instance_config);
  if (json_object_has_value_of_type(jsonObject, "enableSecureBoot",
                                    JSONBoolean))
    shielded_instance_config->enableSecureBoot =
        json_object_get_boolean(jsonObject, "enableSecureBoot");
  if (json_object_has_value_of_type(jsonObject, "enableVtpm", JSONBoolean))
    shielded_instance_config->enableVtpm =
        json_object_get_boolean(jsonObject, "enableVtpm");
  if (json_object_has_value_of_type(jsonObject, "enableIntegrityMonitoring",
                                    JSONBoolean))
    shielded_instance_config->enableIntegrityMonitoring =
        json_object_get_boolean(jsonObject, "enableIntegrityMonitoring");
  return shielded_instance_config;
}

struct ShieldedInstanceIntegrityPolicy *
ShieldedInstanceIntegrityPolicy_from_json(const JSON_Object *const jsonObject) {
  struct ShieldedInstanceIntegrityPolicy *shielded_instance_integrity_policy =
      NULL;
  if (json_object_has_value_of_type(jsonObject, "updateAutoLearnPolicy",
                                    JSONBoolean)) {
    shielded_instance_integrity_policy =
        malloc(sizeof *shielded_instance_integrity_policy);
    shielded_instance_integrity_policy->updateAutoLearnPolicy =
        (bool)json_object_get_boolean(jsonObject, "updateAutoLearnPolicy");
  }
  return shielded_instance_integrity_policy;
}

struct Instance *instance_from_json(const JSON_Object *const jsonObject) {
  struct Instance *instance = malloc(sizeof *instance);

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

  if (json_object_has_value_of_type(jsonObject, "kind", JSONString))
    instance->kind = json_object_get_string(jsonObject, "kind");

  if (json_object_has_value_of_type(jsonObject, "id", JSONString))
    instance->id = json_object_get_string(jsonObject, "id");

  if (json_object_has_value_of_type(jsonObject, "creationTimestamp",
                                    JSONString))
    instance->creationTimestamp =
        json_object_get_string(jsonObject, "creationTimestamp");

  if (json_object_has_value_of_type(jsonObject, "name", JSONString))
    instance->name = json_object_get_string(jsonObject, "name");

  if (json_object_has_value_of_type(jsonObject, "tags", JSONObject))
    instance->tags = Tags_from_json(json_object_get_object(jsonObject, "tags"));

  if (json_object_has_value_of_type(jsonObject, "machineType", JSONString))
    instance->machineType = json_object_get_string(jsonObject, "machineType");

  if (json_object_has_value_of_type(jsonObject, "status", JSONString))
    instance->status =
        str_to_InstanceStatus(json_object_get_string(jsonObject, "status"));

  if (json_object_has_value_of_type(jsonObject, "zone", JSONString))
    instance->zone = json_object_get_string(jsonObject, "zone");

  if (json_object_has_value_of_type(jsonObject, "networkInterfaces",
                                    JSONArray)) {
    const JSON_Array *const json_items =
        json_object_get_array(jsonObject, "networkInterfaces");
    const size_t json_items_n = json_array_get_count(json_items) + 1;
    size_t i;

    if (json_items_n > 1) {
      instance->networkInterfaces =
          malloc(json_items_n * sizeof instance->networkInterfaces);
      for (i = 0; i < json_items_n - 1; i++)
        instance->networkInterfaces[i] =
            NetworkInterface_from_json(json_array_get_object(json_items, i));
      instance->networkInterfaces[i] = NULL;
    }
  }

  if (json_object_has_value_of_type(jsonObject, "disks", JSONArray))
    instance->disks =
        Disks_from_json(json_object_get_array(jsonObject, "disks"));

  if (json_object_has_value_of_type(jsonObject, "metadata", JSONObject))
    instance->metadata =
        Metadata_from_json(json_object_get_object(jsonObject, "metadata"));

  if (json_object_has_value_of_type(jsonObject, "selfLink", JSONString))
    instance->selfLink = json_object_get_string(jsonObject, "selfLink");

  if (json_object_has_value_of_type(jsonObject, "scheduling", JSONObject))
    instance->scheduling =
        Scheduling_from_json(json_object_get_object(jsonObject, "scheduling"));

  if (json_object_has_value_of_type(jsonObject, "cpuPlatform", JSONString))
    instance->cpuPlatform = json_object_get_string(jsonObject, "cpuPlatform");

  if (json_object_has_value_of_type(jsonObject, "labelFingerprint", JSONString))
    instance->labelFingerprint =
        json_object_get_string(jsonObject, "labelFingerprint");

  if (json_object_has_value_of_type(jsonObject, "startRestricted", JSONBoolean))
    instance->startRestricted =
        (bool)json_object_get_boolean(jsonObject, "startRestricted");

  if (json_object_has_value_of_type(jsonObject, "deletionProtection",
                                    JSONBoolean))
    instance->deletionProtection =
        (bool)json_object_get_boolean(jsonObject, "deletionProtection");

  if (json_object_has_value_of_type(jsonObject, "shieldedInstanceConfig",
                                    JSONObject))
    instance->shieldedInstanceConfig = ShieldedInstanceConfig_from_json(
        json_object_get_object(jsonObject, "shieldedInstanceConfig"));

  if (json_object_has_value_of_type(
          jsonObject, "shieldedInstanceIntegrityPolicy", JSONObject))
    instance->shieldedInstanceIntegrityPolicy =
        ShieldedInstanceIntegrityPolicy_from_json(json_object_get_object(
            jsonObject, "shieldedInstanceIntegrityPolicy"));

  if (json_object_has_value_of_type(jsonObject, "fingerprint", JSONString))
    instance->fingerprint = json_object_get_string(jsonObject, "fingerprint");

  if (json_object_has_value_of_type(jsonObject, "lastStartTimestamp",
                                    JSONString))
    instance->lastStartTimestamp =
        json_object_get_string(jsonObject, "lastStartTimestamp");

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

const char *InstanceStatus_to_str(const enum InstanceStatus status) {
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
