#ifndef PID_H
#define PID_H

#include "Twiddler.h"
#include "PIDConfiguration.h"

#include <vector>

#define NUM_PID_PARAMETERS 3

enum PIDParameters {Kp = 0, Kd, Ki};

class PID {
public:

  /*
  * Constructor
  */
  PID(PIDConfiguration& pConfig);

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void InitParameters();

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();

private: 

  // Twiddle parameters after k iterations 
  bool m_do_twiddle_;

  PIDConfiguration m_config_; 

  Twiddler m_twiddler_;
  
  /*
  * Errors
  */
  std::vector<double> m_parameter_error_;

  /*
  * Coefficients
  */ 
  std::vector<double> m_parameters_;

  /*
  * Counters to maintain number of steps
  */ 

  // Use a total of 2000 steps / iterations to 
  // evaluate one pass of the controller
  const int m_eval_steps_count_ = 500;

  // Additionally use a minimum of 100 steps / iterations 
  // for the controller to settle down 
  const int m_warmup_steps_count_ = 100; 

  int m_current_step_;
};

#endif /* PID_H */
