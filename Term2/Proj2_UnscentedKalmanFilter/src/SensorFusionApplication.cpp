#include "SensorFusionApplication.h"

#include <memory>

#include "DataUtils/DataAdapter.h"
const Data::State* SensorFusionApplication::Run(
  Data::Measurement* pCurrentMeasurement,
  const Data::GroundTruth* pGroundTruth) {
  // Push ground truth to the vector of ground truths 
  m_ground_truth_.push_back(*pGroundTruth);

  // Process meaurement using UnscentedKalmanFilter implementation 
  m_unscented_kalman_filter_.ProcessMeasurement(pCurrentMeasurement);

  // Get predicted state from UKF
  Data::State predictedState = m_unscented_kalman_filter_.GetPredictedState();

  // Push estimations back into vector of estimations 
  m_estimations_.push_back(predictedState);
  
  m_NIS_.push_back(m_unscented_kalman_filter_.GetCurrentNIS());
  
  return &m_estimations_[m_estimations_.size() - 1];
}
/*Eigen::VectorXd SensorFusionApplication::CalculateNIS(){
  std::vector<double> NIS(m_predicted_measurement_mean.size()-1, 0); 
  for(int i=0; i<m_predicted_measurement_mean.size()-1< i++){
    Eigen::VectorXd mMean = m_predicted_measurement_mean(i);
    Eigen::MatrixXd mCov = m_predicted_measurement_covariance(i);
  //  Eigen::VectorXd mMeasurement = pCurrentMeasurement.GetVectorizedData();
  
  }
}*/
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
    auto estimatedGroundTruth = m_estimations_[i].GetCartesianCoordinates();
    Eigen::VectorXd residual = estimatedGroundTruth.Diff(m_ground_truth_[i]);
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