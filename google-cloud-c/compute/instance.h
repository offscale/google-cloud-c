#ifndef PP_INSTANCE_H
#define PP_INSTANCE_H

#include <types_common.h>
#include "../compute.h"
#include "firewall.h"
#include "network.h"
#include "../gcloud_export.h"
#include <stdbool.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#include <synchapi.h>

#ifdef interface
#undef interface
#endif

#else

#include <unistd.h>
#include <errno.h>

#define ERROR_CALL_NOT_IMPLEMENTED ENOATTR
#endif


// struct InstanceIncomplete cinstance_to_instance(const struct CInstance *);

struct AccessConfigs {
    const char *type,
            *name,
            *natIP,
            *networkTier,
            *kind;
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
    bool enableSecureBoot/*=false*/, enableVtpm/*=true*/, enableIntegrityMonitoring/*=true*/;
};

struct ShieldedInstanceIntegrityPolicy {
    bool updateAutoLearnPolicy/*=true*/;
};

struct Instance {
    const char *id,
               *creationTimestamp,
               *name,
               *machineType,
               *status,
               *zone;
    struct NetworkInterface *networkInterfaces;
    struct Disk *disks;
    struct Metadata *metadata;
    const char *selfLink;
    struct Scheduling *scheduling;
    const char *cpuPlatform, *labelFingerprint;
    bool startRestricted/*=false*/;
    bool deletionProtection/*=false*/;
    struct ShieldedInstanceConfig *shieldedInstanceConfig;
    struct ShieldedInstanceIntegrityPolicy *shieldedInstanceIntegrityPolicy;
    const char *fingerprint, *lastStartTimestamp, *kind;
};

extern GCLOUD_EXPORT const struct Instance EMPTY_INSTANCE;

struct OptionalInstance {
    bool set;
    struct Instance instance;
};

struct Instances {
    struct Instance *arr;
    size_t size;
};

extern GCLOUD_EXPORT const struct Instances EMPTY_INSTANCES;

/* =============
 * | INSTANCES |
 * ============= */

struct InstanceContext {
    const char *firewall_name, *network_name, *zone;
};

extern GCLOUD_EXPORT struct InstanceContext INSTANCE_CONTEXT;

extern GCLOUD_EXPORT struct Instances instances_list();

// struct Instance instance_create_all(const struct InstanceIncomplete *);

// struct Instance instance_create_all(const struct CInstance *);

extern GCLOUD_EXPORT bool instance_exists(const char *instance_name);

extern GCLOUD_EXPORT struct OptionalInstance instance_insert(const struct InstanceIncomplete *, const char *);

extern GCLOUD_EXPORT struct OptionalInstance instance_get(const char *instance_name);

extern GCLOUD_EXPORT struct OptionalInstance instance_from_json(const JSON_Object *);

extern GCLOUD_EXPORT struct OptionalInstance instance_incomplete_create_all(
        const struct InstanceIncomplete *,
        const char *,
        const char *
);

/*
private:
    static std::string instance_to_json(const struct InstanceIncomplete &instance) ;

    struct Instance from(const json &_json) const;
};
*/

#endif /* PP_INSTANCE_H */
