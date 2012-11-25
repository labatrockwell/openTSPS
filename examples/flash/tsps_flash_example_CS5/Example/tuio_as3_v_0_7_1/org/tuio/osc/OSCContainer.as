package org.tuio.osc {
	
	public class OSCContainer {
		
		private var children:Array;
		public var name:String;
		public var method:IOSCListener;
		public var parent:OSCContainer;
		
		public function OSCContainer(name:String, method:IOSCListener = null){
			this.name = name;
			this.method = method;
		}
		
		public function addChild(child:OSCContainer):void {
			this.children[child.name] = child;
			child.parent = this;
		}
		
		public function getChild(name:String):OSCContainer {
			return this.children[name];
		}
		
		public function getMatchingChildren(pattern:String):Array {
			var out:Array = new Array();
			
			var firstSeperator:int = pattern.indexOf("/");
			var part:String = pattern.substring(0, firstSeperator);
			var rest:String = pattern.substring(firstSeperator + 1, pattern.length); 
			var done:Boolean = (pattern.indexOf("/")==-1);
			
			for each(var child:OSCContainer in this.children) {
				
				if (child.matchName(part)) {
					if (done) {
						if(child.method != null) out.push(child.method);
					} else {
						out = out.concat(child.getMatchingChildren(rest));
					}
				}
				
			}
			
			return out;
		}
		
		public function removeChild(child:OSCContainer):void {
			if (child.hasChildren) child.method = null;
			else this.children[child.name] = null;
		}
		
		public function matchName(pattern:String):Boolean {
			
			if (pattern == this.name) return true;
			
			if (pattern == "*") return true;
			
			//convert address patter to regular expression
			var regExStr:String = "";
			for (var c:uint = 0; c < pattern.length; c++) {
				switch(pattern.charAt(c)) {
					case "{": regExStr += "(" ; break;
					case "}": regExStr += ")" ; break;
					case ",": regExStr += "|" ; break;
					case "*": regExStr += ".*" ; break;
					case "?": regExStr += ".+" ; break;
					default: regExStr += pattern.charAt(c); break;
				}
			}
			
			var regEx:RegExp = new RegExp(regExStr, "g");
			
			if (regEx.test(this.name) && regEx.lastIndex == this.name.length) return true; 
			
			return false;
			
		}
		
		public function get hasChildren():Boolean {
			
			return (children.length > 0);
			
		}
		
	}
	
}