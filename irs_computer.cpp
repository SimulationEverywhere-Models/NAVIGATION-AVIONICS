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
#include "irs_computer.h"
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/** public functions **/

/*******************************************************************
 * Function Name: IRS
 * Description: IRS constructor
 ********************************************************************/
IRScomputer::IRScomputer( const string &name )
: Atomic( name )
, port_in_power( addInputPort( "port_in_power" ) )
, port_in_gyro_x( addInputPort( "port_in_gyro_x" ) )
, port_in_gyro_y( addInputPort( "port_in_gyro_y" ) )
, port_in_gyro_z( addInputPort( "port_in_gyro_z" ) )
, port_in_gps_x( addInputPort( "port_in_gps_x" ) )
, port_in_gps_y( addInputPort( "port_in_gps_y" ) )
, port_in_gps_z( addInputPort( "port_in_gps_z" ) )
, port_out_irs_x( addOutputPort( "port_out_irs_x" ) )
, port_out_irs_y( addOutputPort( "port_out_irs_y" ) )
, port_out_irs_z( addOutputPort( "port_out_irs_z" ) )
//, irs_consec_gps_time_needed_for_hybrid(0,2,0,0) //need to have valid gps for 2 minutes
, irs_consec_gps_time_needed_for_hybrid(0,0,5,0) //changed from 2 minutes to 5 seconds to speed up testing.
{


	string irs_output_period_tmp( MainSimulator::Instance().getParameter( description(), "irs_output_period" ) ) ;
	string irs_align_period_tmp( MainSimulator::Instance().getParameter( description(), "irs_align_period" ) ) ;
	string gps_output_period_tmp( MainSimulator::Instance().getParameter( description(), "gps_output_period" ) ) ;

	if (irs_output_period_tmp != "")
	{
		irs_output_period = irs_output_period_tmp ;
	}

	if (irs_align_period_tmp != "")
	{
		irs_align_period = irs_align_period_tmp ;
	}
	if (gps_output_period_tmp != "")
	{
		gps_output_period = gps_output_period_tmp ;
	}


	// set the initial position of our IRS.
	irs_x_value = 1 ;
	irs_y_value = 2 ;
	irs_z_value = 3 ;

	irs_gyro_x = SENTINAL ;
	irs_gyro_y = SENTINAL ;
	irs_gyro_z = SENTINAL ;

	irs_gps_x = SENTINAL ;
	irs_gps_y = SENTINAL ;
	irs_gps_z = SENTINAL ;

	irs_last_gps_received_time = Time::Zero ;
	irs_cur_gps_received_time = Time::Zero ;

}

/*******************************************************************
 * Function Name: initFunction
 * Description: Do nothing when the IRS starts, we wait for power.
 ********************************************************************/
Model &IRScomputer::initFunction()
{
	cout << "IRS_STATE_OFF (INIT)" << endl ;
	irs_state = IRS_STATE_OFF ;
	this->passivate();
	return *this ;
}

/*******************************************************************
 * Function Name: externalFunction
 * Description:
 ********************************************************************/
Model &IRScomputer::externalFunction( const ExternalMessage &msg )
{
	int new_power_state = POWER_OFF ;

	if( msg.port() == port_in_power )
	{
		new_power_state = static_cast < int > (msg.value());

		if (new_power_state == POWER_ON &&  irs_state == IRS_STATE_OFF )
		{
			cout << msg.time() << " IRS_STATE_ALIGN"  << endl ;
			//we enter ALIGN mode
			irs_state = IRS_STATE_ALIGN ;

			// have to wait in align mode for a while
			holdIn( active, irs_align_period );
		}
		else if (new_power_state == POWER_OFF && ( irs_state == IRS_STATE_ALIGN || irs_state == IRS_STATE_ALIGN_WAITING_FOR_GYROS || irs_state == IRS_STATE_NAV || irs_state == IRS_STATE_HYBRID) )
		{
			cout << msg.time() << " IRS_STATE_OFF"  << endl ;
			// now we are turned off

			irs_gyro_x = SENTINAL ;
			irs_gyro_y = SENTINAL ;
			irs_gyro_z = SENTINAL ;

			irs_gps_x = SENTINAL ;
			irs_gps_y = SENTINAL ;
			irs_gps_z = SENTINAL ;

			irs_state = IRS_STATE_OFF ;
			passivate();
		}
	}

	if ( msg.port() == port_in_gyro_x || msg.port() == port_in_gyro_y || msg.port() == port_in_gyro_z )
	{


		/* save the gyro inputs to the irs_computer and update the IRS
		 * position with the gyro info if we are in the right state to do so (nav or hybrid)
		 * Use the gyro acceleration to move the IRS in space. Here we simply add the numbers
		 * this is technically adding a force/acceleration value to a position value, it could be
		 * expanded in the future
		 */
		if( msg.port() == port_in_gyro_x )
		{
			irs_gyro_x = static_cast < int > (msg.value());
			if ( irs_state > IRS_STATE_ALIGN_WAITING_FOR_GYROS )
			{
				irs_x_value += irs_gyro_x ;
			}
		}

		if( msg.port() == port_in_gyro_y )
		{
			irs_gyro_y = static_cast < int > (msg.value());
			if ( irs_state > IRS_STATE_ALIGN_WAITING_FOR_GYROS )
			{
				irs_y_value += irs_gyro_y ;
			}
		}

		if( msg.port() == port_in_gyro_z )
		{
			irs_gyro_z = static_cast < int > (msg.value());
			if ( irs_state > IRS_STATE_ALIGN_WAITING_FOR_GYROS )
			{
				irs_z_value += irs_gyro_z ;
			}
		}


		if (IRS_STATE_ALIGN_WAITING_FOR_GYROS == irs_state )
		{
			// if we are waiting for the gyros to all spin up

			if (irs_gyro_x != SENTINAL && irs_gyro_y != SENTINAL && irs_gyro_z != SENTINAL)
			{
				cout << msg.time() << " IRS_STATE_NAV"  << endl ;
				// they are all spun up, we can go into nav mode
				irs_state = IRS_STATE_NAV ;
				holdIn( active, irs_output_period );
			}
			else
			{
				// still not spun up, stay keep waiting
				passivate();
			}
		}
		else if (irs_state > IRS_STATE_OFF)
		{
			// we are not waiting for the gyros to spin up after completing the irs tests, so
			// we don't want to change the time advance here, only process the input
			// so set the time advance to the next scheduled one
			holdIn( active, nextChange() );
		}
		else
		{
			// irs is off
			passivate();

		}
	}


	if( msg.port() == port_in_gps_x || msg.port() == port_in_gps_y || msg.port() == port_in_gps_z )
	{
		irs_cur_gps_received_time  = msg.time() ;

		irs_local_gps_delta_time = irs_cur_gps_received_time -  irs_last_gps_received_time ;

		// check to see if we have continuous gps reception, if so, add it to the counter
		if ( irs_local_gps_delta_time <= gps_output_period )
		{
			irs_total_valid_gps_time = irs_total_valid_gps_time +  irs_local_gps_delta_time;
		}
		else
		{
			irs_total_valid_gps_time = Time::Zero ;
		}

		irs_last_gps_received_time = irs_cur_gps_received_time ;

		/* save the gps inputs and if we are in hybrid mode, each time we get a gps
		 * input, we resync the current IRS position to that.
		 */

		if( msg.port() == port_in_gps_x )
		{
			irs_gps_x = static_cast < int > (msg.value());
			if ( IRS_STATE_HYBRID == irs_state )
			{
				irs_x_value = irs_gps_x ;
			}
		}
		else if( msg.port() == port_in_gps_y )
		{
			irs_gps_y = static_cast < int > (msg.value());
			if ( IRS_STATE_HYBRID == irs_state )
			{
				irs_y_value = irs_gps_y ;
			}
		}
		else if( msg.port() == port_in_gps_z )
		{
			irs_gps_z = static_cast < int > (msg.value());
			if ( IRS_STATE_HYBRID == irs_state )
			{
				irs_z_value = irs_gps_z ;
			}
		}

		// we don't want to change the time advance here, only process the input
		// so set the time advance to the next scheduled one
		if (irs_state > IRS_STATE_OFF)
		{
			// we don't want to change the time advance here, only process the input
			// so set the time advance to the next scheduled one
			holdIn( active, nextChange() );
		}
		else
		{
			// irs is off
			passivate();

		}
	}

	return *this;
}

/*******************************************************************
 * Function Name: internalFunction
 * Description:
 ********************************************************************/
Model &IRScomputer::internalFunction( const InternalMessage &msg )
{
	if ( IRS_STATE_OFF == irs_state )
	{
		// we should be in a passive state if we are off, so we should never get here
		cout << msg.time() << " ERRRRRROORRR"  << endl ;
	}
	else if ( IRS_STATE_ALIGN  == irs_state)
	{


		if ( irs_gyro_x != SENTINAL && irs_gyro_y != SENTINAL && irs_gyro_z != SENTINAL )
		{

			cout << msg.time() << " IRS_STATE_NAV"  << endl ;
			// we are in here because we have waited for the IRS to run it's power up tests
			// and we have information coming in from each of the gyros so we
			// can now transition to NAV mode

			irs_state = IRS_STATE_NAV ;
			holdIn( active, irs_output_period );
		}
		else
		{
			cout << msg.time() << " IRS_STATE_ALIGN_WAITING_FOR_GYROS" << endl ;
			// we are in here because we have waited for the IRS to run it's power up tests
			// and we DON'T have information coming in from each of the gyros so we
			irs_state = IRS_STATE_ALIGN_WAITING_FOR_GYROS ;
			passivate();
		}

	}
	else if (  IRS_STATE_NAV  == irs_state)
	{

		if ( irs_total_valid_gps_time >= irs_consec_gps_time_needed_for_hybrid )
		{
			cout << msg.time() << " IRS_STATE_HYBRID"  << endl ;

			irs_state = IRS_STATE_HYBRID ;
		}
		holdIn( active, irs_output_period );

	}
	else if ( IRS_STATE_HYBRID == irs_state)
	{


		// if we lost GPS for a bit then we have to drop back to nav
		if ( irs_total_valid_gps_time < irs_consec_gps_time_needed_for_hybrid )
		{
			cout << msg.time() << " IRS_STATE_NAV"  << endl ;

			irs_state = IRS_STATE_NAV ;
		}
		holdIn( active, irs_output_period );

	}




	return *this ;
}

/*******************************************************************
 * Function Name: outputFunction
 * Description:
 ********************************************************************/
Model &IRScomputer::outputFunction( const InternalMessage &msg )
{
	if (  IRS_STATE_NAV == irs_state || IRS_STATE_HYBRID == irs_state)
	{
		sendOutput( msg.time(), port_out_irs_x, irs_x_value) ;
		sendOutput( msg.time(), port_out_irs_y, irs_y_value) ;
		sendOutput( msg.time(), port_out_irs_z, irs_z_value) ;
	}
	return *this ;
}
