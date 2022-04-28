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

/** include files **/
#include "gps.h"
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/** public functions **/

/*******************************************************************
* Function Name: GPS
* Description: GPS constructor
********************************************************************/
GPS::GPS( const string &name )
: Atomic( name )
, port_in_power( addInputPort( "port_in_power" ) )
, port_out_gps_x( addOutputPort( "port_out_gps_x" ) )
, port_out_gps_y( addOutputPort( "port_out_gps_y" ) )
, port_out_gps_z( addOutputPort( "port_out_gps_z" ) )
//, gps_time_delay( 0, 0, 0, 50 )
{
	string gps_output_period_tmp( MainSimulator::Instance().getParameter( description(), "gps_output_period" ) ) ;
	string gps_acq_period_tmp( MainSimulator::Instance().getParameter( description(), "gps_acq_period" ) ) ;

	if (gps_output_period_tmp != "")
	{
		gps_output_period = gps_output_period_tmp ;
	}

	if (gps_acq_period_tmp != "")
	{
		gps_acq_period = gps_acq_period_tmp ;
	}

	// set the initial position of our GPS.
	gps_x_value = 100 ;
	gps_y_value = 200 ;
	gps_z_value = 300 ;

}

/*******************************************************************
* Function Name: initFunction
* Description: Do nothing when the GPS starts, we wait for power.
********************************************************************/
Model &GPS::initFunction()
{
	gps_state = GPS_STATE_OFF ;
	this->passivate();
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &GPS::externalFunction( const ExternalMessage &msg )
{
	int new_power_state = POWER_OFF ;

	if( msg.port() == port_in_power )
	{
		new_power_state = static_cast < int > (msg.value());

		if (new_power_state == POWER_ON &&  gps_state == GPS_STATE_OFF )
		{
			//we enter ACQ mode
			gps_state = GPS_STATE_ACQ ;

			// have to wait in acq mode for a while
			holdIn( active, gps_acq_period );
		}
		else if (new_power_state == POWER_OFF && ( gps_state == GPS_STATE_ACQ || gps_state == GPS_STATE_NAV) )
		{
			// now we are turned off
			gps_state = GPS_STATE_OFF ;
			passivate();
		}
	}

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &GPS::internalFunction( const InternalMessage & )
{

	if (  GPS_STATE_NAV  == gps_state)
	{
		/* This is a phony way of making the GPS update it's values */
		gps_x_value = gps_x_value + 1 ;
		gps_y_value = gps_y_value - 1 ;
		gps_z_value = gps_z_value + 2 ;

		holdIn( active, gps_output_period );
	}
	else if ( GPS_STATE_ACQ  == gps_state)
	{
		// we can now leave ACQ mode for NAV mode
		gps_state = GPS_STATE_NAV ;
		holdIn( active, gps_output_period );
	}
	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &GPS::outputFunction( const InternalMessage &msg )
{
	if (  GPS_STATE_NAV  == gps_state)
	{
		sendOutput( msg.time(), port_out_gps_x, gps_x_value) ;
		sendOutput( msg.time(), port_out_gps_y, gps_y_value) ;
		sendOutput( msg.time(), port_out_gps_z, gps_z_value) ;
	}
	return *this ;
}
