#ifndef SENSORFUSIONAPPLICATION_H
#define SENSORFUSIONAPPLICATION_H

#include <vector>

#include "Eigen/Dense"
#include "UnscentedKalmanFilter.h"
/** SensorFusionApplication class encapsulates underlying Extended Kalman 
* Filter implementation. The class is responsible for invoking EKF and 
* calculate RMSE of estimations and ground truth.
*/
class SensorFusionApplication {
public:
	SensorFusionApplication() {}
	~SensorFusionApplication() {}

	/** Run method computes predicted state given the current measurement 
	* and ground truth
	* @param pCurrentMeasurement pointer to Measurement
	* @param pGroundTruth point to ground truth
	* returns the predicted state by Kalman Filter
	*/
	const Data::State* Run(Data::Measurement* pCurrentMeasurement, const Data::GroundTruth* pGroundTruth);

	/** calculateRMSE method returns the RMSE given estimations and groundtruth
	* This method returns Eigen::VectorXd of rmse values
	*/
	Eigen::VectorXd CalculateRMSE();

	/* calculateNIS methods returns NIS score at each time step based of current time step's 
	measurement, previous time step's estimated mean and covariance. 
	*/
	double GetCurrentNIS() const{
		return m_NIS_[m_NIS_.size()-1];
	}
private:
	// UnscentedKalmanFilter
	UnscentedKalmanFilter m_unscented_kalman_filter_;

	// vector of estimations
	std::vector<Data::State> m_estimations_;

	//vector of ground truths
	std::vector<Data::GroundTruth> m_ground_truth_;

	// RMSE values of current estimations and ground truths
	Eigen::VectorXd m_current_RMSE_;

	// Vector of NIS values 
	std::vector<double> m_NIS_;
};
#endif