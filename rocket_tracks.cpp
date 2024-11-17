// sync data implementation
#ifdef SYNC_PLAYER
#include "rocket/track.h"
static track_key camera_x_keys[] = {{ 0, 1.000000, KEY_STEP}, };
const sync_track camera_x = { "camera:x", camera_x_keys,1};
static track_key camera_y_keys[] = {{ 0, 60.000000, KEY_STEP}, };
const sync_track camera_y = { "camera:y", camera_y_keys,1};
static track_key camera_z_keys[] = {{ 0, 0.000000, KEY_STEP}, };
const sync_track camera_z = { "camera:z", camera_z_keys,1};
static track_key camera_orbitON_keys[] = {{ 0, 1.000000, KEY_STEP}, };
const sync_track camera_orbitON = { "camera:orbitON", camera_orbitON_keys,1};
static track_key camera_orbitX_keys[] = {{ 0, 20.000000, KEY_RAMP}, { 32, 50.000000, KEY_STEP}, };
const sync_track camera_orbitX = { "camera:orbitX", camera_orbitX_keys,2};
static track_key camera_orbitY_keys[] = {{ 0, 0.000000, KEY_LINEAR}, { 32, 340.000000, KEY_STEP}, };
const sync_track camera_orbitY = { "camera:orbitY", camera_orbitY_keys,2};
static track_key camera_orbitDistance_keys[] = {{ 0, 10.000000, KEY_LINEAR}, { 32, 200.000000, KEY_STEP}, };
const sync_track camera_orbitDistance = { "camera:orbitDistance", camera_orbitDistance_keys,2};
#endif
 // SYNC_PLAYER