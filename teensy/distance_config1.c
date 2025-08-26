static void set_config(acc_detector_distance_config_t *config, distance_preset_config_t preset)
{
    // This snippet is generated to be compatible with RSS A121 v1.0.0
    // If there is a version missmatch the snippet might need some modification

    (void)preset;

    acc_detector_distance_config_sensor_set(config, SENSOR_ID);

    acc_detector_distance_config_start_set(config, 0.010f);
    acc_detector_distance_config_end_set(config, 1.000f);
    acc_detector_distance_config_max_step_length_set(config, 0U);

    acc_detector_distance_config_signal_quality_set(config, 15.700f);
    acc_detector_distance_config_max_profile_set(config, ACC_CONFIG_PROFILE_5);
    acc_detector_distance_config_peak_sorting_set(config, ACC_DETECTOR_DISTANCE_PEAK_SORTING_CLOSEST);
    acc_detector_distance_config_reflector_shape_set(config, ACC_DETECTOR_DISTANCE_REFLECTOR_SHAPE_GENERIC);

    acc_detector_distance_config_threshold_method_set(config, ACC_DETECTOR_DISTANCE_THRESHOLD_METHOD_CFAR);
    acc_detector_distance_config_num_frames_recorded_threshold_set(config, 20U);
    acc_detector_distance_config_fixed_threshold_value_set(config, 100.000f);
    acc_detector_distance_config_threshold_sensitivity_set(config, 0.500f);

    acc_detector_distance_config_close_range_leakage_cancellation_set(config, true);
}