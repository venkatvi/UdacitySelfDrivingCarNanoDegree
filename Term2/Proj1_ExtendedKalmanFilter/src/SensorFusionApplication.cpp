#include "SensorFusionApplication.h"

#include <memory>

#include "DataUtils/DataAdapter.h"
const Data::State* SensorFusionApplication::Run(
  Data::Measurement* pCurrentMeasurement,
  const Data::State* pGroundTruth) {
  m_ground_truth_.push_back(*pGroundTruth);
  m_extended_kalman_filter_.ProcessMeasurement(pCurrentMeasurement);
  Data::State predictedState = m_extended_kalman_filter_.GetPredictedState();
  m_estimations_.push_back(predictedState);
  return &m_estimations_[m_estimations_.size() - 1];
}
Eigen::VectorXd SensorFusionApplication::CalculateRMSE() {
  Eigen::VectorXd rmse(4);
  rmse << 0, 0, 0, 0;

  // check the validity of the following inputs:
  //  * the estimation vector size should not be zero
  //  * the estimation vector size should equal ground truth vector size
  if (m_estimations_.size() != m_ground_truth_.size()
      || m_estimations_.size() == 0) {
    return rmse;
  }

  //accumulate squared residuals
  for (unsigned int i = 0; i < m_estimations_.size(); ++i) {
    Eigen::VectorXd residual = m_estimations_[i].Diff(m_ground_truth_[i]);
    //coefficient-wise multiplication
    residual = residual.array() * residual.array();
    rmse += residual;
  }

  //calculate the mean
  rmse = rmse / m_estimations_.size();

  //calculate the squared root
  rmse = rmse.array().sqrt();

  //return the result
  m_current_RMSE_ = rmse;
  return rmse;
}