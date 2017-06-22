import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data
import numpy as np
import matplotlib.pyplot as plt 

def load_data():
	mnist = input_data.read_data_sets("MNIST_data/", one_hot=True, reshape = False)
	training_data = mnist.train.images
	training_labels = np.asarray(mnist.train.labels, dtype = np.int32)
	print(training_data.shape)
	print(training_labels.shape)
	print (np.mean(training_labels))

	test_data = mnist.test.images
	test_labels = np.asarray(mnist.test.labels, dtype = np.int32)
	print("Printing test information .. . . . . . ")
	print(test_data.shape)
	print(test_labels.shape)
	return mnist

def getLeNet(input):

	mu =0
	sigma=0.1
	# Layer 1: 2DConv
	# Spec: Input image size: 32x32x1, Output image size: 28x28x6
	# Output nodes = 6
	# Filter size = 32-28+1 = 5x5x1
	# Padding = VALID (as same padding will return output size of 32x32x#output nodes)
	# Stride = 1
	
	# image properties
	image_width = 32
	image_height = 32
	color_channels = 1
	
	# output depth 
	layer1_output = 6
		
	# Layer 1: convolutional filter 
	layer1_filter_size_width = 5
	layer1_filter_size_height = 5
	
	
	# weights, biases 
	layer1_weight = tf.Variable(tf.truncated_normal([layer1_filter_size_height, layer1_filter_size_width, color_channels, layer1_output], mean=mu, stddev =sigma))
	layer1_bias = tf.Variable(tf.zeros(layer1_output))
	
	layer_1 =  tf.nn.conv2d(input, layer1_weight, strides=[1, 1, 1, 1], padding="VALID")
	layer_1 = tf.nn.bias_add(layer_1, layer1_bias)
	
	
	# Activation function 1 
	activation_layer_1 = tf.nn.relu(layer_1)
	
	# Pooling Layer 1 
	# Spec:  Input image size: 28x28x6, Output image size: 14x14x6
	# Output nodes: 6
	# Filter size = ksize = 2x2
	# Padding = "VALID"
	# Stride = 2
	pooling_layer1 = tf.nn.max_pool(activation_layer_1, ksize=[1,2,2,1], strides=[1,2,2,1], padding="VALID")
	
	
	# Layer 2: 2D Conv
	# Spec: Input image size = 14x14x6, Output image size: 10x10x16
	# Output nodes: 16
	# Filter size = 14-10+1 = 5x5x1
	# Padding = VALID
	# Stride = 1
	
	# output depth 
	layer2_output = 16
	
	layer2_channels = layer1_output
	
	# Layer 2: convolutional filter
	layer2_filter_size_width = 5
	layer2_filter_size_height = 5
	
	#input image 
	layer2_input = pooling_layer1
	
	# weights, biases 
	layer2_weights = tf.Variable(tf.truncated_normal([layer2_filter_size_height, layer2_filter_size_width, layer2_channels, layer2_output], mean=mu, stddev=sigma))
	layer2_bias = tf.Variable(tf.zeros(layer2_output))
	
	layer2 = tf.nn.conv2d(layer2_input, layer2_weights, strides = [1, 1, 1, 1], padding = "VALID")
	layer2 = tf.nn.bias_add(layer2, layer2_bias)
	
	# Activation function
	activation_layer_2 = tf.nn.relu(layer2)
	
	# Pooling layer 2 
	# Specs: Input image size: 10x10x16, Output image size: 5x5x16
	# Output nodes: 16
	# Filter size = ksize = 2x2
	# Padding = "VALID"
	# Stride = 2
	pooling_layer2 = tf.nn.max_pool(activation_layer_2, ksize=[1,2,2,1], strides=[1,2,2,1], padding="VALID")
	
	# Layer 3: Flatten layer
	# flattens 5x5x16 to 400x1
	flatten_layer = tf.contrib.layers.flatten(pooling_layer2)
	
	# Layer 4: Fully connected layer
	# Spec: Input size: 400, Output = 120
	# Output nodes: 120
	fc_layer1_weights=tf.Variable(tf.truncated_normal([400, 120], mean=mu, stddev=sigma))
	fc_layer1_biases = tf.Variable(tf.zeros(120))
	fully_connected_layer1 = tf.matmul(flatten_layer, fc_layer1_weights) + fc_layer1_biases
	fully_connected_layer1 = tf.nn.relu(fully_connected_layer1)
	
	# Magic Layer: Drop out 
	#fully_connected_layer1 = tf.nn.dropout(fully_connected_layer1, keep_prob)
	
	# Layer 5: Fully connected layer
	# Spec: Input size: 120, Output: 84
	# Output nodes: 84
	fc_layer2_weights=tf.Variable(tf.truncated_normal([120, 84], mean=mu, stddev=sigma))
	fc_layer2_biases = tf.Variable(tf.zeros(84))
	fully_connected_layer2 = tf.matmul(fully_connected_layer1, fc_layer2_weights) + fc_layer2_biases
	fully_connected_layer2 = tf.nn.relu(fully_connected_layer2)
	
	# Layer 6: Fully connected layer
	# Spec: Input size: 84, Output: 10
	# Output nodes: 10
	fc_layer3_weights=tf.Variable(tf.truncated_normal([84, 10], mean=mu, stddev=sigma))
	fc_layer3_biases = tf.Variable(tf.zeros(10))
	fully_connected_layer3 = tf.matmul(fully_connected_layer2, fc_layer3_weights) + fc_layer3_biases
	#fully_connected_layer3 = tf.nn.relu(fully_connected_layer3)
	return fully_connected_layer3
	
def evaluate(X_data, y_data):
	num_examples = len(X_data)
	total_accuracy = 0
	sess = tf.get_default_session()
	for offset in range(0, num_examples, batch_size):
		end = offset + batch_size
		batchx, batchy = X_data[offset:end], y_data[offset:end]
		accuracy = sess.run(accuracy_operation, feed_dict={x:batchx, y:batchy})
		total_accuracy += (accuracy*len(batchx))
	return total_accuracy/num_examples
	
if __name__ == "__main__":
	mnist = load_data()
	X_train, y_train = mnist.train.images, mnist.train.labels
	X_test, y_test = mnist.test.images, mnist.test.labels
	X_validation, y_validation = mnist.validation.images, mnist.validation.labels
	
	print(y_train.shape)
	# pad input images of 28x28x1 to make it 32x32x1
	X_train = np.pad(X_train, ((0,0), (2,2), (2,2), (0,0)), 'constant')
	X_validation = np.pad(X_validation, ((0,0), (2,2), (2,2), (0,0)), 'constant')
	X_test = np.pad(X_test, ((0,0), (2,2), (2,2), (0,0)), 'constant')
	
	indices = np.arange(X_train.shape[0])
	np.random.shuffle(indices)
	X_train, y_train = X_train[indices, :,:,:], y_train[indices]
	
		
	# Standard Scalar? 

	'''
	Data is a 55000x 28x 28x 1 input 
	First layer of cnn has 32 filters each of 5x5x1 size 
	Second layer of cnn has 64 filters each of 5x5x32 size 
	Third layer is a fully connected layer 
	'''
	#Parameters
	learning_rate = 0.001
	epochs = 10
	batch_size = 128

	# Number of sampels to calculate validation and accuracy 
	test_valid_size = 256

	# Network parameters 
	n_classes = 10 # total number of classes from 0-9 digits 
	dropout = 0.75 # ( for max pooling ) ?? 

	'''
	# Store layers, weights and biases 
	weights = {
    'wc1': tf.Variable(tf.random_normal([5, 5, 1, 32])),
    'wc2': tf.Variable(tf.random_normal([5, 5, 32, 64])),
    'wd1': tf.Variable(tf.random_normal([7*7*64, 1024])),
    'out': tf.Variable(tf.random_normal([1024, n_classes]))}

	biases = {
    'bc1': tf.Variable(tf.random_normal([32])),
    'bc2': tf.Variable(tf.random_normal([64])),
    'bd1': tf.Variable(tf.random_normal([1024])),
    'out': tf.Variable(tf.random_normal([n_classes]))}
	'''
	# image properties
	image_width = 32
	image_height = 32
	color_channels = 1
	
	# initial features and labels 
	x = tf.placeholder(tf.float32, shape=(None, image_height, image_width, color_channels))
	y = tf.placeholder(tf.int32, (None))
	
	
	# Model
	leNet =  getLeNet(x)
	
	# define loss and optimizer
	cross_entropy = tf.nn.softmax_cross_entropy_with_logits(logits=leNet, labels=y)
	loss_operation = tf.reduce_mean(cross_entropy)
	optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate)
	training_operation = optimizer.minimize(loss_operation)
	
	# Prediction
	correct_prediction = tf.equal(tf.argmax(leNet, 1), tf.argmax(y, 1))
	accuracy_operation = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
	
	saver = tf.train.Saver()
	# train model 
	with tf.Session() as sess:
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
			
			validation_accuracy = evaluate(X_validation, y_validation)
			print("EPOCH {} ...".format(i+1))
			print("Validation Accuracy = " , validation_accuracy)
			print()
	
		saver.save(sess, './lenet')
		print("Model Saved")
	