#ifndef RADAR_MEASUREMENT_H
#define RADAR_MEASUREMENT_H
#include "./Measurement.h"
#include "./State.h"
/** RadarMeasurement class inherits from Measurement
* class. It overloads constructor to store rho, theta, 
* rho_dot and timestamp at which measurement was 
* recorded.
*/
class RadarMeasurement: public Measurement{
	public:
		/** Constructor
		* @param pTimestamp time at which measurement was recorded
		* @param pRho speed in the direction of movement
		* @param pTheta angle from x-axis at which vehicle is heading
		* @param pRhodot rate of change of speed 
		*/
		RadarMeasurement(long long pTimestamp, float pRho, float pTheta, float pRhoDot):
			Measurement(pTimestamp), 
			mRho_(pRho),
			mTheta_(pTheta),
			mRhoDot_(pRhoDot){}

		/** Destructor
		*/
		virtual ~RadarMeasurement(){};

  /** getStateData pure virtual function is overriden by 
  * concrete implementation
  * This function returns the measurement data transformed into 
  * State space
  */
		virtual State getStateData() const override;

  /** getInputDimensions is a static member function that returns 
  * the number of input dimensions in radar measurement
  */
		static std::size_t getInputDimensions(){ 
   return RadarMeasurement::mInputDimensions_;
  }

  /** getVectorizedData is a pure virtual function overriden by 
  * concrete implementation
  * This function transforms the measurement to Eigen::VectorXd 
  */
		virtual const Eigen::VectorXd getVectorizedData() const override{
			Eigen::VectorXd data = Eigen::VectorXd(RadarMeasurement::mInputDimensions_);
			data << mRho_, mTheta_, mRhoDot_;
			return data;
		}

  /** print is a pure virtual function overriden by RadarMeasurement
  * to output the values of member variables 
  */
		virtual void print(std::ostream& pStream) const override;

  /** getMeasurementType is a pure virtual function overriden by 
  * concrete implementation
  * This function returns a single character "R" representative of 
  * measurement type. The function is used while writing measurement
  * information to output file
  */
		virtual const std::string getMeasurementType() const override{
			return "R";
		}
	private:
		float mRho_;
		float mTheta_;
		float mRhoDot_;
		static constexpr std::size_t mInputDimensions_ = 3; 
};
#endif