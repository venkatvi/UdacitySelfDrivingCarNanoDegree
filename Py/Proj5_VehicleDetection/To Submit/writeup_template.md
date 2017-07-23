## Writeup 
---

**Vehicle Detection Project**

The goals / steps of this project are the following:

* Perform a Histogram of Oriented Gradients (HOG) feature extraction on a labeled training set of images and train a classifier Linear SVM classifier
* Optionally, you can also apply a color transform and append binned color features, as well as histograms of color, to your HOG feature vector. 
* Note: for those first two steps don't forget to normalize your features and randomize a selection for training and testing.
* Implement a sliding-window technique and use your trained classifier to search for vehicles in images.
* Run your pipeline on a video stream (start with the test_video.mp4 and later implement on full project_video.mp4) and create a heat map of recurring detections frame by frame to reject outliers and follow detected vehicles.
* Estimate a bounding box for vehicles detected.

[//]: # (Image References)
[image1]: ./examples/car_and_noncar.png "Car and Non-Car"
[image2]: ./examples/HOG_example_1.png "HOG features"
[image3]: ./examples/sliding_windows.png "Sliding Windows"
[image4]: ./examples/test6_candidate_windows.png "Candidate Windows"
[image5]: ./examples/bboxes_and_heat.png "Bounding boxes and heatmap"
[image6]: ./examples/output.png "Output"
[video1]: ./project_video.mp4

## [Rubric](https://review.udacity.com/#!/rubrics/513/view) Points
---
### Histogram of Oriented Gradients (HOG)

**Explain how (and identify where in your code) you extracted HOG features from the training images.**

In order to get a SVM classifier trained with car and non-car images, a collection of images from Car Database is used. All images are loaded and features are extracted from each image. The implementation uses **DataLoader** class which loads all images in a given set of folders. 

The dataset consists of distinct vehicle vs non-vehicle images. A sample car and non-car image looks like one below:

![alt text][image1]

For each image, a feature extraction pipeline was built in order to extract feature spaces to be used for classifier. This pipeline is implemented using **FeatureExtractor** class. This class implements ths following steps in order to extract features.

1. **Color space transformation**: This is implemented via applyColorSpace method which converts the image to any specific color space. I explored multiple colorspaces (YCrCb, HSV, HSL), however, (RGB) worked best for my algorithm.
2. **Rescale image**: rescaleImage() method scales images to desired scale as images in differnt categories of the dataset are of different sizes. 
3. **Spatial feature extraction**: extractSpatialFeatures() method resizes the input image to (32,32) size. All features of the image are then used as a 1-D vector
4. **Histogram feature extraction**: extractHistogramFeatures() method extracts histogram of all values in each individual channel. Then all the histogram bin values across all channels are combined together. 
5. **HOG feature extraction**: extractHOGFeatures() method extracts histogram of gradients for each channel in the 3 dimensional color space of an image. When visualize parameter of skimage.hog() API is set to true, it additionally returns an image visualizing gradients. 
The parameters used for this API are:
**9 orientations**
**(8,8) pixels per cell**
**(2,2) cells per block**

A sample of this feature space is illustrated by the figure below

![alt text][image2]

6. A collection of all these features are put together and output 

**Explain how you settled on your final choice of HOG parameters.**
I tried different parameter choices for HOG feature
1. orientations - [9, 8]
2. cells per block - (4, 4)
3. pixels per cell - (16, 16)
However, the classification accuracy was best when I resolved to the configurations suggested in the lectures

**Describe how (and identify where in your code) you trained a classifier using your selected HOG features (and color features if you used them).**
In order to train the classifier, the featureExtractorPipeline is run through each image in the dataset. After all features are collected for all cars and non-cars, a dataset of 19168 samples with 8364 feature dimensions were collected. The algorithm is described under "Extract Features" section. 
---
### Learning via SVMs
The algorithm used in this step is described under "Train SVM" section. It had two main phases:
0. **Data Augmentation** - As a pre-preprocessing step, all the images in vehicles and non-vehicles category were flipped and the original dataset of ~8000 images were doubled. 
1. **Preprocessing** - Data is normalized across each dimension using sklearn.preprocessing.StandardScaler APIs. 
2. **Data Split** - sklearn.cross_validation.train_test_split is used to split the normalized data into an 80-20 split 
3. **Training the Classifier** - Training data split is used in sklearn.svm.linearSVC. I tried other classifiers (rbf kernerls), however, this algorithm not only took way longer than linear svm but also yeilded poor classification results on the test set. 
4. **Testing accuracy** - 98.53% was obtained on the 20% data which was segregated as test set. 

**Cache away classifier**
In order to save time from re-training classifier from the dataset, the classifier, the scaled dataset and original dataset are cached away as a pickle file "CarSVC.p"
---
### Sliding Window Search
**Describe how (and identify where in your code) you implemented a sliding window search.  How did you decide what scales to search and how much to overlap windows?**
The core algorithm used for detecting vehicles again has a master pipeline which includes the feature extractor pipeline. This is described by **SlidingWindowsPipeline** class. This class implements the following steps:
1. **Detect vehicle windows** - detect_vehicle() API is used to detect all candidate windows where vehicles are likely to be present. This API has a set of parameters which is used to create sliding windows in an image
* Window Sizes - square windows of size 96px and 128px are used. Other alternative configurations considered are 36px, 48px and 64px. 
* XYOverlaps - 0.5, 0.8 overlaps are tried to determine the overlap between any two consecutive windows. 
* y_start_stop - parameter is restricted to [400, 600] pixels of the image. 
* x_start_stop - parameter is also restricted to the right side of the image in order to not detect vehicles on the opposite lane 

A series of sliding windows are created with above mentioned **window sizes** and **xy overlaps** combinations. 

A sample image with annotated sliding windows is shown below:
![alt text][image3]

For each window, the corresponding segment of the image is extracted, features are extracted using **FeatureExtractorPipeline**. 
Once the features are extracted, a new test point is created using these features and used with the classifier trained earlier. If the classifier predicts that the window has a car, window is added to the candidate windows list. 
For each of the candidate windows, bounding boxes are drawn around the different sized windows which qualified as candidates. 

A sample image with candidate windows is shown below:
![alt text][image4]

2. **Create heatmap** - Create a heatmap indicating the number of overlapping windows
For all candidate windows, heat map is created where all the pixels within the candidate window are kept a score of 1 per window. Hence if a pixel is a part of multiple windows (for example 5 windows), the pixel value in the heat map would be 5. 
3. **Find bounding box** - Create labelling technique to find the bounding box
For identified regions with certain threshold of pixel values, a bounding box is created. 

An example of heatmaps and bounding boxes for the test images are shown in the figure below:
![alt text][image5]

**Show some examples of test images to demonstrate how your pipeline is working.  What did you do to optimize the performance of your classifier?**

A sample output of the entire pipeline is shown below 
![alt text][image6]

---
### Video Implementation

**Provide a link to your final video output.  Your pipeline should perform reasonably well on the entire project video (somewhat wobbly or unstable bounding boxes are ok as long as you are identifying the vehicles most of the time with minimal false positives.)**
Here's a [link to my video result](./project_video.mp4)


**Describe how (and identify where in your code) you implemented some kind of filter for false positives and some method for combining overlapping bounding boxes.**

I recorded the positions of positive detections in each frame of the video.  From the positive detections I created a heatmap and then thresholded that map to identify vehicle positions.  I then used `scipy.ndimage.measurements.label()` to identify individual blobs in the heatmap.  I then assumed each blob corresponded to a vehicle.  I constructed bounding boxes to cover the area of each blob detected.  

Here's an example result showing the heatmap from a series of frames of video, the result of `scipy.ndimage.measurements.label()` and the bounding boxes then overlaid on the last frame of video:

### Here are six frames and their corresponding heatmaps:

### Here is the output of `scipy.ndimage.measurements.label()` on the integrated heatmap from all six frames:
![alt text][image7]

### Here the resulting bounding boxes are drawn onto the last frame in the series:
![alt text][image8]



---

### Discussion
**Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?**
