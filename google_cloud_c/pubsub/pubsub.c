#include <google_cloud_c/pubsub/pubsub.h>

/* Gets the configuration details of a subscription.
 * GET https://pubsub.googleapis.com/v1/{subscription}
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.subscriptions/get
 * */
struct OptionalSubscription get_pubsub_subscription() {
  /* TODO */
  struct Subscription subscription = {NULL};
  struct OptionalSubscription optionalSubscription = {false, subscription};
  return optionalSubscription;
}
