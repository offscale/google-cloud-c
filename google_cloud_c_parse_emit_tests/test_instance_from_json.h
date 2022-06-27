#ifdef __cplusplus
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __cplusplus */

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#define strdup _strdup
#endif /* defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) */

#include <greatest.h>

#include <c89stringutils_string_extras.h>

#include <google_cloud_c_configure.h>

#include <google_cloud_c/compute/instance.h>

#include "instance_mocks.h"

TEST x_access_config_from_json(void) {
  JSON_Value *const access_config_val = json_parse_string(access_config_mock0);
  const JSON_Object *const access_config_json =
      json_value_get_object(access_config_val);
  const struct AccessConfigs *const access_config =
      AccessConfigs_from_json(access_config_json);

  ASSERT_STR_EQ(access_config->kind, "compute#accessConfig");
  ASSERT_STR_EQ(access_config->type, "ONE_TO_ONE_NAT");
  ASSERT_STR_EQ(access_config->name, "external-nat");
  ASSERT_STR_EQ(access_config->natIP, "34.0.0.1");
  ASSERT_STR_EQ(access_config->networkTier, "PREMIUM");

  json_value_free(access_config_val);

  PASS();
}

TEST x_network_interface_from_json(void) {
  JSON_Value *const network_interface_val =
      json_parse_string(network_interface_mock0);
  const JSON_Object *const network_interface_json =
      json_value_get_object(network_interface_val);
  const struct NetworkInterface *const network_interface =
      NetworkInterface_from_json(network_interface_json);

  ASSERT_STR_EQ(network_interface->kind, "compute#networkInterface");
  ASSERT_STR_EQ(network_interface->network,
                "https://www.googleapis.com/compute/v1/projects/myprojectid/"
                "global/networks/myprojectid-bucket-vm0-net");
  ASSERT_STR_EQ(
      network_interface->subnetwork,
      "https://www.googleapis.com/cosmpute/v1/projects/myprojectid/regions/"
      "australia-southeast2/subnetworks/myprojectid-bucket-vm0-net");
  ASSERT_STR_EQ(network_interface->networkIP, "10.0.0.1");
  ASSERT_STR_EQ(network_interface->name, "nic0");
  ASSERT_STR_EQ(network_interface->fingerprint, "ufVDhkGBA88=");
  ASSERT_STR_EQ(network_interface->stackType, "IPV4_ONLY");

  {
    const struct AccessConfigs *const access_config =
        network_interface->accessConfigs[0];
    ASSERT_STR_EQ(access_config->kind, "compute#accessConfig");
    ASSERT_STR_EQ(access_config->type, "ONE_TO_ONE_NAT");
    ASSERT_STR_EQ(access_config->name, "external-nat");
    ASSERT_STR_EQ(access_config->natIP, "34.0.0.1");
    ASSERT_STR_EQ(access_config->networkTier, "PREMIUM");
  }
  ASSERT_EQ(network_interface->accessConfigs[1], NULL);

  json_value_free(network_interface_val);

  PASS();
}

TEST x_instance_from_json(void) {
  JSON_Value *const instance_val = json_parse_string(instance_mock0);
  const JSON_Object *const instance_json = json_value_get_object(instance_val);
  const struct Instance *const instance = instance_from_json(instance_json);

  ASSERT_STR_EQ(instance->kind, "compute#instance");
  ASSERT_STR_EQ(instance->id, "828639566970267330");
  ASSERT_STR_EQ(instance->creationTimestamp, "2022-06-23T18:49:33.789-07:00");
  ASSERT_STR_EQ(instance->name, "myprojectid-bucket-vm0");
  /*ASSERT_STR_EQ(instance->tags, "{'fingerprint': '42WmSpB8rSM='}");*/
  ASSERT_STR_EQ(instance->machineType,
                "https://www.googleapis.com/compute/v1/projects/myprojectid/"
                "zones/australia-southeast2-a/machineTypes/n1-standard-1");
  ASSERT_STR_EQ(InstanceStatus_to_str(instance->status), "RUNNING");
  ASSERT_EQ(instance->status, INSTANCE_RUNNING);
  ASSERT_STR_EQ(instance->zone,
                "https://www.googleapis.com/compute/v1/projects/myprojectid/"
                "zones/australia-southeast2-a");

  {
    struct NetworkInterface *network_interface = instance->networkInterfaces[0];
    ASSERT_STR_EQ(network_interface->kind, "compute#networkInterface");
    ASSERT_STR_EQ(network_interface->network,
                  "https://www.googleapis.com/compute/v1/projects/myprojectid/"
                  "global/networks/myprojectid-bucket-vm0-net");
    ASSERT_STR_EQ(
        network_interface->subnetwork,
        "https://www.googleapis.com/cosmpute/v1/projects/myprojectid/regions/"
        "australia-southeast2/subnetworks/myprojectid-bucket-vm0-net");
    ASSERT_STR_EQ(network_interface->networkIP, "10.0.0.1");
    ASSERT_STR_EQ(network_interface->name, "nic0");
    ASSERT_STR_EQ(network_interface->fingerprint, "ufVDhkGBA88=");
    ASSERT_STR_EQ(network_interface->stackType, "IPV4_ONLY");

    {
      const struct AccessConfigs *const access_config =
          network_interface->accessConfigs[0];
      ASSERT_STR_EQ(access_config->kind, "compute#accessConfig");
      ASSERT_STR_EQ(access_config->type, "ONE_TO_ONE_NAT");
      ASSERT_STR_EQ(access_config->name, "external-nat");
      ASSERT_STR_EQ(access_config->natIP, "34.0.0.1");
      ASSERT_STR_EQ(access_config->networkTier, "PREMIUM");
    }
    ASSERT_EQ(network_interface->accessConfigs[1], NULL);

    ASSERT_EQ(instance->networkInterfaces[1], NULL);
  }

  /*ASSERT_STR_EQ(instance->disks, "[{'kind': 'compute#attachedDisk', 'type':
   * 'PERSISTENT', 'mode': 'READ_WRITE', 'source':
   * 'https://www.googleapis.com/compute/v1/projects/myprojectid/zones/australia-southeast2-a/disks/myprojectid-bucket-vm0',
   * 'deviceName': 'persistent-disk-0', 'index': 0, 'boot': True, 'autoDelete':
   * True, 'licenses':
   * ['https://www.googleapis.com/compute/v1/projects/debian-cloud/global/licenses/debian-10-buster'],
   * 'interface': 'SCSI', 'guestOsFeatures': [{'type': 'UEFI_COMPATIBLE'},
   * {'type': 'VIRTIO_SCSI_MULTIQUEUE'}], 'diskSizeGb': '10'}]");*/

  /*ASSERT_STR_EQ(instance->metadata, "{'kind': 'compute#metadata',
   * 'fingerprint': 'fKPvkD-3x7E=', 'items': [{'key': 'startup-script', 'value':
   * "#!/usr/bin/id"}]}");*/
  ASSERT_STR_EQ(
      instance->selfLink,
      "https://www.googleapis.com/compute/v1/projects/myprojectid/zones/"
      "australia-southeast2-a/instances/myprojectid-bucket-vm0");
  /*ASSERT_STR_EQ(instance->scheduling, "{'onHostMaintenance': 'MIGRATE',
   * 'automaticRestart': True, 'preemptible': False, 'provisioningModel':
   * 'STANDARD'}");*/
  ASSERT_STR_EQ(instance->cpuPlatform, "Intel Broadwell");
  ASSERT_STR_EQ(instance->labelFingerprint, "42WmSpB8rSM=");
  ASSERT_FALSE(instance->startRestricted);
  ASSERT_FALSE(instance->deletionProtection);
  /*ASSERT_STR_EQ(instance->shieldedInstanceConfig, "{'enableSecureBoot': False,
   * 'enableVtpm': True, 'enableIntegrityMonitoring': True}");*/
  /*ASSERT_STR_EQ(instance->shieldedInstanceIntegrityPolicy,
   * "{'updateAutoLearnPolicy': True}");*/
  ASSERT_STR_EQ(instance->fingerprint, "iPWd2IQdU6U=");

  json_value_free(instance_val);

  PASS();
}

SUITE(instance_from_json_suite) {
  RUN_TEST(x_access_config_from_json);
  RUN_TEST(x_network_interface_from_json);
  RUN_TEST(x_instance_from_json);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
