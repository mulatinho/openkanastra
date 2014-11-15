package classes{
	
	import flash.display.MovieClip;
	import flash.events.Event;	
	import flash.events.EventDispatcher;
	import flash.events.MouseEvent;	
	import classes.KanastraSocket;
	import classes.LoginBox;
	
	public class LoginScreen extends EventDispatcher{

		private var errorWindow:ErrorBox;
		private var box:LoginWindow;		
		private var parent:Buraco;
		
		function LoginScreen(prt:Buraco)
		{
			parent = prt;
			
			box = new LoginWindow();
			
			box.x = 320;
			box.y = 240;
			parent.addChild(box);
			
			box.btnSend.addEventListener(MouseEvent.CLICK, onSendPress);
		}
		
		private function onSendPress(evt:MouseEvent):void
		{
			KanastraSocket.getInstance().addEventListener("MSG_RECEIVED", checkLogin);
			SystemLogin();
		}
		
		private function SystemLogin():void
		{
			KanastraSocket.getInstance().login(box.usertxt.text, box.passtxt.text);
		}
		
		private function checkLogin(evt:Event):void
		{
			var msg:String = KanastraSocket.getInstance().getMsg();
			
			trace("msg "+msg);
			
			if (msg.search("You are successfully identified") != -1)
			{
				dispatchEvent(new Event("USR_LOGGED"));
				parent.setPlayerName(box.usertxt.text);
				trace("playername"+parent.getPlayerName());
			}
			else
			{
				errorWindow = new ErrorBox("Invalid username or password!");
				
				errorWindow.x = 320;
				errorWindow.y = 240;
				parent.addChild(errorWindow);
			}
			
			KanastraSocket.getInstance().removeEventListener("MSG_RECEIVED", checkLogin);			
		}
		
		public function destroy()
		{
			parent.removeChild(box);
		}
	}
	
};