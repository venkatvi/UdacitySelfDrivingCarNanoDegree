#ifndef KALMAN_FILTER_H_
#define KALMAN_FILTER_H_
#include "Eigen/Dense"
#include "./Data/State.h"
/* KalmanFilter class has the core 
Kalman Filter implementation with 
predict and update functions
*/ 
class KalmanFilter {
  public:
    /**
     * Constructor
     */
    KalmanFilter();

    /**
     * Destructor
     */
    virtual ~KalmanFilter();

    /**
     * Init Initializes Kalman filter
     * @param P_in Initial state covariance
     * @param F_in Transition matrix
     * @param Q_in Process covariance matrix
     */
    void init(Eigen::MatrixXd &P_in, 
            Eigen::MatrixXd &F_in,
            Eigen::MatrixXd &Q_in);

    /** initializeState with input dimensions 
    * @param pSize dimensions of the State 
    */
    void initializeState(std::size_t pSize);

    /** setState sets the initial state x_ of
    * the filter
    * @param pState const State& 
    */
    void setState(const State& pState);

    /** getState is a member function that returns 
    * a const reference to Eigen::VectorXd of 
    * the current state 
    */
    const Eigen::VectorXd& getState() const{
      return x_;
    }

    /** updateF method updates motion function with 
    * dt values 
    * @param dt delta_t
    */
    void updateF(float dt);

    /** updaetQ method noise matrix 
    * @param dt delta dt 
    * @param noise_ax motion noise in x direction
    * @param noise_ay motion noise in y direction
    */
    void updateQ(float dt, float noise_ax, float noise_ay);
    
    /**
     * Prediction Predicts the state and the state covariance
     * using the process model
     * @param delta_T Time between k and k+1 in s
     */
    void predict();

    /**
     * Updates the state by using standard Kalman Filter equations
     * @param z The measurement at k+1
     */
    void update(const Eigen::VectorXd &z, const Eigen::MatrixXd &H, const Eigen::VectorXd &Hx, const Eigen::MatrixXd &R);
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
