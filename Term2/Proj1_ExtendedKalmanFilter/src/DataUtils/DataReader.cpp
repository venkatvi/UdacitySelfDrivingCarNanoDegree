#include "./DataReader.h"
/** writeResultsToFile method writes pState 
* to output file 
* @param pState contains a pointer to 
* const State
* This method writes current Measurement, ground truth 
* and the predicted state to the output file
*/
void DataUtils::DataReader::writeResultsToFile(
 const State* pState){
 // get measurement and ground truth from DataAdapter
 auto currentMeasurement = mDataAdapter_->getMeasurement();
 auto currentGroundTruth = mDataAdapter_->getGroundTruth();

 // Output measurement type "L" or "R" 
 mOutputDataStream_ << currentMeasurement->getMeasurementType() << ", ";

 // Output predicted state
 mOutputDataStream_ << *pState;

 // Output current measurement
 mOutputDataStream_ << *currentMeasurement;

 // Output current ground truth
 mOutputDataStream_ << *currentGroundTruth; 
 mOutputDataStream_ << std::endl;
}
/** processData method takes in a string and processes data to
* parse measurement and ground truth information. 
* The method uses the measurement and ground truth with 
* SensorFusionApplication to get the predicted state.
* @param pData string containing client data
* returns a pointer to const State
*/
const State* DataUtils::DataReader::processData(
 const std::string& pData){
 // Set Data and parse data with DataAdapter
 mDataAdapter_->setData(pData);
 mDataAdapter_->parseData();

 // Get current measurement and ground truth 
 auto currentMeasurement = mDataAdapter_->getMeasurement();
 auto currentGroundTruth = mDataAdapter_->getGroundTruth();

 // If DataAdapter has current measurement and ground truth parsed
 if (currentMeasurement && currentGroundTruth){
  // Run KalmanFilter using SensorFusionApplication
  auto predictedState = mApplication_->run(currentMeasurement, 
           currentGroundTruth);
  return predictedState;
 }
 return nullptr; 
}