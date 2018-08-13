#ifndef STATE_H_
#define STATE_H_

#include <iostream>

#include "../Eigen/Dense" // For VectorXd
#include "../DataUtils/Tools.h"
#include "../Data/GroundTruth.h"
/** State class stores the properties of an estimate / ground truth storing
* position x, position y, velocity x, velocity y
*/
namespace Data {
class State {
public:
	/** Constructor
	* @param px indicating position x of state
	* @param py indicating position y of state
	* @param vx indicating velocity x of vehicle
	* @param vy indicating velocity y of vehicle
	*/
	State(float pX, float pY, float v, float psi, float psi_dot):
		m_position_x_(pX),
		m_position_y_(pY),
		m_velocity_(v),
		m_psi_(psi),
		m_psi_dot_(psi_dot),
		m_is_x_modified(false),
		m_is_y_modified(false) {
			CeilPosXValue();
			CeilPosYValue();
		}
	~State() {};

	const float GetPositionX() const {
		return m_position_x_;
	}
	const float GetPositionY() const {
		return m_position_y_;
	}
	const float GetVelocity() const {
		return m_velocity_;
	}
	const float GetPsi() const {
		return m_psi_;
	}
	const float GetPsiDot()const {
		return m_psi_dot_;
	}

	/** diff method computes the difference in states given an another state
	* @param other const State as reference
	*/
	Eigen::VectorXd Diff(const State& other) const;

	/** getInputDimensions is a static method that retuns the number of input
	* dimensions in a a state
	*/
	static size_t GetInputDimensions() { return State::mNumMembers_;}

	/** operator << overload is a friend function that writes state into an output stream
	* @param ostream output stream to which State is written
	* @param const State& which needs to be written into ostream
	*/
	friend std::ostream &operator<<(std::ostream&, const State&);

	Eigen::VectorXd GetVectorizedData() const;

	void Normalize()  {
		DataUtils::Tools tools;
		m_psi_ = tools.NormalizeAngle(m_psi_);
	}

	Data::GroundTruth GetCartesianCoordinates() const {
		auto velocity_x_ = m_velocity_ * cos(m_psi_);
		auto velocity_y_ = m_velocity_ * sin(m_psi_);
		GroundTruth groundTruth(m_position_x_, m_position_y_, velocity_x_, velocity_y_);
		return groundTruth;
	}
	bool CheckIfXModified(){
		return m_is_x_modified;
	}
	bool CheckIfYModified(){
		return m_is_y_modified;
	}
	
private:
	void CeilPosXValue(){
		if(fabs(m_position_x_) < 0.0001){
			m_position_x_ = 0.0001;
			m_is_y_modified = true;
		}
	}
	void CeilPosYValue(){
		if(fabs(m_position_y_) < 0.0001){
			m_position_y_ = 0.0001;
			m_is_y_modified = true;
		}
	}
	float m_position_x_;

	float m_position_y_;

	float m_velocity_;

	float m_psi_;

	float m_psi_dot_;

	// static member variable indicating number of dimensions
	// of the state
	static constexpr float mNumMembers_ = 5;

	bool m_is_x_modified;
	bool m_is_y_modified;
};
std::ostream& operator<< (std::ostream& pOStream, const State& pState);
}
#endif