/*
 * SageFunctions.h
 *
 *  Created on: Feb 1, 2016
 *      Author: Driver Station
 */

#ifndef SRC_SAGEFUNCTIONS_H_
#define SRC_SAGEFUNCTIONS_H_


double map(double value,double high, double low);
double trueMap(double val,double valHigh,double valLow,double newHigh, double newLow);
double inverseMap(double value, double high, double low);
double constrain(double value,double highConstraint,double lowConstraint);
float deadZone(float value,float deadzone);


#endif /* SRC_SAGEFUNCTIONS_H_ */
