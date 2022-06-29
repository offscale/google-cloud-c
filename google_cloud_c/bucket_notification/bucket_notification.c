#include <google_cloud_c/bucket_notification/bucket_notification.h>

/* utility functions */

struct BucketNotification *
bucket_notification_from_json(const JSON_Object *const jsonObject) {
  struct BucketNotification *bucket_notification =
      malloc(sizeof(struct BucketNotification));
  bucket_notification->kind = json_object_get_string(jsonObject, "kind");
  bucket_notification->id = json_object_get_string(jsonObject, "id");
  bucket_notification->selfLink =
      json_object_get_string(jsonObject, "selfLink");
  bucket_notification->name = json_object_get_string(jsonObject, "name");
  bucket_notification->bucket = json_object_get_string(jsonObject, "bucket");
  bucket_notification->generation =
      json_object_get_string(jsonObject, "generation");
  bucket_notification->metageneration =
      json_object_get_string(jsonObject, "metageneration");
  bucket_notification->contentType =
      json_object_get_string(jsonObject, "contentType");
  bucket_notification->timeCreated =
      json_object_get_string(jsonObject, "timeCreated");
  bucket_notification->updated = json_object_get_string(jsonObject, "updated");
  bucket_notification->storageClass =
      json_object_get_string(jsonObject, "storageClass");
  bucket_notification->timeStorageClassUpdated =
      json_object_get_string(jsonObject, "timeStorageClassUpdated");
  bucket_notification->size = json_object_get_string(jsonObject, "size");
  bucket_notification->md5Hash = json_object_get_string(jsonObject, "md5Hash");
  bucket_notification->mediaLink =
      json_object_get_string(jsonObject, "mediaLink");
  bucket_notification->crc32c = json_object_get_string(jsonObject, "crc32c");
  bucket_notification->etag = json_object_get_string(jsonObject, "etag");
  return bucket_notification;
}
