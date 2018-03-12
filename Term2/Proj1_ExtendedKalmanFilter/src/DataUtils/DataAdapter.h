#ifndef DATAADAPTER_H
#define DATAADAPTER_H
#include "../Data/Measurement.h" 
#include "../Data/State.h"
#include "../Eigen/Dense"

//Enum class indicating the type of data - laser/radar/both
#include "./DataTypes.h" 

#include <memory> //unique_ptr
#include <sstream> //istringstream

/** DataAdapter class adapts string data
* to measurement and ground truth objects.
*/
namespace DataUtils{
 class DataAdapter{
  public:
   /**
   * Constructor
   * @param pDataType string from command line input 
   * indicating the use of laser / radar / both data
   */
   DataAdapter(const std::string& pDataType);

   /**
   * Destructor
   */
   ~DataAdapter(){}
   
   /*** setData setter for member variable mData_
   * @param pData containing the text of data consisiting of
   * measurements and ground truth from data source
   */
   void setData(const std::string& pData){
       mData_ = pData;
   }

   /*** parseData parses mData_ and extracts 
   * measurement and ground truth information
   */
   void parseData();

   /*** getMeasurement getter for pointer to Measurement
   * This is not a const function as other classes modifies 
   * the measurement pointer.
   */
   Measurement* getMeasurement(){
       return mMeasurement_.get();
   }

   /* getState getter for pointer to State
   */
   const State*  getGroundTruth(){
       return mState_.get();
   }

  private:
   /*** processLaserData const member function to indicate
   * if the member variable mDataType_ is set either Laser 
   * or BOTH. This API is used to process any laser measurements
   */
   bool processLaserData() const{
       return (mDataType_ == SensorDataTypes::LASER || 
               mDataType_ == SensorDataTypes::BOTH);
   }
   /*** processRadarData const member function to indicate
   * if the member variable mDataType_ is set either radar 
   * or BOTH. This API is used to process any radar measurements
   */
   bool processRadarData() const{
       return (mDataType_ == SensorDataTypes::RADAR || 
               mDataType_ == SensorDataTypes::BOTH);
   }
   
   /*** processMeasurement extracts Measurement related 
   * fields from mData_ string using mDataStream_ member
   * This is a factory method which returns a polymorphic 
   * measurement class based on the encoding information
   * in mData_
   */
   void parseMeasurementData();

   /*** processGroundTruthData extracts ground truth related 
   * fields from mData_ string using mDataStream_ member
   */
   void parseGroundTruthData();
   
   // string to store the data from web client or file
   std::string mData_;

   // istringstream to process mData_ to extract 
   // measurements and ground truth information
   std::istringstream mDataStream_;

   // Unique pointer to Measurement
   std::unique_ptr<Measurement> mMeasurement_;

   // Unique pointer to ground truth state
   std::unique_ptr<State> mState_;

   // type of data to be processed by DataAdapter for 
   // storing measurements and ground truths
   SensorDataTypes mDataType_; 

 };
}
#endif