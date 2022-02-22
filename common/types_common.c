#include "types_common.h"

const struct StatusAndArrayCStrArray StatusAndArrayCStrArrayNull = {EXIT_FAILURE, NULL, 0};

const struct StatusAndCstrAndCStr StatusAndCstrAndCStrNull = {EXIT_FAILURE, NULL, 0, NULL, 0};

const struct StatusAndCstr StatusAndCstrNull = {EXIT_FAILURE, NULL, 0};

void cleanup_struct_cstr_array(struct StatusAndArrayCStrArray *status_and_array) {
    if (status_and_array->c_str_arr != NULL) {
        free((void *) status_and_array->c_str_arr);
        status_and_array->size = 0;
    }
}

void cleanup_struct_cstr_cstr(struct StatusAndCstrAndCStr *status_and_cstr_and_cstr) {
    if (status_and_cstr_and_cstr->c_str0 != NULL) {
        free((void *) status_and_cstr_and_cstr->c_str0);
        status_and_cstr_and_cstr->size0 = 0;
    }
    if (status_and_cstr_and_cstr->c_str1 != NULL) {
        free((void *) status_and_cstr_and_cstr->c_str1);
        status_and_cstr_and_cstr->size1 = 0;
    }
}

void cleanup_struct_cstr(struct StatusAndCstr *status_and_cstr) {
    if (status_and_cstr->c_str != NULL) {
        free((void *) status_and_cstr->c_str);
        status_and_cstr->size = 0;
    }
}

void cleanup_struct_array(struct ErrorOrIntArray *status_and_array) {
    if (status_and_array->array != NULL) {
        free((void *) status_and_array->array);
        status_and_array->size = 0;
    }
}

void cleanup_struct_configuration(struct configuration *config) {
    if (config == NULL) return;
    /* Hmm, everything is `strdup` so is heap-allocated, so this should work? */
    free((char*)config->google_access_token);
    /* free((char*)config->google_access_token_expiry); */
    free((char*)config->google_refresh_token);
    free((char*)config->google_project_id);
    free((char*)config->google_bucket_name);
    free((char*)config->google_region);
    free((char*)config->google_zone);
    free((char*)config->google_instance_name);
    free((char*)config->google_instance_public_ip);
    free((char*)config->folder_path);
    free(config);
}
