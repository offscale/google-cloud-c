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
  const char *const localId;
  const char *const email;
  const char *const displayName;
  const char *const language;
  const char *const photoUrl;
  const char *const timeZone;
  const char *const dateOfBirth;
  const char *const passwordHash;
  const char *const salt;
  int version;
  bool emailVerified;
  int passwordUpdatedAt;
  /*const char *const providerUserInfo": [
  {
    object (ProviderUserInfo)
  }
  ],*/
  const char *const validSince;
  bool disabled;
  const char *const lastLoginAt;
  const char *const createdAt;
  const char *const screenName;
  bool customAuth;
  const char *const rawPassword;
  const char *const phoneNumber;
  const char *const customAttributes;
  bool emailLinkSignin;
  const char *const tenantId;
  /*const char *const mfaInfo": [
  {
    object (MfaEnrollment)
  }
  ],*/
  const char *const initialEmail;
  const char *const lastRefreshAt;
};

struct Oauth2UserInfo {
  const char *sub;
  const char *picture;
  const char *email;
  bool email_verified;
};

extern GOOGLE_CLOUD_C_USERINFO_EXPORT struct Oauth2UserInfo *
userinfo_get(const char *);

/* Utility functions */

extern GOOGLE_CLOUD_C_USERINFO_EXPORT struct Oauth2UserInfo *
oauth2_user_info_from_json(const JSON_Object *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_USERINFO_USERINFO_H */
