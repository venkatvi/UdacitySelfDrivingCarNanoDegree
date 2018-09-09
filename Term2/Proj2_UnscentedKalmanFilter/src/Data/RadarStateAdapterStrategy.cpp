#include "../Data/RadarStateAdapterStrategy.h"

#include <iostream>
Data::RadarStateAdapterStrategy::RadarStateAdapterStrategy(
  const std::size_t pMeasurementDimensions):
  Data::StateAdapterStrategy(pMeasurementDimensions),
  m_std_rho_(0.3),
  m_std_theta_(0.03),
  m_std_rhodot_(0.3) {
  mR_ = Eigen::MatrixXd(pMeasurementDimensions, pMeasurementDimensions);
  mR_ << m_std_rho_ * m_std_rho_, 0, 0,
      0, m_std_theta_ * m_std_theta_, 0,
      0, 0, m_std_rhodot_ * m_std_rhodot_;
}
Eigen::MatrixXd Data::RadarStateAdapterStrategy::TransformXSigmaPointsToZSpace(
  Eigen::MatrixXd& pPredictedXSigmaPoints) const  {

  auto m_sigma_points = pPredictedXSigmaPoints.cols();
  Eigen::MatrixXd pZSigPredicted = Eigen::MatrixXd(m_measurement_dimensions_, m_sigma_points);

  // transform sigma points predicted to measurement space
  for (int i = 0 ; i < m_sigma_points; i++ ) {
    const double p_x = pPredictedXSigmaPoints(0, i);
    const double p_y = pPredictedXSigmaPoints(1, i);
    const double v = pPredictedXSigmaPoints(2, i);
    const double psi = pPredictedXSigmaPoints(3, i);

    const double rho = sqrt(p_x * p_x + p_y * p_y);
    const double theta = (p_y == 0 && p_x == 0) ? 0 : atan2(p_y, p_x);
    const double rho_dot = (rho == 0) ? 0 : (p_x * cos(psi) * v + p_y * sin(psi) * v) / rho;

    // measurement model for radar
    pZSigPredicted(0, i) = rho; //sqrt(p_x * p_x + p_y * p_y);
    pZSigPredicted(1, i) = theta; //atan2(p_y, p_x);
    pZSigPredicted(2, i) = rho_dot;//(p_x * v1 + p_y * v2) / sqrt(p_x * p_x  + p_y * p_y);
  }
  return pZSigPredicted;
}
void Data::RadarStateAdapterStrategy::ComputePredictedCrossCorrelation(
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
    Normalize(z_diff, 1);

    Eigen::VectorXd x_diff = pPredictedXSigmaPoints.col(i) - pX;
    Normalize(x_diff, 3);
    
    mT_  = mT_ + pSigmaPointWeights(i) * x_diff * z_diff.transpose();
  }
} 