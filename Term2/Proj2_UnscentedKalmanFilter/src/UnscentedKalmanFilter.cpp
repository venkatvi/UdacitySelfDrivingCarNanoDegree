/*
This file puts together all the work done in the UKF assigment lessons so far
*/
#include "UnscentedKalmanFilter.h"

#include <cmath>

#include "Data/StateAdapterStrategy.h"
UnscentedKalmanFilter::UnscentedKalmanFilter():
	m_is_initialized_(false),
	m_previous_timestamp_(0) {

	// Get dimensions of a state
	const size_t dim_state = Data::State::GetInputDimensions();

	// Initialize noise values
	m_std_a_ = 1.5;
	m_std_yawdd_ = 0.5;

	m_state_dimensions = dim_state;
	m_aug_state_dimensions = dim_state + 2;
	// 
	m_lambda = 3 - m_aug_state_dimensions;
	m_aug_sigma_points = (m_aug_state_dimensions * 2) + 1;
	
	// Initialize m_P_aug_
	m_P_aug_ = Eigen::MatrixXd(m_aug_state_dimensions, m_aug_state_dimensions);
	m_P_aug_.fill(0.0);
	
	//Initialize P
	m_weights_ = Eigen::VectorXd(m_aug_sigma_points);
	double weight_0 = m_lambda / (m_lambda + m_aug_state_dimensions);
	m_weights_(0) = weight_0;
	for (int i = 1; i < (m_aug_sigma_points); i++) {
		m_weights_(i) = 0.5 / (m_aug_state_dimensions + m_lambda);
	}
}
void UnscentedKalmanFilter::ProcessMeasurement(Data::Measurement* pMeasurement)
{	if (!pMeasurement) {
		return;
	}
	if (!m_is_initialized_) {
		// get input dimensions of the state
		const size_t dim_state = Data::State::GetInputDimensions();

		// Initialize state matrices P, F, Q
		//m_kalman_filter_.Init(P_);

		// Set initial state with current measurement
		Data::State pState = pMeasurement->GetStateData();
		m_kalman_filter_.SetState(pState);

		Eigen::MatrixXd P_ = Eigen::MatrixXd(dim_state, dim_state);

		P_ = pMeasurement->GetInitStateCovariance();

		if(pState.CheckIfXModified()){
			P_(0,0)=pState.GetPositionX();
		}
		if(pState.CheckIfYModified()){
			P_(1,1)=pState.GetPositionY();
		}
		m_kalman_filter_.Init(P_);


		m_is_initialized_ = true;
		m_previous_timestamp_ = pMeasurement->GetTimestamp();

		return;
	}
	float dt = ComputeDeltaT(pMeasurement->GetTimestamp());
	
	m_previous_timestamp_ = pMeasurement->GetTimestamp();

	
	Eigen::MatrixXd pAugmentedSigmaPoints = GenerateAugmentedSigmaPoints(
	        m_kalman_filter_.GetState(),
	        m_kalman_filter_.GetStateCovariance());

	PredictSigmaPoints(pAugmentedSigmaPoints, dt);

	//CalculateSigmaPointsWeights();

	Predict();

	PredictMeasurement(pMeasurement);

	Update(pMeasurement);

}

// Step 2: Augment Sigma Points with Process Noise Variables
//x_ = kalman_filter.state
//P_ = kalman_filter.covariance_of_state
Eigen::MatrixXd UnscentedKalmanFilter::GenerateAugmentedSigmaPoints(const Eigen::VectorXd& x_, const Eigen::MatrixXd& P_) {
	// Initialize x_aug
	Eigen::VectorXd x_aug = Eigen::VectorXd(m_aug_state_dimensions);
	x_aug.fill(0);
	x_aug.head(x_.size()) = x_;

	m_P_aug_.topLeftCorner(x_.size(), x_.size()) = P_;
	m_P_aug_(5, 5) = m_std_a_ * m_std_a_;
	m_P_aug_(6, 6) = m_std_yawdd_ * m_std_yawdd_;

	// Define augmented matrix and initialize with 0s
	Eigen::MatrixXd pAugmentedSigmaPoints = Eigen::MatrixXd(m_aug_state_dimensions, m_aug_sigma_points);
	pAugmentedSigmaPoints.fill(0.0);

	FillSigmaPointMatrix(x_aug, m_P_aug_, pAugmentedSigmaPoints);
	return pAugmentedSigmaPoints;
}
// Step 3: Sigma points prediction
/* apriori estimation: X_k+1 | k for sigma points = posterior estimation X_k | k + f(X_k, v_k)
f(X_k , v_k) = process noise
- Posterior estimate is the augmented Sigma Points matrix (7, 15) matrix
- Apriori Estimate is 5x15 matrix obtained by transforming (X_k, v_k) 7 dimensional vector to
X_k+1 vector using a state transition matrix
*/
void UnscentedKalmanFilter::PredictSigmaPoints(Eigen::MatrixXd& XSigAugmented, const double dt) {
	m_sigma_points_predicted_ = Eigen::MatrixXd(m_state_dimensions, m_aug_sigma_points);
	m_sigma_points_predicted_.fill(0.0);

	for (int i = 0; i < (m_aug_sigma_points); i++ ) {
		// extract values for better readabiity
		Eigen::VectorXd colValues  = XSigAugmented.col(i).array();
		m_sigma_points_predicted_.col(i) = ComputeSigmaPointPredictionPerColumn(colValues, dt);
	}
}
// Step 4 : Predict state and covariance
// Actual Predict Step based of predicted sigma points
// reversal of step 1 - convert 5x15 sigma points to 1x5 state
// 5x5 covariance matrix at step K+1
void UnscentedKalmanFilter::Predict() {
	m_kalman_filter_.Predict(m_sigma_points_predicted_, m_weights_);
}

void UnscentedKalmanFilter::CalculateSigmaPointsWeights() {
	int n_aug = m_aug_state_dimensions;
	int lambda = 3 - n_aug;
	// 1. Define weights as a function of lambda
	// Weights 5 x 15 for each cell of XSigPred matrix
	m_weights_ = Eigen::VectorXd(m_aug_sigma_points);
	double weight_0 = lambda / (lambda + n_aug);
	m_weights_(0) = weight_0;

	for (int i = 1; i < (m_aug_sigma_points); i++) {
		m_weights_(i) = 0.5 / (n_aug + lambda);

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
void UnscentedKalmanFilter::PredictMeasurement(Data::Measurement* pMeasurement) {

	auto strategy = pMeasurement->GetStateAdapterStrategy();

	auto m_measurement_dimensions = strategy->GetDimensions();

	strategy->ComputeEstimatedState(m_sigma_points_predicted_, m_weights_);

	strategy->ComputePredictedCrossCorrelation(m_sigma_points_predicted_, m_weights_, m_kalman_filter_.GetState());
}
//Step 6 : Update state using the typical equations in EKF
// Since we are dealing with matrices of sigma points predictions and
// corresponding mean and covariance matrix of predicted measurement,
// We need to also compute the cross correlation matrix of X and Z
void UnscentedKalmanFilter::Update(Data::Measurement* pMeasurement) {
	// get the StateAdapterStrategy
	auto strategy = pMeasurement->GetStateAdapterStrategy();

	auto m_measurement_dimensions = strategy->GetDimensions();

	auto zMean = strategy->GetPredictedMeasurementMean();
	
	auto zCovariance = strategy->GetPredictedMeasurementCovariance();

	auto xzCovariance = strategy->GetPredictedCrossCorrelation();

	m_kalman_filter_.Update(pMeasurement->GetVectorizedData(), zMean, zCovariance, xzCovariance, strategy->CheckNormalizeAngle(), strategy->GetNormalizeAngleIndex());

	m_current_NIS = strategy->CalculateNIS(pMeasurement->GetVectorizedData());

}

// x_ = x_aug matrix
// P_ = P_aug matrix
void UnscentedKalmanFilter::FillSigmaPointMatrix(Eigen::VectorXd&  x_, Eigen::MatrixXd& P_, Eigen::MatrixXd& Xsig) {
	if (Xsig.rows() != x_.size()) {
		return;
	}
	// number of dimensions of state
	auto n_x = x_.size();

	// Lambda is used to define weights used to generate sigma points
	// a.k.a Spreading parameter
	auto lambda = 3 - n_x;

	// weights also involve finding the sqrt of covariance matrix P
	// Compute square root of P
	Eigen::MatrixXd sqrtP = P_.llt().matrixL();

	// First column of 2 * n_x + 1 is just filled with mean values of x_
	// Which is x itself
	Xsig.col(0) = x_;

	for (int i = 0; i < n_x; i++) {
		auto spreadFactor = sqrt(lambda + n_x) * sqrtP.col(i);
		Xsig.col(i + 1) = x_ + spreadFactor;
		Xsig.col(i + 1 + n_x ) = x_ - spreadFactor;
	}
}
Eigen::VectorXd UnscentedKalmanFilter::ComputeSigmaPointPredictionPerColumn(Eigen::VectorXd& xSigAugCol, const double deltaT) {
	double p_x = xSigAugCol(0);
	double p_y = xSigAugCol(1);
	double v = xSigAugCol(2);
	double psi = xSigAugCol(3);
	double psidot = xSigAugCol(4);
	double nu_a = xSigAugCol(5);
	double nu_psidd = xSigAugCol(6);

	// predicted state values
	double px_p, py_p;

	// avoid division by zero;
	// Zero yaw acceleration is dealt with at else clause
	if (fabs(psidot) > 0.001) {
		px_p = p_x + v / psidot * ( sin (psi + psidot * deltaT) - sin(psi));
		py_p = p_y + v / psidot * ( cos(psi) - cos(psi + psidot * deltaT) );
	} else {
		px_p = p_x + v * deltaT * cos(psi);
		py_p = p_y + v * deltaT * sin(psi);
	}

	double v_p = v;
	double psi_p = psi + psidot * deltaT;
	double psidot_p = psidot;

	// add noise to predicted x, y states
	auto psiCoefficient = 0.5 * nu_a * deltaT * deltaT ;
	px_p = px_p + psiCoefficient * cos(psi);
	py_p = py_p + psiCoefficient * sin(psi);
	v_p = v_p + nu_a * deltaT;
	psi_p = psi + psidot * deltaT + 0.5 * nu_psidd * deltaT * deltaT;
	psidot_p = psidot + nu_psidd * deltaT;

	Eigen::VectorXd predictedState = Eigen::VectorXd(m_state_dimensions);
	predictedState << px_p , py_p , v_p, psi_p, psidot_p;
	return predictedState;
}
float UnscentedKalmanFilter::ComputeDeltaT(long long pTimestamp) {
	return (pTimestamp - m_previous_timestamp_) / 1000000.0; //dt - expressed in seconds
}
/*float Normalize(float pTheta) {
	while (pTheta > M_PI) pTheta -= 2.*M_PI;
	while (pTheta < -M_PI) pTheta += 2.*M_PI;
	return pTheta;
}*/
