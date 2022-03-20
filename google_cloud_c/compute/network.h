#ifndef GOOGLE_CLOUD_C_NETWORK_H
#define GOOGLE_CLOUD_C_NETWORK_H

#ifdef __cplusplus
#include <cstdbool>
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c_stdbool.h>
#endif /* __cplusplus */

#include <parson.h>

#include "cloud_auth.h"
#include <google_cloud_c_export.h>

struct Operation {
  const char *id, *name, *operationType, *targetLink, *targetId, *status, *user,
      *progress, *insertTime, *startTime, *selfLink, *kind;
};

struct RoutingConfig {
  const char *routingMode;
};

extern GOOGLE_CLOUD_C_EXPORT const struct RoutingConfig routingConfigNull;

struct Network {
  const char *id, *creationTimestamp, *name, *selfLink;
  bool autoCreateSubnetworks;
  const char **subnetworks;
  struct RoutingConfig *routingConfig;
  const char *kind;
};

extern GOOGLE_CLOUD_C_EXPORT const struct Network EMPTY_NETWORK;

struct OptionalNetwork {
  bool set;
  struct Network network;
};

/* =============
 * |  NETWORK  |
 * ============= */

extern GOOGLE_CLOUD_C_EXPORT bool network_exists(const char *);

extern GOOGLE_CLOUD_C_EXPORT struct OptionalNetwork
network_get(const char *network_name);

extern GOOGLE_CLOUD_C_EXPORT struct OptionalNetwork
network_create(const char *network_name);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_NETWORK_H */
