#ifndef INPUTPARSER_H
#define INPUTPARSER_H
#include <string>
/** InputParser class reads in command-line inputs
* and stores the information. 
*  It is used by main.cpp to feed data to DataReader
*/
namespace DataUtils{
	class InputParser{
		public:
			/** Constructor
			* @param defaultSource  uses to set mSource_
			* @param defaultOutput uses to set mOutput_
			*/
			InputParser(const std::string& defaultSource, 
				const std::string& defaultOutput):
				mSource_(defaultSource), 
				mOutput_(defaultOutput){}

			/** Destructor
			*
			*/
			~InputParser(){}

			/* parseArgs method takes in argc, argv
			* an processes command line parameters
			* @param argc argument count
			* @param argv pointer to array of argument
			* options and values 
			*/
			void parseArgs(int argc, char** argv);

			/** getSource member method which returns
			* data source as const reference
			*/
			const std::string& getSource() const {
				return mSource_;
			}

			/** getOutput member method which returns 
			* output file name as const reference
			*/
			const std::string& getOutput() const {
				return mOutput_;
			}
			
			/** getDatatype member method which returns
			* type of data to be parsed (laser / radar)
			* as const reference
			*/
			const std::string& getDataType() const{
				return mDataType_;
			}

			/** isParseSuccessful member method returns a
			* bool indicating if parsing of information
			* was successful
			*/
			bool isParseSuccessful() const{
				return mIsParseSuccessful_;
			}
		private:

			/** showUsage method is help function
			* which outputs man information of 
			* using this application
			*/
			void showUsage(const std::string& name);

			// data source 
			std::string mSource_;

			// output file name 
			std::string mOutput_;

			// laser / radar data type 
			std::string mDataType_;

			
			bool mIsParseSuccessful_;
	};
}
#endif
