package classes{
	
	import flash.display.SimpleButton;
	import flash.text.TextField;
	import flash.events.MouseEvent;
	import flash.display.MovieClip;
	
	public class ErrorBox extends MovieClip{
		
		function ErrorBox(msg:String)
		{
			errorMsg.text = msg;
			
			btnOk.addEventListener(MouseEvent.CLICK, onPressOK);
		}
		
		private function onPressOK(evt:MouseEvent):void
		{
			parent.removeChild(this);
		}
		
	}
	
}