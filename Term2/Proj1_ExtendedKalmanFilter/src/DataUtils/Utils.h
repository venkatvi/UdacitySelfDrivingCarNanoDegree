#ifndef UTILS_H
#define UTILS_H
#include <string>
/** Utils class is used by WebClientReader 
* to parse string information to extract 
* sensor measurements.
*/
namespace DataUtils{
 class Utils{
  public:
   /** Constructor 
   * @param pStartDelimiter indicates starting delimiter for parsing text data
   * @param pEndDelimiter indicates ending delimiter for parsing text 
   * @param pEOFDelimiter is an EOF delimiter
   */
   Utils(const std::string& pStartDelimiter, 
      const std::string& pEndDelimiter,
      const std::string& pEOFDelimiter):
    mDataStartDelimiter_(pStartDelimiter), 
    mDataEndDelimiter_(pEndDelimiter),
    mEOFDelimiter_(pEOFDelimiter),
    mDefaultString_(""){}

    /** Destructor
    */
   ~Utils(){};

   /** getData member function returns 
   * parsed data given the input string
   */
   const std::string getData(const std::string& s);

   /** hasData member function returns logical 
   * value indicating if input data contains sensor
   * measurement data based on the prefix and suffix characters
   */
   bool hasData(const char* pData, const size_t pLength);

   /** getDefaultString member function returns an
   * empty string
   */
   const std::string& getDefaultString()const 
   {
    return mDefaultString_;
   }

  private:
   // starting delimiter string in a data line
   std::string mDataStartDelimiter_;

   // ending delimiter string in a data line 
   std::string mDataEndDelimiter_;

   // EOF delimiter in the input data
   std::string mEOFDelimiter_;

   //empty default string
   const std::string mDefaultString_;
 };
}
#endif