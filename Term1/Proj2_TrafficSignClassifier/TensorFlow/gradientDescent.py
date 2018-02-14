import numpy as np
import tensorflow as tf

# Assign model parameters
W = tf.Variable([0.3], tf.float32)
b = tf.Variable([-0.3], tf.float32)

# Model input and output 
x = tf.placeholder(tf.float32)
linear_model  = W*x + b
y = tf.placeholder(tf.float32)

# Loss 
loss = tf.reduce_sum(tf.square(linear_model - y))

# Optimizer
optimizer = tf.train.GradientDescentOptimizer(0.01)
train = optimizer.minimize(loss)

# training data
x_train = [1,2, 3,4];
y_train = [0, -1, -2, -3]

# training loop 
init = tf.global_variables_initializer();
sess = tf.Session()
sess.run(init)
for i in range(10000):
	sess.run(train, {x: x_train, y: y_train})

# evaluating training accuracy
x_test = [ 5, 6, 7, 8]
y_test = [-4, -5, -6, -7]
print(sess.run(loss, {x: x_test, y: y_test}))
print(sess.run([W, b]))

file_writer = tf.summary.FileWriter('tblogs', sess.graph)