#include <parson.h>

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/pubsub/subscription.h>

const struct Subscription subscriptionNull = {
    NULL, NULL, 0, 0, NULL, NULL, 0, NULL, 0, 0, NULL, STATE_UNSPECIFIED};

const struct PubsubMessage pubsubMessageNull = {NULL, NULL, NULL, NULL};

/* Gets the configuration details of a subscription.
 * GET https://pubsub.googleapis.com/v1/{subscription}
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/get
 * */
struct OptionalSubscription
get_pubsub_subscription(const char *const subscription_id) {
  char *path;
  asprintf(&path, "/v1/%s", subscription_id);

  {
    const struct ServerResponse response = gcloud_pubsub_get(NULL, path, NULL);
    struct OptionalSubscription optionalSubscription;
    DEBUG_SERVER_RESPONSE("get_pubsub_subscription_response");
    if (response.status_code == 200 && response.body != NULL &&
        response.body[0] != '\0')
      optionalSubscription.set = true,
      optionalSubscription.subscription = subscription_from_json(
          json_value_get_object(json_parse_string(response.body)));
    else
      optionalSubscription.set = false,
      optionalSubscription.subscription = subscriptionNull;
    return optionalSubscription;
  }
}

/* Creates a subscription to a given topic.
 * PUT https://pubsub.googleapis.com/v1/{name}
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/create
 * */
struct OptionalSubscription
create_pubsub_subscription(const char *const subscription_id,
                           const struct Subscription *subscription) {
  char *path;
  asprintf(&path, "/v1/%s", subscription_id);

  {
    const struct ServerResponse response =
        gcloud_pubsub_put(NULL, path, subscription_to_json(subscription), NULL);
    struct OptionalSubscription optionalSubscription;
    DEBUG_SERVER_RESPONSE("create_pubsub_subscription_response");
    if (response.status_code == 200 && response.body != NULL &&
        response.body[0] != '\0')
      optionalSubscription.set = true,
      optionalSubscription.subscription = subscription_from_json(
          json_value_get_object(json_parse_string(response.body)));
    else
      optionalSubscription.set = false,
      optionalSubscription.subscription = subscriptionNull;
    return optionalSubscription;
  }
}

/* Pulls messages from the server.
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/pull
 * POST https://pubsub.googleapis.com/v1/{subscription}:pull
 * */
struct ReceivedMessages
pull_pubsub_subscription(const char *const subscription_id) {
  char *path;
  asprintf(&path, "/v1/%s:pull", subscription_id);

  {
    const struct ServerResponse response =
        gcloud_pubsub_post(NULL, path, NULL, NULL);
    DEBUG_SERVER_RESPONSE("pull_pubsub_subscription_response");
    return receivedMessages_from_json(
        json_value_get_object(json_parse_string(response.body)));
  }
}

/* Acknowledges the messages associated with the ackIds in the
 * AcknowledgeRequest. The Pub/Sub system can remove the relevant messages from
 * the subscription.
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/acknowledge
 * POST https://pubsub.googleapis.com/v1/{subscription}:acknowledge
 * */
bool acknowledge_pubsub_subscription(const char *const subscription_id,
                                     const struct AckIds *ack_ids) {
  char *path;
  asprintf(&path, "/v1/%s", subscription_id);

  {
    const struct ServerResponse response =
        gcloud_pubsub_post(NULL, path, AckIds_to_json_str(ack_ids), NULL);
    DEBUG_SERVER_RESPONSE("acknowledge_pubsub_subscription_response");
    /* free(path) */
    return response.status_code == 200;
  }
}

/* Deletes an existing subscription. All messages retained in the subscription
 * are immediately dropped.
 * DELETE https://pubsub.googleapis.com/v1/{subscription}
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/delete
 * */
bool delete_pubsub_subscription(const char *const subscription_id) {
  char *path;
  asprintf(&path, "/v1/%s", subscription_id);

  {
    const struct ServerResponse response =
        gcloud_pubsub_delete(NULL, path, NULL, NULL);
    DEBUG_SERVER_RESPONSE("delete_pubsub_subscription_response");
    /* free(path) */
    return response.status_code == 200;
  }
}

/* Utility functions */

enum PubSubState str_to_PubSubState(const char *const state) {
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

struct Subscription
subscription_from_json(const JSON_Object *const jsonObject) {
  struct Subscription subscription;

  subscription.name = json_object_get_string(jsonObject, "name");
  subscription.topic = json_object_get_string(jsonObject, "topic");
  if (json_object_has_value_of_type(jsonObject, "ackDeadlineSeconds",
                                    JSONNumber))
    subscription.ackDeadlineSeconds =
        (int)json_object_get_number(jsonObject, "ackDeadlineSeconds");
  else
    subscription.ackDeadlineSeconds = 0;
  if (json_object_has_value_of_type(jsonObject, "retainAckedMessages",
                                    JSONBoolean))
    subscription.retainAckedMessages =
        (bool)json_object_get_boolean(jsonObject, "retainAckedMessages");
  else
    subscription.retainAckedMessages = false;
  subscription.messageRetentionDuration =
      json_object_get_string(jsonObject, "messageRetentionDuration");
  if (json_object_has_value_of_type(jsonObject, "labels", JSONArray))
    subscription.labels =
        (const char **)json_object_get_array(jsonObject, "labels");
  else
    subscription.labels = NULL;
  if (json_object_has_value_of_type(jsonObject, "enableMessageOrdering",
                                    JSONBoolean))
    subscription.enableMessageOrdering =
        (bool)json_object_get_boolean(jsonObject, "enableMessageOrdering");
  else
    subscription.enableMessageOrdering = false;
  subscription.filter = json_object_get_string(jsonObject, "filter");
  if (json_object_has_value_of_type(jsonObject, "detached", JSONBoolean))
    subscription.detached =
        (bool)json_object_get_boolean(jsonObject, "detached");
  else
    subscription.detached = false;
  if (json_object_has_value_of_type(jsonObject, "enableExactlyOnceDelivery",
                                    JSONBoolean))
    subscription.enableExactlyOnceDelivery =
        (bool)json_object_get_boolean(jsonObject, "enableExactlyOnceDelivery");
  else
    subscription.enableExactlyOnceDelivery = false;
  subscription.topicMessageRetentionDuration =
      json_object_get_string(jsonObject, "topicMessageRetentionDuration");
  if (json_object_has_value(jsonObject, "state"))
    subscription.state =
        str_to_PubSubState(json_object_get_string(jsonObject, "state"));
  else
    subscription.state = STATE_UNSPECIFIED;

  return subscription;
}

const char *subscription_to_json(const struct Subscription *subscription) {
  char *s = NULL;

  jasprintf(&s, "{");
  if (subscription->name != NULL && subscription->name[0] != '\0')
    jasprintf(&s, "  \"name\": \"%s\",", subscription->name);
  if (subscription->topic != NULL && subscription->topic[0] != '\0')
    jasprintf(&s, "  \"topic\": \"%s\",", subscription->topic);
  if (subscription->ackDeadlineSeconds)
    jasprintf(&s, "  \"ackDeadlineSeconds\": %d,",
              subscription->ackDeadlineSeconds);
  if (subscription->retainAckedMessages)
    jasprintf(&s, "  \"retainAckedMessages\": %lu,",
              subscription->retainAckedMessages);
  if (subscription->messageRetentionDuration != NULL &&
      subscription->messageRetentionDuration[0] != '\0')
    jasprintf(&s, "  \"messageRetentionDuration\": \"%s\",",
              subscription->messageRetentionDuration);
  /* "  \"labels\": \"%s\"," */
  jasprintf(&s, "  \"enableMessageOrdering\": \"%s\",",
            subscription->enableMessageOrdering);
  if (subscription->filter != NULL && subscription->filter[0] != '\0')
    jasprintf(&s, "  \"filter\": \"%s\",", subscription->filter);
  jasprintf(&s,
            "  \"detached\": %lu,"
            "  \"enableExactlyOnceDelivery\": %lu,",
            subscription->detached, subscription->enableExactlyOnceDelivery);
  if (subscription->topicMessageRetentionDuration != NULL &&
      subscription->topicMessageRetentionDuration[0] != '\0')
    jasprintf(&s, "  \"topicMessageRetentionDuration\": \"%s\",",
              subscription->topicMessageRetentionDuration);
  jasprintf(&s, "  \"state\": \"%s\"}",
            PubSubState_to_str(subscription->state));

  return s;
}

struct ReceivedMessages
receivedMessages_from_json(const JSON_Object *const jsonObject) {
  struct ReceivedMessages receivedMessages = {NULL};
  const JSON_Array *json_items =
      json_object_get_array(jsonObject, "receivedMessages");
  const size_t json_items_n = json_array_get_count(json_items);
  size_t i;
  if (json_items_n > 0) {
    receivedMessages.receivedMessages = (struct ReceivedMessage *)malloc(
        json_items_n * sizeof(struct ReceivedMessage));
    for (i = 0; i < json_items_n; i++) {
      const JSON_Object *const json_obj = json_array_get_object(json_items, i);
      struct ReceivedMessage receivedMessage;
      struct PubsubMessage pubsubMessage = pubsubMessageNull;
      const JSON_Object *const messageObject =
          json_object_get_object(json_obj, "message");

      const bool pubsubMessage_contents =
          json_object_has_value(messageObject, "data") ||
          json_object_has_value(messageObject, "messageId") ||
          json_object_has_value(messageObject, "orderingKey") ||
          json_object_has_value(messageObject, "publishTime");
      if (pubsubMessage_contents) {
        pubsubMessage.data = json_object_get_string(messageObject, "data");
        pubsubMessage.messageId =
            json_object_get_string(messageObject, "messageId");
        pubsubMessage.orderingKey =
            json_object_get_string(messageObject, "orderingKey");
        pubsubMessage.publishTime =
            json_object_get_string(messageObject, "publishTime");
      }
      receivedMessage.message = pubsubMessage;

      receivedMessage.ackId = json_object_get_string(json_obj, "ackId");
      if (json_object_has_value_of_type(json_obj, "deliveryAttempt",
                                        JSONNumber))
        receivedMessage.deliveryAttempt =
            (int)json_object_get_number(json_obj, "deliveryAttempt");
      else
        receivedMessage.deliveryAttempt = 0;

      receivedMessages.receivedMessages[i] = receivedMessage;
    }
  }
  return receivedMessages;
}

const char *AckIds_to_json_str(const struct AckIds *ack_ids) {
  char *s = NULL, *w;
  size_t n;
  jasprintf(&s, "{\"ackIds\": [");

  for (w = (char *)ack_ids->ackIds; w != NULL; w++)
    jasprintf(&s, "\"%s\",", w);
  jasprintf(&s, "\0\0");
  n = strlen(s);
  s[n - 2] = ']';
  s[n - 1] = '}';
  return s;
}
