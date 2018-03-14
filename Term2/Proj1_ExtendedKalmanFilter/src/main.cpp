#include <memory> //std::unique_ptr

#include "DataUtils/DataReaderFactory.h"
#include "DataUtils/InputParser.h"

// Typedef unique ptrs
typedef std::unique_ptr<DataUtils::DataAdapter> DataAdapterUPtr;
typedef std::unique_ptr<SensorFusionApplication> SensorFusionUPtr;
typedef std::unique_ptr<DataUtils::DataReader> DataReaderUPtr;

int main(int argc, char** argv)
{
  std::string source = "./input.txt";
  std::string output = "./output.txt";

  // Initialize parser with default source file
  // and output file locations
  DataUtils::InputParser pParser(source, output);

  // Call parseArgs with command line arguments
  pParser.ParseArgs(argc, argv);

  if (pParser.IsParseSuccessful()) {
    // Create a DataAdapter unique pointer using 
    // command line inputs for data types to be parsed
    DataAdapterUPtr pDataAdapterPtr(
      new DataUtils::DataAdapter(
        pParser.GetDataType()));

    SensorFusionUPtr pApplicationPtr(
      new SensorFusionApplication());

    // Initialize a DataReaderFactory with the source of data 
    DataUtils::DataReaderFactory pDataReaderFactory;
    pDataReaderFactory.SetSource(pParser.GetSource());

    // Get the polymorphic instance of DataReader based on the data source
    auto pDataReader =
      pDataReaderFactory.GetDataReader(
        std::move(pDataAdapterPtr),
        std::move(pApplicationPtr));

    // Create a Unique Pointer to DataReader
    DataReaderUPtr pDataReaderPtr(pDataReader);

    // Set output file location in DataReader
    pDataReaderPtr->SetOutputFile(
      std::string(pParser.GetOutput()));

    pDataReaderPtr->Run();
  }
}