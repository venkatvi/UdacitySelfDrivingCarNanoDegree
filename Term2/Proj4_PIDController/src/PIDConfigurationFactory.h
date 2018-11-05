#ifndef PID_CONFIGURATION_FACTORY_H
#define PID_CONFIGURATION_FACTORY_H
#include "PIDConfigurationType.h"
class PIDConfigurationFactory {
public:
	PIDConfiguration GetConfig(PIDConfigurationType& pType){
		PIDConfiguration rPIDConfig(0,0,0); 
		switch(pType){
			case (PIDConfigurationType::STEER):
			{
				PIDConfiguration sPIDConfig(-0.134611, -0.000270, -3.0534);
				rPIDConfig = sPIDConfig;
				break;
			}
			case (PIDConfigurationType::THROTTLE):
			{
				PIDConfiguration tPIDConfig( -0.7, 0.0,-0.4);
				rPIDConfig = tPIDConfig;
				break;
			}
		}
		return rPIDConfig;
	}
};
#endif
