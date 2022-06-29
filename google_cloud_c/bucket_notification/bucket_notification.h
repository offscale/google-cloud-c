#ifndef GOOGLE_CLOUD_C_CLIENT_BUCKET_NOTIFICATION_H
#define GOOGLE_CLOUD_C_CLIENT_BUCKET_NOTIFICATION_H

#ifdef __cplusplus
#include <cassert>
#include <cstdlib>
#include <cstring>
extern "C" {
#else
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#endif /* __cplusplus */

#include <parson.h>

#include <google_cloud_c/bucket_notification/google_cloud_c_bucket_notification_export.h>

/* =================
 * |   Operation   |
 * ================ */

struct BucketNotification {
  const char *kind;
  const char *id;
  const char *selfLink;
  const char *name;
  const char *bucket;
  const char *generation;
  const char *metageneration;
  const char *contentType;
  const char *timeCreated;
  const char *updated;
  const char *storageClass;
  const char *timeStorageClassUpdated;
  const char *size;
  const char *md5Hash;
  const char *mediaLink;
  const char *crc32c;
  const char *etag;
};

/* utility functions */

extern GOOGLE_CLOUD_C_BUCKET_NOTIFICATION_EXPORT struct BucketNotification *
bucket_notification_from_json(const JSON_Object *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_CLIENT_BUCKET_NOTIFICATION_H */
