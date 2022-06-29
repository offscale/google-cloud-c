#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#endif /* _MSC_VER */

#include <greatest.h>

#include "test_instance_from_json.h"
#include "test_instance_to_json.h"
#include "test_policy_from_json.h"
#include "test_policy_to_json.h"
#include "test_subscription_from_json.h"
#include "test_subscription_to_json.h"

/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(policy_to_json_suite);
  RUN_SUITE(policy_from_json_suite);
  RUN_SUITE(subscription_to_json_suite);
  RUN_SUITE(subscription_from_json_suite);
  RUN_SUITE(instance_from_json_suite);
  RUN_SUITE(instance_to_json_suite);
  GREATEST_MAIN_END();
}
