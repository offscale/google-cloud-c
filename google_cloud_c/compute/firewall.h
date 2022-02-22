#ifndef PP_FIREWALL_H
#define PP_FIREWALL_H

#include <stdio.h>
#include <acquire_stdbool.h>

#include "cloud_auth.h"
#include <google_cloud_c_export.h>

struct Firewall {
    const char * name;
};

extern GOOGLE_CLOUD_C_EXPORT const struct Firewall EMPTY_FIREWALL;

struct OptionalFirewall {
    bool set;
    struct Firewall firewall;
};

/* =============
 * |  FIREWALL |
 * ============= */

extern GOOGLE_CLOUD_C_EXPORT bool firewall_exists(const char *);

extern GOOGLE_CLOUD_C_EXPORT struct OptionalFirewall firewall_create(const char *, const char *);

extern GOOGLE_CLOUD_C_EXPORT struct OptionalFirewall firewall_get(const char *);

#endif /* PP_FIREWALL_H */
