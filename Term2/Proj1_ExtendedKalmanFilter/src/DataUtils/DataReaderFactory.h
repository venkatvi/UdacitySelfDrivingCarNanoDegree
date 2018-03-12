#ifndef DATAREADERFACTORY_H
#define DATAREADERFACTORY_H
#include "./DataReader.h" // Polymorphic base class 
#include "./WebClientReader.h" // Web-client based data reader 
#include "./FileReader.h" // file based data reader
#include "./DataAdapter.h"
#include "../SensorFusionApplication.h" 
namespace DataUtils{
	class DataReaderFactory{
		public:
			/**
		    * Constructor
		    */
			DataReaderFactory(){}

			/**
		    * Destructor
		    */
			~DataReaderFactory(){}

			/*** setSource initializes the source of data
			* in DataReaderFactory
			* @param pSource source of data
			*/
			void setSource(const std::string& pSource){
				mDataSource_ = pSource;
			}

			/*** getReader returns the polymorphic DataReader
			* pointer based on the type of source of data
			* @param pDataAdapterPtr DataAdapter Unique Pointer
			* @param pApplicationPtr SensorFusionApplication Unique Pointer
			*/
			DataReader* getDataReader(
				std::unique_ptr<DataAdapter> pDataAdapterPtr, 
				std::unique_ptr<SensorFusionApplication> pApplicationPtr) const;

		private:
			// source of data 
			std::string mDataSource_;
	};
}
#endif