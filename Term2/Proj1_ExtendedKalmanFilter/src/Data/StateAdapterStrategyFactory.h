#ifndef STATEADAPTERSTRATEGYFACTORY_H
#define STATEADAPTERSTRATEGYFACTORY_H
#include "./StateAdapterStrategy.h"
#include "./RadarStateAdapterStrategy.h"
#include "./LaserStateAdapterStrategy.h"
/** StateAdapterStrategyFactory class provides a factory class
* for choosing a concrete implementation of StateAdapterStrategy
*/
class StateAdapterStrategyFactory{
	public:
  /** Constructor
  */
		StateAdapterStrategyFactory():
      laserStrategy(nullptr), 
      radarStrategy(nullptr){}

  /** Destructor
  */
		~StateAdapterStrategyFactory(){}
		
  /** setSensorType function sets the sensor type 
  */
  void setSensorType(const std::string& pSensorType){
   mSensorType_ = pSensorType;
  }

  /** getStateAdapterStrategy function is a factory method
  * which chooses the concrete StateAdapterStrategy instance
  * based on the sensor type.
  * @param pMeasurementDims dimensions of measurement
  * The input dimensions is required to initialize covariance matrices
  * of concrete instances.
  */ 
		StateAdapterStrategy* getStateAdapterStrategy(
            const std::size_t pMeasurementDimensions);
	private:
  // sensor type 
		std::string mSensorType_;

  // pointer to StateAdapterStrategy for returning LaserStateAdapterStrategy
		StateAdapterStrategy* laserStrategy;

  // pointer to StateAdapterStrategy for returning RadarStateAdapterStrategy
		StateAdapterStrategy* radarStrategy;

};

#endif