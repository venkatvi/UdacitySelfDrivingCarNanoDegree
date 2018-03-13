#include "../Data/StateAdapterStrategyFactory.h"
Data::StateAdapterStrategy* Data::StateAdapterStrategyFactory::GetStateAdapterStrategy(
    const std::size_t pMeasurementDimensions) {
	if (m_sensor_type_.compare("L") == 0) {
		if (!m_laser_strategy_) {
			m_laser_strategy_ = new Data::LaserStateAdapterStrategy(pMeasurementDimensions);
		}
		return m_laser_strategy_;
	} else if (m_sensor_type_.compare("R") == 0) {
		if (!m_radar_strategy_) {
			m_radar_strategy_ = new Data::RadarStateAdapterStrategy(pMeasurementDimensions);
		}
		return m_radar_strategy_;
	} else {
		return nullptr;
	}
}
