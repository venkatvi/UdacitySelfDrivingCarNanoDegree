#include "Twiddler.h"

#include <iostream>
void Twiddler::InitDeltaParameters(const std::vector<double>& pPIDParameters){
	for(std::size_t idx = 0; idx < pPIDParameters.size(); ++idx){
		m_delta_p_.push_back( 0.1 * pPIDParameters[idx]);
	}
}
void Twiddler::Run(const double pCurrentError, std::vector<double>& pParameters){
	
	std::cout << "Twiddling ..... " << std::endl;
	// Compute the sum of all delta parameters 
	auto sum_of_delta_parameters = CalculateSumOfDeltaP();

	// Modify pParameters only if sum of current delta > allowable error tolerance 
	//if(sum_of_delta_parameters > m_allowable_error_tolerance){

		// If the observed error for pParameters is < best_error mantained by
		// twiddler thus far, update error. 
		if(pCurrentError < m_best_error_){
			std::cout << "Improvement! Increasing delta.." << std::endl;

			m_best_error_ = pCurrentError;

			// increase delta 
			m_delta_p_[m_delta_p_index_] *= 1.1;

			// increment index to next parameter
			IncrementIndex();
			
		}
		// if PID parameters have neither been incremented / decremented, 
		// try adding delta first 
		if(!m_delta_added_ && !m_delta_subtracted_){
			std::cout << "Incrementing index " << m_delta_p_index_ << std::endl;

			// Add value to PID parameter 
			pParameters[m_delta_p_index_] += m_delta_p_[m_delta_p_index_];

			// set added flag to true;
			m_delta_added_ = true;

		}else if(m_delta_added_ && !m_delta_subtracted_){ 
			std::cout << "Decrementing index " << m_delta_p_index_ << std::endl;

			// if adding delta has been tried, try subtracting delta 
			pParameters[m_delta_p_index_] += -2 * m_delta_p_[m_delta_p_index_];

			// set substracted flag to true
			m_delta_subtracted_ = true;
		}else{
			std::cout << "Decreasing delta... " << std::endl;

			// If both adding and subtracting delta has been tried, 
			// decrease delta 
			m_delta_p_[m_delta_p_index_] *= 0.9;

			// increment index to next parameter
			IncrementIndex();
		}
	/*}else{
		std::cout << "No op by Twiddler. Current error meets error tolerance thresholds" << std::endl;
	}*/
}

double Twiddler::CalculateSumOfDeltaP(){
	// Compute sum of all delta parameters 
	double sum_of_delta_parameters = 0;
	for(auto& n : m_delta_p_){
		sum_of_delta_parameters += n;
	}
	return sum_of_delta_parameters;
}
void Twiddler::IncrementIndex(){
	int numParams = m_delta_p_.size();
	// increment index of interest by 1
	m_delta_p_index_ = (m_delta_p_index_+1) % numParams;

	// reset m_delta_added and m_delta_subtracted
	m_delta_added_ = false;
	m_delta_subtracted_ = false;
}