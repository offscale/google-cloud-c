#include <stdlib.h>

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/pubsub/policy.h>

const struct Policy policyNull = {NULL, NULL, NULL};

/* Routes */

/* GET https://pubsub.googleapis.com/v1/{resource}:getIamPolicy
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/getIamPolicy
 * */
struct OptionalPolicy
getIamPolicy(const char *const resource,
             const struct GetPolicyOptions *getPolicyOptions) {
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
struct OptionalPolicy setIamPolicy(const char *const resource,
                                   const struct Policy *policy) {
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

const char *
GetPolicyOptions_to_json(const struct GetPolicyOptions *getPolicyOptions) {
  enum { n = 30 };
  char *requestedPolicyVersion = malloc(sizeof(char) * n);
  memcpy(requestedPolicyVersion, "{\"requestedPolicyVersion\": \0}", n);
  requestedPolicyVersion[n - 1] = '\0';
  switch (getPolicyOptions->requestedPolicyVersion) {
  case POLICY_VERSION_0:
    requestedPolicyVersion[n - 3] = '0';
    break;
  case POLICY_VERSION_1:
    requestedPolicyVersion[n - 3] = '1';
    break;
  case POLICY_VERSION_3:
  default:
    requestedPolicyVersion[n - 3] = '3';
  }
  return requestedPolicyVersion;
}

struct Expr expr_from_json(const JSON_Object *const jsonObject) {
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
  if (expr->expression != NULL && expr->expression[0] != '\0')
    jasprintf(&s, "  \"expression\": \"%s\",", expr->expression);
  if (expr->title != NULL && expr->title[0] != '\0')
    jasprintf(&s, "  \"title\": \"%s\",", expr->title);
  if (expr->description != NULL && expr->description[0] != '\0')
    jasprintf(&s, "  \"description\": \"%s\",", expr->description);
  if (expr->location != NULL && expr->location[0] != '\0')
    jasprintf(&s, "  \"location\": \"%s\",", expr->location);
  jasprintf(&s, "\0");
  s[strlen(s) - 1] = '}';
  return s;
}

void expr_cleanup(struct Expr *expr) {
  free((void *)expr->title);
  expr->title = NULL;
  free((void *)expr->expression);
  expr->expression = NULL;
  free((void *)expr->description);
  expr->description = NULL;
  free((void *)expr->location);
  expr->location = NULL;
}

struct Binding bindings_from_json(const JSON_Object *const jsonObject) {
  struct Binding bindings;

  bindings.role = json_object_get_string(jsonObject, "role");

  bindings.members = NULL;
  if (json_object_has_value_of_type(jsonObject, "members", JSONArray)) {
    const JSON_Array *members_json_items =
        json_object_get_array(jsonObject, "members");
    const size_t members_json_items_n =
        json_array_get_count(members_json_items) + 1;
    size_t i;

    if (members_json_items_n > 1) {
      bindings.members = (const char **)malloc(members_json_items_n *
                                               sizeof(const char *const));
      for (i = 0; i < members_json_items_n - 1; i++)
        bindings.members[i] = json_array_get_string(members_json_items, i);
      bindings.members[members_json_items_n] = NULL;
    }
  }

  bindings.condition = NULL;
  if (json_object_has_value_of_type(jsonObject, "condition", JSONArray)) {
    const JSON_Array *expr_json_items =
        json_object_get_array(jsonObject, "condition");
    const size_t expr_json_items_n = json_array_get_count(expr_json_items) + 1;
    size_t i;

    if (expr_json_items_n > 1) {
      bindings.condition =
          (struct Expr **)malloc(expr_json_items_n * sizeof(struct Expr *));
      for (i = 0; i < expr_json_items_n - 1; i++) {
        struct Expr expr =
            expr_from_json(json_array_get_object(expr_json_items, i));
        bindings.condition[i] = &expr;
      }
      bindings.condition[expr_json_items_n] = NULL;
    }
  }

  return bindings;
}

const char *bindings_to_json(const struct Binding *binding) {
  char *s = NULL;
  size_t n;
  jasprintf(&s, "{");
  if (binding->role != NULL && binding->role[0] != '\0')
    jasprintf(&s, "  \"role\": \"%s\",", binding->role);
  if (binding->members != NULL) {
    const char **member;
    jasprintf(&s, "  \"members\": [");
    for (member = binding->members; *member; member++)
      jasprintf(&s, "\"%s\",", *member);

    n = strlen(s);
    s[n - 1] = ']';
    jasprintf(&s, "}");
  }
  if (binding->condition != NULL) {
    struct Expr **expr;
    n = strlen(s);
    if (s[n - 1] == '}')
      s[n - 1] = ',';
    jasprintf(&s, "  \"condition\": [");
    for (expr = binding->condition; *expr; expr++)
      jasprintf(&s, "%s,", expr_to_json(*expr));
    n = strlen(s);
    s[n - 1] = ']';
    jasprintf(&s, "}");
  }
  jasprintf(&s, "\0");
  s[strlen(s) - 1] = '}';
  return s;
}

void bindings_cleanup(struct Binding *binding) {
  free((void *)binding->role);
  binding->role = NULL;

  {
    const char **member;
    for (member = binding->members; *member; member++)
      free((void *)*member);
    binding->members = NULL;
  }

  {
    struct Expr **expr;
    for (expr = binding->condition; *expr; expr++)
      expr_cleanup(*expr);
    binding->condition = NULL;
  }
}

struct Policy policy_from_json(const JSON_Object *const jsonObject) {
  struct Policy policy;

  policy.version = json_object_get_string(jsonObject, "version");

  policy.bindings = NULL;
  if (json_object_has_value_of_type(jsonObject, "bindings", JSONArray)) {
    const JSON_Array *bindings_json_items =
        json_object_get_array(jsonObject, "bindings");
    const size_t bindings_json_items_n =
        json_array_get_count(bindings_json_items) + 1;
    size_t i;

    if (bindings_json_items_n > 1) {
      policy.bindings = (struct Binding **)malloc(bindings_json_items_n *
                                                  sizeof(struct Policy *));
      for (i = 0; i < bindings_json_items_n - 1; i++) {
        struct Binding binding =
            bindings_from_json(json_array_get_object(bindings_json_items, i));
        policy.bindings[i] = &binding;
      }
      policy.bindings[bindings_json_items_n] = NULL;
    }
  }

  policy.etag = json_object_get_string(jsonObject, "etag");

  return policy;
}

const char *policy_to_json(const struct Policy *policy) {
  char *s = NULL;
  size_t n;
  jasprintf(&s, "{");
  if (policy->version != NULL && policy->version[0] != '\0')
    jasprintf(&s, "  \"version\": \"%s\",", policy->version);
  if (policy->bindings != NULL) {
    struct Binding **binding;
    jasprintf(&s, "  \"bindings\": [");
    for (binding = policy->bindings; *binding; binding++)
      jasprintf(&s, "%s,", bindings_to_json(*binding));
    n = strlen(s);
    if (s[n - 1] == ',') /* `if` to handle empty array */
      s[n - 1] = ']';
    else
      jasprintf(&s, "]");
  }
  if (policy->etag != NULL && policy->etag[0] != '\0') {
    n = strlen(s);
    if (n > 1)
      jasprintf(&s, ",");
    jasprintf(&s, "  \"etag\": \"%s\",", policy->etag);
  }
  jasprintf(&s, "\0");
  s[strlen(s) - 1] = '}';
  return s;
}

void policy_cleanup(struct Policy *policy) {
  struct Binding **binding;
  for (binding = policy->bindings; *binding; binding++)
    bindings_cleanup(*binding);
  policy->bindings = NULL;

  free((void *)policy->version);
  policy->version = NULL;

  free((void *)policy->etag);
  policy->etag = NULL;
}
