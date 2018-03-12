#include "State.h"

/** diff method comptues the difference of this object 
* with another state 
* @param other const State reference
* returns the difference between current state and other state 
*/
Eigen::VectorXd State::diff(const State& other) const{
	Eigen::VectorXd residual = Eigen::VectorXd(4);
	residual << (mPositionX_ - other.getPositionX()),
				(mPositionY_ - other.getPositionY()),
				(mVelocityX_ - other.getVelocityX()),
				(mVelocityY_ - other.getVelocityY());
	return residual;
}

/* operator overload << to output member variables to
* output stream 
*/
std::ostream &operator<<(std::ostream& stream, const State& pObject){
	stream << pObject.mPositionX_ << ", ";
	stream << pObject.mPositionY_ << ", "; 
	stream << pObject.mVelocityX_ << ", ";
	stream << pObject.mVelocityY_ << ", ";
	return stream;
}