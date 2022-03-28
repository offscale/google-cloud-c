#ifndef GOOGLE_CLOUD_C_PUBSUB_PUBSUB_H
#define GOOGLE_CLOUD_C_PUBSUB_PUBSUB_H

#ifdef __cplusplus
#include <cstdbool>
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __cplusplus */

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

struct OptionalSubscription {
  bool set;
  struct Subscription subscription;
};

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT const struct Subscription
    EMPTY_SUBSCRIPTION;

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT struct OptionalSubscription
get_pubsub_subscription(const char *);

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT struct OptionalSubscription
create_pubsub_subscription(const char *, struct Subscription);

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT bool
delete_pubsub_subscription(const char *subscription_id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GOOGLE_CLOUD_C_PUBSUB_PUBSUB_H */
