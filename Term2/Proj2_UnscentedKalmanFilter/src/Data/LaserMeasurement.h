#ifndef LASER_MEASUREMENT_H
#define LASER_MEASUREMENT_H
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

#include "../Data/Measurement.h"
#include "../Data/State.h"
/** LaserMeasurement class stores laser sensor information
*/
namespace Data {
class LaserMeasurement: public Measurement {
public:
  /** Constructor
  * @param pTimestamp timestamp at which measurement was recorded
  * @param pPositionX position x
  * @param pPositionY position y
  * Sets the member variables with input arguments
  */
  LaserMeasurement(std::unique_ptr<StateAdapterStrategy> pStrategy,
                   long long pTimestamp,
                   float pPositionX,
                   float pPositionY):
    Measurement(std::move(pStrategy), pTimestamp),
    m_position_x_(pPositionX),
    m_position_y_(pPositionY) {}
  virtual ~LaserMeasurement() {}

  /** getStateData is a virtual member function which returns a predicted state
  * given the laser measurements. For laser, State approximation is given by
  * [px, py, 0, 0]
  */
  State GetStateData() const override {
    auto psi = 0;
    /*auto EPS = std::numeric_limits<double>::epsilon();
    if (fabs(m_position_x_) >  EPS && fabs(m_position_y_) > EPS){
      psi = atan2(m_position_y_, m_position_x_);
    } else {
      psi = 0;
    }*/
    State pState(m_position_x_, m_position_y_, 0, psi, 0);
    return pState;
  }
  Eigen::MatrixXd GetInitStateCovariance() const override{
    Eigen::MatrixXd P = Eigen::MatrixXd(5, 5);

    P << 1, 0, 0, 0, 0,
    0, 1, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 0, 1, 0,
    0, 0, 0, 0, 1;
    
    return P;
  }
  /* getInputDimensions is a static member function returning the total number
  * of dimensions of a laser measurement
  */
  static std::size_t GetInputDimensions() {
    return LaserMeasurement::m_input_dimensions_;
  }

  /** getVectorizedData is a virtual member function which returns the
  * measurement as a Eigen::VectorXd
  */
  const Eigen::VectorXd GetVectorizedData() const override {
    Eigen::VectorXd data =
      Eigen::VectorXd(LaserMeasurement::m_input_dimensions_);
    data << m_position_x_, m_position_y_;
    return data;
  }

  /** getmeasurementType is a virtual member function which returns the
  * measurement type as a character. This function is used while writing
  * measurement data into output file.
  */
  const std::string GetMeasurementType() const override {
    return "L";
  }

  /** print is a virual member function which outputs the value of member
  * variables to output data stream
  */
  void Print(std::ostream& pStream) const override {
    pStream << m_position_x_ << "\t";
    pStream << m_position_y_ << "\t";
  }

private:
  // Position X
  float m_position_x_;

  // Position Y
  float m_position_y_;

  // static member variable which returns 2
  // for input dimensions
  static constexpr std::size_t m_input_dimensions_ = 2;
};
}
#endif