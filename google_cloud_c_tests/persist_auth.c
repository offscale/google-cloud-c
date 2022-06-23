#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#include <io.h>
#define access _access
#define strdup _strdup
#define F_OK EXIT_SUCCESS
#define NUM_FORMAT "lld"
#else
#include <unistd.h>
#define NUM_FORMAT "ld"
#endif /* defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) */

#include <string.h>
/*#include <cauthflow_server.h>
#include "cauthflow_types_common.h"*/
#include <google_auth.h>

#include "google_cloud_c_configure.h"
#include "persist_auth.h"

const char *REFRESH_TOKEN = NULL;
const char *ACCESS_TOKEN = NULL;
time_t ACCESS_TOKEN_EXPIRY = 0;
const char *PROJECT_ID = NULL;

const char *get_file_contents(const char *);

/*struct StrStr {
  const char *first, *second;
};*/

struct StrStr mocked_redirect_dance(const char *s, const char *h) {
  const struct StrStr str_str = {s, h};
  return str_str;
}

extern struct GoogleCloudProject mocked_get_google_auth(
    const char *client_id, const char *client_secret, const char *refresh_token,
    struct StrStr (*redirect_handler)(const char *, const char *)) {
  struct GoogleCloudProject project;
  project.google_access_token = NULL;
  project.google_access_token_expiry = 0;
  project.google_refresh_token = NULL;
  project.projectId = client_id;
  project.name = client_secret;
  project.lifecycleState = refresh_token;
  project.projectNumber = NULL;
  redirect_handler(client_id, refresh_token);
  return project;
}

void auth() {
#define REFRESH_FILEPATH "refresh_token.txt"
#define ACCESS_FILEPATH "access_token.txt"
#define ACCESS_TOKEN_EXPIRY_FILEPATH "access_token_expiry.txt"
#define PROJECT_ID_FILEPATH "project_id.txt"
  if (REFRESH_TOKEN != NULL && ACCESS_TOKEN != NULL && PROJECT_ID != NULL)
    return;
  else if (access(REFRESH_FILEPATH, F_OK) == 0) {
    REFRESH_TOKEN = strdup(get_file_contents(REFRESH_FILEPATH));
    PROJECT_ID = strdup(get_file_contents(PROJECT_ID_FILEPATH));
    /* just delete the access token on auth error and try again,
     * no need for proper handling dance */
    if (access(ACCESS_FILEPATH, F_OK) == 0) {
      ACCESS_TOKEN = strdup(get_file_contents(ACCESS_FILEPATH));
      char *end;
      ACCESS_TOKEN_EXPIRY = strtol(
          strdup(get_file_contents(ACCESS_TOKEN_EXPIRY_FILEPATH)), &end, 10);
    } else {
      struct GoogleCloudProject google_cloud_project = mocked_get_google_auth(
          CLIENT_ID, CLIENT_SECRET, REFRESH_TOKEN, mocked_redirect_dance);
      FILE *fh;
#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
      fopen_s(&fh, ACCESS_FILEPATH, "wt");
#else
      fh = fopen(ACCESS_FILEPATH, "wt");
#endif /* defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) */
      fputs(google_cloud_project.google_access_token, fh);
      fclose(fh);
    }
  } else {
    struct GoogleCloudProject google_cloud_project = mocked_get_google_auth(
        CLIENT_ID, CLIENT_SECRET,
        /*refresh_token*/ REFRESH_TOKEN, mocked_redirect_dance);

    FILE *fh;
#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    fopen_s(&fh, REFRESH_FILEPATH, "wt");
#else
    fh = fopen(REFRESH_FILEPATH, "wt");
#endif /* defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) */

    fputs(google_cloud_project.google_refresh_token, fh);
    fclose(fh);
    REFRESH_TOKEN = google_cloud_project.google_refresh_token;

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    fopen_s(&fh, ACCESS_FILEPATH, "wt");
#else
    fh = fopen(ACCESS_FILEPATH, "wt");
#endif /* defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) */
    fputs(google_cloud_project.google_access_token, fh);
    fclose(fh);
    ACCESS_TOKEN = google_cloud_project.google_access_token;

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    fopen_s(&fh, ACCESS_TOKEN_EXPIRY_FILEPATH, "wt");
#else
    fh = fopen(ACCESS_TOKEN_EXPIRY_FILEPATH, "wt");
#endif /* defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) */
    fprintf(fh, "%" NUM_FORMAT,
            google_cloud_project.google_access_token_expiry);
    fclose(fh);
    ACCESS_TOKEN_EXPIRY = google_cloud_project.google_access_token_expiry;

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    fopen_s(&fh, PROJECT_ID_FILEPATH, "wt");
#else
    fh = fopen(PROJECT_ID_FILEPATH, "wt");
#endif /* defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) */
    fputs(google_cloud_project.projectId, fh);
    fclose(fh);
    PROJECT_ID = google_cloud_project.projectId;
  }
#undef REFRESH_FILEPATH
#undef ACCESS_FILEPATH
#undef ACCESS_TOKEN_EXPIRY_FILEPATH
#undef PROJECT_ID_FILEPATH
}

const char *get_file_contents(const char *filepath) {
#define TOKEN_LEN 512
  FILE *fh;
#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
  fopen_s(&fh, filepath, "rt");
#else
  fh = fopen(filepath, "rt");
#endif /* defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) */
  char token[TOKEN_LEN];
  char *tok = fgets(token, TOKEN_LEN, fh);
  fclose(fh);
  return tok;
#undef TOKEN_LEN
}
