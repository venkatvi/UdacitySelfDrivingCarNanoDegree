#ifndef EXTENDEDKALMANFILTER_H
#define EXTENDEDKALMANFILTER_H

#include "Data/Measurement.h"
#include "Eigen/Dense"
#include "KalmanFilter.h"

/** ExtendedKalmanFilter class encapsulates the
* sensor fusion implementation of laser and
* radar data with a traditional Kalman Filter
*/
class ExtendedKalmanFilter {
public:
	ExtendedKalmanFilter();
	~ExtendedKalmanFilter() {}

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
	void ProcessMeasurement(Measurement*  pMeasurement);

	/* getPredictedState method returns the current
	* predicted state by Kalman Filter
	*/
	State GetPredictedState() const;
private:
	/* computeDeltaT is a private member function
	* which computes the difference from the last
	* timestamp
	* @param pTimestamp 
	*/
	float ComputeDeltaT(long long pTimestamp);

	KalmanFilter m_kalman_filter_;

	// state to record if Kalman Filter is initialized
	bool m_is_initialized_;

	// last known timestamp
	long long m_previous_timestamp_;

	// Kalman Filter parameters
	// Input State
	Eigen::VectorXd x_;

	// State Covariance Matrix
	Eigen::MatrixXd P_;

	// State Noise / Process Covariance Matrix
	Eigen::MatrixXd Q_;

	// State Function / Transition Matrix
	Eigen::MatrixXd F_;
};
#endif