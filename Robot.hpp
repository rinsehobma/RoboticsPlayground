#ifndef _ROBOT_H
#define _ROBOT_H

#include <iostream>
#include <cmath>

#include <vector>

#define NOMINMAX // prevent min max macros from windows.h
#include <Windows.h>
#include <conio.h>

#include <algorithm> // order matters, after Windows.h because std::min macro 


// DIFFERENTIAL DRIVE ROBOT MODEL

class Robot {
private:
	// the following attributes are w.r.t. Robot frame
	double forward_vel;
	double forward_acc;

	double theta_vel;	// [rad/s^2]
	double theta_acc;	// [rad/s^2]

	// the following attributes are w.r.t. World Frame
	// pose
	double x;		// [m]
	double y;		// [m]
	double theta;	// [rad]

	bool use_teleop = false;	// bool to determine if teleop should be used

	// constants
	const double PI = 3.14159;
	const double TIME_STEP = 0.1;		// [s]
	const double FORWARD_VEL_MAX = 5.0;		// [m/s]
	const double FORWARD_ACC_MAX = 2.0;		// [m/s^2]
	const double THETA_VEL_MAX = 0.5;	// [rad/s]
	const double THETA_ACC_MAX = 0.5;	// [rad/s^2]
	
	const double TELEOP_VEL = 5.0;
	const double TELEOP_ROT = 0.05;

public:

	Robot(double x_init, double y_init, double theta_init);		// initialize robot pose
	std::vector<double> GetPosition();							// return robot position [x,y]
	std::vector<double> GetPose();								// return robot pose [x,y,theta]
	std::vector<double> GetVel();								// return robot velocity [forward_vel,theta_vel]
	std::vector<double> GetAcc();								// return robot acceleration [forward_acc,theta_acc]
	void UpdateState(double desired_forward_vel, double desired_theta_vel);
	bool TeleOp();

};





#endif