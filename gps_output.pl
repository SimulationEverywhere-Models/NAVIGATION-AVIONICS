$hour = 1 ; 
$minute = 19; 
$second = 2;
$milisecond = 000 ; 



for ($minute =19 ; $minute <= 22 ; $minute++)
{

	for ($second  = 0 ; $second  <= 59 ; $second++)
	{
		for ($milisecond = 0 ; $milisecond < 1000 ; $milisecond = $milisecond+100 )
		{


			printf( "%02s:%02s:%02s:%03s port_in_gps_x 5\n" , $hour , $minute , $second, $milisecond ) ;
			printf( "%02s:%02s:%02s:%03s port_in_gps_y 6\n" , $hour , $minute , $second, $milisecond ) ;
			printf( "%02s:%02s:%02s:%03s port_in_gps_z 7\n" , $hour , $minute , $second, $milisecond ) ;
		}
	}
}