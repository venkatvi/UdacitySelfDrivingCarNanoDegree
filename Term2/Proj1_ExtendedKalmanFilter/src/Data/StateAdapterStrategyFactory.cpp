#include "./StateAdapterStrategyFactory.h"
/** getStateAdapterStrategy function is a factory method
* which chooses the concrete StateAdapterStrategy instance
* based on the sensor type.
* @param pMeasurementDims dimensions of measurement
* The input dimensions is required to initialize covariance matrices
* of concrete instances.
*/ 
StateAdapterStrategy* StateAdapterStrategyFactory::getStateAdapterStrategy(
          const std::size_t pMeasurementDimensions){
 // if sensor type is laser
	if (mSensorType_.compare("L") == 0){
		if(!laserStrategy){
   // create LaserStateAdapterStrategy instance
			laserStrategy = new LaserStateAdapterStrategy(pMeasurementDimensions);
		}
		return laserStrategy;
	}else if(mSensorType_.compare("R") == 0){
  // if sensor type is radar
		if (!radarStrategy){
   // create RadarStateAdapterStrategy instance
			radarStrategy = new RadarStateAdapterStrategy(pMeasurementDimensions);
		}
		return radarStrategy;
	}else{
		return nullptr;
	}
}
