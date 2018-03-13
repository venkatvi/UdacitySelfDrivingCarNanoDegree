#ifndef INPUTPARSER_H
#define INPUTPARSER_H
#include <string>
/** InputParser class reads in command-line inputs and stores the information
*  It is used by main.cpp to feed data to DataReader
*/
namespace DataUtils {
class InputParser {
public:
	/** Constructor
	* @param defaultSource  uses to set m_source_
	* @param defaultOutput uses to set m_output_
	*/
	InputParser(const std::string& defaultSource,
	            const std::string& defaultOutput):
		m_source_(defaultSource),
		m_output_(defaultOutput) {}
	~InputParser() {}

	/* parseArgs method takes in argc, argv an processes command line
	* parameters
	* @param argc argument count
	* @param argv pointer to array of argument options and values
	*/
	void ParseArgs(int argc, char** argv);

	const std::string& GetSource() const {
		return m_source_;
	}
	const std::string& GetOutput() const {
		return m_output_;
	}
	const std::string& GetDataType() const {
		return m_data_type_;
	}
	bool IsParseSuccessful() const {
		return m_is_parse_successful_;
	}
private:

	/** showUsage method is help function which outputs man information
	*  of using this application
	*/
	void ShowUsage(const std::string& name);

	// data source
	std::string m_source_;

	// output file name
	std::string m_output_;

	// laser / radar data type
	std::string m_data_type_;

	bool m_is_parse_successful_;
};
}
#endif
