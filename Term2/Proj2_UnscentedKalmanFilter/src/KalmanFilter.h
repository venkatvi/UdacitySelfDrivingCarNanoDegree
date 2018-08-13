#ifndef KALMAN_FILTER_H_
#define KALMAN_FILTER_H_
#include "Eigen/Dense"
#include "./Data/State.h"
/* KalmanFilter class has the core Kalman Filter
implementation with predict and update functions
*/
class KalmanFilter {
public:
    KalmanFilter() {};
    virtual ~KalmanFilter() {};

    void SetState(const Data::State& pState) {
        x_ = pState.GetVectorizedData();
    }

    inline const Eigen::VectorXd& GetState() const {
        return x_;
    }

    inline const Eigen::MatrixXd& GetStateCovariance() const {
        return P_;
    }
    inline const Eigen::MatrixXd& GetStateResiduals() const {
        return xResiduals_;
    }
    /**
     * Init Initializes Kalman filter
     * @param P_in Initial state covariance
     */
    void Init(Eigen::MatrixXd &P_in);

    /**
     * Predict predicts the state and the state covariance
     * using the process model
     */
    void Predict(const Eigen::MatrixXd& pSigmaPointsPredicted,
                 const Eigen::VectorXd& pWeights);

    /**
     * Updates the state by using standard Kalman Filter equations
     * @param z The measurement at k+1
     * @param H Measurement function
     * @param Hx Predicted measurement given measurement function and current
     * state estimate
     * @param R Measurement covariance matrix
     */
    void Update(const Eigen::VectorXd& z,
                const Eigen::VectorXd& Hx,
                const Eigen::MatrixXd& zCovariance,
                const Eigen::MatrixXd& xzCovariance,
                bool doNormalizeAngle,
                int angleIndex);
private:

    // state vector
    Eigen::VectorXd x_;

    // state covariance matrix
    Eigen::MatrixXd P_;

    //state sigma point residuals
    Eigen::MatrixXd xResiduals_;
};

#endif /* KALMAN_FILTER_H_ */
