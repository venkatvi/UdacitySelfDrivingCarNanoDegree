#include "./DataAdapter.h" 

// enum of types of data to be processed 
#include "./DataTypes.h"

// Concrete derived measurement classes
#include "../Data/LaserMeasurement.h" 
#include "../Data/RadarMeasurement.h" 

// StateAdapterStrategyFactory class to provide 
// with a polymorphic measurement function classes 
// for each measurement class
#include "../Data/StateAdapterStrategyFactory.h"


#include <iostream>
#include <algorithm> //toupper

/**Constructor 
* @param pDataType string  containing values "laser" or 
* "radar" or "both" from the command line arguments
* The constructor reads this argument and converts it to 
* SensorDataType field that is stored in member variable 
* mDataType_
*/
DataUtils::DataAdapter::DataAdapter(
  const std::string& pDataType){
  // convert input param to upper case for 
  // case-insensitive match
  auto upperCaseStr = pDataType;
  std::transform(upperCaseStr.begin(), 
          upperCaseStr.end(), 
          upperCaseStr.begin(), 
          ::toupper);

  // if string is laser, store SensorDataType as LASER
  if(upperCaseStr.compare("LASER") == 0){

    mDataType_ = DataUtils::SensorDataTypes::LASER;
  
  }else if(upperCaseStr.compare("RADAR") == 0) {
  
    // if string is radar, store SensorDataType as RADAR
    mDataType_ = DataUtils::SensorDataTypes::RADAR;
  
  }else if(upperCaseStr.compare("BOTH") == 0){
  
    // if string is both, store SensorDataType as BOTH
    mDataType_ = DataUtils::SensorDataTypes::BOTH;
  
  }else{
  
    // store SensorDataType as UNKNOWN
    mDataType_ = DataUtils::SensorDataTypes::UNKNOWN;
  
  }   
}
/** parseData creates a istringstream of mData_
* and uses the stringstream to parse measurement and 
* ground truth related information 
*/
void DataUtils::DataAdapter::parseData(){
  // set the stream to mData_
  mDataStream_.str(mData_); 

  // parse measurement information
  parseMeasurementData();

  // parse ground truth information 
  parseGroundTruthData();
}

/** parseMeasurementData expectes data to be of 
* the following format : "L px py timestamp" or
* "R rho theta rho_dot timestamp"
* This member function is a factory method for creating
* a polymorphic measurement class based on the parsed 
* results.
*/
void DataUtils::DataAdapter::parseMeasurementData(){
  // Create a Measurement Pointer 
  Measurement* pMeasurementPtr;

  /* Each derived class of Measurement requires a 
  * corresponding measurement function (h'(x)) to be
  * used in KalmanFilter update() stage. 
  *
  * Also, each derived class of Measurement also requires 
  * a polymorphic implementation for estimating the 
  * "measurement" equivalent of given a state for computing 
  * the error y = z - h(x) to be used in update() stage.
  *
  * An abstract factory pattern is used to construct a
  * parallel hierarchy of StateAdapterStrategies based on the
  * Measurement class hierarchy. 
  *
  * Each Measurement class will also hold on to the 
  * corresponding StateAdapterStrategy. 
  * 
  * The sensory type (laser / radar) is used by the 
  * StateAdapterStrategyFactory class to generate the 
  * polymorphic StateAdapterStrategy class.
  *
  * As seen in the Measurement hierarchy, StateAdapterStrategy 
  * hierarchy is also easily extensible without having a need to 
  * change the underlying implementation of Kalman Filter
  */

  // reads first element from the stream
  std::string sensorType;
  mDataStream_ >> sensorType;

  // Create a StateAdapterStrategyFactory instance
  StateAdapterStrategyFactory pStateAdapterStrategyFactory;
  pStateAdapterStrategyFactory.setSensorType(sensorType);

  std:size_t measurementDimensions = 0;

  // if data is a laser measurement, and command line input 
  // requires laser data to be included for Kalman Filter computation, 
  // process data. 
  if (sensorType.compare("L") == 0 && processLaserData()) {
   // Laser measurement data is of the format:
   // "L px py timestamp"
   float pX;
     float pY;
     long long pTimestamp;

     // Read px, py and timestamp from mData_
     mDataStream_ >> pX;
   mDataStream_ >> pY;
   mDataStream_ >> pTimestamp;
   
   // Create a LaserMeasurement object pointer 
   // using px, py and timestamp as constructor arguments
   pMeasurementPtr = new LaserMeasurement(pTimestamp, pX, pY);

   // Use static method to get the number of input dimensions
   // for Laser Measurements. (This is used to avoid the use of 
   // MAGIC NUMBERS in code)
   measurementDimensions = LaserMeasurement::getInputDimensions();
   
   // Get polymorphic StateAdapterStrategy from StateAdapterStrategyFactory
   // using the measurementDimensions. The dimensions are required to set 
   // the dimensionality of covariance matrices in Strategy classes.

   // Set StateAdapterStrategy in Measurement pointer
   pMeasurementPtr->setStateAdapterStrategy(
     pStateAdapterStrategyFactory.getStateAdapterStrategy(
                   measurementDimensions));

   // Create a unique instance of the Measurement pointer using move semantics
   std::unique_ptr<Measurement> pMeasurementUPtr(pMeasurementPtr);
   mMeasurement_ =  std::move(pMeasurementUPtr);
  } else if (sensorType.compare("R") == 0 && processRadarData()) {
   // if data is a radar measurement, and command line input 
   // requires radar data to be included for Kalman Filter computation, 
   // process data. 

   // Radar measurement is of the format:
   // "R rho theta rho_dot timestamp"

   // Read rho theta rho_dot timestamp from mData_
   float pRho;
     float pTheta;
     float pRhoDot;
   mDataStream_ >> pRho;
   mDataStream_ >> pTheta;
   mDataStream_ >> pRhoDot;

   long long pTimestamp;
   mDataStream_ >> pTimestamp;

   // Create a RadarMeasurement object pointer 
   // using rho, theta, rho_dot and timestamp as constructor 
   // arguments
   pMeasurementPtr = new RadarMeasurement(pTimestamp, 
                       pRho, 
                       pTheta, 
                       pRhoDot);

   // Get input dimensions of radar measurements using a 
   // static member function
   measurementDimensions = RadarMeasurement::getInputDimensions();
   
   // Get polymorphic StateAdapterStrategy from StateAdapterStrategyFactory
   // using the measurementDimensions. The dimensions are required to set 
   // the dimensionality of covariance matrices in Strategy classes.

   // Set StateAdapterStrategy in Measurement pointer
   pMeasurementPtr->setStateAdapterStrategy(
     pStateAdapterStrategyFactory.getStateAdapterStrategy(
               measurementDimensions));

   // Create a unique instance of the Measurement pointer using move semantics
   std::unique_ptr<Measurement> pMeasurementUPtr(pMeasurementPtr);
   mMeasurement_ =  std::move(pMeasurementUPtr);
  }else{
   // If a command line argument does not require reading either laser / radar data
   // Set measurement to NULL.
   mMeasurement_.reset(nullptr);
  }
}
/** parseGroundTruthData expectes data to be of 
* the following format : "gt_x gt_y gt_vx gt_vy" 
* This member function uses mDataStream_ to extract these fields
* and create a unique_ptr to a State object instance
*/
void DataUtils::DataAdapter::parseGroundTruthData(){
 // Read Ground Truth data only when Measurement is already 
 // computed. 
 if (mMeasurement_){
  // Read gt_x, gt_y, gt_vx, gt_vy from mDataStream_
  float pX;
  float pY;
  float vX;
  float vY;
  mDataStream_ >> pX;
  mDataStream_ >> pY;
  mDataStream_ >> vX;
  mDataStream_ >> vY;

  // Create a unique_ptr to State using move semantics
  // and store it as a member variable mState_
  mState_ = std::move(std::make_unique<State>(pX, pY, vX, vY)); 
 }

}