#include "../Data/State.h"

Eigen::VectorXd Data::State::Diff(const Data::State& other) const {
	Eigen::VectorXd residual = Eigen::VectorXd(4);
	residual << (m_position_x_ - other.GetPositionX()),
	         (m_position_y_ - other.GetPositionY()),
	         (m_velocity_x_ - other.GetVelocityX()),
	         (m_velocity_y_ - other.GetVelocityY());
	return residual;
}
namespace Data {
	std::ostream & operator<<(std::ostream& stream, const Data::State& pObject) {
		stream << pObject.GetPositionX()<< "\t";
		stream << pObject.GetPositionY() << "\t";
		stream << pObject.GetVelocityX() << "\t";
		stream << pObject.GetVelocityY() << "\t";
		return stream;
	}
}