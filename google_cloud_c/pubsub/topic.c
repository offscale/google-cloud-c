#include <parson.h>

#include <json_common.h>

#include <google_cloud_c/client/cloud_auth.h>
#include <google_cloud_c/pubsub/topic.h>

const struct Topic EMPTY_TOPIC = {NULL,  NULL, NULL, NULL, ENCODING_UNSPECIFIED,
                                  false, NULL};

/* Gets the configuration of a topic.
 * GET https://pubsub.googleapis.com/v1/{topic}
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/get
 * */
struct OptionalTopic get_topic(const char *topic_id) {
  char *path;
  asprintf(&path, "/v1/%s", topic_id);

  {
    struct ServerResponse response = gcloud_pubsub_get(NULL, path, NULL);
    if (response.status_code == 200 && strlen(response.body) > 0) {
      struct OptionalTopic optionalTopic = {
          true, topic_from_json(json_value_get_object(
                    if_error_exit(json_parse_string(response.body), false)))};
      return optionalTopic;
    } else {
      const struct OptionalTopic optionalTopic = {false, EMPTY_TOPIC};
      return optionalTopic;
    }
  }
}

/* Creates the given topic with the given name.
 * PUT https://pubsub.googleapis.com/v1/{name}
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/create
 * */
struct OptionalTopic create_topic(const char *topic_id, struct Topic topic) {
  char *path;
  asprintf(&path, "/v1/%s", topic_id);

  {
    struct ServerResponse response =
        gcloud_pubsub_put(NULL, path, topic_to_json(topic), NULL);
    if (response.status_code == 200 && strlen(response.body) > 0) {
      struct OptionalTopic optionalTopic = {
          true, topic_from_json(json_value_get_object(
                    if_error_exit(json_parse_string(response.body), false)))};
      return optionalTopic;
    } else {
      const struct OptionalTopic optionalTopic = {false, EMPTY_TOPIC};
      return optionalTopic;
    }
  }
}

/* Deletes the topic with the given name.
 * DELETE https://pubsub.googleapis.com/v1/{topic}
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/delete
 * */
bool delete_topic(const char *topic_id) {
  char *path;
  asprintf(&path, "/v1/%s", topic_id);

  {
    struct ServerResponse response =
        gcloud_pubsub_delete(NULL, path, NULL, NULL);
    /* free(path) */
    return response.status_code == 200;
  }
}

/* Utility functions */

enum Encoding str_to_Encoding(const char *encoding) {
  if (strcmp(encoding, "JSON") == 0)
    return JSON;
  else if (strcmp(encoding, "BINARY") == 0)
    return BINARY;
  else
    return ENCODING_UNSPECIFIED;
}

const char *Encoding_to_str(enum Encoding encoding) {
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

struct Topic topic_from_json(const JSON_Object *jsonObject) {
  struct Topic topic;

  topic.name = json_object_get_string(jsonObject, "name");
  {
    const JSON_Object *jsonMessageStoragePolicy =
        json_object_get_object(jsonObject, "messageStoragePolicy");
    struct MessageStoragePolicy messageStoragePolicy;
    messageStoragePolicy.allowedPersistenceRegions =
        (const char **)json_object_get_array(jsonMessageStoragePolicy,
                                             "allowedPersistenceRegions");
    topic.messageStoragePolicy = messageStoragePolicy;
  }
  topic.kmsKeyName = json_object_get_string(jsonObject, "kmsKeyName");
  {
    const JSON_Object *jsonSchemaSettings =
        json_object_get_object(jsonObject, "SchemaSettings");
    struct SchemaSettings schemaSettings;
    schemaSettings.schema = json_object_get_string(jsonObject, "schema");
    schemaSettings.encoding =
        str_to_Encoding(json_object_get_string(jsonSchemaSettings, "encoding"));
    topic.schemaSettings = schemaSettings;
  }
  topic.satisfiesPzs = (bool)json_object_get_boolean(jsonObject, "kmsKeyName");
  topic.messageRetentionDuration =
      json_object_get_string(jsonObject, "messageRetentionDuration");

  return topic;
}

const char *SchemaSettings_to_json(struct SchemaSettings schemaSettings) {
  char *s=NULL;
  jasprintf(&s, "{");
  if (schemaSettings.schema != NULL && strlen(schemaSettings.schema) > 0)
    jasprintf(&s, "  \"schema\": \"%s\",", schemaSettings.schema);
  if (schemaSettings.encoding)
    jasprintf(&s, "  \"encoding\": \"%s\"",
              Encoding_to_str(schemaSettings.encoding));
  jasprintf(&s, "\0");
  s[strlen(s) - 1] = '}';
  return s;
}

const char *topic_to_json(struct Topic topic) {
  char *s=NULL;
  jasprintf(&s, "{");
  if (topic.name != NULL && strlen(topic.name) > 0)
    jasprintf(&s, "  \"name\": \"%s\",", topic.name);
  if (topic.kmsKeyName != NULL && strlen(topic.kmsKeyName) > 0)
    jasprintf(&s, "  \"kmsKeyName\": \"%s\",", topic.kmsKeyName);
  if (topic.satisfiesPzs)
    jasprintf(&s, "  \"satisfiesPzs\": %ld,", topic.satisfiesPzs);
  if (topic.messageRetentionDuration != NULL &&
      strlen(topic.messageRetentionDuration) > 0)
    jasprintf(&s, "  \"messageRetentionDuration\": \"%s\",",
              topic.messageRetentionDuration);
  /* TODO: MessageStoragePolicy */
  if (topic.schemaSettings.schema != NULL &&
      strlen(topic.schemaSettings.schema) > 0)
    jasprintf(&s, "  \"schemaSettings\": %s",
              SchemaSettings_to_json(topic.schemaSettings));
  jasprintf(&s, "\0");
  s[strlen(s) - 1] = '}';
  return s;
}
