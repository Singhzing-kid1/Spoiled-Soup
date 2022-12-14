// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// motorA2              motor         12              
// motorA1              motor         14              
// motorB2              motor         13              
// motorB1              motor         11              
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----        
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void forwardBackward(double axis){
    motorA1.spin(forward, axis, percent);
    motorA2.spin(reverse, axis, percent);
    motorB1.spin(reverse, axis, percent);
    motorB2.spin(forward, axis, percent);  
}

void strafe(double axis){
    motorA1.spin(reverse, axis, percent);
    motorA2.spin(forward, axis, percent);
    motorB1.spin(reverse, axis, percent);
    motorB2.spin(forward, axis, percent);  
}

void pointTurn(double axis){
    motorA1.spin(reverse, axis, percent);
    motorA2.spin(reverse, axis, percent);
    motorB1.spin(forward, axis, percent);
    motorB2.spin(forward, axis, percent);  
}

void diagonalA  (double axis1, double axis2){
    motorA1.spin(forward, (axis1 + axis2)/2, percent);
    motorA2.spin(reverse, (axis1 + axis2)/2, percent);
}

void diagonalB(double axis1, double axis2){
    motorB1.spin(reverse, (axis1 + axis2)/2, percent);
    motorB2.spin(forward, (axis1 + axis2)/2, percent);
}

void stopAll(){
    driveTrain.stop();
    pickerUpper.stop();
}

void usercontrol(void) {
  // User control code here, inside the loop

  double axis3 = Controller1.Axis3.position(); // put the values of the axises into variable's
  double axis4 = Controller1.Axis4.position();
  double axis1 = Controller1.Axis1.position();

  bool axis3Cond1;
  bool axis3Cond2;
  bool axis3Cond3;

  bool axis4Cond1;
  bool axis4Cond2;
  bool axis4Cond3;

  bool mixCond1;
  bool mixCond2;
  bool mixCond3;
  bool mixCond4;


  bool axis1Cond1;
  bool axis1Cond2;

  
  bool positiveEncoders; 
  bool negativeEncoders;
  bool encodersAreWithinRange; 

  while (1) {

    axis3Cond1 = axis3 > 10 ? true : false; // get possible Conditions for axis 3
    axis3Cond2 = axis3 < -10 ? true : false;
    axis3Cond3 = axis3 == 0 ? true : false;

    axis4Cond1 = axis4 > 10 ? true : false; // get possible Conditions for axis 4
    axis4Cond2 = axis4 < -10 ? true : false;
    axis4Cond3 = axis4 == 0 ? true : false;

    mixCond1 = axis3Cond1 && axis4Cond1 ? true : false; // get possible Conditions for axis 3 and 4
    mixCond2 = axis3Cond2 && axis4Cond2 ? true : false;
    mixCond3 = axis3Cond1 && axis4Cond2 ? true : false;
    mixCond4 = axis3Cond2 && axis4Cond1 ? true : false;

    axis1Cond1 = axis1 > 10 ? true : false; // get possible Conditions for axis 1
    axis1Cond2 = axis1 < -10 ? true : false;

    positiveEncoders = pickerUpper.rotation(degrees) >= 0 && pickerUpper.rotation(degrees) <= 360 ? true : false;
    negativeEncoders = pickerUpper.rotation(degrees) <= 0 && pickerUpper.rotation(degrees) >= -360 ? true : false;
    encodersAreWithinRange = positiveEncoders || negativeEncoders ? true : false;

    Controller1.ButtonUp.pressing() ? pickerUpper.spin(forward, 100, percent) : Controller1.ButtonDown.pressing() ? pickerUpper.spin(reverse, 100, percent) : doNothing(); // is the down button pressed?

    !encodersAreWithinRange ? pickerUpper.resetRotation() : doNothing(); // if the encoder value is not within range, reset the encoder value

    (axis3Cond1 || axis3Cond2) && axis4Cond3 ? forwardBackward(axis3) : (axis4Cond1 || axis4Cond2) && axis3Cond3 ? strafe(axis4) : axis1Cond1 || axis1Cond2 ? pointTurn(axis1) : mixCond1 || mixCond2 ? diagonalA(axis3, axis4) : mixCond3 ? diagonalB(axis3, -1*axis4) : mixCond4 ? diagonalB(-1*axis3, axis4) : stopAll();

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
