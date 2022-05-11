#ifndef GOOGLE_CLOUD_C_FIREWALL_H
#define GOOGLE_CLOUD_C_FIREWALL_H

#include <stdio.h>

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/compute/google_cloud_c_compute_export.h>

#ifdef __cplusplus
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __cplusplus */

struct Firewall {
  const char *name;
};

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT const struct Firewall firewallNull;

struct OptionalFirewall {
  bool set;
  struct Firewall firewall;
};

/* =============
 * |  FIREWALL |
 * ============= */

/* Check if firewall exists
 * https://cloud.google.com/compute/docs/reference/rest/v1/firewalls/get
 */
extern GOOGLE_CLOUD_C_COMPUTE_EXPORT bool firewall_exists(const char *);

/* Creates a firewall rule in the specified project using the data included in
 * the request.
 * https://cloud.google.com/compute/docs/reference/rest/v1/firewalls/insert
 * */
extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct OptionalFirewall
firewall_create(const char *, const char *);

/* Returns the specified firewall.
 * https://cloud.google.com/compute/docs/reference/rest/v1/firewalls/get
 */
extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct OptionalFirewall
firewall_get(const char *);

/* Utility functions */

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Firewall
Firewall_from_name(const char *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_FIREWALL_H */
