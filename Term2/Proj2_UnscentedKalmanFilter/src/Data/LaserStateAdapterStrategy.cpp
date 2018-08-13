#include "../Data/LaserStateAdapterStrategy.h"
#include <iostream>
/** Constructor
* @param pMeasurementDimensions indicates dimensions of measurements
* for laser
* This method passes on the dimensions to base class. This class also
* initialized the measurement covariance matrix
*/
Data::LaserStateAdapterStrategy::LaserStateAdapterStrategy(
    const std::size_t pMeasurementDimensions):
	Data::StateAdapterStrategy(pMeasurementDimensions),
	m_std_px_(0.15),
	m_std_py_(0.15) {
	mR_ = Eigen::MatrixXd(pMeasurementDimensions, pMeasurementDimensions);

	mR_ << m_std_px_ * m_std_px_, 0,
	    0, m_std_py_ * m_std_py_;
}
Eigen::MatrixXd Data::LaserStateAdapterStrategy::TransformXSigmaPointsToZSpace(
    Eigen::MatrixXd& pPredictedXSigmaPoints) const {

	auto m_sigma_points = pPredictedXSigmaPoints.cols();
	Eigen::MatrixXd pZSigPredicted = Eigen::MatrixXd(m_measurement_dimensions_, m_sigma_points);

	// transform sigma points predicted to measurement space
	for (int i = 0 ; i < m_sigma_points; i++ ) {
		double p_x = pPredictedXSigmaPoints(0, i);
		double p_y = pPredictedXSigmaPoints(1, i);
		pZSigPredicted(0, i) = p_x;
		pZSigPredicted(1, i) = p_y;
	}
	return pZSigPredicted;
}
void Data::LaserStateAdapterStrategy::ComputePredictedCrossCorrelation(
	const Eigen::MatrixXd& pPredictedXSigmaPoints, 
	const Eigen::VectorXd& pSigmaPointWeights,
	const Eigen::VectorXd& pX
	){
	auto m_x_size = pX.size();
	auto m_aug_sigma_points = mZSig_.cols();
	mT_ = Eigen::MatrixXd(m_x_size, m_measurement_dimensions_);
	mT_.fill(0.0);
	for(int i=0; i<m_aug_sigma_points; i++){
		Eigen::VectorXd z_diff = mZSig_.col(i) - mZ_;
		Eigen::VectorXd x_diff = pPredictedXSigmaPoints.col(i) - pX;
		
		Normalize(x_diff, 3);

		mT_ = mT_ + pSigmaPointWeights(i) * x_diff * z_diff.transpose();
	}
}	
