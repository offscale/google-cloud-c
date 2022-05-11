#ifndef GOOGLE_CLOUD_C_COMPUTE_H
#define GOOGLE_CLOUD_C_COMPUTE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <google_cloud_c/common/google_cloud_c_types_common.h>
#include <google_cloud_c/compute/instance.h>
#include <google_cloud_c/multi/google_cloud_c_multi_export.h>

/*
struct CInstance {
    const char *const id,
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

extern GOOGLE_CLOUD_C_MULTI_EXPORT struct StatusAndArrayCStrArray
compute(const struct configuration *, const char *, bool);

extern GOOGLE_CLOUD_C_MULTI_EXPORT struct StatusAndCstrAndCStr
create_fw_net_instance(const struct configuration *,
                       const struct InstanceIncomplete *, const char *);

extern GOOGLE_CLOUD_C_MULTI_EXPORT struct StatusAndCstr
get_instance_ip(const struct configuration *, const char *);

#ifdef __cplusplus
} /*extern "C"*/
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_COMPUTE_H */
