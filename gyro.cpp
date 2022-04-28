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

/** include files **/
#include "gyro.h"
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )
#include <math.h>
#include <ctime>

/** public functions **/

/*******************************************************************
* Function Name: Gyro
* Description: Gyro constructor
********************************************************************/
Gyro::Gyro( const string &name )
: Atomic( name )
, port_in_power( addInputPort( "port_in_power" ) )
, port_out_accel( addOutputPort( "port_out_accel" ) )
//, gyro_time_delay( 0, 0, 0, 50 )
{
	string gyro_output_period_tmp( MainSimulator::Instance().getParameter( description(), "gyro_output_period" ) ) ;
	string gyro_spinup_period_tmp( MainSimulator::Instance().getParameter( description(), "gyro_spinup_period" ) ) ;

	if (gyro_output_period_tmp != "")
	{
		gyro_output_period = gyro_output_period_tmp ;
	}

	if (gyro_spinup_period_tmp != "")
	{
		gyro_spinup_period = gyro_spinup_period_tmp ;
	}

	// set the initial acceleration of our Gyro.
	gyro_accel_value = 0 ;


}

/*******************************************************************
* Function Name: initFunction
* Description: Do nothing when the Gyro starts, we wait for power.
********************************************************************/
Model &Gyro::initFunction()
{
	this->passivate();
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &Gyro::externalFunction( const ExternalMessage &msg )
{
	int new_power_state = POWER_OFF ;



	if( msg.port() == port_in_power )
	{




		new_power_state = static_cast < int > (msg.value());

		if (new_power_state == POWER_ON &&  gyro_state == GYRO_STATE_OFF )
		{
			//we enter SPINUP mode
			gyro_state = GYRO_STATE_SPINUP ;

			// have to wait in spinup mode for a while
			holdIn( active, gyro_spinup_period );
		}
		else if (new_power_state == POWER_OFF && ( gyro_state == GYRO_STATE_SPINUP || gyro_state == GYRO_STATE_ON) )
		{
			// now we are turned off
			gyro_state = GYRO_STATE_OFF ;
			passivate();
		}
	}

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &Gyro::internalFunction( const InternalMessage & )
//Model &Gyro::internalFunction( const InternalMessage &msg )
{

	if (  GYRO_STATE_ON  == gyro_state)
	{
		/* This is a phony way of making the Gyro update it's values
		 *
		 * NOTE THIS CODE WAS USED FOR GYRO ATOMIC MODEL TESTING
		 * */
		//gyro_accel_value = gyro_accel_value + 1 ;

		/* This is a phony way of making the Gyro update it's values
		 * NOTE THIS CODE WAS USED FOR WHEN GYRO WAS COUPLED INTO THE IRS MODEL
		 * */
		if (rand()>RAND_MAX/2)
		{
			gyro_accel_value = -1 ;
		}
		else
		{
			gyro_accel_value = 1  ;
		}


		holdIn( active, gyro_output_period );

	}
	else if ( GYRO_STATE_SPINUP  == gyro_state)
	{



		// we can now leave SPINUP mode for ON mode
		gyro_state = GYRO_STATE_ON ;
		holdIn( active, gyro_output_period );
	}
	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &Gyro::outputFunction( const InternalMessage &msg )
{

	if (  GYRO_STATE_ON  == gyro_state)
	{





		sendOutput( msg.time(), port_out_accel, gyro_accel_value) ;
	}

	return *this ;
}
