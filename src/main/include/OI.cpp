/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <OI.h>
#include <frc/Joystick.h>
//Axis allows you to just type axis(number),
//rather than typing trueMap(stick.GetRawAxis(number), 1.0, -1.0, 1.0, 0.0)
//which takes forever

double axis(int axisNumber) {
    return trueMap(stick.GetRawAxis(axisNumber), 1.0, -1.0, 1.0, 0.0);
};

bool intake() { //intake the ball
    return stick.GetRawButton(1);
}

bool shooter() { //shoot the ball
    return stick.GetRawButton(2);
}

bool armTop() { //arm to top level
    return stick.GetRawButton(3);
}

bool armMiddle() { //arm to middle level
    return stick.GetRawButton(4);
}

bool armBottom() { //arm to bottom level
    return stick.GetRawButton(5);
}

bool armReset() { //fully retract arm
    return stick.GetRawButton(6);
}

bool panelUp() { //pick up the panel
    return stick.GetRawButton(7);
}

bool panelDown() { //place the panel
    return stick.GetRawButton(8);
}

bool armAlign() { //align the robot with the rocket
    return stick.GetRawButton(9);
}

OI::OI() { //I don't really know why this is here
}