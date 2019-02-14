/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <OI.h>
#include <SageFunctions.h>
#include <frc/Joystick.h>

//Axis allows you to just type axis(number),
//rather than typing trueMap(stick.GetRawAxis(number), 1.0, -1.0, 1.0, 0.0)
//which takes forever
frc::AnalogInput ai{0};  // declares analog in port 0 as ai
frc::Joystick stick{0};

//double targetPosition() {
//   return trueMap(ai.GetVoltage(), 3.3, 0.0, 0.2, -0.2); //maps the shit on
//   the other shit
//}

double axis(int axisNumber) {
  return stick.GetRawAxis(axisNumber);
}

bool intake() {                  //intake the ball
  return stick.GetRawButton(7);  //left trigger
}

bool shooter() {                 //shoot the ball
  return stick.GetRawButton(8);  //right trigger
}

bool armTop() {                //arm to top level
  //return stick.GetPOV() == 0;  //D-pad up
  return stick.GetRawButton(4);
}

bool armMiddle() {               //arm to middle level
  //return stick.GetPOV() == 270;  //D-pad either side
  return stick.GetRawButton(1);
}

bool armBottom() {               //arm to bottom level
  //return stick.GetPOV() == 180;  //D-pad bottom
  return stick.GetRawButton(2);
}

bool armReset() {                //fully retract arm
  return stick.GetPOV() == 90;
}

bool panelUp() {          //pick up the panel
  return stick.GetRawButton(6);  //right bumper on logitech
}

bool panelDown() {        //place the panel
  return stick.GetRawButton(5);  //left bumper on logitech
}

bool armAlign() {                //align the robot with the rocket
  //return stick.GetRawButton(2);  //B button
}
OI::OI() {
  //I don't really know why this is here
}