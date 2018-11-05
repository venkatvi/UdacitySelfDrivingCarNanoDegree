#ifndef PIDConfiguration_H
#define PIDConfiguration_H
class PIDConfiguration {
public:
	PIDConfiguration(const double pKp, 
		const double pKi, 
		const double pKd):
		m_Kp_(pKp),
		m_Ki_(pKi),
		m_Kd_(pKd),
		m_twiddle_(false),
		m_twiddle_err_tol_(0.0){}
	
	~PIDConfiguration() {}

	// Getters for PID config parameters 
	const double GetKp() const {
		return m_Kp_;
	}

	const double GetKi() const{
		return m_Ki_;
	}

	const double GetKd() const {
		return m_Kd_;
	}

	const bool DoTwiddle() const {
		return m_twiddle_;
	}
	const double GetTwiddleErrTol() const {
		return m_twiddle_err_tol_;
	}

	void SetTwiddleParameters(const double pErrTol){
		m_twiddle_err_tol_ = pErrTol;
		if (m_twiddle_err_tol_){
			m_twiddle_ = true;
		}
	}
private:
	//P
	double m_Kp_;
	//I
	double m_Ki_; 

	//D
	double m_Kd_;

	//flag to decide if twiddle is required  while tuning pid
	bool m_twiddle_;

	//twiddle error tolerance
	double m_twiddle_err_tol_;
};
#endif 