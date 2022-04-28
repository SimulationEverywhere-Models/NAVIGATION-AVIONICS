/*******************************************************************
*
*  DESCRIPTION: Gyro Navigation Avionics Atomic Model
*
*  AUTHOR: Ken Edwards
*
*  EMAIL: ken@kje.ca
*
*  DATE: November 2009
*
*******************************************************************/

#ifndef __GYRO_H
#define __GYRO_H

#define GYRO_STATE_OFF    0
#define GYRO_STATE_SPINUP 1
#define GYRO_STATE_ON     2

#define POWER_OFF 0
#define POWER_ON  1

#include "atomic.h"     // class Atomic

class Gyro : public Atomic
{
public:
	Gyro( const string &name = "Gyro" );					//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &port_in_power ;
	      Port &port_out_accel ;

	int gyro_state ;

	int gyro_accel_value ;


	Time gyro_output_period ;
	Time gyro_spinup_period ;


};	// class Gyro

// ** inline ** //
inline
string Gyro::className() const
{
	return "Gyro" ;
}

#endif   //__GYRO_H
