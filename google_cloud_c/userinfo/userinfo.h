#ifndef GOOGLE_CLOUD_C_USERINFO_USERINFO_H
#define GOOGLE_CLOUD_C_USERINFO_USERINFO_H

#ifdef __cplusplus
#include <cstdlib>
extern "C" {
#else
#include <stdlib.h>
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __STDC_VERSION__ >= 199901L */
#endif /* __cplusplus */

#include <google_cloud_c/userinfo/google_cloud_c_userinfo_export.h>

struct UserInfo {
  const char *localId;
  const char *email;
  const char *displayName;
  const char *language;
  const char *photoUrl;
  const char *timeZone;
  const char *dateOfBirth;
  const char *passwordHash;
  const char *salt;
  int version;
  bool emailVerified;
  int passwordUpdatedAt;
  /*const char *providerUserInfo": [
  {
    object (ProviderUserInfo)
  }
  ],*/
  const char *validSince;
  bool disabled;
  const char *lastLoginAt;
  const char *createdAt;
  const char *screenName;
  bool customAuth;
  const char *rawPassword;
  const char *phoneNumber;
  const char *customAttributes;
  bool emailLinkSignin;
  const char *tenantId;
  /*const char *mfaInfo": [
  {
    object (MfaEnrollment)
  }
  ],*/
  const char *initialEmail;
  const char *lastRefreshAt;
};

struct Oauth2UserInfo {
  const char *sub;
  const char *picture;
  const char *email;
  bool email_verified;
};

struct OptionalOauth2UserInfo {
  bool set;
  struct Oauth2UserInfo oauth2UserInfo;
};

extern GOOGLE_CLOUD_C_USERINFO_EXPORT struct OptionalOauth2UserInfo
userinfo_get(const char *);

extern GOOGLE_CLOUD_C_USERINFO_EXPORT const struct Oauth2UserInfo
    oauth2UserInfoNull;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_USERINFO_USERINFO_H */
