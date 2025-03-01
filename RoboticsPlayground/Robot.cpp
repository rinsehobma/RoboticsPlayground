#include "Robot.hpp"

// initialize the robot object with an initial pose
Robot::Robot(double x_init, double y_init, double theta_init) {
	x = x_init;
	y = y_init;
	theta = theta_init;

	forward_vel = 0.0;
	theta_vel = 0.0;

	forward_acc = 0.0;
	theta_acc = 0.0;


};


std::vector<double> Robot::GetPosition() {
	return { x, y };
};

std::vector<double> Robot::GetPose() {
	return { x, y, theta };
};

std::vector<double> Robot::GetVel() {
	return { forward_vel, theta_vel };
};

std::vector<double> Robot::GetAcc() {
	return { forward_acc, theta_acc };
};

void Robot::UpdateState(double desired_forward_vel, double desired_theta_vel) {
	if (!TeleOp()) {
		// update Robot velocities
		double temp_forward_vel = std::min(std::max(desired_forward_vel, 0.0), FORWARD_VEL_MAX);
		double temp_forward_acc = (temp_forward_vel - forward_vel) / TIME_STEP;

		forward_acc = std::min(std::max(temp_forward_acc, -FORWARD_ACC_MAX), FORWARD_ACC_MAX);
		forward_vel = std::min(std::max(forward_vel + forward_acc * TIME_STEP, 0.0), FORWARD_VEL_MAX);

		double temp_theta_vel = std::min(std::max(desired_theta_vel, -THETA_VEL_MAX), THETA_VEL_MAX);
		double temp_theta_acc = (temp_theta_vel - theta_vel) / TIME_STEP;

		theta_acc = std::min(std::max(temp_theta_acc, -THETA_ACC_MAX), THETA_ACC_MAX);
		theta_vel = std::min(std::max(theta_vel + theta_acc * TIME_STEP, -THETA_VEL_MAX), THETA_VEL_MAX);

		// update robot pose w.r.t. World Frame
		theta = theta + theta_vel * TIME_STEP;
		if (theta > PI) {
			theta = theta - 2 * PI;
		}
		else if (theta < -PI) {
			theta = 2 * PI + theta;
		}

		x = x + forward_vel * std::cos(theta);
		y = y + forward_vel * std::sin(theta);
	}
};

bool  Robot::TeleOp() {
	use_teleop = false;

	if (GetAsyncKeyState(VK_UP)) {
		std::cout << "Going forward" << std::endl;
		use_teleop = true;
		x = x + TELEOP_VEL * std::cos(theta);
		y = y + TELEOP_VEL * std::sin(theta);
	}

	if (GetAsyncKeyState(VK_DOWN)) {
		std::cout << "Going backward" << std::endl;
		use_teleop = true;
		x = x - TELEOP_VEL * std::cos(theta);
		y = y - TELEOP_VEL * std::sin(theta);
	}

	if (GetAsyncKeyState(VK_LEFT)) {
		std::cout << "Rotating left" << std::endl;
		use_teleop = true;
		theta = theta + TELEOP_ROT;
	}

	if (GetAsyncKeyState(VK_RIGHT)) {
		std::cout << "Rotating right" << std::endl;
		use_teleop = true;
		theta = theta - TELEOP_ROT;
	}

	return use_teleop;
	
};