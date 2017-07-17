#**Traffic Sign Recognition**
**Build a Traffic Sign Recognition Project**
The goals / steps of this project are the following:
* Load the data set (see below for links to the project data set)
* Explore, summarize and visualize the data set
* Design, train and test a model architecture
* Use the model to make predictions on new images
* Analyze the softmax probabilities of the new images
* Summarize the results with a written report
---
## Rubric Points
##### Please find below the answers to the [rubric points](https://review.udacity.com/#!/rubrics/481/view) individually and a description of how each point is addressed in the implementation
---
### Files Submitted
* Jupyter notebook (Traffic_sign_classifier.ipynb)
* HTML page of implementation (Traffic_sign_classifier.html)
* project_writeup.md (This file)
* test_images (Folder that contains test images from the web along with metadata information)
* images_for_writeup (Folder that contains the images required in this write up documents)

* As per instructions the following files have **not** been included in the submission
* _pickle_ file which includes the training, validation and test datasets (originally present in the zip file)
* _signnames.csv_ which contains the names of signs vs class ids. 


* All images referenced in writeup are linked with the relative path to the github repository where these files can also be found
* Link to github directory: https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/tree/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit
---
### Dataset Exploration

#### Dataset Summary
I used the numpy library to calculate summary statistics of the traffic signs data set:
* The size of training set is 34799
* The size of the validation set is 4410
* The size of test set is 12630
** Total number of images = 51839
** Percentage of train/valid/test datasets are therefore 67.12% - 8.5% - 24.4% split
* The shape of a traffic sign image is (32,32,3)
* The number of unique classes/labels in the data set is 43

#### Exploratory Visualization
**_Sample Distribution_**

The image below presents the sample distribution for the 43 classes across training, test and validation datasets.

![Sample Distribution](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/SampleDistribution.png)


**_Class Representation Analysis_**
Further, an exploration on which traffic signal classes have minimum images vs maximum images yielded the following results:
* There were three classes namely, 'Speed limit (20km/h)', 'Dangerous curve to the left', 'Go straight or left' which had a sample size of 270 images only
* 'Speed limit (50km/h)' is the sign class with maximum representation of nearly 3000 images in all
* Mean number of images across all classes = 1206
Further analyzing the distributional difference across sign classes, there seem to be three broad buckets of sample distribution, high, medium and low images / samples representation of sign classes
* Near about 10 classes had a high sample representation around 2500 to 3000 samples per class
* Next set of 9 classes had a medium sample representation with 1500 to 2000 samples per class
* The lower order sign classes each had under 1000 samples per class and almost had a uniform distribution with minimum being 200 samples and maximum of 1000 samples per sign

![Number of images per class](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/SamplesPerClass.png)


**_Mean Representative Analysis_**
In order to understand how the different representative images of a traffic sign map to the class label, a simple experiment of adding all images of a specific class and then taking the mean over the number of samples in the class is done.

![Mean representative for Speed(20km/h) sign](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/MeanRepresentative_Speed20kmPerhr.png)
---
### Design and Test a Model Architecture
#### Preprocessing
The submission describes the preprocessing techniques used and why these techniques were chosen.
##### Data Augmentation
In order to have a good sample size of images for all classes, images present in the dataset were augmented with new images created using rotation, translation, lossy resizing techniques.  A pipeline was defined which can take up one or more of these techniques at random and apply to an input image. The new image with the image transformation techniques applied is then added to the original dataset along with the right label.

For example, a Slippery road sign is transformed to a new image by applying the augment_image() pipeline as shown below:
![Original Image](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/Before_Augmentation_Pipeline.png) ![Augmented Image](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/After_Augmentation_Pipeline.png)

For each class, the difference of the number of images available from the mean of the dataset is taken. The image augmentation technique is applied for existing representative images of the class to increase the sample by difference val.

For example, if class 1 has 200 images and mean of the dataset is 1200, each image is run through the augment_image pipeline 5 times to generate 1000 new images. At the end of applying this algorithm, all classes are required to have at least 1200 images or more.

Applying this algorithm, a total of 26939 images are added to the original dataset.

After new images are added to the dataset, the images are again split to train-test-validation datasets with original ratios of 67.1%-24.5%-8.5% respectively.

The dataset now contains 78208 images. The new sample distribution looks like the image below:

![Sample distribution post augmentation](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/New_Sample_Distribution.png)

##### Grayscale Conversion
The entire model was training originally with (32,32,3) images without any dropout layer first. Although the training-validation accuracy were high, testing accuracy on images from web were very low (1 out of 6 classifications were correct).
Further investigating into the sample images available in training, validation and test data and comparing them against the images downloaded from the web,
differences in sharpness and contrast were also found between the two classes.
Also, since the images were not cropped around their ROI (region of interest triangles as provided by the German traffic signs dataset), some of the background colors are also likely to attribute to the classification noise.
In order to remove the bias, the images were converted to gray scale which helps in learning the gradients of shapes of signs by the model. 

Here is an example of a traffic sign image before and after grayscaling.

![Before and After Gray Scaling](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/GrayScale2.png)


**_Histogram Equalization_**
In order to remove the biases due to lighting and augment fuzzy data images, histogram equalization technique was used.

Here is an example of an original image and an augmented image:
![Histogram Equalized Images](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup//HistogramEqualization.PNG)

##### Training-Validation split
As observed from the initial statistics of the given data, the training data forms around 89% of the (training + validation) mixture which could lead to overfitting while training the neural network. Further, the sample distribution of classes in training and validation datasets require to similar for us to guarantee a high validation accuracy.
This was achieved by first concatenating all the training and validation datasets, shuffling them and creating an 80-20% split.
As a result, the modified datasets were
* Number of training samples : 47324
* Number of validation samples: 11831


#### Model Architecture
Final model used for training traffic sign images consisted of the following layers:

| Layer          		|     Description             					|
|:---------------------:|:---------------------------------------------:|
| Input          		| 32x32x1 Gray scale image						|
| Convolution 5x5     	| 1x1 stride, VALID padding, outputs 28x28x6	|
| RELU     				|        									    |
| Max pooling  		    | 2x2 stride,  outputs 14x14x6      			|
| Convolution 5x5   	| 1x1 stride, VALID padding, outputs 10x10x16   |
| RELU     				| 									           	|
| Max pooling       	| 2x2 stride,  outputs 5x5x16       			|
| Flatten        		| outputs 400x1           						|
| Fully Connected       | outputs 120x1                                 |
| RELU                  |                                               |
| Fully Connected       | outputs 84x1                                  |
| RELU                  |                                               |
| Dropout               | keep_probability = 0.9                        |
| Fully Connected       | outputs 43x1                                  |
| Softmax    			| outputs 43x1 normalized                       |


#### Model Training
To train the model, the following model parameters were used:
* **Adam Optimizer** with a learning rate (eta) = 0.001 was used to minimize the loss_operation which compares the predictions of model with that of y values / labels from the dataset.
* In order to avoid overfitting, a keep_probability of 0.9 was used in **dropout layer**.
* The training data was used in 20 epochs (owing to large dataset) and during each epoch **mini-batches of 128 samples** were used to tune to optimizer's hyperparameters.
* The y labels were also converted to one_hot encoding in order to be used for softmax_cross_entropy_with_logits function to get the entropy of classification
* Accuracy function is the mean of all correct predictions when the validation / test set is used on the trained model.

#### Solution Approach
At the end of 20 epochs of training,

**Training** set accuracy = **98.19%**
**Validation** set accuracy = **94.25%**
**Test** set accuracy = **93.94%**

**What was the first architecture that was tried and why was it chosen?**
+ The initial architecture used 32x32x3 images.
+ Both with and without dropout layers were experimented on the training and validation datasets
+ The dataset was not reshuffled / augmented.
+ 10 epochs were used to train
+ Smaller learning rate of 0.00001 was also tried

**What were some problems with the initial architecture?**
+ 32x32x3 images without changing the given sample distribution for training and validation sets resulted in very high validation accuracy 98.6% while the test accuracy was very poor ~ 48%
+ Addition of dropout layer worsened the performance of the model on the validation set
+ Smaller learning rate also worsened the validation accuracy as only 10 epochs were used to train.


**How was the architecture adjusted and why was it adjusted?**
**Which parameters were tuned? How were they adjusted and why?**
**What are some of the important design choices and why were they chosen? For example, why might a convolution layer work well with this problem? How might a dropout layer help with creating a successful model?**

+ In order to boost the testing accuracy, couple of changes were introduced
1. Dataset was augmented with additional images in order to boost the sample size of each class
2. Reshuffling of training and validation datasets to redrawn samples from the shuffle dataset.  This change caused the test accuracy to go up to 80% from 48%
3. Introduction of a small dropout layer to avoid bias / overfitting in the training phase. Although inclusion of dropout layer caused a drop in training and validation accuracy, it improved test accuracy to 83%
4. Gray scaling images and sharpening them using histogram equalization - this changed the model architecture to use 32x32x1 images instead of 32x32x3 feature spaces. Test accuracy improved to 86%
5. Keeping the training epochs to 10 caused the validation accuracy to stop at 90% only. Increasing the epochs to 20 owing to large dataset further improved the accuracy.
6. Larger learning rate of 0.001 - this enhanced the training and validation accuracy under 20 epochs. Test accuracy improved to 94%

** What architecture was chosen?**
LeNet architecture was used with minor modifications.


**Why did you believe it would be relevant to the traffic sign application?**

LeNet is a popular convolutional neural network model used in the domain of image classification. Using this with traffic signs was experimental in understanding if the network can be retrained with a new dataset to perform a classification job.

**How does the final model's accuracy on the training, validation and test set provide evidence that the model is working well?**
A high training and validation accuracy along with a reasonably high testing accuracy proves that the LeNet has performed well with an entirely new dataset
Model Architecture
---
### Test a Model on New Images
#### Acquiring New Images

Here are ten German traffic signs from the training set provided in original dataset:

![Speed 20kmh](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/Class0_00006_00028.png) ![Stop Sign](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/Class14_00014_00020.png) ![No Vehicles](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/Class15_00016_00027.png)
![Road Work](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/Class25_00038_00009.png) ![Pedestrians](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/Class27_00007_00020.png) ![End of no passing by vehicles over 3.5 metric tons](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/Class42_00007_00028.png)
![No Entry](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/Class17_00003_0000.png)![End of speed limit (80 km/h)](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/Class6_00000_0000.png)
![No passing for vehicles over 3.5 metric tons](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/Class10_00006_00008.png)
![Keep Left](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/Class39_00000_0000.png)

First two images have the signs presented with a small rotation (of about 5-10 degrees).Specifically, sign mapped to Speed 20km/h represents the sign with minimum number of sample data. 

The other images have very little clarity on the signs due to the size of the image (small). Hence, these images were chosen to see if the network will do a good job classifying them.
There are also images which mean the opposite of each of (For e.g., No passing for vehicles over 3.5 metric tons and End of no passing sign) which will be an interesting data set to test the model on. 

These images are of different sizes and hence need to processed through the gray scale and histogram equalization pipeline for them to be used for testing. 
In addition to that, they are also resized to (32,32) size to be input to the model. 

#### Performance on New Images

The model has a 100% accuracy in the test images. 

Here are the results of the prediction:
| ClassId  |Image           								|     Prediction            					|
|:--------:|:----------------------------------------------:|:---------------------------------------------:|
| 0        | Speed 20km/h      							    | Speed 20km/h           						|
| 6        | End of speed limit (80 km/h)			     	| End of speed limit (80km/h)					|
| 10       | No passing for vehicles over 3.5 metric tons   | No passing for vehicles over 3.5 metric tons 	|
| 14       | Stop Sign      								| Stop Sign          							|
| 15       | No Vehicles   								    | No Vehicles         							|
| 17       | No Entry       								| No Entry	         							|
| 25       | Road Work      								| Road Work          							|
| 27       | Pedestrians   									| Road narrows on the right 					|
| 39       | Keep left 										| Keep left										|
| 42       | End of no passing 								| End of no passing         					|

The model was able to correctly guess all 10 of the traffic signs, which gives an accuracy of 100%. This compares favorably to the accuracy on the test set of 94%. All preprocessing steps applied to training, validation and test sets were also applied to new images.

#### Model Certainty - Softmax Probabilities

The code for making predictions on my final model is located under "Predict the sign for each test image and analyze performance" section.

The top six probabilities and classes of predictions for the test images are visually represented. 
For e.g. For the first sign, the top 6 prediction probabilities are:

![Prediction for Speed 20Km/Hr Sign](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup/Prediction_20KmPerHr.png)

As seen from the prediction, the top probability is pretty high compared to the second best or third best prediction when softmax layer is applied to leNet layer for all images.

---

### (Optional) Visualizing the Neural Network

#### Visual output of your trained network's feature maps.

As a test, two signs were used the 20 km/h sign (which is a circular sign) and pedestrians sign (triangular sign).

Using the helper function to output the 6 feature maps of the first convolutional layer, shows that, the feature spaces are tried to learn the circular and triangular shape of the sign respectively.

The second layer of 16 feature maps, are trying to abstract the geometrical representations (circle / triangle in the respective case) into more fundamental representations like set of contiguous darker / brighter pixels.
![CNN Layer 1 Output](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup//VisualOutput_Layer1.PNG)

![CNN Layer 2 Output](https://github.com/venkatvi/UdacitySelfDrivingCarNanoDegree/blob/master/Py/Proj2_TrafficSignClassifier/P2/To%20Submit/images_for_writeup//VisualOutput_Layer2.PNG)

