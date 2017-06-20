import matplotlib.image as mpimg
import cv2
import os
car = "test_image.png"
cv_image=cv2.imread(car)
flipped_image=cv_image.copy()	
flipped_image=cv2.flip(cv_image, 1)
file_parts = os.path.split(car)
file_name_parts = file_parts[1].split(".")
flipped_file_name = file_parts[0] + "_" + file_name_parts[0] + "_flipped.png";
mpimg.imsave(flipped_file_name, flipped_image)

print(cv_image.shape)
print(flipped_image.shape)
