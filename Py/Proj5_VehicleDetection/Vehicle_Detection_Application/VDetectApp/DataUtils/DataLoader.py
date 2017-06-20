import glob

class DataLoader:
	def __init__(self, baseFolder=[None]):
		self.rootdir = baseFolder;
	def loadData(self):
		car_images_regex = "./data/sample_vehicles/*/*.png";
		car_images = glob.glob(car_images_regex);
		self.car_images = car_images;
		print(car_images)
		
		non_car_images_regex = "./data/sample_non-vehicles/*/*.png"
		non_car_images = glob.glob(non_car_images_regex)
		self.non_car_images = non_car_images;
		print(non_car_images)