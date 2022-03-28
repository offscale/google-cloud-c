#include <google_cloud_c/client/cloud_auth.h>

extern CURLU *set_compute_url_path(CURLU *, const char *);

extern CURLU *set_cloud_resource_url_path(CURLU *, const char *);

struct AuthContext AUTH_CONTEXT = {NULL, NULL};

struct AuthContext set_auth_context(const char *project_id,
                                    const char *google_access_token) {
  AUTH_CONTEXT.project_id = project_id;
  AUTH_CONTEXT.google_access_token = google_access_token;
  return AUTH_CONTEXT;
}

struct curl_slist *set_auth_and_json_headers(struct curl_slist *headers) {
  headers = curl_slist_append(headers, "Accept: application/json");
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, "charset: utf-8");

  if (AUTH_CONTEXT.google_access_token != NULL) {
    /*  2048? As per https://developers.google.com/identity/protocols/oauth2
     *  512? As per refresh token docs on ^
     * */
#define ACCESS_TOKEN_SIZE 512
    {
      char bearer[ACCESS_TOKEN_SIZE + /*strlen("Authorization: Bearer ")*/ 22 +
                  1] = "Authorization: Bearer ";
      const size_t token_size = strlen(AUTH_CONTEXT.google_access_token);
      assert(token_size < ACCESS_TOKEN_SIZE && token_size > 0);
#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
      strncat_s(bearer, ACCESS_TOKEN_SIZE, AUTH_CONTEXT.google_access_token,
                token_size);
#else
      strncat(bearer, AUTH_CONTEXT.google_access_token, token_size);
#endif
      headers = curl_slist_append(headers, bearer);
    }
#undef ACCESS_TOKEN_SIZE
  }
  if (AUTH_CONTEXT.project_id != NULL) {
    char *goog_user_project;
    asprintf(&goog_user_project, "X-Goog-User-Project: %s",
             AUTH_CONTEXT.project_id);
    headers = curl_slist_append(headers, goog_user_project);
  }

  return headers;
}

CURLU *set_compute_url_path(CURLU *urlp, const char *path) {
  CURLUcode rc;
  const size_t path_n = strlen(path);
  if (urlp == NULL) {
    urlp = curl_url();
  }
  curl_url_set(urlp, CURLUPART_SCHEME, "https", 0);
  rc = curl_url_set(urlp, CURLUPART_HOST, "compute.googleapis.com", 0);
  assert(rc == CURLUE_OK);
  /*rc = curl_url_set(urlp, CURLUPART_PORT, "443", 0);*/
  assert(path_n > 0);
  {
    unsigned short j = path[0] == '/' ? 1 : 0;
    if (path[j] == 'c' && path[1 + j] == 'o' && path[2 + j] == 'm' &&
        path[3 + j] == 'p' && path[4 + j] == 'u' && path[5 + j] == 't' &&
        path[6 + j] == 'e' && (path[7 + j] == '/' || path[7 + j] == '\0'))
      rc = curl_url_set(urlp, CURLUPART_PATH, path, 0);
    else {
      char *appended_path;
      asprintf(&appended_path, "/compute%s%s", path[0] == '/' ? "" : "/", path);
      rc = curl_url_set(urlp, CURLUPART_PATH, appended_path, 0);
    }
    assert(rc == CURLUE_OK);
  }

  return urlp;
}

CURLU *set_cloud_resource_url_path(CURLU *urlp, const char *path) {
  CURLUcode rc;
  const size_t path_n = strlen(path);
  if (urlp == NULL)
    urlp = curl_url();
  curl_url_set(urlp, CURLUPART_SCHEME, "https", 0);
  rc = curl_url_set(urlp, CURLUPART_HOST, "cloudresourcemanager.googleapis.com",
                    0);
  assert(rc == CURLUE_OK);
  assert(path_n > 0);
  {
    unsigned short j = path[0] == '/' ? 1 : 0;
    if (path[j] == 'v' && /* path[1 + j] == '1' && */ path[2 + j] == '/') {
      rc = curl_url_set(urlp, CURLUPART_PATH, path, 0);
    } else {
      char *appended_path;
      asprintf(&appended_path, "/v1%s%s", path[0] == '/' ? "" : "/", path);
      rc = curl_url_set(urlp, CURLUPART_PATH, appended_path, 0);
    }
    assert(rc == CURLUE_OK);
  }

  return urlp;
}

CURLU *set_storage_api_url_path(CURLU *urlp, const char *path) {
  CURLUcode rc;
  const size_t path_n = strlen(path);
  if (urlp == NULL)
    urlp = curl_url();
  curl_url_set(urlp, CURLUPART_SCHEME, "https", 0);
  rc = curl_url_set(urlp, CURLUPART_HOST, "storage.googleapis.com", 0);
  rc = curl_url_set(urlp, CURLUPART_PATH, path, 0);

  return urlp;
}

/*
CURLU * set_body(CURLU *urlp, const char *path) {
    return urlp;
}
*/

struct ServerResponse gcloud_post(CURLU *urlp, const char *path,
                                  const char *body,
                                  struct curl_slist *headers) {
  return https_wrapper(set_compute_url_path(urlp, path), make_request_post,
                       body, set_auth_and_json_headers(headers));
}

struct ServerResponse gcloud_put(CURLU *urlp, const char *path,
                                 const char *body, struct curl_slist *headers) {
  return https_wrapper(set_compute_url_path(urlp, path), make_request_put, body,
                       set_auth_and_json_headers(headers));
}

struct ServerResponse gcloud_get(CURLU *urlp, const char *path,
                                 struct curl_slist *headers) {
  return https_wrapper(set_compute_url_path(urlp, path), NULL, NULL,
                       set_auth_and_json_headers(headers));
}

struct ServerResponse gcloud_cloud_resource_post(CURLU *urlp, const char *path,
                                                 const char *body,
                                                 struct curl_slist *headers) {
  return https_wrapper(set_cloud_resource_url_path(urlp, path),
                       make_request_post, body,
                       set_auth_and_json_headers(headers));
}

struct ServerResponse gcloud_cloud_resource_put(CURLU *urlp, const char *path,
                                                const char *body,
                                                struct curl_slist *headers) {
  return https_wrapper(set_cloud_resource_url_path(urlp, path),
                       make_request_put, body,
                       set_auth_and_json_headers(headers));
}

struct ServerResponse gcloud_cloud_resource_get(CURLU *urlp, const char *path,
                                                struct curl_slist *headers) {
  return https_wrapper(set_cloud_resource_url_path(urlp, path), NULL, NULL,
                       set_auth_and_json_headers(headers));
}

struct ServerResponse gcloud_storage_post(CURLU *urlp, const char *path,
                                          const char *body,
                                          struct curl_slist *headers) {
  return https_wrapper(set_storage_api_url_path(urlp, path), make_request_post,
                       body, set_auth_and_json_headers(headers));
}

struct ServerResponse gcloud_storage_put(CURLU *urlp, const char *path,
                                         const char *body,
                                         struct curl_slist *headers) {
  return https_wrapper(set_storage_api_url_path(urlp, path), make_request_put,
                       body, set_auth_and_json_headers(headers));
}

struct ServerResponse gcloud_storage_get(CURLU *urlp, const char *path,
                                         struct curl_slist *headers) {
  return https_wrapper(set_storage_api_url_path(urlp, path), NULL, NULL,
                       set_auth_and_json_headers(headers));
}

/*
Request Auth::make_request(const char *path, const char *verb) const {
    const URL u = URL("https://compute.googleapis.com/compute" + path);
    Request req;
    req.verb = verb;
    req.uri.protocol = "HTTP";
    req.uri.protocol_version = "1.0";
    req.uri.use_ssl = true;
    req.uri.port = 443;
    req.uri.host = u.domain;
    req.uri.path = u.path;
    req.uri.querystring = u.encoded_querystring();
    req.uri.fragment = u.fragment;
    req.headers.push_back("HOST: " + req.uri.host);
    req.headers.push_back("Authorization: Bearer " +
std::string(google_access_token)); return req;
}
*/
