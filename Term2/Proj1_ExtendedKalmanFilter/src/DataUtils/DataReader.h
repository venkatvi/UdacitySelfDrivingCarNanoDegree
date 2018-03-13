#ifndef DATAREADER_H
#define DATAREADER_H
#include <string>
#include <memory> // unique_ptr
#include <iostream>
#include <fstream> // ofstream

#include "DataUtils/DataAdapter.h"
#include "SensorFusionApplication.h"

/** DataReader class hierarchy is a set of polymorphic classes for reading data
* from different data sources - Simulator, data file
*/
namespace DataUtils {
class DataReader {
public:
  /** Constructor
  * @param pDataAdapter unique ptr to DataAdapter
  * @param pApplication unique ptr to SensorFusionApplication
  * Uses move semantics to initialize member variables
  * m_data_adapter_ and mSensorFusionApplication_
  */
  DataReader(std::unique_ptr<DataAdapter> pDataAdapter,
             std::unique_ptr<SensorFusionApplication> pApplication):
    m_data_adapter_(std::move(pDataAdapter)),
    m_application_(std::move(pApplication)) {}

  /** Destructor
  * close m_output_data_stream_
  */
  ~DataReader() {
    m_output_data_stream_.close();
  }

  /** setOutputFile sets the m_output_file_name_ with file name
  * It also initializes the m_output_data_stream_ with output data file so that 
  * output can be written into the output file.
  */
  void SetOutputFile(const std::string& pFileName) {
    m_output_file_name_ = pFileName;
    // if output data stream is already open,
    // close it
    if (m_output_data_stream_.is_open()) {
      m_output_data_stream_.close();
    }
    // Re-open the data stream with output file
    m_output_data_stream_.open(m_output_file_name_);
  }

  /** getDataAdapter getter member function returns a pointer to DataAdapter.
  * The class still has the ownership of DataAdapter member
  */
  DataAdapter* GetDataAdapter() {
    return m_data_adapter_.get();
  }

  /** getApplication getter member function returns a pointer to 
  * SensorFusionApplication member. The class still has the ownership of
  * SensorFusionApplication.
  */
  SensorFusionApplication* GetApplication() {
    return m_application_.get();
  }

  /** run method is a pure virtual member function which has concrete 
  * definitions in the concerte derived class implementations of this class.
  */
  virtual void run() = 0;


  /** processData process string pData, parses measurements, ground truth 
  * information using DataAdapter member and then uses SensorFusionApplication 
  * member to compute KalmanFilter estimations
  * @param pData string containing the source information returns pointer to 
  * const State which contains the estimated state. 
  * The memory of the State is owned by KalmanFilter
  */
  const Data::State* processData(const std::string& pData);

  /** writeResultsToFile writes results to output file stored in 
  * m_output_file_name_ member
  * @param pState pointer to const State containing estimates of Kalman Filter
  */
  void writeResultsToFile(const State* pState);

protected:
  // output file name
  std::string m_output_file_name_;

  // output file data stream
  std::ofstream m_output_data_stream_;

  // unique pointer to DataAdapter
  std::unique_ptr<DataAdapter> m_data_adapter_;

  // unique pointer to SensorFusionApplication
  std::unique_ptr<SensorFusionApplication> m_application_;

};
}
#endif