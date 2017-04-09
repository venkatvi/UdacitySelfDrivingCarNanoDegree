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
*Lane Detection Pipeline* consisted of the following steps. 
1. Conversion of image to grayscale - this was done using grayscale helper function
2. Computation of gaussian blur of the grayscale image  - this was done using gaussian_blue helper function. I used a kernel size of 5. 
3. Canny edge detection - With a threshold range of [40, 120], canny edges in the image were detected - this was done again using canny helper function which used opencv libraries to detect canny edges. 
4. Creating a mask for the region of interest where the lanes were meant to be detected. 
I first used a hard-coded set of vertices as the corner coordinates of the rectangular region to create a mask. 
[0, height_of_image], [480, 310], [490, 310], [width_of_image, height_of_image]
These hard-coded coordinates to find lanes in the masked area worked well for the first two problems. The challenge problem’s image size was different from the other two problems and hence the hard-coded vertices did not work. 
So, I modified the vertices to 
[0, height_of_image-20], width_of_image/2 - 10, height_of_image/2 + 40], [width_of_image/2 + 10, height_of_image/2 + 40], [width_of_image, height_of_image -20]
The offset of 20 px from height_of image for the bottom vertices was required to remove noisy lines formed at the boundary of the vehicle. 
In effect, a rectangular area starting approximately at the midpoint of the image, spanning gradually over the image width was appropriate for creating a mask. 
5. Create hough_lines in the region of interest - this was done using hough_lines helper function. In order to connect edges to form lane lines, min_line_length of 100 and max_line_gap was 150 was required to identify lines for the broken white lined lanes. 
This function also called draw_lines functions which connected the line segments by marking a red line between them. With default implementation of draw_lines, for each line_segment detected using hough_transform, a line was drawn. 
I used an averaging technique to come up with an average slope of all the line segments identified of positive slope (indicating a left lane) and negative slope (indicating a right lane) respectively. Using the average positive (negative) slope , i extrapolated a line from top left vertex (top-right vertex) to bottom left vertex (bottom right vertex) to form the left lane (right lane). Post drawing the line, hough_lines returned an image with lines drawn on a mask. 
6. The last step i the pipeline was to overlay the mask with lines on the original image to get the lines drawn on the lanes - this was done using weighted_img function. 



In order to draw a single line on the left and right lanes, I modified the draw_lines() function by ...

If you'd like to include images to show how the pipeline works, here is how to include an image: 

![alt text][image1]


### 2. Identify potential shortcomings with your current pipeline


One potential shortcoming would be what would happen when ... 

Another shortcoming could be ...


### 3. Suggest possible improvements to your pipeline

A possible improvement would be to ...

Another potential improvement could be to ...
