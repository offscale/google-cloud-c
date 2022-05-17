#include "policy_mocks.h"
#include <stddef.h>

#define EXPR_MOCK0                                                             \
  "{"                                                                          \
  "  \"expression\": \"expression0\","                                         \
  "  \"title\": \"title0\","                                                   \
  "  \"description\": \"description0\","                                       \
  "  \"location\": \"location0\""                                              \
  "}"

#define EXPR_MOCK1                                                             \
  "{"                                                                          \
  "  \"expression\": \"expression1\","                                         \
  "  \"title\": \"title1\","                                                   \
  "  \"description\": \"description1\","                                       \
  "  \"location\": \"location1\""                                              \
  "}"

const char expr_mock0[] = EXPR_MOCK0;
const char expr_mock1[] = EXPR_MOCK1;

const char *expressions_mock[] = {EXPR_MOCK0, EXPR_MOCK1, NULL};

const char bindings_mock0[] = "{"
                              "  \"role\": \"role\","
                              "  " MEMBERS_MOCK0 ","
                              "  \"condition\": [" EXPR_MOCK0 "," EXPR_MOCK1 "]"
                              "}";

#define BINDING_MOCK1                                                          \
  "{"                                                                          \
  "  \"role\": \"role\","                                                      \
  "  \"members\": [\"member0\",\"member1\"],"                                  \
  "  \"condition\": ["                                                         \
  "{"                                                                          \
  "  \"expression\": \"expression0\","                                         \
  "  \"title\": \"title0\","                                                   \
  "  \"description\": \"description0\","                                       \
  "  \"location\": \"location0\""                                              \
  "},"                                                                         \
  "{"                                                                          \
  "  \"expression\": \"expression1\","                                         \
  "  \"title\": \"title1\","                                                   \
  "  \"description\": \"description1\","                                       \
  "  \"location\": \"location1\""                                              \
  "}"                                                                          \
  "]"                                                                          \
  "}"
const char bindings_mock1[] = BINDING_MOCK1;

const char policy_mock0[] = "{\"policy\": {"
                            "  \"version\": \"version\","
                            "  \"bindings\": [" BINDING_MOCK1 "],"
                            "  \"etag\": \"etag\""
                            "}}";
