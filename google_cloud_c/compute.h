#ifndef GOOGLE_CLOUD_C_COMPUTE_H
#define GOOGLE_CLOUD_C_COMPUTE_H

#include <types_common.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <google_cloud_c_export.h>

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

extern GOOGLE_CLOUD_C_EXPORT struct StatusAndArrayCStrArray
compute(const struct configuration *, const char *, bool);

extern GOOGLE_CLOUD_C_EXPORT struct StatusAndCstrAndCStr
create_fw_net_instance(const struct configuration *,
                       const struct InstanceIncomplete *);

extern GOOGLE_CLOUD_C_EXPORT struct StatusAndCstr
get_instance_ip(const struct configuration *, const char *);

#ifdef __cplusplus
} /*extern "C"*/
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_COMPUTE_H */
