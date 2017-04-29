from tensorflow.examples.tutorials.mnist import input_data
import numpy as np
import matplotlib.pyplot as plt 

def load_data():
	mnist = input_data.read_data_sets(".", one_hot=True, reshape = False)
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

if __name__ == "__main__":
	mnist = load_data()

	import tensorflow as tensorflow

	'''
	Data is a 55000x 28x 28x 1 input 
	First layer of cnn has 32 filters each of 5x5x1 size 
	Second layer of cnn has 64 filters each of 5x5x32 size 
	Third layer is a fully connected layer 
	'''
	#Parameters
	learning_rate = 0.00001
	epochs = 10
	batch_size = 128

	# Number of sampels to calculate validation and accuracy 
	test_valid_size = 256

	# Network parameters 
	n_classes = 10 # total number of classes from 0-9 digits 
	dropout = 0.75 # ( for max pooling ) ?? 

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