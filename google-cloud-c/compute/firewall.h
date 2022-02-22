#ifndef PP_FIREWALL_H
#define PP_FIREWALL_H

#include <stdio.h>
#include <stdbool.h>

#include "cloud_auth.h"
#include "../gcloud_export.h"

struct Firewall {
    const char * name;
};

extern GCLOUD_EXPORT const struct Firewall EMPTY_FIREWALL;

struct OptionalFirewall {
    bool set;
    struct Firewall firewall;
};

/* =============
 * |  FIREWALL |
 * ============= */

extern GCLOUD_EXPORT bool firewall_exists(const char *);

extern GCLOUD_EXPORT struct OptionalFirewall firewall_create(const char *, const char *);

extern GCLOUD_EXPORT struct OptionalFirewall firewall_get(const char *);

#endif /* PP_FIREWALL_H */
