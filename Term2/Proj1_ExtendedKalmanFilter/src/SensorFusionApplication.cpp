#include "SensorFusionApplication.h"
#include "./DataUtils/DataAdapter.h" 
#include <memory>
/** run method computes predicted state given the 
* current measurement and ground truth
* @param pCurrentMeasurement pointer to Measurement
* @param pGroundTruth point to ground truth
* returns the predicted state by Kalman Filter
*/
const State* SensorFusionApplication::run(
          Measurement* pCurrentMeasurement, 
          const State* pGroundTruth){
  // add ground truth to vector of ground truths
  mGroundTruth_.push_back(*pGroundTruth);

  // invoke Extended Kalman Filter (EKF) to process 
  // measurement
  mExtendedKalmanFilter_.processMeasurement(pCurrentMeasurement);

  // compute predicted state from EKF
  State predictedState = mExtendedKalmanFilter_.getPredictedState();

  // add predicted state to vector of estimations
  mEstimations_.push_back(predictedState);

  // return the last predicted state
  return &mEstimations_[mEstimations_.size()-1];
}

/** calculateRMSE method returns the RMSE given 
* estimations and ground truth
* This method returns Eigen::VectorXd of rmse values
*/
Eigen::VectorXd SensorFusionApplication::calculateRMSE(){
  // initalize rmse
  Eigen::VectorXd rmse(4);
  rmse << 0,0,0,0;

  // check the validity of the following inputs:
  //  * the estimation vector size should not be zero
  //  * the estimation vector size should equal ground truth vector size
  if(mEstimations_.size() != mGroundTruth_.size()
      || mEstimations_.size() == 0){
    return rmse;
  }

  //accumulate squared residuals
  for(unsigned int i=0; i < mEstimations_.size(); ++i){

    Eigen::VectorXd residual = mEstimations_[i].diff(mGroundTruth_[i]);

    //coefficient-wise multiplication
    residual = residual.array()*residual.array();
    rmse += residual;
  }

  //calculate the mean
  rmse = rmse/mEstimations_.size();

  //calculate the squared root
  rmse = rmse.array().sqrt();

  //return the result
  mCurrentRMSE_ = rmse;
  return rmse;
}