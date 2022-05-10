#ifndef GOOGLE_CLOUD_C_CLIENT_CLOUD_AUTH_H
#define GOOGLE_CLOUD_C_CLIENT_CLOUD_AUTH_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <curl_simple_https.h>

#define IS_CURLUE_OK(rc) assert((rc) == CURLUE_OK)

#if defined(_GNU_SOURCE) || defined(HAVE_ASPRINTF)
#include <stdio.h>
#else
#include <c89stringutils_string_extras.h>
#endif

#include <google_cloud_c/client/google_cloud_c_client_export.h>

#define DEBUG_SERVER_RESPONSE(name)                                            \
  fprintf(response.status_code == 200 ? stdout : stderr, name ": %ld\t%s\n",   \
          response.status_code, response.body == NULL ? "" : response.body)

/* ============
 * |   AUTH   |
 * ============ */

struct AuthContext {
  const char *project_id, *google_access_token;
};

struct GClient {
  struct ServerResponse(*client);
};

extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct AuthContext AUTH_CONTEXT;

extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct AuthContext
set_auth_context(const char *, const char *);

extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct curl_slist *
set_auth_and_json_headers(struct curl_slist *);

extern GOOGLE_CLOUD_C_CLIENT_EXPORT CURLU *set_body(CURLU *, const char *);

extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_get(CURLU *, const char *, struct curl_slist *);
extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_post(CURLU *, const char *, const char *, struct curl_slist *);
extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_put(CURLU *, const char *, const char *, struct curl_slist *);
extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_delete(CURLU *, const char *, const char *, struct curl_slist *);

extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_cloud_resource_get(CURLU *, const char *, struct curl_slist *);
extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_cloud_resource_post(CURLU *, const char *, const char *,
                           struct curl_slist *);
extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_cloud_resource_put(CURLU *, const char *, const char *,
                          struct curl_slist *);
extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_cloud_resource_delete(CURLU *, const char *, const char *,
                             struct curl_slist *);

extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_storage_get(CURLU *, const char *, struct curl_slist *);
extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_storage_post(CURLU *, const char *, const char *, struct curl_slist *);
extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_storage_put(CURLU *, const char *, const char *, struct curl_slist *);
extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_storage_delete(CURLU *, const char *, const char *, struct curl_slist *);

extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_pubsub_get(CURLU *, const char *, struct curl_slist *);
extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_pubsub_post(CURLU *, const char *, const char *, struct curl_slist *);
extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_pubsub_put(CURLU *, const char *, const char *, struct curl_slist *);
extern GOOGLE_CLOUD_C_CLIENT_EXPORT struct ServerResponse
gcloud_pubsub_delete(CURLU *, const char *, const char *, struct curl_slist *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_CLIENT_CLOUD_AUTH_H */
