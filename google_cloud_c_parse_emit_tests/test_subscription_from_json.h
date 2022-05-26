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

TEST x_receivedMessages_from_json(void) {
  struct ReceivedMessage **receivedMessagesMock =
      malloc(3 * sizeof **receivedMessagesMock);
  static const char mock[] = "{\n"
                             "  \"receivedMessages\": [\n"
                             "    {\n"
                             "       \"ackId\": \"ackId0\",\n"
                             "       \"message\": {\n"
                             "           \"data\": \"data0\",\n"
                             /*"           \"attributes\": {\n"
                             "             string: string,\n"
                             "           },\n"*/
                             "           \"messageId\": \"messageId0\",\n"
                             "           \"publishTime\": \"publishTime0\",\n"
                             "           \"orderingKey\": \"orderingKey0\"\n"
                             "       },\n"
                             "       \"deliveryAttempt\": 0\n"
                             "    },\n"
                             "    {\n"
                             "       \"ackId\": \"ackId1\",\n"
                             "       \"message\": {\n"
                             "           \"data\": \"data1\",\n"
                             /*"           \"attributes\": {\n"
                             "             string: string,\n"
                             "           },\n"*/
                             "           \"messageId\": \"messageId1\",\n"
                             "           \"publishTime\": \"publishTime1\",\n"
                             "           \"orderingKey\": \"orderingKey1\"\n"
                             "       },\n"
                             "       \"deliveryAttempt\": 0\n"
                             "    }\n"
                             "  ]\n"
                             "}";
  struct ReceivedMessages *receivedMessages =
      receivedMessages_from_json(json_object(json_parse_string(mock)));

  receivedMessagesMock[0] = malloc(sizeof *receivedMessagesMock[0]);
  receivedMessagesMock[0]->ackId = strdup("ackId0");
  receivedMessagesMock[0]->message =
      malloc(sizeof *receivedMessagesMock[0]->message);
  receivedMessagesMock[0]->message->data = strdup("data0");
  receivedMessagesMock[0]->message->messageId = strdup("messageId0");
  receivedMessagesMock[0]->message->orderingKey = strdup("orderingKey0");
  receivedMessagesMock[0]->message->publishTime = strdup("publishTime0");
  receivedMessagesMock[0]->deliveryAttempt = 0;

  receivedMessagesMock[1] = malloc(sizeof *receivedMessagesMock[1]);
  receivedMessagesMock[1]->ackId = strdup("ackId1");
  receivedMessagesMock[1]->message =
      malloc(sizeof *receivedMessagesMock[1]->message);
  receivedMessagesMock[1]->message->data = strdup("data1");
  receivedMessagesMock[1]->message->messageId = strdup("messageId1");
  receivedMessagesMock[1]->message->orderingKey = strdup("orderingKey1");
  receivedMessagesMock[1]->message->publishTime = strdup("publishTime1");
  receivedMessagesMock[1]->deliveryAttempt = 0;

  receivedMessagesMock[2] = NULL;

  {
    struct ReceivedMessage *receivedMessage, *receivedMessageMock;
    size_t i;
    for (i = 0; receivedMessages->receivedMessages[i]; ++i) {
      receivedMessage = receivedMessages->receivedMessages[i],
      receivedMessageMock = receivedMessagesMock[i];

      ASSERT_STR_EQ(receivedMessageMock->ackId, receivedMessage->ackId);
      ASSERT_STR_EQ(receivedMessageMock->message->publishTime,
                    receivedMessage->message->publishTime);
      ASSERT_STR_EQ(receivedMessageMock->message->orderingKey,
                    receivedMessage->message->orderingKey);
      ASSERT_STR_EQ(receivedMessageMock->message->messageId,
                    receivedMessage->message->messageId);
      ASSERT_STR_EQ(receivedMessageMock->message->data,
                    receivedMessage->message->data);
      ASSERT_EQ(receivedMessageMock->deliveryAttempt,
                receivedMessage->deliveryAttempt);

      receivedMessage_cleanup(receivedMessage);
      receivedMessage_cleanup(receivedMessageMock);
    }
    ASSERT_EQ(receivedMessages->receivedMessages[i], NULL);
    ASSERT_EQ(receivedMessagesMock[i], NULL);
    receivedMessage_cleanup(receivedMessages->receivedMessages[i]);
    receivedMessage_cleanup(receivedMessagesMock[i]);
  }

  PASS();
}

SUITE(subscription_from_json_suite) {
  RUN_TEST(x_subscription_from_json);
  RUN_TEST(x_receivedMessages_from_json);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
