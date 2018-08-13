/*
This file puts together all the work done in the UKF assigment lessons so far
*/
#ifndef UKF_SKELETON_H
#define UKF_SKELETON_H

#include <iostream>

#include "Eigen/Dense"
class UKF_Skeleton {
public:
	UKF_Skeleton() {}
	~UKF_Skeleton() {}
	// 1. Init with x_, P_,
	/* 2. Init with Process noise variables :
	* std_a, std_yawdd (which are related to derivative of velocity)
	*/
	// 3.
	Init(int pStateDimensions) {
		m_std_a_ = 0.2
		           m_std_yawdd_ = 0.2;

		m_X_ = Eigen::VectorXd(pStateDimensions);

		m_P_ = Eigen::MatrixXd(pStateDimensions, pStateDimensions);
	}


	// Step1 : Generate Sigma Points
	void GenerateSigmaPoints(Eigen::VectorXd x_, Eigen::MatrixXd& P_) {
		/*
		Given an x , P vector for n_x dimesnions and n_x x n_x dimensions,
		UKF's first step is to generate sigma points
		*/
		auto n_x = x_.size();

		// Xsig is of dimension (n_x , 2*n_x + 1)
		m_X_sig_ = Eigen::MatrixXd(n_x, 2 * n_x + 1);

		FillSigmaPointMatrix(x_, P_, &Xsig);

	}
	// Step 2: Augment Sigma Points with Process Noise Variables
	void GenerteAugmentedSigmaPoints(Eigen::VectorXd x_, Eigen::MatrixXd P_) {
		// Create n_aug dimensions which is used to initialize the m_X_Augmented_sig_ matrix
		auto n_aug = m_X_sig_.rows() + m_num_process_noise_variables;

		// Define augmented matrix
		m_X_Augmented_sig_ = Eigen::MatrixXd(n_aug, 2 * n_aug + 1);

		// Initialize x_aug
		Eigen::VectorXd x_aug = Eigen::VectorXd(n_aug);
		x_aug.fill(0);
		x_aug.head(x_.size()) = x_;

		// Initialize P_aug
		Eigen::MatrixXd P_aug = Eigen::MatrixXd(n_aug, n_aug);
		P_aug.fill(0.0);
		P_aug.topLeftCorner(x_.size(), x_.size()) = P_;
		P_aug(5, 5) = m_std_a_ * m_std_a_;
		P_aug(6, 6) = m_std_yawdd_ * m_std_yawdd_;

		FillSigmaPointMatrix(x_aug, P_aug, &m_X_Augmented_sig_);
	}
	// Step 3: Sigma points prediction
	/* apriori estimation: X_k+1 | k for sigma points = posterior estimation X_k | k + f(X_k, v_k)
	f(X_k , v_k) = process noise
	- Posterior estimate is the augmented Sigma Points matrix (7, 15) matrix
	- Apriori Estimate is 5x15 matrix obtained by transforming (X_k, v_k) 7 dimensional vector to
	X_k+1 vector using a state transition matrix
	*/
	void PredictSigmaPoints(Eigen::MatrixXd& XSigAugmented, const double deltaT) {
		// Posterior state dimension
		int n_aug = m_X_Augmented_sig_.rows();

		// Apriori state dimension
		int n_x = m_X_sig_.rows();

		m_X_predicted_ = Eigen::MatrixXd(n_x, 2 * n_aug + 1);

		for (int i = 0; i < (2 * n_aug + 1); i++ ) {
			// extract values for better readabiity
			auto colValues  = XSigAugmented.col(i);
			m_X_predicted_.col(i) = ComputeSigmaPointPredictionPerColumn(colValues);
		}

		std::cout << "Predicted Sigma Points: " << std::endl << m_X_predicted_ << std::endl;
	}
	// Step 4 : Predict state and covariance
	// Actual Predict Step based of predicted sigma points
	// reversal of step 1 - convert 5x15 sigma points to 1x5 state
	// 5x5 covariance matrix at step K+1
	void Predict(Eigen::MatrixXd& XSigPred) {
		int n_x = m_X_.size();

		int n_aug = m_X_Augmented_sig_.row();

		double lambda = 3 - n_aug;

		// 1. Define weights as a function of lambda
		// Weights 5 x 15 for each cell of XSigPred matrix
		m_Weights_ = Eigen::VectorXd(n_x, 2 * n_aug + 1);
		double weight_0 = lambda / (lambda + n_aug);
		m_Weights_(0) = weight_0;

		for (int i = 1; i < (2 * n_aug + 1); i++) {
			m_Weights_(i) = 0.5 / (n_aug + lambda);
		}

		// predicted mean state
		m_X_.fill(0.0);
		for (int i = 0; i < (2 * n_aug + 1); i++) {
			m_X_ = m_X_ + m_Weights_(i) * XSigPred.col(i);
		}

		// predict covariance
		m_P_.fill(0.0);

		// Store the residuals for future use
		i_x_diff_ = Eigen::MatrixXd(m_X_.size(), XSigPred.cols());
		for (int i = 0; i < (2 * n_aug + 1); i++) {

			Eigen::VectorXd x_diff = XSigPred.col(i) - m_X_;

			// normalize angle
			if (x_diff(3) > M_PI || x_diff(3) < -M_PI) {
				x_diff(3) = atan(x_diff(3));
			}

			i_x_diff_.col(i) = x_diff;

			m_P_ = m_P_ + m_Weights_(i) * x_diff * x_diff.transpose();
		}

	}
	// Step 5: Update
	// Step 5.1: Use measurement model to convert the predicted state to measurement state
	// - Here is where the polymorphic implementation might come in handy to choose different
	// measurement models
	// NOTE: h(x) is non-linear function while predicted state in UKF assumes a gaussian process.
	// In EKF, we used Jaccobian to approximate
	// In UKF, we will use the sigma points technique to generate gaussian of h(x)
	// The idea is to come up with a gaussian distribution h(x) - i.e. mean and variance of h(x)
	// Unlike the process model, the noise in measurement model is linear and additive.
	// So we will not require the augmentation step. (this is where step 1 i useful)
	// We use the same predicted sigma points Xk+1 (5 x 15)|k in Step 1 and use them in the measurement function
	// Zk+1 | k = h(X_k+1 | k ) + w_k+1
	// In predict step: We used the predicted sigma points to predict mean and covariance of a 5 dimensional state vector
	//	5x15 dimensional matrix
	// In update step: the measurement model for Radar is a three dimensional model (rho, theta, rhodot)
	//	3x15 dimensional matrix
	// So, we use predicted sigma points to predict the mean and covariance of 3 dimensional measurement vector
	// In addition, we also add the measurement covariance error R to covariance matrix
	// Measurement Noise R = additive noise
	// For lidar - its 2x15 dimensional matrix
	void PredictMeasurement(int pMeasurementDimensions, bool isRadar) {
		// Initialize z predicted matrix
		m_Zsig_predicted_ = Eigen::MatrixXd(pMeasurementDimensions, m_X_predicted_.cols());

		// transform sigma points predicted to measurement space
		for (int i = 0 ; i < m_X_predicted_.cols(); i++ ) {
			double p_x = m_X_predicted_(0, i);
			double p_y = m_X_predicted_(1, i);
			double v = m_X_predicted_(2, i);
			double yaw = m_X_predicted_(3, i);

			double v1 = cos(yaw) * v;
			double v2 = cos(yaw) * v;

			if (isRadar) {
				// measurement model for radar
				m_Zsig_predicted_(0, i) = sqrt(p_x * p_x + p_y * p_y);
				m_Zsig_predicted_(1, i) = atan2(p_y, p_x);
				m_Zsig_predicted_(2, i) = (p_x * v1 + p_y * v2) / sqrt(p_x * p_x  + p_y * p_y);
			} else {
				m_Zsig_predicted_(0, i) = p_x;
				m_Zsig_predicted_(1, i) = p_y;
			}
		}

		// mean predicted measurement
		m_Z_ = Eigen::VectorXd(pMeasurementDimensions);
		m_Z_.fill(0.0);
		for (int i = 0 ; i < m_X_predicted_.cols(); i++) {
			m_Z_ = m_Z_ + m_Weights_(i) + m_Zsig_predicted_.col(i);
		}

		//innovation covariance matrix S (covariance of predicted measurement)
		m_S_ = Eigen::MatrixXd(pMeasurementDimensions, pMeasurementDimensions);
		m_S_.fill(0.0);

		// Store the residuals for future use
		i_z_diff_ = Eigen::MatrixXd(m_Z_.size(), m_Zsig_predicted_.cols());

		for (int i = 0; i < m_X_predicted_.cols(); i++) {
			// residual
			Eigen::VectorXd zdiff = m_Zsig_predicted_.col(i) - m_Z_;

			if (isRadar) {
				// normalize angle, theta
				if (z_diff(1) > M_PI || z_diff(1) < -M_PI) {
					z_diff(1) = atan2(z_dif(1));
				}
			}

			i_z_diff.col(i) = z_diff;
			m_S_ = m_S_ + m_Weights_(i) * z_diff * z_diff.transpose();
		}
		m_R_ = Eigen::MatrixXd(pMeasurementDimensions, pMeasurementDimensions);
		if (isRadar) {
			m_R_ << m_std_rho_ * m_std_rho_, 0, 0,
			     0, m_std_phi_ * m_std_phi_, 0,
			     0, 0, m_std_rhodot_ * m_std_rhodot_;

		} else {
			m_R_ << m_std_px_ * m_std_px_, 0,
			     0, m_std_py_ * m_std_py_;
		}
		m_S_ = m_S_ + m_R_;

		std::cout << "m_Z_: " << std::endl << m_Z_ << std::endl;
		std::cout << "m_S_: " << std::endl << m_S_ << std::endl;
	}
	//Step 6 : Update state using the typical equations in EKF
	// Since we are dealing with matrices of sigma points predictions and
	// corresponding mean and covariance matrix of predicted measurement,
	// We need to also compute the cross correlation matrix of X and Z
	void Update(Eigen::VectorXd& pMeasurement, bool isRadar) {
		m_T_ = Eigen::MatrixXd(m_X_.rows(), m_Z_.rows());
		m_T_.fill(0);
		for (int i = 0; i < m_X_predicted_.cols(); i++) {
			// Retrieve residuals from previous calculations
			m_T_ = m_T_ + m_Weights_(i) * i_x_diff.col(i) * i_z_diff_.col(i).transpose();
		}

		// Kalman Gain
		Eigen::MatrixXd K = m_T_ * m_S_.inverse();

		// Residual
		Eigen::VectorXd y = pMeasurement - m_Z_;

		// angle normalization
		if (isRadar) {
			if (y(1) > M_PI || y(1) < -M_PI) {
				y(1) = atan2(y(1));
			}
		}

		m_X_ = m_X_ + K * y;
		m_P_ = m_P_ - K * m_S_ * K.transpose();

		std::cout << "Updated state X:" << std::endl << m_X_ << std::endl;
		std::cout << "Updated state covariance P: " << std::endl << m_P_ << std::endl;

	}
private:
	void FillSigmaPointMatrix(Eigen::VectorXd&  x_, Eigen::MatrixXd& P_, Eigen::MatrixXd& Xsig) {
		if (Xsig.rows() ! = x_.size()) {
			return;
		}
		// number of dimensions of state
		auto n_x = x_.size();

		// Lambda is used to define weights used to generate sigma points
		// a.k.a Spreading parameter
		auto lambda = 3 - n_x;

		// weights also involve finding the sqrt of covariance matrix P
		// Compute square root of P
		auto sqrtP = P_.llt().matrixL();

		// First column of 2 * n_x + 1 is just filled with mean values of x_
		// Which is x itself
		Xsig.col(0) = x_;

		for (int i = 0; i < n_x; i++) {
			auto spreadFactor = sqrt(lambda + n_x) * sqrtP.col(i);
			Xsig.col(i + 1) = x_ + spreadFactor;
			Xsig.col(i + 1 + n_x ) = x_ - spreadFactor;
		}

		std::cout << "Sigma points Matrix: " << std::endl << m_X_sig_ << std::endl;
	}
	Eigen::VectorXd ComputeSigmaPointPredictionPerColumn(Eigen::VectorXd& xSigAugCol, const double deltaT) {
		double p_x = xSigAugCol(0);
		double p_y = xSigAugCol(1);
		double v = xSigAugCol(2);
		double yaw = xSigAugCol(3);
		double yawd = xSigAugCol(4);
		double nu_a = xSigAugCol(5);
		double nu_yawdd = xSigAugCol(6);

		// predicted state values
		double px_p, py_p;

		// avoid division by zero;
		// Zero yaw acceleration is dealt with at else clause
		if (fabs(yawd) > 0.001) {
			px_p = p_x + v / yawd * ( sin (yaw + yawd * deltaT) - sin(yaw));
			py_p = p_y + v / yawd * ( cos(yaw) - cos(yaw + yawd * deltaT) );
		} else {
			px_p = p_x + v * deltaT * cos(yaw);
			py_p = p_y + v * deltaT * sin(yaw);
		}

		double v_p = v;
		double yaw_p = yaw + yawd * deltaT;
		double yawd_p = yawd;

		// add noise to predicted x, y states
		auto yawCoefficient = 0.5 * nu_a * deltaT * deltaT ;
		px_p = px_p + yawCoefficient * cos(yaw);
		py_p = py_p + yawCoefficient * sin(yaw);
		v_p = v_p + nu_a * deltaT;
		yaw_p = yaw_p + 0.5 * nu_yawdd + deltaT * deltaT;
		yawd_p = yawd_p + nu_yawdd + deltaT;

		Eigen::VectorXd predictedState = Eigen::VectorXd(5);
		predictedState << px_p , py_p , v_p, yaw_p, yawd_p;

		return predictedState;
	}
// SigmaPoints Matrix
	Eigen::MatrixXd m_X_sig_;

// Augmented Sigma Points Matrix
	Eigen::MatrixXd m_X_Augmented_sig_;

// Predicted state matrix
	Eigen::MatrixXd m_X_predicted_;

// Weights required for transformation
	Eigen::VectorXd m_Weights_;

// Predicted sigma points in measurement space
	Eigen::MatrixXd m_Zsig_predicted_;

	Eigen::MatrixXd m_Z_; // mean of predicted measurement

	Eigen::MatrixXd m_S_; // covariance of predicted measurement

	// Cross correlation matrix
	Eigen::MatrixXd m_T_;

	// Other useful intermediate matrices
	Eigen::MatrixXd i_x_diff_;

	Eigen::MatrixXd i_z_diff_;
// UKF Vars
// State variables

// State vector
	Eigen::VectorXd m_X_;

// State covariance
	Eigen::MatrixXd m_P_;

// Noise parameters
// 1. Motion noise parameters
// acceleration noise
	double m_std_a_;

// yaw angle noise
	double m_std_yawdd_;

// 2. Measurement noise parameters
// 2.1 Radar measurement dimensions noise
// std_rho
	double m_std_rho_ = 0.3;

// std_phi
	double m_std_phi_ = 0.0175;

// std_rhodot
	double m_std_rhodot_ = 0.1;


// 2.2 Laser measurement dimensions noise
// std_px
	double m_std_px_ = 0.0225;

// std_py
	double m_std_py_ = 0.0025;

// # of variables in process noise
	size_t m_num_process_noise_variables;

	constexpr double deltaT = 0.1;

};
