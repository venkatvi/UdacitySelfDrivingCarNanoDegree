#include "../DataUtils/DataReaderFactory.h"
DataUtils::DataReader* DataUtils::DataReaderFactory::GetDataReader(
    std::unique_ptr<DataAdapter> pDataAdapterPtr,
    std::unique_ptr<SensorFusionApplication> pApplicationPtr) const
{
	DataUtils::DataReader* pDataReader;
	// if source of the data is from "Simulator"
	// initialize a WebClientReader
	if (m_data_source_.compare("Simulator") == 0) {
		pDataReader =
		    new DataUtils::WebClientReader(
		    std::move(pDataAdapterPtr),
		    std::move(pApplicationPtr));
	} else {
		// source is a file and initialize a FileReader
		auto pFileReader =
		    new DataUtils::FileReader(
		    std::move(pDataAdapterPtr),
		    std::move(pApplicationPtr));

		// Set input file source in FileReader
		pFileReader->SetSource(m_data_source_);
		pDataReader = pFileReader;
	}
	return pDataReader;
}