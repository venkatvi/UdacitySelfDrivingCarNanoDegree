## Writeup ## Writeup 
---

**Advanced Lane Finding Project**

The goals / steps of this project are the following:

* Compute the camera calibration matrix and distortion coefficients given a set of chessboard images.
* Apply a distortion correction to raw images.
* Use color transforms, gradients, etc., to create a thresholded binary image.
* Apply a perspective transform to rectify binary image ("birds-eye view").
* Detect lane pixels and fit to find the lane boundary.
* Determine the curvature of the lane and vehicle position with respect to center.
* Warp the detected lane boundaries back onto the original image.
* Output visual display of the lane boundaries and numerical estimation of lane curvature and vehicle position.

[//]: # (Image References)

[image1]: ./examples/chessboard_corners.png "Chessboard Corners"
[image2]: ./examples/undistort_output.png "Undistorted"
[image3]: ./examples/test1.jpg "Sample Image"
[image4]: ./examples/test1_Undistorted.png "Undistorted image"
[image5]: ./examples/test1_ColorTransform.png "Color transform"
[image6]: ./examples/test1_Gradients.png "Sobel gradients "
[image7]: ./examples/test1_PerspectiveTransform.png "Perspective Transform"
[image8]: ./examples/test1_Histogram.png "Histogram"
[image9]: ./examples/test1_SlidingWindows.png "Sliding Windows"
[image10]: ./examples/test1_Output.png "Output"
[video1]: ./test_videos_output/project_video.mp4 "Video"

## [Rubric](https://review.udacity.com/#!/rubrics/571/view) Points
---
The writeup / README should include a statement and supporting figures / images that explain how each rubric item was addressed, and specifically where in the code each step was handled.

### Camera Calibration
#### 1. Briefly state how you computed the camera matrix and distortion coefficients. Provide an example of a distortion corrected calibration image.
The code for this step is located under "Camera Calibration" section of the jupyter notebook. 

CameraCaliberator is python class used to encapsulate the responsibilities of camera caliberation. The class has a "caliberate" method  which has three primary functions:

1. "Object Points" are computed using the number of corners input parameter. These object points (x,y,z) coordinates will be the chessboard corners of the real-world object such that these points remain the same for all images used for camera caliberation. i.e. Chessboard real world image is a fixed on (x,y) plane at z=0. 
2.  In order to caliberate camera, each image in the input folder is processed using cv2.findChessboardCorners method and object points computed (for a 2-D plane given 9x6 input corners) in order to get the 2-D image points. These corners are also drawn on the distorted image and shown. 

![alt text][image1]

3. Image points and object points for each image are then appended and stored. 
OpenCV function caliberateCamera is then used with inputs image points and object points to come up with a distortion matrix. 

4. Distortion matrix coefficients are then applied to a distorted image using cv2.undistort() function which is used by CameraCaliberator.undistort 
method to undistort an input image. Figure below shows the before and after undistortion images. 

![alt text][image2]

### Pipeline (test images)
The pipeline used for detecting lanes in an image consists of 6 major phases. The class used for the pipeline is LaneDetectionPipeline. execute() function of this class is used for images and process_image() function is used for image frames in videos. 

An outline of all the six phases are explained below with a sample test image "test_images/test1.jpg" 
![alt text][image3]

**Provide an example of a distortion-corrected image.**
1. **Undistortion** - Distortion correction that was calculated via camera calibration has been correctly applied to each image. CameraCaliberator class's undistort function is used for this transform. An example of a distortion corrected image can be found below
![alt text][image4]

**Describe how (and identify where in your code) you used color transforms, gradients or other methods to create a thresholded binary image. Provide an example of a binary image result.**
2. **Color Transform** - The undistorted image is then converted to HLS color space. The shadows present in the images (for example test4.jpg) often affect the upcoming transforms which hinder lane detection. Thus, the luminance channel is used to remove the existing values and replace it with a fixed value. A threshold is then applied to Saturation channel to get all pixels between values (100, 255) [higher the saturation value, the more closer the pixel is to the actual color which works in favor of removing shadows]. This module / transform is done using opencv APIs. The result of this transform on test1_undistorted.jpg is shown below. 
![alt text][image5]
3. **Gradient Transform** - The color transformed image is then put through a sobel operator to get the gradients. This transformation itself has sub-phases / pipelines
* In order to minimize of the noise, a mask is applied on the input image to focus on a trapezoidal area of reference to detect the lanes. 
For test images and project_video: [200, image_height], [450, 320], [480,320], [image_height, image_width] dimensions are used. Opencv functions bitwise_and is used for this step. 
* Absolute value of gradients in x and y orientation are obtained. These absolute values are thresholded between 20 and 100. cv2.Sobel function is used in this step. 
* These are further scaled and normalized
* The scaled gradients are used to compute the magnitude of the gradient. The magnitude is threshold between 1 and 4. 
* Direction of the gradient is also computed - The direction is thresholded between [0.4, 1]. Numpy's arctan2 function is used in this step. 
Output of this transform can be seen below:
![alt text][image6]

**Describe how (and identify where in your code) you performed a perspective transform and provide an example of a transformed image.**
4. **Perspective Transform**
Once the gradients are computed and cleaned up, an approximate eyeballing is done on the trapezoidal area of the lanes and mapped to a rectangular area in the warped image to get a perspective matrix. This is done via cv2.getPerspectiveTransform APIs 
* src coordinates : [200,720],[600,450],[1150,720],[700,450]
* dest coordinates:  [350,720],[350,0],  [1000,720],[1000,0]
An inverse perspective transform is also done to cache away the inverse matrix. 
Output of this transform can be seen below:
![alt text][image7]

**Describe how (and identify where in your code) you identified lane-line pixels and fit their positions with a polynomial?**
5. **Lane Detection**
Note: The algorithm required in this phase is defined via variable functions in LaneDetector class. 
Once warped image is obtained, sliding_windows technique as suggested in the lectures are applied on the lower half of the image to detect lane lines so that polynomial can be fit. In order to locate the approximate rectangular window area in order to search for lane related pixels, a histogram distribution is obtained to get bins with maximum number of pixels along the y dimension. 
A sample histogram of test1_PerspectiveTransform.png is shown below
![alt text][image8]
Clear peaks are shown around 400 and 850 pixels on x-dimension indicating lane peaks. Hence in these areas, sliding windows are set to start searching. A margin of 100 pixels is used to create a bounding box. 
As search is moved from bottom to top of the image, the sliding windows are moved to left or right around the center of maximum number of pixels. 

Finally, in each bounding box, all pixels are collected along with their x,y coordinates and polyFit API is used to fit a line across all the potential x,y values. 
**Provide an example image of your result plotted back down onto the road such that the lane area is identified clearly.**
The fit from the rectified image has been warped back onto the original image and plotted to identify the lane boundaries. This demonstrates that the lane boundaries were correctly identified. 
A sample image visualizing sliding windows and the lane lines detection are shown below:
![alt text][image9]

**Describe how (and identify where in your code) you calculated the radius of curvature of the lane and the position of the vehicle with respect to center.**
*After finding lanes, the radius of curvature is computed individually for left and right lane assuming road follows the curvature of the circle. Mean curvature of left and right lanes are then taken as output. The algorithm as suggested in the lectures use meters per pixel conversion factor.  
* For the position of the vehicle, the camera is mounted at the center of the car and the deviation of the midpoint of the lane from the center of the image is considered as the offset. This helps in estimating the vehicle is located with respect to the center of the lane. Using the lane positions, the offset of the lane from the mid-point of the image is computed. The offset computed in pixels is converted to meters. 
 
6. **Output generation**
With lane lines detected, the inverse perspective transform is used to map the lane lines in warped image to the original image. cv2.warpPrespective API is used for this step. 

The corresponding output is shown below:
![alt text][image10]

### Pipeline (video)
**Provide a link to your final video output.**
The pipleline described above for images is reused for videos. MoviePy package is used for disecting videos into frames (images).  Each image is fed to the traditional pipeline and the output image is annotated with lanes, radius of curvature and vehicle position. Annotated images are strung back together to get the output video. 

Output video is saved in test_videos_output folder. 
![alt text][video1]

### Discussion
Briefly discuss any problems / issues you faced in your implementation of this project. Where will your pipeline likely fail? What could you do to make it more robust?
This project had multiple challenges
* **Color transforms** For images, project video and challenge videos, the HLS color space thresholds had to be changed significantly. For example, the test images and project video had frames where shadows had to be removed. For challenge video, the lane lines had tar on them so were very hard to detect. 
* **Gradient transforms** This phase had a trapezoidal mask applied on images. The mask area is very different when project video and challenge video are concerned. This is potentially caused for driving up the car on different inclinations of the road or position of camera on cars of different dashboard heights. 
* **Perspective transforms** This is purely a hueristic based algorithm, where one needs to eyeball the image to come up with src and dest points used for constructing perspective matrix. Comparison across three videos given shows that src and dest points are significantly different.
* **Sliding Windows** This phase had a lot of challenges in terms of using the core algorithm across image frames. Main issues faced are 
1. Missing lanes in image frames - at times due to change in lighting, the pipeline parameters donot detect lanes. In these situations, the following solutions were tried
2. fits from the previous image frame are retained. However, these did not yeild good results. 
3. fits and plotys were cached away to be used. These gives the same x pizels as the previous image and hence does not work for frames where the real-world lanes are curving. 
4. Hence, the information obtained from sliding windows were used to recreate the x,y pixels where the potential lane lines  can be found. In order to do this and an "averaging effect" across frames, nonzero_x and y found in the sliding windows algorithm were stored across n frames such that the last k points are maintained. In this example project, k = 2000. 
5. In case of missing lanes (where one lane is found in sliding window algorithm while the other one is missing), the xy pixel values are used to recreate parallel lanes in the warped image. 

Despite all these changes, pipeline is sensitive to quick change of curvature in roads, different light conditions and curbs that are too close to lane situations. 

Researching on this topic for other research material shows that this still a fairly hard problem to solve for and there is active academic research in this area in two major dimensions
1. Computer vision techniques to get a live feedback on lanes
2. Statistical caliberation of steering wheel wrt to a relative point ahead - (another car in front or in side) to detect lanes. 

