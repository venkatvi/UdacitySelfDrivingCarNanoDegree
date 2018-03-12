#ifndef DATAREADER_H
#define DATAREADER_H
#include "./DataAdapter.h" 
#include "../SensorFusionApplication.h"
#include <string> 
#include <memory> // unique_ptr
#include <iostream>
#include <fstream> // ofstream

/** DataReader class hierarchy is a set
* of polymorphic classes for reading data
* from different data sources - Simulator,
* data file
*/
namespace DataUtils{
 class DataReader{
  public:
   /** Constructor
   * @param pDataAdapter unique ptr to DataAdapter
   * @param pApplication unique ptr to SensorFusionApplication
   * Uses move semantics to initialize member variables
   * mDataAdapter_ and mSensorFusionApplication_
   */
   DataReader(std::unique_ptr<DataAdapter> pDataAdapter, 
    std::unique_ptr<SensorFusionApplication> pApplication): 
    mDataAdapter_(std::move(pDataAdapter)), 
    mApplication_(std::move(pApplication)){}

   /** Destructor
   * close mOutputDataStream_  
   */
   ~DataReader(){ 
    mOutputDataStream_.close();
   }

   /** setOutputFile sets the mOutputFileName_ 
   * member variable with file name 
   * It also initializes the mOutputDataStream_ 
   * with output data file so that output can be 
   * written into the output file.
   */
   void setOutputFile(const std::string& pFileName){
    mOutputFileName_ = pFileName;
    // if output data stream is already open,
    // close it
    if (mOutputDataStream_.is_open()){
     mOutputDataStream_.close();
    }
    // Re-open the data stream with output file 
    mOutputDataStream_.open(mOutputFileName_);
   }

   /** getDataAdapter getter member function 
   * returns a pointer to DataAdapter. 
   * The class still has the ownership of DataAdapter
   * member
   */
   DataAdapter* getDataAdapter() {
    return mDataAdapter_.get();
   }

   /** getApplication getter member function 
   * returns a pointer to SensorFusionApplication
   * member. The class still has the ownership of 
   * SensorFusionApplication. 
   */
   SensorFusionApplication* getApplication(){
    return mApplication_.get();
   }

   /** run method is a pure virtual member function
   * which has concrete definitions in the concerte
   * derived class implementations of this class.
   */
   virtual void run() = 0;

   
   /** processData process string pData, parses
   * measurements, ground truth information using 
   * DataAdapter member and then uses 
   * SensorFusionApplication member to compute 
   * KalmanFilter estimations
   * @param pData string containing the source information
   * returns pointer to const State which contains the 
   * estimated state
   */
   const State* processData(const std::string& pData);

   /** writeResultsToFile writes results to output file
   * stored in mOutputFileName_ member
   * @param pState pointer to const State containing 
   * estimates of Kalman Filter
   */
   void writeResultsToFile(const State* pState);

  protected:
   // output file name 
   std::string mOutputFileName_;

   // output file data stream
   std::ofstream mOutputDataStream_;

   // unique pointer to DataAdapter 
   std::unique_ptr<DataAdapter> mDataAdapter_;

   // unique pointer to SensorFusionApplication
   std::unique_ptr<SensorFusionApplication> mApplication_;
  
 };
}
#endif