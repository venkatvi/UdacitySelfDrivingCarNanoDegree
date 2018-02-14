import matplotlib.pyplot as plt 
import matplotlib.image as mpimg
import numpy as np 
import cv2

def detectLines(image):
	low_threshold = 1
	high_threshold = 10
	edges = cv2.Canny(image, low_threshold, high_threshold)
	return edges
if __name__ == '__main__':
	# read image and convert to gray scale 
	image = mpimg.imread('exit-ramp.jpg')
	gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)

	# Optional Step: define a kernel size for gaussian smoothing/ blurring
	kernel_size = 3
	blur_gray = cv2.GaussianBlur(gray, (kernel_size, kernel_size), 0)

	edges = detectLines(blur_gray)

	#Plot edges
	plt.imshow(edges, cmap='Greys_r')



	cmake -D PYTHON_EXECUTABLE=$WORKON_HOME/bin/python \
 -D PYTHON_PACKAGES_PATH=$WORKON_HOME/lib/python2.7/site-packages \
 -D INSTALL_PYTHON_EXAMPLES=ON \
 -D PYTHON_INCLUDE_DIR=/usr/local/Cellar/python/2.7.9/Frameworks/Python.framework/Headers \
 -D PYTHON_LIBRARY=/Library/Python/2.7/
..
make -j8
make install

