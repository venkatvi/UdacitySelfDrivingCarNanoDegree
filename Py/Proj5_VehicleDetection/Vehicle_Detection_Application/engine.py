from CameraCaliberator import *
from trainSVMFromData import *
def add_heat(heatmap, windows):
    # Iterate through list of bboxes
    for window in windows:
        # Add += 1 for all pixels inside each bbox
        # Assuming each "window" takes the form ((x1, y1), (x2, y2))
        heatmap[window[0][1]:window[1][1], window[0][0]:window[1][0]] += 1

    # Return updated heatmap
    return heatmap# Iterate through list of bboxes
    
def apply_threshold(heatmap, threshold):
    # Zero out pixels below the threshold
    heatmap[heatmap <= threshold] = 0
    # Return thresholded map
    return heatmap

def draw_labeled_bboxes(img, labels):
	imcopy = np.copy(img)
    # Iterate through all detected cars
	for car_number in range(1, labels[1]+1):
		# Find pixels with each car_number label value
		nonzero = (labels[0] == car_number).nonzero()
		# Identify x and y values of those pixels
		nonzeroy = np.array(nonzero[0])
		nonzerox = np.array(nonzero[1])
		# Define a bounding box based on min/max x and y
		bbox = ((np.min(nonzerox), np.min(nonzeroy)), (np.max(nonzerox), np.max(nonzeroy)))
		# Draw the box on the image
		cv2.rectangle(imcopy, bbox[0], bbox[1], (0,0,255), 6)
	return imcopy
	
def draw_boxes(img, bboxes, color=(0,0,255), thick=6):
	imcopy = np.copy(img)
	for bbox in bboxes:
		imcopy = cv2.rectangle(imcopy, bbox[0], bbox[1], color, thick)
	return imcopy
	
def detect_vehicle(isImage, file, root_folder, output_folder):
	file_parts = file.split(".")
	featureExtractor = FeatureExtractor(isImage, file, root_folder)
	mpimg.imsave(output_folder + "/"+ file_parts[0] + "_original.png", featureExtractor.image);
	
	#featureExtractor.setImage(cc.undistort(featureExtractor.image))
	#mpimg.imsave(output_folder + "/"+ file_parts[0] + "_undistort.png", featureExtractor.image);
	
	copied_image = np.copy(featureExtractor.image)
	another_copy_image = np.copy(featureExtractor.image)
	heat = np.zeros_like(copied_image[:,:,0]).astype(np.float)
	print(featureExtractor.image.shape)
	boxes_image = []
	all_windows = []
	index = 0
	for window_size in windows_sizes:
		print("Window Size: " + str(window_size));
		windows = [];
		x_start_stop = [None, None]
		y_start_stop = [None, None]
		for overlap in xy_overlaps:
			tempWindows = SlidingWindowConfigurator.slideWindow(image=copied_image, x_start_stop=x_start_stop, y_start_stop=y_start_stop, 
					xy_window=(window_size, window_size), xy_overlap=(overlap, overlap))
			windows.extend(tempWindows)
			
		on_windows = [];
		print(windows)
		for window in windows:
			mpimg.imsave(output_folder + "/"+ file_parts[0] + "_before_resize.png", featureExtractor.image);
			test_image = cv2.resize(featureExtractor.image[window[0][1]:window[1][1], window[0][0]:window[1][0]], (64, 64))
			print(test_image.shape)
			mpimg.imsave(output_folder + "/"+ file_parts[0] + "_window.png", test_image);
			
			subFeatures = computePerImageFeatures(True, test_image, root_folder=None, y_start=None, y_stop=None, color_space='RGB', scale=1, orientations=9, pixels_per_cell=8, cells_per_block=2, feature_vec=False, concatenate_features=True)
		
			if scalar.mean_.shape[0] == subFeatures.shape[0]:
				#test_features = scalar.transform(np.array(subFeatures).reshape(1,-1))
				test_features = subFeatures
				prediction = clf.predict(test_features);
				if prediction == 1:
					on_windows.append(window)	
			else:
				print(scalar.mean_.shape[0])
				print(subFeatures.shape[0])
		
		copied_image = draw_boxes(copied_image, on_windows, box_colors[index], thick=6)
		all_windows.extend(on_windows)
		print(len(all_windows))
		index=index+1
		plt.imshow(copied_image) 
		
	if isImage == False:
		file_parts = file.split(".")
		mpimg.imsave(output_folder + "/" + file_parts[0] +"_boxes.png" , copied_image);
		
		heat = add_heat(heat, all_windows)		
		heat = apply_threshold(heat, threshold)
		''''
		heatmap_std = heat.std(ddof=1)
		if heatmap_std != 0.0:
			heat = (heat-heat.mean())/heatmap_std
		heat = apply_threshold(heat, np.max([heat.std(), 1]))    
		'''
		heatmap = np.clip(heat, 0, 255)
		mpimg.imsave(output_folder + "/" + file_parts[0] +"_heatmap.png" , heatmap);
		
		labels = label(heatmap)
		another_copy_image = draw_labeled_bboxes(another_copy_image, labels)
		
		mpimg.imsave(output_folder + "/" + file_parts[0] +"_final_box.png" , another_copy_image);
def process_image(image):
	global frame_count
	
	image = cc.undistort(image)
	mpimg.imsave("image_"+ str(frame_count) + ".png", image);
	frame_count += 1
	
	featureExtractor = FeatureExtractor(True, image, None)
	copied_image = np.copy(featureExtractor.image)
	another_copy_image = np.copy(featureExtractor.image)
	heat = np.zeros_like(copied_image[:,:,0]).astype(np.float)
	
	boxes_image = []
	all_windows = []
	index = 0
	for window_size in windows_sizes:
		windows = [];
		for overlap in xy_overlaps:
			tempWindows = SlidingWindowConfigurator.slideWindow(image=copied_image, x_start_stop=x_start_stop, y_start_stop=y_start_stop, 
					xy_window=(window_size, window_size), xy_overlap=(overlap, overlap))
			windows.extend(tempWindows)
			
		on_windows = [];
		for window in windows:
			test_image = cv2.resize(featureExtractor.image[window[0][1]:window[1][1], window[0][0]:window[1][0]], (64, 64))
			
			subFeatures = computePerImageFeatures(True, test_image, root_folder=None, y_start=None, y_stop=None, color_space='HLS', scale=1, orientations=9, pixels_per_cell=8, cells_per_block=2, feature_vec=False, concatenate_features=True)
		
			if scalar.mean_.shape[0] == subFeatures.shape[0]:
				test_features = scalar.transform(np.array(subFeatures).reshape(1,-1))
				prediction = clf.predict(test_features);
				if prediction == 1:
					on_windows.append(window)	
		copied_image = draw_boxes(copied_image, on_windows, color=box_colors[index], thick=6)
		all_windows.extend(on_windows)
		index+=1
	mpimg.imsave("sample_boxes.png" , copied_image);
	
	heat = add_heat(heat, all_windows)		
	heat = apply_threshold(heat, threshold)
	heatmap_std = heat.std(ddof=1)
	if heatmap_std != 0.0:
		heat = (heat-heat.mean())/heatmap_std
	heat = apply_threshold(heat, np.max([heat.std(), 1]))    
	
	heatmap = np.clip(heat, 0, 255)
	mpimg.imsave("sample_heatmap.png" , heatmap);
	
	labels = label(heatmap)
	another_copy_image = draw_labeled_bboxes(another_copy_image, labels)
	
	mpimg.imsave("sample_final_box.png" , another_copy_image);
	return another_copy_image
if __name__ == "__main__":
	#pickle_file_name = trainSVMFromData();
	import pickle
	pickle_file_name = "car_svc.p"
	data = pickle.load(open(pickle_file_name, "rb"));
	clf = data['Classifier'];
	scalar = data['Scalar'];
	
	pickle_file_name = "camera_calibration.p"
	data = pickle.load(open(pickle_file_name, "rb"));
	cc = data["CameraCalibrator"]
	
	x_start_stop=[700, 1296]
	y_start_stop = [400, 600]
	windows_sizes = [64];
	xy_overlaps = [0.5]
	box_colors = [(0,0, 255), (0,255,0), (255,0,0), (0, 255, 255), (255, 255, 0),(255,0,255), (0, 128, 255)];
	threshold = 4;
	frame_count = 0
	
	
	from VDetectApp.FeatureExtractor.FeatureExtractor import FeatureExtractor
	from VDetectApp.SlidingWindowConfigurator.SlidingWindowConfigurator import SlidingWindowConfigurator
	import cv2
	import os, sys
	import matplotlib.image as mpimg
	import matplotlib.pyplot as plt
	from scipy.ndimage.measurements import label
	
	''''
	from moviepy.editor import VideoFileClip
	import imageio
	imageio.plugins.ffmpeg.download()
	test_output = 'test_videos_output/project_video.mp4'
	clip2 = VideoFileClip('project_video.mp4').subclip(5, 10)
	pclip = clip2.fl_image(process_image)
	pclip.write_videofile(test_output, audio=False)
	'''
	
	# output_video_file = 'test_video_detected.mp4'
	# video_capture = cv2.VideoCapture("test_video.mp4")
	# success, image = video_capture.read()
	
	# count = 0
	# success = True
	# while success:
		# success, image = video_capture.read()
		# cv2.imwrite("frame%d.jpg" %count, image)
		# count+=1
	
	
	root_folder = "test_images";
	output_folder = "test_images_output"
	file_list = os.listdir(root_folder)
	for file in file_list:
		print(file)
		detect_vehicle(False, file, root_folder, output_folder)
	