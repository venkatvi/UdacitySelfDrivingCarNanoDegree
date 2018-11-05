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
For Steering Angle: 
- On choosing only P component, (by setting I, D to 0), the CTE increases in very few steps after simulation and the vehicle swerves around the center of the track. 
- Adding a small I component helps in reducing the swerving around the center as it accumulates the error over the last n steps 
- In order to avoid vehicle toppling over turns / curbs, an differential component was also added to make sure the vehicle is able to adjust the error created to the rate of change of steering angle value around turns. 

An initial value of P = 0.13, I = 0.0002, D = 3.0 was chosen (by trial and error approach). Twiddle algorithm was implemented to further tune parameters. 
The first 100 steps are considering "warm up" where the parameters are observed. Then the twiddle algorithm tunes the parameters once in every 500 steps. A default speed of 30 mph was used to observe the simulation implemented with PID controller for steering. 


In addition to the steering angle, PID controller was also implemented for throttle. The obtained throttle value was further capped at 75 mph. 

An initial value of P = 0.4, D = 0.2 was chosen for throttle PID. Intergator component is excluded as the cte with this cumulated error at higher speeds was making the vehicle move very erratically around the center of the track. A small value of D component helped in turning around the bends. However, a higher value of P component was required to ensure the proportional error due to higher speeds is penalized. 

### Final Parameters 
For steering angle, the final values stabilized around : P = 0.135, I = 0.00028, D = 3.05

For throttle, final values after using twiddle algorithm are: P = 0.7, D = 0.4

## Simulation 
The vehicle successfully laps around the track.

[Simulation Video with PID implemented for steering angle only](../Results/SteerPID_720p.mov)


Simulation with PID implemented for steering angle and throttle 
