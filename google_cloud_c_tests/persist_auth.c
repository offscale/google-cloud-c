#include "persist_auth.h"
#include "google_cloud_c_configure.h"
#include <cauthflow_server.h>
#include <google_auth.h>
#include <stdio.h>
#include <unistd.h>

const char *REFRESH_TOKEN = NULL;
const char *ACCESS_TOKEN = NULL;
const char *PROJECT_ID = NULL;

const char *get_file_contents(const char *);

void auth() {
#define REFRESH_FILEPATH "refresh_token.txt"
#define ACCESS_FILEPATH "access_token.txt"
#define PROJECT_ID_FILEPATH "project_id.txt"
  if (REFRESH_TOKEN != NULL && ACCESS_TOKEN != NULL && PROJECT_ID != NULL)
    return;
  else if (access(REFRESH_FILEPATH, F_OK) == 0) {
    REFRESH_TOKEN = strdup(get_file_contents(REFRESH_FILEPATH));
    PROJECT_ID = strdup(get_file_contents(PROJECT_ID_FILEPATH));
    /* just delete the access token on auth error and try again,
     * no need for proper handling dance */
    if (access(ACCESS_FILEPATH, F_OK) == 0)
      ACCESS_TOKEN = strdup(get_file_contents(ACCESS_FILEPATH));
    else {
      struct GoogleCloudProject google_cloud_project = get_google_auth(
          CLIENT_ID, CLIENT_SECRET, REFRESH_TOKEN, redirect_dance);
      FILE *fh = fopen(ACCESS_FILEPATH, "wt");
      fputs(google_cloud_project.google_access_token, fh);
      fclose(fh);
    }
  } else {
    struct GoogleCloudProject google_cloud_project = get_google_auth(
        CLIENT_ID, CLIENT_SECRET, /*refresh_token*/ NULL, redirect_dance);

    FILE *fh = fopen(REFRESH_FILEPATH, "wt");
    fputs(google_cloud_project.google_refresh_token, fh);
    fclose(fh);
    REFRESH_TOKEN = google_cloud_project.google_refresh_token;

    fh = fopen(ACCESS_FILEPATH, "wt");
    fputs(google_cloud_project.google_access_token, fh);
    fclose(fh);
    ACCESS_TOKEN = google_cloud_project.google_access_token;

    fh = fopen(PROJECT_ID_FILEPATH, "wt");
    fputs(google_cloud_project.projectId, fh);
    fclose(fh);
    PROJECT_ID = google_cloud_project.projectId;
  }
#undef REFRESH_FILEPATH
#undef ACCESS_FILEPATH
#undef PROJECT_ID_FILEPATH
}

const char *get_file_contents(const char *filepath) {
#define TOKEN_LEN 512
  FILE *fh = fopen(filepath, "rt");
  char token[TOKEN_LEN];
  char *tok = fgets(token, TOKEN_LEN, fh);
  fclose(fh);
  return tok;
#undef TOKEN_LEN
}
