
# Project 2 - Unscented Kalman Filter Project 
Self-Driving Car Engineer Nanodegree Program - Term 2

This github repository contains the source code, design documents and result files of Unscented Kalman Filter Project in the term 2 of SDC Nanodegree. 

## Installation
    git clone https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/tree/master/Term2/Proj2_UnscentedKalmanFilter.git
    
### Build Requirements    
For a successful build, the following installation requirements must be met:

-   cmake >= 3.5
    -   All OSes:  [click here for installation instructions](https://cmake.org/install/)
-   make >= 4.1 (Linux, Mac), 3.81 (Windows)
    -   Linux: make is installed by default on most Linux distros
    -   Mac:  [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
    -   Windows:  [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
-   gcc/g++ >= 5.4
    -   Linux: gcc / g++ is installed by default on most Linux distros
    -   Mac: same deal as make -  [install Xcode command line tools](https://developer.apple.com/xcode/features/)
    -   Windows: recommend using  [MinGW](http://www.mingw.org/)
  
## Build Instructions
  >- cd Proj2_UnscentedKalmanFilter
  >- cd build
  >- cmake ../
  >- make 
    

## Libraries and Source Code Dependencies
The project utilizes the Eigen Library for efficient matrix computations and uWebSocketIO for using the Simulator provided to run and visualize the Kalman Filter Estimates. Additionally, the project also uses a Jupyter notebook provided for adding visualizations on RMSE values obtained while tweaking different noise parameters in the Filter.

## Architectural Design 
A Sensor Fusion Application using an Unscented Kalman Filter under-the-hood that is capable of processing Radar and Lidar measurements is implemented using Object Oriented design techniques with adherence to SOLID principles.  The UML Class diagram and sequence diagrams  of the key workflows can be found under **Proj2_UnscentedKalmanFilter/README/Design** 

The project uses polymorphic class hierarchies and strategy hierarchies to encapsulate the Predict and Update steps of a Kalman Filter for lidar and radar measurements. 

## Implementation Details
The code is styled using [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Results
- RMSE values for Dataset 1 on running the UKF implementation yeilds results within expected thresholds < [0.09, 0.10, 0.40, 0.30]
- UKF is also run on Dataset 2 and results obtained are within the thresholds
-  Using Laser data only increases the RMSE a little more than the expected levels. However, the UKF estimates are still in a satisfactory trajectory when compared to the sensor fusion UKF results (baseline).
- Using Radar data only significantly changes the RMSE results (gets worse). This can be attributed to the linear approximation of the measurement function which in isolation does not give expected estimates. 
- Overall, the sensor fusion model works better then pure laser or radar Kalman Filters
- Additional experiments were done by modifying 
>- Q process covariance matrix values
>- R_laser measurement noise matrix for laser data
>- R_radar measurement noise matrix for radar data 

The results of all experiments are shared in a Jupyter notebook "**Unscented Kalman Filter - Results & Discussion.ipynb**" that can be found under **Proj2_UnscentedKalmanFilter/README/Results**

