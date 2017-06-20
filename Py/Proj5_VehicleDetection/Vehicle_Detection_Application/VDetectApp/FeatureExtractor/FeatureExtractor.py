import matplotlib.image as mpimg
import numpy as np
import cv2
from skimage.feature import hog
class FeatureExtractor:
	def __init__(self, isImage, image_file, root_folder=None):
		if isImage == True:
			self.image = image_file
		else:
			if root_folder is not None:
				s = "\\"
				sequence = (root_folder, image_file)
				fileFullPath = s.join(sequence)
				self.image = mpimg.imread(fileFullPath)
				#self.image = self.image.astype(np.float32)/255.0
			else:
				self.image = mpimg.imread(image_file);
		self.image = self.image[:,:,:3]
	def setImage(self, image):
		self.image = image
	def applyColorSpace(self, color_space='RGB'):
		feature_image = np.copy(self.image)
		if color_space != 'RGB':
			if color_space == 'HSV':
				feature_image = cv2.cvtColor(self.image, cv2.COLOR_RGB2HSV)
			elif color_space == 'LUV':
				feature_image = cv2.cvtColor(self.image, cv2.COLOR_RGB2LUV)
			elif color_space == 'HLS':
				feature_image = cv2.cvtColor(self.image, cv2.COLOR_RGB2HLS)
			elif color_space == 'YUV':
				feature_image = cv2.cvtColor(self.image, cv2.COLOR_RGB2YUV)
			elif color_space == 'YCrCb':
				feature_image = cv2.cvtColor(self.image, cv2.COLOR_RGB2YCrCb)
			elif color_space == 'HLS':
				feature_image = cv2.cvtColor(self.image, cv2.COLOR_RGB2HLS)
		self.feature_image = feature_image	
	def rescaleImage(self, scale=1):
		self.scaled_feature_image = self.feature_image
		if scale!=1:
			image_shape = self.feature_image.shape
			self.scaled_feature_image = cv2.resize(self.feature_image, (np.int(image_shape[1]/scale), np.int(image_shape[0]/scale)))
		
	def extractSpatialFeatures(self, spatial_size=(32,32)):
		features = cv2.resize(self.scaled_feature_image, spatial_size).ravel()
		self.spatial_features = features
		
	def extractHistogramFeatures(self, number_of_histogram_bins=32, bins_range=(0, 256)):
		
		channel_1 = np.histogram(self.scaled_feature_image[:,:,0], bins=number_of_histogram_bins)
		channel_2 = np.histogram(self.scaled_feature_image[:,:,1], bins=number_of_histogram_bins)
		channel_3 = np.histogram(self.scaled_feature_image[:,:,2], bins=number_of_histogram_bins)
		self.histogram_features = np.concatenate((channel_1[0],channel_2[0], channel_3[0]))
	
	def getHogFeatures(self, image_channel, orientations, pixel_per_cell, cell_per_block, visualise, feature_vec):
		if visualise == True:
			features, hog_image = hog(image_channel, orientations=orientations, pixels_per_cell=(pixel_per_cell, pixel_per_cell), cells_per_block=(cell_per_block, cell_per_block), transform_sqrt=True, visualise=visualise, feature_vector=feature_vec)
			return features, hog_image
		else:
			features = hog(image_channel, orientations=orientations, pixels_per_cell=(pixel_per_cell, pixel_per_cell), cells_per_block=(cell_per_block, cell_per_block), transform_sqrt=True, visualise=visualise, feature_vector=feature_vec)
			return features
	def extractHOGFeatures(self, hog_channel="ALL", orientations=9, pixels_per_cell=8, cells_per_block=2, visualise=True, feature_vec=True):
		if hog_channel=="ALL":
			input_channels=[0, 1, 2];
		else:
			input_channels = [hog_channel];
			
		self.hog_features = []
		for channel in input_channels:
			temp_hog_features = self.getHogFeatures(self.scaled_feature_image[:,:,channel], orientations, pixels_per_cell, cells_per_block, visualise, feature_vec)
			self.hog_features.append(temp_hog_features)
	def cropImage(self, y_start, y_stop):
		self.image = self.image[y_start:y_stop, :, :]
		
		
	