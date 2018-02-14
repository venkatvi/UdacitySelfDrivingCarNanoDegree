import tensorflow as tf 
import numpy as np
import random

'''
Tensor Flow constants
'''
hello_constant  	= tf.constant('Hello World!')
integer_constant 	= tf.constant(1234)
pi_constant 		= tf.constant(np.pi)
float_constant 		= tf.constant(random.random())
with tf.Session() as sess:
	output = sess.run(hello_constant)
	print(output)
	output = sess.run(integer_constant)
	print(output)
	output = sess.run(pi_constant)
	print(output)
	output = sess.run(float_constant)
	print(output)
	output = sess.run(float_constant)
	print(output)

'''
Tensor flow placeholders
'''
x 					= tf.placeholder(tf.string)
y   				= tf.placeholder(tf.string)

with tf.Session() as sess:
	output = sess.run(x,  feed_dict={x: 'Hello there ', y: 'hi there '})
	print(output)
	output = sess.run(y,  feed_dict={x: 'Hello there ', y: 'hi there '})
	print(output)