#ifndef GOOGLE_CLOUD_C_INSTANCE_H
#define GOOGLE_CLOUD_C_INSTANCE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <google_cloud_c/common/google_cloud_c_types_common.h>
#include <google_cloud_c/compute/firewall.h>
#include <google_cloud_c/compute/google_cloud_c_compute_export.h>
#include <google_cloud_c/compute/network.h>
#include <google_cloud_c/multi/compute.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#include <synchapi.h>

#ifdef interface
#undef interface
#endif /* interface */

#else

#include <errno.h>
#include <unistd.h>

#define ERROR_CALL_NOT_IMPLEMENTED ENOATTR
#endif

/* struct InstanceIncomplete cinstance_to_instance(const struct CInstance *); */

struct AccessConfigs {
  const char *type, *name, *natIP, *networkTier, *kind;
};

struct NetworkInterface {
  const char *network, *subnetwork, *networkIP, *name, *kind, *fingerprint,
      *stackType;
  struct AccessConfigs **accessConfigs;
};

struct GuestOsFeatures {
  const char *type;
};

struct Disk {
  const char *kind;
  const char *type;
  const char *mode;
  const char *source;
  const char *deviceName;
  int index;
  bool boot;
  bool autoDelete;
  const char **licenses;
  const char *interface;
  struct GuestOsFeatures **guestOsFeatures;
  const char *diskSizeGb;
};

struct Item {
  const char *key;
  const char *value;
};

struct Metadata {
  const char *kind;
  const char *fingerprint;
  struct Item **items;
};

struct Scheduling {
  const char *onHostMaintenance;
  bool automaticRestart, preemptible;
  const char *provisioningModel;
};

struct ShieldedInstanceConfig {
  bool enableSecureBoot /*=false*/, enableVtpm /*=true*/,
      enableIntegrityMonitoring /*=true*/;
};

struct ShieldedInstanceIntegrityPolicy {
  bool updateAutoLearnPolicy /*=true*/;
};

enum InstanceStatus {
  PROVISIONING = -1,
  STAGING = 0,
  INSTANCE_RUNNING = RUNNING,
  STOPPING,
  SUSPENDING,
  SUSPENDED,
  REPAIRING,
  TERMINATED
};

struct Tags {
  /* obviously there can be arbitrary properties, so this is more a placeholder
   */
  const char *fingerprint;
};

struct Instance {
  const char *id, *creationTimestamp, *name, *machineType, *zone;
  enum InstanceStatus status;
  struct NetworkInterface **networkInterfaces;
  struct Disk **disks;
  struct Metadata *metadata;
  const char *selfLink;
  struct Scheduling *scheduling;
  const char *cpuPlatform, *labelFingerprint;
  bool startRestricted /*=false*/;
  bool deletionProtection /*=false*/;
  struct ShieldedInstanceConfig *shieldedInstanceConfig;
  struct ShieldedInstanceIntegrityPolicy *shieldedInstanceIntegrityPolicy;
  const char *fingerprint, *lastStartTimestamp, *kind;
  struct Tags *tags;
};

struct Instances {
  struct Instance **arr;
  size_t size;
};

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT const struct Instances instancesNull;

/* =============
 * | INSTANCES |
 * ============= */

struct InstanceContext {
  const char *firewall_name, *network_name, *zone;
};

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct InstanceContext INSTANCE_CONTEXT;

/* Retrieves the list of instances contained within the specified zone.
 * https://cloud.google.com/compute/docs/reference/rest/v1/instances/list
 * */
extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Instances instances_list(void);

/* struct Instance instance_create_all(const struct InstanceIncomplete *);

 * struct Instance instance_create_all(const struct CInstance *); */

/* Returns the specified Instance resource.
 * https://cloud.google.com/compute/docs/reference/rest/v1/instances/get */
extern GOOGLE_CLOUD_C_COMPUTE_EXPORT bool
instance_exists(const char *instance_name);

/* Creates an instance resource in the specified project using the data included
 * in the request.
 * https://cloud.google.com/compute/docs/reference/rest/v1/instances/insert */
extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Instance *
instance_insert(const struct InstanceIncomplete *, const char *, const char *);

/* Returns the specified Instance resource.
 * https://cloud.google.com/compute/docs/reference/rest/v1/instances/get */
extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Instance *
instance_get(const char *instance_name);

/* Utility functions */

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Instance *
instance_from_json(const JSON_Object *);

/* Creates network, firewall, and instance */
extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Instance *
instance_incomplete_create_all(const struct InstanceIncomplete *, const char *,
                               const char *, const char *);

/* Utility functions */

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct NetworkInterface *
NetworkInterface_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct AccessConfigs *
AccessConfigs_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT const char *
AccessConfigs_to_json(const struct AccessConfigs *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Tags *
Tags_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct GuestOsFeatures **
GuestOsFeatures_arr_from_json(const JSON_Array *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct GuestOsFeatures *
GuestOsFeatures_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Scheduling *
Scheduling_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct ShieldedInstanceConfig *
ShieldedInstanceConfig_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct ShieldedInstanceIntegrityPolicy *
ShieldedInstanceIntegrityPolicy_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Item **
items_from_json(const JSON_Array *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Disk **
Disks_from_json(const JSON_Array *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Disk *
Disk_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT enum InstanceStatus
str_to_InstanceStatus(const char *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT const char *
    InstanceStatus_to_str(enum InstanceStatus);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_INSTANCE_H */
