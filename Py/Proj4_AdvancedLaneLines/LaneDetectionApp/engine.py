from CameraCaliberator import *
import pickle
if __name__ == "__main__":
	cc = CameraCaliberator("../camera_cal","../camera_cal_output", (9,6))
	cc.caliberate();
	
	data = {'CameraCalibrator': cc};
	pickle_file_name = "camera_calibration.p";
	pickle.dump(data, open(pickle_file_name, "wb"))