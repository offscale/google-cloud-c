#include <google_cloud_c/common/google_cloud_c_json_common.h>

const char **json_array_to_cstr_array(const JSON_Array *const json_items) {
  const char **cstr_arr = NULL;
  const size_t json_items_n = json_array_get_count(json_items) + 1;
  size_t i;

  if (json_items_n > 1) {
    cstr_arr = malloc(json_items_n * sizeof *cstr_arr);
    for (i = 0; i < json_items_n - 1; i++)
      cstr_arr[i] = json_array_get_string(json_items, i);
    cstr_arr[i] = NULL;
  }
  return cstr_arr;
}
