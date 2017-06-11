print('Step 3: Extract features with Sliding Window')
	## Configuration for sliding window technique ## 
	pixels_per_cell=8;
	cells_per_block=2;
	window_size=64;
	cells_per_step=2;
	scale = 1
	color_space = 'HSV';
	orientations=9;
	
	import pickle
	data = pickle.load(open("car_svc.p", "rb"));
	clf = data['Classifier'];
	params = clf.get_params();
	print(clf.coef_)
	print(clf.intercept_)
	
	scalar = data['Scalar'];
	
	from VDetectApp.SlidingWindowConfigurator.SlidingWindowConfigurator import SlidingWindowConfigurator
	slidingWindow = SlidingWindowConfigurator(pixels_per_cell, cells_per_block, window_size, cells_per_step);
	
	
	# Ystart , YStop configuration
	y_start = 400
	y_stop = 656
	
	import cv2
	import os, sys
	import matplotlib.image as mpimg
	import matplotlib.pyplot as plt
	root_folder = "test_images";
	file_list = os.listdir(root_folder)
	for file in file_list:
		print(file)
		from VDetectApp.FeatureExtractor.FeatureExtractor import FeatureExtractor	
		featureExtractor = FeatureExtractor(file, root_folder)
		
		copied_image = np.copy(featureExtractor.image)
		print(copied_image.shape)
		
		if y_start is not None:
		#	print('#.1b: Crop Image');
			featureExtractor.cropImage(y_start, y_stop);
		
		#print('#.2: Apply Color Space');
		featureExtractor.applyColorSpace(color_space=color_space)
		
		#print('#.3: Rescale image');
		featureExtractor.rescaleImage(scale=scale)
		
		
		#print('#.6: HOG features');
		#eatureExtractor.extractHOGFeatures(hog_channel=0, orientations=orientations, pixels_per_cell=pixels_per_cell, cells_per_block=cells_per_block, visualise=False, feature_vec=False)
		#og1 = np.array(featureExtractor.hog_features);
		#featureExtractor.hog_features  = []
		
		hog1 = featureExtractor.getHogFeatures(featureExtractor.scaled_feature_image[:,:,0], orientations=orientations, pixel_per_cell=pixels_per_cell, cell_per_block=cells_per_block, visualise=False, feature_vec=False)
		hog2 = featureExtractor.getHogFeatures(featureExtractor.scaled_feature_image[:,:,1], orientations=orientations, pixel_per_cell=pixels_per_cell, cell_per_block=cells_per_block, visualise=False, feature_vec=False)
		hog3 = featureExtractor.getHogFeatures(featureExtractor.scaled_feature_image[:,:,2], orientations=orientations, pixel_per_cell=pixels_per_cell, cell_per_block=cells_per_block, visualise=False, feature_vec=False)
		print(hog1.shape)
		#hog2 = np.array(featureExtractor.hog_features);
		#featureExtractor.hog_features  = []
		#featureExtractor.extractHOGFeatures(hog_channel=2, orientations=orientations, pixels_per_cell=pixels_per_cell, cells_per_block=cells_per_block, visualise=False, feature_vec=False)
		#hog3 = np.array(featureExtractor.hog_features);
		#featureExtractor.hog_features  = []
		
		img = featureExtractor.image;
		#img = mpimg.imread(file);
		slidingWindow.computeNumberOfBlocks(featureExtractor.scaled_feature_image[:,:,0]);
		slidingWindow.computeNumberOfBlocksInWindow();
		slidingWindow.computeSlidingWindowSteps();
		
		## Pipeline 
		print('##Executing sliding window pipeline----------')
		print(slidingWindow.number_of_xsteps)
		print(slidingWindow.number_of_ysteps)
		
		for xb in range(slidingWindow.number_of_xsteps):
			for yb in range(slidingWindow.number_of_ysteps):
				ypos = yb * cells_per_step;
				xpos = xb * cells_per_step;
				
				#print('## Extract HOG for the image patch ----- ')
				#print (str(ypos) + '-' + str(xpos))
				hog1_feat = hog1[ypos:ypos+slidingWindow.number_of_blocks_per_window, xpos:xpos+slidingWindow.number_of_blocks_per_window].ravel();
				hog2_feat = hog2[ypos:ypos+slidingWindow.number_of_blocks_per_window, xpos:xpos+slidingWindow.number_of_blocks_per_window].ravel();
				hog3_feat = hog3[ypos:ypos+slidingWindow.number_of_blocks_per_window, xpos:xpos+slidingWindow.number_of_blocks_per_window].ravel();
				hog_features = np.hstack((hog1_feat, hog2_feat, hog3_feat))
				#print(hog_features.shape)
				
				xleft = xpos * pixels_per_cell
				ytop = ypos * pixels_per_cell
				
				# extract image patch
				sub_img = cv2.resize(featureExtractor.image[ytop:ytop+window_size,xleft:xleft+window_size], (window_size, window_size))
				#file_parts = file.split(".")
				#mpimg.imsave( file_parts[0] +"_" + str(xb) + "_" + str(yb) + "_windows.png" , sub_img);
				
				featureExtractorSub  = FeatureExtractor(file, root_folder);
				featureExtractorSub.setImage(sub_img);
				featureExtractorSub.applyColorSpace('RGB');
				featureExtractorSub.rescaleImage(1);
				featureExtractorSub.extractSpatialFeatures();
				featureExtractorSub.extractHistogramFeatures();
				
				#print(str(featureExtractorSub.spatial_features.shape) + "-" + str(featureExtractorSub.histogram_features.shape))
				all_features = np.hstack((featureExtractorSub.spatial_features, featureExtractorSub.histogram_features, hog_features)).reshape(1, -1)
				
				if scalar.mean_.shape[0] == all_features.shape[1]:
					test_features = scalar.transform(all_features)
					test_prediction = clf.predict(test_features)
					if test_prediction == 1:
						print("Car predicted")
						xbox_left = np.int(xleft * scale)
						ytop_draw = np.int(ytop*scale)
						win_draw =np.int(window_size*scale)
						print(str(xbox_left) + "-" + str(ytop_draw) + "-" + str(win_draw))
						cv2.rectangle(copied_image, (xbox_left, ytop_draw+y_start), (xbox_left+win_draw, ytop_draw + win_draw + y_start), (0, 0, 1), 6)
				else:
					print(all_features.shape)
					print(scalar.mean_.shape)
						
		
		plt.imshow(copied_image)
		file_parts = file.split(".")
		mpimg.imsave(file_parts[0] +"_windows_new.png" , copied_image);
		