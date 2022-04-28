[top]

components : gyro_x@Gyro 
components : gyro_y@Gyro
components : gyro_z@Gyro
components : irs_comp@IRScomputer

in : port_in_power

in : port_in_gps_x
in : port_in_gps_y
in : port_in_gps_z

out : irs_x_pos
out : irs_y_pos
out : irs_z_pos

Link : port_in_power port_in_power@gyro_x
Link : port_in_power port_in_power@gyro_y
Link : port_in_power port_in_power@gyro_z

Link : port_in_power port_in_power@irs_comp

Link : port_out_accel@gyro_x port_in_gyro_x@irs_comp
Link : port_out_accel@gyro_y port_in_gyro_y@irs_comp
Link : port_out_accel@gyro_z port_in_gyro_z@irs_comp

Link : port_in_gps_x port_in_gps_x@irs_comp
Link : port_in_gps_y port_in_gps_y@irs_comp
Link : port_in_gps_z port_in_gps_z@irs_comp

Link : port_out_irs_x@irs_comp irs_x_pos
Link : port_out_irs_y@irs_comp irs_y_pos
Link : port_out_irs_z@irs_comp irs_z_pos

[gyro_x]
gyro_output_period : 00:00:00:010
gyro_spinup_period : 00:00:20:000

[gyro_y]
gyro_output_period : 00:00:00:010
gyro_spinup_period : 00:00:20:000

[gyro_z]
gyro_output_period : 00:00:00:010
gyro_spinup_period : 00:00:20:000

[irs_comp]
gps_output_period : 00:00:00:100
irs_output_period : 00:00:00:020
irs_align_period : 00:02:00:000 
