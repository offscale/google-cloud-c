#ifndef GOOGLE_CLOUD_C_PUBSUB_POLICY_H
#define GOOGLE_CLOUD_C_PUBSUB_POLICY_H

#ifdef __cplusplus
#include <cstdlib>
extern "C" {
#else
#include <stdlib.h>
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __STDC_VERSION__ >= 199901L */
#endif /* __cplusplus */

#include <parson.h>

#include <google_cloud_c/pubsub/google_cloud_c_pubsub_export.h>

/* Structures */

/* https://cloud.google.com/pubsub/docs/reference/rest/v1/Policy#expr
 * Represents a textual expression in the Common Expression Language (CEL)
 * syntax.
 * */
struct Expr {
  const char *expression, *title, *description, *location;
};

/* https://cloud.google.com/pubsub/docs/reference/rest/v1/Policy#binding
 * Associates `members`, or principals, with a `role`.
 * */
struct Binding {
  const char *role;
  const char **members;
  struct Expr *condition;
};

/* https://cloud.google.com/pubsub/docs/reference/rest/v1/Policy
 * An Identity and Access Management (IAM) policy, which specifies access
 * controls for Google Cloud resources. */
struct Policy {
  const char *version;
  struct Binding *bindings;
  const char *etag;
};

struct OptionalPolicy {
  bool set;
  struct Policy policy;
};

enum RequestedPolicyVersion {
  POLICY_VERSION_0,
  POLICY_VERSION_1,
  POLICY_VERSION_3 = 3
};

struct GetPolicyOptions {
  enum RequestedPolicyVersion requestedPolicyVersion;
};

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT const struct Policy policyNull;

/* Routes */

/* Gets the access control policy for a resource. Returns an empty policy if the
 * resource exists and does not have a policy set.
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/getIamPolicy
 * */
extern GOOGLE_CLOUD_C_PUBSUB_EXPORT struct OptionalPolicy
getIamPolicy(const char *, struct GetPolicyOptions *);

/* Sets the access control policy on the specified resource. Replaces any
 * existing policy.
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/setIamPolicy
 * */
extern GOOGLE_CLOUD_C_PUBSUB_EXPORT struct OptionalPolicy
setIamPolicy(const char *, struct Policy *);

/* utility functions */

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT struct Expr
expr_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT const char *
expr_to_json(const struct Expr *);

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT struct Binding
bindings_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT const char *
bindings_to_json(const struct Binding *);

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT struct Policy
policy_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT const char *
policy_to_json(const struct Policy *);

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT const char *
GetPolicyOptions_to_json(struct GetPolicyOptions *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_PUBSUB_POLICY_H */
