#include "../Data/State.h"

Eigen::VectorXd Data::State::Diff(const Data::State& other) const {
	Eigen::VectorXd residual = Eigen::VectorXd(4);
	residual << (m_position_x_ - other.GetPositionX()),
	         (m_position_y_ - other.GetPositionY()),
	         (m_velocity_ - other.GetVelocity()),
	         (m_psi_ - other.GetPsi()),
	         (m_psi_dot_ - other.GetPsiDot());
	return residual;
}

std::ostream & Data::operator<<(std::ostream& stream, const Data::State& pObject) {
	stream << pObject.GetPositionX() << "\t";
	stream << pObject.GetPositionY() << "\t";
	stream << pObject.GetVelocity() << "\t";
	stream << pObject.GetPsi() << "\t";
	stream << pObject.GetPsiDot() << "\t";
	return stream;
}

Eigen::VectorXd Data::State::GetVectorizedData() const{
	Eigen::VectorXd stateVector = Eigen::VectorXd(State::mNumMembers_);
	//TODO:
	stateVector << m_position_x_ , m_position_y_, m_velocity_, m_psi_, m_psi_dot_;
	return stateVector;
}
