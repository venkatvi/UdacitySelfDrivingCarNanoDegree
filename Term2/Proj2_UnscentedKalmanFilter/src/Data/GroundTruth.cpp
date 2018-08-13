#include "GroundTruth.h"

std::ostream & Data::operator<<(std::ostream& stream, const GroundTruth& pObject){
	stream << pObject.GetPositionX() << "\t";
	stream << pObject.GetPositionY() << "\t";
	stream << pObject.GetVelocityX() << "\t";
	stream << pObject.GetVelocityY() << "\t";
	return stream;
}