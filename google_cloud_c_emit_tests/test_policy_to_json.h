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

TEST x_expr_to_json(void) {
  struct Expr *expr = (struct Expr *)malloc(sizeof(struct Expr) * 1);
  expr->description = strdup("description"),
  expr->expression = strdup("expression"), expr->location = strdup("location"),
  expr->title = strdup("title");
  ASSERT_STR_EQ(
      expr_to_json(expr),
      "{"
      "  \"expression\": \"expression\",  \"title\": \"title\","
      "  \"description\": \"description\",  \"location\": \"location\""
      "}");
  expr_cleanup(expr);
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
  binding->role = strdup("role"), binding->members = NULL,
  binding->condition = NULL;

  ASSERT_STR_EQ(bindings_to_json(binding), "{  \"role\": \"role\"}");

  binding->members = malloc(sizeof(char *) * 3);
  binding->members[0] = strdup("member0"),
  binding->members[1] = strdup("member1"), binding->members[2] = NULL;

  ASSERT_STR_EQ(
      bindings_to_json(binding),
      "{  \"role\": \"role\",  \"members\": [\"member0\",\"member1\"]}");

  binding->condition = (struct Expr **)malloc(sizeof(struct Expr) * 3);

  binding->condition[0] = malloc(sizeof(struct Expr) * 1);
  binding->condition[0]->description = strdup("description0"),
  binding->condition[0]->expression = strdup("expression0"),
  binding->condition[0]->location = strdup("location0"),
  binding->condition[0]->title = strdup("title0");

  binding->condition[1] = malloc(sizeof(struct Expr) * 1);
  binding->condition[1]->description = strdup("description1"),
  binding->condition[1]->expression = strdup("expression1"),
  binding->condition[1]->location = strdup("location1"),
  binding->condition[1]->title = strdup("title1");

  binding->condition[2] = NULL;

  ASSERT_STR_EQ(
      bindings_to_json(binding),
      "{  \"role\": \"role\",  \"members\": [\"member0\",\"member1\"],  "
      "\"condition\": [{  \"expression\": \"expression0\",  \"title\": "
      "\"title0\",  \"description\": \"description0\",  \"location\": "
      "\"location0\"},{  \"expression\": \"expression1\",  \"title\": "
      "\"title1\",  \"description\": \"description1\",  \"location\": "
      "\"location1\"}]}");

  bindings_cleanup(binding);
  PASS();
}

TEST x_policy_to_json(void) {
  struct Policy *policy = (struct Policy *)malloc(sizeof(struct Policy) * 1);
  struct Binding *binding =
      (struct Binding *)malloc(sizeof(struct Binding) * 1);

  binding->role = strdup("role");

  binding->members = malloc(sizeof(char *) * 3);
  binding->members[0] = strdup("member0"),
  binding->members[1] = strdup("member1"), binding->members[2] = NULL;

  binding->condition = (struct Expr **)malloc(sizeof(struct Expr) * 3);

  binding->condition[0] = malloc(sizeof(struct Expr) * 1);
  binding->condition[0]->description = strdup("description0"),
  binding->condition[0]->expression = strdup("expression0"),
  binding->condition[0]->location = strdup("location0"),
  binding->condition[0]->title = strdup("title0");

  binding->condition[1] = malloc(sizeof(struct Expr) * 1);
  binding->condition[1]->description = strdup("description1"),
  binding->condition[1]->expression = strdup("expression1"),
  binding->condition[1]->location = strdup("location1"),
  binding->condition[1]->title = strdup("title1");

  binding->condition[2] = NULL;

  ASSERT_STR_EQ(
      bindings_to_json(binding),
      "{  \"role\": \"role\",  \"members\": [\"member0\",\"member1\"],  "
      "\"condition\": [{  \"expression\": \"expression0\",  \"title\": "
      "\"title0\",  \"description\": \"description0\",  \"location\": "
      "\"location0\"},{  \"expression\": \"expression1\",  \"title\": "
      "\"title1\",  \"description\": \"description1\",  \"location\": "
      "\"location1\"}]}");

  policy->etag = strdup("etag"), policy->version = strdup("version");

  policy->bindings = malloc(sizeof(struct Binding *) * 2);
  policy->bindings[0] = binding;
  policy->bindings[1] = NULL;

  ASSERT_STR_EQ(policy_to_json(policy),
                "{\"policy\": {"
                "  \"version\": \"version\","
                "  \"bindings\": [{"
                "  \"role\": \"role\","
                "  \"members\": [\"member0\",\"member1\"],"
                "  \"condition\": ["
                "{"
                "  \"expression\": \"expression0\","
                "  \"title\": \"title0\","
                "  \"description\": \"description0\","
                "  \"location\": \"location0\"},"
                "{"
                "  \"expression\": \"expression1\","
                "  \"title\": \"title1\","
                "  \"description\": \"description1\","
                "  \"location\": \"location1\"}]}],"
                "  \"etag\": \"etag\"}}");

  policy_cleanup(policy);
  PASS();
}

SUITE(parse_emit_policy_suite) {
  RUN_TEST(x_expr_to_json);
  RUN_TEST(x_GetPolicyOptions_to_json);
  RUN_TEST(x_bindings_to_json);
  RUN_TEST(x_policy_to_json);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
