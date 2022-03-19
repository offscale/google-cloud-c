#ifndef GOOGLE_CLOUD_C_CLOUD_AUTH_H
#define GOOGLE_CLOUD_C_CLOUD_AUTH_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <curl/curl.h>
#include <curl_simple_https.h>

#if defined(_GNU_SOURCE) || defined(HAVE_ASPRINTF)
#include <stdio.h>
#else
#define C89STRINGUTILS_IMPLEMENTATION
#include <c89stringutils_string_extras.h>
#endif

#define DEBUG_SERVER_RESPONSE(name) fprintf(response.status_code == 200 ? stdout : stderr, \
            name": %ld\t%s\n", \
            response.status_code, \
            response.body == NULL ? "" : response.body)

#include <google_cloud_c_export.h>

/* ============
 * |   AUTH   |
 * ============ */

struct AuthContext {
    const char *project_id, *google_access_token;
};

extern GOOGLE_CLOUD_C_EXPORT struct AuthContext AUTH_CONTEXT;

extern GOOGLE_CLOUD_C_EXPORT struct AuthContext set_auth_context(const char *, const char *);

extern GOOGLE_CLOUD_C_EXPORT struct curl_slist * set_auth_and_json_headers(struct curl_slist *);

extern GOOGLE_CLOUD_C_EXPORT CURLU * set_url_path(CURLU *, const char *);

extern GOOGLE_CLOUD_C_EXPORT CURLU * set_body(CURLU *, const char *);

extern GOOGLE_CLOUD_C_EXPORT struct ServerResponse gcloud_post(CURLU *, const char *, const char *, struct curl_slist *);
extern GOOGLE_CLOUD_C_EXPORT struct ServerResponse gcloud_put(CURLU *, const char *, const char *, struct curl_slist *);
extern GOOGLE_CLOUD_C_EXPORT struct ServerResponse gcloud_get(CURLU *, const char *, struct curl_slist *);

#endif /* !GOOGLE_CLOUD_C_CLOUD_AUTH_H */
