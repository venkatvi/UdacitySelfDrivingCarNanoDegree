#include "RadarMeasurement.h"
/** getStateData method converts measurement to State
*/
State RadarMeasurement::getStateData() const{

	float x = cos(mTheta_) * mRho_;
	float y = sin(mTheta_) * mRho_;
	float vx = cos(mTheta_) * mRhoDot_;
	float vy = sin(mTheta_) * mRhoDot_;
	State pState(x, y, vx, vy);
	return pState;
}

/** print method outputs rho, theta, rho_dot to
* output stream
* @param pStream ostream 
*/
void RadarMeasurement::print(std::ostream& pStream) const{
	pStream << mRho_ << ", ";
	pStream << mTheta_ << ", "; 
	pStream << mRhoDot_ << ", ";
}