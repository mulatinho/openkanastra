package classes
{
	import flash.display.MovieClip;
	import flash.events.Event;	
	import flash.events.EventDispatcher;
	import flash.events.MouseEvent;	
	import classes.KanastraSocket;
	
	
	public class RoomSelectScreen extends EventDispatcher
	{
		private var parent:Buraco;
		private var RoomAssets:RoomScreen;
		
		private var roomsNum:uint;
		private var selectedRoom:uint;
		
		private var RoomButtons:Array;
		
		function RoomSelectScreen(prt:Buraco)
		{
			parent = prt;
			
			RoomAssets = new RoomScreen();
			parent.addChild(RoomAssets);
			
			roomsNum = 1;
			
			RoomAssets.x = 0;
			RoomAssets.y = 0;	

			RoomAssets.roomWindow.visible = false;		
			
			RoomAssets.btnNewRoom.addEventListener(MouseEvent.CLICK, createRoom);
			
			RoomButtons = new Array();
			
			KanastraSocket.getInstance().addEventListener("MSG_RECEIVED", RoomsReceived);
			KanastraSocket.getInstance().getRoomList();
		}
		
		private function RoomsReceived(evt:Event):void
		{
			var msg:String = KanastraSocket.getInstance().getMsg();
			
			if (msg != "There are no rooms in this moment")
			{
				for (var i:uint = 0; i < msg.length; i++)
				{
					if (msg.charAt(i) == ":")
					{
						RoomButtons.push(new RoomButton());
						RoomButtons[roomsNum-1].x = 30 + (90 * (roomsNum-1));
						RoomButtons[roomsNum-1].y = 50;
						RoomButtons[roomsNum-1].num = roomsNum;
						RoomButtons[roomsNum-1].roomN.text = roomsNum;
						
						RoomAssets.addChild(RoomButtons[roomsNum-1]);
						
						RoomButtons[roomsNum-1].addEventListener(MouseEvent.CLICK, joinRoom);
						
						roomsNum++;
					}
				}
			}
			else
			{
				trace("No rooms");
			}
		}
		
		private function createRoom(evt:Event):void
		{
			RoomAssets.roomWindow.visible = true;
			selectedRoom = roomsNum;
			RoomAssets.roomWindow.btnSend.addEventListener(MouseEvent.CLICK, sendRoomInfo);
		}
		
		private function joinRoom(evt:Event):void
		{
			RoomAssets.roomWindow.visible = true;
			selectedRoom = evt.currentTarget.num;			
			RoomAssets.roomWindow.btnSend.addEventListener(MouseEvent.CLICK, sendRoomInfo);
			
			//KanastraSocket.getInstance().addEventListener("MSG_RECEIVED", onRoomCreated);
			//KanastraSocket.getInstance().createRoom(evt.target.num, RoomAssets.roomWindow.roomPass.text);
			
		}
		
		private function sendRoomInfo(evt:Event):void
		{
			trace("selectedRoom "+selectedRoom);
			KanastraSocket.getInstance().addEventListener("MSG_RECEIVED", onRoomCreated);
			KanastraSocket.getInstance().createRoom(selectedRoom, RoomAssets.roomWindow.roomPass.text);
			
			parent.setRoom(selectedRoom);
		}
		
		private function onRoomCreated(evt:Event):void
		{
			dispatchEvent(new Event("GAME_JOINED"));
			KanastraSocket.getInstance().removeEventListener("MSG_RECEIVED", onRoomCreated);
		}
		
		public function destroy()
		{
			for (var i:uint = 0; i < (roomsNum-1); i++)
				RoomAssets.removeChild(RoomButtons[i]);
				
			parent.removeChild(RoomAssets);
		}
	}
}