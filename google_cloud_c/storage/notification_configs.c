#include <json_common.h>
#include <parson.h>

#include <google_cloud_c/compute/cloud_auth.h>
#include <google_cloud_c/storage/notification_configs.h>

const struct Notification EMPTY_NOTIFICATION = {NULL, NULL, NULL, NULL,
                                                NULL, NULL, NULL, NULL};

struct Notification notification_parse(const JSON_Object *);
struct OptionalNotification
notification_response_handler(struct ServerResponse *);

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
  return notification_response_handler(&response);
}

/* View a Pub/Sub notification configuration on a given bucket.
 * GET
 * https://storage.googleapis.com/storage/v1/b/bucket/notificationConfigs/notification
 * https://cloud.google.com/storage/docs/json_api/v1/notifications/get
 * */
struct OptionalNotification
get_bucket_notification(const char *bucket_name, const char *notification_id) {
  char *path;
  asprintf(&path, "/v1/b/%s/notificationConfigs", bucket_name);
  struct ServerResponse response = gcloud_storage_get(NULL, path, NULL);
  /*free(path);*/
  DEBUG_SERVER_RESPONSE("get_bucket_notification");
  return notification_response_handler(&response);
}

/* Creates a Pub/Sub notification configuration for a given bucket.
 * POST https://storage.googleapis.com/storage/v1/b/bucket/notificationConfigs
 * https://cloud.google.com/storage/docs/json_api/v1/notifications/insert
 * */
struct OptionalNotification insert_bucket_notification(const char *bucket_name,
                                                       const char *topic) {
  char *path, *body;
  asprintf(&path, "/v1/b/%s/notificationConfigs", bucket_name);
  asprintf(&body, "{\"topic\": \"%s\", \"payload_format\": \"JSON_API_V1\"}",
           topic);
  struct ServerResponse response = gcloud_storage_post(NULL, path, body, NULL);
  /*free(path);
  free(body);*/
  DEBUG_SERVER_RESPONSE("insert_bucket_notification");
  return notification_response_handler(&response);
}

/* helper functions */

struct Notification notification_parse(const JSON_Object *created_json_object) {
  const struct Notification notification = {
      json_object_get_string(created_json_object, "id"),
      json_object_get_string(created_json_object, "topic"),
      (const char **)json_object_get_array(created_json_object, "event_types"),
      json_object_get_string(created_json_object, "payload_format"),
      json_object_get_string(created_json_object, "object_name_prefix"),
      json_object_get_string(created_json_object, "etag"),
      json_object_get_string(created_json_object, "selfLink"),
      json_object_get_string(created_json_object, "kind")};
  return notification;
}

struct OptionalNotification
notification_response_handler(struct ServerResponse *response) {
  if ((*response).status_code == 200 && strlen((*response).body) > 0) {
    const JSON_Value *created_json_value =
        if_error_exit(json_parse_string((*response).body), false);
    const JSON_Object *created_json_object =
        json_value_get_object(created_json_value);

    if (json_object_has_value(created_json_object, "topic")) {
      struct Notification notification =
          notification_parse(created_json_object);
      const struct OptionalNotification optional_notification = {true,
                                                                 notification};
      return optional_notification;
    }
  }
  const struct OptionalNotification optional_notification = {
      false, EMPTY_NOTIFICATION};
  return optional_notification;
}
