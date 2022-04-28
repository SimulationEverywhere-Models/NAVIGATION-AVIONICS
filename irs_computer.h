/*******************************************************************
*
*  DESCRIPTION: IRS Computer Navigation Avionics Atomic Model
*
*  AUTHOR: Ken Edwards
*
*  EMAIL: ken@kje.ca
*
*  DATE: November 2009
*
*******************************************************************/

#ifndef __IRS_H
#define __IRS_H

#define IRS_STATE_OFF    0
#define IRS_STATE_ALIGN  1
#define IRS_STATE_ALIGN_WAITING_FOR_GYROS 2
#define IRS_STATE_NAV    3
#define IRS_STATE_HYBRID 4

#define POWER_OFF 0
#define POWER_ON  1

#define GPS_RATE .05

#define SENTINAL -9999

#include "atomic.h"     // class Atomic

class IRScomputer : public Atomic
{
public:
	IRScomputer( const string &name = "IRScomputer" );					//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &port_in_power ;

	const Port &port_in_gyro_x ;
	const Port &port_in_gyro_y ;
	const Port &port_in_gyro_z ;

	const Port &port_in_gps_x ;
	const Port &port_in_gps_y ;
	const Port &port_in_gps_z ;

	      Port &port_out_irs_x;
	      Port &port_out_irs_y;
	      Port &port_out_irs_z;

	//int output_time_delay ; // 20 Hz
	int irs_state ;

	int irs_x_value ;
	int irs_y_value ;
	int irs_z_value ;

	int irs_gyro_x ;
	int irs_gyro_y ;
	int irs_gyro_z ;

	int irs_gps_x ;
	int irs_gps_y ;
	int irs_gps_z ;

	Time irs_output_period ;
	Time irs_align_period  ;
	Time irs_last_gps_received_time ;
	Time irs_cur_gps_received_time ;
	Time irs_total_valid_gps_time ;
	Time irs_local_gps_delta_time ;

	Time irs_consec_gps_time_needed_for_hybrid ;
	Time gps_output_period ;



};	// class IRS

// ** inline ** //
inline
string IRScomputer::className() const
{
	return "IRScomputer" ;
}

#endif   //__IRS_H
