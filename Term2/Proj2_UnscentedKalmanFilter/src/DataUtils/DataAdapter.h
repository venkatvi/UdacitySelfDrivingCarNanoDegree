#ifndef DATAADAPTER_H
#define DATAADAPTER_H
#include <memory> //unique_ptr
#include <sstream> //istringstream

#include "../Data/Measurement.h"
#include "../Eigen/Dense"
//Enum class indicating the type of data - laser/radar/both
#include "../DataUtils/DataTypes.h"

/** DataAdapter class adapts string data to measurement and ground truth objects.
*/
namespace DataUtils {
class DataAdapter {
public:
   /**
   * Constructor
   * @param pDataType string from command line input
   * indicating the use of laser / radar / both data
   */
   DataAdapter(const std::string& pDataType);
   ~DataAdapter() {}

   /*** SetData setter for member variable m_data_
   * @param pData containing the text of data consisiting of
   * measurements and ground truth from data source
   */
   void SetData(const std::string& pData) {
      m_data_ = pData;
   }

   /*** ParseData parses m_data_ and extracts
   * measurement and ground truth information
   */
   void ParseData();

   /*** GetMeasurement returns pointer to Measurement. This is not a const
   * function as other classes modifies the measurement pointer. The
   * ownership of this resource is still owned by DataAdapter.
   */
   Data::Measurement* GetMeasurement() {
      return m_measurement_.get();
   }

   /* GetState getter for pointer to State. This is not a const
   * function as other classes modifies the pointer. The  ownership of this
   * resource is still owned by DataAdapter.
   */
   const Data::GroundTruth*  GetGroundTruth() {
      return m_groundtruth_.get();
   }

private:
   /** ProcessLaserData const member function to indicate if the member
   * variable m_data_type_ is set either Laser or BOTH. This API is used to
   * process any laser measurements
   */
   bool ProcessLaserData() const {
      return (m_data_type_ == SensorDataTypes::LASER ||
              m_data_type_ == SensorDataTypes::BOTH);
   }
   /** ProcessRadarData const member function to indicate if the member
   * variable m_data_type_ is set either radar or BOTH. This API is used to
   * process any radar measurements
   */
   bool ProcessRadarData() const {
      return (m_data_type_ == SensorDataTypes::RADAR ||
              m_data_type_ == SensorDataTypes::BOTH);
   }

   /** pProcessMeasurement extracts Measurement related fields from m_data_
   *  string using m_data_stream_ member. This is a factory method which returns
   *  a polymorphic measurement class based on the encoding information
   * in m_data_
   */
   void ParseMeasurementData();

   /*** processGroundTruthData extracts ground truth related fields from m_data_
   *  string using m_data_stream_ member
   */
   void ParseGroundTruthData();

   // string to store the data from web client or file
   std::string m_data_;

   // istringstream to process m_data_ to extract
   // measurements and ground truth information
   std::istringstream m_data_stream_;

   std::unique_ptr<Data::Measurement> m_measurement_;

   std::unique_ptr<Data::GroundTruth> m_groundtruth_;

   // type of data to be processed by DataAdapter for
   // storing measurements and ground truths
   SensorDataTypes m_data_type_;

};
}
#endif