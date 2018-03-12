#ifndef FILEREADER_H
#define FILEREADER_H
#include <string>
#include <fstream> //ifstream
#include "./DataReader.h" 
/** FileReader class inherits from DataReader
* and implements data from a file
*/
namespace DataUtils{
 class FileReader : public DataReader{
  public:
   /** Constructor
   * @param pDataAdapter contains a unique_ptr of DataAdapter
   * @param pApplication contains a unique_ptr of 
   * SensorFusionApplication
   * The constructor passes on parameters to Base Class
   */
   FileReader(std::unique_ptr<DataAdapter> pDataAdapter, 
    std::unique_ptr<SensorFusionApplication> pApplication): 
    DataReader(std::move(pDataAdapter), 
         std::move(pApplication)){}

   /** run method is overriden virtual method
   */
   virtual void run();

   /** setSource method sets the input data source file name
   * This file is used to read data from and is fed to 
   * SensorFusionApplication
   */
   void setSource(const std::string& pFileName){
    mFileName_ = pFileName;
   }

  private:
   // input file name 
   std::string mFileName_;

   // file reader stream 
   std::ifstream mFileReader_;
 };
}
#endif