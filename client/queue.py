class	Queue:
	def __init__(self):
		self.buf = []
		self.state = 0
		self.length = 0
		self.running = False

	def	start(self, buf = []):
		if self.running == False:
			self.running = True
			self.state = 0
			self.buf = buf
			self.length = len(buf)

	def	end(self):
		if self.running == False or self.state == self.length + 1:
			self.state = 0
			self.buf = []
			self.length = 0
			self.running = False

	def	get_state_elt(self):
		return self.buf[self.state] if self.state < self.length else None

	def	update(self, int):
		self.state = self.state + int

	def	append_one(self, element):
		self.buf.append(element)
		self.length = self.length + 1

	def	append(self, element):
		if type(element) == list:
			for elt in element:
				self.append_one(elt)
		else:
			self.append_one(element)
	
	def	remove(self, element):
		self.buf.remove(element)
		self.length = self.length - 1

	def	reset(self):
		self.buf = []
		self.length = 0
