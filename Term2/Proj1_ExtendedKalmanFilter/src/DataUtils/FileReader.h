#ifndef FILEREADER_H
#define FILEREADER_H
#include <string>
#include <fstream> //ifstream

#include "DataUtils/DataReader.h"
// FileReader class inherits from DataReader and implements data from a file
namespace DataUtils {
class FileReader : public DataReader {
public:
  /** Constructor
  * @param pDataAdapter contains a unique_ptr of DataAdapter
  * @param pApplication contains a unique_ptr of SensorFusionApplication
  * The constructor passes on parameters to Base Class
  */
  FileReader(std::unique_ptr<DataAdapter> pDataAdapter,
             std::unique_ptr<SensorFusionApplication> pApplication):
    DataReader(std::move(pDataAdapter),
               std::move(pApplication)) {}

  virtual void Run();
  
  void SetSource(const std::string& pFileName) {
    m_file_name_ = pFileName;
  }
private:
  // input file name
  std::string m_file_name_;

  // file reader stream
  std::ifstream m_file_reader_;
};
}
#endif