#ifndef INPUTPARSER_H
#define INPUTPARSER_H
#include <string>
/** InputParser class reads in command-line inputs and stores the information
*  It is used by main.cpp to feed into PID controller algorithm
*/
class InputParser {
public:
	/** Constructor
	* @param defaultSource  uses to set m_source_
	* @param defaultOutput uses to set m_output_
	*/
	InputParser():m_implement_throttle_pid_(true) {}
	~InputParser() {}

	/* parseArgs method takes in argc, argv an processes command line
	* parameters
	* @param argc argument count
	* @param argv pointer to array of argument options and values
	*/
	void ParseArgs(int argc, char** argv);
	
	bool IsParseSuccessful() const {
		return m_is_parse_successful_;
	}
	const double GetSteerParams() const {
	  return m_steer_twiddle_err_tol_;
	}
	const double GetThrottleParams() const{
	  return m_throttle_twiddle_err_tol_;
	}
	bool ImplementPIDForThrottle() const {
		return m_implement_throttle_pid_;
	}
private:

	/** showUsage method is help function which outputs man information
	*  of using this application
	*/
	void ShowUsage(const std::string& name);

	// twiddle parameter for steer
	double m_steer_twiddle_err_tol_; 

	// twiddle parameter for throttle
	double m_throttle_twiddle_err_tol_;

	bool m_implement_throttle_pid_;

	bool m_is_parse_successful_;
};
#endif
