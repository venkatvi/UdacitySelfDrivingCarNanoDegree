import numpy as np
class SlidingWindowConfigurator:
	def __init__(self, pixels_per_cell, cells_per_block, window_size, cells_per_step):
		self.pixels_per_cell = pixels_per_cell
		self.cells_per_block = cells_per_block
		self.window_size = window_size
		self.cells_per_step = 2
	def computeNumberOfBlocks(self, image):
		ydim = image.shape[0];
		xdim = image.shape[1];
		
		self.number_of_xblocks = xdim//self.pixels_per_cell - self.cells_per_block + 1;
		self.number_of_yblocks = ydim//self.pixels_per_cell - self.cells_per_block + 1;
	def computeNumberOfBlocksInWindow(self):
		self.number_of_blocks_per_window = self.window_size//self.pixels_per_cell - self.cells_per_block + 1;
	
	def computeSlidingWindowSteps(self):
		self.number_of_xsteps = (self.number_of_xblocks - self.number_of_blocks_per_window) // self.cells_per_step
		self.number_of_ysteps = (self.number_of_yblocks - self.number_of_blocks_per_window) // self.cells_per_step	
		
		
	def slideWindow(image, x_start_stop=[None, None], y_start_stop=[None,None], xy_window=(64,64), xy_overlap=(0.5,0.5)):
		if x_start_stop[0] == None:
			x_start_stop[0] = 0;
		if x_start_stop[1] == None:
			x_start_stop[1] = image.shape[1]
		if y_start_stop[0] == None:
			y_start_stop[0] = 0;
		if y_start_stop[1]  == None:
			y_start_stop[1] = image.shape[0];
		
		# Compute xy span
		xspan = x_start_stop[1] - x_start_stop[0];
		yspan = y_start_stop[1] - y_start_stop[0]; 
		
		# Compute number of pixels per step in x/y 
		nx_pixels_per_step = np.int(xy_window[0]*(1-xy_overlap[0]))
		ny_pixels_per_step = np.int(xy_window[1]*(1-xy_overlap[1]))
		
		# Compute number of windows in x/y
		nx_buffer = np.int(xy_window[0] * (xy_overlap[0]))
		ny_buffer = np.int(xy_window[1] * (xy_overlap[1]))
		
		nx_windows = np.int((xspan-nx_buffer)/nx_pixels_per_step)
		ny_windows = np.int((yspan-ny_buffer)/ny_pixels_per_step)
		
		window_list = []
		
		for ys in range(ny_windows):
			for xs in range(nx_windows):
				startx = xs *nx_pixels_per_step + x_start_stop[0]
				endx = startx + xy_window[0]
				starty = ys*ny_pixels_per_step + y_start_stop[0]
				endy = starty+xy_window[1]
				
				window_list.append(((startx, starty), (endx, endy)))
		
		return window_list