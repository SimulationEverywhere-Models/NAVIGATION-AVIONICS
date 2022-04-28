[top]

components : ic@IRScomputer 

out : port_out_irs_x
out : port_out_irs_y
out : port_out_irs_z

in : port_in_power

in : port_in_gps_x
in : port_in_gps_y
in : port_in_gps_z

in : port_in_gyro_x
in : port_in_gyro_y
in : port_in_gyro_z

Link : port_in_power port_in_power@ic

Link : port_in_gps_x port_in_gps_x@ic
Link : port_in_gps_y port_in_gps_y@ic
Link : port_in_gps_z port_in_gps_z@ic

Link : port_in_gyro_x port_in_gyro_x@ic
Link : port_in_gyro_y port_in_gyro_y@ic
Link : port_in_gyro_z port_in_gyro_z@ic

Link : port_out_irs_x@ic port_out_irs_x
Link : port_out_irs_y@ic port_out_irs_y
Link : port_out_irs_z@ic port_out_irs_z

[ic]
gps_output_period : 00:00:00:100
irs_output_period : 00:00:00:020
irs_align_period : 00:02:00:000 
