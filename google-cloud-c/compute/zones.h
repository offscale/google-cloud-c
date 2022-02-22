#ifndef PP_ZONES_H
#define PP_ZONES_H

#include <stdbool.h>
#include <parson.h>

#include "cloud_auth.h"
#include "../gcloud_export.h"

struct Zone {
    const char *id;
    const char *creationTimestamp;
    const char *name;
    const char *description;
    /*enum*/ const char *status;
    // Deprecated deprecated;
    const char *region;
    const char *selfLink;
    const char ** availableCpuPlatforms;
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

extern GCLOUD_EXPORT struct Zones zone_list();

#endif /* PP_ZONES_H */
