#ifndef EXTENDEDKALMANFILTER_H
#define EXTENDEDKALMANFILTER_H
#include "Eigen/Dense"
#include "KalmanFilter.h"
#include "./Data/Measurement.h"
/** ExtendedKalmanFilter class encapsulates the 
* sensor fusion implementation of laser and 
* radar data with a traditional Kalman Filter
*/
class ExtendedKalmanFilter{
	public:
  /** Constructor
  */
  ExtendedKalmanFilter();

  /** Destructor
  */
   ~ExtendedKalmanFilter(){}

		/** processMeasurement method processes input 
		* measurement and updates Kalman Filter to get 
		* the predicted state 
		* @param pMeasurement current measurement pointer
		* Due to a polymorphic implementation of Measurement
		* class, this method does not have laser / radar 
		* specific high coupling implementation.
		*/
		void processMeasurement(Measurement*  pMeasurement);

		/* getPredictedState method returns the current 
		* predicted state by Kalman Filter 
		*/
		State getPredictedState() const;
	private:
		/* computeDeltaT is a private member function 
		* which computes the difference from the last 
		* timestamp
		* @param pTimestamp 
		*/
		float computeDeltaT(long long pTimestamp);

  // kalman filter
		KalmanFilter mKalmanFilter_; 

		// state to record if Kalman Filter is initialized
		bool mIsInitialized_;

		// last known timestamp
		long long mPreviousTimestamp_;

		// Kalman Filter parameters
		// Input State
		Eigen::VectorXd x_;

		// State Covaraince Matrix 
		Eigen::MatrixXd P_;

		// State Noise 
		Eigen::MatrixXd Q_;

		// State Function
		Eigen::MatrixXd F_;
};
#endif