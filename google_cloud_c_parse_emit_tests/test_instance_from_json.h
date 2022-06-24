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

#include <google_cloud_c/compute/instance.h>

#include "instance_mocks.h"

TEST x_instance_from_json(void) {
  const JSON_Value *const instance_val = json_parse_string(instance_mock0);
  const JSON_Object *const instance_json = json_value_get_object(instance_val);
  const struct Instance *const instance = instance_from_json(instance_json);

  ASSERT_STR_EQ(instance->kind, "compute#instance");
  /*
  ASSERT_STR_EQ(instance->instanceession, "instanceession0");
  ASSERT_STR_EQ(instance->location, "location0");
  ASSERT_STR_EQ(instance->title, "title0");*/
  /*instance_cleanup(instance);*/
  /*json_value_free(instance_val);*/

  PASS();
}

SUITE(instance_from_json_suite) {
  RUN_TEST(x_instance_from_json);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
