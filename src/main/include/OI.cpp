/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/
#include <OI.h>
#include <SageFunctions.h>
#include <frc/Joystick.h>

/***************************************************************
 * how to get POV goosy-whatsits:                              *
 * POV is the D-Pad                                            *
 * return stick.getPOV() = 0 will tell you if the up is pressed*
 * use 90 for right, 180, for down, 270 for left,              *
 * -1 for not pressed.                                         *
 * *************************************************************/
//Axis allows you to just type axis(number),
//rather than typing trueMap(stick.GetRawAxis(number), 1.0, -1.0, 1.0, 0.0)
//which takes forever
frc::Joystick stick{0};

//double targetPosition() {
//   return trueMap(ai.GetVoltage(), 3.3, 0.0, 0.2, -0.2); //maps the shit on
//   the other shit
//}

double axis(int axisNumber) {
  return stick.GetRawAxis(axisNumber);
}
bool pivotUp() {
  return stick.GetRawButton(6);
}
bool pivotDown() {
  return axis(3) > 0.25;
}
bool ballIntake() {                  //intake the ball
  return stick.GetRawButton(1);  //left trigger
}

bool shooter() {                 //shoot the ball
  return stick.GetRawButton(2);  //right trigger
}

bool armUp() {                //arm to top level
  //return stick.GetPOV() == 0;  //D-pad up
  return axis(2) > 0.25;
}

bool armUpper() {           // put the arm upper position
  return stick.GetRawButton(7); // two windows button
}

bool armMiddle() {         // put the arm at middle position
  return stick.GetRawButton(8); // three lines button
}

bool armLower() {          // put the arm at lower position
  return stick.GetRawButton(9); // push down on left joystick
}
bool armHold() {               //arm to bottom level
  //return stick.GetPOV() == 180;  //D-pad bottom
  return stick.GetRawButton(5);
}

bool armReset() {                //fully retract arm
  return stick.GetPOV() == 90;
}
bool panelIntake() {
  return stick.GetRawButton(3);
}
bool panelOuttake() {
  return stick.GetRawButton(4);
}
OI::OI() {
  //I don't really know why this is here
}