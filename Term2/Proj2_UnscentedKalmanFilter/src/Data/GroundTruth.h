#ifndef GROUNDTRUTH_H
#define GROUNDTRUTH_H

#include <iostream>

#include "../Eigen/Dense" // For VectorXd
#include "../DataUtils/Tools.h"

namespace Data {
class GroundTruth {
public:
	/** Constructor
	* @param px indicating position x of state
	* @param py indicating position y of state
	* @param vx indicating velocity x of vehicle
	* @param vy indicating velocity y of vehicle
	*/
	GroundTruth(float pX, float pY, float vx, float vy):
		m_position_x_(pX),
		m_position_y_(pY),
		m_velocity_x_(vx),
		m_velocity_y_(vy) {}
	~GroundTruth() {};

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
	* @param other const GroundTruth as reference
	*/
	Eigen::VectorXd Diff(const GroundTruth& other) const {
		Eigen::VectorXd residual = Eigen::VectorXd(4);
		residual << (m_position_x_ - other.GetPositionX()),
		         (m_position_y_ - other.GetPositionY()),
		         (m_velocity_x_ - other.GetVelocityX()),
		         (m_velocity_y_ - other.GetVelocityY());
		return residual;
	}

	/** getInputDimensions is a static method that retuns the number of input
	* dimensions in a a state
	*/
	static size_t GetInputDimensions() { return GroundTruth::mNumMembers_;}

	/** operator << overload is a friend function that writes state into an output stream
	* @param ostream output stream to which GroundTruth is written
	* @param const GroundTruth& which needs to be written into ostream
	*/
	friend std::ostream &operator<<(std::ostream&, const GroundTruth&);
	
	Eigen::VectorXd GetVectorizedData() const {
		Eigen::VectorXd groundTruthVector = Eigen::VectorXd(GroundTruth::mNumMembers_);
		groundTruthVector << m_position_x_, m_position_y_, m_velocity_x_, m_velocity_y_;
		return groundTruthVector;
	}
private:
	float m_position_x_;

	float m_position_y_;

	float m_velocity_x_;

	float m_velocity_y_;

	// static member variable indicating number of dimensions
	// of the state
	static constexpr float mNumMembers_ = 4;
};

std::ostream& operator<< (std::ostream& stream, const GroundTruth& pObject);
}
#endif