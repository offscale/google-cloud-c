#ifndef GOOGLE_CLOUD_C_FIREWALL_H
#define GOOGLE_CLOUD_C_FIREWALL_H

#include <stdio.h>

#include "cloud_auth.h"
#include <google_cloud_c/compute/google_cloud_c_compute_export.h>

#ifdef __cplusplus
#include <cstdbool>
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __cplusplus */

struct Firewall {
  const char *name;
};

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT const struct Firewall EMPTY_FIREWALL;

struct OptionalFirewall {
  bool set;
  struct Firewall firewall;
};

/* =============
 * |  FIREWALL |
 * ============= */

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT bool firewall_exists(const char *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct OptionalFirewall
firewall_create(const char *, const char *);

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct OptionalFirewall
firewall_get(const char *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_FIREWALL_H */
