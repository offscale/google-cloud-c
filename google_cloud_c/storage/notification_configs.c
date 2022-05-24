#include <parson.h>

#include <c89stringutils_string_extras.h>

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/storage/notification_configs.h>

struct Notification *
notification_response_handler(const struct ServerResponse *);

/* View a Pub/Sub notification configuration on a given bucket
 * GET
 * https://storage.googleapis.com/storage/v1/b/bucket/notificationConfigs/notification
 * https://cloud.google.com/storage/docs/json_api/v1/notifications/get */
struct Notification *
get_notification_configuration(const struct configuration *config,
                               const char *const id) {
  char *path;
  asprintf(&path, "/storage/v1/b/%s/notificationConfigs/%s",
           config->google_bucket_name, id);
  {
    const struct ServerResponse response = gcloud_get(NULL, path, NULL);
    DEBUG_SERVER_RESPONSE("get_notification_configuration");
    return notification_response_handler(&response);
  }
}

/* View a Pub/Sub notification configuration on a given bucket.
 * GET
 * https://storage.googleapis.com/storage/v1/b/bucket/notificationConfigs/notification
 * https://cloud.google.com/storage/docs/json_api/v1/notifications/get
 * */
struct Notification *
get_bucket_notification(const char *const bucket_name,
                        const char *const notification_id) {
  char *path;
  asprintf(&path, "/storage/v1/b/%s/notificationConfigs/%s", bucket_name,
           notification_id);
  {
    const struct ServerResponse response = gcloud_storage_get(NULL, path, NULL);
    /*free(path);*/
    DEBUG_SERVER_RESPONSE("get_bucket_notification");
    return notification_response_handler(&response);
  }
}

/* Creates a Pub/Sub notification configuration for a given bucket.
 * POST https://storage.googleapis.com/storage/v1/b/bucket/notificationConfigs
 * https://cloud.google.com/storage/docs/json_api/v1/notifications/insert
 * */
struct Notification *insert_bucket_notification(const char *const bucket_name,
                                                const char *const topic) {
  char *path, *body;
  asprintf(&path, "/storage/v1/b/%s/notificationConfigs", bucket_name);
  asprintf(&body, "{\"topic\": \"%s\", \"payload_format\": \"JSON_API_V1\"}",
           topic);
  {
    const struct ServerResponse response =
        gcloud_storage_post(NULL, path, body, NULL);
    /*free(path);
    free(body);*/
    DEBUG_SERVER_RESPONSE("insert_bucket_notification");
    return notification_response_handler(&response);
  }
}

/* helper functions */

struct Notification *
notification_from_json(const JSON_Object *const created_json_object) {
  struct Notification *notification = malloc(sizeof *notification);

  notification->id = json_object_get_string(created_json_object, "id");
  notification->topic = json_object_get_string(created_json_object, "topic");
  if (json_object_has_value_of_type(created_json_object, "event_types",
                                    JSONArray))
    notification->event_types = (const char **)json_object_get_array(
        created_json_object, "event_types");
  else
    notification->event_types = NULL;
  notification->payload_format =
      json_object_get_string(created_json_object, "payload_format");
  notification->object_name_prefix =
      json_object_get_string(created_json_object, "object_name_prefix");
  notification->etag = json_object_get_string(created_json_object, "etag");
  notification->selfLink =
      json_object_get_string(created_json_object, "selfLink");
  notification->kind = json_object_get_string(created_json_object, "kind");

  return notification;
}

struct Notification *
notification_response_handler(const struct ServerResponse *const response) {
  if ((*response).status_code == 200 && (*response).body[0] != '\0') {
    const JSON_Value *const created_json_value =
        json_parse_string((*response).body);
    const JSON_Object *const created_json_object =
        json_value_get_object(created_json_value);

    if (json_object_has_value(created_json_object, "topic"))
      return notification_from_json(created_json_object);
  }

  return NULL;
}
