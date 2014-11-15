package classes{
	
	import flash.display.MovieClip;
	import flash.text.TextField;
	import flash.events.Event;
	import flash.events.MouseEvent;	
	import classes.KanastraSocket;	
	
	public class Buraco extends MovieClip{
		
		private var currentState:String;
		private var Login:LoginScreen;
		private var RoomSelect:RoomSelectScreen;
		
		private var GameScreen:Game;
		
		private var currentRoom:uint;
		private var playerName:String;
		
		private var logShowing:Boolean;
		
		public function Buraco()
		{
			currentState = "PLAYING";
			
			KanastraSocket.getInstance().addEventListener("CONNECTED", onConnect);
			KanastraSocket.getInstance().addEventListener("IOERROR", onError);			
			KanastraSocket.getInstance().addEventListener("SECERROR", onSecError);
			KanastraSocket.getInstance().addEventListener("MSG_RECEIVED", onMsgRcvd);
			
			logShowing = false;
			btnShowLog.addEventListener(MouseEvent.CLICK, showLog);
			
			KanastraSocket.getInstance().startConnection();
		}
	
		private function showLog(evt:Event):void
		{
			if (logShowing)
				serverLog.y = -231;
			else
				serverLog.y = 20;
			
			logShowing = !logShowing;
		}		
	
		private function onError(evt:Event):void
		{

		}	
		
		private function onSecError(evt:Event):void
		{

		}	
		
		private function onMsgRcvd(evt:Event):void
		{
			serverLog.text += KanastraSocket.getInstance().getMsg();
		}			
	
		private function onConnect(evt:Event):void
		{
			startLoginScreen();
		}
		
		private function startLoginScreen():void
		{
			Login = new LoginScreen(this);
			
			Login.addEventListener("USR_LOGGED", onUserLogged);
		}
		
		private function onUserLogged(evt:Event):void
		{
			Login.destroy();
			
			RoomSelect = new RoomSelectScreen(this);
			
			RoomSelect.addEventListener("GAME_JOINED", onGameJoined);
		}
		
		private function onGameJoined(evt:Event):void
		{
			RoomSelect.destroy();
			GameScreen = new Game(this);
		}
		
		public function setRoom(room:uint)
		{
			currentRoom = room;
		}
		
		public function getCurrentRoom():uint
		{
			return currentRoom;
		}		
		
		public function setPlayerName(name:String)
		{
			playerName = name;
		}
		
		public function getPlayerName():String
		{
			return playerName;
		}			
		
	};
	
};