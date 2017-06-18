from CameraCaliberator import *
if __name__ == "__main__":
	cc = CameraCaliberator("../camera_cal","../camera_cal_output", (9,6))
	cc.caliberate();
	
	