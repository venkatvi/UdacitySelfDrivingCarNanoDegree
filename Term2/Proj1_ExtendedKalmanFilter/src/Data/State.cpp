#include "Data/State.h"

/** diff method comptues the difference of this object
* with another state
* @param other const State reference
* returns the difference between current state and other state
*/
Eigen::VectorXd Data::State::diff(const Data::State& other) const {
	Eigen::VectorXd residual = Eigen::VectorXd(4);
	residual << (m_position_x_ - other.GetPositionX()),
	         (m_position_y_ - other.GetPositionY()),
	         (m_velocity_x_ - other.GetVelocityX()),
	         (m_velocity_y_ - other.GetVelocityY());
	return residual;
}

/* operator overload << to output member variables to
* output stream
*/
std::ostream &operator<<(std::ostream& stream, const State& pObject) {
	stream << pObject.m_position_x_ << ", ";
	stream << pObject.m_position_y_ << ", ";
	stream << pObject.m_velocity_x_ << ", ";
	stream << pObject.m_velocity_y_ << ", ";
	return stream;
}