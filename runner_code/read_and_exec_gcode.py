DEFAULT_GCODE_FILE_NAME = 'test_gcode.gcode'


class GCode:

	gcode_fd = None
	def __init__(self, gcode_file_name=None) -> None:
		if not gcode_file_name:
			gcode_file_name = DEFAULT_GCODE_FILE_NAME
			
		self.gcode_fd = open(gcode_file_name, 'r')


	def fetch_gcode_line(self):
		if self.is_gcode_available():
			return self.gcode_fd.readline()
		else:
			return None
	
	def is_gcode_available(self) -> bool:
		current_file_location = self.gcode_fd.tell()
		if (self.gcode_fd.readline()):
			self.gcode_fd.seek(current_file_location)
			return True
		else:
			self.gcode_fd.seek(current_file_location)
			return False



x = GCode()
while (x.is_gcode_available()):
	print(x.fetch_gcode_line())