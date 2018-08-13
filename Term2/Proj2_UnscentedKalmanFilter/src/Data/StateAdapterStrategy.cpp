#include "StateAdapterStrategy.h"
void Data::StateAdapterStrategy::ComputeEstimatedState(
    Eigen::MatrixXd& pPredictedXSigmaPoints,
    const Eigen::VectorXd& pSigmaPointWeights) {

	auto m_sigma_points = pPredictedXSigmaPoints.cols();

	auto pZSigmaPoints = TransformXSigmaPointsToZSpace(pPredictedXSigmaPoints);

	mZSig_ = pZSigmaPoints;
	
	ComputePredictedMeasurementMean(pZSigmaPoints, pSigmaPointWeights);

	ComputePredictedMeasurementCovariance(pZSigmaPoints, pSigmaPointWeights);

}

void Data::StateAdapterStrategy::ComputePredictedMeasurementMean(
    Eigen::MatrixXd& pZSigmaPoints,
    const Eigen::VectorXd& pSigmaPointWeights) {
	mZ_ = Eigen::VectorXd(m_measurement_dimensions_);
	mZ_.fill(0.0);

	auto m_sigma_points = pZSigmaPoints.cols();

	for (int i = 0 ; i < m_sigma_points; i++) {
		mZ_ = mZ_ + pSigmaPointWeights(i) * pZSigmaPoints.col(i);
	}
}

void Data::StateAdapterStrategy::ComputePredictedMeasurementCovariance(
    Eigen::MatrixXd& pZSigmaPoints,
    const Eigen::VectorXd& pSigmaPointWeights) {
	mS_ = Eigen::MatrixXd(m_measurement_dimensions_, m_measurement_dimensions_);
	mS_.fill(0.0);

	auto m_sigma_points = pZSigmaPoints.cols();

	// Store the residuals for future use
	mZResiduals_ = Eigen::MatrixXd(m_measurement_dimensions_, m_sigma_points);

	for (int i = 0; i < m_sigma_points; i++) {
		// residual
		Eigen::VectorXd z_diff = pZSigmaPoints.col(i) - mZ_;

		//Normalize(z_diff);
		

		mZResiduals_.col(i) = z_diff;
		mS_ = mS_ + pSigmaPointWeights(i) * z_diff * z_diff.transpose();
	}

	mS_ = mS_ + mR_;
}
