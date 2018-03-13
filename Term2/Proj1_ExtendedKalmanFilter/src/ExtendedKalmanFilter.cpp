#include "ExtendedKalmanFilter.h"

#include <iostream>

#include "Data/LaserMeasurement.h"
#include "Data/RadarMeasurement.h"
ExtendedKalmanFilter::ExtendedKalmanFilter():
  m_is_initialized_(false),
  m_previous_timestamp_(0) {
  // Get dimensions of a state
  const size_t dim_state = Data::State::GetInputDimensions();

  // Initialize State Covariance matrix
  P_ = Eigen::MatrixXd(dim_state, dim_state);
  P_ << 1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1000, 0,
  0, 0, 0, 1000;

  // Initialize Motion function / State Transition Function
  F_ = Eigen::MatrixXd(dim_state, dim_state);
  F_ << 1, 0, 1, 0,
  0, 1, 0, 1,
  0, 0, 1, 0,
  0, 0, 0, 1;

  // Initialize State Noise / Process Covariance Matrix
  Q_ = Eigen::MatrixXd(dim_state, dim_state);
}
void ExtendedKalmanFilter::ProcessMeasurement(Data::Measurement* pMeasurement) {
  if (!pMeasurement) {
    return;
  }
  if (!m_is_initialized_) {
    // get input dimensions of the state
    const size_t dim_state = Data::State::GetInputDimensions();

    // For first measurement, initialize state with input dimensions
    m_kalman_filter_.InitializeState(dim_state);

    // Initialize state matrices P, F, Q
    m_kalman_filter_.Init(P_, F_, Q_);

    // Set initial state with current measurement
    m_kalman_filter_.SetState(pMeasurement->GetStateData());


    m_is_initialized_ = true;
    m_previous_timestamp_ = pMeasurement->GetTimestamp();
    return;
  }
  // compute delta t to be used in F and Q matrices
  float dt = ComputeDeltaT(pMeasurement->GetTimestamp());

  m_previous_timestamp_ = pMeasurement->GetTimestamp();

  // Update motion function method with dt
  m_kalman_filter_.UpdateF(dt);

  // Update Q with noise_ax and noise_ay
  float noise_ax = 9;
  float noise_ay = 9;
  m_kalman_filter_.UpdateQ(dt, noise_ax, noise_ay);

  // Call predict function
  m_kalman_filter_.Predict();

  /* Call update method
  Here the polymorphic StateAdapterStrategy class is used to get the
  respective measurement covariance matrices and measurement function.
  */

  // get the StateAdapterStrategy
  auto strategy = pMeasurement->GetStateAdapterStrategy();

  // get Measurement function using estimated state
  Eigen::MatrixXd H_ =
    strategy->GetMeasurementFunction(m_kalman_filter_.GetState());

  // compute estimated position x and position y using estimated state
  Eigen::MatrixXd Hx_ =
    strategy->ComputeEstimatedState(m_kalman_filter_.GetState());

  Eigen::MatrixXd R_ = strategy->GetMeasurementCovariance();
  Eigen::VectorXd z_ = pMeasurement->GetVectorizedData();

  // call update function with measurement function, estimated state
  // and measurement covariance matrix
  m_kalman_filter_.Update(z_, H_, Hx_, R_);

}
Data::State ExtendedKalmanFilter::GetPredictedState() const {
  auto predictedState = m_kalman_filter_.GetState();
  Data::State pState(predictedState(0),
               predictedState(1),
               predictedState(2),
               predictedState(3));
  return pState;
}
float ExtendedKalmanFilter::ComputeDeltaT(long long pTimestamp) {
  return (pTimestamp - m_previous_timestamp_) / 1000000.0; //dt - expressed in seconds
}
