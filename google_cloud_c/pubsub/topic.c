#include <parson.h>

#include <c89stringutils_string_extras.h>

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/common/google_cloud_c_json_common.h>
#include <google_cloud_c/pubsub/topic.h>

/* Gets the configuration of a topic.
 * GET https://pubsub.googleapis.com/v1/{topic}
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/get
 * */
struct Topic *get_topic(const char *const topic_id) {
  char *path;
  asprintf(&path, "/v1/%s", topic_id);

  {
    const struct ServerResponse response = gcloud_pubsub_get(NULL, path, NULL);
    DEBUG_SERVER_RESPONSE("get_topic_response");
    return response.status_code == 200 && response.body != NULL &&
                   response.body[0] != '\0'
               ? topic_from_json(
                     json_value_get_object(json_parse_string(response.body)))
               : NULL;
  }
}

/* Creates the given topic with the given name.
 * PUT https://pubsub.googleapis.com/v1/{name}
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/create
 * */
struct Topic *create_topic(const char *const topic_id,
                           const struct Topic *const topic) {
  char *path;
  asprintf(&path, "/v1/%s", topic_id);

  {
    const struct ServerResponse response =
        gcloud_pubsub_put(NULL, path, topic_to_json(topic), NULL);
    DEBUG_SERVER_RESPONSE("create_topic_response");
    return response.status_code == 200 && response.body != NULL &&
                   response.body[0] != '\0'
               ? topic_from_json(
                     json_value_get_object(json_parse_string(response.body)))
               : NULL;
  }
}

/* Deletes the topic with the given name.
 * DELETE https://pubsub.googleapis.com/v1/{topic}
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/delete
 * */
bool delete_topic(const char *const topic_id) {
  char *path;
  asprintf(&path, "/v1/%s", topic_id);

  {
    const struct ServerResponse response =
        gcloud_pubsub_delete(NULL, path, NULL, NULL);
    DEBUG_SERVER_RESPONSE("delete_topic_response");
    /* free(path) */
    return response.status_code == 200;
  }
}

/* Utility functions */

enum Encoding str_to_Encoding(const char *const encoding) {
  if (strcmp(encoding, "JSON") == 0)
    return JSON;
  else if (strcmp(encoding, "BINARY") == 0)
    return BINARY;
  else
    return ENCODING_UNSPECIFIED;
}

const char *Encoding_to_str(const enum Encoding encoding) {
  switch (encoding) {
  case JSON:
    return "JSON";
  case BINARY:
    return "BINARY";
  case ENCODING_UNSPECIFIED:
  default:
    return "ENCODING_UNSPECIFIED";
  }
}

struct Topic *topic_from_json(const JSON_Object *const jsonObject) {
  struct Topic *topic = malloc(sizeof *topic);

  topic->name = json_object_get_string(jsonObject, "name");
  if (json_object_has_value(jsonObject, "messageStoragePolicy")) {
    const JSON_Object *const jsonMessageStoragePolicy =
        json_object_get_object(jsonObject, "messageStoragePolicy");
    struct MessageStoragePolicy *messageStoragePolicy =
        malloc(sizeof *messageStoragePolicy);
    messageStoragePolicy->allowedPersistenceRegions =
        json_array_to_cstr_array(json_object_get_array(
            jsonMessageStoragePolicy, "allowedPersistenceRegions"));
    topic->messageStoragePolicy = messageStoragePolicy;
  } else
    topic->messageStoragePolicy = NULL;
  topic->kmsKeyName = json_object_get_string(jsonObject, "kmsKeyName");
  if (json_object_has_value(jsonObject, "SchemaSettings")) {
    const JSON_Object *const jsonSchemaSettings =
        json_object_get_object(jsonObject, "SchemaSettings");
    struct SchemaSettings *schemaSettings = malloc(sizeof *schemaSettings);
    schemaSettings->schema =
        json_object_get_string(jsonSchemaSettings, "schema");
    if (json_object_has_value(jsonSchemaSettings, "encoding"))
      schemaSettings->encoding = str_to_Encoding(
          json_object_get_string(jsonSchemaSettings, "encoding"));
    else
      schemaSettings->encoding = ENCODING_UNSPECIFIED;
    topic->schemaSettings = schemaSettings;
  } else
    topic->schemaSettings = NULL;
  if (json_object_has_value_of_type(jsonObject, "satisfiesPzs", JSONBoolean))
    topic->satisfiesPzs =
        (bool)json_object_get_boolean(jsonObject, "satisfiesPzs");
  else
    topic->satisfiesPzs = false;
  topic->messageRetentionDuration =
      json_object_get_string(jsonObject, "messageRetentionDuration");

  return topic;
}

const char *
SchemaSettings_to_json(const struct SchemaSettings *const schemaSettings) {
  char *s = NULL;
  jasprintf(&s, "{");
  if (schemaSettings->schema != NULL && schemaSettings->schema[0] != '\0')
    jasprintf(&s, "  \"schema\": \"%s\",", schemaSettings->schema);
  if (schemaSettings->encoding)
    jasprintf(&s, "  \"encoding\": \"%s\"",
              Encoding_to_str(schemaSettings->encoding));
  jasprintf(&s, "\0");
  s[strlen(s) - 1] = '}';
  return s;
}

const char *topic_to_json(const struct Topic *const topic) {
  char *s = NULL;
  jasprintf(&s, "{");
  if (topic->name != NULL && topic->name[0] != '\0')
    jasprintf(&s, "  \"name\": \"%s\",", topic->name);
  if (topic->kmsKeyName != NULL && topic->kmsKeyName[0] != '\0')
    jasprintf(&s, "  \"kmsKeyName\": \"%s\",", topic->kmsKeyName);
  jasprintf(&s, "  \"satisfiesPzs\": %s,",
            topic->satisfiesPzs ? "true" : "false");
  if (topic->messageRetentionDuration != NULL &&
      topic->messageRetentionDuration[0] != '\0')
    jasprintf(&s, "  \"messageRetentionDuration\": \"%s\",",
              topic->messageRetentionDuration);
  /* TODO: MessageStoragePolicy */
  if (topic->schemaSettings->schema != NULL &&
      topic->schemaSettings->schema[0] != '\0')
    jasprintf(&s, "  \"schemaSettings\": %s",
              SchemaSettings_to_json(topic->schemaSettings));
  jasprintf(&s, "\0");
  s[strlen(s) - 1] = '}';
  return s;
}
