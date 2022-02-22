#ifndef PP_COMPUTE_H
#define PP_COMPUTE_H

#include <types_common.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif

#include "gcloud_export.h"

#include "compute/instance.h"

/*
struct CInstance {
    const char *id,
               *creationTimestamp,
               *name,
               *description,
               *machineType,
               *statusMessage,
               *zone,
               *cpuPlatform,
               *hostname,
               *lastStartTimestamp,
               *lastStopTimestamp,
               *lastSuspendedTimestamp,
               *kind;
    // std::map<std::string, std::string> metadata;
    bool supportsPzs;
};
*/

extern GCLOUD_EXPORT struct StatusAndArrayCStrArray
compute(const struct configuration *, const char *, bool);

extern GCLOUD_EXPORT struct StatusAndCstrAndCStr
create_fw_net_instance(const struct configuration *, const struct InstanceIncomplete *);

extern GCLOUD_EXPORT struct StatusAndCstr
get_instance_ip(const struct configuration *, const char *);

#ifdef	__cplusplus
} /*extern "C"*/
#endif

#endif /* PP_COMPUTE_H */
