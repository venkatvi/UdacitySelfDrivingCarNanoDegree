import matplotlib.image as mpimg
import matplotlib.pyplot as plot 
import numpy as np 
import time
from VDetectApp.DataUtils.DataLoader import DataLoader
from VDetectApp.computeFeatures import *
from sklearn.svm import LinearSVC
from sklearn.preprocessing import StandardScaler
from sklearn.cross_validation import train_test_split
from sklearn.metrics import accuracy_score
import pickle
def trainSVMFromData():
	############################################################
	print('______________Step 1: Load data______________')
	# SubModules are not automatically imported from root import 
	
	dataLoader = DataLoader();
	dataLoader.loadData(); 

	car_images = dataLoader.car_images;
	non_car_images = dataLoader.non_car_images;

	print('There are ' + 
				str(len(car_images)) + ' images of cars and ' + 
				str(len(non_car_images)) + ' images of non-cars')

	############################################################
	print('______________Step 2: Extract features without Sliding Window______________')
	
	start_t = time.time();
	car_features = [];
	for car in car_images:
		car_features.append(computePerImageFeatures(False, car, concatenate_features=True));
		
	non_car_features=[];
	for non_car in non_car_images:
		non_car_features.append(computePerImageFeatures(False, non_car, concatenate_features=True));
	end_t = time.time();
	print('Feature Extraction took '+ str(round(end_t-start_t, 2)) + ' seconds');
	
	############################################################
	print('______________Step 3: Set up data for SVM______________')
	X = np.vstack((car_features, non_car_features)).astype('float64')
	
	print('Step 3a: Data Preprocessing - Normalization to Standard Scaling')
	
	X_scaler = StandardScaler().fit(X)
	scaled_X = X_scaler.transform(X)
	y = np.hstack((np.ones(len(car_features)), np.zeros(len(non_car_features))))
	
	print('______________Step 3b: Split data into training-test-validation set______________')
	rand_state = np.random.randint(0, 100)
	X_train, X_test, y_train, y_test = train_test_split(scaled_X, y, test_size=0.2, random_state = rand_state)
	
	############################################################
	print('______________Step 4: Train an SVM with grid search______________')
	start_t = time.time();
	
	clf = LinearSVC();
	#clf = GridSearchCV(svcClassifier, parameters, scoring='accuracy', verbose=10);
	clf.fit(X_train, y_train);
	end_t = time.time();
	print('Learning SVM took '+ str(round(end_t-start_t, 2)) + ' seconds');
	
	true_pred = clf.predict(X_test)
	
	acc = accuracy_score(true_pred, y_test)
	print(acc)
	
	data = {'Classifier': clf, 'Scalar':X_scaler};
	pickle_file_name = "car_svc.p";
	pickle.dump(data, open(pickle_file_name, "wb"))
	return pickle_file_name
	###########################################################