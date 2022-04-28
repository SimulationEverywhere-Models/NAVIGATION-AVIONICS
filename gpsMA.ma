[top]

components : gps@GPS 

out : port_out_gps_x
out : port_out_gps_y
out : port_out_gps_z

in : port_in_power

Link : port_in_power port_in_power@GPS

Link : port_out_gps_x@GPS port_out_gps_x
Link : port_out_gps_y@GPS port_out_gps_y
Link : port_out_gps_z@GPS port_out_gps_z

[gps]
gps_output_period : 00:00:00:100
gps_acq_period : 00:00:20:000 
