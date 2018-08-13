#ifndef TOOLS_H_
#define TOOLS_H_

#include <cmath>
#include <math.h>

namespace DataUtils {
class Tools {
public:
  /**
  * Constructor.
  */
  Tools() {}

  /**
  * Destructor.
  */
  ~Tools() {}

  /**
  * A helper method to Normalize Angle.
  */
  double NormalizeAngle(double angle) {
    /*if (val > M_PI || val < -M_PI) {
      val = atan2(sin(val), cos(val));
    }
    return val;*/
    while (angle> M_PI) angle-=2.*M_PI;
    while (angle<-M_PI) angle+=2.*M_PI;
    return angle;
  }
};
}

#endif /* TOOLS_H_ */