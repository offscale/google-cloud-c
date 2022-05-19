#ifndef GOOGLE_CLOUD_C_PUBSUB_PUBSUB_H
#define GOOGLE_CLOUD_C_PUBSUB_PUBSUB_H

#ifdef __cplusplus
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __cplusplus */

#include <parson.h>

#include <google_cloud_c/pubsub/google_cloud_c_pubsub_export.h>

enum PubSubState { STATE_UNSPECIFIED, ACTIVE, RESOURCE_ERROR };

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT enum PubSubState
str_to_PubSubState(const char *);

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT const char *
    PubSubState_to_str(enum PubSubState);

/*
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions#Subscription
 */
struct Subscription {
  const char *name;
  const char *topic;
  /* "pushConfig": {
    object (PushConfig)
  } */
  int ackDeadlineSeconds;
  bool retainAckedMessages;
  const char *messageRetentionDuration;
  const char **labels;
  bool enableMessageOrdering;
  /* "expirationPolicy": {
    object (ExpirationPolicy)
  } */

  const char *filter;
  /* "deadLetterPolicy": {
    object (DeadLetterPolicy)
  }
   "retryPolicy": {
    object (RetryPolicy)
  }
   */
  bool detached;
  bool enableExactlyOnceDelivery;
  const char *topicMessageRetentionDuration;
  enum PubSubState state;
};

/* https://cloud.google.com/pubsub/docs/reference/rest/v1/PubsubMessage */
struct PubsubMessage {
  const char *data, *messageId, *publishTime, *orderingKey;
  /* "attributes": {
      string: string,
      ...
    }, */
};

/* https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/pull#ReceivedMessage
 */
struct ReceivedMessage {
  const char *ackId;
  struct PubsubMessage *message;
  int deliveryAttempt;
};

struct ReceivedMessages {
  struct ReceivedMessage **receivedMessages;
};

struct AckIds {
  const char *const *ackIds;
};

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT const char *
AckIds_to_json_str(const struct AckIds *);

/* Gets the configuration details of a subscription.
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/get
 */
extern GOOGLE_CLOUD_C_PUBSUB_EXPORT struct Subscription *
get_pubsub_subscription(const char *);

/* Creates a subscription to a given topic.
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/create
 */
extern GOOGLE_CLOUD_C_PUBSUB_EXPORT struct Subscription *
create_pubsub_subscription(const char *, const struct Subscription *);

/* Pulls messages from the server.
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/pull
 * */
extern GOOGLE_CLOUD_C_PUBSUB_EXPORT struct ReceivedMessages *
pull_pubsub_subscription(const char *subscription_id);

/* Acknowledges the messages associated with the ackIds in the
 * AcknowledgeRequest. The Pub/Sub system can remove the relevant messages from
 * the subscription.
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/acknowledge
 * */
extern GOOGLE_CLOUD_C_PUBSUB_EXPORT bool
acknowledge_pubsub_subscription(const char *subscription_id,
                                const struct AckIds *ack_ids);

/* Deletes an existing subscription. All messages retained in the subscription
 * are immediately dropped.
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/delete
 * */
extern GOOGLE_CLOUD_C_PUBSUB_EXPORT bool
delete_pubsub_subscription(const char *subscription_id);

/* utility functions */

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT struct Subscription *
subscription_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT const char *
subscription_to_json(const struct Subscription *);

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT void
subscription_cleanup(struct Subscription *);

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT struct ReceivedMessages *
receivedMessages_from_json(const JSON_Object *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_PUBSUB_PUBSUB_H */
