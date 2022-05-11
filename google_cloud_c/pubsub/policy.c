#include <stdlib.h>

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/pubsub/policy.h>

const struct Policy policyNull = {NULL, NULL, NULL};

/* Routes */

/* GET https://pubsub.googleapis.com/v1/{resource}:getIamPolicy
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/getIamPolicy
 * */
struct OptionalPolicy getIamPolicy(const char *resource,
                                   struct GetPolicyOptions *getPolicyOptions) {
  char *path;
  asprintf(&path, "/v1/%s:getIamPolicy", resource);
  {
    CURLU *urlp = curl_url();

    if (getPolicyOptions != NULL)
      IS_CURLUE_OK(curl_url_set(urlp, CURLUPART_QUERY,
                                GetPolicyOptions_to_json(getPolicyOptions), 0));

    {
      const struct ServerResponse response =
          gcloud_pubsub_get(urlp, path, NULL);
      struct OptionalPolicy optionalPolicy;
      DEBUG_SERVER_RESPONSE("getIamPolicy");
      if (response.status_code == 200 && response.body != NULL &&
          response.body[0] != '\0')
        optionalPolicy.set = true,
        optionalPolicy.policy = policy_from_json(
            json_value_get_object(json_parse_string(response.body)));
      else
        optionalPolicy.set = true, optionalPolicy.policy = policyNull;
      return optionalPolicy;
    }
  }
}

/* POST https://pubsub.googleapis.com/v1/{resource}:setIamPolicy
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/setIamPolicy
 */
struct OptionalPolicy setIamPolicy(const char *resource,
                                   struct Policy *policy) {
  char *path;
  asprintf(&path, "/v1/%s:setIamPolicy", resource);
  {
    const struct ServerResponse response = gcloud_pubsub_post(
        NULL, path, policy == NULL ? NULL : policy_to_json(policy), NULL);
    struct OptionalPolicy optionalPolicy;
    DEBUG_SERVER_RESPONSE("setIamPolicy");
    if (response.status_code == 200 && response.body != NULL &&
        response.body[0] != '\0')
      optionalPolicy.set = true,
      optionalPolicy.policy = policy_from_json(
          json_value_get_object(json_parse_string(response.body)));
    else
      optionalPolicy.set = false, optionalPolicy.policy = policyNull;
    return optionalPolicy;
  }
}

/* utility functions */

struct Expr expr_from_json(const JSON_Object *jsonObject) {
  struct Expr expr;
  expr.expression = json_object_get_string(jsonObject, "expression");
  expr.title = json_object_get_string(jsonObject, "title");
  expr.description = json_object_get_string(jsonObject, "description");
  expr.title = json_object_get_string(jsonObject, "location");
  return expr;
}

const char *expr_to_json(const struct Expr *expr) {
  char *s = NULL;
  jasprintf(&s, "{");
  if (expr->expression != NULL && strlen(expr->expression) > 0)
    jasprintf(&s, "  \"expression\": \"%s\",", expr->expression);
  if (expr->title != NULL && strlen(expr->title) > 0)
    jasprintf(&s, "  \"title\": \"%s\",", expr->title);
  if (expr->description != NULL && strlen(expr->description) > 0)
    jasprintf(&s, "  \"description\": \"%s\",", expr->description);
  if (expr->location != NULL && strlen(expr->location) > 0)
    jasprintf(&s, "  \"location\": \"%s\",", expr->location);
  jasprintf(&s, "\0");
  s[strlen(s) - 1] = '}';
  return s;
}

struct Binding bindings_from_json(const JSON_Object *jsonObject) {
  struct Binding bindings;

  bindings.role = json_object_get_string(jsonObject, "role");

  bindings.members = NULL;
  if (json_object_has_value_of_type(jsonObject, "members", JSONArray)) {
    const JSON_Array *members_json_items =
        json_object_get_array(jsonObject, "members");
    const size_t members_json_items_n =
        json_array_get_count(members_json_items);
    size_t i;

    if (members_json_items_n > 0) {
      bindings.members =
          (const char **)malloc(members_json_items_n * sizeof(const char *));
      for (i = 0; i < members_json_items_n; i++)
        bindings.members[i] = json_array_get_string(members_json_items, i);
    }
  }

  bindings.condition = NULL;
  if (json_object_has_value_of_type(jsonObject, "condition", JSONArray)) {
    const JSON_Array *expr_json_items =
        json_object_get_array(jsonObject, "condition");
    const size_t expr_json_items_n = json_array_get_count(expr_json_items);
    size_t i;

    if (expr_json_items_n > 0) {
      bindings.condition =
          (struct Expr *)malloc(expr_json_items_n * sizeof(struct Expr));
      for (i = 0; i < expr_json_items_n; i++)
        bindings.condition[i] =
            expr_from_json(json_array_get_object(expr_json_items, i));
    }
  }

  return bindings;
}

const char *bindings_to_json(const struct Binding *binding) {
  char *s = NULL;
  size_t n;
  jasprintf(&s, "{");
  if (binding->role != NULL && strlen(binding->role) > 0)
    jasprintf(&s, "  \"role\": \"%s\",", binding->role);
  if (binding->members != NULL) {
    char *member;
    jasprintf(&s, "  \"members\": [");
    for (member = (char *)binding->members; member != NULL; member++)
      jasprintf(&s, "\"%s\",", member);
    jasprintf(&s, "  \"members\": [");
    n = strlen(s);
    if (s[n - 1] == ',') /* `if` to handle empty array */
      s[n - 1] = ']';
    else
      jasprintf(&s, "]");
  }
  if (binding->condition) {
    struct Expr *expr;
    jasprintf(&s, "  \"condition\": [");
    for (expr = binding->condition; expr != NULL; expr++)
      jasprintf(&s, "%s,", expr_to_json(expr));
    n = strlen(s);
    if (s[n - 1] == ',') /* `if` to handle empty array */
      s[n - 1] = ']';
    else
      jasprintf(&s, "]");
  }
  jasprintf(&s, "\0");
  s[strlen(s) - 1] = '}';
  return s;
}

struct Policy policy_from_json(const JSON_Object *jsonObject) {
  struct Policy policy;

  policy.version = json_object_get_string(jsonObject, "version");

  policy.bindings = NULL;
  if (json_object_has_value_of_type(jsonObject, "bindings", JSONArray)) {
    const JSON_Array *bindings_json_items =
        json_object_get_array(jsonObject, "bindings");
    const size_t bindings_json_items_n =
        json_array_get_count(bindings_json_items);
    size_t i;

    if (bindings_json_items_n > 0) {
      policy.bindings = (struct Binding *)malloc(bindings_json_items_n *
                                                 sizeof(struct Policy));
      for (i = 0; i < bindings_json_items_n; i++)
        policy.bindings[i] =
            bindings_from_json(json_array_get_object(bindings_json_items, i));
    }
  }

  policy.etag = json_object_get_string(jsonObject, "etag");

  return policy;
}

const char *policy_to_json(const struct Policy *policy) {
  char *s = NULL;
  size_t n;
  jasprintf(&s, "{");
  if (policy->version != NULL && strlen(policy->version) > 0)
    jasprintf(&s, "  \"version\": \"%s\",", policy->version);
  if (policy->bindings != NULL) {
    struct Binding *binding;
    jasprintf(&s, "  \"bindings\": [");
    for (binding = policy->bindings; binding != NULL; binding++)
      jasprintf(&s, "%s,", bindings_to_json(binding));
    n = strlen(s);
    if (s[n - 1] == ',') /* `if` to handle empty array */
      s[n - 1] = ']';
    else
      jasprintf(&s, "]");
  }
  if (policy->etag != NULL && strlen(policy->etag) > 0)
    jasprintf(&s, "  \"etag\": \"%s\",", policy->etag);
  jasprintf(&s, "\0");
  s[strlen(s) - 1] = '}';
  return s;
}

const char *
GetPolicyOptions_to_json(struct GetPolicyOptions *getPolicyOptions) {
  enum { n = 29 };
  static char requestedPolicyVersion[n] = "{\"requestedPolicyVersion\": \0}";
  requestedPolicyVersion[n - 2] =
      (char)getPolicyOptions->requestedPolicyVersion;
  return requestedPolicyVersion;
}
