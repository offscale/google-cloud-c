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

TEST x_expr_from_json(void) {
  JSON_Value *expr_val = json_parse_string(expr_mock0);
  const JSON_Object *expr_json = json_value_get_object(expr_val);
  struct Expr *expr = expr_from_json(expr_json);

  ASSERT_STR_EQ(expr->description, "description0");
  ASSERT_STR_EQ(expr->expression, "expression0");
  ASSERT_STR_EQ(expr->location, "location0");
  ASSERT_STR_EQ(expr->title, "title0");
  expr_cleanup(expr);
  /*json_value_free(expr_val);*/

  PASS();
}

TEST x_bindings_from_json(void) {
  JSON_Value *binding_val = json_parse_string(bindings_mock0);
  const JSON_Object *binding_json = json_value_get_object(binding_val);
  struct Binding *binding = bindings_from_json(binding_json);

  ASSERT_STR_EQ(binding->role, "role");

  {
    const char **member0, **member1, *members[] = {"member0", "member1"};
    for (member0 = binding->members, member1 = members; *member0;
         member0++, member1++)
      ASSERT_STR_EQ(*member0, *member1);
  }

  {
    struct Expr **expr;
    const char **expr_mock;
    for (expr = binding->condition, expr_mock = expressions_mock; *expr;
         expr++, expr_mock++)
      ASSERT_STR_EQ(expr_to_json(*expr), *expr_mock);
  }

  /*bindings_cleanup(binding);*/
  json_value_free(binding_val);
  PASS();
}

TEST x_policy_from_json(void) {
  JSON_Value *policy_val = json_parse_string(policy_mock0);
  const JSON_Object *policy_json = json_value_get_object(policy_val);
  struct Policy *policy = policy_from_json(policy_json);

  ASSERT_STR_EQ(policy->version, "version");

  {
    struct Binding **binding;
    for (binding = policy->bindings; *binding; binding++) {
      ASSERT_STR_EQ((**binding).role, "role");

      {
        const char **member0, **member1, *members[] = {"member0", "member1"};
        for (member0 = (**binding).members, member1 = members; *member0;
             member0++, member1++)
          ASSERT_STR_EQ(*member0, *member1);
      }

      {
        struct Expr **expr;
        const char **expr_mock;
        for (expr = (**binding).condition, expr_mock = expressions_mock; *expr;
             expr++, expr_mock++)
          ASSERT_STR_EQ(expr_to_json(*expr), *expr_mock);
      }
    }
  }

  ASSERT_STR_EQ(policy->etag, "etag");

  policy_cleanup(policy);
  PASS();
}

SUITE(policy_from_json_suite) {
  RUN_TEST(x_expr_from_json);
  RUN_TEST(x_bindings_from_json);
  RUN_TEST(x_policy_from_json);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
