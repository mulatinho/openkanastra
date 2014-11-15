package classes{
	
	import flash.events.*;
	import flash.net.Socket;
	import flash.system.Security;
	
	public class KanastraSocket extends Socket{
		
		public static var socket:KanastraSocket;
		
		private var buffer:String;
		
		function KanastraSocket()
		{
			addEventListener(Event.CONNECT, connectHandler);
			addEventListener(IOErrorEvent.IO_ERROR, ioErrorHandler);
			addEventListener(SecurityErrorEvent.SECURITY_ERROR, securityErrorHandler);

			addEventListener(ProgressEvent.SOCKET_DATA, socketDataHandler);			
			//Security.allowDomain("www.infinitus-systems.com");
			Security.loadPolicyFile("xmlsocket://www.infinitus-systems.com:843");
			
			//super("infinitus-systems.com", 16666);
		}
		
		public static function getInstance():KanastraSocket
		{
			if (socket == null)
			{
				socket = new KanastraSocket();
			}
			
			return socket;
		}
		
		public function startConnection()
		{
			if (!connected)
				connect("www.infinitus-systems.com", 16666);
		}
		
		public function login(user:String, pass:String)
		{
			writeUTFBytes("AUTHUSER "+user+" "+pass+" \n");
			flush();
		}
		
		public function getRoomList()
		{
			writeUTFBytes("ROOMLIST \n");
			flush();
		}
		
		public function getRoomInfo(n:uint)
		{
			writeUTFBytes("ROOMLIST "+n+" \n");
			flush();
		}		
		
		public function createRoom(room:uint, pass:String)
		{
			writeUTFBytes("USERJOIN "+room+" "+pass+" \n");
			flush();
		}
		
		public function getCards()
		{
			writeUTFBytes("USERCARD \n");
			flush();		
		}
		
		public function getStackCard()
		{
			writeUTFBytes("GETNCARD 1 \n");
			flush();		
		}
		
		public function getTrashCard()
		{
			writeUTFBytes("GETNCARD 0 \n");
			flush();		
		}
		
		public function leaveCard(card:String)
		{
			writeUTFBytes("LEFTCARD "+card+" \n");
			flush();		
		}
		
		public function moveCards(cards:Array, group:uint)
		{
			var sendStr:String = "MOVEHAND "+cards[0];
			
			
			for (var i:uint = 1; i < cards.length; i++)
				sendStr += ","+cards[i];
				
			writeUTFBytes(sendStr+" "+group+" \n");
			flush();		
		}					
		
		private function connectHandler(evt:Event):void
		{
			dispatchEvent(new Event("CONNECTED"));
		}
		
		private function ioErrorHandler(evt:Event):void
		{
			trace("IOERROR!");
			dispatchEvent(new Event("IOERROR"));
		}
		
		private function securityErrorHandler(evt:Event):void
		{
			trace("SECERROR!");			
			dispatchEvent(new Event("SECERROR"));
		}		
		
			
		private function socketDataHandler(evt:ProgressEvent):void
		{
			buffer = socket.readUTFBytes(evt.bytesLoaded);
	
			trace("rcvd "+buffer);
			dispatchEvent(new Event("MSG_RECEIVED"));
		}		
		
		public function getMsg():String
		{
			return buffer;
		}
		
		
	};
	
};