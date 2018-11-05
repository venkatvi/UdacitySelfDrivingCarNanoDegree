#ifndef TWIDDLER_H
#define TWIDDLER_H

#include <limits>
#include <vector>
class Twiddler{
public:
	// Constructor
	Twiddler(const double pTolerances): 
		m_allowable_error_tolerance(pTolerances){}

	// Destructor
	~Twiddler(){}

	void InitDeltaParameters(const std::vector<double>& pPIDParameters);

	// Run twiddle  algorithm one modifying one parameter each time 
	void Run(const double currentError, std::vector<double>& pParameters); 
private:
	double CalculateSumOfDeltaP();

	void IncrementIndex();

	// delta_parameters; 
	std::vector<double> m_delta_p_; 

	// best_error found so far
	double m_best_error_ = std::numeric_limits<double>::max(); 

	// allowable error tolerance
	double m_allowable_error_tolerance; 

	// delta vector index 
	int m_delta_p_index_ = 0;

	// flags for mantaining twiddle direction logic 
	// to add / subtract / decrease delta 
	bool m_delta_added_ = false;

	bool m_delta_subtracted_ = false;


};
#endif