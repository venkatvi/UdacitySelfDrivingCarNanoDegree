# Project utils / APIs 
from CameraCaliberator import *
from LaneDetectionPipeline import *

# for load /saving cache
import pickle

# for file APIs
import os, sys 
# for video to image utils 
from moviepy.editor import VideoFileClip
import imageio
def undistort_sample_images(camera_caliberator, root_folder, output_folder):
	# Loop through all images in the root_folders
	undistorted_output_folder = output_folder
	file_list = os.listdir(root_folder)
	for file in file_list:
		print("Undistorting for "+ file)
		
		# read image
		image = cv2.imread(root_folder+"/"+ file)
			
		# Use camera caliberator to undistort image
		undistorted_image = camera_caliberator.undistort(image);
		
		# save image in the output_folder 
		file_parts  = file.split(".")
		mpimg.imsave(undistorted_output_folder + "/" + file_parts[0] +"_undistorted.png" , undistorted_image);
if __name__ == "__main__":
	#cc = CameraCaliberator("../camera_cal","../camera_cal_output", (9,6))
	#cc.caliberate();
	#undistort_sample_images(cc, "../camera_cal", "../camera_cal_undistorted")
	
	#data = {'CameraCalibrator': cc};
	#pickle_file_name = "camera_calibration.p";
	#pickle.dump(data, open(pickle_file_name, "wb"))
	
	pickle_file_name = "camera_calibration.p"
	data = pickle.load(open(pickle_file_name, "rb"));
	cc = data["CameraCalibrator"]

	'''
	root_folder = "../test_images"
	output_folder = "../test_images_output"
	ldp = LaneDetectionPipeline(cc, root_folder, output_folder);
	ldp.execute()
	'''
	
	imageio.plugins.ffmpeg.download()
	test_output = '../test_videos_output/project_video.mp4'
	clip2 = VideoFileClip('../project_video.mp4')
	
	output_folder = "../test_videos_output"
	ldp = LaneDetectionPipeline(cc, ".", output_folder);
	pclip = clip2.fl_image(ldp.process_image).subclip(0, 5)
	pclip.write_videofile(test_output, audio=False)
	