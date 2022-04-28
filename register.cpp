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

#include "modeladm.h"
#include "mainsimu.h"
#include "gps.h"
#include "gyro.h"
#include "irs_computer.h"


void MainSimulator::registerNewAtomics()
{
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Gyro>(), "Gyro" ) ;

	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<GPS>(), "GPS" ) ;

	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<IRScomputer>(), "IRScomputer" ) ;
}
