#ifndef SENSORFUSIONAPPLICATION_H
#define SENSORFUSIONAPPLICATION_H

#include "./ExtendedKalmanFilter.h"
#include "Eigen/Dense"
#include <vector>
/** SensorFusionApplication class encapsulates 
* underlying Extended Kalman Filter implementation.
* The class is responsible for invoking EKF
* and calculate RMSE of estimations and ground truth.
*/
class SensorFusionApplication{
	public:
		/** Constructor
		*/
		SensorFusionApplication(){}
		
		/** Destructor
		*/
		~SensorFusionApplication(){}
		
		/** run method computes predicted state given the 
		* current measurement and ground truth
		* @param pCurrentMeasurement pointer to Measurement
		* @param pGroundTruth point to ground truth
		* returns the predicted state by Kalman Filter
		*/
		const State* run(Measurement* pCurrentMeasurement, const State* pGroundTruth);

		/** calculateRMSE method returns the RMSE given 
  * estimations and ground truth
  * This method returns Eigen::VectorXd of rmse values
  */
		Eigen::VectorXd calculateRMSE();
	private:
  /* getExtendedKalmanFilter is a protected getter 
  * returning the member mExtendedKalmanFilter_
  */
		ExtendedKalmanFilter &getExtendedKalmanFilter(){
			return mExtendedKalmanFilter_;
		}
  // ExtendedKalmanFilter
		ExtendedKalmanFilter mExtendedKalmanFilter_;

  // vector of estimations
		std::vector<State> mEstimations_;

  //vector of ground truths
		std::vector<State> mGroundTruth_;

  // RMSE values of current estimations and ground truths
		Eigen::VectorXd mCurrentRMSE_;
};	
#endif