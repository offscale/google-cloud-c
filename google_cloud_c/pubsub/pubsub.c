#include <parson.h>

#include <json_common.h>

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/pubsub/pubsub.h>

const struct Subscription EMPTY_SUBSCRIPTION = {
    NULL, NULL, 0, 0, NULL, NULL, 0, NULL, 0, 0, NULL, STATE_UNSPECIFIED};

/* Gets the configuration details of a subscription.
 * GET https://pubsub.googleapis.com/v1/{subscription}
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/get
 * */
struct OptionalSubscription
get_pubsub_subscription(const char *subscription_id) {
  char *path;
  asprintf(&path, "/v1/%s", subscription_id);

  {
    struct ServerResponse response = gcloud_pubsub_get(NULL, path, NULL);
    if (response.status_code == 200 && strlen(response.body) > 0) {
      struct OptionalSubscription optionalSubscription = {
          true, subscription_from_json(json_value_get_object(
                    if_error_exit(json_parse_string(response.body), false)))};
      return optionalSubscription;
    } else {
      const struct OptionalSubscription optionalSubscription = {
          false, EMPTY_SUBSCRIPTION};
      return optionalSubscription;
    }
  }
}

/* Creates a subscription to a given topic.
 * PUT https://pubsub.googleapis.com/v1/{name}
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/create
 * */
struct OptionalSubscription
create_pubsub_subscription(const char *subscription_id,
                           struct Subscription subscription) {
  char *path;
  asprintf(&path, "/v1/%s", subscription_id);

  {
    struct ServerResponse response =
        gcloud_pubsub_put(NULL, path, subscription_to_json(subscription), NULL);
    if (response.status_code == 200 && strlen(response.body) > 0) {
      struct OptionalSubscription optionalSubscription = {
          true, subscription_from_json(json_value_get_object(
                    if_error_exit(json_parse_string(response.body), false)))};
      return optionalSubscription;
    } else {
      const struct OptionalSubscription optionalSubscription = {
          false, EMPTY_SUBSCRIPTION};
      return optionalSubscription;
    }
  }
}

/* Deletes an existing subscription. All messages retained in the subscription
 * are immediately dropped.
 * DELETE https://pubsub.googleapis.com/v1/{subscription}
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/delete
 * */
bool delete_pubsub_subscription(const char *subscription_id) {
  char *path;
  asprintf(&path, "/v1/%s", subscription_id);

  {
    struct ServerResponse response =
        gcloud_pubsub_delete(NULL, path, NULL, NULL);
    /* free(path) */
    return response.status_code == 200;
  }
}

/* Utility functions */

enum PubSubState str_to_PubSubState(const char *state) {
  if (strcmp(state, "ACTIVE") == 0)
    return ACTIVE;
  else if (strcmp(state, "RESOURCE_ERROR") == 0)
    return RESOURCE_ERROR;
  else
    return STATE_UNSPECIFIED;
}

const char *PubSubState_to_str(enum PubSubState state) {
  switch (state) {
  case ACTIVE:
    return "ACTIVE";
  case RESOURCE_ERROR:
    return "RESOURCE_ERROR";
  case STATE_UNSPECIFIED:
  default:
    return "STATE_UNSPECIFIED";
  }
}

struct Subscription subscription_from_json(const JSON_Object *jsonObject) {
  struct Subscription subscription;

  subscription.name = json_object_get_string(jsonObject, "name");
  subscription.topic = json_object_get_string(jsonObject, "topic");
  subscription.ackDeadlineSeconds =
      (int)json_object_get_number(jsonObject, "ackDeadlineSeconds");
  subscription.retainAckedMessages =
      (bool)json_object_get_boolean(jsonObject, "retainAckedMessages");
  subscription.messageRetentionDuration =
      json_object_get_string(jsonObject, "messageRetentionDuration");
  subscription.labels =
      (const char **)json_object_get_array(jsonObject, "labels");
  subscription.enableMessageOrdering =
      (bool)json_object_get_boolean(jsonObject, "name");
  subscription.filter = json_object_get_string(jsonObject, "filter");
  subscription.detached = (bool)json_object_get_boolean(jsonObject, "detached");
  subscription.enableExactlyOnceDelivery =
      (bool)json_object_get_boolean(jsonObject, "enableExactlyOnceDelivery");
  subscription.topicMessageRetentionDuration =
      json_object_get_string(jsonObject, "topicMessageRetentionDuration");
  subscription.state =
      str_to_PubSubState(json_object_get_string(jsonObject, "state"));

  return subscription;
}

const char *subscription_to_json(struct Subscription subscription) {
  char *s;
  asprintf(&s,
           "{"
           "  \"name\": \"%s\","
           "  \"topic\": \"%s\","
           "  \"ackDeadlineSeconds\": %d,"
           "  \"retainAckedMessages\": %lu,"
           "  \"messageRetentionDuration\": \"%s\","
           /* "  \"labels\": \"%s\"," */
           "  \"enableMessageOrdering\": %lu,"
           "  \"filter\": \"%s\","
           "  \"detached\": %lu,"
           "  \"enableExactlyOnceDelivery\": %lu,"
           "  \"topicMessageRetentionDuration\": \"%s\","
           "  \"state\": \"%s\""
           "}",
           subscription.name, subscription.topic,
           subscription.ackDeadlineSeconds, subscription.retainAckedMessages,
           subscription.messageRetentionDuration,
           /* subscription.labels, */
           subscription.enableMessageOrdering, subscription.filter,
           subscription.detached, subscription.enableExactlyOnceDelivery,
           subscription.topicMessageRetentionDuration,
           PubSubState_to_str(subscription.state));
  return s;
}
