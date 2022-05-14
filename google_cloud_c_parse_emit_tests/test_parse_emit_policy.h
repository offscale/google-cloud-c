#ifdef __cplusplus
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __cplusplus */

#include <greatest.h>

#include <c89stringutils_string_extras.h>

#include <google_cloud_c_configure.h>

#include <google_cloud_c/pubsub/policy.h>

TEST x_expr_to_json(void) {
  struct Expr *expr = (struct Expr *)malloc(sizeof(struct Expr) * 1);
  expr->description = "description", expr->expression = "expression",
  expr->location = "location", expr->title = "title";
  ASSERT_STR_EQ(
      expr_to_json(expr),
      "{"
      "  \"expression\": \"expression\",  \"title\": \"title\","
      "  \"description\": \"description\",  \"location\": \"location\""
      "}");
  free(expr);
  PASS();
}

TEST x_GetPolicyOptions_to_json(void) {
  struct GetPolicyOptions *getPolicyOptions =
      (struct GetPolicyOptions *)malloc(sizeof(struct GetPolicyOptions) * 1);
  getPolicyOptions->requestedPolicyVersion = POLICY_VERSION_1;
  ASSERT_STR_EQ(GetPolicyOptions_to_json(getPolicyOptions),
                "{\"requestedPolicyVersion\": 1}");
  free(getPolicyOptions);
  PASS();
}

TEST x_bindings_to_json(void) {
  struct Binding *binding =
      (struct Binding *)malloc(sizeof(struct Binding) * 1);
  binding->role = "role", binding->members = NULL, binding->condition = NULL;

  ASSERT_STR_EQ(bindings_to_json(binding), "{  \"role\": \"role\"}");

  binding->members = malloc(sizeof(char *) * 3);
  binding->members[0] = "member0", binding->members[1] = "member1",
  binding->members[2] = NULL;

  ASSERT_STR_EQ(
      bindings_to_json(binding),
      "{  \"role\": \"role\",  \"members\": [\"member0\",\"member1\"]}");

  binding->condition = (struct Expr **)malloc(sizeof(struct Expr) * 3);

  binding->condition[0] = malloc(sizeof(struct Expr) * 1);
  binding->condition[0]->description = "description0",
  binding->condition[0]->expression = "expression0",
  binding->condition[0]->location = "location0",
  binding->condition[0]->title = "title0";

  binding->condition[1] = malloc(sizeof(struct Expr) * 1);
  binding->condition[1]->description = "description1",
  binding->condition[1]->expression = "expression1",
  binding->condition[1]->location = "location1",
  binding->condition[1]->title = "title1";

  binding->condition[2] = NULL;

  ASSERT_STR_EQ(
      bindings_to_json(binding),
      "{  \"role\": \"role\",  \"members\": [\"member0\",\"member1\"],  "
      "\"condition\": [{  \"expression\": \"expression0\",  \"title\": "
      "\"title0\",  \"description\": \"description0\",  \"location\": "
      "\"location0\"},{  \"expression\": \"expression1\",  \"title\": "
      "\"title1\",  \"description\": \"description1\",  \"location\": "
      "\"location1\"}]}");

  free(binding->members);
  free(binding->condition[0]);
  free(binding->condition[1]);
  free(binding->condition);
  free(binding);
  PASS();
}

SUITE(parse_emit_policy_suite) {
  RUN_TEST(x_expr_to_json);
  RUN_TEST(x_GetPolicyOptions_to_json);
  RUN_TEST(x_bindings_to_json);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
