#include "./FileReader.h"
#include <iostream>
#include <fstream>
/** run method  uses mFileReader_ to read data from
* mFileName_ . It process data using processData() 
* method. When a predicted state is available, it uses 
* SensorFusionApplication to calculateRMSE 
* It finally writes the results to output file.
*/
void DataUtils::FileReader::run(){
 // Read data
 mFileReader_.open(mFileName_, std::ifstream::in);

 std::string line;
 while(!mFileReader_.eof()){
  //read line 
  line.clear();
  getline(mFileReader_, line);

  // if line starts with "L" or "R"
  if(line[0] == 'L' || line[0] == 'R'){
   // process data 
   auto predictedState = processData(line);

   // If a predicted state is available, calculate
   //RMSE
   if(predictedState){
    auto RMSE = mApplication_->calculateRMSE();

    //Output: "L/R"  'px_est','py_est','vx_est','vy_est',
    // 'px_meas','py_meas',
    // 'px_gt','py_gt','vx_gt','vy_gt'
    writeResultsToFile(predictedState);
    
    // Output RMSE in commmand line  
    std::cout << "RMSE: " << RMSE << std::endl;
   }
  }
 }
 // close the file
 mFileReader_.close();
}
