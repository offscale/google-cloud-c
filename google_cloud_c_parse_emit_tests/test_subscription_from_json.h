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

#include <google_cloud_c/pubsub/subscription.h>

#include "subscription_mocks.h"

TEST x_subscription_from_json(void) {
  JSON_Value *subscription_val = json_parse_string(subscription_mock0);
  const JSON_Object *subscription_json =
      json_value_get_object(subscription_val);
  struct Subscription *subscription = subscription_from_json(subscription_json);

  ASSERT_STR_EQ(subscription->name, "name");
  ASSERT_STR_EQ(subscription->topic, "topic");
  ASSERT_STR_EQ(subscription->messageRetentionDuration,
                "messageRetentionDuration");
  ASSERT_EQ(subscription->ackDeadlineSeconds, 0);
  ASSERT_EQ(subscription->retainAckedMessages, true);
  ASSERT_STR_EQ(subscription->messageRetentionDuration,
                "messageRetentionDuration");
  ASSERT(subscription->labels == NULL);
  /*ASSERT_STR_EQ(subscription->labels[0], "foo");
  ASSERT(subscription->labels[1] == NULL);*/
  ASSERT_EQ(subscription->enableMessageOrdering, true);
  ASSERT_STR_EQ(subscription->filter, "filter");
  ASSERT_EQ(subscription->detached, true);
  ASSERT_EQ(subscription->enableExactlyOnceDelivery, true);
  ASSERT_STR_EQ(subscription->topicMessageRetentionDuration,
                "topicMessageRetentionDuration");
  ASSERT_EQ(subscription->state, STATE_UNSPECIFIED);

  subscription_cleanup(subscription);
  PASS();
}

SUITE(subscription_from_json_suite) { RUN_TEST(x_subscription_from_json); }

#ifdef __cplusplus
}
#endif /* __cplusplus */
