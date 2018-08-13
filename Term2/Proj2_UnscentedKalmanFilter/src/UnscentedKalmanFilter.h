/*
This file puts together all the work done in the UKF assigment lessons so far
*/
#ifndef UNSCENTED_KALMAN_FILTER_H
#define UNSCENTED_KALMAN_FILTER_H

#include <iostream>

#include "Eigen/Dense"
#include "Data/Measurement.h"
#include "Data/State.h"
#include "Data/StateAdapterStrategy.h"
#include "KalmanFilter.h"
class UnscentedKalmanFilter {
public:
	UnscentedKalmanFilter();
	~UnscentedKalmanFilter() {}

	/** ProcessMeasurement processes input measurement
	* and updates Kalman Filter to get the predicted state. 
	*
	* This method requires a non-null Measurement pointer
	* and does not create any memory resources of measurement
	* pointer 
	*
	* @param pMeasurement pointer to Measurement
	*
	* Due to a polymorphic implementation of Measurement
	* class, this method does not have laser / radar
	* specific high coupling implementation.
	*/
	void ProcessMeasurement(Data::Measurement*  pMeasurement);

	/* getPredictedState method returns the current
	* predicted state by Kalman Filter
	*/
	Data::State GetPredictedState() const{
		auto predictedState = m_kalman_filter_.GetState();
  		Data::State pState(predictedState(0),
               predictedState(1),
               predictedState(2),
               predictedState(3),
               predictedState(4));
  		return pState;
	}

	double GetCurrentNIS() const{
		return m_current_NIS;
	}
private:
	/* computeDeltaT is a private member function
	* which computes the difference from the last
	* timestamp
	* @param pTimestamp 
	*/
	float ComputeDeltaT(long long pTimestamp);

	// Step 1: Augment Sigma Points with Process Noise Variables
	Eigen::MatrixXd GenerateAugmentedSigmaPoints(const Eigen::VectorXd& x_, const Eigen::MatrixXd& P_);

	// Step 3: Sigma points prediction
	/* apriori estimation: X_k+1 | k for sigma points = posterior estimation X_k | k + f(X_k, v_k)
	f(X_k , v_k) = process noise
	- Posterior estimate is the augmented Sigma Points matrix (7, 15) matrix
	- Apriori Estimate is 5x15 matrix obtained by transforming (X_k, v_k) 7 dimensional vector to
	X_k+1 vector using a state transition matrix
	*/
	void PredictSigmaPoints(Eigen::MatrixXd& pXAugmentedSigmaPointa, const double deltaT);


	// Step 4 : Predict state and covariance
	// Actual Predict Step based of predicted sigma points
	// reversal of step 1 - convert 5x15 sigma points to 1x5 state
	// 5x5 covariance matrix at step K+1
	void Predict();

	// Step 5: Update
	// Step 5.1: Use measurement model to convert the predicted state to measurement state
	// - Here is where the polymorphic implementation might come in handy to choose different
	// measurement models
	// NOTE: h(x) is non-linear function while predicted state in UKF assumes a gaussian process.
	// In EKF, we used Jaccobian to approximate
	// In UKF, we will use the sigma points technique to generate gaussian of h(x)
	// The idea is to come up with a gaussian distribution h(x) - i.e. mean and variance of h(x)
	// Unlike the process model, the noise in measurement model is linear and additive.
	// So we will not require the augmentation step. (this is where step 1 i useful)
	// We use the same predicted sigma points Xk+1 (5 x 15)|k in Step 1 and use them in the measurement function
	// Zk+1 | k = h(X_k+1 | k ) + w_k+1
	// In predict step: We used the predicted sigma points to predict mean and covariance of a 5 dimensional state vector
	//	5x15 dimensional matrix
	// In update step: the measurement model for Radar is a three dimensional model (rho, theta, rhodot)
	//	3x15 dimensional matrix
	// So, we use predicted sigma points to predict the mean and covariance of 3 dimensional measurement vector
	// In addition, we also add the measurement covariance error R to covariance matrix
	// Measurement Noise R = additive noise
	// For lidar - its 2x15 dimensional matrix
	void PredictMeasurement(Data::Measurement* pMeasurement);

	//Step 6 : Update state using the typical equations in EKF
	// Since we are dealing with matrices of sigma points predictions and
	// corresponding mean and covariance matrix of predicted measurement,
	// We need to also compute the cross correlation matrix of X and Z
	void Update(Data::Measurement* pMeasurement);
private:
	void FillSigmaPointMatrix(Eigen::VectorXd&  x_, Eigen::MatrixXd& P_, Eigen::MatrixXd& Xsig);
	Eigen::VectorXd ComputeSigmaPointPredictionPerColumn(Eigen::VectorXd& xSigAugCol, const double deltaT);
	void CalculateSigmaPointsWeights();

	bool m_is_initialized_; 

	long long m_previous_timestamp_;

	KalmanFilter m_kalman_filter_;

	// Predicted sigma points matrix matrix
	Eigen::MatrixXd m_sigma_points_predicted_;

	// Weights of sigma points used for transformation
	Eigen::VectorXd m_weights_;


	// Noise parameters
	// 1. Motion noise parameters
	// Process noise standard deviation longitudinal acceleration in m/s^2
	double m_std_a_;

	// Process noise standard deviation yaw acceleration in rad/s^2
	double m_std_yawdd_;


	// # of variables in process noise
	int m_num_process_noise_variables;

	///* State dimension
	int m_state_dimensions; 

	///* Augmented state dimension
	int m_aug_state_dimensions;

	int m_sigma_points;

	int m_aug_sigma_points;

	int m_measurement_dimensions;

	double m_lambda;

	double m_current_NIS;

};
#endif