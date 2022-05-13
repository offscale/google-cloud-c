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
  expr->description = "description";
  expr->expression = "expression";
  expr->location = "location";
  expr->title = "title";
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
  struct GetPolicyOptions *getPolicyOptions = (struct GetPolicyOptions *)malloc(sizeof(struct GetPolicyOptions) * 1);
  getPolicyOptions->requestedPolicyVersion = POLICY_VERSION_1;
  ASSERT_STR_EQ(
      GetPolicyOptions_to_json(getPolicyOptions),
      "{\"requestedPolicyVersion\": 1}"
      );
  free(getPolicyOptions);
  PASS();
}

TEST x_bindings_to_json(void) {
  struct Binding *binding = (struct Binding *)malloc(sizeof(struct Binding) * 1);
  binding->role = "role";
      ASSERT_STR_EQ(
      bindings_to_json(binding),
      "{\"requestedPolicyVersion\": 1}"
  );
  binding->members = NULL;
  binding->condition = NULL;
  printf("bindings_to_json() = \"%s\"\n", bindings_to_json(binding));
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
