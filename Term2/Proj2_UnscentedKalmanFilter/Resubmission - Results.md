# Resubmission - Results
The last submission had wrong values of std_a and std_yawdd in the code which led to wrong values of output. This was a submission mistake. As one can observe, all through the README files and results, the correct values chosen for the project are discussed and illustrated via plots and figures. 

## Corrected files
UnscentedKalmanFilter.cpp has been modified to reflect on the right values of std_a = 1.5 and std_yawdd = 0.5 
With these the following are RMSE values for different inputs

### Simulator on Dataset1 using both lidar and radar data 
RMSE values are:
>- pX: 0.0690825
>- pY: 0.0825685
>- vX: 0.337184
>- vY: 0.219523

### Simulator on Dataset1 using only lidar data 
RMSE values are: 
>- pX: 0.0994159
>- pY: 0.0946825
>- vX: 0.614447
>- vY: 0.257404

As seen here, using only the lidar data violates the threshold requirements of velocity in direction x. this can be especially seen in cases when the angle of curvature information during turns is not available in lidar data. 

### Simulator on Datset1 using only radar data 
RMSE values are: 
>- pX: 0.163132
>- pY: 0.255279
>- vX: 0.382128
>- vY: 0.850205
As seen here, the angle of turn is improved by reducing the RMSE for vX. However, due to unavailability of lidar for accurate distance prediction of nearby points, the position values RMSE have worsened. 

### Text file on Dataset 1 using radar and lidar data 
RMSE values are: 
>- pX: 0.0690415
>- pY: 0.0824913
>- vX: 0.33699
>- vY: 0.219355


## Other code changes as per review comments
>- Const variable initializations were implemented in RadarStateAdapterStrategy.cpp
>- P_aug matrix has been intialized in constructor in order to save the reinitialization. 

 