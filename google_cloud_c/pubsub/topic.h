#ifndef GOOGLE_CLOUD_C_TOPIC_H
#define GOOGLE_CLOUD_C_TOPIC_H

#ifdef __cplusplus
#include <cstdbool>
extern "C" {
#elif __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
#include <google_cloud_c/common/google_cloud_c_stdbool.h>
#endif /* __cplusplus */

#include <google_cloud_c/pubsub/google_cloud_c_pubsub_export.h>

#include <parson.h>

/*
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics#MessageStoragePolicy
 */
struct MessageStoragePolicy {
  const char **allowedPersistenceRegions;
};

/*
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics#SchemaSettings
 */
enum Encoding { ENCODING_UNSPECIFIED, JSON, BINARY };

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT enum Encoding str_to_Encoding(const char *);

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT const char *Encoding_to_str(enum Encoding);

struct SchemaSettings {
  const char *schema;
  enum Encoding encoding;
};

/*
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics#Topic
 */
struct Topic {
  const char *name;
  /*"labels": {
    string: string,
      ...
  },*/
  struct MessageStoragePolicy messageStoragePolicy;
  const char *kmsKeyName;
  struct SchemaSettings schemaSettings;
  bool satisfiesPzs;
  const char *messageRetentionDuration;
};

struct OptionalTopic {
  bool set;
  struct Topic topic;
};

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT const struct Topic EMPTY_TOPIC;

/* Gets the configuration of a topic.
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/get
 */
extern GOOGLE_CLOUD_C_PUBSUB_EXPORT struct OptionalTopic
get_topic(const char *);

/* Creates the given topic with the given name.
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/create
 */
extern GOOGLE_CLOUD_C_PUBSUB_EXPORT struct OptionalTopic
create_topic(const char *, struct Topic);

/* Deletes the topic with the given name.
 * https://cloud.google.com/pubsub/docs/reference/rest/v1/projects.topics/delete
 * */
extern GOOGLE_CLOUD_C_PUBSUB_EXPORT bool delete_topic(const char *topic_id);

/* utility functions */

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT struct Topic
topic_from_json(const JSON_Object *);

extern GOOGLE_CLOUD_C_PUBSUB_EXPORT const char *topic_to_json(struct Topic);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GOOGLE_CLOUD_C_TOPIC_H */
