/*
 * SageFunctions.cpp
 *
 *  Created on: Feb 1, 2016
 *      Author: Driver Station
 */

#include "math.h"

double map(double value,double high, double low)
{
	return ((value+fabs(low))/((high+fabs(low))/2))-1;
}
double trueMap(double val,double valHigh,double valLow,double newHigh, double newLow)
{
	double midVal=((valHigh-valLow)/2)+valLow;
	double newMidVal=((newHigh-newLow)/2)+newLow;
	double ratio=(newHigh-newLow)/(valHigh-valLow);
	return (((val-midVal)*ratio)+newMidVal);
}
double inverseMap(double value, double high, double low)
{
	return(((value+1)*((high+fabs(low))/2)))+low;
}
double constrain(double value,double highConstraint,double lowConstraint)
{
	if(value>highConstraint)
		value=highConstraint;
	if(value<lowConstraint)
		value=lowConstraint;
	return value;
}
float deadZone(float value,float deadzone)
{
	if(value>deadzone||value<-deadzone)
		return value;
	else
		return 0.0;
}
