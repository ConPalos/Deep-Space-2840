/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/
#include <iostream>
#include <memory>
#include <string>

#include "Robot.h"
#include <frc/WPILib.h>
#include <frc/IterativeRobot.h>
#include <frc/SmartDashboard/SendableChooser.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <timer.h>
#include <cameraServer/CameraServer.h>
#include <frc/Spark.h>
#include <frc/Joystick.h>
#include <OI.h>
#include <OI.cpp>
#include <SageFunctions.h>

#include <math.h>

frc::Joystick stick{0};
frc::Spark left{0}, right{2}, box{1};
frc::RobotDrive myRobot{left, right};
frc::AnalogInput ai{0};
frc::AnalogPotentiometer armTilt{1};

double leftJoy, rightJoy, targetPosition; /*
leftJoy and RightJoy are the left joystick and the right joystick respectively.
The axes can be declared using the function leftJoy = axis(axisNumber) or rightJoy = axis(axisNumber).
targetPosition returns the X Coordinate of the target object where -2.0 is the furthest left, 0.0 is the center,
and 2.0 is the furthest right
*/
void Robot::RobotInit()
{
  m_chooser.SetDefaultOption(kAutoNameDefault, kAutoNameDefault);
  m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
}

/**
 * This function is called every robot packet, no matter the mode. Use
 * this for items like diagnostics that you want ran during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() {}

/**
 * This autonomous (along with the chooser code above) shows how to select
 * between different autonomous modes using the dashboard. The sendable chooser
 * code works with the Java SmartDashboard. If you prefer the LabVIEW Dashboard,
 * remove all of the chooser code and uncomment the GetString line to get the
 * auto name from the text box below the Gyro.
 *
 * You can add additional auto modes by adding additional comparisons to the
 * if-else structure below with additional strings. If using the SendableChooser
 * make sure to add them to the chooser code above as well.
 */
void Robot::AutonomousInit()
{
  m_autoSelected = m_chooser.GetSelected();
  // m_autoSelected = SmartDashboard::GetString(
  //     "Auto Selector", kAutoNameDefault);
  std::cout << "Auto selected: " << m_autoSelected << std::endl;

  if (m_autoSelected == kAutoNameCustom)
  {
    // Custom Auto goes here
  }
  else
  {
    // Default Auto goes here
  }
}

void Robot::AutonomousPeriodic()
{
  if (m_autoSelected == kAutoNameCustom)
  {
    // Custom Auto goes here
  }
  else
  {
    // Default Auto goes here
  }
}

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic()
{
  leftJoy = axis(1);                                              //declares leftJoy as axis number 1
  rightJoy = axis(3);                                             //declares rightJoy as axis number 3
  targetPosition = trueMap(ai.GetVoltage(), 3.3, 0.0, 0.2, -0.2); //see declarations for details
  myRobot.TankDrive(leftJoy, rightJoy);                           //declares leftJoy and rightJoy as the left and right side, respectively

  if (intake())
  {
    box.Set(0.3); //moves the box with a speed of 0.3 if intake button is pressed
    //the intake button can be found in OI.cpp
  }
  else if (shooter())
  {
    placeCargo(); //moves the box with a speed of -0.3 if shooter button is pressed
    //the shooter button can be found in OI.cpp
  }
  else if (!intake() && !shooter())
  {
    box.Set(0.0); //if neither are pressed, nothing happens? I'm unclear on this one.
  }

  if (armAlign())
  { //if somebody presses the armAlign button
    if (abs(targetPosition) < 0.05)
    {                             //if the camera says that the object is less than 0.05 away from the center
      myRobot.TankDrive(0.3, 0.3) //go forward with a speed of 0.3
    }
    else if (targetPosition > -0.1 && targetPosition < 0)
    {                                                    //if the camera says that the object is less then 0.1 to the left
      myRobot.TankDrive(0.2 + abs(targetPosition), 0.2); //go left with a speed of 0.2
      //and right with a speed of 0.2 + the absolute value of targetPosition
    }
    else if (targetPosition < 0.1 && targetPosition > 0)
    { //this one is the inverse of the last one
      myRobot.TankDrive(0.2, 0.2+abs(targetPosition);
    }
    else if (targetPosition <= 0.2 && targetPosition > 0)
    {                                            //if the camera says that the object is more than 0.1 to the right
      myRobot.TankDrive(0.0, abs(targetPosition);//turn left with a speed of the absolute value of targetPosition
    }
    else if (targetPosition >= -0.2 && targetPosition < 0)
    { //and vice versa
      myRobot.TankDrive(abs(targetPosition), 0.0);
    }
  }

  void Robot::TestPeriodic() {}

  void placeCargo()
  {
    box.Set(-.5);
  }

#ifndef RUNNING_FRC_TESTS
#endif