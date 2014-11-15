package classes{
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	public class Game{
		
		private const NUM_CARDS = 108;
		
		private var Table:OKTable;
		private var roomNum:uint;
		
		private var currentState:String;
		
		private var Players:Array;
		private var cards:Array;
		
		private var playerTurn:uint;
		
		private var maxPlayerCard:uint;
		
		private var parent:Buraco;
		
		private var Morto:MovieClip;
		private var tableCards:Array;
		
		private var Monte:MovieClip;
		private var Discart:discartBtn;
		private var Down:btnDown;
		
		private var cardsInitialized:Boolean;
		private var firstTurn:Boolean;
		
		private var msgBoard:MessageBoard;
		private var turnNotify:TurnNotifier;
		private var mcMesa:MesaMc;

		public function Game(prt:Buraco)
		{
			
			parent = prt;
			roomNum = parent.getCurrentRoom();
			
			
			mcMesa = new MesaMc();
			mcMesa.x = 260;
			mcMesa.y = 230;
			parent.addChild(mcMesa);						
			
			msgBoard = new MessageBoard();
			msgBoard.x = 260;
			msgBoard.y = 230;
			parent.addChild(msgBoard);
			msgBoard.visible = false;				
			
			KanastraSocket.getInstance().addEventListener("MSG_RECEIVED", onServerMsg);
			
			Players = new Array(2);
			
			turnNotify = new TurnNotifier();
			turnNotify.x = 666;
			turnNotify.y = 74;
			parent.addChild(turnNotify);
			
			cardsInitialized = false;
			firstTurn = false;
			trace("msg :"+parent.getPlayerName());			
		}
		
		private function onServerMsg(evt:Event)
		{
			var msg:String = KanastraSocket.getInstance().getMsg();
			
			var cardStart:uint;
				
			var cardi:int = 0;
			var cardn:int = 0;		
			
			if (msg.search("game will begin in five seconds") > -1)
			{
				msgBoard.visible = true;
				msgBoard.msg.text = msg;
				Players[0] = new Player();
				Players[1] = new Player();
				
				Table = new OKTable();
				parent.addChild(Table);
				
				KanastraSocket.getInstance().getCards();
			}
			else if (msg.search("its time to") > -1)
			{
					
				UpdateTable(msg);
				if (msg.search(parent.getPlayerName()) > -1)
				{
					startPlayerTurn();
					turnNotify.msg.text = "SUA VEZ DE JOGAR";
				}
				else
				{
					turnNotify.msg.text = "AGUARDE O ADVERSARIO JOGAR";
					endPlayerTurn();
				}
			}
			else if (!cardsInitialized && msg.search("Your cards") > -1)
			{
				trace("My cards");
				cardStart = msg.search("Your cards")+12;

				cardi = 0;
				cardn = 0;
				var i:uint = cardStart;
				while (msg.charAt(i) != "\n")
				{
					if (msg.charAt(i) != " ")
					{
						if (cardi == 0)
						{
							cardi = i;
							cardn++;
						}
						else
							cardn++;
					}
					else
					{
						if (cardi != 0)
						{
							trace("add card "+msg.substr(cardi, 2));
							Players[0].addCard(msg.substr(cardi, 2));
							cardi = 0;
							cardn = 0;
						}
					}
					
					i++;
				}
				
				cardsInitialized = true;
				
				setTable();
				
				if (firstTurn)
					startPlayerTurn();
			}
			else if (msg.search("make a new group of cards") > -1)
			{
				var cardsPos:uint = msg.search("cards");
				 
				while (msg.charAt(cardsPos) != "*") cardsPos++;
				
				trace("new group "+msg.substr(cardsPos));
				if (msg.search(parent.getPlayerName()) > -1)
				{
					Players[0].makeGroup(Table.getSelectedCards());
					Table.removeSelectedCards();
					Table.showGroup(msg.substr(cardsPos), 99, 0);
				}
				else
				{
					Table.showGroup(msg.substr(cardsPos), 99, 1);		
				}
					
			}
			else if (msg.search("added cards") > -1)
			{
				
				i = msg.search("group");
				
				while (msg.charAt(i) != "*") i++;
				i++;
				
				var gnum:uint = i;
				while (msg.charAt(gnum) != "*") gnum++;	
				
				gnum = gnum - i;

				gnum = uint(msg.substr(i, gnum));
				
				var cardsPos2:uint = msg.search("cards");
				while (msg.charAt(cardsPos2) != "*") cardsPos2++;
				
				i = cardsPos2+1;
				
				while (msg.charAt(i) != "*") i++;
				
				if (msg.search(parent.getPlayerName()) > -1)
				{
					Players[0].remakeGroup(Table.getSelectedCards(), gnum);
					Table.removeSelectedCards();
					Table.showGroup(msg.substr(cardsPos2, i-cardsPos2), gnum, 0);
				}
				else
				{
					Table.showGroup(msg.substr(cardsPos2, i-cardsPos2), gnum, 1);
				}
					
			}			
			else if (msg.search("you got a new card") > -1)
			{
				cardStart = msg.search("GAME")+8;
				
				cardi = 0;
				cardn = 0;
				i = cardStart;
				while (msg.charAt(i) != "*") i++;
				i++;
				cardi = i;
				cardn++;
				while (msg.charAt(i) != "*"){
					i++;
					cardn++;
				}
				
				trace("add card "+msg.substr(cardi, 2));
				Players[0].addCard(msg.substr(cardi, 2));
				Table.addCard(msg.substr(cardi, 2), 0);
			}
			else if (msg.search("got new cards") > -1)
			{
				Table.clearMorto(Morto);
				for (i = 0; i < tableCards.length; i++)
				{
					Players[0].addCard(tableCards[i]);
					Table.addCard(tableCards[i], 0);
				}
			}			
			
			else if (msg.search("to garbage.") > -1)
			{
				
				i = msg.length-1;
				while (msg.charAt(i) != "*") i--;
				i--;
				while (msg.charAt(i) != "*") i--;
				i++;
				
				var selCard:String = msg.substr(i, 2);
				
				if (msg.search(parent.getPlayerName()) > -1)
				{
					Players[0].removeCard(selCard);
					Table.addCardMorto(selCard, Morto);
					Table.removeCard(selCard, 0);
				}
				else
				{
					Players[1].removeACard();
					Table.addCardMorto(selCard, Morto);	
					Table.removeCard(selCard, 1);
				}
				
				KanastraSocket.getInstance().getCards();
				
			}
			else if (msg.search("DISCARDED") > -1)
			{
				UpdateTable(msg);
			}
			else if (msg.search("wins") > -1)
			{
				if (msg.search(parent.getPlayerName()) > -1)
					turnNotify.msg.text = "VOCE VENCEU!!";
				else
					turnNotify.msg.text = "VOCE PERDEU :(";				
			}
		}
		
		public function onGetCard(evt:Event)
		{
			KanastraSocket.getInstance().getStackCard();
		}
		
		public function onGetTrash(evt:Event)
		{
			KanastraSocket.getInstance().getTrashCard();
		}
		
		public function onDiscart(evt:Event)
		{
			var disCards:Array = Table.getSelectedCards();
			
			if (disCards.length == 1){
				KanastraSocket.getInstance().leaveCard(disCards[0]);
			}
		}		
		
		public function onMoveCards(evt:Event)
		{
			KanastraSocket.getInstance().moveCards(Table.getSelectedCards(), Players[0].getNumGroups());
		}	
		
		public function startPlayerTurn():void
		{
			if (Monte)
			{
				Monte.addEventListener(MouseEvent.CLICK, onGetCard);
				Morto.addEventListener(MouseEvent.CLICK, onGetTrash);
				Discart.addEventListener(MouseEvent.CLICK, onDiscart);
				Down.addEventListener(MouseEvent.CLICK, onMoveCards);
				
				Table.startCardButtons();
			}
			else
				firstTurn = true;
		}
		
		public function endPlayerTurn():void
		{
			if (Monte)
			{
				Monte.removeEventListener(MouseEvent.CLICK, onGetCard);
				Morto.removeEventListener(MouseEvent.CLICK, onGetTrash);
				Discart.removeEventListener(MouseEvent.CLICK, onDiscart);
				Down.removeEventListener(MouseEvent.CLICK, onMoveCards);
				
				Table.stopCardButtons();			
				Table.removeSelectedCards();			
			}
		}		
		
		
		public function setTable():void
		{
			Table.showCards(Players[0], 0);
			
			for (var i:uint = 0; i < 11; i++)
				Players[1].addCard("0");
			
			Table.showCards(Players[1], 1);
			
			Morto = new MovieClip();
			Morto.x = 220;
			Morto.y = 180;
			parent.addChild(Morto);
			
			Monte = new CardClip();
			Monte.x = 120;
			Monte.y = 180;
			Monte.gotoAndStop("back");
			parent.addChild(Monte);
			
			Discart = new discartBtn();
			Discart.x = 530;
			Discart.y = 450;
			parent.addChild(Discart);
			
			Down = new btnDown();
			Down.x = 635;
			Down.y = 450;
			parent.addChild(Down);					
			
			msgBoard.visible = false;
			mcMesa.setVisible = true;			
		}
		
		public function UpdateTable(msg:String):void
		{
			var pos:int = msg.search("DISCARDED");
			trace("update table: "+msg+" "+pos);
			
			if (pos == -1) return;
			
			Table.clearMorto(Morto);
			tableCards = new Array();
			
			pos += "DISCARDED".length + 1;
			
			trace("char at pos "+msg.charAt(pos));
			
			while (msg.charAt(pos) != "*")
			{
				if (msg.charAt(pos) != " " && msg.charAt(pos) != "\n")
				{
					
					Table.addCardMorto(msg.substr(pos, 2), Morto);
					tableCards.push(msg.substr(pos, 2));
					pos+=2;
				}
				else
					pos++;
			}
			
		}
		
	};
	
};