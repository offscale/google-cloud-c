#include <google_cloud_c/compute/zones.h>

#include <c89stringutils_string_extras.h>

struct Zones zone_list(void) {
  /* https://cloud.google.com/compute/docs/reference/rest/v1/zones/list
   * GET https://compute.googleapis.com/compute/v1/projects/{project}/zones */
  char *path;
  asprintf(&path, "/compute/v1/projects/%s/zones", AUTH_CONTEXT.project_id);

  {
    const struct ServerResponse response = gcloud_get(NULL, path, NULL);
    struct Zones _zones = {NULL, 0};
    DEBUG_SERVER_RESPONSE("zone_list");
    if (response.status_code == 200 && response.body != NULL &&
        response.body[0] != '\0') {
      const JSON_Value *const zones_json_item =
          json_parse_string(response.body);
      const JSON_Array *const zone_json_items = json_object_get_array(
          json_value_get_object(zones_json_item), "items");
      const size_t zone_json_items_n = json_array_get_count(zone_json_items);
      if (zone_json_items_n > 0) {
        size_t i;

        struct Zone **zones = malloc(zone_json_items_n * sizeof **zones);
        for (i = 0; i < zone_json_items_n; i++)
          zones[i] = zone_from_json(json_array_get_object(zone_json_items, i));
        _zones.arr = zones, _zones.size = zone_json_items_n;
      }
    } else
      fputs("Empty response.body", stderr);
    return _zones;
  }
}

/* utility functions */

struct Zone *zone_from_json(const JSON_Object *const jsonObject) {
  struct Zone *zone = malloc(sizeof *zone);

  zone->id = json_object_get_string(jsonObject, "id");
  zone->creationTimestamp =
      json_object_get_string(jsonObject, "creationTimestamp");
  zone->name = json_object_get_string(jsonObject, "name");
  zone->description = json_object_get_string(jsonObject, "description");
  zone->status = json_object_get_string(jsonObject, "status");
  zone->region = json_object_get_string(jsonObject, "region");
  zone->selfLink = json_object_get_string(jsonObject, "selfLink");
  zone->availableCpuPlatforms = NULL;
  if (json_object_has_value_of_type(jsonObject, "supportsPzs", JSONBoolean))
    zone->supportsPzs =
        (bool)json_object_get_boolean(jsonObject, "supportsPzs");
  else
    zone->supportsPzs = false;
  zone->kind = json_object_get_string(jsonObject, "kind");

  return zone;
}
