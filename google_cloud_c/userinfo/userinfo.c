/* https://cloud.google.com/identity-platform/docs/reference/rest/v1/UserInfo */
#include "userinfo.h"
#include "cloud_auth.h"
#include <curl/urlapi.h>
#include <curl_simple_https.h>
#include <json_common.h>

struct Oauth2UserInfo oauth2_user_info_parse(const JSON_Object *);

/* undocumented by Google? */
struct Oauth2UserInfo userinfo_get(const char *google_access_token) {
  CURLU *urlp = curl_url();
  CURLUcode rc = curl_url_set(urlp, CURLUPART_URL, "https://www.googleapis.com/oauth2/v3/userinfo", 0);
  {
    char *access_token;
    asprintf(&access_token, "access_token=%s", google_access_token == NULL ? AUTH_CONTEXT.google_access_token: google_access_token);
    rc = curl_url_set(urlp, CURLUPART_QUERY, access_token, 0);
  }
  {
    struct ServerResponse response = https_json_post(urlp, NULL, NULL);
    if_bad_status_exit(&response);
    return oauth2_user_info_parse(
        json_value_get_object(json_parse_string(response.body)));
  }
}

/* Utility functions */

struct Oauth2UserInfo
oauth2_user_info_parse(const JSON_Object *oauth2_user_info_obj) {
  struct Oauth2UserInfo oauth2_user_info;
  oauth2_user_info.sub = json_object_get_string(oauth2_user_info_obj, "sub");
  oauth2_user_info.picture =
      json_object_get_string(oauth2_user_info_obj, "picture");
  oauth2_user_info.email =
      json_object_get_string(oauth2_user_info_obj, "email");
  oauth2_user_info.email_verified =
      (bool)json_object_get_boolean(oauth2_user_info_obj, "email_verified");
  return oauth2_user_info;
}
