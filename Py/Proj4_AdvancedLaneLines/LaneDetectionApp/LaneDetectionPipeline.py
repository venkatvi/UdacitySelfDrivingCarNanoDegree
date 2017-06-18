# cv2
import cv2

# os, sys for iterating over files in folders
import os, sys

# 
import numpy as np

# for copying image and save them to disk 
import matplotlib.image as mpimg

import matplotlib.pyplot as plt
class LaneDetectionPipeline:
	def __init__(self, cameraCalibration, root_folder, output_folder):
		self.root_folder = root_folder
		self.output_folder = output_folder
		self.cameraCalibration = cameraCalibration
		self.image_in_colorspace = []
	def execute(self):
		print ("Starting pipeline for lane detection... ")
		# Loop through all images in the root_folders
		file_list = os.listdir(self.root_folder)
		for file in file_list:
			print("Image: " + file)
			# read image
			image = cv2.imread(self.root_folder+"/"+ file)
			
			# Use camera caliberator to undistort image
			image = self.cameraCalibration.undistort(image);
			# save image in the output_folder 
			file_parts  = file.split(".")
			mpimg.imsave(self.output_folder + "/" + file_parts[0] +"_undistorted.png" , image);
			
			color_binary = self.apply_color_threshold(image)
			mag_sobel_binary, direction_binary = self.apply_gradient_threshold(self.image_in_colorspace)
			
			# Combine the two binary thresholds
			combined_binary = np.zeros_like(color_binary)
			combined_binary[(color_binary == 1) | (mag_sobel_binary == 1)] = 1
			# save image in the output_folder 
			mpimg.imsave(self.output_folder + "/" + file_parts[0] +"_out.png" , combined_binary, cmap="gray");
			
			# Compute perspective matrix
			M = self.compute_perspective_matrix(combined_binary);
			
			# Compute warped image
			warped_image = self.apply_perspective(M, combined_binary)
			# save image in the output_folder 
			mpimg.imsave(self.output_folder + "/" + file_parts[0] +"_warped.png" , warped_image, cmap="gray");
			
			histogram = self.find_lanes(warped_image, file_parts[0], 9);
			
			#plt.close(fig)
	
	def compute_margins(self, total_height, window_height, window_id, margin, leftx_current, rightx_current):
		y_low = total_height - (window_id + 1) * window_height
		y_high = total_height - window_id * window_height
		xleft_low = leftx_current - margin
		xleft_high = leftx_current + margin
		xright_low = rightx_current - margin
		xright_high = rightx_current + margin
		return xleft_low, xleft_high, xright_low, xright_high, y_low, y_high
		
	def slide_windows(self, image, nwindows, window_height, nonzero_x, nonzero_y, leftx_current, rightx_current, margin, minpix):
		sliding_window_image = np.copy(image);
		left_lane_inds = []
		right_lane_inds = []
		for window in range(nwindows):
			# identify window boundaries x, y , right and left 
			l_low, l_high, r_low, r_high, y_low, y_high = self.compute_margins(image.shape[0], window_height, window, margin, leftx_current, rightx_current);
			
			# Draw the windows on the visualization image
			cv2.rectangle(sliding_window_image,(l_low,y_low),(l_high,y_high),(0,255,0), 2) 
			cv2.rectangle(sliding_window_image,(r_low,y_low),(r_high,y_high),(0,255,0), 2) 
			
			# Identify the nonzero pixels in x and y within the window
			good_left_inds = ((nonzero_y >= y_low) & (nonzero_y < y_high) & (nonzero_x >= l_low) & (nonzero_x < l_high)).nonzero()[0]
			good_right_inds = ((nonzero_y >= y_low) & (nonzero_y < y_high) & (nonzero_x >= r_low) & (nonzero_x < r_high)).nonzero()[0]
			
			# If you found > minpix pixels, recenter next window on their mean position
			if len(good_left_inds) > minpix:
				leftx_current = np.int(np.mean(nonzero_x[good_left_inds]))
			if len(good_right_inds) > minpix:        
				rightx_current = np.int(np.mean(nonzero_x[good_right_inds]))
				
			# Append these indices to the lists
			left_lane_inds.append(good_left_inds)
			right_lane_inds.append(good_right_inds)
			
		# Concatenate the arrays of indices
		left_lane_inds = np.concatenate(left_lane_inds)
		right_lane_inds = np.concatenate(right_lane_inds)
		
		return sliding_window_image, left_lane_inds, right_lane_inds
	
	def fit_lane_line(self, left_lane_inds, right_lane_inds, nonzero_x, nonzero_y):
		# Extract left and right line pixel positions
		leftx = nonzero_x[left_lane_inds]
		lefty = nonzero_y[left_lane_inds] 
		rightx = nonzero_x[right_lane_inds]
		righty = nonzero_y[right_lane_inds] 

		# Fit a second order polynomial to each
		left_fit = np.polyfit(lefty, leftx, 2)
		right_fit = np.polyfit(righty, rightx, 2)
		
		return left_fit, right_fit
	def find_lanes(self, image, out_file_name, number_sliding_windows=9, margin=100, min_qual_pixels=50):
		histogram = np.sum(image[image.shape[0]//2:,:], axis=0)
		#fig = plt.figure(1);
		#plt.plot(histogram)
		# save image in the output_folder 
		#plt.savefig(self.output_folder + "/" + out_file_name +"_histogram.png");
		
		# Find the peak of the left and right halves of the histogram
		# These will be the starting point for the left and right lines
		midpoint = np.int(histogram.shape[0]/2)
		leftx_base = np.argmax(histogram[:midpoint])
		rightx_base = np.argmax(histogram[midpoint:]) + midpoint
		
		nwindows = number_sliding_windows
		window_height = np.int(image.shape[0]/nwindows)
		
		# Identify all nonzero pixesl of the image
		nonzero = image.nonzero()
		nonzero_x = np.array(nonzero[0])
		nonzero_y = np.array(nonzero[1])
		
		leftx_current = leftx_base
		rightx_current = rightx_base
		
		# Set width of windows +/- margin
		margin = margin
		# Set min number of pixels to be found to recenter the window_height
		minpix = min_qual_pixels
		
		# Slide windows over the image from bottom to top to get the line fit
		out_img, left_lane_inds, right_lane_inds = self.slide_windows(image, nwindows, window_height, nonzero_x, nonzero_y, leftx_current, rightx_current, margin, minpix);
		
		# once the candidates of line indices are obtained, fit a polynomial
		left_fit, right_fit = self.fit_lane_line(left_lane_inds, right_lane_inds, nonzero_x, nonzero_y)
		
		# Visualize the fitted lines on the input image
		ploty = np.linspace(0, image.shape[0]-1, image.shape[0] )
		left_fitx = left_fit[0]*ploty**2 + left_fit[1]*ploty + left_fit[2]
		right_fitx = right_fit[0]*ploty**2 + right_fit[1]*ploty + right_fit[2]

		out_img[nonzeroy[left_lane_inds], nonzerox[left_lane_inds]] = [255, 0, 0]
		out_img[nonzeroy[right_lane_inds], nonzerox[right_lane_inds]] = [0, 0, 255]
		plt.imshow(out_img)
		plt.plot(left_fitx, ploty, color='yellow')
		plt.plot(right_fitx, ploty, color='yellow')
		plt.xlim(0, 1280)
		plt.ylim(720, 0)
	
	def compute_perspective_matrix(self, image):
		image_shape = image.shape;
		w = image_shape[1];
		h = image_shape[0];
		offset = 200;
		src = np.float32([[210, h-20], [w/2 - 40, h/2 + 90 ], [w/2 + 40 , h/2 + 90],[1070, h-20]])
		dst = np.float32([[210+offset, h-20], [210+offset, 0 ], [1070- offset , 0],[1070-offset, h-20]])
		M = cv2.getPerspectiveTransform(src, dst)
		return M
	def apply_perspective(self, M, image):
		img_size = (image.shape[1], image.shape[0])
		warped_image = cv2.warpPerspective(image, M, img_size, flags=cv2.INTER_LINEAR)
		return warped_image
		
	def apply_color_threshold(self, image, color_space="HLS", thresh_channel=2, thresh=(170, 255)):
		image_in_colorspace = [];
		if color_space == "HLS":
			image_in_colorspace = cv2.cvtColor(image, cv2.COLOR_RGB2HLS)
		elif color_space == "HSV":
			image_in_colorspace = cv2.cvtColor(image, cv2.COLOR_RGB2HSV)
		else:
			image_in_colorspace = np.copy(image)
		
		thresholding_channel = image_in_colorspace[:, :, thresh_channel]

		color_binary = np.zeros_like(thresholding_channel)
		color_binary[(thresholding_channel >= thresh[0]) & (thresholding_channel <= thresh[1])] = 1
		self.image_in_colorspace = thresholding_channel
		return color_binary
	def get_scaled_sobel(self, image, orient="x"):
		sobel_val = [];
		if orient == "x":
			sobel_val = cv2.Sobel(image, cv2.CV_64F, 1, 0)
		elif orient == "y":
			sobel_val = cv2.Sobel(image, cv2.CV_64F, 1, 0)
		abs_sobel = np.absolute(sobel_val)
		scaled_sobel = np.uint8(255*abs_sobel/np.max(abs_sobel))
		return scaled_sobel
		
	def apply_gradient_threshold(self, image, abs_thresh=(20, 100), direction_thresh=(0.7, 1.3)):
		#gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
		scaled_sobel_x = self.get_scaled_sobel(image, orient="x")
		mag_sobel_binary = np.zeros_like(scaled_sobel_x)
		mag_sobel_binary[(scaled_sobel_x >= abs_thresh[0]) & (scaled_sobel_x <= abs_thresh[1])] = 1
		
		scaled_sobel_y = self.get_scaled_sobel(image, orient="y")
		direction_gradient = np.arctan2(scaled_sobel_y, scaled_sobel_x)
		direction_binary = np.zeros_like(direction_gradient)
		direction_binary[(direction_gradient > direction_thresh[0]) & (direction_gradient <= abs_thresh[1])] = 1
		return mag_sobel_binary, direction_binary