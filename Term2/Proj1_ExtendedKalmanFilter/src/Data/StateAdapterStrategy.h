#ifndef STATEADAPTERSTRATEGY_H
#define STATEADAPTERSTRATEGY_H
#include "../Eigen/Dense"
/** StateAdapterStrategy class is a base class
* of a polymorphic hierarchy for providing strategies 
* of converting states to approximate estimate x and y
* using measurement functions and measurement
* covariance matrix
*/
class StateAdapterStrategy{
	public:
		/** Constructor
		* @param pMeasurementDimensions measurement dimensions
		*/
		StateAdapterStrategy(const std::size_t pMeasurementDimensions):
					mMeasurementDimensions_(pMeasurementDimensions){}

		/** Destructor
		*
		*/
		virtual ~StateAdapterStrategy(){}

		/* computeEstimatedState is a pure virtual function which
		* uses input estimated state and 
		* computes approximate measurement using measurement function 
		* @param pState const Eigen::VectorXd containing input state
		* returns the approximate estimate of position x and y
		* @param pState const Eigen::VectorXd containing state estimate
		*/
		virtual Eigen::VectorXd computeEstimatedState(const Eigen::VectorXd& pState) = 0;
		
		/* getMeasurementFunction is a pure virtual function which 
		* returns the h(x) function
		* @param pState const Eigen::VectorXd containing current 
		* state estimate of dim nx1 rows
		* returns the Eigen::MatriXd of (measurement dims x n) size
		*/
		virtual Eigen::MatrixXd getMeasurementFunction(const Eigen::VectorXd& pState) = 0;
		
		/* getMeasurementFunction is a pure virtual function which 
		* returns the covariance matrix for laser measurements 
		* returns const Eigen::MatrixXd of (n x n) dims
		*/
		virtual const Eigen::MatrixXd getMeasurementCovariance() const = 0;
	protected:
		// input measurement dimensions
		std::size_t mMeasurementDimensions_;

		// measurement function matrix
		Eigen::MatrixXd mH_;

		// measurement covariance matrix
		Eigen::MatrixXd mR_;
};
#endif