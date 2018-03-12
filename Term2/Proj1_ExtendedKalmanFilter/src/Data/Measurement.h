#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_
#include "./State.h"
#include "./StateAdapterStrategy.h"
#include <iostream>
/** Measurement class provides a base class  
* framework for polymorphic implementation 
* of laser and radar measurements 
*/
class Measurement{
	public:
  /** Destructor 
  */
		virtual ~Measurement() {};

  /** getTimestamp member function 
  * returns mTimestamp_  
  */
		long long getTimestamp() const {
			return mTimestamp_;
		}

  /** getStateData is a pure virtual member
  * function implemented by derived classes
  */ 
		virtual State getStateData() const = 0;

  /** setStateAdapterStrategy member function sets the 
  * StateAdapterStrategy for the given measurement 
  * @param pStrategy pointer to StateAdapterStrategy
  * The StateAdapterStrategy polymorphic hierarchy 
  * is used in an abstract factory pattern with Measurement
  * class hierarchy
  */
		void setStateAdapterStrategy(
           StateAdapterStrategy* pStrategy){
			mStateAdapterStrategy_ = pStrategy;
		}

  /** getStateAdapterStrategy member function returns the 
  * member mStateAdapterStrategy_ as a non-const pointer
  * This method is used by the application to further modify
  * measurement covariance matrices*/
		StateAdapterStrategy* getStateAdapterStrategy() {
			return mStateAdapterStrategy_;
		}

  /** getVectorizedData is a pure virtual member function
  * implemented by concrete derived classes 
  */
		virtual const Eigen::VectorXd getVectorizedData() const = 0;

  /** getMeasurementType is a pure virtual member function 
  * implemented by concrete derived classes 
  */
		virtual const std::string getMeasurementType() const = 0;
	
 protected:
  /** Constructor
  * protecteed constructor
  */
		Measurement(long long pTimestamp):mTimestamp_(pTimestamp){}

  /** print method is a pure virtual function 
  * that is used to print the details of measurement
  * class to the ostream
  * @param pStream ostream used to print the details of 
  * measurement class
  */
		virtual void print(std::ostream& pStream) const = 0;
		
  /** operator << overload as a friend function to 
  * direct output to ostream 
  */
		friend std::ostream &operator<<(std::ostream& pStream, 
                      const Measurement& pMeasurement){
			pMeasurement.print(pStream);
			return pStream;
		}
		
	private:
		long long mTimestamp_;
		StateAdapterStrategy* mStateAdapterStrategy_;

};
#endif 