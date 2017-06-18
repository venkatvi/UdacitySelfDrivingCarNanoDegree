# for pretty much everything 
import numpy as np 

# for camera caliberation
import cv2

# for opening files
import os, sys

# for copying image and save them to disk 
import matplotlib.image as mpimg
class CameraCaliberator:
	def __init__(self, test_images_folder, output_folder, number_of_corners=(8,6)):
		self.root_folder = test_images_folder
		self.output_folder = output_folder
		self.nx = number_of_corners[0]
		self.ny = number_of_corners[1]
		self.image_points = []
		self.object_points = []
		self.image_shape = None
	
	def caliberate(self):
		# Compute default set of object points 
		object_points = np.zeros((self.ny*self.nx, 3), np.float32)
		
		# reshape object points to x,y coordinates 
		object_points[:,:2] = np.mgrid[0:self.nx, 0:self.ny].T.reshape(-1, 2)
		
		print(object_points)
		# Loop through all images in the root_folders
		file_list = os.listdir(self.root_folder)
		for file in file_list:
			print("Caliberating for "+ file)
			# read image
			image = cv2.imread(self.root_folder+"/"+ file)
			
			# convert image to grayscale 
			gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
			
			if self.image_shape is None:
				self.image_shape = gray.shape[::-1]
				
			# use gray scale image to find chess board corners 
			ret, corners = cv2.findChessboardCorners(gray, (self.nx,self.ny), None)
			
			# generate corners identified image
			img_corners = np.copy(image)
			cv2.drawChessboardCorners(img_corners, (self.nx,self.ny), corners, ret)
			
			# save image in the output_folder 
			file_parts  = file.split(".")
			mpimg.imsave(self.output_folder + "/" + file_parts[0] +"_corners.png" , img_corners);
			
			if corners is not None:
				self.image_points.append(corners)
				self.object_points.append(object_points)
		
		print(len(self.object_points))
		print(len(self.image_points))
		print(self.image_shape)
		# Calibrate camera using object points and image points
		ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(self.object_points, self.image_points, self.image_shape, None, None)
		if self.ret == True:
			self.CameraMatrix = mtx
			self.distortionMatrix = dist
			self.RadialVectors = rvecs
			self.TangentialVectors = tvecs
		
			