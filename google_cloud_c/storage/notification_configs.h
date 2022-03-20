#ifndef GOOGLE_CLOUD_C_NOTIFICATION_CONFIGS_H
#define GOOGLE_CLOUD_C_NOTIFICATION_CONFIGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <google_cloud_c_export.h>
#include <types_common.h>

/* A subscription to receive Pub/Sub notifications.
 * https://cloud.google.com/storage/docs/json_api/v1/notifications#resource */
struct Notification {
  const char *id;
  const char *topic;

  const char **event_types;
  /*"custom_attributes": {(key): string},*/
  const char *payload_format;

  const char *object_name_prefix;

  const char *etag;

  const char *selfLink;

  const char *kind /* "storage#notification" */;
};

struct OptionalNotification {
  bool set;
  struct Notification notification;
};

extern GOOGLE_CLOUD_C_EXPORT const struct Notification EMPTY_NOTIFICATION;

extern GOOGLE_CLOUD_C_EXPORT struct OptionalNotification
get_notification_configuration(const struct configuration *, const char *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_NOTIFICATION_CONFIGS_H */
