# Project 4 - PID Controller
## Compilation
- Code compiles using cmake 3.12 and make 4.2.1. 
- The CMakeLists.txt has been modified to add new source files 

## Run commands
- The program can be run by executing the following command:
> ./pid

Twiddle algorithm can be enabled for PID controller for steer and throttle by using the command line arguments:
>./pid -s <error tolerance for steer PID controller> -t <error tolerance for throttle PID controller>
e.g. ./pid -s 0.0000001 -t 0.00000001 

## Implementation 
The PID procedure implementated for steering angle and throttle follow the lectures. In addition to base algorithm implementing PID controller, the implementation also includes a twiddler which can tune in parameters for every 200 steps. 

## Reflection 
### Effects of P, I, D component on implementation 

### Final Parameters 


## Simulation 
The vehicle successfully laps around the track.
