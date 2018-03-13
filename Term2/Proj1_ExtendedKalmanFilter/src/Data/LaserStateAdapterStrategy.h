#ifndef LASERSTATEADAPTERSTRATEGY_H
#define LASERSTATEADAPTERSTRATEGY_H
#include "Data/StateAdapterStrategy.h"
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

  /** computeEstimatedState method uses input estimated state and
  * computes approximate measurement using measurement function
  * @param pState const Eigen::VectorXd containing input state
  * returns the approximate estimate of position x and y
  */
  virtual Eigen::VectorXd computeEstimatedState(const Eigen::VectorXd& pState);

  /** getMeasurementFunction method returns the h(x) function
  * for LaserStateAdapter
  * @param pState const Eigen::VectorXd containing current
  * state estimate of dim nx1 rows
  * returns the Eigen::MatriXd of (measurement dims x n) size
  */
  virtual Eigen::MatrixXd getMeasurementFunction(const Eigen::VectorXd& pState);

  /** getMeasurementCovariance method returns the covariance matrix
  * for laser measurements
  * returns const Eigen::MatrixXd of (n x n) dims
  */
  virtual const Eigen::MatrixXd getMeasurementCovariance() const;
};
}
#endif