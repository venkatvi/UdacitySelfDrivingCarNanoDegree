#ifndef LASERSTATEADAPTERSTRATEGY_H
#define LASERSTATEADAPTERSTRATEGY_H
#include "../Data/StateAdapterStrategy.h"
/** LaserStateAdapterStrategy class inherits from StateAdapterStrategy.
* This class initializes the measurement covariance matrices and defines the
* measurement function.
*/
namespace Data {
class LaserStateAdapterStrategy: public StateAdapterStrategy {
public:
  /** Constructor
  * @param pMeasurementDimensions number of input dimensions used
  * initialize covariance matrix
  */
  LaserStateAdapterStrategy(const std::size_t pMeasurementDimensions);
  virtual ~LaserStateAdapterStrategy() {}

  virtual void ComputePredictedCrossCorrelation(
    const Eigen::MatrixXd& pPredictedXSigmaPoints, 
    const Eigen::VectorXd& pSigmaPointWeights,
    const Eigen::VectorXd& pX
    ); 
  virtual bool CheckNormalizeAngle() const {
    return false;
  }
  virtual int GetNormalizeAngleIndex() const {
    return -1;
  }
  virtual double CalculateNIS(const Eigen::VectorXd& pMeasurement) {
    Eigen::VectorXd zDiff = mZ_ - pMeasurement; 
    double nis = zDiff.transpose() * mS_.inverse() * zDiff;
    return nis;
  }
private:
  virtual Eigen::MatrixXd TransformXSigmaPointsToZSpace(Eigen::MatrixXd& pPredictedXSigmaPoints) const;


  // Laser measurement noise standard deviation position1 in m
  double m_std_px_;

  // Laser measurement noise standard deviation position2 in m
  double m_std_py_;
};
}
#endif