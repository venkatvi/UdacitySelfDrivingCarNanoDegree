#ifndef DATATYPES_H
#define DATATYPES_H
namespace DataUtils {
/** SensorDataTypes enumeration lists the different types of data i.e. Laser / 
* Radar. This enum is used to selectively use only Laser or Radar data in
* Kalman Filter
*/
enum SensorDataTypes {
	LASER = 0,
	RADAR = 1,
	BOTH = 2,
	UNKNOWN = 3
};
}
#endif