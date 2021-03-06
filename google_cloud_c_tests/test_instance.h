#ifdef __cplusplus
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __cplusplus */

#include <greatest.h>

#include <google_cloud_c_configure.h>

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

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/compute/instance.h>

TEST x_instance_list_should_be(void) {
  struct Instances instances = instances_list();
  ASSERT_GT(instances.size, 0);
  PASS();
}

TEST x_create_fw_net_instance_should_be(void) {
  struct InstanceIncomplete c_instance = {/*id*/ NULL,
                                          /*creationTimestamp*/ NULL,
                                          /*name*/ "test_instance0",
                                          /*description*/ NULL,
                                          /* machineType */ "n1-standard-1",
                                          /*statusMessage*/ NULL,
                                          /*zone*/ NULL,
                                          /*cpuPlatform*/ NULL,
                                          /*hostname*/ NULL,
                                          /*lastStartTimestamp*/ NULL,
                                          /*lastStopTimestamp*/ NULL,
                                          /*lastSuspendedTimestamp*/ NULL,
                                          /*kind*/ NULL,
                                          /*supportsPzs*/ true};
  struct configuration config;
  config.google_access_token = ACCESS_TOKEN;
  config.google_access_token_expiry = (time_t)ACCESS_TOKEN_EXPIRY;
  config.google_project_id = AUTH_CONTEXT.project_id;
  config.google_region = NULL;
  config.google_zone = NULL;
  config.google_instance_name = NULL;
  config.google_instance_public_ip = NULL;
  config.folder_path = NULL;
  int status;
  {
    struct StatusAndCstrAndCStr createComputeResp =
        create_fw_net_instance(&config, &c_instance,
                               /* shell_script */ NULL);
    status = createComputeResp.status;
    if (status != EXIT_SUCCESS) {
      fprintf(stderr, "createComputeResp.status: %d\n", status);
      status = EXIT_FAILURE;
      cleanup_struct_cstr_cstr(&createComputeResp);
    }
    cleanup_struct_cstr_cstr(&createComputeResp);
  }

  {
    struct Instances instances = instances_list();
    ASSERT_GT(instances.size, 0);
  }
  if (status == EXIT_SUCCESS)
    PASS();
  else
    FAIL();
}

/* Suites can group multiple tests with common setup. */
SUITE(instance_suite) {
  auth();
  set_auth_context(PROJECT_ID, ACCESS_TOKEN);
  RUN_TEST(x_instance_list_should_be);
  RUN_TEST(x_create_fw_net_instance_should_be);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
