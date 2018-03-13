#ifndef KALMAN_FILTER_H_
#define KALMAN_FILTER_H_
#include "Eigen/Dense"
#include "./Data/State.h"
/* KalmanFilter class has the core Kalman Filter
implementation with predict and update functions
*/
class KalmanFilter {
public:
    KalmanFilter(){};
    virtual ~KalmanFilter(){};

    void SetState(const State& pState) {
        x_ <<  pState.getPositionX(), pState.getPositionY(), pState.getVelocityX(),  pState.getVelocityY();
    }

    inline const Eigen::VectorXd& SetState() const {
        return x_;
    }

    /**
     * Init Initializes Kalman filter
     * @param P_in Initial state covariance
     * @param F_in Transition matrix
     * @param Q_in Process covariance matrix
     */
    void Init(Eigen::MatrixXd &P_in,
              Eigen::MatrixXd &F_in,
              Eigen::MatrixXd &Q_in);

    /** InitializeState with input dimensions
    * @param pSize dimensions of the State
    */
    void InitializeState(std::size_t pSize);


    /** UpdateF updates motion function with dt values
    * @param dt delta_t
    */
    void UpdateF(float dt);

    /** UpdaetQ updates noise matrix Q given dt and acceleration noise
    * @param dt delta dt
    * @param noise_ax motion noise in x direction
    * @param noise_ay motion noise in y direction
    */
    void UpdateQ(float dt, float noise_ax, float noise_ay);

    /**
     * Predict predicts the state and the state covariance
     * using the process model
     */
    void Predict();

    /**
     * Updates the state by using standard Kalman Filter equations
     * @param z The measurement at k+1
     * @param H Measurement function 
     * @param Hx Predicted measurement given measurement function and current 
     * state estimate
     * @param R Measurement covariance matrix 
     */
    void Update(const Eigen::VectorXd &z, const Eigen::MatrixXd &H, const Eigen::VectorXd &Hx, const Eigen::MatrixXd &R);
private:

    // state vector
    Eigen::VectorXd x_;

    // state covariance matrix
    Eigen::MatrixXd P_;

    // state transition matrix
    Eigen::MatrixXd F_;

    // process covariance matrix
    Eigen::MatrixXd Q_;
};

#endif /* KALMAN_FILTER_H_ */
