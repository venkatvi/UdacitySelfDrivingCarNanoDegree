from Line import *

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
		self.frame_count = 0
		self.Line = Line()
		self.prev_left_fit = []
		self.prev_right_fit = []
	def execute(self):
		print ("Starting pipeline for lane detection... ")
		# Loop through all images in the root_folders
		file_list = os.listdir(self.root_folder)
		for file in file_list:
			print("Image: " + file)
			# read image
			image = cv2.imread(self.root_folder+"/"+ file)
			
			output_image  = self.process_image(file, image);
	def process_image(self, image, file="sample"):
		# Use camera caliberator to undistort image
		image = self.cameraCalibration.undistort(image);
		# save image in the output_folder 
		file_parts  = file.split(".")
		mpimg.imsave(self.output_folder + "/" + file_parts[0] +"_undistorted.png" , image);
		
		color_binary = self.apply_color_threshold(image)
		mpimg.imsave(self.output_folder + "/" + file_parts[0] +"_colorbinary.png" , color_binary);
		
		mag_sobel_binary, direction_binary = self.apply_gradient_threshold(color_binary)
		
		# Combine the two binary thresholds
		combined_binary = np.zeros_like(color_binary)
		combined_binary[(mag_sobel_binary == 1) | (direction_binary == 1)] = 1
		# save image in the output_folder 
		mpimg.imsave(self.output_folder + "/" + file_parts[0] +"_out.png" , combined_binary, cmap="gray");
		
		# Compute perspective matrices
		M, Minv = self.compute_perspective_matrix(combined_binary);
		
		# Compute warped image
		warped_image = self.apply_perspective(M, combined_binary)
		# save image in the output_folder 
		mpimg.imsave(self.output_folder + "/" + file_parts[0] +"_warped.png" , warped_image, cmap="gray");
		
		if self.frame_count == 0:
			left_fit, right_fit, left_fitx, right_fitx, ploty = self.find_lanes(warped_image, file_parts[0], 9);
		else:
			left_fit, right_fit, left_fitx, right_fitx, ploty = self.find_lanes_without_sliding_windows(warped_image, file_parts[0], self.prev_left_fit, self.prev_right_fit, 9 );
		
		self.prev_left_fit = left_fit
		self.prev_right_fit = right_fit
		
		left_curvature, right_curvature = self.compute_radius_of_curvature(ploty, left_fitx, right_fitx)
		mean_curvature = np.mean([left_curvature, right_curvature])/1000.0;
		output_image = self.generate_output(image, warped_image, Minv, left_fitx, right_fitx, ploty, mean_curvature)
		
		# save image in the output_folder 
		mpimg.imsave(self.output_folder + "/" + file_parts[0] +"_final_output.png" , output_image);
		
		self.frame_count+=1
		
		return output_image
	
	def apply_color_threshold(self, image, color_space="HLS", thresh_channel=2, thresh=(70, 255)):
		image_in_colorspace = [];
		if color_space == "HLS":
			image_in_colorspace = cv2.cvtColor(image, cv2.COLOR_RGB2HLS)
		elif color_space == "HSV":
			image_in_colorspace = cv2.cvtColor(image, cv2.COLOR_RGB2HSV)
		else:
			image_in_colorspace = np.copy(image)
		
		thresholding_channel = image_in_colorspace[:, :, thresh_channel]

		'''
		yellow_hsv_low  = np.array([ 0, 80, 200])
		yellow_hsv_high = np.array([ 40, 255, 255])
		res = apply_color_mask(image_HSV,warped,yellow_hsv_low,yellow_hsv_high)
		
		white_hsv_low  = np.array([  20,   0,   200])
		white_hsv_high = np.array([ 255,  80, 255])
		'''
		
		color_binary = np.zeros_like(thresholding_channel)
		color_binary[(thresholding_channel >= thresh[0]) & (thresholding_channel <= thresh[1])] = 1
		self.image_in_colorspace = thresholding_channel
		return color_binary
		
	def apply_gradient_threshold(self, image, abs_thresh=(20, 100), direction_thresh=(0.7, 1.3)):
		#gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
		scaled_sobel_x = self.get_scaled_sobel(image, orient="x")
		mag_sobel_binary = np.zeros_like(scaled_sobel_x)
		mag_sobel_binary[(scaled_sobel_x >= abs_thresh[0]) & (scaled_sobel_x <= abs_thresh[1])] = 1
		
		scaled_sobel_y = self.get_scaled_sobel(image, orient="y")
		direction_gradient = np.arctan2(scaled_sobel_y, scaled_sobel_x)
		direction_binary = np.zeros_like(direction_gradient)
		direction_binary[(direction_gradient > direction_thresh[0]) & (direction_gradient <= direction_thresh[1])] = 1
		return mag_sobel_binary, direction_binary
		
	def compute_perspective_matrix(self, image):
		image_shape = image.shape;
		w = image_shape[1];
		h = image_shape[0];
		offset = 200;
		#src = np.float32([[210, h-20], [w/2 - 40, h/2 + 90 ], [w/2 + 40 , h/2 + 90],[1070, h-20]])
		#dst = np.float32([[210+offset, h-20], [210+offset, 0 ], [1070- offset , 0],[1070-offset, h-20]])
		src = np.float32([ [200,720],[600,450],[1150,720],[700,450]])
		dst = np.float32([ [350,720],[350,0],  [1000,720],[1000,0]])
		M = cv2.getPerspectiveTransform(src, dst)
		Minv = cv2.getPerspectiveTransform(dst, src)
		return M, Minv
		
	def apply_perspective(self, M, image):
		img_size = (image.shape[1], image.shape[0])
		warped_image = cv2.warpPerspective(image, M, img_size, flags=cv2.INTER_LINEAR)
		return warped_image
	
	
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
		nonzero_y = np.array(nonzero[0])
		nonzero_x = np.array(nonzero[1])
		
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

		
		fig = plt.figure(1);
		out_img[nonzero_y[left_lane_inds], nonzero_x[left_lane_inds]] = [255, 0, 0]
		out_img[nonzero_y[right_lane_inds], nonzero_x[right_lane_inds]] = [0, 0, 255]
		plt.imshow(out_img)
		plt.plot(left_fitx, ploty, color='yellow')
		plt.plot(right_fitx, ploty, color='yellow')
		plt.xlim(0, 1280)
		plt.ylim(720, 0)
		#plt.show()
		plt.savefig(self.output_folder + "/" + out_file_name +"_detected_lanes.png");
		plt.close(fig);
	
		return left_fit, right_fit, left_fitx, right_fitx, ploty
	def find_lanes_without_sliding_windows(self, image, out_file_name,  prev_left_fit, prev_right_fit, number_sliding_windows=9, margin=100, min_qual_pixels=50):
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
		nonzero_y = np.array(nonzero[0])
		nonzero_x = np.array(nonzero[1])
		
		leftx_current = leftx_base
		rightx_current = rightx_base
		
		# Set width of windows +/- margin
		margin = margin
		# Set min number of pixels to be found to recenter the window_height
		minpix = min_qual_pixels
		
		
		left_lane_inds = ((nonzero_x > (prev_left_fit[0]*(nonzero_y**2) + prev_left_fit[1]*nonzero_y + prev_left_fit[2] - margin)) & (nonzero_x < (prev_left_fit[0]*(nonzero_y**2) + prev_left_fit[1]*nonzero_y + prev_left_fit[2] + margin))) 
		right_lane_inds = ((nonzero_x > (prev_right_fit[0]*(nonzero_y**2) + prev_right_fit[1]*nonzero_y + prev_right_fit[2] - margin)) & (nonzero_x < (prev_right_fit[0]*(nonzero_y**2) + prev_right_fit[1]*nonzero_y + prev_right_fit[2] + margin)))  

		# once the candidates of line indices are obtained, fit a polynomial
		left_fit, right_fit = self.fit_lane_line(left_lane_inds, right_lane_inds, nonzero_x, nonzero_y)
		
		# Visualize the fitted lines on the input image
		ploty = np.linspace(0, image.shape[0]-1, image.shape[0] )
		left_fitx = left_fit[0]*ploty**2 + left_fit[1]*ploty + left_fit[2]
		right_fitx = right_fit[0]*ploty**2 + right_fit[1]*ploty + right_fit[2]


		out_img = np.dstack((image, image, image))*255		
		fig = plt.figure(1);
		out_img[nonzero_y[left_lane_inds], nonzero_x[left_lane_inds]] = [255, 0, 0]
		out_img[nonzero_y[right_lane_inds], nonzero_x[right_lane_inds]] = [0, 0, 255]
		plt.imshow(out_img)
		plt.plot(left_fitx, ploty, color='yellow')
		plt.plot(right_fitx, ploty, color='yellow')
		plt.xlim(0, 1280)
		plt.ylim(720, 0)
		#plt.show()
		plt.savefig(self.output_folder + "/" + out_file_name +"_detected_lanes.png");
		plt.close(fig);
	
		return left_fit, right_fit, left_fitx, right_fitx, ploty
	def compute_radius_of_curvature(self, ploty, left_fitx, right_fitx):
		#Define y-value where we want radius of curvature
		# I'll choose the maximum y-value, corresponding to the bottom of the image
		y_eval = np.max(ploty)
		
		# Define conversions in x and y from pixels space to meters
		ym_per_pix = 30/720 # meters per pixel in y dimension
		xm_per_pix = 3.7/700 # meters per pixel in x dimension

		# Fit new polynomials to x,y in world space
		left_fit_cr = np.polyfit(ploty*ym_per_pix, left_fitx*xm_per_pix, 2)
		right_fit_cr = np.polyfit(ploty*ym_per_pix, right_fitx*xm_per_pix, 2)
		
		# Calculate the new radii of curvature
		left_curverad = ((1 + (2*left_fit_cr[0]*y_eval*ym_per_pix + left_fit_cr[1])**2)**1.5) / np.absolute(2*left_fit_cr[0])
		right_curverad = ((1 + (2*right_fit_cr[0]*y_eval*ym_per_pix + right_fit_cr[1])**2)**1.5) / np.absolute(2*right_fit_cr[0])
		
		return left_curverad, right_curverad

	def generate_output(self, original_image, warped_image, Minv, left_fitx, right_fitx, ploty, curvature):
		# Create an image to draw the lines on
		warp_zero = np.zeros_like(warped_image).astype(np.uint8)
		color_warp = np.dstack((warp_zero, warp_zero, warp_zero))

		# Recast the x and y points into usable format for cv2.fillPoly()
		pts_left = np.array([np.transpose(np.vstack([left_fitx, ploty]))])
		pts_right = np.array([np.flipud(np.transpose(np.vstack([right_fitx, ploty])))])
		pts = np.hstack((pts_left, pts_right))

		# Draw the lane onto the warped blank image
		cv2.fillPoly(color_warp, np.int_([pts]), (0,255, 0))

		# Warp the blank back to original image space using inverse perspective matrix (Minv)
		newwarp = cv2.warpPerspective(color_warp, Minv, (warped_image.shape[1], warped_image.shape[0])) 
		
		# Combine the result with the original image
		result_image = cv2.addWeighted(original_image, 1, newwarp, 0.3, 0)
	
		lane_width = np.absolute(left_fitx[0] - right_fitx[0])
		lane_center = left_fitx[0] + (lane_width/2)
		offset = lane_center - (warped_image.shape[1]/2)
		xm_per_pix = 3.7/700 # meters per pixel in x dimension
		offset_in_meters = offset*xm_per_pix
		
		offset_direction = "left";
		if offset < 0:
			offset_direction = "right"
		
		# Write details on radius of curvature and lane center offset
		font_face = cv2.FONT_HERSHEY_SIMPLEX
		font_scale = 1
		font_color = (255, 255, 255)
		#result_image = cv2.putText(result_image,"Radius of Curvature = " + str(curvature) + " km", (0, 50), font_face, font_scale, font_color)
		#result_image = cv2.putText(result_image,"Vehicle is " + str(np.absolute(offset_in_meters)) + " m " + offset_direction +  " of center", (0, 100), font_face, font_scale, font_color)
		return result_image
		
	def get_scaled_sobel(self, image, orient="x"):
		sobel_val = [];
		if orient == "x":
			sobel_val = cv2.Sobel(image, cv2.CV_64F, 1, 0)
		elif orient == "y":
			sobel_val = cv2.Sobel(image, cv2.CV_64F, 0, 1)
		abs_sobel = np.absolute(sobel_val)
		scaled_sobel = np.uint8(255*abs_sobel/np.max(abs_sobel))
		return scaled_sobel
	
	def compute_margins(self, total_height, window_height, window_id, margin, leftx_current, rightx_current):
		y_low = total_height - (window_id + 1) * window_height
		y_high = total_height - window_id * window_height
		xleft_low = leftx_current - margin
		xleft_high = leftx_current + margin
		xright_low = rightx_current - margin
		xright_high = rightx_current + margin
		return xleft_low, xleft_high, xright_low, xright_high, y_low, y_high
		
	def slide_windows(self, image, nwindows, window_height, nonzero_x, nonzero_y, leftx_current, rightx_current, margin, minpix):
		# Create an output image to draw on and  visualize the result
		sliding_window_image = np.dstack((image, image, image))*255
		left_lane_inds = []
		right_lane_inds = []
		for window in range(nwindows):
			# identify window boundaries x, y , right and left 
			l_low, l_high, r_low, r_high, y_low, y_high = self.compute_margins(image.shape[0], window_height, window, margin, leftx_current, rightx_current);
			# Draw the windows on the visualization image
			sliding_window_image = cv2.rectangle(sliding_window_image,(l_low,y_low),(l_high,y_high),(0,255,0), 6) 
			sliding_window_image = cv2.rectangle(sliding_window_image,(r_low,y_low),(r_high,y_high),(0,255,0), 6) 
			
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
	