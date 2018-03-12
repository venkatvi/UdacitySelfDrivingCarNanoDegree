#include "InputParser.h"
#include <iostream>
/** parseArgs method inputs argc and argv 
* @param argc argument count
* @param argv containing argument options and values
*/
void DataUtils::InputParser::parseArgs(int argc, char** argv){
  
 if (argc < 2){
  showUsage(argv[0]);
  mIsParseSuccessful_ = false;
 }
 for (int i = 1; i < argc; ++i) {
  std::string arg = argv[i];
  if ((arg == "-h") || (arg == "--help")) {
   showUsage(argv[0]);
   mIsParseSuccessful_ = false;
  } else if ((arg == "-s") || (arg == "--source")) {
   if (i + 1 < argc) { // Make sure we aren't at the end of argv!
    mSource_ = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
   }  
  } else if ((arg == "-o") || (arg == "--output")) {
   if (i+1 < argc){
    mOutput_ = argv[++i];
   }
  } else if ((arg == "-t") || (arg == "--type")) {
   if (i+1 < argc){
    mDataType_ = argv[++i];
    }
  } 
  else
  {
   showUsage(argv[0]);
   mIsParseSuccessful_ = false;
  }
 }
 mIsParseSuccessful_ = true;
}
void DataUtils::InputParser::showUsage(const std::string& name)
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