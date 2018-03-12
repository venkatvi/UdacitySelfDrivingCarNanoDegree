#include "./LaserMeasurement.h"
/** getStateData function converts measurement
* to State object
*/
State LaserMeasurement::getStateData() const {
	State pState(mPositionX_, mPositionY_, 0, 0);
	return pState;
}

/** print function writes member variables to 
* ostream
*/
void LaserMeasurement::print(std::ostream& pStream) const{
	pStream << mPositionX_ << ", ";
	pStream << mPositionY_ << ", ";
}