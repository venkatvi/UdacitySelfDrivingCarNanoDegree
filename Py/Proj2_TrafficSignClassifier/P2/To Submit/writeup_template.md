#**Traffic Sign Recognition** 

##Writeup Template

###You can use this file as a template for your writeup if you want to submit it as a markdown file, but feel free to use some other method and submit a pdf if you prefer.

---

**Build a Traffic Sign Recognition Project**

The goals / steps of this project are the following:
* Load the data set (see below for links to the project data set)
* Explore, summarize and visualize the data set
* Design, train and test a model architecture
* Use the model to make predictions on new images
* Analyze the softmax probabilities of the new images
* Summarize the results with a written report


[//]: # (Image References)

[image1]: ./examples/visualization.jpg "Visualization"
[image2]: ./examples/grayscale.jpg "Grayscaling"
[image3]: ./examples/random_noise.jpg "Random Noise"
[image4]: ./examples/placeholder.png "Traffic Sign 1"
[image5]: ./examples/placeholder.png "Traffic Sign 2"
[image6]: ./examples/placeholder.png "Traffic Sign 3"
[image7]: ./examples/placeholder.png "Traffic Sign 4"
[image8]: ./examples/placeholder.png "Traffic Sign 5"

## Rubric Points
###Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/481/view) individually and describe how I addressed each point in my implementation.  

---
###Writeup / README

####1. Provide a Writeup / README that includes all the rubric points and how you addressed each one. You can submit your writeup as markdown or pdf. You can use this template as a guide for writing the report. The submission includes the project code.

You're reading it! and please find attached along with this submission jupyter notebook Traffic_Sign_Classifier.ipynb which contains the entire code. 

###Data Set Summary & Exploration

####1. Provide a basic summary of the data set. 
I used the numpy library to calculate summary statistics of the traffic
signs data set:

* The size of training set is 34799
* The size of the validation set is 4410 
* The size of test set is 12630
* The shape of a traffic sign image is (32,32,3)
* The number of unique classes/labels in the data set is 43

####2. Include an exploratory visualization of the dataset.

Here is an exploratory visualization of the data set. The image below presents the sample distrubtion for the 43 classes across training, test and validation datasets

![Sample Distribution]({{site.baseurl}}//SampleDistribution.png)

Further, an exploration on which traffic signals have minimum sample vs maximum sample yeilded the following results:

* There were three classes namely, 'Speed limit (20km/h)', 'Dangerous curve to the left', 'Go straight or left' which had a sample size of 270 images only
* 'Speed limit (50km/h)' is the sign class with maximum representation of nearly 3000 images in all 

Further analyzing the distributional difference across sign classes, there seem to be three broad buckets of sample distribution, high, medium and low sample representation of sign classes 

* Near about 10 classes had a high sample representation around 2500 to 3000 samples per class
* Next set of 9 classes had a medium sample representation with 1500 to 2000 samples per class
* The lower order sign classes each had under 1000 samples per class and almost had a uniform distribution with minimum being 200 samples and maximum of 1000 samples per sign


###Design and Test a Model Architecture

####1. Describe how you preprocessed the image data. What techniques were chosen and why did you choose these techniques? Consider including images showing the output of each preprocessing technique. Pre-processing refers to techniques such as converting to grayscale, normalization, etc. (OPTIONAL: As described in the "Stand Out Suggestions" part of the rubric, if you generated additional data for training, describe why you decided to generate additional data, how you generated the data, and provide example images of the additional data. Then describe the characteristics of the augmented training set like number of images in the set, number of images for each class, etc.)

#####1. Training-Validation split 
As observed from the initial statistics of the given data, the training data forms around 89% of the (training + validation) mixture which could lead to overfitting while training the neural network. Further, the sample distribution of classes in training and validation datasets require to  similar for us to guarantee a high validation accuracy. 

This was achieved by first concatenating all the training and validation datasets, shuffling them 
and creating an 80-20% split. 

As a result, the modified datasets were
* Number of training samples : 31468
* Number of validation samples: 7841

#####2. Grayscale conversion
The entire model was training originally with (32,32,3) images without any dropout layer first. This had a very high validation accuracy of 97% and test accuracy of 89%. However, when the network was used on images downloaded from the web, the classification accuracy was as low as 50% 

Further investigating in to the sample images available in training, validation and test data and comparing them against the images downloaded from the web, it was found that the images in the given dataset were not as sharp as the downloaded images and were also subject to different lighting (sunlight vs night etc). 

Also, since the images were not cropped around their ROI (region of interest triangles as provided by the web dataset), some of the background colors were also attributing to the noise. 

In order to remove the bias, the images were converted to gray scale which helps in learning the gradients of shapes of signs by the CNN.  

Here is an example of a traffic sign image before and after grayscaling.
![Before and After Gray Scaling]({{site.baseurl}}//GrayScale.png)

3. Data Augmentation

In order to remove the biases due to lighting and augment fuzzy data images, histogram equalization technique was used. 

Here is an example of an original image and an augmented image:

![Histogram Equalized Images]({{site.baseurl}}//HistogramEqualization.png)


####2. Describe what your final model architecture looks like including model type, layers, layer sizes, connectivity, etc.) Consider including a diagram and/or table describing the final model.

My final model consisted of the following layers:

| Layer         		|     Description	        					| 
|:---------------------:|:---------------------------------------------:| 
| Input         		| 32x32x1 Gray scale image
| Convolution 5x5     	| 1x1 stride, VALID padding, outputs 28x28x6 	|
| RELU					|												|
| Max pooling	      	| 2x2 stride,  outputs 14x14x6   				|
| Convolution 5x5	    | 1x1 stride, VALID padding, outputs 10x10x16   |
| RELU					|												|
| Max pooling	      	| 2x2 stride,  outputs 5x5x16   				|
| Flatten       		| outputs 400x1     							|
| Fully Connected       | outputs 120x1                                 |
| RELU                  |                                               |
| Fully Connected       | outputs 84x1                                  |
| RELU                  |                                               |
| Dropout               | keep_probability = 0.9                        |
| Fully Connected       | outputs 43x1                                  |
| Softmax				| outputs 43x1 normalized                       |

####3. Describe how you trained your model. The discussion can include the type of optimizer, the batch size, number of epochs and any hyperparameters such as learning rate.

To train the model, I used the following model parameters

Adam Optimizer with a learning rate (eta) = 0.001 was used to minimize the loss_operation which compares the predictions of model with that of y values / labels from the dataset. 

In order to avoid overfitting, a keep_probability of 0.9 was used in dropout layer. 

The training data was used in 10 epochs and during each epoch mini-batches of 128 samples were used to tune to optimizer's hyperparameters. 

The y labels were also converted to one_hot encoding in order to be used for softmax_cross_entropy_with_logits function to get the entropy of classification

Accuracy function is the mean of all correct predictions when the validation / test set is used on the trained model. 


####4. Describe the approach taken for finding a solution and getting the validation set accuracy to be at least 0.93. Include in the discussion the results on the training, validation and test sets and where in the code these were calculated. Your approach may have been an iterative process, in which case, outline the steps you took to get to the final solution and why you chose those steps. Perhaps your solution involved an already well known implementation or architecture. In this case, discuss why you think the architecture is suitable for the current problem.

At the end of ten epochs of training, 
* training set accuracy = 97.26%
* validation set accuracy = 94.52%
* test set accuracy of  = 86.84%

If an iterative approach was chosen:
* What was the first architecture that was tried and why was it chosen?
The initial architecture used 32x32x3 images.
Both with and without dropout layers were experimented on the training and validation datasets
The dataset was not reshuffled / augmented. 
Smaller learning rate of 0.00001 was also tried

* What were some problems with the initial architecture?
32x32x3 images without changing the given sample distribution for training and validation sets resulted in very high validation accuracy 98.6% while the test accuracy was very poor ~ 48%
Addition of dropout layer worsened the performance of the model on the validation set 
Smaller learning rate also worserned the validation accuracy as only 10 epochs were used to train. 

* How was the architecture adjusted and why was it adjusted? Typical adjustments could include choosing a different model architecture, adding or taking away layers (pooling, dropout, convolution, etc), using an activation function or changing the activation function. One common justification for adjusting an architecture would be due to overfitting or underfitting. A high accuracy on the training set but low accuracy on the validation set indicates over fitting; a low accuracy on both sets indicates under fitting.
* Which parameters were tuned? How were they adjusted and why?
* What are some of the important design choices and why were they chosen? For example, why might a convolution layer work well with this problem? How might a dropout layer help with creating a successful model?
In order to boost the testing accuracy, couple of changes were introduced
1. Gray scaling images and sharpening them using histogram equalization - this changed the model architecture to use 32x32x1 images instead of 32x32x3 feature spaces. 
2. Reshuffling of training and validation datasets to redrawn samples from the shuffle dataset.  This change caused the test accuracy to go up to 80% from 48%
3. Introduction of a small dropout layer to avoid bias / overfitting in the training phas. Although inclusion of dropout layer caused a drop in training and validation accuracy, it improved test accuracy to 86%
4. Larger learning rate of 0.001 - this enhanced the training and validation accuracy under 10 epochs 


If a well known architecture was chosen:
* What architecture was chosen?
LetNet architecture was used with minor modifications. 

* Why did you believe it would be relevant to the traffic sign application?
LeNet is a popular convolutional neural network model used in the domain of image classification. Using this with traffic signs was experimental in understanding if the network can be retrained with a new dataset to perform a classification job.

* How does the final model's accuracy on the training, validation and test set provide evidence that the model is working well?
A high training and validation accuracy along with a reasonably high testing accuracy proves that the LeNet has performed well with an entirely new dataset. 

###Test a Model on New Images

####1. Choose five German traffic signs found on the web and provide them in the report. For each image, discuss what quality or qualities might be difficult to classify.

Here are five German traffic signs that I found on the web:

![Speed 20kmh][Class0_00006_00028] ![Stop Sign][Class14_00014_00020] ![No Vehicles][Class15_00016_00027] 
![Road Work][Class25_00038_00009] ![Pedestrians][Class27_00007_00020] ![End of no passing by vehicles over 3.5 metric tons] [Class42_00007_00028]


First two images have the signs presented with a small rotation (of about 5-10 degrees). Specifically, sign mapped to Speed 20km/h represents the sign with minimum number of sample data. The other four images have very little clarity on the signs due to the size of the image (small). Hence, these images were chosen to see if the network will do a good job classifying them.

####2. Discuss the model's predictions on these new traffic signs and compare the results to predicting on the test set. At a minimum, discuss what the predictions were, the accuracy on these new predictions, and compare the accuracy to the accuracy on the test set (OPTIONAL: Discuss the results in more detail as described in the "Stand Out Suggestions" part of the rubric).

Here are the results of the prediction:

| Image			        |     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| Speed 20km/h     		| Speed 20km/h   								| 
| Stop Sign     		| Stop Sign										|
| No Vehicles			| No Vehicles									|
| Road Work      		| Road Work 					 				|
| Pedestrians			| Road narrows on the right       				|
| End of no passing		| End of no passing   							|


The model was able to correctly guess all 5 of the 5 traffic signs, which gives an accuracy of 83.3%. This compares favorably to the accuracy on the test set of 86%. All preprocessing steps applied to training, validation and test sets were also applied to new images. 


####3. Describe how certain the model is when predicting on each of the five new images by looking at the softmax probabilities for each prediction. Provide the top 5 softmax probabilities for each image along with the sign type of each probability. (OPTIONAL: as described in the "Stand Out Suggestions" part of the rubric, visualizations can also be provided such as bar charts)

The code for making predictions on my final model is located under "Predict the sign for each test image and analyze performance" section. 

The top six probabilities and classes of predictions for the test images can be found below:

TopKV2(values=array([[  1.00000000e+00,   6.32379782e-12,   4.62945984e-15,
          2.84407660e-15,   1.26943398e-15,   8.31134404e-17],
       [  1.00000000e+00,   7.95384266e-13,   6.04641662e-14,
          2.18792845e-15,   2.01380029e-15,   3.14437185e-16],
       [  9.99185264e-01,   7.76304572e-04,   3.27605630e-05,
          1.57451632e-06,   1.32207140e-06,   1.14814918e-06],
       [  9.97973263e-01,   1.68242468e-03,   1.43554178e-04,
          7.74274085e-05,   7.29794410e-05,   3.19579740e-05],
       [  9.08886433e-01,   8.73697102e-02,   3.59696266e-03,
          1.36190778e-04,   6.89168701e-06,   2.09370546e-06],
       [  9.99776542e-01,   2.22432034e-04,   5.61477634e-07,
          3.82995296e-07,   3.09572883e-08,   3.22151017e-09]], dtype=float32), indices=array([[ 0,  1,  4, 38, 40,  8],
       [14, 12, 38, 37, 33,  1],
       [15, 38, 34,  3, 10, 39],
       [25, 22, 38, 26, 28, 31],
       [24, 27, 26, 28, 30, 29],
       [42, 10,  6, 41, 12,  9]]))

As seen from the prediction, the top probability is pretty high compared to the second best or third best prediction when softmax layer is applied to leNet layer for all six images. 

For the fifth image related pedestrians, the prediction was "Roads narrow towards right". The top six probabilties were 

| Image			        |     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
|9.09E-01				|Roads narrows on the right						|
|8.74E-02				|Pedestrians									|
|3.60E-03				|Traffic Signals								|
|1.36E-04				|Children crossing								|
|6.89E-06				|Beware of ice/snow								|
|2.09E-06				|Bicycles crossing								|


### (Optional) Visualizing the Neural Network (See Step 4 of the Ipython notebook for more details)
####1. Discuss the visual output of your trained network's feature maps. What characteristics did the neural network use to make classifications?
As a test, two signs were used the 20 km/h sign (which is a circular sign) and pedestrians sign (triangular sign). 

Using the helper function to output the 6 feature maps of the first convolutional layer, shows that, the feature spaces are tried to learn the circular and triangular shape of the sign respectively. 
The second layer of 16 feature maps, are trying to abstract the geometrical representations (circle / triangle in the respective case) into more fundamental representations like set of contiguous darker / brighter pixels. 

