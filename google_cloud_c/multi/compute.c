#include <assert.h>
#include <google_cloud_c/common/google_cloud_c_types_common.h>
#include <google_cloud_c/compute/cloud_auth.h>
#include <google_cloud_c/compute/instance.h>
#include <google_cloud_c/compute/zones.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#define strdup _strdup
#endif

struct StatusAndCstrAndCStr
create_fw_net_instance(const struct configuration *const config,
                       const struct InstanceIncomplete *const instance,
                       const char *shell_script) {
  puts("create_fw_net_instance");
  set_auth_context(config->google_project_id, config->google_access_token);
  INSTANCE_CONTEXT.zone = config->google_zone;
  const struct OptionalInstance optionalInstance =
      instance_incomplete_create_all(
          instance,
          /*network_name*/ NULL,
          /*firewall_name*/ NULL,
          /*shell_script*/ shell_script); /* instance_create_all */

  if (!optionalInstance.set) {
    const struct StatusAndCstrAndCStr instance_name_ip = {EXIT_FAILURE, NULL, 0,
                                                          NULL, 0};
    return instance_name_ip;
  }

  const struct Instance _instance = optionalInstance.instance;

  assert(strlen(_instance.networkInterfaces[0].accessConfigs[0].natIP) > 0);

  printf("{\n"
         "  \"instance_name\": \"%s\",\n"
         "  \"natIP\": \"%s\",\n"
         "  \"create_fw_net_instance\": true\n"
         "}",
         _instance.name, _instance.networkInterfaces[0].accessConfigs[0].natIP);

  if (strlen(_instance.name) == 0)
    return StatusAndCstrAndCStrNull;

  const struct StatusAndCstrAndCStr instance_name_ip = {
      EXIT_SUCCESS, _instance.name, (ssize_t)strlen(_instance.name),
      _instance.networkInterfaces[0].accessConfigs[0].natIP,
      (ssize_t)strlen(_instance.networkInterfaces[0].accessConfigs[0].natIP)};
  return instance_name_ip;
}

struct StatusAndCstr get_instance_ip(const struct configuration *const config,
                                     const char *instance_name) {
  AUTH_CONTEXT.project_id = config->google_project_id,
  AUTH_CONTEXT.google_access_token = config->google_access_token;
  INSTANCE_CONTEXT.zone = config->google_zone;
  const struct OptionalInstance optionalInstance = instance_get(instance_name);
  if (!optionalInstance.set) {
    const struct StatusAndCstr instance_ip = {EXIT_FAILURE, NULL, 0};
    return instance_ip;
  }

  const struct Instance _instance = optionalInstance.instance;

  if (strlen(_instance.name) == 0)
    return StatusAndCstrNull;

  assert(strlen(_instance.networkInterfaces[0].accessConfigs[0].natIP) > 0);
  printf("{\n"
         "  \"instance_name\": \"%s\",\n"
         "  \"natIP\": \"%s\",\n"
         "  \"get_instance_ip\": true\n"
         "}\n",
         _instance.name, _instance.networkInterfaces[0].accessConfigs[0].natIP);

  const struct StatusAndCstr instance_ip = {
      EXIT_SUCCESS,
      _instance.networkInterfaces[0].accessConfigs[0].natIP,
      (ssize_t)strlen(_instance.networkInterfaces[0].accessConfigs[0].natIP),
  };
  return instance_ip;
}

struct StatusAndArrayCStrArray compute(const struct configuration *const config,
                                       const char *kind, bool list_only) {
  AUTH_CONTEXT.project_id = config->google_project_id,
  AUTH_CONTEXT.google_access_token = config->google_access_token;
  INSTANCE_CONTEXT.zone = config->google_zone;

  if (list_only) {
    if (strcmp(kind, "instances") == 0) {
      const struct Instances _instances = instances_list();
      char **_instanceNames;
      size_t i;

      _instanceNames = (char **)malloc(_instances.size * sizeof(char *));
      for (i = 0; i < _instances.size; ++i)
        _instanceNames[i] = strdup(_instances.arr[i].name);

      const struct StatusAndArrayCStrArray instanceNames = {
          EXIT_SUCCESS, _instanceNames, _instances.size};
      return instanceNames;
    } else if (strcmp(kind, "zones") == 0) {
      const struct Zones _zones = zone_list();
      char **_zoneNames;
      size_t i;

      _zoneNames = (char **)malloc(_zones.size * sizeof(char *));
      for (i = 0; i < _zones.size; ++i)
        _zoneNames[i] = strdup(_zones.arr[i].name);

      const struct StatusAndArrayCStrArray zoneNames = {
          EXIT_SUCCESS, _zoneNames, _zones.size};
      return zoneNames;
    }
  } else {
    fputs("TODO: Implement", stderr);
    exit(EXIT_FAILURE);
  }

  return StatusAndArrayCStrArrayNull;
}

extern struct StatusAndCstr
get_notification_configuration(const struct configuration *, const char *);
