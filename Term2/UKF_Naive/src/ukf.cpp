#include "ukf.h"
#include "Eigen/Dense"
#include <iostream>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;

/**
 * Initializes Unscented Kalman filter
 * This is scaffolding, do not modify
 */
UKF::UKF() {
  is_initialized_ = false;
  // if this is false, laser measurements will be ignored (except during init)
  use_laser_ = true;

  // if this is false, radar measurements will be ignored (except during init)
  use_radar_ = true;

  // initial state vector
  x_ = VectorXd(5);

  // initial covariance matrix
  P_ = MatrixXd(5, 5);

  // Process noise standard deviation longitudinal acceleration in m/s^2
  std_a_ = 1.5;

  // Process noise standard deviation yaw acceleration in rad/s^2
  std_yawdd_ = 0.5;

  //DO NOT MODIFY measurement noise values below these are provided by the sensor manufacturer.
  // Laser measurement noise standard deviation position1 in m
  std_laspx_ = 0.15;

  // Laser measurement noise standard deviation position2 in m
  std_laspy_ = 0.15;

  // Radar measurement noise standard deviation radius in m
  std_radr_ = 0.3;

  // Radar measurement noise standard deviation angle in rad
  std_radphi_ = 0.03;

  // Radar measurement noise standard deviation radius change in m/s
  std_radrd_ = 0.3;
  //DO NOT MODIFY measurement noise values above these are provided by the sensor manufacturer.

///* State dimension
  n_x_ = 5;

  ///* Augmented state dimension
  n_aug_ = 7;

  ///* Sigma point spreading parameter
  lambda_ = 3 - n_aug_;

  n_aug_sigma_points_ = 2 * n_aug_ + 1;
  weights_ = Eigen::VectorXd(n_aug_sigma_points_);
  double weight_0 = lambda_ / (lambda_ + n_aug_);
  weights_(0) = weight_0;

  for (int i = 1; i < (n_aug_sigma_points_); i++) {
    weights_(i) = 0.5 / (n_aug_ + lambda_);
  }
}

UKF::~UKF() {}

/**
 * @param {MeasurementPackage} meas_package The latest measurement data of
 * either radar or laser.
 */
void UKF::ProcessMeasurement(MeasurementPackage measurement_pack) {
  if (!is_initialized_) {
    /**
    TODO:
      * Initialize the state ekf_.x_ with the first measurement.
      * Create the covariance matrix.
      * Remember: you'll need to convert radar from polar to cartesian coordinates.
    */
    // first measurement
    x_ = VectorXd(5);
    x_ << 1, 1, 1, 1, 1;

    P_ = MatrixXd(5, 5);
    P_ << 1, 0, 0, 0, 0,
    0, 1, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 0, 1, 0,
    0, 0, 0, 0, 1;
    if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
      /**
      Convert radar from polar to cartesian coordinates and initialize state.
      */
      //x/rho = cos(thetha);
      //y/rho = sin(theta);
      float rho = measurement_pack.raw_measurements_[0];
      float theta = measurement_pack.raw_measurements_[1];
      float drho = measurement_pack.raw_measurements_[2];
      float theta_rad = theta;

      float x = cos(theta_rad) * rho;
      float y = sin(theta_rad) * rho;
      float v = 0;
      float psi = 0;
      float psidot = 0;
      x_ << x, y, v, psi, psidot;

    }
    else if (measurement_pack.sensor_type_ == MeasurementPackage::LASER) {
      /**
      Initialize state.
      */
      float p_x = measurement_pack.raw_measurements_[0];
      float p_y = measurement_pack.raw_measurements_[1];
      float v = 0; // 5
      float psi = 0;//(p_x == 0 && p_y == 0) ? 0 : atan2(p_y, p_x);
      float psi_dot = 0;

      x_ << p_x, p_y, v, psi, psi_dot;
    }
    if (fabs(x_(0)) < 0.0001) {
      x_(0) = 0.0001;
      P_(0, 0) = x_(0);
    }
    if (fabs(x_(1)) < 0.0001) {
      x_(1) = 0.0001;
      P_(1, 1) = x_(1);
    }
    previous_timestamp_ = measurement_pack.timestamp_;

    // done initializing, no need to predict or update
    is_initialized_ = true;
    std::cout << "x_: " << std::endl << x_ << std::endl;
    std::cout << "P_: " << std::endl << P_ << std::endl;
    return;
  }

  /*****************************************************************************
   *  Prediction
   ****************************************************************************/
  //compute the time elapsed between the current and previous measurements
  float dt = (measurement_pack.timestamp_ - previous_timestamp_) / 1000000.0; //dt - expressed in seconds
  previous_timestamp_ = measurement_pack.timestamp_;

  // Predict state vector x and state covariance matrix P, watch out for large dt values
  /*while (dt > 0.1)
  {
    Prediction(0.05);
    dt -= 0.05;
  }*/
  std::cout << "DT: " << dt << std::endl;
  Prediction(dt);
  /*****************************************************************************
   *  Update
   ****************************************************************************/

  if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
    // Radar updates
    UpdateRadar(measurement_pack);
  } else {
    UpdateLidar(measurement_pack);
  }

  // print the output
  cout << "x_ = " << x_ << endl;
  cout << "P_ = " << P_ << endl;
}

/**
 * Predicts sigma points, the state, and the state covariance matrix.
 * @param {double} delta_t the change in time (in seconds) between the last
 * measurement and this one.
 */
void UKF::Prediction(double dt) {
  /**
  TODO:

  Complete this function! Estimate the object's location. Modify the state
  vector, x_. Predict sigma points, the state, and the state covariance matrix.
  */
  // Step 1: Generate Augmented Sigma Points
  Eigen::VectorXd x_aug = Eigen::VectorXd(n_aug_);
  x_aug.fill(0.0);
  x_aug.head(x_.size()) = x_;

  Eigen::MatrixXd P_aug = Eigen::MatrixXd(n_aug_, n_aug_);
  P_aug.fill(0.0);
  P_aug.topLeftCorner(x_.size(), x_.size()) = P_;
  P_aug(5, 5) = std_a_ * std_a_;
  P_aug(6, 6) = std_yawdd_ * std_yawdd_;

  Eigen::MatrixXd XAugSig_ = Eigen::MatrixXd(n_aug_, n_aug_sigma_points_);
  Eigen::MatrixXd PSqrt = P_aug.llt().matrixL();
  XAugSig_.fill(0.0);

  XAugSig_.col(0) = x_aug;
  for (int i = 0; i < n_aug_; i++) {
    XAugSig_.col(i + 1) = x_aug + sqrt(lambda_ + n_aug_) * PSqrt.col(i);
    XAugSig_.col(i + 1 + n_x_) = x_aug - sqrt(lambda_ + n_aug_) * PSqrt.col(i);
  }

  // Step 2: Generate SigmaPoint Predictions
  /// Generate 5x15 prediction from 7x15
  //Eigen::MatrixXd XSigPred_ = Eigen::MatrixXd(n_x_, n_aug_sigma_points_);
  Xsig_pred_ = Eigen::MatrixXd(n_x_, n_aug_sigma_points_);
  Xsig_pred_.fill(0.0);

  //XSigPred_.fill(0.0);
  for (int i = 0; i < n_aug_sigma_points_; i ++ ) {
    double px = XAugSig_(0, i);
    double py = XAugSig_(1, i);
    double v = XAugSig_(2, i);
    double psi = XAugSig_(3, i);
    double psidot = XAugSig_(4, i);
    double nu_a = XAugSig_(5, i);
    double nu_psidotdot = XAugSig_(6, i);

    // predicted state values
    double px_p, py_p;

    // avoid division by zero;
    // Zero yaw acceleration is dealt with at else clause
    if (fabs(psidot) > 0.001) {
      px_p = px + v / psidot * ( sin (psi + psidot * dt) - sin(psi));
      py_p = py + v / psidot * ( cos(psi) - cos(psi + psidot * dt) );
    } else {
      px_p = px + v * dt * cos(psi);
      py_p = py + v * dt * sin(psi);
    }

    double v_p = v;
    double psi_p = psi + psidot * dt;
    double psidot_p = psidot;

    // add noise to predicted x, y states
    auto psiCoefficient = 0.5 * nu_a * dt * dt ;
    px_p = px_p + psiCoefficient * cos(psi);
    py_p = py_p + psiCoefficient * sin(psi);
    v_p = v_p + nu_a * dt;
    psi_p = psi_p + 0.5 * nu_psidotdot + dt * dt;
    psidot_p = psidot_p + nu_psidotdot + dt;

    Eigen::VectorXd predictedState = Eigen::VectorXd(n_x_);
    predictedState << px_p, py_p, v_p, psi_p, psidot_p;
    Xsig_pred_.col(i) = predictedState;
  }
  // Step 3: Predict mean
  x_.fill(0.0);
  for (int i = 0; i < n_aug_sigma_points_; i++) {
    x_ = x_ + weights_(i) * Xsig_pred_.col(i);
  }

  // Step 4: Predict covariance
  P_.fill(0);
  for (int i = 0; i < n_aug_sigma_points_; i++) {
    Eigen::VectorXd x_diff = Xsig_pred_.col(i) - x_;
    x_diff(3) = Normalize(x_diff(3));

    P_ = P_ + weights_(i) * x_diff * x_diff.transpose();
  }
  std::cout << "Prediction: " << std::endl << "x_:" << std::endl << x_ << std::endl <<  "P_:" << P_ << std::endl;
}


/**
 * Updates the state and the state covariance matrix using a laser measurement.
 * @param {MeasurementPackage} meas_package
 */
void UKF::UpdateLidar(MeasurementPackage meas_package) {
  Eigen::MatrixXd R = Eigen::MatrixXd(n_z_laser_, n_z_laser_);
  R << std_laspx_* std_laspx_, 0,
  0, std_laspy_* std_laspy_;

  // Step 1: Compute Sigma POints in Measurement Space
  Eigen::MatrixXd ZSig = Eigen::MatrixXd(n_z_laser_, n_aug_sigma_points_);
  for (int i = 0; i < n_aug_sigma_points_; i++) {
    double p_x = Xsig_pred_(0, i);
    double p_y = Xsig_pred_(1, i);

    ZSig(0, i) = p_x;
    ZSig(1, i) = p_y;
  }
  // Step 2: Predict Measurement Mean
  Eigen::VectorXd z_pred = Eigen::VectorXd(n_z_laser_);
  z_pred.fill(0);
  for (int i = 0; i < n_aug_sigma_points_; i++) {
    z_pred = z_pred + weights_(i) * ZSig.col(i);
  }

  // Step 3: Predict Measurement Covariance
  Eigen::MatrixXd S = Eigen::MatrixXd(n_z_laser_, n_z_laser_);
  S.fill(0.0);
  for (int i = 0; i < n_aug_sigma_points_; i++) {
    Eigen::VectorXd z_diff = ZSig.col(i) - z_pred;

    S = S + weights_(i) * z_diff * z_diff.transpose();
  }

  // Step 3.1 Add R to S;
  S = S + R;
  std::cout << "S: " << std::endl << S << std::endl;
  // Step 4: Create cross-correlation matrix between x and z
  Eigen::MatrixXd T = Eigen::MatrixXd(n_x_, n_z_laser_);
  T.fill(0.0);
  for (int i = 0; i < n_aug_sigma_points_; i++) {
    Eigen::VectorXd z_diff = ZSig.col(i) - z_pred;
    Eigen::VectorXd x_diff = Xsig_pred_.col(i) - x_;
    x_diff(3) = Normalize(x_diff(3));

    T = T + weights_(i) * x_diff * z_diff.transpose();
  }
  std::cout << "T: " << std::endl << T << std::endl;
  // Step 5: Compute K
  Eigen::MatrixXd K = T * S.inverse();
  std::cout << "K: " << std::endl << K << std::endl;
  // Step 6 : Compute residual
  Eigen::VectorXd z  = meas_package.raw_measurements_;
  Eigen::VectorXd y = z - z_pred;
  std::cout << "y: " << std::endl << y << std::endl;
  x_ = x_ + K * y;
  P_ = P_ - K * S * K.transpose();
}

/**
 * Updates the state and the state covariance matrix using a radar measurement.
 * @param {MeasurementPackage} meas_package
 */
void UKF::UpdateRadar(MeasurementPackage meas_package) {
  Eigen::MatrixXd R = Eigen::MatrixXd(n_z_radar_, n_z_radar_);
  R << std_radr_* std_radr_, 0, 0,
  0, std_radphi_* std_radphi_, 0,
  0, 0, std_radrd_ * std_radrd_;

  // Step 1: Compute Sigma POints in Measurement Space
  Eigen::MatrixXd ZSig = Eigen::MatrixXd(n_z_radar_, n_aug_sigma_points_);
  ZSig.fill(0);
  for (int i = 0; i < n_aug_sigma_points_; i++) {
    double p_x = Xsig_pred_(0, i);
    double p_y = Xsig_pred_(1, i);
    double v = Xsig_pred_(2, i);
    double psi = Xsig_pred_(3, i);

    double rho = sqrt(p_x * p_x + p_y * p_y);
    double theta = (p_y == 0 && p_x == 0) ? 0 : atan2(p_y, p_x);
    double rho_dot = (rho == 0) ? 0 : (p_x * cos(psi) * v + p_y * sin(psi) * v) / rho;

    if (rho < 0.0001) {
      std::cout << "x: " << p_x << " y: " << p_y << std::endl; 
    }
    /*if (fabs(p_x) < 0.0001)
      ZSig(1, i) = M_PI / 2;     // Assume object is pointing straight up
    else
      ZSig(1, i) = theta;

    if (rho < 0.0001)
      ZSig(2, i) = 0.0001;  // With zero rho assume rho dot is zero as well
    else
      ZSig(2, i) = rho_dot;*/
    ZSig(0, i)=rho;
    ZSig(1, i)=theta;
    ZSig(2, i)=rho_dot;
   }
  std::cout << "ZSig: " << std::endl << ZSig << std::endl;

  // Step 2: Predict Measurement Mean
  Eigen::VectorXd z_pred = Eigen::VectorXd(n_z_radar_);
  z_pred.fill(0);
  for (int i = 0; i < n_aug_sigma_points_; i++) {
    z_pred = z_pred + weights_(i) * ZSig.col(i);
  }
  std::cout << "z_pred:" << std::endl << z_pred << std::endl;
  // Step 3: Predict Measurement Covariance
  Eigen::MatrixXd S = Eigen::MatrixXd(n_z_radar_, n_z_radar_);
  S.fill(0.0);
  for (int i = 0; i < n_aug_sigma_points_; i++) {
    Eigen::VectorXd z_diff = ZSig.col(i) - z_pred;
    z_diff(1) = Normalize(z_diff(1));

    S = S + weights_(i) * z_diff * z_diff.transpose();
  }
  std::cout << "S: " << std::endl << S << std::endl;
  // Step 3.1 Add R to S;
  S = S + R;

  // Step 4: Create cross-correlation matrix between x and z
  Eigen::MatrixXd T = Eigen::MatrixXd(n_x_, n_z_radar_);
  T.fill(0.0);
  for (int i = 0; i < n_aug_sigma_points_; i++) {
    Eigen::VectorXd z_diff = ZSig.col(i) - z_pred;
    z_diff(1) = Normalize(z_diff(1));

    Eigen::VectorXd x_diff = Xsig_pred_.col(i) - x_;
    x_diff(3) = Normalize(x_diff(3));

    T = T + weights_(i) * x_diff * z_diff.transpose();
  }
  std::cout << "T: " << std::endl << T << std::endl;
  // Step 5: Compute K
  Eigen::MatrixXd K = T * S.inverse();

  // Step 6 : Compute residual
  Eigen::VectorXd z  = meas_package.raw_measurements_;
  Eigen::VectorXd y = z - z_pred;
  y(1) = Normalize(y(1));
  std::cout << "y: " << std::endl << y << std::endl;
  x_ = x_ + K * y;
  P_ = P_ - K * S * K.transpose();
}
double Normalize(double angle) {
  while (angle> M_PI) angle-=2.*M_PI;
  while (angle<-M_PI) angle+=2.*M_PI;
  return angle;
  /*if (pTheta > M_PI) {
    pTheta -= 2. * M_PI;
  } else if (pTheta < -M_PI) {
    pTheta += 2. * M_PI;
  }*/
  //return atan2(sin(pTheta), cos(pTheta));
  /*angle = fmod(angle + M_PI, 2 * M_PI);
  if (angle < 0)
    angle += 2 * M_PI;
  return angle - M_PI;*/
}
