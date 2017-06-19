import numpy as np
def computePerImageFeatures(isImage, file, root_folder=None, y_start=None, y_stop=None, color_space='RGB', scale=1, orientations=9, pixels_per_cell=8, cells_per_block=2, feature_vec=False, concatenate_features=False):
	#print('#.1: Feature Extraction');		
	from VDetectApp.FeatureExtractor.FeatureExtractor import FeatureExtractor	
	featureExtractor = FeatureExtractor(isImage, file, root_folder=root_folder);

	if y_start is not None:
	#	print('#.1b: Crop Image');
		featureExtractor.cropImage(y_start, y_stop);
	
	#print('#.2: Apply Color Space');
	featureExtractor.applyColorSpace(color_space=color_space)
	
	#print('#.3: Rescale image');
	featureExtractor.rescaleImage(scale=scale)
	
	#print('#.4: Spatial Features');
	featureExtractor.extractSpatialFeatures(spatial_size=(32,32))
	
	#print('#.5: Histogram Features');
	#featureExtractor.extractHistogramFeatures(number_of_histogram_bins=32, bins_range=(0,256))	
	
	#print('#.6: HOG features');
	featureExtractor.extractHOGFeatures(hog_channel="ALL", orientations=orientations, pixels_per_cell=pixels_per_cell, cells_per_block=cells_per_block, visualise=False, feature_vec=feature_vec)
	
	concatenated_features = [];
	if concatenate_features == True:
	#	print('#.7: Concatenate all features');
		concatenated_features = np.concatenate((featureExtractor.spatial_features, np.ravel(featureExtractor.hog_features)))	
	else:
		concatenated_features={'spatial_features': featureExtractor.spatial_features, 'histogram_features': featureExtractor.histogram_features, 'HOG_features': featureExtractor.hog_features};
	return concatenated_features