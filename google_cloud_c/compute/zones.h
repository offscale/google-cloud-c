#ifndef GOOGLE_CLOUD_C_ZONES_H
#define GOOGLE_CLOUD_C_ZONES_H

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

struct Zone {
  const char *id;
  const char *creationTimestamp;
  const char *name;
  const char *description;
  /*enum*/ const char *status;
  // Deprecated deprecated;
  const char *region;
  const char *selfLink;
  const char **availableCpuPlatforms;
  bool supportsPzs;
  const char *kind;
};

struct Zones {
  struct Zone *arr;
  size_t size;
};

/* =============
 * |   ZONES   |
 * ============= */

extern GOOGLE_CLOUD_C_EXPORT struct Zones zone_list();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_ZONES_H */
