#include "../DataUtils/DataAdapter.h"

#include <iostream>
#include <algorithm> //toupper

// Concrete derived measurement classes
#include "../Data/LaserMeasurement.h"
#include "../Data/RadarMeasurement.h"
// StateAdapterStrategyFactory class to provide
// with a polymorphic measurement function classes
// for each measurement class
#include "../Data/StateAdapterStrategyFactory.h"
// enum of types of data to be processed
#include "../DataUtils/DataTypes.h"
DataUtils::DataAdapter::DataAdapter(
  const std::string& pDataType) {
  // convert input param to upper case for
  // case-insensitive match
  auto upperCaseStr = pDataType;
  std::transform(upperCaseStr.begin(),
                 upperCaseStr.end(),
                 upperCaseStr.begin(),
                 ::toupper);

  if (upperCaseStr.compare("LASER") == 0) {
    m_data_type_ = DataUtils::SensorDataTypes::LASER;
  } else if (upperCaseStr.compare("RADAR") == 0) {
    m_data_type_ = DataUtils::SensorDataTypes::RADAR;
  } else if (upperCaseStr.compare("BOTH") == 0) {
    m_data_type_ = DataUtils::SensorDataTypes::BOTH;
  } else {
    m_data_type_ = DataUtils::SensorDataTypes::UNKNOWN;
  }
}
void DataUtils::DataAdapter::ParseData() {
  // set the stream to m_data_
  m_data_stream_.str(m_data_);
  ParseMeasurementData();
  ParseGroundTruthData();
}
void DataUtils::DataAdapter::ParseMeasurementData() {
  Data::Measurement* pMeasurementPtr;

  /* Each derived class of Measurement requires a
  * corresponding measurement function (h'(x)) to be
  * used in KalmanFilter update() stage.
  *
  * Also, each derived class of Measurement also requires
  * a polymorphic implementation for estimating the
  * "measurement" equivalent of given a state for computing
  * the error y = z - h(x) to be used in update() stage.
  *
  * An abstract factory pattern is used to construct a
  * parallel hierarchy of StateAdapterStrategies based on the
  * Measurement class hierarchy.
  *
  * Each Measurement class will also hold on to the
  * corresponding StateAdapterStrategy.
  *
  * The sensory type (laser / radar) is used by the
  * StateAdapterStrategyFactory class to generate the
  * polymorphic StateAdapterStrategy class.
  *
  * As seen in the Measurement hierarchy, StateAdapterStrategy
  * hierarchy is also easily extensible without having a need to
  * change the underlying implementation of Kalman Filter
  */

  // reads first element from the stream
  std::string sensorType;
  m_data_stream_ >> sensorType;

  // Create a StateAdapterStrategyFactory instance based on sensor type
  Data::StateAdapterStrategyFactory pStateAdapterStrategyFactory;
  pStateAdapterStrategyFactory.SetSensorType(sensorType);

  std: size_t measurementDimensions = 0;

  // if data is a laser measurement, and command line input
  // requires laser data to be included for Kalman Filter computation,
  // process data.
  if (sensorType.compare("L") == 0 && ProcessLaserData()) {
    // Laser measurement data is of the format:
    // "L px py timestamp"
    float pX;
    float pY;
    long long pTimestamp;

    // Read px, py and timestamp from m_data_
    m_data_stream_ >> pX;
    m_data_stream_ >> pY;
    m_data_stream_ >> pTimestamp;

    // Create a LaserMeasurement object pointer
    // using px, py and timestamp as constructor arguments
    pMeasurementPtr = new Data::LaserMeasurement(pTimestamp, pX, pY);

    // Use static method to get the number of input dimensions
    // for Laser Measurements. (This is used to avoid the use of
    // MAGIC NUMBERS in code)
    measurementDimensions = Data::LaserMeasurement::GetInputDimensions();

    // Get polymorphic StateAdapterStrategy from StateAdapterStrategyFactory
    // using the measurementDimensions. The dimensions are required to set
    // the dimensionality of covariance matrices in Strategy classes.

    // Set StateAdapterStrategy in Measurement pointer
    pMeasurementPtr->SetStateAdapterStrategy(
      pStateAdapterStrategyFactory.GetStateAdapterStrategy(
        measurementDimensions));

    // Create a unique instance of the Measurement pointer using move semantics
    std::unique_ptr<Data::Measurement> pMeasurementUPtr(pMeasurementPtr);
    m_measurement_ =  std::move(pMeasurementUPtr);
  } else if (sensorType.compare("R") == 0 && ProcessRadarData()) {
    // if data is a radar measurement, and command line input
    // requires radar data to be included for Kalman Filter computation,
    // process data.

    // Radar measurement is of the format:
    // "R rho theta rho_dot timestamp"

    // Read rho theta rho_dot timestamp from m_data_
    float pRho;
    float pTheta;
    float pRhoDot;
    m_data_stream_ >> pRho;
    m_data_stream_ >> pTheta;
    m_data_stream_ >> pRhoDot;

    long long pTimestamp;
    m_data_stream_ >> pTimestamp;

    // Create a RadarMeasurement object pointer
    // using rho, theta, rho_dot and timestamp as constructor
    // arguments
    pMeasurementPtr = new Data::RadarMeasurement(pTimestamp,
        pRho,
        pTheta,
        pRhoDot);

    // Get input dimensions of radar measurements using a
    // static member function
    measurementDimensions = Data::RadarMeasurement::GetInputDimensions();

    // Get polymorphic StateAdapterStrategy from StateAdapterStrategyFactory
    // using the measurementDimensions. The dimensions are required to set
    // the dimensionality of covariance matrices in Strategy classes.

    // Set StateAdapterStrategy in Measurement pointer
    pMeasurementPtr->SetStateAdapterStrategy(
      pStateAdapterStrategyFactory.GetStateAdapterStrategy(
        measurementDimensions));

    // Create a unique instance of the Measurement pointer using move semantics
    std::unique_ptr<Data::Measurement> pMeasurementUPtr(pMeasurementPtr);
    m_measurement_ =  std::move(pMeasurementUPtr);
  } else {
    // If a command line argument does not require reading either laser / radar data
    // Set measurement to NULL.
    m_measurement_.reset(nullptr);
  }
}
void DataUtils::DataAdapter::ParseGroundTruthData() {
// Read Ground Truth data only when Measurement is already
// computed.
  if (m_measurement_) {
    // Read gt_x, gt_y, gt_vx, gt_vy from m_data_stream_
    float pX;
    float pY;
    float vX;
    float vY;
    m_data_stream_ >> pX;
    m_data_stream_ >> pY;
    m_data_stream_ >> vX;
    m_data_stream_ >> vY;

    // Create a unique_ptr to State using move semantics
    // and store it as a member variable m_state_
    m_state_ = std::move(std::make_unique<Data::State>(pX, pY, vX, vY));
  }

}