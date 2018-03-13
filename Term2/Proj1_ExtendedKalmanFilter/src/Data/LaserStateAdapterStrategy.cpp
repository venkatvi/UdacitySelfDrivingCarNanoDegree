#include "../Data/LaserStateAdapterStrategy.h"
/** Constructor 
* @param pMeasurementDimensions indicates dimensions of measurements 
* for laser
* This method passes on the dimensions to base class. This class also 
* initialized the measurement covariance matrix 
*/
Data::LaserStateAdapterStrategy::LaserStateAdapterStrategy(
          const std::size_t pMeasurementDimensions):
	         Data::StateAdapterStrategy(pMeasurementDimensions) {
	mR_ = Eigen::MatrixXd(pMeasurementDimensions, pMeasurementDimensions);

	mR_ << 0.0225, 0,
	    0, 0.0225;
}

/** computeEstimatedState takes in predicted state as pstate 
* and computes the approximate measurement value using h(x)
* @param pState the current predicted state
* returns approximated position x, y using measurement function
*/
Eigen::VectorXd Data::LaserStateAdapterStrategy::ComputeEstimatedState(
                                  const Eigen::VectorXd& pState){
	Eigen::VectorXd Hx = mH_ * pState;
	return Hx;
}

/** getMeasurementFunction  returns the measurement function matrix
* @param pState const Eigen::VectorXd of dims nx1 
* returns Eigen::MatrixXd of dims m x n where m is dimensions of 
* measurement
*/
Eigen::MatrixXd Data::LaserStateAdapterStrategy::GetMeasurementFunction(
                                   const Eigen::VectorXd& pState){
	mH_ = Eigen::MatrixXd(m_measurement_dimensions_, pState.rows());
	for(size_t i=0;i < m_measurement_dimensions_ ;i++){
		mH_(i,i)=1;
		for(size_t j=0; j<pState.rows(); j++){
			if (i!=j){
				mH_(i,j) = 0;
			}
		}
	}
	return mH_;
}

/** getMeasurementCovariance returns the measurement covariance matrix
* returns Eigen::MatrixXd of dims m x m where m is dimensions of measurement
*/
const Eigen::MatrixXd Data::LaserStateAdapterStrategy::GetMeasurementCovariance() const {
	return mR_;
}