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

#include <google_cloud_c/pubsub/policy.h>

#include "policy_mocks.h"

TEST x_expr_to_json(void) {
  struct Expr *expr = malloc(sizeof *expr);
  expr->description = strdup("description0"),
  expr->expression = strdup("expression0"),
  expr->location = strdup("location0"), expr->title = strdup("title0");
  ASSERT_STR_EQ(expr_to_json(expr), expr_mock0);
  expr_cleanup(expr);
  PASS();
}

TEST x_GetPolicyOptions_to_json(void) {
  struct GetPolicyOptions *getPolicyOptions = malloc(sizeof *getPolicyOptions);
  getPolicyOptions->requestedPolicyVersion = POLICY_VERSION_1;
  ASSERT_STR_EQ(GetPolicyOptions_to_json(getPolicyOptions),
                "{\"requestedPolicyVersion\": 1}");
  free(getPolicyOptions);
  PASS();
}

TEST x_bindings_to_json(void) {
  struct Binding *binding = malloc(sizeof *binding);
  binding->role = strdup("role"), binding->members = NULL,
  binding->condition = NULL;

  ASSERT_STR_EQ(bindings_to_json(binding), "{  \"role\": \"role\"}");

  binding->members = malloc(3 * sizeof binding->members);
  binding->members[0] = strdup("member0"),
  binding->members[1] = strdup("member1"), binding->members[2] = NULL;

  ASSERT_STR_EQ(bindings_to_json(binding),
                "{  " ROLE_MOCK0 ",  " MEMBERS_MOCK0 "}");

  binding->condition = malloc(3 * sizeof binding->condition);

  binding->condition[0] = malloc(sizeof *binding->condition[0]);
  binding->condition[0]->description = strdup("description0"),
  binding->condition[0]->expression = strdup("expression0"),
  binding->condition[0]->location = strdup("location0"),
  binding->condition[0]->title = strdup("title0");

  binding->condition[1] = malloc(sizeof *binding->condition[1]);
  binding->condition[1]->description = strdup("description1"),
  binding->condition[1]->expression = strdup("expression1"),
  binding->condition[1]->location = strdup("location1"),
  binding->condition[1]->title = strdup("title1");

  binding->condition[2] = NULL;

  ASSERT_STR_EQ(bindings_to_json(binding), bindings_mock0);

  bindings_cleanup(binding);
  PASS();
}

TEST x_policy_to_json(void) {
  struct Policy *policy = malloc(sizeof *policy);
  struct Binding *binding = malloc(sizeof *binding);

  binding->role = strdup("role");

  binding->members = malloc(3 * sizeof binding->members);
  binding->members[0] = strdup("member0"),
  binding->members[1] = strdup("member1"), binding->members[2] = NULL;

  binding->condition = malloc(3 * sizeof binding->condition);

  binding->condition[0] = malloc(sizeof *binding->condition[0]);
  binding->condition[0]->description = strdup("description0"),
  binding->condition[0]->expression = strdup("expression0"),
  binding->condition[0]->location = strdup("location0"),
  binding->condition[0]->title = strdup("title0");

  binding->condition[1] = malloc(sizeof *binding->condition[1]);
  binding->condition[1]->description = strdup("description1"),
  binding->condition[1]->expression = strdup("expression1"),
  binding->condition[1]->location = strdup("location1"),
  binding->condition[1]->title = strdup("title1");

  binding->condition[2] = NULL;

  ASSERT_STR_EQ(bindings_to_json(binding), bindings_mock0);

  policy->etag = strdup("etag"), policy->version = strdup("version");

  policy->bindings = malloc(2 * sizeof policy->bindings);
  policy->bindings[0] = binding;
  policy->bindings[1] = NULL;

  ASSERT_STR_EQ(policy_to_json(policy), policy_mock0);

  policy_cleanup(policy);
  PASS();
}

SUITE(policy_to_json_suite) {
  RUN_TEST(x_expr_to_json);
  RUN_TEST(x_GetPolicyOptions_to_json);
  RUN_TEST(x_bindings_to_json);
  RUN_TEST(x_policy_to_json);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
