/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <Robot.h>
#include <iostream>
#include <memory>
#include <string>

#include <OI.h>
#include <SageFunctions.h>
#include <cameraServer/CameraServer.h>
#include <frc/IterativeRobot.h>
#include <frc/Joystick.h>
#include <frc/SmartDashboard/SendableChooser.h>
#include <frc/SmartDashboard/SmartDashboard.h>
#include <frc/timer.h>
#include <frc/Spark.h>
#include <frc/WPILib.h>
#include <OI.cpp>

#include <math.h>

// declaring the sticks and whatnot

double speed, turn;

frc::Spark left{0}, right{2}, box{1};  // declares the motors
frc::RobotDrive myRobot{left, right};  // left controls left side, right controls right side
frc::AnalogPotentiometer armTilt{1};  // declares armTilt as the potentiometer in port 1
frc::DoubleSolenoid panelLift{1, 2}, arm1{3, 4}, arm2{5, 6};

void Robot::RobotInit() {
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
void Robot::AutonomousInit() {
  m_autoSelected = m_chooser.GetSelected();
  m_autoSelected = frc::SmartDashboard::GetString("Auto Selector", kAutoNameDefault);
  std::cout << "Auto selected: " << m_autoSelected << std::endl;

  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  }
  else {
    // Default Auto goes here
  }
}

void Robot::AutonomousPeriodic() {
  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  }
  else {
    // Default Auto goes here
  }
}

void Robot::TeleopInit() {}

/*
* Every packet sent during the robot's teleoperated mode checks for the condition of the joystick.
* Each function is a check for a specific button, axis, or POV on the joystick. See OI.cpp for the
* button each function is connected to.
*/

void Robot::TeleopPeriodic() {
  turn = axis(2);   // right stick. use stick(4) if xbox 360
  speed = -axis(1);  // right stick. use stick(5) if xbox 360

  myRobot.ArcadeDrive(speed, turn);

  if (intake()) {  // if intake button is pressed, move box with a speed of 0.3
    box.Set(0.3); // out of -1.0 to 1.0
  }
  else if (shooter()) {  // if shooter button is pressed, move box with a
    box.Set(-0.5);         // speed of -0.5 out of -1.0 to 1.0
  }
  else if (!intake() && !shooter()) {  // if neither button is pressed, do diddly squat
    box.Set(0.0);
  }

  if (armAlign()) {  // if armAlign is pressed
    if (abs(targetPosition()) < 0.05) { // if the object is less that 0.05 away from the center on a scale of -0.2 to 0.2
      myRobot.ArcadeDrive(0.3, 0.0);  // move forward with a speed of 0.3 out of -1.0 to 1.0
    }
    else if (abs(targetPosition()) < 0.1) { // if the object is less than 0.05 away from the center
      myRobot.ArcadeDrive(0.2, -targetPosition());  // move forward with a speed of 0.2 and turn in the opposite direction of targetPosition
    }
    else if (abs(targetPosition()) <= 0.2) { // if the object is less than or equal to 0.2 (maximum) away from the center
      myRobot.ArcadeDrive(0.0, -targetPosition());  // turn in the opposite direction of targetPosition which moves it towards the object
    }
  }

  if (armReset()) {
    arm1.Set(frc::DoubleSolenoid::Value::kReverse);
    arm2.Set(frc::DoubleSolenoid::Value::kReverse);
  }
  else if (armBottom()) {
    arm1.Set(frc::DoubleSolenoid::Value::kReverse);
    arm2.Set(frc::DoubleSolenoid::Value::kForward);
  }
  else if (armMiddle()) {
    arm1.Set(frc::DoubleSolenoid::Value::kForward);
    arm2.Set(frc::DoubleSolenoid::Value::kReverse);
  }
  else if (armTop()) {
    arm1.Set(frc::DoubleSolenoid::Value::kForward);
    arm2.Set(frc::DoubleSolenoid::Value::kForward);
  }
}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif