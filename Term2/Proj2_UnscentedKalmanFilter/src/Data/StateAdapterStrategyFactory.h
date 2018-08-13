#ifndef STATEADAPTERSTRATEGYFACTORY_H
#define STATEADAPTERSTRATEGYFACTORY_H
#include "../Data/StateAdapterStrategy.h"
#include "../Data/RadarStateAdapterStrategy.h"
#include "../Data/LaserStateAdapterStrategy.h"
/** StateAdapterStrategyFactory class provides a factory class
* for choosing a concrete implementation of StateAdapterStrategy
*/
namespace Data {
class StateAdapterStrategyFactory {
public:
  StateAdapterStrategyFactory():
    m_laser_strategy_(nullptr),
    m_radar_strategy_(nullptr) {}

  ~StateAdapterStrategyFactory() {}

  void SetSensorType(const std::string& pSensorType) {
    m_sensor_type_ = pSensorType;
  }

  /** getStateAdapterStrategy function is a factory method which chooses the 
  * concrete StateAdapterStrategy instance based on the sensor type.
  * @param pMeasurementDims dimensions of measurement
  * The input dimensions is required to initialize covariance matrices of 
  * concrete instances.
  */
  Data::StateAdapterStrategy* GetStateAdapterStrategy(
    const std::size_t pMeasurementDimensions);
private:
  std::string m_sensor_type_;
  Data::StateAdapterStrategy* m_laser_strategy_;
  Data::StateAdapterStrategy* m_radar_strategy_;
};
}
#endif