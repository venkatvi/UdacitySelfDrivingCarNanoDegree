# **Finding Lane Lines on the Road** 

## Writeup Template

---

**Finding Lane Lines on the Road**

The goals / steps of this project are the following:
* Make a pipeline that finds lane lines on the road
* Reflect on the work done in a written report


[//]: # (Image References)

[image1]: ./examples/grayscale.jpg "Grayscale"

---

### Reflection

### 1. Describe your pipeline. As part of the description, explain how you modified the draw_lines() function.

The video (used for test) are segmented in to images and for each image the “Lane detection pipeline” was applied. 
**Lane Detection Pipeline** consisted of the following steps. 
1. __Conversion of image to grayscale__ - this was done using grayscale helper function
2. __Computation of gaussian blur of the grayscale image__  - this was done using gaussian_blue helper function. I used a kernel size of 5. 
3. __Canny edge detection__ - With a threshold range of [40, 120], canny edges in the image were detected - this was done again using canny helper function which used opencv libraries to detect canny edges. 
4. __Creating a mask for the region of interest where the lanes were meant to be detected__ 
Initially a hard-coded set of vertices as the corner coordinates of the rectangular region were used to create a mask. 
`[0, height_of_image], [480, 310], [490, 310], [width_of_image, height_of_image]`
These hard-coded coordinates to find lanes in the masked area worked well for the first two problems. The challenge problem’s image size was different from the other two problems and hence the hard-coded vertices did not work. 
So, the vertices were modified to 
`[0, height_of_image-20], width_of_image/2 - 10, height_of_image/2 + 40], [width_of_image/2 + 10, height_of_image/2 + 40], [width_of_image, height_of_image -20]`
The offset of 20 px from height_of image for the bottom vertices was required to remove noisy lines formed at the boundary of the vehicle. 
In effect, a rectangular area starting approximately at the midpoint of the image, spanning gradually over the image width was appropriate for creating a mask. 
5. __Create hough_lines in the region of interest__ - this was done using hough_lines helper function. In order to connect edges to form lane lines, min_line_length of 100 and max_line_gap was 150 was required to identify lines for the broken white lined lanes. 
This function also called draw_lines functions which connected the line segments by marking a red line between them. Post drawing the lines, hough_lines returned an image with lines drawn on a mask. 
6. __Generate Output__ - The last step in the pipeline was to overlay the mask with lines on the original image to get the lines drawn on the lanes - this was done using weighted_img function. 
The generated output is available in ./test_videos_output folder. 

In order to draw a single line on the left and right lanes, draw_lines() function was modified to maintain information about the following
1. Number of frames used
2. Average negative slope seen across image frames
3. Average positive slope seen across image frames

The left and right lane lines form line segments with positive slope and negative slope respectively. In order to come up with a single line to draw on the lane, the following steps were used:
1. __Removal of noisy slopes__: In order to use the right candidate for computing the average slopes, i removed all slopes where `abs(slope) < 0.5`
2. __Averaging of over all positive (negative) slopes__: For each image frame, lane lines were to be drawn. In order to draw the lane lines with a consistent slope across frames, the slope of candidate lines were averaged over and the information was maintained globally. 
3. __Drawing lane lines__: Using the average positive (negative) slope , a line was extrapolated from top left vertex (top-right vertex) to bottom left vertex (bottom right vertex) to form the left lane (right lane respectively).


### 2. Identify potential shortcomings with your current pipeline
1. __Fixed Rectangular area for mask__: Current pipeline has a fundamental assumption that the rectangular area required to detect lines is fixed. This will not work well on curvy roads where the vertices can be changed to avoid noisy hough lines. 
2. __Not immune to shadows or road color changes__: While detecting edges, (especially with challenge.mp4), the pipeline detects edges marked by shadows of the trees and changes in color of road etc. These cause additional noise in detecting lanes
3. __Average slopes for curvy lanes__: Maintaining one global average positive and negative slope does not work for curvy roads as the curvature of the road needs to be represented by multiple smaller line-segments. 

### 3. Suggest possible improvements to your pipeline

1. __Handle Road Color Changes__: To handle varying road colors, a preprocessing step can be designed to transform the image such that the road color corresponds to a darker color. This can be done by identifying the color that is present in abundance and transform that color to a darker shade. 
2. __Handle curvy lanes __: In case of curved roads, maintaining multiple piece-wise slopes can help in detecting the curved lanes. Additional machine learning algorithms (like k-means clustering) can also be used to cluster the observed slopes in to multiple clusters in order to form the piece-wise slopes
3. __Varying rectangular mask area__: Based on the nature of the slopes, a second pass through the process_image routine to handle slopes in  a specific cluster can be adopted. In the second pass, the area of image used for hough_transforms can be modified to just fit the lines in the cluster in order  to piece-wise lines. 


