import tensorflow as tf 
import numpy as np

tf.logging.set_verbosity(tf.logging.INFO)
# tf.contrib.layers.* creates variables
features  = [tf.contrib.layers.real_valued_column("x", dimension=2)]
estimator = tf.contrib.learn.LinearRegressor(feature_columns=features)

x = np.array([[1, 2], [2, 3],[3, 4],[4,5]])
y = np.array([0., -1, -2, -3])

input_fn = tf.contrib.learn.io.numpy_input_fn({"x":x}, y, batch_size=1, num_epochs=1000)

estimator.fit(input_fn=input_fn, steps =100)

print(estimator.evaluate(input_fn=input_fn))
