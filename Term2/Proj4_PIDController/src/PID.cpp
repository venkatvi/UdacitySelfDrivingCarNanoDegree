#include "PID.h"

#include<iostream>

PID::PID(PIDConfiguration& pConfig): 
	m_config_(pConfig), 
	m_twiddler_(pConfig.GetTwiddleErrTol()),
	m_parameter_error_(NUM_PID_PARAMETERS,0),
	m_parameters_(NUM_PID_PARAMETERS, 0){} 
		  
PID::~PID() {}

void PID::InitParameters() {
	//bool doTwiddle, const double twiddlerErrorTolerance):
	m_do_twiddle_ = m_config_.DoTwiddle();
	
	
	m_current_step_ = 0;

	m_parameters_[PIDParameters::Kp]= m_config_.GetKp(); 
	m_parameters_[PIDParameters::Ki]= m_config_.GetKi(); 
	m_parameters_[PIDParameters::Kd]= m_config_.GetKd(); 
	
	m_parameter_error_[PIDParameters::Kp]= 0; 
	m_parameter_error_[PIDParameters::Ki]= 0; 
	m_parameter_error_[PIDParameters::Kd]= 0; 
	
	if (m_do_twiddle_){
		m_twiddler_.InitDeltaParameters(m_parameters_);
	}
}

void PID::UpdateError(double cte) {
	//std::cout << "Step: " << m_current_step_ << " CTE: " << cte << std::endl;

	m_parameter_error_[PIDParameters::Kd] = cte - m_parameter_error_[PIDParameters::Kp];
	m_parameter_error_[PIDParameters::Kp] = cte; 
	m_parameter_error_[PIDParameters::Ki] += cte;

	double totalError = 0;
	if (m_current_step_ % (m_warmup_steps_count_ + m_eval_steps_count_) > m_warmup_steps_count_){
		totalError = TotalError();
	}
	if(m_do_twiddle_ && m_current_step_ %(m_warmup_steps_count_ + m_eval_steps_count_) == 0){
		m_twiddler_.Run(totalError, m_parameters_);
	}
	m_current_step_++;
}

double PID::TotalError() {
	double totalError = 0; 
	for (std::size_t idx = 0; idx < m_parameters_.size(); ++idx){
		totalError += m_parameters_[idx] * m_parameter_error_[idx];
	}
	return totalError;
}

