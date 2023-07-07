class	Queue:
	def __init__(self):
		self.buf = []
		self.state = 0
		self.length = 0
		self.running = False

	def	start(self, buf):
		if self.running == False:
			self.running = True
			self.state = 0
			self.buf = buf
			self.length = len(buf)

	def	end(self):
		if self.state == self.length:
			self.state = 0
			self.length = 0
			self.running = False

	def	update(self, int):
		self.state = self.state + int

	def	append(self, element):
		if type(element) == list:
			for elt in element:
				self.buf.append(elt)
		else:
			self.buf.append(element)
	
	def	remove(self, element):
		self.buf.remove(element)

	def	pop(self, index):
		self.buf.pop(index)

	def	clear(self):
		self.buf = []
