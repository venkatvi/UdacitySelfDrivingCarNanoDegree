#include "KalmanFilter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

// Please note that the Eigen library does not initialize
// VectorXd or MatrixXd objects with zeros upon creation.
#include <iostream>
void KalmanFilter::Init(MatrixXd &P_in, MatrixXd &F_in, MatrixXd &Q_in) {
  P_ = P_in;
  F_ = F_in;
  Q_ = Q_in;
}
void KalmanFilter::InitializeState(std::size_t pNumberOfStateVariables) {
  x_ = VectorXd(pNumberOfStateVariables);
  for (int i = 0; i < pNumberOfStateVariables; i++) {
    x_(i) = 1;
  }
}
void KalmanFilter::UpdateF(float dt) {
  float dt_2 = dt * dt;
  float dt_3 = dt_2 * dt;
  float dt_4 = dt_3 * dt;

  //Modify the F matrix so that the time is integrated
  F_(0, 2) = dt;
  F_(1, 3) = dt;
}
void KalmanFilter::UpdateQ(float dt, float noise_ax, float noise_ay) {
  float dt_2 = dt * dt;
  float dt_3 = dt_2 * dt;
  float dt_4 = dt_3 * dt;

  //set the process covariance matrix Q
  Q_ <<  dt_4*noise_ax / 4, 0, dt_3*noise_ax / 2, 0,
  0, dt_4*noise_ay / 4, 0, dt_3*noise_ay / 2,
  dt_3*noise_ax / 2, 0, dt_2*noise_ax, 0,
  0, dt_3*noise_ay / 2, 0, dt_2*noise_ay;
}
void KalmanFilter::Predict() {
  x_ = F_ * x_;
  MatrixXd Ft = F_.transpose();
  P_ = F_ * P_ * Ft + Q_;
}
void KalmanFilter::Update(const VectorXd &z, const MatrixXd &H, const VectorXd &Hx, const MatrixXd &R) {
  // compute the prediction error 
  VectorXd y = z - Hx;

  using namespace std;
  // If y is a radar measurement, adjust theta
  // TODO: Polymorphic implementation of error
  if (y.size() == 3) {
    y(1) = atan2(sin(y(1)), cos(y(1)));
  }

  // Compute Kalman Gain
  MatrixXd Ht = H.transpose();

  MatrixXd S = H * P_ * Ht + R;

  MatrixXd Si = S. inverse();

  MatrixXd K = P_ * Ht * Si;

  // Get new state
  x_ = x_ + (K * y);

  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);

  // Update covariance matrix
  P_ = (I - K * H) * P_;
}
