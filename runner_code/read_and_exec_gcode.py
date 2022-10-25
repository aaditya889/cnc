from hashlib import new
import requests
import json
import time


# DEFAULT_GCODE_FILE_NAME = 'test_gcode.gcode'
DEFAULT_GCODE_FILE_NAME = 'pcb.gcode'
# DEFAULT_GCODE_FILE_NAME = 'squares.gcode'


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
idx = 0
while (x.is_gcode_available()):
	new_gcode_line = x.fetch_gcode_line().strip()
	if (not (new_gcode_line.startswith("G01") or new_gcode_line.startswith("G1") or new_gcode_line.startswith("G02") or new_gcode_line.startswith("G2") or new_gcode_line.startswith("G0") or new_gcode_line.startswith("G00"))):
		continue
	print(f"{idx}Trying to add gcode line: {new_gcode_line}")
	gcode_payload = {"gcode_line": new_gcode_line}
	req = requests.post(url="http://adinodemcu/push_gcode_line", data=gcode_payload)
	print(req.text)
	req.close()
	# print(req.headers)
	req.close
	time.sleep(0.5)
	idx += 1
	if idx == 500:
		time.sleep(60*2)
	# break
