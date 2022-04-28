[top]

components : gyro@Gyro 

out : port_out_accel

in : port_in_power

Link : port_in_power port_in_power@Gyro

Link : port_out_accel@Gyro port_out_accel


[gyro]
gyro_output_period : 00:00:00:010
gyro_spinup_period : 00:00:20:000
