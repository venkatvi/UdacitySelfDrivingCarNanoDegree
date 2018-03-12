#include "KalmanFilter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

// Please note that the Eigen library does not initialize 
// VectorXd or MatrixXd objects with zeros upon creation.
#include <iostream>
/**
* Constructor
*/
KalmanFilter::KalmanFilter() {}

/**
* Destructor
*/
KalmanFilter::~KalmanFilter() {}

/**
* Init Initializes Kalman filter
* @param P_in Initial state covariance
* @param F_in Transition matrix
* @param Q_in Process covariance matrix
*/
void KalmanFilter::init(MatrixXd &P_in, MatrixXd &F_in, MatrixXd &Q_in) {
  P_ = P_in;
  F_ = F_in;
  Q_ = Q_in;
}

/** initializeState with input dimensions 
* @param pSize dimensions of the State 
*/
void KalmanFilter::initializeState(std::size_t pNumberOfStateVariables){
  x_ = VectorXd(pNumberOfStateVariables);
  for(int i=0;i<pNumberOfStateVariables;i++){
    x_(i) = 1;
  }

}

/** setState sets the initial state x_ of
* the filter
* @param pState const State& 
*/
void KalmanFilter::setState(const State& pState){
  x_ <<  pState.getPositionX(), pState.getPositionY(), pState.getVelocityX(),  pState.getVelocityY();
}

/** updateF method updates motion function with 
* dt values 
* @param dt delta_t
*/
void KalmanFilter::updateF(float dt){
  float dt_2 = dt * dt;
  float dt_3 = dt_2 * dt;
  float dt_4 = dt_3 * dt;

  //Modify the F matrix so that the time is integrated
  F_(0, 2) = dt;
  F_(1, 3) = dt;
}

/** updaetQ method noise matrix 
* @param dt delta dt 
* @param noise_ax motion noise in x direction
* @param noise_ay motion noise in y direction
*/
void KalmanFilter::updateQ(float dt, float noise_ax, float noise_ay){
  float dt_2 = dt * dt;
  float dt_3 = dt_2 * dt;
  float dt_4 = dt_3 * dt;

  //set the process covariance matrix Q
  Q_ <<  dt_4*noise_ax/4, 0, dt_3*noise_ax/2, 0,
         0, dt_4*noise_ay/4, 0, dt_3*noise_ay/2,
         dt_3*noise_ax/2, 0, dt_2*noise_ax, 0,
         0, dt_3*noise_ay/2, 0, dt_2*noise_ay;

}

/**
* Prediction Predicts the state and the state covariance
* using the process model
* @param delta_T Time between k and k+1 in s
*/
void KalmanFilter::predict() {
  x_ = F_ * x_; 
  MatrixXd Ft = F_.transpose();
  P_ = F_ * P_* Ft + Q_;
}

/**
 * Updates the state by using standard Kalman Filter equations
 * @param z The measurement at k+1
 */
void KalmanFilter::update(const VectorXd &z, const MatrixXd &H, const VectorXd &Hx, const MatrixXd &R) {
  VectorXd y = z - Hx;

  using namespace std;
  if (y.size() == 3) {
    y(1) = atan2(sin(y(1)), cos(y(1)));
  }
  MatrixXd Ht = H.transpose();
 
  MatrixXd S = H * P_ * Ht + R;
 
  MatrixXd Si = S. inverse();
 
  MatrixXd K = P_ * Ht * Si;
 
  // new state 
  x_ = x_ + (K * y);

  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);

  P_ = (I - K * H) * P_;
}
