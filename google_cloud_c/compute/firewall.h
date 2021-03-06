#ifndef GOOGLE_CLOUD_C_FIREWALL_H
#define GOOGLE_CLOUD_C_FIREWALL_H

#ifdef __cplusplus
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __cplusplus */

#include <stdio.h>

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/compute/google_cloud_c_compute_export.h>
#include <google_cloud_c/operation/operation.h>

struct Firewall {
  const char *name;
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
extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Operation *
firewall_create(const char *, const char *);

/* Returns the specified firewall.
 * https://cloud.google.com/compute/docs/reference/rest/v1/firewalls/get
 */
extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Firewall *
firewall_get(const char *);

/* Utility functions */

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Firewall *
Firewall_from_name(const char *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_FIREWALL_H */
