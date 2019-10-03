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
//weeee github
#include <OI.h>
#include <SageFunctions.h>
#include <cameraServer/CameraServer.h>
#include <frc/IterativeRobot.h>
#include <frc/Joystick.h>
#include <frc/SmartDashboard/SendableChooser.h>
#include <frc/SmartDashboard/SmartDashboard.h>
#include <frc/Timer.h>
#include <frc/Spark.h>
#include <frc/Encoder.h>
#include <frc/WPILib.h>
#include <OI.cpp>
#include <frc/PowerDistributionPanel.h>
#include <frc/LiveWindow/LiveWindow.h>
#include "networktables/NetworkTable.h"
#include "networkTables/NetworkTableInstance.h"
//#include "PigeonIMU.h"
#include <ctre/Phoenix.h>

#include <math.h>

// declaring the sticks and whatnot
double speed, turn;

double armScale = 1.0; //THIS IS THE SCALING FACTOR FOR THE ENCODER TO BE CALCULATED ON THURSDAY
/*
                     _____________                   __            
|            |      |                  \        /   |  |          /\
|            |      |                   \      /    |  |         /  \
|            |      |                    \    /     |  |        /_  _\
|————————————|      |————————             \  /      |  |          || 
|            |      |                      ||       |__|          ||
|            |      |                      ||        __           ||
|            |      |_____________         ||       |__|          ||

INSTRUCTIONS:

1) Check the value of count after raising the arm from the ground to the lower cargo.
2) Check the value of count after raising the arm from the ground to the middle cargo.
3) Check the value of count after raising the arm from the ground to the upper cargo.
4) Find the GCF of those numbers. If they're all coprime, go to step seven.
5) The greatest common factor exists! Congratulations! Set armScale to the value of the GCF you just found.
6) Divide your three numbers by the GCF.
7) Put those numbers into the arm sequence. It'll be marked with a large ASCII 1, 2, and 3 respectively.

*/

frc::Spark left{5}, left2{4}, right{0}, right2{1}, pivot1{3}, box1{2}, pivot2{6}, arm{7}, box2{8};  // declares the motors
frc::RobotDrive myRobot{left2, left, right2, right};  // left controls left side, right controls right side
//frc::Talon Talon{7};
//TalonSRX arm = {14};
frc::Encoder *armTilt = new frc::Encoder(0, 1, true, frc::Encoder::EncodingType::k4X); //declares quadrature encoder "armTilt" with ports 0 and 1 without inverted direction
frc::Compressor *compressor = new frc::Compressor(0); //declares compressor
//frc::DoubleSolenoid panelLift{0, 1}; //declares panelLift as the pneumatic cylinder controlled by ports 1 and 2
//armTilt->SetMinRate(1);
frc::Solenoid extend{0};
frc::Solenoid grab{1};
double sensitivity = 1.0;

void Robot::RobotInit() {
  m_chooser.SetDefaultOption(kAutoNameDefault, kAutoNameDefault);
  m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
  //arm.Set(ControlMode::PercentOutput, 0);
  armTilt->Reset();
  armTilt->SetDistancePerPulse(armScale);
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
  //encoder stuff!!
  int count = armTilt->Get();
  double distance = armTilt->GetDistance();
  bool direction = armTilt->GetDirection();
  /*  ___
     /   |
    / /| |
   |_/ | |
       | |
       | |
       | |
    ___| |___
   |_________|
  */
  double lowerTilt = 1.0;
  /*  __________________
     |                  |
     |_______________   |
                     |  |
      _______________|  |
     |                  |
     |   _______________|
     |  |       
     |  |_______________
     |                  |
     |__________________|
  */
  double middleTilt = 1.0;
  /* ____________________
    |                    |
    |_________________   |
                      |  |
     _________________|  |
    |                    |
    |_________________   |
                      |  |
     _________________|  |
    |                    |
    |____________________|
  */
  double upperTilt = 1.0;
  //limelight stuff!!
  std::shared_ptr<NetworkTable> table = nt::NetworkTableInstance::GetDefault().GetTable("limelight");
  double targetOffsetAngle_Horizontal = table->GetNumber("tx",0.0); //returns the offset of the target from the center of the camera in the x direction
  double targetOffsetAngle_Vertical = table->GetNumber("ty",0.0); //returns the offset of the target from the center of the camera in the y direction
  double targetArea = table->GetNumber("ta",0.0); //I have no idea, but the webpage told me to put it here
  double targetSkew = table->GetNumber("ts",0.0); //Ditto
  if (stick.GetPOV(0)) {
    sensitivity = sensitivity + 0.01;
  }
  else if (stick.GetPOV(180)) {
    sensitivity = sensitivity - 0.01;
  }
  if (sensitivity > 1.0) {
    sensitivity = 1.0;
  }
  else if (sensitivity < 0.1) {
    sensitivity = 0.1;
  }
  
  compressor->SetClosedLoopControl(true); //turns on closed loop control, which makes the compressor turn on unless the pressure is about 120 PSI, in which case it turns off
  turn = -axis(4)*sensitivity;  // left stick. use stick(4) if xbox 360
  speed = axis(1)*sensitivity;  // right stick. use stick(5) if xbox 360
  myRobot.ArcadeDrive(speed*sensitivity, turn);
  if (pivotUp()) {  // if intake button is pressed, move box with a speed of 0.3
    pivot1.Set(-0.4); // out of -1.0 to 1.0
    pivot2.Set(-0.4);
  }
  else if (pivotDown()) {  // if shooter button is pressed, move box with a
    pivot1.Set(0.25); // speed of -0.5 out of -1.0 to 1.0
    pivot2.Set(0.25);
  }
  else if (!pivotUp() && !pivotDown()) {  // if neither button is pressed, do diddly squat
    pivot1.Set(0.0);
    pivot2.Set(0.0);
  }
  
  if (ballIntake()) {
    //panelLift.Set(frc::DoubleSolenoid::Value::kForward);
    box1.Set(-0.6);
    box2.Set(0.6);
  }
  else if (shooter()) {
    //panelLift.Set(frc::DoubleSolenoid::Value::kReverse);
    box1.Set(0.6);
    box2.Set(-0.6);
  }
  else if (!ballIntake() && !shooter()) {
    box1.Set(0.0);
    box2.Set(0.0);
  }
  if (panelIntake()) {
    //panelLift.Set(frc::DoubleSolenoid::Value::kReverse);
    extend.Set(true);
    frc::Wait(0.1);
    frc::Wait(0.1);
    frc::Wait(0.1);
    grab.Set(true);
  }
  else if (panelOuttake()) {
    //panelLift.Set(frc::DoubleSolenoid::Value::kForward);
    grab.Set(false);
    frc::Wait(0.1);
    frc::Wait(0.1);
    frc::Wait(0.1);
    extend.Set(false);
  }
  else if (!panelIntake() && !panelOuttake()) {
    //panelLift.Set(frc::DoubleSolenoid::Value::kOff);
    grab.Set(false);
    extend.Set(false);
  }
  if (armUp()) {
    arm.Set(0.6);
  }
  else if (armHold()) {
    arm.Set(-0.3);
  }
  else if (stick.GetRawButton(8)) {
    arm.Set(0.3);
  }
  else if (!armUp() && !armHold() && !stick.GetRawButton(8)) {
    arm.Set(0.0);
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
/*
  if(armReset()) {
    if(distance > 0) {
      arm.Set(-0.5);
    }
    else {
      arm.Set(0.0);
    }
  }
  else if(armLower()) {
    if(distance > lowerTilt) {
      arm.Set(-0.5);
    }
    if(distance < lowerTilt) {
      arm.Set(0.5);
    }
    else {
      arm.Set(0.0);
    }
  }
  else if(armMiddle()) {
    if(distance > middleTilt) {
      arm.Set(-0.5);
    }
    if(distance < middleTilt) {
      arm.Set(0.5);
    }
    else {
      arm.Set(0.0);
    }
  }
  else if(armUpper()) {
    if(distance > upperTilt) {
      arm.Set(-0.25);
    }
    if(distance < upperTilt) {
      arm.Set(0.5);
    }
    else {
      arm.Set(0.0);
    }
  }
  else {
    arm.Set(0);
  }*/

  /*if (armHold()) {
    arm.Set(0.3);
  }
  else if (armUp()) {
    arm.Set(0.7);
  }
  else if (!armHold() && !armUp()) {
    arm.Set(0.0);
  }*/
}
// }


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