#ifndef UTILS_H
#define UTILS_H
#include <string>
/** Utils class is used by WebClientReader to parse string information to
* extract sensor measurements.
*/
namespace DataUtils {
class Utils {
public:
  /** Constructor
  * @param pStartDelimiter indicates starting delimiter for parsing text data
  * @param pEndDelimiter indicates ending delimiter for parsing text
  * @param pEOFDelimiter is an EOF delimiter
  */
  Utils(const std::string& pStartDelimiter,
        const std::string& pEndDelimiter,
        const std::string& pEOFDelimiter):
    m_start_delim_(pStartDelimiter),
    m_end_delim_(pEndDelimiter),
    m_EOF_delim_(pEOFDelimiter),
    m_default_string_("") {}

  ~Utils() {};

  // GetData member function returns parsed data given the input string
  const std::string GetData(const std::string& s);

  /** hasData member function returns logical value indicating if input data
  * contains sensor measurement data based on the prefix and suffix characters
  */
  bool HasData(const char* pData, const size_t pLength);

private:
  // starting delimiter string in a data line
  std::string m_start_delim_;

  // ending delimiter string in a data line
  std::string m_end_delim_;

  // EOF delimiter in the input data
  std::string m_EOF_delim_;

  //empty default string
  const std::string m_default_string_;
};
}
#endif