#include "RadarStateAdapterStrategy.h"
#include <iostream>
/** Constructor
 * @param pMeasurementDimensions indicating dimensions of radar 
 * measurements
 * Constructor passes the dimensions to base class and uses 
 * the dimensions to initialize covariance matrix;
 */
RadarStateAdapterStrategy::RadarStateAdapterStrategy(
			const std::size_t pMeasurementDimensions):
			StateAdapterStrategy(pMeasurementDimensions) {
	mR_ = Eigen::MatrixXd(pMeasurementDimensions, pMeasurementDimensions);
	mR_ << 0.09, 0, 0,
        0, 0.0009, 0,
        0, 0, 0.09;
}
/** computeEstimatedState method uses input estimated state and 
* computes approximate measurement using measurement function 
* @param pState const Eigen::VectorXd containing input state
* returns the approximate estimate of position x and y
* @param pState const Eigen::VectorXd containing state estimate
*/
Eigen::VectorXd RadarStateAdapterStrategy::computeEstimatedState(
                                          const Eigen::VectorXd& pState){
 Eigen::VectorXd Hx = convertCartesianToPolarCoordinates(pState);
 return Hx;
}

/** getMeasurementFunction method returns the h(x) function
* for LaserStateAdapter 
* @param pState const Eigen::VectorXd containing current 
* state estimate of dim nx1 rows
* returns the Eigen::MatriXd of (measurement dims x n) size
*/
Eigen::MatrixXd RadarStateAdapterStrategy::getMeasurementFunction(
                                          const Eigen::VectorXd& pState){
 mH_ = Eigen::MatrixXd(mMeasurementDimensions_, pState.rows());
 calculateJacobian(pState);
 return mH_;
}

void RadarStateAdapterStrategy::calculateJacobian(
                   const Eigen::VectorXd& pState){
 if (pState.rows() != mH_.cols()){
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
 if(fabs(c1) < 0.001){
  std::cout << "CalculateJacobian () - Error - Division by Zero" << std::endl;
  return;
 }

 //compute the Jacobian matrix
 mH_ << (px/c2), (py/c2), 0, 0,
    -(py/c1), (px/c1), 0, 0,
    py*(vx*py - vy*px)/c3, px*(px*vy - py*vx)/c3, px/c2, py/c2;
}

/** calculateJacobian method calculates linear approximation of 
* radar measurement function using its first order derivative 
* (Jacobian) in the taylor series expansion
* @param pState const Eigen::VectorXd indicating current state
*/
Eigen::VectorXd RadarStateAdapterStrategy::convertCartesianToPolarCoordinates(const Eigen::VectorXd& pState){
 //define threshold
 double THRESH = 0.0001;

 // Convert state to polar coordinates
 Eigen::VectorXd polarCoordinates = Eigen::VectorXd(mMeasurementDimensions_);
 double px = pState(0);
 double py = pState(1);
 double vx = pState(2);
 double vy = pState(3);

 double rho = sqrt(px*px + py*py);
 double phi = atan2(py, px);
 double drho = (rho > THRESH) ? ( px * vx + py * vy ) / rho : 0.0;

  polarCoordinates << rho, phi, drho;
  return polarCoordinates;
}

/** getMeasurementCovariance method returns the covariance matrix
* for laser measurements 
* returns const Eigen::MatrixXd of (n x n) dims
*/
const Eigen::MatrixXd RadarStateAdapterStrategy::getMeasurementCovariance() const {
 return mR_;
}
