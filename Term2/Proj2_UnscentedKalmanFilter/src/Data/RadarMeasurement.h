#ifndef RADAR_MEASUREMENT_H
#define RADAR_MEASUREMENT_H
#include "../Data/Measurement.h"
#include "../Data/State.h"
#include "../DataUtils/Tools.h"
/** RadarMeasurement class inherits from Measurement class. It overloads
* constructor to store rho, theta, rho_dot and timestamp at which measurement
* was recorded.
*/
namespace Data {
class RadarMeasurement: public Measurement {
public:
	/** Constructor
	* @param pTimestamp time at which measurement was recorded
	* @param pRho speed in the direction of movement
	* @param pTheta angle from x-axis at which vehicle is heading
	* @param pRhodot rate of change of speed
	*/
	RadarMeasurement(std::unique_ptr<StateAdapterStrategy> pStrategy,
	                 long long pTimestamp,
	                 float pRho,
	                 float pTheta,
	                 float pRhoDot):
		Measurement(std::move(pStrategy), pTimestamp),
		m_rho_(pRho),
		m_theta_(pTheta),
		m_rho_dot_(pRhoDot) {}

	virtual ~RadarMeasurement() {};

	/** getStateData pure virtual function is overriden by
	* concrete implementation
	* This function returns the measurement data transformed into
	* State space
	*/
	State GetStateData() const override {
		float x = cos(m_theta_) * m_rho_;
		float y = sin(m_theta_) * m_rho_;
		float v = m_rho_dot_;

		auto psi = 0;
		/*auto EPS = std::numeric_limits<double>::epsilon();
		if (fabs(x) >  EPS && fabs(y) > EPS) {
			psi = atan2(y, x);
		} else {
			psi = 0;
		}
		DataUtils::Tools tools;
		psi = tools.NormalizeAngle(psi);
		*/
		
		float psi_dot = 0;
		State pState(x, y, v, psi, psi_dot);
		return pState;
	}

	/** getInputDimensions is a static member function that returns
	* the number of input dimensions in radar measurement
	*/
	static std::size_t GetInputDimensions() {
		return Data::RadarMeasurement::m_inputDimensions_;
	}

	/** getVectorizedData is a pure virtual function overriden by
	* concrete implementation
	* This function transforms the measurement to Eigen::VectorXd
	*/
	const Eigen::VectorXd GetVectorizedData() const override {
		Eigen::VectorXd data = Eigen::VectorXd(RadarMeasurement::m_inputDimensions_);
		data << m_rho_, m_theta_, m_rho_dot_;
		return data;
	}

	/** print is a pure virtual function overriden by RadarMeasurement
	* to output the values of member variables
	*/
	void Print(std::ostream& pStream) const  override {
		auto pState = GetStateData();
		pStream << pState.GetPositionX() << "\t";
		pStream << pState.GetPositionY() << "\t";
	}

	/** getMeasurementType is a pure virtual function overriden by
	* concrete implementation
	* This function returns a single character "R" representative of
	* measurement type. The function is used while writing measurement
	* information to output file
	*/
	const std::string GetMeasurementType() const override {
		return "R";
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
private:
	// distance
	float m_rho_;
	// angle of direction
	float m_theta_;
	//speed
	float m_rho_dot_;
	static constexpr std::size_t m_inputDimensions_ = 3;
};
}
#endif