#ifndef GOOGLE_CLOUD_C_ZONES_H
#define GOOGLE_CLOUD_C_ZONES_H

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

struct Zone {
  const char *id;
  const char *creationTimestamp;
  const char *name;
  const char *description;
  /*enum*/ const char *status;
  /* Deprecated deprecated; */
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

/* Retrieves the list of Zone resources available to the specified project.
 * https://cloud.google.com/compute/docs/reference/rest/v1/zones/list */
extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Zones zone_list();

/* Utility functions */

extern GOOGLE_CLOUD_C_COMPUTE_EXPORT struct Zone
zone_from_json(const JSON_Object *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_ZONES_H */
