#ifndef PP_CLOUD_AUTH_H
#define PP_CLOUD_AUTH_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <curl/curl.h>
#include <curl_simple_https.h>

#if defined(_GNU_SOURCE) || defined(HAVE_ASPRINTF)
#include <stdio.h>
#else
#include <bsd-asprintf.h>
#endif

#include "../gcloud_export.h"

/* ============
 * |   AUTH   |
 * ============ */

struct AuthContext {
    const char *project_id, *google_access_token;
};

extern GCLOUD_EXPORT struct AuthContext AUTH_CONTEXT;

extern GCLOUD_EXPORT struct AuthContext set_auth_context(const char *, const char *);

extern GCLOUD_EXPORT struct curl_slist * set_auth_and_json_headers(struct curl_slist *);

extern GCLOUD_EXPORT CURLU * set_url_path(CURLU *, const char *);

extern GCLOUD_EXPORT CURLU * set_body(CURLU *, const char *);

extern GCLOUD_EXPORT struct ServerResponse gcloud_post(CURLU *, const char *, const char *, struct curl_slist *);
extern GCLOUD_EXPORT struct ServerResponse gcloud_put(CURLU *, const char *, const char *, struct curl_slist *);
extern GCLOUD_EXPORT struct ServerResponse gcloud_get(CURLU *, const char *, struct curl_slist *);

#endif /* PP_CLOUD_AUTH_H */
