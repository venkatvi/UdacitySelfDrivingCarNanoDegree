# Project Rubric
## Compilation
- Code compiles using cmake 3.12 and make 4.2.1. 
- The CMakeLists.txt has been modified to add new source files 
## Accuracy
- The program can be run in two ways:
>- Using text input file
`./UnscentedKF -s ../data/obj_pose-laser-radar-synthetic-input.txt -o output.txt -t both`
>- Using simulator
`./UnscentedKF -s "Simulator" -o output.txt -t both`
This additionally requires the simulator to be open and run
- The RMSE values using text input file (or Dataset 1) are 

>- pX - 0.0690825
>- pY - 0.0825685
>- vX - 0.337184
>- vY - 0.219523

These values meets the threshold specifications of [0.09, .10, .40, 0.30] respectively. 
Also, these values are less than the ones observed while using an Extended Kalman Filter implementation in the first project of this term where the values were: 

>- X - 0.107213
>- Y - 0.0953379
>- VX - 0.476465
>- VY - 0.48935

## Algorithm Accuracy
-  Sensor Fusion algorithm implemented follows the general processing flow as taught in the lessons - implementing, the initialize, predict and update steps
- Kalman Filter algorithm handles the first measurements appropriately. This is used to update the initial state vector, covariance matrices and timestamp
- Kalman Filter algorithm first predicts then updates.
- Kalman Filter algorithm handles both radar and laser measurements.
>-  Further the implementation is also parameterized to use only laser / radar. This can be achieved by using the following command
>`./ExtendedKF -s "Simulator" -o output.txt -t <data type>` 
> where `<data type>` = "laser" / "radar" / "both"

## Code Efficiency
- In order to avoid unnecessary branching, a polymorphic class hierarchy has been used to implement the Kalman Filter thereby making it easily extensible in the future for new sensor types 
- All values once calculated are stored in respective classes and exposed via const member function getters.
- Control flow checks are minimally used
- Smart pointers are used in order to avoid multiple redundant resource allocation 