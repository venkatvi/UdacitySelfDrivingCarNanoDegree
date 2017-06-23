#Load pickled data
import pickle

import numpy as np

# for lenet architecture
import tensorflow as tf
from tensorflow.contrib.layers import flatten

def getLeNet(x):
	mu = 0
	sigma = 0.1
	
	conv1_W = tf.Variable(tf.truncated_normal(shape=(5,5,3,6), mean=mu, stddev=sigma))
	conv1_b = tf.Variable(tf.zeros(6))
	conv1 = tf.nn.conv2d(x, conv1_W, strides=[1,1,1,1], padding="VALID") +  conv1_b
	conv1 = tf.nn.relu(conv1)
	conv1 = tf.nn.max_pool(conv1, ksize=[1,2,2,1], strides=[1,2,2,1], padding="VALID")
	
	conv2_W = tf.Variable(tf.truncated_normal(shape=(5,5,6,16), mean=mu, stddev=sigma))
	conv2_b = tf.Variable(tf.zeros(16))
	conv2 = tf.nn.conv2d(conv1, conv2_W, strides=[1,1,1,1], padding="VALID") + conv2_b
	conv2 = tf.nn.relu(conv2)
	conv2 = tf.nn.max_pool(conv2, ksize=[1,2,2,1], strides=[1,2,2,1], padding="VALID")
	
	fc0   = flatten(conv2)
	
	fc1_W = tf.Variable(tf.truncated_normal(shape=(400, 120), mean=mu, stddev=sigma))
	fc1_b = tf.Variable(tf.zeros(120))
	fc1 = tf.matmul(fc0, fc1_W) + fc1_b
	fc1 = tf.nn.relu(fc1)
	
	fc2_W = tf.Variable(tf.truncated_normal(shape=(120, 84), mean=mu, stddev=sigma))
	fc2_b = tf.Variable(tf.zeros(84))
	fc2 = tf.matmul(fc1, fc2_W) + fc2_b
	fc2 = tf.nn.relu(fc2)
	
	fc3_W = tf.Variable(tf.truncated_normal(shape=(84, 43), mean=mu, stddev=sigma))
	fc3_b = tf.Variable(tf.zeros(43))
	logits = tf.matmul(fc2, fc3_W) + fc3_b
	return logits
	
def loadData():
	training_file = "../traffic-signs-data/train.p"
	validation_file="../traffic-signs-data/valid.p"
	testing_file = "../traffic-signs-data/test.p"

	with open(training_file, mode='rb') as f:
		train = pickle.load(f)
	with open(validation_file, mode='rb') as f:
		valid = pickle.load(f)
	with open(testing_file, mode='rb') as f:
		test = pickle.load(f)
		
	X_train, y_train = train['features'], train['labels']
	#y_train = np.resize(y_train, (X_train.shape[0], 1))
	X_valid, y_valid = valid['features'], valid['labels']
	#y_valid = np.resize(y_valid, (X_valid.shape[0], 1))
	X_test, y_test = test['features'], test['labels']
	#y_test = np.resize(y_test, (X_test.shape[0], 1))
	data = {'train': (X_train, y_train), 'test': (X_test , y_test), 'valid': (X_valid, y_valid)}
	return data
def preprocessData(data):
	training_data = data["train"]
	test_data = data["test"]
	valid_data = data["valid"]
	
	# Preprocessing step 1: Shuffle training + validation data to create a uniform distribution of data across classes
	# in the training and validation sets
	X_train = training_data[0]
	y_train = training_data[1]
	X_valid = valid_data[0]
	y_valid = valid_data[1]
	X = np.vstack((X_train, X_valid))
	y = np.vstack((y_train[:,], y_valid[:,]))

	indices = np.arange(X.shape[0])
	np.random.shuffle(indices)
	training_samples = int(np.ceil(X.shape[0]*0.8))
	training_indices, validation_indices = indices[:training_samples], indices[training_samples:]
	X_train, y_train = X[training_indices, :], y[training_indices, :]
	X_valid, y_valid = X[validation_indices, :], y[validation_indices, :]
	data["train"] = (X_train, y_train)
	data["valid"] = (X_valid, y_valid)
	return data
def evaluate(X_data, y_data, dropout):
	num_examples = len(X_data)
	total_accuracy = 0
	sess = tf.get_default_session()
	for offset in range(0, num_examples, batch_size):
		end = offset + batch_size
		batchx, batchy = X_data[offset:end], y_data[offset:end]
		accuracy = sess.run(accuracy_operation, feed_dict={x:batchx, y:batchy})
		#accuracy = 0
		#one_hot_y_valid = sess.run(one_hot_y,  feed_dict={y:batchy})
		#print(one_hot_y_valid.shape)
		total_accuracy += (accuracy*len(batchx))
	return total_accuracy/num_examples
def train_and_validate(data):

	training_data = data["train"]
	validation_data = data["valid"]
	test_data = data["test"]
	X_train = training_data[0]
	y_train = training_data[1]
	X_valid = validation_data[0]
	y_valid = validation_data[1]
	X_test = test_data[0]
	y_test = test_data[1]
	
	saver = tf.train.Saver()
	
	with tf.Session() as sess:
		file_writer = tf.summary.FileWriter('.', sess.graph)
		sess.run(tf.global_variables_initializer())
		num_examples = len(X_train)
		
		for i in range(epochs):
			indices = np.arange(X_train.shape[0])
			np.random.shuffle(indices)
			X_train, y_train = X_train[indices, :,:,:], y_train[indices]
			
			for offset in range(0, num_examples, batch_size):
				end = offset + batch_size
				batchx, batchy = X_train[offset:end], y_train[offset:end]
				sess.run(training_operation, feed_dict={x:batchx, y:batchy})
	
			validation_accuracy = evaluate(X_valid, y_valid, dropout)
			print("EPOCH {} ...".format(i+1))
			print("Validation Accuracy = " , validation_accuracy)
			print()

		saver.save(sess, './lenet')
		print("Model Saved")

		# Calculate Test Accuracy
		#test_accuracy = evaluate(X_test, y_test, dropout)
		#print('Testing Accuracy: {}'.format(test_accuracy))
			
				
	
if __name__ == "__main__":
	data = loadData();
	#preprocessed_data = preprocessData(data);
	preprocessed_data = data
	
	learning_rate=0.001
	dropout = 0.75
	epochs = 10
	batch_size=128
	
	x = tf.placeholder(tf.float32, (None, 32,32,3))
	y = tf.placeholder(tf.int32, (None))
	one_hot_y = tf.one_hot(y, 43)
	print(one_hot_y.get_shape)
	keep_prob = tf.placeholder(tf.float32, (None))
	
	leNet = getLeNet(x)
	print(leNet.get_shape)
	cross_entropy = tf.nn.softmax_cross_entropy_with_logits(logits=leNet, labels=one_hot_y)
	loss_operation = tf.reduce_mean(cross_entropy)
	optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate)
	training_operation = optimizer.minimize(loss_operation)
	
	correct_prediction = tf.equal(tf.argmax(leNet, 1), tf.argmax(one_hot_y, 1))
	accuracy_operation = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
	
	train_and_validate(preprocessed_data)