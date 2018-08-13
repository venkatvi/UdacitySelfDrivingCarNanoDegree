#include "KalmanFilter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

// Please note that the Eigen library does not initialize
// VectorXd or MatrixXd objects with zeros upon creation.
#include <iostream>
void KalmanFilter::Init(MatrixXd &P_in) {
  P_ = P_in;
}

void KalmanFilter::Predict(const Eigen::MatrixXd& pSigmaPointsPredicted,
                           const Eigen::VectorXd& pWeights) {
  // predicted mean state
  x_.fill(0.0);
  P_.fill(0.0);
  auto m_sigma_points = pSigmaPointsPredicted.cols();
  for (int i = 0; i < (m_sigma_points); i++) {
    x_ = x_ + pWeights(i) * pSigmaPointsPredicted.col(i);
  }

  // predict covariance
  // Store the residuals for future use
  xResiduals_ = Eigen::MatrixXd(x_.size(), pSigmaPointsPredicted.cols());
  for (int i = 0; i < (m_sigma_points); i++) {

    Eigen::VectorXd x_diff = pSigmaPointsPredicted.col(i) - x_;
    
    DataUtils::Tools tools;
    x_diff(3) = tools.NormalizeAngle(x_diff(3));

    xResiduals_.col(i) = x_diff;

    P_ = P_ + pWeights(i) * x_diff * x_diff.transpose();
  }
}
void KalmanFilter::Update(const Eigen::VectorXd& z, const Eigen::VectorXd& Hx,
                          const Eigen::MatrixXd& zCovariance,
                          const Eigen::MatrixXd& xzCovariance, 
                          bool doNormalizeAngle,
                          int angleIndex) {
  // Kalman Gain
  Eigen::MatrixXd K = xzCovariance * zCovariance.inverse();

  // Residual
  Eigen::VectorXd y = z - Hx;
  
  if (doNormalizeAngle){
    DataUtils::Tools tools;
    y(angleIndex) = tools.NormalizeAngle(y(angleIndex));
  }
  x_ = x_ + (K * y);
  P_ = P_ - (K * zCovariance * K.transpose());
}

