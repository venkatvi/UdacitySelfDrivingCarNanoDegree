#include "ExtendedKalmanFilter.h"
#include "./Data/LaserMeasurement.h"
#include "./Data/RadarMeasurement.h"
#include <iostream>

using namespace std;
/** Constructor
*/
ExtendedKalmanFilter::ExtendedKalmanFilter():
  mIsInitialized_(false),
  mPreviousTimestamp_(0)
 {
  // Initialize State matrices 
  const size_t dim_state = State::getInputDimensions();

  // State Covariance matrix 
  P_ = Eigen::MatrixXd(dim_state, dim_state);
  P_ << 1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1000, 0,
        0, 0, 0, 1000;

  // Motion function
  F_ = Eigen::MatrixXd(dim_state, dim_state);
  F_ << 1, 0, 1, 0,
             0, 1, 0, 1,
             0, 0, 1, 0,
             0, 0, 0, 1;
  
  // State Noise         
  Q_ = Eigen::MatrixXd(dim_state, dim_state);
} 

/** processMeasurement method processes input 
* measurement and updates Kalman Filter to get 
* the predicted state 
* @param pMeasurement current measurement pointer
* Due to a polymorphic implementation of Measurement
* class, this method does not have laser / radar 
* specific high coupling implementation.
*/
void ExtendedKalmanFilter::processMeasurement(Measurement* pMeasurement) {
  if (!mIsInitialized_) {
    // for first measurement, set states of Kalman Filter
    // get input dimensions of the state 
    const size_t dim_state = State::getInputDimensions();

    // Initialize state with input dimensions
    mKalmanFilter_.initializeState(dim_state);

    // Initialize P, F, Q
    mKalmanFilter_.init(P_, F_, Q_);

    // Set initial state with current measurement
    mKalmanFilter_.setState(pMeasurement->getStateData());


    mIsInitialized_ = true;
    
    // Set previous timestamp
    mPreviousTimestamp_ = pMeasurement->getTimestamp();
    return;
  }
  if (pMeasurement){
    // compute delta ta
    float dt = computeDeltaT(pMeasurement->getTimestamp());

    // set previous timestamp
    mPreviousTimestamp_ = pMeasurement->getTimestamp();
    
    // Update motion function method with dt
    mKalmanFilter_.updateF(dt);
    
    // Update Q with noise_ax and noise_ay
    float noise_ax = 9;
    float noise_ay = 9;
    mKalmanFilter_.updateQ(dt, noise_ax, noise_ay);
    
    // Call predict function
    mKalmanFilter_.predict();
  
    /* Call update method
    Here the polymorphic StateAdapterStrategy class is used to get the 
    respective measurement covariance matrices and measurement function.
    */

    // get the StateAdapterStrategy
    auto strategy = pMeasurement->getStateAdapterStrategy();

    // get Measurement function
    Eigen::MatrixXd H_ = 
          strategy->getMeasurementFunction(mKalmanFilter_.getState());
    
    // compute estimated position x and position y
    Eigen::MatrixXd Hx_ = 
          strategy->computeEstimatedState(mKalmanFilter_.getState());
    
    // compute Measurement Covariance matrix
    Eigen::MatrixXd R_ = strategy->getMeasurementCovariance();

    // compute the vectorized data of measurement
    Eigen::VectorXd z_ = pMeasurement->getVectorizedData();

    // call update function with measurement function, estimated state 
    // and measurement covariance matrix
    mKalmanFilter_.update(z_, H_, Hx_, R_);
    
  }
}

/* getPredictedState method returns the current 
* predicted state by Kalman Filter 
*/
State ExtendedKalmanFilter::getPredictedState() const {
  auto predictedState = mKalmanFilter_.getState();
  State pState(predictedState(0), 
               predictedState(1), 
               predictedState(2), 
               predictedState(3));
  return pState;
}

/* computeDeltaT is a private member function 
* which computes the difference from the last 
* timestamp
* @param pTimestamp 
*/
float ExtendedKalmanFilter::computeDeltaT(long long pTimestamp){
  // find diff between timestamps
  return (pTimestamp - mPreviousTimestamp_) / 1000000.0; //dt - expressed in seconds 
}
