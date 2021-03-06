#include <assert.h>

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/common/google_cloud_c_types_common.h>
#include <google_cloud_c/compute/instance.h>
#include <google_cloud_c/compute/zones.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#define strdup _strdup
#endif /* defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) */

struct StatusAndCstrAndCStr
create_fw_net_instance(const struct configuration *const config,
                       const struct InstanceIncomplete *const instance,
                       const char *const shell_script) {
  puts("create_fw_net_instance");
  set_auth_context(config->google_project_id, config->google_access_token);
  INSTANCE_CONTEXT.zone = config->google_zone;
  {
    const struct Instance *const optionalInstance =
        instance_incomplete_create_all(
            instance,
            /*network_name*/ NULL,
            /*firewall_name*/ NULL,
            /*shell_script*/ shell_script); /* instance_create_all */
    struct StatusAndCstrAndCStr instance_name_ip = StatusAndCstrAndCStrNull;

    if (optionalInstance != NULL) {
      assert(
          optionalInstance->networkInterfaces[0]->accessConfigs[0]->natIP[0] !=
          '\0');

      printf(
          "{\n"
          "  \"instance_name\": \"%s\",\n"
          "  \"natIP\": \"%s\",\n"
          "  \"create_fw_net_instance\": true\n"
          "}",
          optionalInstance->name,
          optionalInstance->networkInterfaces[0]->accessConfigs[0]->natIP[0] !=
                  '\0'
              ? optionalInstance->networkInterfaces[0]->accessConfigs[0]->natIP
              : "(null)");

      if (optionalInstance->name != NULL && optionalInstance->name[0] != '\0') {
        instance_name_ip.status = EXIT_SUCCESS;
        instance_name_ip.c_str0 = optionalInstance->name;
        instance_name_ip.size0 = (ssize_t)strlen(optionalInstance->name);
        instance_name_ip.c_str1 =
            optionalInstance->networkInterfaces[0]->accessConfigs[0]->natIP;
        instance_name_ip.size1 = (ssize_t)strlen(
            optionalInstance->networkInterfaces[0]->accessConfigs[0]->natIP);
      }
    }
    return instance_name_ip;
  }
}

struct StatusAndCstr get_instance_ip(const struct configuration *const config,
                                     const char *const instance_name) {
  AUTH_CONTEXT.project_id = config->google_project_id,
  AUTH_CONTEXT.google_access_token = config->google_access_token;
  INSTANCE_CONTEXT.zone = config->google_zone;
  {
    const struct Instance *const optionalInstance = instance_get(instance_name);
    if (optionalInstance == NULL) {
      const struct StatusAndCstr instance_ip = {EXIT_FAILURE, NULL, 0};
      return instance_ip;
    }

    {
      assert(
          optionalInstance->networkInterfaces[0]->accessConfigs[0]->natIP[0] !=
          '\0');
      printf(
          "{\n"
          "  \"instance_name\": \"%s\",\n"
          "  \"natIP\": \"%s\",\n"
          "  \"get_instance_ip\": true\n"
          "}\n",
          optionalInstance->name,
          optionalInstance->networkInterfaces[0]->accessConfigs[0]->natIP[0] !=
                  '\0'
              ? optionalInstance->networkInterfaces[0]->accessConfigs[0]->natIP
              : "(null)");

      {
        struct StatusAndCstr instance_ip = {
            EXIT_SUCCESS,
            NULL,
            0,
        };
        instance_ip.c_str =
            optionalInstance->networkInterfaces[0]->accessConfigs[0]->natIP;
        instance_ip.size = (ssize_t)strlen(
            optionalInstance->networkInterfaces[0]->accessConfigs[0]->natIP);
        return instance_ip;
      }
    }
  }
}

struct StatusAndArrayCStrArray compute(const struct configuration *const config,
                                       const char *const kind, bool list_only) {
  struct StatusAndArrayCStrArray zoneNames = StatusAndArrayCStrArrayNull;

  AUTH_CONTEXT.project_id = config->google_project_id,
  AUTH_CONTEXT.google_access_token = config->google_access_token;
  INSTANCE_CONTEXT.zone = config->google_zone;

  if (list_only) {
    if (strcmp(kind, "instances") == 0) {
      const struct Instances _instances = instances_list();
      struct StatusAndArrayCStrArray instanceNames = {EXIT_SUCCESS, NULL, 0};
      char **_instanceNames;
      size_t i;

      if (_instances.size > 0) {
        _instanceNames = malloc(_instances.size * sizeof **_instanceNames);
        for (i = 0; i < _instances.size; ++i)
          _instanceNames[i] = strdup(_instances.arr[i]->name);
        instanceNames.c_str_arr = _instanceNames;
        instanceNames.size = _instances.size;
      }

      return instanceNames;
    } else if (strcmp(kind, "zones") == 0) {
      const struct Zones _zones = zone_list();
      char **_zoneNames = NULL;
      size_t i;
      if (_zones.size > 0) {
        _zoneNames = malloc(_zones.size * sizeof **_zoneNames);
        for (i = 0; i < _zones.size; ++i)
          _zoneNames[i] = strdup(_zones.arr[i]->name);

        zoneNames.status = EXIT_SUCCESS;
        zoneNames.c_str_arr = _zoneNames;
        zoneNames.size = _zones.size;
      } else
        fputs("No Zones found", stderr);
    }
  } else {
    fputs("TODO: Implement", stderr);
    exit(EXIT_FAILURE);
  }

  return zoneNames;
}

extern struct StatusAndCstr
get_notification_configuration(const struct configuration *, const char *);
