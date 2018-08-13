#ifndef STATEADAPTERSTRATEGY_H
#define STATEADAPTERSTRATEGY_H
#include "../Eigen/Dense"
#include "../DataUtils/Tools.h" 
/** StateAdapterStrategy class is a base class
* of a polymorphic hierarchy for providing strategies
* of converting states to approximate estimate x and y
* using measurement functions and measurement
* covariance matrix
*/
namespace Data {
class StateAdapterStrategy {
public:
	/** Constructor
	* @param pMeasurementDimensions measurement dimensions
	*/
	StateAdapterStrategy(const std::size_t pMeasurementDimensions):
		m_measurement_dimensions_(pMeasurementDimensions) {}
	virtual ~StateAdapterStrategy() {}

	const size_t GetDimensions() const {
		return m_measurement_dimensions_;
	}

	const Eigen::MatrixXd& GetPredictedMeasurementMean() const {
		return mZ_;
	}
	const Eigen::MatrixXd& GetPredictedMeasurementCovariance() const {
		return mS_;
	}
	const Eigen::MatrixXd& GetPredictedCrossCorrelation() const{
		return mT_;
	}
	const Eigen::MatrixXd& GetZResiduals() const {
		return mZResiduals_;
	}
	virtual double CalculateNIS(const Eigen::VectorXd& pMeasurement) = 0;

	/* computeEstimatedState is a pure virtual function which uses input
	* estimated state and computes approximate measurement using measurement
	* function
	* @param pPredictedSigmaPoints const Eigen::MatrixXd containing sigma
	* points predicted for input state space
	* returns the approximate estimate of position x and y
	* @param pState const Eigen::VectorXd containing state estimate
	*/
	void ComputeEstimatedState(
	    Eigen::MatrixXd& pPredictedXSigmaPoints,
	    const Eigen::VectorXd& pSigmaPointWeights);


	/*
	*/
	virtual void ComputePredictedCrossCorrelation(
	const Eigen::MatrixXd& pPredictedXSigmaPoints, 
	const Eigen::VectorXd& pSigmaPointWeights,
	const Eigen::VectorXd& pX
	) = 0; 
	/* getMeasurementFunction is a pure virtual function which
	* returns the covariance matrix for laser measurements
	* returns const Eigen::MatrixXd of (n x n) dims
	*/
	const Eigen::MatrixXd GetMeasurementCovariance() const {
		return mR_;
	}
	Eigen::MatrixXd mZSig_;

	virtual bool CheckNormalizeAngle() const = 0;
	virtual int GetNormalizeAngleIndex() const = 0;
protected:
	virtual Eigen::MatrixXd TransformXSigmaPointsToZSpace(
	    Eigen::MatrixXd& pPredictedXSigmaPoints) const = 0;

	void Normalize(Eigen::VectorXd& pMeasurement, int index) {
		DataUtils::Tools tools;
    	pMeasurement(index) = tools.NormalizeAngle(pMeasurement(index));
    }


	void ComputePredictedMeasurementMean(Eigen::MatrixXd& pZSigmaPoints,
	                                     const Eigen::VectorXd& pSigmaPointWeights);

	void ComputePredictedMeasurementCovariance(Eigen::MatrixXd& pZSigmaPoints,
	        const Eigen::VectorXd& pSigmaPointWeights);

	// input measurement dimensions
	std::size_t m_measurement_dimensions_;

	// measurement function matrix
	Eigen::MatrixXd mH_;

	// measurement covariance matrix
	Eigen::MatrixXd mR_;

	// predicted mean measurement
	Eigen::MatrixXd mZ_;

	// predicted measurement covariance
	Eigen::MatrixXd mS_;

	// residual of predicted measurement vs. actual measurement
	Eigen::MatrixXd mZResiduals_;

	// CrossCorrelation of z and x 
	Eigen::MatrixXd mT_;

};
}
#endif