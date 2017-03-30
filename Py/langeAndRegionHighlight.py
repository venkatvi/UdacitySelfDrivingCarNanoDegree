import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np


def laneHighlight(image):
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
	#plt.imshow(color_select)
	#plt.show()
	return thresholds

def regionHighlight(image, color_thresholds):
	print image.shape
	#Copy image to hightlight lane 
	lane_image = np.copy(image)

	#Define a triangle region of interest.
	#[0,0] defines the upper left corner in image
	left_bottom = [170, 512]
	right_bottom = [790, 537]
	apex = [470, 310]

	fit_left = np.polyfit((left_bottom[0], apex[0]), (left_bottom[1], apex[1]), 1)
	fit_right = np.polyfit((right_bottom[0], apex[0]), (right_bottom[1], apex[1]), 1)
	fit_bottom = np.polyfit((left_bottom[0], right_bottom[0]), (left_bottom[1], right_bottom[1]), 1)

	XX, YY = np.meshgrid(np.arange(0, xsize), np.arange(0, ysize))
	region_thresholds = (YY > (XX*fit_left[0] + fit_left[1])) &\
						(YY > (XX*fit_right[0] + fit_right[1])) &\
						(YY < (XX*fit_bottom[0] + fit_bottom[1]))
	
	lane_image[~color_thresholds & region_thresholds] = [1, 0, 0]
	plt.imshow(lane_image)
	plt.show()
if __name__ == '__main__':
	# Read Image
	image = mpimg.imread('test copy.png')
	print('This image is: ', type(image), ' with dimensions: ', image.shape)

	# Read x and y dimensions of image
	ysize = image.shape[0]
	xsize = image.shape[1]

	color_thresholds = laneHighlight(image)
	regionHighlight(image, color_thresholds)