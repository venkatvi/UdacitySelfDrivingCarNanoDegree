# cv2
import cv2

# os, sys for iterating over files in folders
import os, sys

# 
import numpy as np

# for copying image and save them to disk 
import matplotlib.image as mpimg

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
			file_parts  = file.split(".")
			mpimg.imsave(self.output_folder + "/" + file_parts[0] +"_out.png" , combined_binary, cmap="gray");
			
			# Compute perspective matrix
			M = self.compute_perspective_matrix(combined_binary);
			
			# Compute warped image
			warped_image = self.apply_perspective(M, combined_binary)
			
			# save image in the output_folder 
			file_parts  = file.split(".")
			mpimg.imsave(self.output_folder + "/" + file_parts[0] +"_warped.png" , warped_image, cmap="gray");
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