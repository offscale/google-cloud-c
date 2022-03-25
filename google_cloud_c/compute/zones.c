#include <google_cloud_c/compute/zones.h>
#include <json_common.h>

struct Zones zone_list() {
  /* https://cloud.google.com/compute/docs/reference/rest/v1/zones/list
   * GET https://compute.googleapis.com/compute/v1/projects/{project}/zones */
  char *path;
  asprintf(&path, "/compute/v1/projects/%s/zones", AUTH_CONTEXT.project_id);

  struct ServerResponse response = gcloud_get(NULL, path, NULL);
  DEBUG_SERVER_RESPONSE("zone_list");
  assert(response.status_code == 200);

  const JSON_Value *zones_json_item =
      if_error_exit(json_parse_string(response.body), false);
  const JSON_Array *zone_json_items =
      json_object_get_array(json_value_get_object(zones_json_item), "items");
  const size_t zone_json_items_n = json_array_get_count(zone_json_items);
  size_t i;

  struct Zone *zones =
      (struct Zone *)malloc(zone_json_items_n * sizeof(struct Zone));
  for (i = 0; i < zone_json_items_n; i++) {
    const JSON_Object *zone_json = json_array_get_object(zone_json_items, i);
    const struct Zone zone = {
        /* id */ json_object_get_string(zone_json, "id"),
        /* creationTimestamp */
        json_object_get_string(zone_json, "creationTimestamp"),
        /* name */ json_object_get_string(zone_json, "name"),
        /* description */ json_object_get_string(zone_json, "description"),
        /* status */ json_object_get_string(zone_json, "status"),
        // /* deprecated */ zone.object.at("deprecated").object,
        /* region */ json_object_get_string(zone_json, "region"),
        /* selfLink */ json_object_get_string(zone_json, "selfLink"),
        /* availableCpuPlatforms */
        NULL, // zone.object.at("availableCpuPlatforms").array,
        /* supportsPzs */
        (bool)json_object_get_boolean(zone_json, "supportsPzs"),
        /* kind */ json_object_get_string(zone_json, "kind"),
    };
    zones[i] = zone;
  }
  const struct Zones _zones = {zones, zone_json_items_n};
  return _zones;
}
