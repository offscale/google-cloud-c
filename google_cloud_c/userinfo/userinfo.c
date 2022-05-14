/* https://cloud.google.com/identity-platform/docs/reference/rest/v1/UserInfo */
#include <curl/urlapi.h>
#include <parson.h>

#include <c89stringutils_string_extras.h>

#include <curl_simple_https.h>

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/userinfo/userinfo.h>

const struct Oauth2UserInfo oauth2UserInfoNull = {NULL, NULL, NULL, false};

/* undocumented by Google? */
struct OptionalOauth2UserInfo
userinfo_get(const char *const google_access_token) {
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
    const struct ServerResponse response = https_json_post(urlp, NULL, NULL);
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
oauth2_user_info_from_json(const JSON_Object *const jsonObject) {
  struct Oauth2UserInfo oauth2_user_info;
  oauth2_user_info.sub = json_object_get_string(jsonObject, "sub");
  oauth2_user_info.picture = json_object_get_string(jsonObject, "picture");
  oauth2_user_info.email = json_object_get_string(jsonObject, "email");
  if (json_object_has_value_of_type(jsonObject, "email_verified", JSONBoolean))
    oauth2_user_info.email_verified =
        (bool)json_object_get_boolean(jsonObject, "email_verified");
  else
    oauth2_user_info.email_verified = false;
  return oauth2_user_info;
}
