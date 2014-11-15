package classes{

	import flash.display.MovieClip;
	import flash.events.MouseEvent;

	class OKTable extends MovieClip {

		private var mcCards:Array;
		private var tableCards:Array;
		private var trash:Array;

		private var selectedCards:Array;

		private var mcTableCards:CardClip;
		private var groups:Array;
		private var groupsMc:Array;

		function OKTable() {
			mcCards = new Array(2);
			
			groups = new Array(2);
			groups[0] = new Array();
			groups[1] = new Array();
			
			groupsMc = new Array(2);
			
			groupsMc[0] = new Array();
			groupsMc[1] = new Array();	
			
			trash = new Array();

		}
		
		private function getCardNumber(x:String):String {
			switch (x.charAt(0)) {
				case "T" :
					return "10";
				default :
					return x.charAt(0);
			}
		}
		
		private function getCardSuit(x:String):String {
			if (x.charAt(1) == "1" || x.charAt(1) == "2") {
				return "j";
			}
			return x.charAt(1).toLowerCase();


		}
		
		public function showCards(player:Player, id:uint) {
			var i:uint;
			var pCards:Array = player.getCards();

			mcCards[id] = new Array();

			if (id == 0) {
				for (i = 0; i < pCards.length; i++) {
					mcCards[id][i] = new CardClip();
					mcCards[id][i].gotoAndStop("front");
					mcCards[id][i].n.text = getCardNumber(pCards[i]);
					mcCards[id][i].s.gotoAndStop(getCardSuit(pCards[i]));

					addChild(mcCards[id][i]);

					mcCards[id][i].x = 100+(20*i);
					mcCards[id][i].y = 500;

					mcCards[id][i].cardIdx = pCards[i];
				}
			} else {
				for (i = 0; i < pCards.length; i++) {
					mcCards[id][i] = new CardClip();
					mcCards[id][i].gotoAndStop("back");

					addChild(mcCards[id][i]);

					mcCards[id][i].x = 100+(20*i);
					mcCards[id][i].y = 20;
				}
			}
			selectedCards = new Array();
		}
		
		public function startCardButtons() {
			trace("startCards " +mcCards[0].length)
			for (var i:uint = 0; i < mcCards[0].length; i++) {
				mcCards[0][i].addEventListener(MouseEvent.CLICK, onClickCard);
			}

		}
		
		public function removeCard(card:String, id:uint) {
			var size:uint = mcCards[id].length;

			if (id == 0) {
				for (var i:uint = 0; i < mcCards[id].length; i++) {
					if (mcCards[id][i].cardIdx == card) {
						removeChild(mcCards[id][i]);
						mcCards[id].splice(i, 1);
						return;
					}
				}
			} else {
				removeChild(mcCards[id][mcCards[id].length-1]);
				mcCards[id].pop();
			}
		}
		
		public function addCard(card:String, id:uint) {
			var size:uint = mcCards[id].length;
			var newCard:CardClip;

			if (id == 0) {
				newCard = new CardClip();
				newCard.gotoAndStop("front");
				newCard.n.text = getCardNumber(card);
				newCard.s.gotoAndStop(getCardSuit(card));

				addChild(newCard);

				newCard.x = 100+(20*size);
				newCard.y = 500;

				newCard.cardIdx = card;
				newCard.addEventListener(MouseEvent.CLICK, onClickCard);
				
				updatePlayerCards();
			} else {
				newCard = new CardClip();
				newCard.gotoAndStop("back");

				addChild(newCard);

				newCard.x = 100+(20*size);
				newCard.y = 20;
			}
			mcCards[id].push(newCard);
		}
		
		public function addCardMorto(card:String, Morto:MovieClip) {
			var newCard:CardClip = new CardClip();
			newCard.gotoAndStop("front");
			newCard.n.text = getCardNumber(card);
			newCard.s.gotoAndStop(getCardSuit(card));
			newCard.cardIdx = card;

			if (trash.length < 8)
			{
				newCard.y = 4;
				newCard.x = 20*trash.length;				
			}
			else
			{
				newCard.y = 42;
				newCard.x = 20*(trash.length - 8);				
			}
			
			Morto.addChild(newCard);
			
			trash.push(newCard);
		}
		
		public function clearMorto(Morto:MovieClip)
		{
			for (var i:uint = 0; i < trash.length; i++)
				Morto.removeChild(trash[i]);
				
			trash = new Array();
		}
		
		public function stopCardButtons() {

			for (var i:uint = 0; i < mcCards[0].length; i++) {
				mcCards[0][i].removeEventListener(MouseEvent.CLICK, onClickCard);
			}

		}
		
		private function onClickCard(evt:MouseEvent):void {
			
			if (MovieClip(evt.currentTarget ).y == 500) {
				selectedCards.push(evt.currentTarget );
				MovieClip(evt.currentTarget ).y -= 20;
			} else {
				for (var i:uint = 0; i < selectedCards.length; i++) {
					if (selectedCards[i].cardIdx == evt.currentTarget .cardIdx) {
						selectedCards.splice(i, 1);
						MovieClip(evt.currentTarget ).y += 20;
						break;
					}
				}
			}
		}
		
		public function getSelectedCards():Array {
			var selected:Array = new Array();

			for (var i:uint = 0; i < selectedCards.length; i++) {
				selected.push(selectedCards[i].cardIdx);
			}

			return selected;
		}
		
		public function removeSelectedCards() {
			for (var i:uint = 0; i < selectedCards.length; i++) {
				for (var j:uint = 0; j < mcCards[0].length; j++) {
					if (selectedCards[i].cardIdx == mcCards[0][j].cardIdx) {
						removeChild(mcCards[0][j]);
						mcCards[0].splice(j, 1);
						break;
					}
				}
			}
			selectedCards = new Array();
		}
		
		public function showGroup(msg:String, group:uint, player:uint) {
			var pos:int = msg.search(/\*[0-9\,A-Za-z]+\*\./) + 1;
			var cards:Array = msg.substr(pos).split(/[\,\*]/);

			var tempCards:Array = new Array();

			var i:uint;
			for (i = 0; i < cards.length; i++) {
				if (cards[i].length > 1 && !(/[\.\*]+/.test(cards[i])) )
				{
					trace("adding "+cards[i]);
					var newCard = new CardClip();
					newCard.gotoAndStop("front");
					newCard.n.text = getCardNumber(cards[i]);
					newCard.s.gotoAndStop(getCardSuit(cards[i]));

					newCard.cardIdx = cards[i];

					tempCards.push(newCard);
				}
			}
			
			var tempCan = new Canastra();
			tempCan.x = (groupsMc[player].length * 166) + 10;

			if (player == 0)
				tempCan.y = 350;
			else
				tempCan.y = 120;			
			
			if (group == 99) 
			{
				group = groups[player].length;
				groups[player].push(tempCards);
				groupsMc[player].push(tempCan);				
			}
			else
			{
				for (i = 0; i < groups[player][group].length; i++)
				{
					 
					 groupsMc[player][group].removeChild(groups[player][group][i]);
				}
				
				groups[player][group] = tempCards;
				removeChild(groupsMc[player][group]);
			}
			
				
			tempCan.id = group;
			
			
			for (i = 0; i < groups[player][group].length; i++)
			{
				 groups[player][group][i].x = i * 20;
				 groups[player][group][i].y = 0;
				 groups[player][group][i].width = 29;
				 groups[player][group][i].height = 41;
				 
				 groupsMc[player][group].addChild(groups[player][group][i]);
			}
			
			if (player == 0)
				groupsMc[player][group].addEventListener(MouseEvent.CLICK, onClickCan);

			
			addChild(groupsMc[player][group]);
			
		}
		
		public function addToGroup(msg:String, group:uint, player:uint) {
			var pos:int = msg.search(/\*[0-9\,A-Za-z]+\*\./) + 1;
			var cards:Array = msg.substr(pos).split(/[\,\*]/);

			var currentSize:uint = groups[player][group].length;
			
			var i:uint;
			for (i = 0; i < cards.length; i++) {
				if (cards[i].length > 1 && !(/[\.\*]+/.test(cards[i])) )
				{
					trace("adding "+cards[i]);
					var newCard = new CardClip();
					newCard.gotoAndStop("front");
					newCard.n.text = getCardNumber(cards[i]);
					newCard.s.gotoAndStop(getCardSuit(cards[i]));

					newCard.cardIdx = cards[i];

					groups[player][group].push(newCard);
				}
			}
			
			
			for (i = 0; i < groups[player][group].length; i++)
			{
				 groups[player][group][i].x = i * 20;
				 groups[player][group][i].y = 0;			 
				 groups[player][group][i].width = 29;
				 groups[player][group][i].height = 41;
				 
				 if (i >= currentSize)
					 groupsMc[player][group].addChild(groups[player][group][i]);
				 
				 
			}
		
			
		}		
		
		
		private function onClickCan(evt:MouseEvent)
		{
			if (selectedCards.length > 0)
				KanastraSocket.getInstance().moveCards(getSelectedCards(), evt.currentTarget.id);
		}
		
		public function updatePlayerCards()
		{
			for (var i:uint = 0; i < mcCards[0].length; i++)
			{
				mcCards[0][i].x = 100+(20*i);
			}
		}
	}

}