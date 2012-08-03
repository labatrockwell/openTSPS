from tsps import tsps_connection

class tspsExample(tsps_connection):
	def __init__(self, address="localhost", port=7681):
		self.connect(address, port)

	def onPersonEntered(self, person):
		print "## PERSON ENTERED ##"

	def onPersonUpdated(self, person):
		print "## PERSON UPDATED ##"

	def onPersonLeft(self, person):
		print "## PERSON LEFT ##"

tspsExample()