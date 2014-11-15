package classes{
	
	import flash.display.MovieClip;
	
	class Player{
		private var name:String;
		private var cards:Array;
		private var groups:Array;
		
		public function Player()
		{
			cards = new Array();
			groups = new Array();
		}
		
		public function addCard(card:String):void
		{
			cards.push(card);
		}
		
		public function getCards():Array
		{
			return cards;
		}
		
		public function removeCard(card:String)
		{
			
			for (var i:uint = 0; i < cards.length; i++)
			{
				if (cards[i] == card)
				{
					cards.splice(i, 1);
					return;
				}
				
			}
		}
		
		public function makeGroup(groupCards:Array)
		{
			groups.push(new Array());
			var N:uint = groups.length - 1;
			
			for (var i:uint = 0; i < cards.length; i++)
			{
				if (cards[i] == groupCards[i])
				{
					groups[N].push(groupCards[i]);
					cards.splice(i, 1);
				}
				
			}
		}
		
		public function remakeGroup(groupCards:Array, id:uint)
		{
			//groups.push(new Array());
			//var N:uint = groups.length - 1;
			
			for (var i:uint = 0; i < cards.length; i++)
			{
				if (cards[i] == groupCards[i])
				{
					groups[id].push(groupCards[i]);
					cards.splice(i, 1);
				}
				
			}
		}
		
		public function getNumGroups():int
		{
			return groups.length;
		}
		
		public function removeACard()
		{
			cards.pop();
		}
		
	};
	
};