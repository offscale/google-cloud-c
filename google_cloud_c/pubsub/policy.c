#include <stdlib.h>

#include <c89stringutils_string_extras.h>

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/pubsub/policy.h>

/* Routes */

/* GET https://pubsub.googleapis.com/v1/{resource}:getIamPolicy
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/getIamPolicy
 * */
struct Policy *
getIamPolicy(const char *const resource,
             const struct GetPolicyOptions *const getPolicyOptions) {
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
      DEBUG_SERVER_RESPONSE("getIamPolicy");
      return response.status_code == 200 && response.body != NULL &&
                     response.body[0] != '\0'
                 ? policy_from_json(
                       json_value_get_object(json_parse_string(response.body)))
                 : NULL;
    }
  }
}

/* POST https://pubsub.googleapis.com/v1/{resource}:setIamPolicy
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/setIamPolicy
 */
struct Policy *setIamPolicy(const char *const resource,
                            const struct Policy *const policy) {
  char *path;
  asprintf(&path, "/v1/%s:setIamPolicy", resource);
  {
    const struct ServerResponse response = gcloud_pubsub_post(
        NULL, path, policy == NULL ? NULL : policy_to_json(policy), NULL);
    DEBUG_SERVER_RESPONSE("setIamPolicy");
    return response.status_code == 200 && response.body != NULL &&
                   response.body[0] != '\0'
               ? policy_from_json(
                     json_value_get_object(json_parse_string(response.body)))
               : NULL;
  }
}

/* utility functions */

const char *GetPolicyOptions_to_json(
    const struct GetPolicyOptions *const getPolicyOptions) {
  enum { n = 30 };
  char *requestedPolicyVersion = malloc(n * sizeof *requestedPolicyVersion);
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

struct Expr *expr_from_json(const JSON_Object *const jsonObject) {
  struct Expr *expr = malloc(sizeof *expr);
  expr->expression = json_object_get_string(jsonObject, "expression");
  expr->title = json_object_get_string(jsonObject, "title");
  expr->description = json_object_get_string(jsonObject, "description");
  expr->location = json_object_get_string(jsonObject, "location");
  return expr;
}

const char *expr_to_json(const struct Expr *const expr) {
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

void expr_cleanup(struct Expr *const expr) {
  free((void *)expr->title);
  expr->title = NULL;
  free((void *)expr->expression);
  expr->expression = NULL;
  free((void *)expr->description);
  expr->description = NULL;
  free((void *)expr->location);
  expr->location = NULL;
}

struct Binding *bindings_from_json(const JSON_Object *const jsonObject) {
  struct Binding *bindings = malloc(sizeof *bindings);

  bindings->role = json_object_get_string(jsonObject, "role");

  bindings->members = NULL;
  if (json_object_has_value_of_type(jsonObject, "members", JSONArray)) {
    const JSON_Array *const members_json_items =
        json_object_get_array(jsonObject, "members");
    const size_t members_json_items_n =
        json_array_get_count(members_json_items) + 1;
    size_t i;

    if (members_json_items_n > 1) {
      bindings->members =
          malloc(members_json_items_n * sizeof bindings->members);
      for (i = 0; i < members_json_items_n - 1; i++)
        bindings->members[i] = json_array_get_string(members_json_items, i);
      bindings->members[members_json_items_n - 1] = NULL;
    }
  }

  bindings->condition = NULL;
  if (json_object_has_value_of_type(jsonObject, "condition", JSONArray)) {
    const JSON_Array *const expr_json_items =
        json_object_get_array(jsonObject, "condition");
    const size_t expr_json_items_n = json_array_get_count(expr_json_items) + 1;
    size_t i;

    if (expr_json_items_n > 1) {
      bindings->condition =
          malloc(expr_json_items_n * sizeof bindings->condition);
      for (i = 0; i < expr_json_items_n - 1; i++) {
        struct Expr *expr =
            expr_from_json(json_array_get_object(expr_json_items, i));
        bindings->condition[i] = expr;
      }
      bindings->condition[expr_json_items_n - 1] = NULL;
    }
  }

  return bindings;
}

const char *bindings_to_json(const struct Binding *const binding) {
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

void bindings_cleanup(struct Binding *const binding) {
  free((void *)binding->role);
  binding->role = NULL;

  if (binding->members != NULL) {
    const char **member;
    for (member = binding->members; *member; member++)
      free((void *)*member);
    binding->members = NULL;
  }

  if (binding->condition != NULL) {
    struct Expr **expr;
    for (expr = binding->condition; *expr; expr++)
      expr_cleanup(*expr);
    binding->condition = NULL;
  }
}

struct Policy *policy_from_json(const JSON_Object *const jsonObject) {
  struct Policy *policy = malloc(sizeof *policy);

  const JSON_Object *policy_obj =
      json_object_has_value_of_type(jsonObject, "policy", JSONObject)
          ? json_object_get_object(jsonObject, "policy")
          : jsonObject;

  policy->version = json_object_get_string(policy_obj, "version");

  policy->bindings = NULL;
  if (json_object_has_value_of_type(policy_obj, "bindings", JSONArray)) {
    const JSON_Array *const bindings_json_items =
        json_object_get_array(policy_obj, "bindings");
    const size_t bindings_json_items_n =
        json_array_get_count(bindings_json_items) + 1;
    size_t i;

    if (bindings_json_items_n > 1) {
      policy->bindings =
          malloc(bindings_json_items_n * sizeof policy->bindings);
      for (i = 0; i < bindings_json_items_n - 1; i++) {
        struct Binding *binding =
            bindings_from_json(json_array_get_object(bindings_json_items, i));
        policy->bindings[i] = binding;
      }
      policy->bindings[bindings_json_items_n - 1] = NULL;
    }
  }

  policy->etag = json_object_get_string(policy_obj, "etag");

  return policy;
}

const char *policy_to_json(const struct Policy *const policy) {
  char *s = NULL;
  size_t n;
  jasprintf(&s, "{\"policy\": {");
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
  jasprintf(&s, "}\0");
  s[strlen(s) - 2] = '}';
  return s;
}

void policy_cleanup(struct Policy *const policy) {
  struct Binding **binding;

  free((void *)policy->version);
  policy->version = NULL;

  if (policy->bindings != NULL)
    for (binding = policy->bindings; *binding; binding++)
      bindings_cleanup(*binding);
  policy->bindings = NULL;

  free((void *)policy->etag);
  policy->etag = NULL;
}
