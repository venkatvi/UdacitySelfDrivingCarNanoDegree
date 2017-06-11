
from trainSVMFromData import *
def draw_boxes(img, bboxes, color=(0,0,1), thick=6):
	imcopy = np.copy(img)
	for bbox in bboxes:
		cv2.rectangle(imcopy, bbox[0], bbox[1], color, thick)
	return imcopy
if __name__ == "__main__":
	#pickle_file_name = trainSVMFromData();
	pickle_file_name = "car_svc.p"
	import pickle
	data = pickle.load(open(pickle_file_name, "rb"));
	clf = data['Classifier'];
	scalar = data['Scalar'];
	
	x_start_stop=[500, 1200]
	y_start_stop = [400, 656]
	windows_sizes = [32, 48, 64, 96, 128];
	xy_overlaps = [0.5, 0.75]
	box_colors = [(0,0, 1), (0,1,0), (0,0,1), (0, 1, 1), (1, 1, 0)];
	
	from VDetectApp.FeatureExtractor.FeatureExtractor import FeatureExtractor
	from VDetectApp.SlidingWindowConfigurator.SlidingWindowConfigurator import SlidingWindowConfigurator
	import cv2
	import os, sys
	import matplotlib.image as mpimg
	import matplotlib.pyplot as plt
	root_folder = "test_images";
	file_list = os.listdir(root_folder)
	
	for file in file_list:
		print(file)
		featureExtractor = FeatureExtractor(False, file, root_folder)
		copied_image = np.copy(featureExtractor.image)
		index = 0
		for window_size in windows_sizes:
			print("Window Size: " + str(window_size));
			windows = [];
			for overlap in xy_overlaps:
				print("Overlap: " + str(overlap));
				tempWindows = SlidingWindowConfigurator.slideWindow(image=copied_image, x_start_stop=x_start_stop, y_start_stop=y_start_stop, 
						xy_window=(window_size, window_size), xy_overlap=(overlap, overlap))
				windows.extend(tempWindows);
			
			on_windows = [];
			for window in windows:
				test_image = cv2.resize(featureExtractor.image[window[0][1]:window[1][1], window[0][0]:window[1][0]], (64, 64))
				
				subFeatures = computePerImageFeatures(True, test_image, root_folder=None, y_start=None, y_stop=None, color_space='HSV', scale=1, orientations=9, pixels_per_cell=8, cells_per_block=2, feature_vec=False, concatenate_features=True)
			
				if scalar.mean_.shape[0] == subFeatures.shape[0]:
					test_features = scalar.transform(np.array(subFeatures).reshape(1,-1))
					prediction = clf.predict(test_features);
					if prediction == 1:
						on_windows.append(window)
				else:
					print(scalar.mean_.shape)
					print(subFeatures.shape)
					
			copied_image = draw_boxes(copied_image, on_windows, box_colors[index], thick=6);
			index=index+1
		file_parts = file.split(".")
		mpimg.imsave(file_parts[0] +"_windows_new.png" , copied_image);