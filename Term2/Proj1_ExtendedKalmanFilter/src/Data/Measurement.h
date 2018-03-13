#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_
#include <iostream>
#include <memory>

#include "../Data/State.h"
#include "../Data/StateAdapterStrategy.h"
/** Measurement class provides a base class framework for polymorphic
* implementation of laser and radar measurements
*/
namespace Data {
class Measurement {
public:
  virtual ~Measurement() {};

  long long GetTimestamp() const {
    return m_timestamp_;
  }

  virtual State GetStateData() const = 0;

  /** setStateAdapterStrategy member function sets the StateAdapterStrategy for
  * the given measurement
  * @param pStrategy pointer to StateAdapterStrategy
  * The StateAdapterStrategy polymorphic hierarchy is used in an abstract factory 
  * pattern with Measurement class hierarchy
  */
  void SetStateAdapterStrategy(
    StateAdapterStrategy* pStrategy) {
    m_state_adapter_strategy_->reset(pStrategy);
  }

  /** getStateAdapterStrategy member function returns the member
  * m_state_adapter_strategy_ as a non-const pointer
  * This method is used by the application to further modify
  * measurement covariance matrices. The ownership of the member is still 
  * retained by Measurement class.
  */
  StateAdapterStrategy* GetStateAdapterStrategy() {
    return m_state_adapter_strategy_->get();
  }

  /** getVectorizedData is a pure virtual member function
  * implemented by concrete derived classes
  */
  virtual const Eigen::VectorXd GetVectorizedData() const = 0;

  /** getMeasurementType is a pure virtual member function
  * implemented by concrete derived classes
  */
  virtual const std::string GetMeasurementType() const = 0;

protected:
  /** Constructor
  * protected constructor
  */
  Measurement(long long pTimestamp): m_timestamp_(pTimestamp) {}

  /** print method is a pure virtual function
  * that is used to print the details of measurement
  * class to the ostream
  * @param pStream ostream used to print the details of
  * measurement class
  */
  virtual void Print(std::ostream& pStream) const = 0;

  /** operator << overload as a friend function to
  * direct output to ostream
  */
  friend std::ostream &operator<<(std::ostream& pStream,
                                  const Measurement& pMeasurement) {
    pMeasurement.Print(pStream);
    return pStream;
  }

private:
  long long m_timestamp_;
  std::unique_ptr<StateAdapterStrategy>* m_state_adapter_strategy_;

};
}
#endif