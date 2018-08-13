
#ifndef RADARSTATEADAPTERSTRATEGY_H
#define RADARSTATEADAPTERSTRATEGY_H
#include "../Data/StateAdapterStrategy.h"
/** RadarStateAdapterStrategy class inherits from StateAdapterStrategy and
* implements strategy for Radar measurements
*/
namespace Data {
class RadarStateAdapterStrategy: public StateAdapterStrategy {
public:
    /** Constructor
    * @param pMeasurementDimensions indicating dimensions of radar
    * measurements
    * Constructor passes the dimensions to base class and uses
    * the dimensions to initialize covariance matrix;
    */
    RadarStateAdapterStrategy(const std::size_t pMeasurementDimensions);

    virtual ~RadarStateAdapterStrategy() {}

    virtual bool CheckNormalizeAngle() const {
        return true;
    }
    virtual int GetNormalizeAngleIndex() const {
        return 1;
    }
    void ComputePredictedCrossCorrelation(
        const Eigen::MatrixXd& pPredictedXSigmaPoints, 
        const Eigen::VectorXd& pSigmaPointWeights,
        const Eigen::VectorXd& pX
        );
    virtual double CalculateNIS(const Eigen::VectorXd& pMeasurement) {
        Eigen::VectorXd zDiff = mZ_ - pMeasurement; 
        Normalize(zDiff, 1);

        double nis = zDiff.transpose() * mS_.inverse() * zDiff;
        return nis;
    }
 private:

    virtual Eigen::MatrixXd TransformXSigmaPointsToZSpace(Eigen::MatrixXd& pPredictedXSigmaPoints) const;

    
    // Radar measurement noise standard deviation radius in m
    double m_std_rho_;

    // Radar measurement noise standard deviation angle in rad
    double m_std_theta_;

    // Radar measurement noise standard deviation radius change in m/s
    double m_std_rhodot_;

};
}
#endif