# NOTE: You don't need to edit this code.
import time
import tensorflow as tf
import numpy as np
#from scipy.misc import imread
from caffe_classes import class_names
from alexnet import AlexNet
import matplotlib.image as mpimg
import matplotlib.pyplot as plt

# placeholders
x = tf.placeholder(tf.float32, (None, 227, 227, 3))

# By keeping `feature_extract` set to `False`
# we indicate to keep the 1000 class final layer
# originally used to train on ImageNet.
probs = AlexNet(x, feature_extract=False)
init = tf.global_variables_initializer()
sess = tf.Session()
sess.run(init)

# Read Images
im1 = mpimg.imread("poodle.png")
im1 = (im1[:, :, :3]).astype(np.float32)
#im1 = im1 - np.mean(im1)
mpimg.imsave("poodle2.png", im1);

#im1 = im1 - np.mean(im1)

im2 = mpimg.imread("weasel.png")
im2 = (im2[:, :, :3]).astype(np.float32)
mpimg.imsave("weasel2.png", im2);
#im2 = im2 - np.mean(im2)

# Run Inference
t = time.time()
output = sess.run(probs, feed_dict={x: [im1, im2]})

# Print Output
for input_im_ind in range(output.shape[0]):
    inds = np.argsort(output)[input_im_ind, :]
    print("Image", input_im_ind)
    for i in range(5):
        print("%s: %.3f" % (class_names[inds[-1 - i]], output[input_im_ind, inds[-1 - i]]))
    print()

print("Time: %.3f seconds" % (time.time() - t))
