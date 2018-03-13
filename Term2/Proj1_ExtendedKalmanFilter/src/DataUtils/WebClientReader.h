#ifndef JSONDATAREADER_H
#define JSONDATAREADER_H
#include "json.hpp"
#include "uWS/uWS.h"
#include "DataUtils/DataReader.h"
/** WebClientReader class inherits from DataReader
* and implements methods for reading data from
* WebSocket
*/
namespace DataUtils {
class WebClientReader: public DataReader {
public:
	/** Constructor
	* @param pDataAdapter unique ptr to DataAdapter
	* @param pApplication unique ptr to SensorFusionApplication
	* Passes the input parameters to Base Class
	*/
	WebClientReader(std::unique_ptr<DataAdapter> pDataAdapter,
	                std::unique_ptr<SensorFusionApplication> pApplication):
		DataReader(std::move(pDataAdapter),
		           std::move(pApplication)) {}

	/** Destructor
	*/
	~WebClientReader() {
		std::cout << "Calling Reader destructor" << std::endl;
	}

	/** run method implements the logic of reading data from
	* web socket, parsing information and computing KalmanFilter
	* states */
	virtual void Run();

private:
	// Web Server
	uWS::Hub mServerHub_;
};
}
#endif