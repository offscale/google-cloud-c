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
#endif

#else

#include <errno.h>
#include <unistd.h>

#define ERROR_CALL_NOT_IMPLEMENTED ENOATTR
#endif

// struct InstanceIncomplete cinstance_to_instance(const struct CInstance *);

struct AccessConfigs {
  const char *type, *name, *natIP, *networkTier, *kind;
};

struct NetworkInterface {
  const char *network, *subnetwork, *networkIP, *name;
  struct AccessConfigs *accessConfigs;
};

struct GuestOsFeatures {
  const char *type;
};

struct Disk {
  const char *type;
  const char *mode;
  const char *source;
  int index;
  bool boot, autoDelete;
  const char **licenses;
  const char *interface;
  struct GuestOsFeatures *guestOsFeatures;
  const char *diskSizeGb, *kind;
};

struct Item {
  const char *key, *value;
};

struct Metadata {
  struct Item *items;
  const char *kind;
};

struct Scheduling {
  const char *onHostMaintenance;
  bool automaticRestart, preemptible;
};

struct ShieldedInstanceConfig {
  bool enableSecureBoot /*=false*/, enableVtpm /*=true*/,
      enableIntegrityMonitoring /*=true*/;
};

struct ShieldedInstanceIntegrityPolicy {
  bool updateAutoLearnPolicy /*=true*/;
};

struct Instance {
  const char *id, *creationTimestamp, *name, *machineType, *status, *zone;
  struct NetworkInterface *networkInterfaces;
  struct Disk *disks;
  struct Metadata *metadata;
  const char *selfLink;
  struct Scheduling *scheduling;
  const char *cpuPlatform, *labelFingerprint;
  bool startRestricted /*=false*/;
  bool deletionProtection /*=false*/;
  struct ShieldedInstanceConfig *shieldedInstanceConfig;
  struct ShieldedInstanceIntegrityPolicy *shieldedInstanceIntegrityPolicy;
  const char *fingerprint, *lastStartTimestamp, *kind;
};

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT const struct Instance EMPTY_INSTANCE;

struct OptionalInstance {
  bool set;
  struct Instance instance;
};

struct Instances {
  struct Instance *arr;
  size_t size;
};

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT const struct Instances EMPTY_INSTANCES;

/* =============
 * | INSTANCES |
 * ============= */

struct InstanceContext {
  const char *firewall_name, *network_name, *zone;
};

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct InstanceContext INSTANCE_CONTEXT;

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Instances instances_list();

// struct Instance instance_create_all(const struct InstanceIncomplete *);

// struct Instance instance_create_all(const struct CInstance *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT bool
instance_exists(const char *instance_name);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct OptionalInstance
instance_insert(const struct InstanceIncomplete *, const char *, const char *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct OptionalInstance
instance_get(const char *instance_name);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct OptionalInstance
instance_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct OptionalInstance
instance_incomplete_create_all(const struct InstanceIncomplete *, const char *,
                               const char *, const char *);

/*
private:
    static std::string instance_to_json(const struct InstanceIncomplete
&instance) ;

    struct Instance from(const json &_json) const;
};
*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_INSTANCE_H */
