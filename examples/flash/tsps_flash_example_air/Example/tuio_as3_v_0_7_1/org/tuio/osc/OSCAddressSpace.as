package org.tuio.osc {
	
	public class OSCAddressSpace {
		
		private var root:OSCContainer;
		
		public function OSCAddressSpace() {
			this.root = new OSCContainer("");
		}
		
		public function addMethod(address:String, method:IOSCListener):void {
			var parts:Array = address.split("/");
			var part:String;
			var currentNode:OSCContainer = root;
			var nextNode:OSCContainer;
			while (parts.length > 0) {
				part = parts.pop();
				nextNode = currentNode.getChild(part);
				if (nextNode == null) {
					nextNode = new OSCContainer(part);
					currentNode.addChild(nextNode);
				}
				currentNode = nextNode;
			}
			currentNode.method = method;
		}
		
		public function removeMethod(address:String):void {
			var parts:Array = address.split("/");
			var part:String;
			var currentNode:OSCContainer = root;
			var nextNode:OSCContainer;
			while (parts.length > 0) {
				part = parts.pop();
				nextNode = currentNode.getChild(part);
				if (nextNode == null) {
					break;
				}
				currentNode = nextNode;
			}
			currentNode.parent.removeChild(currentNode);
		}
		
		public function getMethods(pattern:String):Array {
			return root.getMatchingChildren(pattern.substr(1, pattern.length));
		}
		
	}
	
}