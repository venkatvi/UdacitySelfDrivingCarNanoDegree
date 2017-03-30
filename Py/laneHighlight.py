import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np

# Read Image
image = mpimg.imread('test copy.png')
print('This image is: ', type(image), ' with dimensions: ', image.shape)

# Read x and y dimensions of image
ysize = image.shape[0]
xsize = image.shape[1]

# Copy image to change it 
color_select = np.copy(image)

# Set thresholds for RGB values below which the image will be set to [0,0,0] (black)
red_threshold = 0.8
green_threshold = 0.8
blue_threshold = 0.8

rgb_threshold = [red_threshold, green_threshold, blue_threshold]

thresholds = (image[:,:,0] < rgb_threshold[0] )\
			| (image[:,:,1] < rgb_threshold[1]) \
			| (image[:,:,2] < rgb_threshold[2]) 


color_select[thresholds] = [0, 0, 0]

# Plot the image which highlights only lanes 
plt.imshow(color_select)
plt.show()