#include <google_cloud_c_configure.h>

#include <greatest.h>
#include <stdbool.h>

#include "persist_auth.h"

#ifdef _MSC_VER
#define NUM_FORMAT "%zu"
typedef size_t num_type;
#elif defined(__linux__) || defined(linux) || defined(__linux)
#define NUM_FORMAT "%d"
typedef int num_type;
#else
#define NUM_FORMAT "%lu"
typedef unsigned long num_type;
#endif /* _MSC_VER */

#include <compute/cloud_auth.h>
#include <compute/instance.h>

TEST x_instance_list_should_be(void) {
  /* TODO: Actually test things */
  auth();
  set_auth_context(PROJECT_ID, ACCESS_TOKEN);
  {
    struct Instances instances = instances_list();
    PASS();
  }
}

/* Suites can group multiple tests with common setup. */
SUITE(instance_suite) { RUN_TEST(x_instance_list_should_be); }
