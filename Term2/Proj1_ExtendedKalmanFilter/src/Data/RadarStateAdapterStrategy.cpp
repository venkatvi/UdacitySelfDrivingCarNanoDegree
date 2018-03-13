#include "../Data/RadarStateAdapterStrategy.h"

#include <iostream>
Data::RadarStateAdapterStrategy::RadarStateAdapterStrategy(
  const std::size_t pMeasurementDimensions):
  Data::StateAdapterStrategy(pMeasurementDimensions) {
  mR_ = Eigen::MatrixXd(pMeasurementDimensions, pMeasurementDimensions);
  mR_ << 0.09, 0, 0,
      0, 0.0009, 0,
      0, 0, 0.09;
}
Eigen::VectorXd Data::RadarStateAdapterStrategy::ComputeEstimatedState(
  const Eigen::VectorXd& pState) {
  Eigen::VectorXd Hx = ConvertCartesianToPolarCoordinates(pState);
  return Hx;
}
Eigen::MatrixXd Data::RadarStateAdapterStrategy::GetMeasurementFunction(
  const Eigen::VectorXd& pState) {
  mH_ = Eigen::MatrixXd(m_measurement_dimensions_, pState.rows());
  CalculateJacobian(pState);
  return mH_;
}
void Data::RadarStateAdapterStrategy::CalculateJacobian(
  const Eigen::VectorXd& pState) {
  if (pState.rows() != mH_.cols()) {
    return;
  }
  //recover state parameters
  float px = pState(0);
  float py = pState(1);
  float vx = pState(2);
  float vy = pState(3);

  //pre-compute a set of terms to avoid repeated calculation
  float c1 = px * px + py * py;
  float c2 = sqrt(c1);
  float c3 = (c1 * c2);

  //check division by zero
  if (fabs(c1) < 0.001) {
    std::cout << "CalculateJacobian () - Error - Division by Zero" << std::endl;
    return;
  }

  //compute the Jacobian matrix
  mH_ << (px / c2), (py / c2), 0, 0,
      -(py / c1), (px / c1), 0, 0,
      py*(vx * py - vy * px) / c3, px*(px * vy - py * vx) / c3, px / c2, py / c2;
}

Eigen::VectorXd Data::RadarStateAdapterStrategy::ConvertCartesianToPolarCoordinates(
  const Eigen::VectorXd& pState) {
  //define threshold
  double THRESH = 0.0001;

  // Convert state to polar coordinates
  Eigen::VectorXd polarCoordinates = Eigen::VectorXd(m_measurement_dimensions_);
  double px = pState(0);
  double py = pState(1);
  double vx = pState(2);
  double vy = pState(3);

  double rho = sqrt(px * px + py * py);
  double phi = atan2(py, px);
  double drho = (rho > THRESH) ? ( px * vx + py * vy ) / rho : 0.0;

  polarCoordinates << rho, phi, drho;
  return polarCoordinates;
}
const Eigen::MatrixXd Data::RadarStateAdapterStrategy::GetMeasurementCovariance() const {
  return mR_;
}
