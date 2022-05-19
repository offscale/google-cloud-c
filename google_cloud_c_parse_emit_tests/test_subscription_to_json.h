#ifdef __cplusplus
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __cplusplus */

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#define strdup _strdup
#endif /* defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) */

#include <greatest.h>

#include <c89stringutils_string_extras.h>

#include <google_cloud_c_configure.h>

#include "subscription_mocks.h"
#include <google_cloud_c/pubsub/subscription.h>

TEST x_subscription_to_json(void) {
  struct Subscription *subscription = malloc(sizeof(struct Subscription));

  subscription->name = "name";
  subscription->topic = "topic";
  subscription->messageRetentionDuration = "messageRetentionDuration";
  subscription->ackDeadlineSeconds = 0;
  subscription->retainAckedMessages = true;
  subscription->messageRetentionDuration = "messageRetentionDuration";
  subscription->labels = malloc(sizeof(char *) * 2);
  subscription->labels[0] = "foo";
  subscription->labels[1] = NULL;
  subscription->enableMessageOrdering = true;
  subscription->filter = "filter";
  subscription->detached = true;
  subscription->enableExactlyOnceDelivery = true;
  subscription->topicMessageRetentionDuration = "topicMessageRetentionDuration";
  subscription->state = STATE_UNSPECIFIED;

  ASSERT_STR_EQ(subscription_to_json(subscription), subscription_mock0);

  subscription_cleanup(subscription);
  PASS();
}

SUITE(subscription_to_json_suite) { RUN_TEST(x_subscription_to_json); }

#ifdef __cplusplus
}
#endif /* __cplusplus */
