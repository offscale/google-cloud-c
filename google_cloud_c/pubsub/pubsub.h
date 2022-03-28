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

struct Subscription {
  const char *topic;
  /* TODO:
   * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions#Subscription
   */
};

struct OptionalSubscription {
  bool set;
  struct Subscription subscription;
};

struct OptionalSubscription GOOGLE_CLOUD_C_PUBSUB_EXPORT
get_pubsub_subscription();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GOOGLE_CLOUD_C_PUBSUB_PUBSUB_H */
