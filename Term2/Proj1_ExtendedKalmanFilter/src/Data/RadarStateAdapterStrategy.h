
#ifndef RADARSTATEADAPTERSTRATEGY_H
#define RADARSTATEADAPTERSTRATEGY_H
#include "./StateAdapterStrategy.h"
/** RadarStateAdapterStrategy class inherits 
* from StateAdapterStrategy and implements 
* strategy for Radar measurements
*/
class RadarStateAdapterStrategy: public StateAdapterStrategy{
	public:
    /** Constructor
    * @param pMeasurementDimensions indicating dimensions of radar 
    * measurements
    * Constructor passes the dimensions to base class and uses 
    * the dimensions to initialize covariance matrix;
    */
		RadarStateAdapterStrategy(const std::size_t pMeasurementDimensions);
			
    /** Destructor
    */ 
		virtual ~RadarStateAdapterStrategy() {}

    /** computeEstimatedState method uses input estimated state and 
    * computes approximate measurement using measurement function 
    * @param pState const Eigen::VectorXd containing input state
    * returns the approximate estimate of position x and y
    * @param pState const Eigen::VectorXd containing state estimate
    */
		virtual Eigen::VectorXd computeEstimatedState(const Eigen::VectorXd& pState);

    /** getMeasurementFunction method returns the h(x) function
    * for LaserStateAdapter 
    * @param pState const Eigen::VectorXd containing current 
    * state estimate of dim nx1 rows
    * returns the Eigen::MatriXd of (measurement dims x n) size
    */
  	virtual Eigen::MatrixXd getMeasurementFunction(const Eigen::VectorXd& pState);

	private:
    /** calculateJacobian method calculates linear approximation of 
    * radar measurement function using its first order derivative 
    * (Jacobian) in the taylor series expansion
    * @param pState const Eigen::VectorXd indicating current state
    */
  	void calculateJacobian(const Eigen::VectorXd& pState);
    
    /** getMeasurementCovariance method returns the covariance matrix
    * for laser measurements 
    * returns const Eigen::MatrixXd of (n x n) dims
    */
  	virtual const Eigen::MatrixXd getMeasurementCovariance() const; 

    /** calculateJacobian method calculates linear approximation of 
    * radar measurement function using its first order derivative 
    * (Jacobian) in the taylor series expansion
    * @param pState const Eigen::VectorXd indicating current state
    */
    Eigen::VectorXd convertCartesianToPolarCoordinates(const Eigen::VectorXd& pState);
};
#endif