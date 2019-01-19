/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <iostream>
#include <memory>
#include <string>
#include <Robot.h>

#include <frc/WPILib.h>
#include <frc/IterativeRobot.h>
#include <frc/SmartDashboard/SendableChooser.h>
#include <frc/SmartDashboard/SmartDashboard.h>
#include <timer.h>
#include <cameraServer/CameraServer.h>
#include <frc/Spark.h>
#include <frc/Joystick.h>
#include <OI.h>
#include <OI.cpp>
#include <SageFunctions.h>

#include <math.h>

//declaring the sticks and whatnot

double speed, sensitivity, turn, targetPosition;

frc::Joystick stick{0};
frc::Spark left{0}, right{2}, box{1};
frc::RobotDrive myRobot{left, right};
frc::AnalogInput ai{0};
frc::AnalogPotentiometer armTilt{1};

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
  // m_autoSelected = SmartDashboard::GetString(
  //     "Auto Selector", kAutoNameDefault);
  std::cout << "Auto selected: " << m_autoSelected << std::endl;

  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  } else {
    // Default Auto goes here
  }
}

void Robot::AutonomousPeriodic() {
  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  } else {
    // Default Auto goes here
  }
}

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() {
  sensitivity = axis(1);
  turn = axis(3);
  speed = /*trueMap(stick.GetRawAxis(1), 1, -1, 1, 0) * */sensitivity;
	targetPosition = trueMap(ai.GetVoltage(), 3.3, 0.0, 0.2, -0.2);

  myRobot.ArcadeDrive(speed, turn);

  if (intake()) {
    box.Set(0.3);
  }
  else if (shooter()) {
    placeCargo();
  }
  else if (!intake() && !shooter()) {
    box.Set(0.0);
  }

  if (armTop()) {
		if (abs(targetPosition) < 0.05) {
			myRobot(0.3, 0.0);
		}
		else if (abs(targetPosition) < 0.1) {
			myRobot(0.2, -targetPosition);
		}
		else if (abs(targetPosition) <= 0.2) {
			myRobot(0.0, -targetPosition);
		}
  }
}

void Robot::TestPeriodic() {}

void placeCargo() {

  box.Set(-0.5);
}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
