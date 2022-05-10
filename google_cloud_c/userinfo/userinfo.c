/* https://cloud.google.com/identity-platform/docs/reference/rest/v1/UserInfo */
#include <curl/urlapi.h>
#include <parson.h>

#include <curl_simple_https.h>

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/userinfo/userinfo.h>

const struct Oauth2UserInfo oauth2UserInfoNull = {NULL, NULL, NULL, false};

struct Oauth2UserInfo
oauth2_user_info_from_json(const JSON_Object *oauth2_user_info_obj);

/* undocumented by Google? */
struct OptionalOauth2UserInfo userinfo_get(const char *google_access_token) {
  CURLU *urlp = curl_url();
  IS_CURLUE_OK(curl_url_set(
      urlp, CURLUPART_URL, "https://www.googleapis.com/oauth2/v3/userinfo", 0));
  {
    char *access_token;
    asprintf(&access_token, "access_token=%s",
             google_access_token == NULL ? AUTH_CONTEXT.google_access_token
                                         : google_access_token);
    IS_CURLUE_OK(curl_url_set(urlp, CURLUPART_QUERY, access_token, 0));
  }
  {
    struct ServerResponse response = https_json_post(urlp, NULL, NULL);
    struct OptionalOauth2UserInfo optionalOauth2UserInfo;
    if (response.code == 200)
      optionalOauth2UserInfo.set = true,
      optionalOauth2UserInfo.oauth2UserInfo = oauth2_user_info_from_json(
          json_value_get_object(json_parse_string(response.body)));
    else
      optionalOauth2UserInfo.set = false,
      optionalOauth2UserInfo.oauth2UserInfo = oauth2UserInfoNull;
    return optionalOauth2UserInfo;
  }
}

/* Utility functions */

struct Oauth2UserInfo
oauth2_user_info_from_json(const JSON_Object *oauth2_user_info_obj) {
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
