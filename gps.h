/*******************************************************************
*
*  DESCRIPTION: GPS Navigation Avionics Atomic Model
*
*  AUTHOR: Ken Edwards
*
*  EMAIL: ken@kje.ca
*
*  DATE: November 2009
*
*******************************************************************/

#ifndef __GPS_H
#define __GPS_H

#define GPS_STATE_OFF 0
#define GPS_STATE_ACQ 1
#define GPS_STATE_NAV 2

#define POWER_OFF 0
#define POWER_ON  1

#include "atomic.h"     // class Atomic

class GPS : public Atomic
{
public:
	GPS( const string &name = "GPS" );					//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &port_in_power ;
	      Port &port_out_gps_x;
	      Port &port_out_gps_y;
	      Port &port_out_gps_z;

	//int output_time_delay ; // 20 Hz
	int gps_state ;

	int gps_x_value ;
	int gps_y_value ;
	int gps_z_value ;

	Time gps_output_period ;
	Time gps_acq_period    ;


};	// class GPS

// ** inline ** //
inline
string GPS::className() const
{
	return "GPS" ;
}

#endif   //__GPS_H
