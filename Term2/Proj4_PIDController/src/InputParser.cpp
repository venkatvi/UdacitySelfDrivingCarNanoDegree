#include "InputParser.h"

#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
void InputParser::ParseArgs(int argc, char** argv) {
  if (argc < 2) {
    ShowUsage(argv[0]);
    m_is_parse_successful_ = false;
  }
  int opt;
  while((opt = getopt(argc, argv, "s:t:")) != -1){
    switch(opt){
      case 's':
        m_steer_twiddle_err_tol_ = atof(optarg); 
        std::cout << "Twiddle learning rate for steer: " << optarg << std::endl;
        break;
      case 't':
        m_throttle_twiddle_err_tol_ = atof(optarg);
        std::cout << "Twiddle learning rate for throttle: " << optarg << std::endl;
    }
  }
  m_is_parse_successful_ = true;
}
void InputParser::ShowUsage(const std::string& name)
{
  std::cerr << "Usage: " << name << " <option(s)> values \n"
            << "Options:\n"
            << "\t-h,--help\t\tShow this help message\n"
            << "\t-s, VALUE\t\tTwiddle learning rate for steer\n"
            << "\t-t, VALUE\t\tTwiddle learning rate for throttle\n"
            << std::endl;
}