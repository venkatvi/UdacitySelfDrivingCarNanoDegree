#ifndef LASER_MEASUREMENT_H
#define LASER_MEASUREMENT_H
#include "./Measurement.h"
#include "./State.h"
#include <iostream>
/** LaserMeasurement class stores 
* laser sensor information
*/
class LaserMeasurement: public Measurement{
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
			mPositionX_(pPositionX),
			mPositionY_(pPositionY){}

  /** Destructor
  */
		virtual ~LaserMeasurement(){}

  /** getStateData is a virtual member function
  * which returns a predicted state given the laser 
  * measurements. For laser, State approximation 
  * is given by [px, py, 0, 0]
  */
		virtual State getStateData() const override; 

  /* getInputDimensions is a static member function 
  * returning the total number of dimensions 
  * of a laser dimension 
  */
		static std::size_t getInputDimensions(){ 
   return LaserMeasurement::mInputDimensions_;
  }

  /** getVectorizedData is a virtual member function 
  * which returns the measurement as a Eigen::VectorXd
  */
		virtual const Eigen::VectorXd getVectorizedData() const override{
			Eigen::VectorXd data = 
       Eigen::VectorXd(LaserMeasurement::mInputDimensions_);
			data << mPositionX_, mPositionY_;
			return data;
		}

  /** getmeasurementType is a virtual member function which
  * returns the measurement type as a character. This function
  * is used while writing measurement data into output file
  */
		virtual const std::string getMeasurementType() const override{
			return "L";
		}

	protected:
  /** print is a virual member function which outputs the
  * value of member variables to output data stream
  */
		virtual void print(std::ostream& pStream) const override;

	private:
  // Position X
		float mPositionX_;

  // Position Y
		float mPositionY_;

  // static member variable which returns 2 
  // for input dimensions
		static constexpr std::size_t mInputDimensions_ = 2;
};
#endif