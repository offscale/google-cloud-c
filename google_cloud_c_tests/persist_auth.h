#ifndef GOOGLE_CLOUD_C_TESTS_PERSIST_AUTH_H
#define GOOGLE_CLOUD_C_TESTS_PERSIST_AUTH_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <sys/types.h>

extern const char *REFRESH_TOKEN;
extern const char *ACCESS_TOKEN;
extern const char *PROJECT_ID;
extern time_t ACCESS_TOKEN_EXPIRY;

extern void auth();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_TESTS_PERSIST_AUTH_H */
