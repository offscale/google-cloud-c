#include <google_cloud_c/pubsub/pubsub.h>

const struct Subscription EMPTY_SUBSCRIPTION = {
    NULL, NULL, 0, 0, NULL, NULL, 0, NULL, 0, 0, NULL, STATE_UNSPECIFIED};

/* Gets the configuration details of a subscription.
 * GET https://pubsub.googleapis.com/v1/{subscription}
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/get
 * */
struct OptionalSubscription get_pubsub_subscription() {
  /* TODO */
  struct OptionalSubscription optionalSubscription = {false,
                                                      EMPTY_SUBSCRIPTION};
  return optionalSubscription;
}
