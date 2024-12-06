// sync data implementation
#ifdef SYNC_PLAYER
#include "rocket/track.h"
static track_key camera_x_keys[] = {{ 0, 4.999997, KEY_RAMP}, { 32, 4.400000, KEY_RAMP}, { 41, 0.300000, KEY_LINEAR}, { 45, -0.400000, KEY_RAMP}, { 64, -4.499999, KEY_STEP}, { 80, 0.000000, KEY_STEP}, { 88, 0.000000, KEY_STEP}, { 232, 0.000000, KEY_LINEAR}, { 449, 0.000000, KEY_STEP}, { 466, 3.000000, KEY_STEP}, { 544, 0.000000, KEY_STEP}, { 545, 0.000000, KEY_RAMP}, { 664, 0.000000, KEY_RAMP}, { 677, -15.000000, KEY_LINEAR}, { 760, -14.000000, KEY_STEP}, { 800, -15.000000, KEY_LINEAR}, { 810, -110.000000, KEY_STEP}, { 816, -15.000000, KEY_LINEAR}, { 831, 10.000000, KEY_STEP}, { 832, 0.000000, KEY_LINEAR}, { 840, -60.000000, KEY_LINEAR}, { 920, -591.000000, KEY_STEP}, { 921, 0.000000, KEY_STEP}, { 1120, 0.000000, KEY_RAMP}, { 1121, 0.000000, KEY_STEP}, { 1248, 100.000000, KEY_STEP}, };
const sync_track camera_x = { "camera:x", camera_x_keys,26};
static track_key camera_y_keys[] = {{ 0, 0.000000, KEY_RAMP}, { 32, 0.000000, KEY_STEP}, { 80, 0.000000, KEY_STEP}, { 88, -2.000000, KEY_STEP}, { 232, 2.000000, KEY_STEP}, { 376, 4.000000, KEY_LINEAR}, { 424, 5.000000, KEY_LINEAR}, { 449, 3.000000, KEY_LINEAR}, { 544, 0.000000, KEY_STEP}, { 545, 50.000000, KEY_RAMP}, { 632, -108.000000, KEY_RAMP}, { 640, -110.000000, KEY_RAMP}, { 664, 60.000000, KEY_RAMP}, { 688, 49.000000, KEY_LINEAR}, { 722, 10.000000, KEY_LINEAR}, { 728, 3.000000, KEY_LINEAR}, { 736, 0.000000, KEY_LINEAR}, { 744, -2.000000, KEY_LINEAR}, { 760, -3.000000, KEY_RAMP}, { 800, 41.000000, KEY_LINEAR}, { 809, 110.000000, KEY_STEP}, { 816, 0.000000, KEY_LINEAR}, { 831, 10.000000, KEY_STEP}, { 836, -114.000000, KEY_LINEAR}, { 840, -99.000000, KEY_LINEAR}, { 863, -10.000000, KEY_LINEAR}, { 920, -180.000000, KEY_STEP}, { 921, 0.000000, KEY_STEP}, { 1120, 150.000000, KEY_LINEAR}, { 1121, 0.000000, KEY_STEP}, };
const sync_track camera_y = { "camera:y", camera_y_keys,30};
static track_key camera_z_keys[] = {{ 0, 0.000000, KEY_STEP}, { 80, 0.000000, KEY_STEP}, { 88, 0.000000, KEY_STEP}, { 232, 0.000000, KEY_LINEAR}, { 440, -3.000000, KEY_RAMP}, { 449, -13.000000, KEY_STEP}, { 466, -13.000000, KEY_STEP}, { 544, 0.000000, KEY_STEP}, { 545, 0.000000, KEY_RAMP}, { 664, -22.000000, KEY_RAMP}, { 688, -40.000000, KEY_LINEAR}, { 722, -13.000000, KEY_RAMP}, { 728, -16.000000, KEY_RAMP}, { 736, -18.000000, KEY_RAMP}, { 760, -36.000000, KEY_LINEAR}, { 800, 55.000000, KEY_STEP}, { 816, 10.000000, KEY_STEP}, { 831, 0.000000, KEY_LINEAR}, { 840, -350.000000, KEY_STEP}, { 920, 0.000000, KEY_STEP}, { 921, 0.000000, KEY_STEP}, { 1120, 100.000000, KEY_STEP}, };
const sync_track camera_z = { "camera:z", camera_z_keys,22};
static track_key camera_orbitON_keys[] = {{ 0, 0.000000, KEY_STEP}, { 80, 1.000000, KEY_STEP}, { 88, 1.000000, KEY_STEP}, { 232, 0.000000, KEY_STEP}, { 241, 0.000000, KEY_STEP}, { 263, 0.000000, KEY_STEP}, { 376, 1.000000, KEY_STEP}, { 466, 1.000000, KEY_STEP}, { 544, 0.000000, KEY_STEP}, { 545, 12.000000, KEY_RAMP}, { 664, 1.000000, KEY_STEP}, { 799, 0.000000, KEY_STEP}, { 800, 0.000000, KEY_LINEAR}, { 831, 1.000000, KEY_LINEAR}, { 832, 0.000000, KEY_STEP}, { 920, 0.000000, KEY_STEP}, { 1120, 0.000000, KEY_STEP}, };
const sync_track camera_orbitON = { "camera:orbitON", camera_orbitON_keys,17};
static track_key camera_orbitX_keys[] = {{ 0, 0.000000, KEY_STEP}, { 80, 0.000000, KEY_STEP}, { 88, 24.000000, KEY_STEP}, { 232, -3.000000, KEY_STEP}, { 376, 4.000000, KEY_LINEAR}, { 400, 13.000000, KEY_LINEAR}, { 465, 3.000000, KEY_LINEAR}, { 488, 22.000000, KEY_STEP}, { 544, 4.000000, KEY_LINEAR}, { 576, 25.000000, KEY_RAMP}, { 664, 57.000000, KEY_LINEAR}, { 704, 52.000000, KEY_LINEAR}, { 722, 47.000000, KEY_RAMP}, { 736, 3.000000, KEY_LINEAR}, { 744, 0.000000, KEY_LINEAR}, { 760, 6.000000, KEY_LINEAR}, { 800, 0.000000, KEY_STEP}, { 808, 20.000000, KEY_LINEAR}, { 816, 50.000000, KEY_STEP}, { 831, 50.000000, KEY_STEP}, { 920, 0.000000, KEY_STEP}, };
const sync_track camera_orbitX = { "camera:orbitX", camera_orbitX_keys,21};
static track_key camera_orbitY_keys[] = {{ 0, -4.000000, KEY_STEP}, { 80, 54.000000, KEY_STEP}, { 88, -160.000000, KEY_LINEAR}, { 231, -40.000000, KEY_STEP}, { 232, 0.000000, KEY_STEP}, { 376, 180.000000, KEY_LINEAR}, { 464, 50.000000, KEY_LINEAR}, { 488, 44.000000, KEY_RAMP}, { 528, 0.000000, KEY_STEP}, { 544, 0.000000, KEY_LINEAR}, { 545, 77.000000, KEY_LINEAR}, { 557, 79.000000, KEY_LINEAR}, { 592, 140.000000, KEY_RAMP}, { 664, 12.000000, KEY_LINEAR}, { 744, 1.000000, KEY_RAMP}, { 776, -70.000000, KEY_LINEAR}, { 800, -95.000000, KEY_STEP}, { 808, -20.000000, KEY_LINEAR}, { 831, -29.000000, KEY_STEP}, { 920, 0.000000, KEY_STEP}, };
const sync_track camera_orbitY = { "camera:orbitY", camera_orbitY_keys,20};
static track_key camera_orbitDistance_keys[] = {{ 0, 5.000000, KEY_STEP}, { 80, 0.000000, KEY_STEP}, { 88, 25.000000, KEY_STEP}, { 232, 0.000000, KEY_STEP}, { 376, 40.000000, KEY_LINEAR}, { 400, 31.000000, KEY_LINEAR}, { 448, 31.000000, KEY_LINEAR}, { 464, 30.000000, KEY_STEP}, { 465, 20.000000, KEY_LINEAR}, { 488, 33.000000, KEY_STEP}, { 544, 80.000000, KEY_LINEAR}, { 550, 90.000000, KEY_RAMP}, { 616, 400.000000, KEY_RAMP}, { 632, 450.000000, KEY_RAMP}, { 640, 360.000000, KEY_RAMP}, { 664, 33.000000, KEY_LINEAR}, { 744, 31.000000, KEY_RAMP}, { 776, 102.000000, KEY_RAMP}, { 800, 140.000000, KEY_STEP}, { 816, 140.000000, KEY_LINEAR}, { 831, 20.000000, KEY_STEP}, { 920, 0.000000, KEY_STEP}, };
const sync_track camera_orbitDistance = { "camera:orbitDistance", camera_orbitDistance_keys,22};
static track_key camera_speed_keys[] = {{ 0, 0.000000, KEY_STEP}, { 80, 0.000000, KEY_STEP}, { 88, 0.000000, KEY_STEP}, { 232, 0.000000, KEY_STEP}, { 545, 0.000000, KEY_STEP}, { 831, 0.300000, KEY_STEP}, { 837, 10.000000, KEY_STEP}, { 920, 0.000000, KEY_STEP}, };
const sync_track camera_speed = { "camera:speed", camera_speed_keys,8};
static track_key camera_interpolateOn_keys[] = {{ 0, 0.000000, KEY_STEP}, { 80, 0.000000, KEY_STEP}, { 88, 0.000000, KEY_STEP}, { 232, 0.000000, KEY_STEP}, { 545, 0.000000, KEY_STEP}, { 795, 1.000000, KEY_STEP}, { 816, 0.000000, KEY_STEP}, };
const sync_track camera_interpolateOn = { "camera:interpolateOn", camera_interpolateOn_keys,7};
static track_key camera_wiggle_keys[] = {{ 0, 0.000000, KEY_STEP}, { 80, 0.000000, KEY_STEP}, { 88, 0.000000, KEY_STEP}, { 232, 0.000000, KEY_STEP}, { 545, 0.000000, KEY_STEP}, };
const sync_track camera_wiggle = { "camera:wiggle", camera_wiggle_keys,5};
static track_key camera_fov_keys[] = {{ 0, 0.000000, KEY_RAMP}, { 24, -40.099998, KEY_STEP}, { 64, -40.099998, KEY_RAMP}, { 72, -50.000000, KEY_STEP}, { 80, 0.000000, KEY_STEP}, { 232, -43.000000, KEY_STEP}, { 280, -43.000000, KEY_RAMP}, { 320, 0.000000, KEY_STEP}, { 660, 0.000000, KEY_RAMP}, { 686, -20.000000, KEY_LINEAR}, { 832, 0.000000, KEY_RAMP}, { 848, -19.000000, KEY_RAMP}, { 880, -6.000000, KEY_RAMP}, { 904, 0.000000, KEY_STEP}, { 1120, 0.000000, KEY_LINEAR}, };
const sync_track camera_fov = { "camera:fov", camera_fov_keys,15};
static track_key camera_posX_keys[] = {{ 0, 2.500000, KEY_RAMP}, { 48, 0.000000, KEY_STEP}, { 232, 68.000000, KEY_STEP}, { 320, 42.500008, KEY_STEP}, { 704, 160.000000, KEY_LINEAR}, { 800, 152.000000, KEY_STEP}, { 832, 320.000000, KEY_STEP}, { 920, 0.000000, KEY_STEP}, { 921, 0.000000, KEY_STEP}, { 1120, 360.000000, KEY_STEP}, };
const sync_track camera_posX = { "camera:posX", camera_posX_keys,10};
static track_key camera_posY_keys[] = {{ 0, -3.000000, KEY_STEP}, { 232, -50.000000, KEY_STEP}, { 320, -37.000000, KEY_STEP}, { 704, 200.000000, KEY_LINEAR}, { 800, 20.000000, KEY_STEP}, { 832, 90.000000, KEY_STEP}, { 920, 0.000000, KEY_STEP}, { 921, 0.000000, KEY_STEP}, { 1120, 60.000000, KEY_STEP}, };
const sync_track camera_posY = { "camera:posY", camera_posY_keys,9};
static track_key camera_posZ_keys[] = {{ 0, -6.000000, KEY_STEP}, { 232, -40.000000, KEY_STEP}, { 320, -70.000000, KEY_STEP}, { 376, 71.000000, KEY_STEP}, { 704, -150.000000, KEY_LINEAR}, { 800, 0.000000, KEY_STEP}, { 832, -1260.000000, KEY_LINEAR}, { 880, -1400.000000, KEY_STEP}, { 920, 0.000000, KEY_STEP}, { 921, 0.000000, KEY_STEP}, { 1120, -160.000000, KEY_STEP}, };
const sync_track camera_posZ = { "camera:posZ", camera_posZ_keys,11};
static track_key camera_viewDistance_keys[] = {{ 0, 1000.000000, KEY_STEP}, };
const sync_track camera_viewDistance = { "camera:viewDistance", camera_viewDistance_keys,1};
static track_key camera_targetOn_keys[] = {{ 0, 1.000000, KEY_STEP}, { 830, 0.000000, KEY_STEP}, { 832, 0.000000, KEY_STEP}, { 837, 1.000000, KEY_STEP}, { 1121, 1.000000, KEY_STEP}, { 1208, 0.000000, KEY_STEP}, };
const sync_track camera_targetOn = { "camera:targetOn", camera_targetOn_keys,6};
static track_key departure_text_keys[] = {{ 0, 0.000000, KEY_STEP}, { 24, 1.000000, KEY_STEP}, { 64, 2.000000, KEY_STEP}, };
const sync_track departure_text = { "departure:text", departure_text_keys,3};
static track_key departure_textX_keys[] = {{ 0, -6.999998, KEY_STEP}, };
const sync_track departure_textX = { "departure:textX", departure_textX_keys,1};
static track_key departure_textY_keys[] = {{ 0, 1.600000, KEY_STEP}, };
const sync_track departure_textY = { "departure:textY", departure_textY_keys,1};
static track_key departure_textScale_keys[] = {{ 0, -0.700000, KEY_STEP}, };
const sync_track departure_textScale = { "departure:textScale", departure_textScale_keys,1};
static track_key departure_width_keys[] = {{ 0, 18.300001, KEY_STEP}, };
const sync_track departure_width = { "departure:width", departure_width_keys,1};
static track_key departure_height_keys[] = {{ 0, 5.099998, KEY_STEP}, };
const sync_track departure_height = { "departure:height", departure_height_keys,1};
static track_key departure_depth_keys[] = {{ 0, 0.100000, KEY_STEP}, };
const sync_track departure_depth = { "departure:depth", departure_depth_keys,1};
static track_key departure_hangarX_keys[] = {{ 0, -15.000015, KEY_STEP}, };
const sync_track departure_hangarX = { "departure:hangarX", departure_hangarX_keys,1};
static track_key departure_hangarY_keys[] = {{ 0, 7.100000, KEY_STEP}, };
const sync_track departure_hangarY = { "departure:hangarY", departure_hangarY_keys,1};
static track_key departure_hangarZ_keys[] = {{ 0, -28.800003, KEY_STEP}, };
const sync_track departure_hangarZ = { "departure:hangarZ", departure_hangarZ_keys,1};
static track_key scene_keys[] = {{ 0, 1.000000, KEY_STEP}, { 80, 0.000000, KEY_STEP}, { 832, 2.000000, KEY_STEP}, { 920, 3.000000, KEY_STEP}, { 1120, 4.000000, KEY_STEP}, };
const sync_track scene = { "scene", scene_keys,5};
static track_key fade_out_keys[] = {{ 0, 1.000000, KEY_LINEAR}, { 4, 0.000000, KEY_STEP}, { 72, 0.000000, KEY_LINEAR}, { 79, 1.000000, KEY_LINEAR}, { 86, 1.000000, KEY_RAMP}, { 88, 1.000000, KEY_LINEAR}, { 90, 0.000000, KEY_STEP}, { 225, 0.000000, KEY_LINEAR}, { 231, 1.000000, KEY_LINEAR}, { 234, 1.000000, KEY_LINEAR}, { 236, 0.000000, KEY_STEP}, { 318, 0.000000, KEY_LINEAR}, { 320, 1.000000, KEY_LINEAR}, { 322, 1.000000, KEY_LINEAR}, { 325, 0.000000, KEY_STEP}, { 374, 0.000000, KEY_RAMP}, { 376, 1.000000, KEY_RAMP}, { 378, 1.000000, KEY_RAMP}, { 381, 0.000000, KEY_STEP}, { 464, 0.000000, KEY_RAMP}, { 465, 1.000000, KEY_RAMP}, { 466, 1.000000, KEY_RAMP}, { 470, 0.000000, KEY_STEP}, { 541, 0.000000, KEY_RAMP}, { 543, 1.000000, KEY_RAMP}, { 545, 1.000000, KEY_RAMP}, { 547, 0.000000, KEY_STEP}, { 807, 0.000000, KEY_LINEAR}, { 816, 1.000000, KEY_LINEAR}, { 818, 0.000000, KEY_STEP}, { 830, 0.000000, KEY_LINEAR}, { 832, 1.000000, KEY_LINEAR}, { 837, 1.000000, KEY_LINEAR}, { 840, 0.000000, KEY_STEP}, { 912, 0.000000, KEY_RAMP}, { 920, 0.100000, KEY_RAMP}, { 928, 0.000000, KEY_STEP}, { 1118, 0.000000, KEY_RAMP}, { 1120, 1.000000, KEY_LINEAR}, { 1125, 1.000000, KEY_RAMP}, { 1136, 0.000000, KEY_STEP}, { 1232, 0.000000, KEY_RAMP}, { 1242, 1.000000, KEY_STEP}, };
const sync_track fade_out = { "fade_out", fade_out_keys,43};
static track_key end_demo_keys[] = {{ 0, 0.000000, KEY_STEP}, { 1264, 1.000000, KEY_STEP}, };
const sync_track end_demo = { "end_demo", end_demo_keys,2};
static track_key drawTerrain_keys[] = {{ 0, 0.000000, KEY_STEP}, { 537, 1.000000, KEY_STEP}, { 657, 0.000000, KEY_STEP}, { 687, 1.000000, KEY_STEP}, };
const sync_track drawTerrain = { "drawTerrain", drawTerrain_keys,4};
static track_key drawSpaceport_keys[] = {{ 0, 1.000000, KEY_STEP}, };
const sync_track drawSpaceport = { "drawSpaceport", drawSpaceport_keys,1};
static track_key door_hangar_keys[] = {{ 0, -14.000000, KEY_STEP}, { 243, -14.000000, KEY_RAMP}, { 245, -14.299995, KEY_RAMP}, { 252, -15.000002, KEY_RAMP}, { 288, -36.000000, KEY_STEP}, };
const sync_track door_hangar = { "door:hangar", door_hangar_keys,5};
static track_key door_hangarX_keys[] = {{ 0, -9.100000, KEY_STEP}, };
const sync_track door_hangarX = { "door:hangarX", door_hangarX_keys,1};
static track_key door_elevator_keys[] = {{ 0, -14.000000, KEY_STEP}, { 648, -14.000000, KEY_RAMP}, { 650, -14.300001, KEY_RAMP}, { 656, -14.500006, KEY_LINEAR}, { 680, -37.000000, KEY_STEP}, };
const sync_track door_elevator = { "door:elevator", door_elevator_keys,5};
static track_key door_elevatorX_keys[] = {{ 0, 79.000000, KEY_STEP}, { 120, 79.000000, KEY_STEP}, };
const sync_track door_elevatorX = { "door:elevatorX", door_elevatorX_keys,2};
static track_key platform_x_keys[] = {{ 80, 5.699997, KEY_LINEAR}, { 196, 5.699997, KEY_LINEAR}, { 296, 8.000000, KEY_RAMP}, { 336, 65.000000, KEY_LINEAR}, { 688, 65.999985, KEY_STEP}, };
const sync_track platform_x = { "platform:x", platform_x_keys,5};
static track_key platform_y_keys[] = {{ 80, -48.100006, KEY_STEP}, { 196, -48.100006, KEY_STEP}, { 664, -48.100006, KEY_RAMP}, { 688, 9.999996, KEY_STEP}, };
const sync_track platform_y = { "platform:y", platform_y_keys,4};
static track_key platform_z_keys[] = {{ 80, -41.100010, KEY_LINEAR}, { 196, -41.100010, KEY_LINEAR}, { 378, -42.000000, KEY_RAMP}, { 464, 202.000000, KEY_STEP}, { 466, 250.000000, KEY_RAMP}, { 488, 271.000000, KEY_LINEAR}, { 688, 274.800049, KEY_STEP}, };
const sync_track platform_z = { "platform:z", platform_z_keys,7};
static track_key platform_rotY_keys[] = {{ 80, 270.000000, KEY_LINEAR}, { 346, 270.000000, KEY_RAMP}, { 364, 180.000000, KEY_STEP}, { 496, 180.000000, KEY_RAMP}, { 536, 0.000000, KEY_STEP}, };
const sync_track platform_rotY = { "platform:rotY", platform_rotY_keys,5};
static track_key platform_railingY_keys[] = {{ 0, -13.600014, KEY_LINEAR}, { 336, -13.600014, KEY_LINEAR}, { 346, -15.599983, KEY_STEP}, { 364, -15.599983, KEY_LINEAR}, { 376, -13.600014, KEY_STEP}, { 488, -13.600014, KEY_LINEAR}, { 496, -16.299994, KEY_STEP}, { 536, -16.299994, KEY_LINEAR}, { 544, -13.000000, KEY_STEP}, };
const sync_track platform_railingY = { "platform:railingY", platform_railingY_keys,9};
static track_key ship_x_keys[] = {{ 0, 0.800000, KEY_STEP}, { 768, 0.800000, KEY_STEP}, { 921, 250.000000, KEY_LINEAR}, { 976, 300.000000, KEY_LINEAR}, { 1120, 81.000000, KEY_RAMP}, { 1121, 80.000000, KEY_LINEAR}, { 1158, 170.000000, KEY_LINEAR}, { 1224, 280.000000, KEY_RAMP}, { 1236, 730.000000, KEY_STEP}, };
const sync_track ship_x = { "ship:x", ship_x_keys,9};
static track_key ship_y_keys[] = {{ 0, 8.400000, KEY_STEP}, { 768, 8.400000, KEY_LINEAR}, { 808, 11.000000, KEY_RAMP}, { 837, 247.000000, KEY_LINEAR}, { 912, 480.000000, KEY_STEP}, { 921, -260.000000, KEY_LINEAR}, { 976, -220.000000, KEY_LINEAR}, { 1120, -70.000000, KEY_STEP}, { 1121, -270.000000, KEY_LINEAR}, { 1158, -170.000000, KEY_LINEAR}, { 1224, -30.000000, KEY_RAMP}, { 1236, 330.000000, KEY_STEP}, };
const sync_track ship_y = { "ship:y", ship_y_keys,12};
static track_key ship_z_keys[] = {{ 0, -4.000000, KEY_STEP}, { 768, -4.000000, KEY_RAMP}, { 816, -700.000000, KEY_LINEAR}, { 832, -1000.000000, KEY_LINEAR}, { 912, -2320.000000, KEY_STEP}, { 921, 30.000000, KEY_LINEAR}, { 976, 160.000000, KEY_LINEAR}, { 1120, 170.000000, KEY_RAMP}, { 1121, -100.000000, KEY_LINEAR}, { 1158, 50.000000, KEY_LINEAR}, { 1224, 40.000000, KEY_RAMP}, { 1236, 310.000000, KEY_STEP}, };
const sync_track ship_z = { "ship:z", ship_z_keys,12};
static track_key ship_rotX_keys[] = {{ 0, 0.000000, KEY_STEP}, { 768, 0.000000, KEY_LINEAR}, { 804, 1.000000, KEY_LINEAR}, { 836, 50.000000, KEY_STEP}, { 837, 0.000000, KEY_LINEAR}, { 841, 0.000000, KEY_STEP}, { 912, 15.000000, KEY_STEP}, { 921, 20.000000, KEY_LINEAR}, { 976, 20.000000, KEY_LINEAR}, { 1120, 14.000000, KEY_STEP}, { 1121, 20.000000, KEY_LINEAR}, { 1224, -20.000000, KEY_STEP}, };
const sync_track ship_rotX = { "ship:rotX", ship_rotX_keys,12};
static track_key ship_rotY_keys[] = {{ 0, 180.000000, KEY_STEP}, { 841, 180.000000, KEY_STEP}, { 921, 14.000000, KEY_LINEAR}, { 976, 14.000000, KEY_LINEAR}, { 1120, 20.099998, KEY_STEP}, { 1121, -14.399994, KEY_LINEAR}, { 1158, 10.000000, KEY_LINEAR}, { 1224, 30.000000, KEY_STEP}, };
const sync_track ship_rotY = { "ship:rotY", ship_rotY_keys,8};
static track_key ship_rotZ_keys[] = {{ 0, 0.000000, KEY_STEP}, { 921, 20.000000, KEY_LINEAR}, { 976, 20.000000, KEY_LINEAR}, { 1120, -10.000000, KEY_STEP}, { 1121, 0.000000, KEY_STEP}, { 1224, 0.000000, KEY_STEP}, };
const sync_track ship_rotZ = { "ship:rotZ", ship_rotZ_keys,6};
static track_key ship_scale_keys[] = {{ 0, 1.200000, KEY_STEP}, { 921, 1.000000, KEY_LINEAR}, { 1064, 1.000000, KEY_STEP}, { 1120, 1.000000, KEY_STEP}, { 1121, 0.400000, KEY_RAMP}, { 1233, 0.300000, KEY_STEP}, };
const sync_track ship_scale = { "ship:scale", ship_scale_keys,6};
static track_key ship_illumination_keys[] = {};
const sync_track ship_illumination = { "ship:illumination", ship_illumination_keys,0};
static track_key ship_detailScale_keys[] = {{ 0, 0.001000, KEY_STEP}, { 360, 0.003000, KEY_STEP}, { 776, 0.010000, KEY_STEP}, { 816, 0.001000, KEY_STEP}, { 835, -1.000000, KEY_STEP}, };
const sync_track ship_detailScale = { "ship:detailScale", ship_detailScale_keys,5};
static track_key ship_gearRotX_keys[] = {};
const sync_track ship_gearRotX = { "ship:gearRotX", ship_gearRotX_keys,0};
static track_key ship_gearRotY_keys[] = {{ 112, -180.000000, KEY_STEP}, };
const sync_track ship_gearRotY = { "ship:gearRotY", ship_gearRotY_keys,1};
static track_key ship_gearScale_keys[] = {{ 112, -0.800000, KEY_STEP}, { 299, -0.850000, KEY_STEP}, { 816, -1.000000, KEY_STEP}, };
const sync_track ship_gearScale = { "ship:gearScale", ship_gearScale_keys,3};
static track_key ship_gearX_keys[] = {{ 112, 0.200000, KEY_STEP}, { 299, 0.500000, KEY_STEP}, };
const sync_track ship_gearX = { "ship:gearX", ship_gearX_keys,2};
static track_key ship_gearY_keys[] = {{ 112, -4.400001, KEY_STEP}, };
const sync_track ship_gearY = { "ship:gearY", ship_gearY_keys,1};
static track_key ship_gearZ_keys[] = {{ 112, 10.300000, KEY_STEP}, };
const sync_track ship_gearZ = { "ship:gearZ", ship_gearZ_keys,1};
static track_key ship_gearBetween_keys[] = {{ 112, 3.200001, KEY_STEP}, };
const sync_track ship_gearBetween = { "ship:gearBetween", ship_gearBetween_keys,1};
static track_key ship_gearFrontBack_keys[] = {{ 112, -23.500002, KEY_STEP}, };
const sync_track ship_gearFrontBack = { "ship:gearFrontBack", ship_gearFrontBack_keys,1};
static track_key terrain_x_keys[] = {{ 0, -350.000000, KEY_LINEAR}, { 544, -609.000000, KEY_LINEAR}, { 600, -609.000000, KEY_STEP}, { 832, 0.000000, KEY_LINEAR}, { 840, -160.000000, KEY_LINEAR}, };
const sync_track terrain_x = { "terrain:x", terrain_x_keys,5};
static track_key terrain_y_keys[] = {{ 0, -27.900002, KEY_LINEAR}, { 544, -2.700005, KEY_LINEAR}, { 600, -2.700005, KEY_STEP}, };
const sync_track terrain_y = { "terrain:y", terrain_y_keys,3};
static track_key terrain_z_keys[] = {{ 0, 119.000000, KEY_LINEAR}, { 544, -550.000000, KEY_LINEAR}, { 600, -550.000000, KEY_STEP}, { 832, -1000.000000, KEY_LINEAR}, { 840, -2280.000000, KEY_LINEAR}, };
const sync_track terrain_z = { "terrain:z", terrain_z_keys,5};
static track_key terrain_scale_keys[] = {{ 0, 71.000000, KEY_LINEAR}, { 544, 41.000000, KEY_LINEAR}, { 600, 41.000000, KEY_STEP}, };
const sync_track terrain_scale = { "terrain:scale", terrain_scale_keys,3};
static track_key terrain_rotY_keys[] = {{ 0, 90.000000, KEY_LINEAR}, { 544, 0.000000, KEY_LINEAR}, { 600, 0.000000, KEY_STEP}, };
const sync_track terrain_rotY = { "terrain:rotY", terrain_rotY_keys,3};
static track_key terrain_heightMult_keys[] = {{ 0, 4.000000, KEY_STEP}, };
const sync_track terrain_heightMult = { "terrain:heightMult", terrain_heightMult_keys,1};
static track_key terrain_uvRange_keys[] = {{ 0, 8.000000, KEY_STEP}, };
const sync_track terrain_uvRange = { "terrain:uvRange", terrain_uvRange_keys,1};
static track_key earth_x_keys[] = {{ 920, -285.000000, KEY_LINEAR}, { 968, -285.000000, KEY_LINEAR}, { 1112, 431.300018, KEY_STEP}, };
const sync_track earth_x = { "earth:x", earth_x_keys,3};
static track_key earth_y_keys[] = {{ 920, 60.000000, KEY_LINEAR}, { 1051, 80.000000, KEY_LINEAR}, };
const sync_track earth_y = { "earth:y", earth_y_keys,2};
static track_key earth_scale_keys[] = {{ 920, 0.300000, KEY_RAMP}, { 1112, 1.000000, KEY_STEP}, };
const sync_track earth_scale = { "earth:scale", earth_scale_keys,2};
static track_key earth_moonX_keys[] = {{ 920, -203.000000, KEY_LINEAR}, { 1000, -286.000000, KEY_LINEAR}, { 1032, -286.000000, KEY_LINEAR}, { 1112, -449.300018, KEY_STEP}, };
const sync_track earth_moonX = { "earth:moonX", earth_moonX_keys,4};
static track_key earth_moonY_keys[] = {{ 920, 0.000000, KEY_STEP}, };
const sync_track earth_moonY = { "earth:moonY", earth_moonY_keys,1};
static track_key earth_moonScale_keys[] = {{ 920, 8.000000, KEY_STEP}, };
const sync_track earth_moonScale = { "earth:moonScale", earth_moonScale_keys,1};
static track_key earth_moonIllumination_keys[] = {{ 920, 0.000000, KEY_LINEAR}, { 922, 0.000000, KEY_LINEAR}, { 1000, 1.000000, KEY_LINEAR}, { 1032, 1.000000, KEY_LINEAR}, { 1112, 0.300000, KEY_STEP}, };
const sync_track earth_moonIllumination = { "earth:moonIllumination", earth_moonIllumination_keys,5};
static track_key gate_x_keys[] = {{ 0, 0.000000, KEY_LINEAR}, { 1121, 471.000000, KEY_LINEAR}, { 1240, 388.000000, KEY_STEP}, };
const sync_track gate_x = { "gate:x", gate_x_keys,3};
static track_key gate_y_keys[] = {{ 0, 0.000000, KEY_LINEAR}, { 1121, 130.000000, KEY_LINEAR}, { 1240, 52.000000, KEY_STEP}, };
const sync_track gate_y = { "gate:y", gate_y_keys,3};
static track_key gate_z_keys[] = {{ 0, 0.000000, KEY_LINEAR}, { 1121, 280.000000, KEY_LINEAR}, { 1240, 43.000000, KEY_STEP}, };
const sync_track gate_z = { "gate:z", gate_z_keys,3};
static track_key gate_scale_keys[] = {{ 0, 1.000000, KEY_LINEAR}, { 1121, 8.000000, KEY_LINEAR}, { 1240, 8.000000, KEY_STEP}, };
const sync_track gate_scale = { "gate:scale", gate_scale_keys,3};
static track_key gate_rotX_keys[] = {{ 0, 0.000000, KEY_LINEAR}, { 1121, 10.000000, KEY_LINEAR}, { 1240, 10.000000, KEY_STEP}, };
const sync_track gate_rotX = { "gate:rotX", gate_rotX_keys,3};
static track_key gate_rotY_keys[] = {{ 0, 0.000000, KEY_LINEAR}, { 1121, -42.000000, KEY_LINEAR}, { 1240, -52.000000, KEY_STEP}, };
const sync_track gate_rotY = { "gate:rotY", gate_rotY_keys,3};
static track_key gate_rotZ_keys[] = {{ 0, 0.000000, KEY_LINEAR}, { 1121, 123.000000, KEY_LINEAR}, { 1240, 123.000000, KEY_STEP}, };
const sync_track gate_rotZ = { "gate:rotZ", gate_rotZ_keys,3};
static track_key gate_ringRot_keys[] = {{ 0, 0.000000, KEY_LINEAR}, { 1121, 41.000000, KEY_LINEAR}, { 1200, 52.000000, KEY_RAMP}, { 1216, 90.000000, KEY_RAMP}, { 1224, 230.000000, KEY_RAMP}, { 1232, 440.000000, KEY_RAMP}, { 1242, 70.000000, KEY_STEP}, };
const sync_track gate_ringRot = { "gate:ringRot", gate_ringRot_keys,7};
static track_key credit_x_keys[] = {{ 0, 26.000000, KEY_STEP}, };
const sync_track credit_x = { "credit:x", credit_x_keys,1};
static track_key credit_y_keys[] = {{ 0, 0.000000, KEY_STEP}, };
const sync_track credit_y = { "credit:y", credit_y_keys,1};
static track_key credit_z_keys[] = {{ 0, -260.000000, KEY_STEP}, };
const sync_track credit_z = { "credit:z", credit_z_keys,1};
static track_key credit_rotY_keys[] = {{ 0, -90.000000, KEY_LINEAR}, };
const sync_track credit_rotY = { "credit:rotY", credit_rotY_keys,1};
static track_key credit_size_keys[] = {{ 0, 10.000000, KEY_LINEAR}, };
const sync_track credit_size = { "credit:size", credit_size_keys,1};
static track_key credit_textSize_keys[] = {{ 0, 2.000000, KEY_STEP}, { 319, 1.000000, KEY_RAMP}, { 320, 0.000000, KEY_RAMP}, { 321, 1.000000, KEY_STEP}, { 327, 1.000000, KEY_RAMP}, { 328, 0.000000, KEY_RAMP}, { 329, 1.000000, KEY_STEP}, { 335, 1.000000, KEY_RAMP}, { 336, 0.000000, KEY_RAMP}, { 337, 1.000000, KEY_STEP}, { 343, 1.000000, KEY_RAMP}, { 344, 0.000000, KEY_RAMP}, { 345, 1.000000, KEY_STEP}, { 351, 1.000000, KEY_RAMP}, { 352, 0.000000, KEY_RAMP}, { 353, 1.000000, KEY_STEP}, { 359, 1.000000, KEY_RAMP}, { 360, 0.000000, KEY_RAMP}, { 361, 1.000000, KEY_STEP}, { 367, 1.000000, KEY_RAMP}, { 368, 0.000000, KEY_RAMP}, { 369, 1.000000, KEY_STEP}, { 375, 1.000000, KEY_RAMP}, { 376, 0.000000, KEY_RAMP}, { 377, 1.000000, KEY_STEP}, { 383, 1.000000, KEY_RAMP}, { 384, 0.000000, KEY_RAMP}, { 385, 1.000000, KEY_STEP}, { 391, 1.000000, KEY_RAMP}, { 392, 0.000000, KEY_RAMP}, { 393, 1.000000, KEY_STEP}, { 399, 1.000000, KEY_RAMP}, { 400, 0.000000, KEY_RAMP}, { 401, 1.000000, KEY_STEP}, { 407, 1.000000, KEY_RAMP}, { 408, 0.000000, KEY_RAMP}, { 409, 1.000000, KEY_STEP}, { 415, 1.000000, KEY_RAMP}, { 416, 0.000000, KEY_RAMP}, { 417, 1.000000, KEY_STEP}, { 423, 1.000000, KEY_RAMP}, { 424, 0.000000, KEY_RAMP}, { 425, 1.000000, KEY_STEP}, { 431, 1.000000, KEY_RAMP}, { 432, 0.000000, KEY_RAMP}, { 433, 1.000000, KEY_STEP}, { 439, 1.000000, KEY_RAMP}, { 440, 0.000000, KEY_RAMP}, { 441, 1.000000, KEY_STEP}, { 447, 1.000000, KEY_RAMP}, { 448, 0.000000, KEY_RAMP}, { 449, 1.000000, KEY_STEP}, { 455, 1.000000, KEY_RAMP}, { 456, 0.000000, KEY_RAMP}, { 457, 1.000000, KEY_STEP}, { 463, 1.000000, KEY_RAMP}, { 464, 0.000000, KEY_RAMP}, { 465, 1.000000, KEY_STEP}, { 471, 1.000000, KEY_RAMP}, { 472, 0.000000, KEY_RAMP}, { 473, 1.000000, KEY_STEP}, { 479, 1.000000, KEY_RAMP}, { 480, 0.000000, KEY_RAMP}, { 481, 1.000000, KEY_STEP}, { 487, 1.000000, KEY_RAMP}, { 488, 0.000000, KEY_RAMP}, { 489, 1.000000, KEY_STEP}, { 495, 1.000000, KEY_RAMP}, { 496, 0.000000, KEY_RAMP}, { 497, 1.000000, KEY_STEP}, { 503, 1.000000, KEY_RAMP}, { 504, 0.000000, KEY_RAMP}, { 505, 1.000000, KEY_STEP}, { 511, 1.000000, KEY_RAMP}, { 512, 0.000000, KEY_RAMP}, { 513, 1.000000, KEY_STEP}, { 519, 1.000000, KEY_RAMP}, { 520, 0.000000, KEY_RAMP}, { 521, 1.000000, KEY_STEP}, { 527, 1.000000, KEY_RAMP}, { 528, 0.000000, KEY_RAMP}, { 529, 1.000000, KEY_STEP}, { 535, 1.000000, KEY_RAMP}, { 536, 0.000000, KEY_RAMP}, { 537, 1.000000, KEY_STEP}, { 543, 1.000000, KEY_RAMP}, { 544, 0.000000, KEY_RAMP}, { 545, 1.000000, KEY_STEP}, };
const sync_track credit_textSize = { "credit:textSize", credit_textSize_keys,88};
static track_key credit_textX_keys[] = {{ 0, -11.000000, KEY_LINEAR}, };
const sync_track credit_textX = { "credit:textX", credit_textX_keys,1};
static track_key credit_textY_keys[] = {{ 0, 5.000000, KEY_STEP}, };
const sync_track credit_textY = { "credit:textY", credit_textY_keys,1};
static track_key credit_between_keys[] = {{ 0, 42.000000, KEY_LINEAR}, };
const sync_track credit_between = { "credit:between", credit_between_keys,1};
static track_key credit_index_keys[] = {{ 320, 0.000000, KEY_STEP}, { 328, 1.000000, KEY_STEP}, { 336, 2.000000, KEY_STEP}, { 344, 3.000000, KEY_STEP}, { 352, 4.000000, KEY_STEP}, { 360, 5.000000, KEY_STEP}, { 368, 0.000000, KEY_STEP}, { 376, 1.000000, KEY_STEP}, { 384, 2.000000, KEY_STEP}, { 392, 3.000000, KEY_STEP}, { 400, 4.000000, KEY_STEP}, { 408, 5.000000, KEY_STEP}, { 416, 0.000000, KEY_STEP}, { 424, 1.000000, KEY_STEP}, { 432, 2.000000, KEY_STEP}, { 440, 3.000000, KEY_STEP}, { 448, 4.000000, KEY_STEP}, { 456, 5.000000, KEY_STEP}, { 464, 0.000000, KEY_STEP}, { 472, 1.000000, KEY_STEP}, { 480, 2.000000, KEY_STEP}, { 488, 3.000000, KEY_STEP}, { 496, 4.000000, KEY_STEP}, { 504, 5.000000, KEY_STEP}, { 512, 0.000000, KEY_STEP}, { 520, 1.000000, KEY_STEP}, { 528, 2.000000, KEY_STEP}, { 536, 3.000000, KEY_STEP}, { 544, 4.000000, KEY_STEP}, };
const sync_track credit_index = { "credit:index", credit_index_keys,29};
#endif
 // SYNC_PLAYER