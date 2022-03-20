#include "notification_configs.h"
#include "compute/cloud_auth.h"
#include <json_common.h>
#include <parson.h>

const struct Notification EMPTY_NOTIFICATION = {NULL, NULL, NULL, NULL,
                                                NULL, NULL, NULL, NULL};

/* View a Pub/Sub notification configuration on a given bucket
 * GET
 * https://storage.googleapis.com/storage/v1/b/bucket/notificationConfigs/notification
 * https://cloud.google.com/storage/docs/json_api/v1/notifications/get */
struct OptionalNotification
get_notification_configuration(const struct configuration *config,
                               const char *id) {
  char *path;
  asprintf(&path, "/storage/v1/b/%s/notificationConfigs/%s",
           config->google_bucket_name, id);
  struct ServerResponse response = gcloud_get(NULL, path, NULL);
  DEBUG_SERVER_RESPONSE("get_notification_configuration");
  if (response.status_code == 200 && strlen(response.body) > 0) {
    /* const char *_body = strstr(response.c_str, "\r\n\r\n"); */
    const JSON_Value *created_json_value =
        if_error_exit(json_parse_string(response.body), false);
    const JSON_Object *created_json_object =
        json_value_get_object(created_json_value);

    if (json_object_has_value(created_json_object, "topic")) {
      const struct Notification notification = {
          json_object_get_string(created_json_object, "id"),
          json_object_get_string(created_json_object, "topic"),
          (const char **)json_object_get_array(created_json_object,
                                               "event_types"),
          json_object_get_string(created_json_object, "payload_format"),
          json_object_get_string(created_json_object, "object_name_prefix"),
          json_object_get_string(created_json_object, "etag"),
          json_object_get_string(created_json_object, "selfLink"),
          json_object_get_string(created_json_object, "kind")};
      const struct OptionalNotification optional_notification = {true,
                                                                 notification};
      return optional_notification;
    }
  }
  const struct OptionalNotification optional_notification = {
      false, EMPTY_NOTIFICATION};
  return optional_notification;
}
