#include <stddef.h>

#include "instance_mocks.h"

#define ACCESS_CONFIG_MOCK0                                                    \
  "        {\n"                                                                \
  "          \"kind\": \"compute#accessConfig\",\n"                            \
  "          \"type\": \"ONE_TO_ONE_NAT\",\n"                                  \
  "          \"name\": \"external-nat\",\n"                                    \
  "          \"natIP\": \"34.0.0.1\",\n"                                       \
  "          \"networkTier\": \"PREMIUM\"\n"                                   \
  "        }\n"

const char access_config_mock0[] = ACCESS_CONFIG_MOCK0;

#define NETWORK_INTERFACE_MOCK0                                                \
  "    {\n"                                                                    \
  "      \"kind\": \"compute#networkInterface\",\n"                            \
  "      \"network\": "                                                        \
  "\"https://www.googleapis.com/compute/v1/projects/myprojectid/"              \
  "global/networks/myprojectid-bucket-vm0-net\",\n"                            \
  "      \"subnetwork\": "                                                     \
  "\"https://www.googleapis.com/cosmpute/v1/projects/myprojectid/"             \
  "regions/australia-southeast2/subnetworks/"                                  \
  "myprojectid-bucket-vm0-net\",\n"                                            \
  "      \"networkIP\": \"10.0.0.1\",\n"                                       \
  "      \"name\": \"nic0\",\n"                                                \
  "      \"accessConfigs\": [\n" ACCESS_CONFIG_MOCK0 "      ],\n"              \
  "      \"fingerprint\": \"ufVDhkGBA88=\",\n"                                 \
  "      \"stackType\": \"IPV4_ONLY\"\n"                                       \
  "    }\n"

const char network_interface_mock0[] = NETWORK_INTERFACE_MOCK0;

#define DISK_MOCK0                                                             \
  "    {\n"                                                                    \
  "      \"kind\": \"compute#attachedDisk\",\n"                                \
  "      \"type\": \"PERSISTENT\",\n"                                          \
  "      \"mode\": \"READ_WRITE\",\n"                                          \
  "      \"source\": "                                                         \
  "\"https://www.googleapis.com/compute/v1/projects/myprojectid/"              \
  "zones/australia-southeast2-a/disks/"                                        \
  "myprojectid-bucket-vm0\",\n"                                                \
  "      \"deviceName\": \"persistent-disk-0\",\n"                             \
  "      \"index\": 0,\n"                                                      \
  "      \"boot\": true,\n"                                                    \
  "      \"autoDelete\": true,\n"                                              \
  "      \"licenses\": [\n"                                                    \
  "        "                                                                   \
  "\"https://www.googleapis.com/compute/v1/projects/debian-cloud/global/"      \
  "licenses/debian-10-buster\"\n"                                              \
  "      ],\n"                                                                 \
  "      \"interface\": \"SCSI\",\n"                                           \
  "      \"guestOsFeatures\": [\n"                                             \
  "        {\n"                                                                \
  "          \"type\": \"UEFI_COMPATIBLE\"\n"                                  \
  "        },\n"                                                               \
  "        {\n"                                                                \
  "          \"type\": \"VIRTIO_SCSI_MULTIQUEUE\"\n"                           \
  "        }\n"                                                                \
  "      ],\n"                                                                 \
  "      \"diskSizeGb\": \"10\"\n"                                             \
  "    }\n"

const char disk_mock0[] = DISK_MOCK0;

#define METADATA_MOCK_ITEM0                                                    \
  "      {\n"                                                                  \
  "        \"key\": \"startup-script\",\n"                                     \
  "        \"value\": \"#!/bin/id\""                                           \
  "      }\n"

#define METADATA_MOCK0                                                         \
  "{\n"                                                                        \
  "    \"kind\": \"compute#metadata\",\n"                                      \
  "    \"fingerprint\": \"dPeprQ1djYA=\",\n"                                   \
  "    \"items\": [\n" METADATA_MOCK_ITEM0 "    ]\n"                           \
  "  }"

#define SCHEDULING_MOCK0                                                       \
  "{\n"                                                                        \
  "    \"onHostMaintenance\": \"MIGRATE\",\n"                                  \
  "    \"automaticRestart\": true,\n"                                          \
  "    \"preemptible\": false,\n"                                              \
  "    \"provisioningModel\": \"STANDARD\"\n"                                  \
  "  }"

#define SHIELDED_INSTANCE_CONFIG_MOCK0                                         \
  "{\n"                                                                        \
  "    \"enableSecureBoot\": false,\n"                                         \
  "    \"enableVtpm\": true,\n"                                                \
  "    \"enableIntegrityMonitoring\": true\n"                                  \
  "  }"

#define SHIELDED_INSTANCE_INTEGRITY_POLICY_MOCK0                               \
  "{\n"                                                                        \
  "    \"updateAutoLearnPolicy\": true\n"                                      \
  "  }"

#define TAGS_MOCK0                                                             \
  "{\n"                                                                        \
  "    \"fingerprint\": \"42WmSpB8rSM=\"\n"                                    \
  "  }"

#define INSTANCE_MOCK0                                                         \
  "{\n"                                                                        \
  "  \"kind\": \"compute#instance\",\n"                                        \
  "  \"id\": \"828639566970267330\",\n"                                        \
  "  \"creationTimestamp\": \"2022-06-23T18:49:33.789-07:00\",\n"              \
  "  \"name\": \"myprojectid-bucket-vm0\",\n"                                  \
  "  \"tags\": " TAGS_MOCK0 ",\n"                                              \
  "  \"machineType\": "                                                        \
  "\"https://www.googleapis.com/compute/v1/projects/myprojectid/"              \
  "zones/australia-southeast2-a/machineTypes/n1-standard-1\",\n"               \
  "  \"status\": \"RUNNING\",\n"                                               \
  "  \"zone\": "                                                               \
  "\"https://www.googleapis.com/compute/v1/projects/myprojectid/"              \
  "zones/australia-southeast2-a\",\n"                                          \
  "  \"networkInterfaces\": [\n" NETWORK_INTERFACE_MOCK0 "  ],\n"              \
  "  \"disks\": [\n" DISK_MOCK0 "  ],\n"                                       \
  "  \"metadata\": " METADATA_MOCK0 ",\n"                                      \
  "  \"selfLink\": "                                                           \
  "\"https://www.googleapis.com/compute/v1/projects/myprojectid/"              \
  "zones/australia-southeast2-a/instances/"                                    \
  "myprojectid-bucket-vm0\",\n"                                                \
  "  \"scheduling\": " SCHEDULING_MOCK0 ",\n"                                  \
  "  \"cpuPlatform\": \"Intel Broadwell\",\n"                                  \
  "  \"labelFingerprint\": \"42WmSpB8rSM=\",\n"                                \
  "  \"startRestricted\": false,\n"                                            \
  "  \"deletionProtection\": false,\n"                                         \
  "  \"shieldedInstanceConfig\": " SHIELDED_INSTANCE_CONFIG_MOCK0 ",\n"        \
  "  "                                                                         \
  "\"shieldedInstanceIntegrityPolicy\":"                                       \
  " " SHIELDED_INSTANCE_INTEGRITY_POLICY_MOCK0 ",\n"                           \
  "  \"fingerprint\": \"iPWd2IQdU6U=\",\n"                                     \
  "  \"lastStartTimestamp\": \"2022-06-23T18:49:40.998-07:00\"\n"              \
  "}"                                                                          \
  "}"

const char instance_mock0[] = INSTANCE_MOCK0;
