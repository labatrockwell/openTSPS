from tsps import tsps_connection

class tspsExample(tsps_connection):
	def __init__(self, address="localhost", port=7681):
		self.connect(address, port)

	def onPersonEntered(self, person):
		pass;

	def onPersonUpdated(self, person):
		pass;

	def onPersonLeft(self, person):
		pass;

tspsExample("lab-mini-06.local")