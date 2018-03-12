/* Abstract Factory to get polymorphic DataReader based on the input source type*/
#include "./DataUtils/DataReaderFactory.h" 

/*//Parser to parse command line input choices of
source of data - simulator / file, output file name and 
data type (laser/ radar/ both)*/
#include "./DataUtils/InputParser.h"  


#include <memory> //std::unique_ptr

// Typedef unique ptrs 
typedef std::unique_ptr<DataUtils::DataAdapter> DataAdapterUPtr;
typedef std::unique_ptr<SensorFusionApplication> SensorFusionUPtr;
typedef std::unique_ptr<DataUtils::DataReader> DataReaderUPtr;

int main(int argc, char** argv) 
{   
 // Default values for input source file and 
 // output data file
 std::string source = "./input.txt";   
 std::string output = "./output.txt";
 
 // Initialize parser with default source file 
 // and output file locations
 DataUtils::InputParser pParser(source, output);

 // Call parseArgs with command line arguments
 pParser.parseArgs(argc, argv); 

 // When parsing of command line arguments is 
 // successful
 if (pParser.isParseSuccessful()){

   // Create a DataAdapter unique pointer
   DataAdapterUPtr pDataAdapterPtr(
     new DataUtils::DataAdapter(
           pParser.getDataType()));

   // Create a SensorFusionApplication unique pointer
   SensorFusionUPtr pApplicationPtr(
     new SensorFusionApplication());

   // Initialize a DataReaderFactory with the source of 
   // data from the command line arguments     
   DataUtils::DataReaderFactory pDataReaderFactory;
   pDataReaderFactory.setSource(source);     

   // Get the polymorphic instance of DataReader based 
   // on the data source
   auto pDataReader =
   pDataReaderFactory.getDataReader(
               std::move(pDataAdapterPtr),
               std::move(pApplicationPtr));     

   // Create a Unique Pointer to DataReader 
   DataReaderUPtr pDataReaderPtr(pDataReader);

   // Set output file location in DataReader
   pDataReaderPtr->setOutputFile(
             std::string(pParser.getOutput()));

   // Run the DataReader to get data for running 
   // the SensorFusion Application
   pDataReaderPtr->run();   
 } 
}