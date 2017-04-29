import tensorflow as tf 
import numpy as np 

# features 
# labels 
# mode = ENUM: {train, evaluation, prediction}
def model(features, labels, mode):
	print(features['x'].shape[1])
	
	# build a linear mode and predict values
	W = tf.get_variable("W", [features['x'].shape[1]], dtype=tf.float64)
	b = tf.get_variable("b", [1], dtype=tf.float64)
	dotProduct = tf.reduce_sum(tf.multiply(W, tf.cast(features['x'], tf.float64)))
	y = dotProduct + b
	
	# Loss sub-graph 
	loss = tf.reduce_sum(tf.square(y-labels))

	# training sub-grph
	global_step = tf.train.get_global_step()
	optimizer = tf.train.GradientDescentOptimizer(0.01)
	train = tf.group(optimizer.minimize(loss), tf.assign_add(global_step, 1))

	# ModelFnOps connects subgraphs 
	return tf.contrib.learn.ModelFnOps(mode = mode, predictions = y, loss = loss, train_op = train)
	

if __name__ == "__main__":
	# Every custom model requires, features, labels and mode ? 
	estimator  = tf.contrib.learn.Estimator(model_fn=model)
	x = np.array([[1, 2], [2, 3], [3, 4], [4, 5]])
	y = np.array([0, -1, -2, -3])
	input_fn = tf.contrib.learn.io.numpy_input_fn({"x": x}, y, batch_size=4, num_epochs=100)

	estimator.fit(input_fn=input_fn, steps=1000)
	print(estimator.evaluate(input_fn=input_fn, steps=10))

	sess = tf.Session();
	sess.run(estimator);

	file_writer = tf.summary.FileWriter('tblogs', sess.graph)