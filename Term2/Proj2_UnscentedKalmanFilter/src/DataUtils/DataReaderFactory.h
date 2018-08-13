#ifndef DATAREADERFACTORY_H
#define DATAREADERFACTORY_H
#include "../DataUtils/DataReader.h" // Polymorphic base class 
#include "../DataUtils/WebClientReader.h" // Web-client based data reader 
#include "../DataUtils/FileReader.h" // file based data reader
#include "../DataUtils/DataAdapter.h"
#include "../SensorFusionApplication.h"
namespace DataUtils {
class DataReaderFactory {
public:
	DataReaderFactory() {}
	~DataReaderFactory() {}

	void SetSource(const std::string& pSource) {
		m_data_source_ = pSource;
	}

	/*** getReader returns the polymorphic DataReader pointer based on the type
	* of source of data
	* @param pDataAdapterPtr DataAdapter Unique Pointer
	* @param pApplicationPtr SensorFusionApplication Unique Pointer
	*/
	DataReader* GetDataReader(
	    std::unique_ptr<DataAdapter> pDataAdapterPtr,
	    std::unique_ptr<SensorFusionApplication> pApplicationPtr) const;

private:
	// source of data
	std::string m_data_source_;
};
}
#endif