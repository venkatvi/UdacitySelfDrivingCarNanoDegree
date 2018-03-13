#ifndef STATE_H_
#define STATE_H_

#include <iostream>

#include "../Eigen/Dense" // For VectorXd

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
	State(float pX, float pY, float vX, float vY):
		m_position_x_(pX),
		m_position_y_(pY),
		m_velocity_x_(vX),
		m_velocity_y_(vY) {}
	~State() {};

	const float GetPositionX() const {
		return m_position_x_;
	}
	const float GetPositionY() const {
		return m_position_y_;
	}
	const float GetVelocityX() const {
		return m_velocity_x_;
	}
	const float GetVelocityY() const {
		return m_velocity_y_;
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
private:
	float m_position_x_;

	float m_position_y_;

	float m_velocity_x_;

	float m_velocity_y_;

	// static member variable indicating number of dimensions
	// of the state
	static constexpr float mNumMembers_ = 4;
};
}
#endif