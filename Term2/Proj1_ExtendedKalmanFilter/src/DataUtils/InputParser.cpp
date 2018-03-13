#include "../DataUtils/InputParser.h"

#include <iostream>
void DataUtils::InputParser::ParseArgs(int argc, char** argv) {
  if (argc < 2) {
    ShowUsage(argv[0]);
    m_is_parse_successful_ = false;
  }
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if ((arg == "-h") || (arg == "--help")) {
      ShowUsage(argv[0]);
      m_is_parse_successful_ = false;
    } else if ((arg == "-s") || (arg == "--source")) {
      if (i + 1 < argc) { // Make sure we aren't at the end of argv!
        m_source_ = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
      }
    } else if ((arg == "-o") || (arg == "--output")) {
      if (i + 1 < argc) {
        m_output_ = argv[++i];
      }
    } else if ((arg == "-t") || (arg == "--type")) {
      if (i + 1 < argc) {
        m_data_type_ = argv[++i];
      }
    }
    else
    {
      ShowUsage(argv[0]);
      m_is_parse_successful_ = false;
    }
  }
  m_is_parse_successful_ = true;
}
void DataUtils::InputParser::ShowUsage(const std::string& name)
{
  std::cerr << "Usage: " << name << " <option(s)> values \n"
            << "Options:\n"
            << "\t-h,--help\t\tShow this help message\n"
            << "\t-s,--source SOURCE\tSpecify the source of dataset.\n"
            << "\tValid (Case-sensitive) options are : \n"
            << "\t 1. \"Simulator\" or \n"
            << "\t 2. Input file name. Default option is \"./input.txt\"\n"
            << "\t-o,--output OUTPUT\tSpecify the output file name. Default option is \"./output.txt\"\n"
            << "\t-t,--type TYPE\t Specify the type of data to be used.\n"
            << "\tValid (Case-sensitive) options are: \n"
            << "\t \"Laser\" for using laser data only\n"
            << "\t \"Radar\" for using radar data only\n"
            << "\t \"Both\" for using both types of data. Default option is \"Both\" \n"
            << std::endl;
}