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

frc::Joystick stick{0};
double axis(int axisNumber)
{
    return stick.GetRawAxis(axisNumber);
}

bool intake()
{ //intake the ball
    return stick.GetRawButton(5); //left bumper
}

bool shooter()
{ //shoot the ball
    return stick.GetRawButton(6); //right bumper
}

bool armTop()
{ //arm to top level
    return stick.GetPOV() == 0; //D-pad up
}

bool armMiddle()
{ //arm to middle level
    return stick.GetPOV() == 270; //D-pad either side
}

bool armBottom()
{ //arm to bottom level
    return stick.GetPOV() == 180; //D-pad bottom
}

bool armReset()
{ //fully retract arm
    return stick.GetPOV() == 90;
}

bool panelUp()
{ //pick up the panel
    return axis(2) == 1.0; //left trigger
}

bool panelDown()
{ //place the panel
    return axis(3) == 1.0; //right trigger
}

bool armAlign()
{ //align the robot with the rocket
    return stick.GetRawButton(2); //B button
}

OI::OI()
{ //I don't really know why this is here
}