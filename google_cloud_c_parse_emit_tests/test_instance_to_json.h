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

TEST x_access_config_to_json(void) {
  const struct AccessConfigs access_config = {
      "ONE_TO_ONE_NAT", "external-nat",         "34.0.0.1",
      "PREMIUM",        "compute#accessConfig",
  };

  const char *const access_config_str =
      AccessConfigs_to_json((struct AccessConfigs *)&access_config);

  ASSERT_STR_EQ(access_config_str, access_config_mock0);

  PASS();
}

TEST x_network_interface_to_json(void) {
  const struct AccessConfigs access_config = {
      "ONE_TO_ONE_NAT", "external-nat",         "34.0.0.1",
      "PREMIUM",        "compute#accessConfig",
  };
  struct NetworkInterface network_interface = {
      "https://www.googleapis.com/compute/v1/projects/myprojectid/"
      "global/networks/myprojectid-bucket-vm0-net",
      "https://www.googleapis.com/compute/v1/projects/myprojectid/"
      "regions/australia-southeast2/subnetworks/"
      "myprojectid-bucket-vm0-net",
      "10.0.0.1",
      "nic0",
      "compute#networkInterface",
      "ufVDhkGBA88=",
      "IPV4_ONLY",
      NULL};
  network_interface.accessConfigs =
      malloc(2 * sizeof network_interface.accessConfigs);
  network_interface.accessConfigs[0] = (struct AccessConfigs *)&access_config;
  network_interface.accessConfigs[1] = NULL;
  const char *const network_interface_str =
      NetworkInterface_to_json((struct NetworkInterface *)&network_interface);

  ASSERT_STR_EQ(network_interface_str, network_interface_mock0);

  PASS();
}

TEST x_disk_to_json(void) {
  struct Disk disk;
  ASSERT_STR_EQ(Disk_to_json(&disk), disk_mock0);
  PASS();
}

TEST x_instance_to_json(void) {
  /*JSON_Value *const instance_val = json_parse_string(instance_mock0);
  const JSON_Object *const instance_json = json_value_get_object(instance_val);
  const struct Instance *const instance = instance_to_json(instance_json);

  ASSERT_STR_EQ(instance->kind, "compute#instance");
  ASSERT_STR_EQ(instance->id, "828639566970267330");
  ASSERT_STR_EQ(instance->creationTimestamp, "2022-06-23T18:49:33.789-07:00");
  ASSERT_STR_EQ(instance->name, "myprojectid-bucket-vm0");*/
  /*ASSERT_STR_EQ(instance->tags, "{'fingerprint': '42WmSpB8rSM='}");*/
  /*ASSERT_STR_EQ(instance->machineType,
                "https://www.googleapis.com/compute/v1/projects/myprojectid/"
                "zones/australia-southeast2-a/machineTypes/n1-standard-1");
  ASSERT_STR_EQ(InstanceStatus_to_str(instance->status), "RUNNING");
  ASSERT_EQ(instance->status, INSTANCE_RUNNING);
  ASSERT_STR_EQ(instance->zone,
                "https://www.googleapis.com/compute/v1/projects/myprojectid/"
                "zones/australia-southeast2-a");

  {
    const struct NetworkInterface *const network_interface =
        instance->networkInterfaces[0];
    ASSERT_STR_EQ(network_interface->kind, "compute#networkInterface");
    ASSERT_STR_EQ(network_interface->network,
                  "https://www.googleapis.com/compute/v1/projects/myprojectid/"
                  "global/networks/myprojectid-bucket-vm0-net");
    ASSERT_STR_EQ(
        network_interface->subnetwork,
        "https://www.googleapis.com/compute/v1/projects/myprojectid/regions/"
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

  {
    const struct Disk *const disk = instance->disks[0];
    ASSERT_STR_EQ(disk->kind, "compute#attachedDisk");
    ASSERT_STR_EQ(disk->type, "PERSISTENT");
    ASSERT_STR_EQ(disk->mode, "READ_WRITE");
    ASSERT_STR_EQ(disk->source,
                  "https://www.googleapis.com/compute/v1/projects/myprojectid/"
                  "zones/australia-southeast2-a/disks/myprojectid-bucket-vm0");
    ASSERT_STR_EQ(disk->deviceName, "persistent-disk-0");
    ASSERT_EQ(disk->index, 0);
    ASSERT_FALSE(!disk->boot);
    ASSERT_FALSE(!disk->autoDelete);
    ASSERT_STR_EQ(disk->licenses[0],
                  "https://www.googleapis.com/compute/v1/projects/debian-cloud/"
                  "global/licenses/debian-10-buster");
    ASSERT_EQ(disk->licenses[1], NULL);
    ASSERT_STR_EQ(disk->interface, "SCSI");
    {
      struct GuestOsFeatures **guestOsFeatures;
      const char *guest_os_features_types[] = {"UEFI_COMPATIBLE",
                                               "VIRTIO_SCSI_MULTIQUEUE", NULL};
      const char *const *mock_guest_os_features_type;
      for (mock_guest_os_features_type = guest_os_features_types,
          guestOsFeatures = disk->guestOsFeatures;
           *guest_os_features_types != NULL && *guestOsFeatures != NULL;
           ++mock_guest_os_features_type, ++guestOsFeatures)
        ASSERT_STR_EQ((*guestOsFeatures)->type, *mock_guest_os_features_type);
      ASSERT_EQ(*mock_guest_os_features_type, NULL);
      ASSERT_EQ(*guestOsFeatures, NULL);
    }
    ASSERT_STR_EQ(disk->diskSizeGb, "10");

    ASSERT_EQ(instance->disks[1], NULL);
  }

  {
    const struct Metadata *const metadata = instance->metadata;
    ASSERT_STR_EQ(metadata->kind, "compute#metadata");
    ASSERT_STR_EQ(metadata->fingerprint, "dPeprQ1djYA=");

    ASSERT_STR_EQ(metadata->items[0]->key, "startup-script");
    ASSERT_STR_EQ(metadata->items[0]->value, "#!/bin/id");
    ASSERT_EQ(metadata->items[1], NULL);
  }

  ASSERT_STR_EQ(
      instance->selfLink,
      "https://www.googleapis.com/compute/v1/projects/myprojectid/zones/"
      "australia-southeast2-a/instances/myprojectid-bucket-vm0");

  ASSERT_STR_EQ(instance->scheduling->onHostMaintenance, "MIGRATE");
  ASSERT_FALSE(!instance->scheduling->automaticRestart);
  ASSERT_FALSE(instance->scheduling->preemptible);
  ASSERT_STR_EQ(instance->scheduling->provisioningModel, "STANDARD");

  ASSERT_STR_EQ(instance->cpuPlatform, "Intel Broadwell");
  ASSERT_STR_EQ(instance->labelFingerprint, "42WmSpB8rSM=");
  ASSERT_FALSE(instance->startRestricted);
  ASSERT_FALSE(instance->deletionProtection);

  ASSERT_FALSE(instance->shieldedInstanceConfig->enableSecureBoot);
  ASSERT_FALSE(!instance->shieldedInstanceConfig->enableVtpm);
  ASSERT_FALSE(!instance->shieldedInstanceConfig->enableIntegrityMonitoring);

  ASSERT_FALSE(
      !instance->shieldedInstanceIntegrityPolicy->updateAutoLearnPolicy);

  ASSERT_STR_EQ(instance->fingerprint, "iPWd2IQdU6U=");

  json_value_free(instance_val);*/

  PASS();
}

SUITE(instance_to_json_suite) {
  RUN_TEST(x_access_config_to_json);
  RUN_TEST(x_network_interface_to_json);
  RUN_TEST(x_instance_to_json);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
