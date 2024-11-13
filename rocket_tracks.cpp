// sync data implementation
#ifdef SYNC_PLAYER
#include "rocket/track.h"
static track_key camera_x_keys[] = {{ 0, 0.000000, KEY_LINEAR}, { 3, 100.000000, KEY_STEP}, };
const sync_track camera_x = { "camera:x", camera_x_keys,2};
static track_key camera_y_keys[] = {{ 0, 0.000000, KEY_STEP}, };
const sync_track camera_y = { "camera:y", camera_y_keys,1};
static track_key camera_z_keys[] = {{ 0, 0.000000, KEY_STEP}, };
const sync_track camera_z = { "camera:z", camera_z_keys,1};
#endif
 // SYNC_PLAYER