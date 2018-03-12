#ifndef STATE_H_
#define STATE_H_

#include "../Eigen/Dense" // For VectorXd
#include <iostream>

/** State class stores the properties
* of an estimate / ground truth storing 
* position x, position y, velocity x, velocity y
*/
class State{
	public:
		/** Constructor
		* @param px indicating position x of state
		* @param py indicating position y of state
		* @param vx indicating velocity x of vehicle
		* @param vy indicating velocity y of vehicle
		*/
		State(float pX, float pY, float vX, float vY):
			mPositionX_(pX),
			mPositionY_(pY),
			mVelocityX_(vX),
			mVelocityY_(vY){}

		/** Destructor
		*/
		~State(){};

		/** getter functions - const member functions 
		* which returns values of member variables 
		*/
		const float getPositionX() const {
			return mPositionX_;
		}
		const float getPositionY() const {
			return mPositionY_;
		}
		const float getVelocityX() const {
			return mVelocityX_;
		}
		const float getVelocityY() const {
			return mVelocityY_;
		}

		/** diff method computes the difference in states 
		* given an another state
		* @param other const State as reference
		*/
		Eigen::VectorXd diff(const State& other) const;

		/** getInputDimensions is a static method 
		* that retuns the number of input dimensions in a a state
		*/
		static size_t getInputDimensions() { return State::mNumMembers_;}

		/** operator << overload is a friend function that
		* writes state into an output stream
		* @param ostream output stream to which State is written
		* @param const State& which needs to be written into ostream
		*/
		friend std::ostream &operator<<(std::ostream&, const State&);
	private:
		// position x
		float mPositionX_;

		// position y
		float mPositionY_;

		// velocity x
		float mVelocityX_;

		// velocity y
		float mVelocityY_;

		// static member variable indicating number of dimensions
		// of the state
		static constexpr float mNumMembers_ = 4;
};

// Typedef  
typedef State StateRMSE;
#endif 