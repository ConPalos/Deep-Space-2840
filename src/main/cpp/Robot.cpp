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
#include <frc/Encoder.h>
#include <frc/WPILib.h>
#include <OI.cpp>
#include <frc/PowerDistributionPanel.h>

#include <math.h>

// declaring the sticks and whatnot

double speed, turn;

frc::Spark left{5}, left2{4}, right{0}, right2{1}, pivot1{3}, arm{7}, box{2}, pivot2{6};  // declares the motors
frc::RobotDrive myRobot{left2, left, right2, right};  // left controls left side, right controls right side
//frc::Encoder armTilt{0};  // declares armTilt as the encoder in port 1
//frc::Compressor *compressor = new //frc::DoubleSolenoid panelLift{0, 1}; //declares panelLift as the pneumatic cylinder controlled by ports 1 and 2
//frc::Compressor(0);

void Robot::RobotInit() {
  m_chooser.SetDefaultOption(kAutoNameDefault, kAutoNameDefault);
  m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
  //frc::PowerDistributionPanel ClearStickyFaults();
  
}

/**
 * This function is called every robot packet, no matter the mode. Use
 * this for items like diagnostics that you want ran during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() {
  frc::PowerDistributionPanel ClearStickyFaults();
  turn = -axis(4);   // right stick. use stick(4) if xbox 360
  speed = axis(1);  // right stick. use stick(5) if xbox 360
  //compressor->SetClosedLoopControl(true);
  myRobot.ArcadeDrive(speed, turn);
  if (pivotUp()) {  // if intake button is pressed, move box with a speed of 0.3
    pivot1.Set(-0.25); // out of -1.0 to 1.0
    pivot2.Set(-0.25);
  }
  else if (pivotDown()) {  // if shooter button is pressed, move box with a
    pivot1.Set(0.25);         // speed of -0.5 out of -1.0 to 1.0
    pivot2.Set(0.25);
  }
  else if (!pivotUp() && !pivotDown()) {  // if neither button is pressed, do diddly squat
    pivot1.Set(0.0);
    pivot2.Set(0.0);
  }
  
  if (intake()) {
    //panelLift.Set(frc::DoubleSolenoid::Value::kForward);
    box.Set(0.8);
  }
  else if (shooter()) {
    //panelLift.Set(frc::DoubleSolenoid::Value::kReverse);
    box.Set(-0.8);
  }
  else if (!intake() && !shooter()) {
    box.Set(0.0);
  }

  /*if (armAlign()) {  // if armAlign is pressed
    if (abs(targetPosition()) < 0.05) { // if the object is less that 0.05 away
  from the center on a scale of -0.2 to 0.2 myRobot.ArcadeDrive(0.3, 0.0);  //
  move forward with a speed of 0.3 out of -1.0 to 1.0
    }
    else if (abs(targetPosition()) < 0.1) { // if the object is less than 0.05
  away from the center myRobot.ArcadeDrive(0.2, -targetPosition());  // move
  forward with a speed of 0.2 and turn in the opposite direction of
  targetPosition
    }
    else if (abs(targetPosition()) <= 0.2) { // if the object is less than or
  equal to 0.2 (maximum) away from the center myRobot.ArcadeDrive(0.0,
  -targetPosition());  // turn in the opposite direction of targetPosition which
  moves it towards the object
    }
  }*/
//777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777
  //The encoder will send a PWM signal of width 1 to 4096 microseconds depending on position at 244 Hz
//   if (armReset()) {
    
//   }
   if (stick.GetRawButton(5)) {
     arm.Set(0.3);
   }
   else if (stick.GetRawButton(6)) {
     arm.Set(0.75);
   }
   else if (!stick.GetRawButton(2) && !stick.GetRawButton(6) && !stick.GetRawButton(5)) {
     arm.Set(0.0);
   }
// }

//if (int frc::GenericHID::GetPOV(int pov = 0) const) {
//  arm.Set(0.75);
//}
//else if (frc::GenericHID::GetPOV(int pov = 180) const) {
//  arm.Set(-0.1);
//}
//else if (int frc::GenericHID::GetPOV(int pov = 270) const) {
//  arm.Set(0.3);
//}
//else if (!int frc::GenericHID::GetPOV(int pov = 0) const && !int frc::GenericHID::GetPOV(int pov = 180) const && !int frc::GenericHID::GetPOV(int pov = 270) const) {
//  arm.Set(0.0);
}

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

void Robot::TeleopPeriodic() {}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif