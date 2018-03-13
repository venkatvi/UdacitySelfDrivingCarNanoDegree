#ifndef LASER_MEASUREMENT_H
#define LASER_MEASUREMENT_H
#include <iostream>

#include "../Data/Measurement.h"
#include "../Data/State.h"
/** LaserMeasurement class stores laser sensor information
*/
namespace Data{
class LaserMeasurement: public Measurement {
public:
  /** Constructor
  * @param pTimestamp timestamp at which measurement was recorded
  * @param pPositionX position x
  * @param pPositionY position y
  * Sets the member variables with input arguments
  */
  LaserMeasurement(long long pTimestamp,
                   float pPositionX,
                   float pPositionY):
    Measurement(pTimestamp),
    m_position_x_(pPositionX),
    m_position_y_(pPositionY) {}
  virtual ~LaserMeasurement() {}

  /** getStateData is a virtual member function which returns a predicted state
  * given the laser measurements. For laser, State approximation is given by
  * [px, py, 0, 0]
  */
  virtual State GetStateData() const override {
    State pState(m_position_x_, m_position_y_, 0, 0);
    return pState;
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
  virtual const Eigen::VectorXd GetVectorizedData() const override {
    Eigen::VectorXd data =
      Eigen::VectorXd(LaserMeasurement::m_input_dimensions_);
    data << m_position_x_, m_position_y_;
    return data;
  }

  /** getmeasurementType is a virtual member function which returns the
  * measurement type as a character. This function is used while writing
  * measurement data into output file.
  */
  virtual const std::string GetMeasurementType() const override {
    return "L";
  }

protected:
  /** print is a virual member function which outputs the value of member
  * variables to output data stream
  */
  virtual void Print(std::ostream& pStream) const override {
    pStream << m_position_x_ << ", ";
    pStream << m_position_y_ << ", ";
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