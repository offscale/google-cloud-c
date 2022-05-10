#ifndef GOOGLE_CLOUD_C_NETWORK_H
#define GOOGLE_CLOUD_C_NETWORK_H

#ifdef __cplusplus
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __cplusplus */

#include <parson.h>

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/compute/google_cloud_c_compute_export.h>

struct Operation {
  const char *id, *name, *operationType, *targetLink, *targetId, *status, *user,
      *progress, *insertTime, *startTime, *selfLink, *kind;
};

struct RoutingConfig {
  const char *routingMode;
};

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT const struct RoutingConfig
    routingConfigNull;

struct Network {
  const char *id, *creationTimestamp, *name, *selfLink;
  bool autoCreateSubnetworks;
  const char **subnetworks;
  struct RoutingConfig *routingConfig;
  const char *kind;
};

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT const struct Network EMPTY_NETWORK;

struct OptionalNetwork {
  bool set;
  struct Network network;
};

/* =============
 * |  NETWORK  |
 * ============= */

/* CHECK IF NETWORK EXISTS
 * https://cloud.google.com/compute/docs/reference/rest/v1/networks/get */
extern GOOGLE_CLOUD_C_COMPUTE_EXPORT bool network_exists(const char *);

/* Returns the specified network.
 * https://cloud.google.com/compute/docs/reference/rest/v1/networks/get */
extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct OptionalNetwork
network_get(const char *network_name);

/* Creates a network in the specified project using the data included in the
 * request.
 * https://cloud.google.com/compute/docs/reference/rest/v1/networks/insert */
extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct OptionalNetwork
network_create(const char *network_name);

/* utility functions */

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Network
network_from_json(const JSON_Object *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_NETWORK_H */
