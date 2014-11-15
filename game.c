#include "buraco.h"

char *BARALHO[] = {
	"8H", "9H", "TH", "JH", "QH", "KH", "AH", "2H", "3H", "4H", "5H", "6H", "7H",
	"2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC", "JC", "QC", "KC", "AC",
	"2h", "3h", "4h", "5h", "Qh", "Kh", "Ah", "6h", "7h", "8h", "9h", "Th", "Jh", 
	"2c", "3c", "4c", "5c", "6c", "7c", "8c", "9c", "Tc", "Jc", "Qc", "Kc", "Ac",
	"2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD", "JD", "QD", "KD", "AD",
	"2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS", "JS", "QS", "KS", "AS",
	"2d", "3d", "4d", "5d", "6d", "7d", "8d", "9d", "Td", "Jd", "Qd", "Kd", "Ad",
	"2s", "3s", "4s", "5s", "6s", "7s", "8s", "9s", "Ts", "Js", "Qs", "Ks", "As",
	"C1", "C2", "c1", "c2"
};

int
buraco_sort_cards(char brl[])
{
	int i, x, z;
	char baralho[BURACO_CARDS] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0
	};

	srand(time(NULL));
	for (i=0;i<BURACO_CARDS;i++)
	{
		loop:
		x = 0;
		x = (rand() % BURACO_CARDS);

		for (z=0; z<BURACO_CARDS-1; z++)
		{
			if (baralho[z] == x)
				goto loop;
		}

		baralho[i] = x;
		brl[i] = baralho[i];
	}

	x = (rand() % BURACO_CARDS);
	i = brl[x];
	brl[x] = brl[BURACO_CARDS-1];
	brl[BURACO_CARDS-1] = i;


	return 0;
}

int
buraco_count_user_cards(struct ROOM *table, int userid)
{
	struct ROOM_idx *ridx;
	int i, ret;

	ret = 0;
	ridx = buraco_mem_get_room(table->roomid);


	if (ridx->users[0] == userid)
	{
		for (i=0; table->in_game.cards_p1[i]!=-1; i++)
			ret++;
	}
	else if (ridx->users[1] == userid)
	{
		for (i=0; table->in_game.cards_p2[i]!=-1; i++)
			ret++;
	}
	else if (ridx->users[2] == userid)
	{
		for (i=0; table->in_game.cards_p3[i]!=-1; i++)
			ret++;
	}
	else if (ridx->users[3] == userid)
	{
		for (i=0; table->in_game.cards_p4[i]!=-1; i++)
			ret++;
	}

	return ret;
}

int
buraco_count_garbage(struct ROOM *table)
{
	int i, ret;

	ret = 0;

	for (i=0; table->in_game.garbage[i]!=-1; i++)
		ret++;

	return ret;
}

int
buraco_get_card_value(struct ROOM *table, char *ch)
{
	int i=0, pos=0;

	if (table)
	{
		for (i=0;i<BURACO_CARDS;i++)
		{
			pos = table->in_game.baralho[i];
			if (!strncmp(BARALHO[pos], ch, strlen(ch)))
				return i;
		}
	}

	return -1;
}

int
buraco_verify_user_card(struct ROOM *table, int userid, char *ch)
{
	struct ROOM_idx *ridx;
	int i, x, card;

	ridx = buraco_mem_get_room(table->roomid);
	if (!ridx)
		return -1;

	x = 0; i = 0; card = 0;
	if ((i = buraco_get_card_value(table, ch)) < 0)
		return -1;

	card = table->in_game.baralho[i];
	x = buraco_count_user_cards(table, userid);

	if (userid == ridx->users[0])
	{
		for (i=0; i<x; i++)
		{
			if (table->in_game.cards_p1[i] == card)
				return 0;
		}
	}
	else if (userid == ridx->users[1])
	{
		for (i=0; i<x; i++)
		{
			if (table->in_game.cards_p2[i] == card)
				return 0;
		}
	}
	else if (userid == ridx->users[2])
	{
		for (i=0; i<x; i++)
		{
			if (table->in_game.cards_p3[i] == card)
				return 0;
		}
	}
	else if (userid == ridx->users[3])
	{
		for (i=0; i<x; i++)
		{
			if (table->in_game.cards_p4[i] == card)
				return 0;
		}
	}

	return -1;
}

int buraco_remove_card_from_user(struct ROOM *table, int userid, char *card)
{
	struct USER_idx *uidx;
	struct ROOM_idx *ridx;
	char ret[SZ_STRING_MID];
	int i, x, z, tcard;

	i = 0; x = -1; z = 0; tcard = 0;

	ridx = buraco_mem_get_room(table->roomid);
	if (ridx)
	{
		i = buraco_get_card_value(table, card);
		tcard = table->in_game.baralho[i];
//		fprintf(stdout, "buraco_remove_cards_from_user() init 1, tcard: %d\n", tcard);

		if (ridx->users[0] == userid)
		{
			z = buraco_count_user_cards(table, userid);
			for (i=0;i<z;i++)
			{
				if (table->in_game.cards_p1[i] == tcard)
					x = i;
			}	
		}
		if (ridx->users[1] == userid)
		{
			z = buraco_count_user_cards(table, userid);
			for (i=0;i<z;i++)
			{
				if (table->in_game.cards_p2[i] == tcard)
					x = i;
			}	
		}
		if (ridx->users[2] == userid)
		{
			z = buraco_count_user_cards(table, userid);
			for (i=0;i<z;i++)
			{
				if (table->in_game.cards_p3[i] == tcard)
					x = i;
			}	
		}
		if (ridx->users[3] == userid)
		{
			z = buraco_count_user_cards(table, userid);
			for (i=0;i<z;i++)
			{
				if (table->in_game.cards_p4[i] == tcard)
					x = i;
			}	
		}

		if (x==-1)
		{
			snprintf(ret, sizeof(ret)-1, "*ERROR* You dont have this card *%s* in your hand.\n\0", card);
			send(userid, ret, strlen(ret), 0);

			return -1;
		} 
		else
		{
			i = buraco_count_user_cards(table, userid);
			if (ridx->users[0] == userid)
			{
				for (z=x; z<i; z++)
					table->in_game.cards_p1[z] = table->in_game.cards_p1[z+1];
			}
			if (ridx->users[1] == userid)
			{
				for (z=x; z<i; z++)
					table->in_game.cards_p2[z] = table->in_game.cards_p2[z+1];
			}
			if (ridx->users[2] == userid)
			{
				for (z=x; z<i; z++)
					table->in_game.cards_p3[z] = table->in_game.cards_p3[z+1];
			}
			if (ridx->users[3] == userid)
			{
				for (z=x; z<i; z++)
					table->in_game.cards_p4[z] = table->in_game.cards_p4[z+1];
			}
		}
		
	}

	//FIXME
	//Verificar se as cartas do usuario acabaram, se acabaram pega o morto
	//
	if (!buraco_count_user_cards(table, userid))
	{
		if (!ridx->dead_cards)
		{
			for (x=0; table->in_game.room_dead_one[x]!=-1; x++)
			{
				if (ridx->users[0] == userid)
					table->in_game.cards_p1[x] = table->in_game.room_dead_one[x];
				else if (ridx->users[1] == userid)
					table->in_game.cards_p2[x] = table->in_game.room_dead_one[x];
				else if (ridx->users[2] == userid)
					table->in_game.cards_p3[x] = table->in_game.room_dead_one[x];
				else if (ridx->users[3] == userid)
					table->in_game.cards_p4[x] = table->in_game.room_dead_one[x];
			}
			ridx->dead_cards = 1;
		}
		else if (ridx->dead_cards == 1)
		{
			for (x=0; table->in_game.room_dead_two[x]!=-1; x++)
			{
				if (ridx->users[0] == userid)
					table->in_game.cards_p1[x] = table->in_game.room_dead_two[x];
				else if (ridx->users[1] == userid)
					table->in_game.cards_p2[x] = table->in_game.room_dead_two[x];
				else if (ridx->users[2] == userid)
					table->in_game.cards_p3[x] = table->in_game.room_dead_two[x];
				else if (ridx->users[3] == userid)
					table->in_game.cards_p4[x] = table->in_game.room_dead_two[x];
			}
			ridx->dead_cards = 2;
		}
		else
		{
			uidx = buraco_mem_get_user(userid);
			snprintf(ret, sizeof(ret)-1, "*GAME ROOM* user *%s* wins the game!\n\0", uidx->nick);
			buraco_message_to_room(ridx->id, ret);			

			return 0;
		}
	}


	return 0;
}

int
buraco_return_pos_gcard(char ch)
{
	int ret=14;

	if (ch=='C' || ch=='c')
		ret=14;
	if (ch=='2')
		ret=14;
	if (ch=='3')
		ret=2;
	if (ch=='4')
		ret=3;
	if (ch=='5')
		ret=4;
	if (ch=='6')
		ret=5;
	if (ch=='7')
		ret=6;
	if (ch=='8')
		ret=7;
	if (ch=='9')
		ret=8;
	if (ch=='T')
		ret=9;
	if (ch=='J')
		ret=10;
	if (ch=='Q')
		ret=11;
	if (ch=='K')
		ret=12;

	return ret;
}

int
buraco_move_cards_to_group(struct ROOM *table, int userid, char *rcards, int group, int game_cards)
{
	struct CARDS *cardsidx, *t;
	struct USER_idx *uidx;
	int aces, joker;
	int i, x, j, card, exists, verify;
	char *cards[SZ_NAIPE];
	char gcards[SZ_STRING_MID], ret[SZ_STRING_MAX];

	fprintf(stdout, "buraco_move_cards_to_group() init group:%d\n");
	if (!rcards)
		return -1;

	exists = 0; card = 0; i = 0; x = 0;

	cards[0] = (char *)strkey(rcards, 0, ','); cards[1] = (char *)strkey(rcards, 1, ','); 
	cards[2] = (char *)strkey(rcards, 2, ','); cards[3] = (char *)strkey(rcards, 3, ','); 
	cards[4] = (char *)strkey(rcards, 4, ','); cards[5] = (char *)strkey(rcards, 5, ','); 
	cards[6] = (char *)strkey(rcards, 6, ','); cards[7] = (char *)strkey(rcards, 7, ','); 
	cards[8] = (char *)strkey(rcards, 8, ','); cards[9] = (char *)strkey(rcards, 9, ','); 
	cards[10] = (char *)strkey(rcards, 10, ','); cards[11] = (char *)strkey(rcards, 11, ',');
	cards[12] = (char *)strkey(rcards, 12, ','); cards[13] = (char *)strkey(rcards, 13, ',');
	cards[14] = (char *)strkey(rcards, 14, ','); cards[15] = (char *)strkey(rcards, 15, ',');

	// Algo
	//
	// Process cards
	//   Count one per one
	//     Verify if they had the same suit
	//   Update Points
	//   Move cards
	//
	// REMOVE CARD FROM USER

	verify = 0; verify = tolower(cards[0][1]);
	cardsidx = NULL;
	cardsidx = buraco_mem_get_gcards(table->roomid, group, game_cards);

	if (cardsidx==NULL)
	{
		buraco_mem_add_gcards(table->roomid, group, game_cards); usleep(9);

		if (!cards[2])
		{
			snprintf(ret, sizeof(ret)-1, "*ERROR* You need to move at least three cards of your hand.\n\0");
			send(userid, ret, strlen(ret), 0);

			return -1;
		}

		cardsidx = NULL;
		cardsidx = buraco_mem_get_gcards(table->roomid, group, game_cards);
		if (cardsidx)
			cardsidx->suit = verify;
	}
	else
	{
		if (cardsidx->id == game_cards) 
			exists++;
	}

        t = cfst;
        while (t)
        {
                fprintf(stdout, "\033[1mcards--debug: roomid:%d->%d group:%d->%d id:%d->%d suit:%.3d:%c\n\033[m", t->roomid, table->roomid, t->group, group, t->id, game_cards, t->suit, t->suit);
                t = t->next;
        }


	i = 0; aces = 0; joker = 0;
	while (cards[i] != NULL && cards[i][0]!=0)
	{
		verify = 0;
		verify = tolower((int)cards[i][1]);

		if (cards[i][0] == 'A')
			aces++;
		if (cards[i][0] == '2' || cards[i][0] == 'c' || cards[i][0] == 'C')
		{
			fprintf(stdout, "2 ou coringa na area\n");
			joker++;
		}

		if (cardsidx->suit != verify)
		{
			if (!joker)
			{
				buraco_mem_del_gcards(table->roomid, group, game_cards);
				snprintf(ret, sizeof(ret)-1, "*ERROR* These cards doesn't have the same suit.\n\0");
				send(userid, ret, strlen(ret), 0);
				return -1;
			}
		}

		if (buraco_verify_user_card(table, userid, cards[i]) != 0)
		{
			buraco_mem_del_gcards(table->roomid, group, game_cards);
			snprintf(ret, sizeof(ret)-1, "*ERROR* You don't have this card *%s*.\n\0", cards[i]);
			send(userid, ret, strlen(ret), 0);
			return -1;
		}

		i++;
	}

	if (joker>=3 || aces>=2)
	{
		buraco_mem_del_gcards(table->roomid, group, game_cards);
		snprintf(ret, sizeof(ret)-1, "*ERROR* Too many 'joker' cards in game.\n\0");
		send(userid, ret, strlen(ret), 0);
		return -1;
	}
	fprintf(stdout, "buraco_move_cards_to_group() ...1\n");

	i = 0;
	while (cards[i]!=NULL && cards[i][0]!=0)
	{
		x = -1;

		if (cards[i][0] == 'A')
		{
			if (cardsidx->cards[13] == -1 && cardsidx->cards[12] != -1)
				x=13;
			else if (cardsidx->cards[0] == -1)
				x=0;
		}
		else if ((cards[i][0] == '2' || cards[i][0] == 'c' || cards[i][0] == 'C') && joker!=0)
			x=14;

		if (x==-1)
			x = buraco_return_pos_gcard(cards[i][0]);
		
	fprintf(stdout, "buraco_move_cards_to_group() ...111111\n");
		if (!cards[1])
		{			
			fprintf(stdout, "APENAS UMAAAAAAAAAAAAAAA x-1:%d x+1:%d\n", cardsidx->cards[x-1], cardsidx->cards[x+1]);

			if (x==0 && cardsidx->cards[1] != -1)
				return -1;	
			else if (cardsidx->cards[x-1] == -1 && cardsidx->cards[x+1] == -1 && x!=14)
				return -1;
			else if (x==13 && cardsidx->cards[x-1] != -1)
				return -1;

	fprintf(stdout, "buraco_move_cards_to_group() ...1222222  x:%d cardsidx->cards[x]:%d\n", x, cardsidx->cards[x]);
			if (cardsidx->cards[x]!=-1 && (cardsidx->cards[x]=='2' || cardsidx->cards[x]=='c' || cardsidx->cards[x]=='C'))
			{
				fprintf(stdout, "cardsidx->cards[%d] = %d", x , cardsidx->cards[x]);	
				if (cardsidx->cards[13]==-1)
					for (j=x; cardsidx->cards[j]!=-1; j++);
				else
					for (j=x; cardsidx->cards[j]!=-1; j--);
				
				fprintf(stdout, "cardsidx->cards[%d] = %d", j, cardsidx->cards[x]);	
				cardsidx->cards[j] = cardsidx->cards[x];
			}
			else if (cardsidx->cards[x]!=-1)
			{
				fprintf(stdout, "cardsidx->cards[%d] = %d", x , cardsidx->cards[x]);	
				if (cardsidx->cards[13]==-1)
					for (j=x; cardsidx->cards[j]!=-1; j++);
				else
					for (j=x; cardsidx->cards[j]!=-1; j--);
				
				fprintf(stdout, "cardsidx->cards[%d] = %d", j, cardsidx->cards[x]);	
				cardsidx->cards[j] = cardsidx->cards[x];
			}
	fprintf(stdout, "buraco_move_cards_to_group() ...133333\n");
		}
		
		card = buraco_get_card_value(table, cards[i]);
		cardsidx->cards[x] = table->in_game.baralho[card];
		i++;
	}

	fprintf(stdout, "buraco_move_cards_to_group() ...2\n");
	x=0; verify = 0; j=0;
	for (i=0;i<SZ_NAIPE;i++)
	{
		if (x>2)
			verify++;

		if (!x)
		{
			if (cardsidx->cards[i] != -1)
				x++;
		}
		else
		{
			if (cardsidx->cards[i] != -1)
				x++;
			else if (cardsidx->cards[i] == -1 && joker>0)
			{ x++; j=i; joker--; }
			else 
				x=0;
		}
	}

	fprintf(stdout, "buraco_move_cards_to_group() ...3 ... verify:%d j:%d\n", verify, j);
	if (!verify) 
	{	
		buraco_mem_del_gcards(table->roomid, group, game_cards);

        t = cfst;
        while (t)
        {
                fprintf(stdout, "\033[1mcards--debug: roomid:%d->%d group:%d->%d id:%d->%d suit:%.3d:%c\n\033[m", t->roomid, table->roomid, t->group, group, t->id, game_cards, t->suit, t->suit);
                t = t->next;
        }
		snprintf(ret, sizeof(ret)-1, "*ERROR* Not a sequential cards..\n\0");
		send(userid, ret, strlen(ret), 0);
		return -1;
	}

	fprintf(stdout, "buraco_move_cards_to_group() ...4\n");
	if (j>=0 && j<=SZ_NAIPE)
	{
		if (cardsidx->cards[1]==-1 && cardsidx->cards[2]!=-1 && strncmp((char*)&cardsidx->cards[14], "2", 1)==0)
			cardsidx->cards[1] = cardsidx->cards[14];
		else
			cardsidx->cards[j] = cardsidx->cards[14];

		cardsidx->cards[14] = -1;
	}

	fprintf(stdout, "buraco_move_cards_to_group() ...5\n");
	i=0;
	while (cards[i])
	{
		buraco_remove_card_from_user(table, userid, cards[i]);
		i++;
	}

	for (i=0;i<(int)strlen(gcards);i++)
		gcards[i] = '\0';

	fprintf(stdout, "buraco_move_cards_to_group() ...6 strlen(gcards):%d\n", strlen(gcards));
	//FIXME
	//buraco_count_points_of_cards(table, rcards);
	//
	uidx = buraco_mem_get_user(userid);
	if (uidx)
	{
		for (i=0; i<SZ_NAIPE; i++)
		{
			card = 0;
			if (cardsidx->cards[i] != -1)
			{
				card = cardsidx->cards[i];
				if (!strlen(gcards))
					strcpy(gcards, BARALHO[card]);
				else
				{
					snprintf(ret, sizeof(ret)-1, ",%s\0", BARALHO[card]);
					strcat(gcards, ret);
				}
			}
		}
		
		if (exists)
			snprintf(ret, sizeof(ret)-1, "*GAME ROOM* user *%s* added cards *%s* to group *%d*\n\0", uidx->nick, gcards, cardsidx->id);
		else
			snprintf(ret, sizeof(ret)-1, "*GAME ROOM* user *%s* make a new group of cards *%s*\n\0", uidx->nick, gcards);

		buraco_message_to_room(table->roomid, ret);
	}
		/*
		x = 0;
		for (i=0; i<SZ_NAIPE; i++)
		{
			card = 0;
			if (cardsidx->cards[i] != -1)
			{
				card = cardsidx->cards[i];
				gcards[x] = BARALHO[card];
				x++;
			}
		}
		snprintf(ret, sizeof(ret)-1, "*GAME ROOM* user *%s* make a new group of cards *%s\0", uidx->nick);
		buraco_message_to_room(table->roomid, ret);
		*/
	fprintf(stdout, "buraco_move_cards_to_group() END\n");

	return 0;
}

int
buraco_discard_to_garbage(struct ROOM *table, int userid, char *ch)
{
	int card, pos_g;
	char ret[SZ_STRING_MID];

	pos_g = 0; card = 0;

	pos_g = buraco_count_garbage(table);
	if (pos_g == BURACO_TABLE_CARDS-1)
	{
		snprintf(ret, sizeof(ret)-1, "*ERROR* Too many discarded cards in the game.\n\0");
		send(userid, ret, strlen(ret), 0);

		return -1;
	}

	card = buraco_get_card_value(table, ch);
	table->in_game.garbage[pos_g] = table->in_game.baralho[card];

	buraco_remove_card_from_user(table, userid, ch);

	return 0;
}

/* get the card of principal deck */
int
buraco_get_table_card(struct ROOM *table, int userid)
{
	int x, i, card;
	struct ROOM_idx *ridx;

	x = 0; i = 0; card = 0;
	ridx = buraco_mem_get_room(table->roomid);

	for (i=BURACO_TABLE_CARDS-1;table->in_game.table_cards[i]==-1;i--);
	
	if (i<0)
		return -1;

	card = table->in_game.table_cards[i];

	if (ridx->users[0] == userid)
	{
		for (x=0;table->in_game.cards_p1[x]!=-1;x++);
		table->in_game.cards_p1[x] = card;
	}
	else if (ridx->users[1] == userid)
	{
		for (x=0;table->in_game.cards_p2[x]!=-1;x++);
		table->in_game.cards_p2[x] = card;
	}
	else if (ridx->users[2] == userid)
	{
		for (x=0;table->in_game.cards_p3[x]!=-1;x++);
		table->in_game.cards_p3[x] = card;
	}
	else if (ridx->users[3] == userid)
	{
		for (x=0;table->in_game.cards_p4[x]!=-1;x++);
		table->in_game.cards_p4[x] = card;
	}

	table->in_game.table_cards[i] = -1;

	return card;
}

/* get cards of the garbage to user */
//FIXME: Check if the user have too many cards to get the garbage
int
buraco_get_garbage_card(struct ROOM *table, int userid)
{
	int i, x, z, card;
	struct ROOM_idx *ridx;

	i = 0; x = 0; z = 0; card = 0;

	ridx = buraco_mem_get_room(table->roomid);

	if (ridx->users[0] == userid)
		for (x=0;table->in_game.cards_p1[x]!=-1;x++);
	else if (ridx->users[1] == userid)
		for (x=0;table->in_game.cards_p2[x]!=-1;x++);
	else if (ridx->users[2] == userid)
		for (x=0;table->in_game.cards_p3[x]!=-1;x++);
	else if (ridx->users[3] == userid)
		for (x=0;table->in_game.cards_p4[x]!=-1;x++);

	z = buraco_count_garbage(table);
	if (ridx->users[0] == userid)
		for (i=0; i<z; table->in_game.cards_p1[x+i]=table->in_game.garbage[i],i++);
	else if (ridx->users[1] == userid)
		for (i=0; i<z; table->in_game.cards_p2[x+i]=table->in_game.garbage[i],i++);
	else if (ridx->users[2] == userid)
		for (i=0; i<z; table->in_game.cards_p3[x+i]=table->in_game.garbage[i],i++);
	else if (ridx->users[3] == userid)
		for (i=0; i<z; table->in_game.cards_p4[x+i]=table->in_game.garbage[i],i++);

	card = -1;
	for (i=0; i<BURACO_DEAD_CARDS; table->in_game.garbage[i]=card,i++);

	return 0;
}

/* resume room to users */
int
buraco_resume_to_room(int roomid, struct ROOM *table)
{
	struct ROOM_idx *aux;
	struct CARDS *cidx;
	char ret[SZ_STRING_MID];
	int card=0,pos=0,i=0,x=0;

	snprintf(ret, sizeof(ret)-1, "*POINTS OF BLUE: %.5d*   *POINTS OF RED: %.5d*\n\0", table->points_red, table->points_blue);
	buraco_message_to_room(roomid, ret);

	aux = buraco_mem_get_room(roomid);
	if (aux)
	{
		cidx = cfst;
		if (cidx) {
			snprintf(ret, sizeof(ret)-1, "*TEAM 1*  \0");
			buraco_message_to_room(roomid, ret);

			while (cidx)
			{
				if (cidx->roomid==table->roomid && cidx->group==0)
				{
					//FIXME
					//Show Groups Cards
					snprintf(ret, sizeof(ret)-1, "id:%d[ \0", cidx->id);
					buraco_message_to_room(roomid, ret);

					for (i=0;i<SZ_NAIPE;i++)
					{
						if (cidx->cards[i]!=-1)
						{
							card = 0;
							card = cidx->cards[i];
							snprintf(ret, sizeof(ret)-1, "%s \0", BARALHO[card]);
							buraco_message_to_room(roomid, ret);
						}
					}
					
					snprintf(ret, sizeof(ret)-1, "]  ");
					buraco_message_to_room(roomid, ret);
				}
		
				cidx = cidx->next;
			}	

			snprintf(ret, sizeof(ret)-1, "\n*TEAM 2*  \0");
			buraco_message_to_room(roomid, ret);

			cidx = cfst;
			while (cidx)
			{
				if (cidx->roomid==table->roomid && cidx->group==1)
				{
					snprintf(ret, sizeof(ret)-1, "id:%d[ \0", cidx->id);
					buraco_message_to_room(roomid, ret);

					for (i=0;i<SZ_NAIPE;i++)
					{
						if (cidx->cards[i]!=-1)
						{
							card = 0;
							card = cidx->cards[i];
							snprintf(ret, sizeof(ret)-1, "%s \0", BARALHO[card]);
							buraco_message_to_room(roomid, ret);
						}
					}

					snprintf(ret, sizeof(ret)-1, "] \0");
					buraco_message_to_room(roomid, ret);
				}

				cidx = cidx->next;	
			}

			snprintf(ret, sizeof(ret)-1, "\n\0");
			buraco_message_to_room(roomid, ret);
		}
		else
		{
			snprintf(ret, sizeof(ret)-1, "*NO GROUPS OF CARDS YET*\n\0");
			buraco_message_to_room(roomid, ret);
		}
	}

	snprintf(ret, sizeof(ret)-1, "*CARDS DISCARDED* \0");
	buraco_message_to_room(roomid, ret);

	for (x=0;x<BURACO_DEAD_CARDS;x++)
	{
		if (table->in_game.garbage[x] != -1)
		{
			pos = table->in_game.garbage[x];
			snprintf(ret, sizeof(ret)-1, "%s \0", BARALHO[pos]);
			buraco_message_to_room(roomid, ret);
		}
	}

	snprintf(ret, sizeof(ret)-1, "\n\0");
	buraco_message_to_room(roomid, ret);

	return 0;// SHOW CARDS TO USER
}

int
buraco_show_cards_to_user(int userid, struct ROOM *table)
{
	struct USER_idx *uidx;
	struct ROOM_idx *ridx;
	char ret[SZ_STRING_MIN];
	int z, card;

	uidx = buraco_mem_get_user(userid);
	if (uidx)
	{
		ridx = buraco_mem_get_room(uidx->roomid);

		if (ridx)
		{
			snprintf(ret, sizeof(ret)-1, "*Your cards* \0");
			send(uidx->userid, ret, strlen(ret), 0);

			for (z=0;z<BURACO_TABLE_CARDS;z++)
			{
				card = 0;

				if (ridx->users[0] == userid)
					card = table->in_game.cards_p1[z];
				else if (ridx->users[1] == userid)
					card = table->in_game.cards_p2[z];
				else if (ridx->users[2] == userid)
					card = table->in_game.cards_p3[z];
				else if (ridx->users[3] == userid)
					card = table->in_game.cards_p4[z];
				
				if (card!=-1)
				{
					snprintf(ret, sizeof(ret)-1, "%s \0", BARALHO[card]);
					send(uidx->userid, ret, strlen(ret), 0); usleep(2);
				}
			}

			snprintf(ret, sizeof(ret)-1, "\n\0");
			send(uidx->userid, ret, strlen(ret), 0);
		}
	}

	return 0;
}

int
buraco_close_game(int roomid)
{
	struct ROOM_idx *ridx;
	struct USER_idx *uidx;
	int users;

	fprintf(stdout, "buraco_close_game() in\n");
	ridx = buraco_mem_get_room(roomid);
	if (!ridx)
		return -1;

	if (ridx)
	{
		ridx->inGame = 0;

		for (users=0; users<BURACO_USERS_PER_TABLE; users++)
		{
			uidx = NULL;
			uidx = buraco_mem_get_user(ridx->users[users]);
			if (uidx)
			{
				uidx->roomid = 0;
				uidx->inGame = 0;
			}

			ridx->users[users] = 0;
		}
	}

	buraco_mem_del_game(roomid);
	buraco_mem_del_room(roomid);
	fprintf(stdout, "buraco_close_game() out\n");

	return 0;
}

int
buraco_interpret_game(struct ROOM *room, char *buf, int *who, int *newcard, int *garbage)
{
	struct USER_idx *uidx;
	struct ROOM_idx *ridx;
	char *gameargs[4];
	int i, x, card;
	char ret[SZ_STRING_MAX];


	ridx = NULL;
	ridx = buraco_mem_get_room(room->roomid);
	if (!ridx)
		return -1;

	uidx = NULL;
	uidx = buraco_mem_get_user(ridx->users[*who]);

	if (!uidx)
		return -1;

	gameargs[0] = (char *)strkey(buf, 0, ' '); gameargs[1] = (char *)strkey(buf, 1, ' ');
	gameargs[2] = (char *)strkey(buf, 2, ' '); gameargs[3] = (char *)strkey(buf, 3, ' ');

	if (gameargs[0])
	{
		if (!strncmp("USERCARD", gameargs[0], strlen(gameargs[0])))
		{
			buraco_show_cards_to_user(uidx->userid, room);
		}
		else if (*newcard==0 && !strncmp("GETNCARD", gameargs[0], strlen(gameargs[0])))
		{
			if (gameargs[1])
			{
				i = atoi(gameargs[1]);
				if (i) // TABLE CARD
				{
					card = buraco_get_table_card(room, uidx->userid);
					snprintf(ret, sizeof(ret)-1, "*GAME* you got a new card *%s*\n\0", BARALHO[card]);
				} 
				else // LIXO
				{
					buraco_get_garbage_card(room, uidx->userid);
					snprintf(ret, sizeof(ret)-1, "*GAME* you got new cards\n\0");
					//++*garbage;
				}

				send(uidx->userid, ret, strlen(ret), 0);
				++*newcard;
			} 
			else 
			{
				snprintf(ret, sizeof(ret)-1, "*ERROR* command *GETNCARD* had too few arguments. (eg.: GETNCARD 1)\n\0");
				send(uidx->userid, ret, strlen(ret), 0);
			}
		}
		else if (!strncmp("MOVEHAND", gameargs[0], strlen(gameargs[0])))
		{
			if (gameargs[2])
			{
				x = atoi(gameargs[2]);
			
				if (ridx->users[0]==uidx->userid || ridx->users[2]==uidx->userid)
					i=0;
				else if (ridx->users[1]==uidx->userid || ridx->users[3]==uidx->userid)
					i=1;

				buraco_move_cards_to_group(room, uidx->userid, gameargs[1], i, x);
			}
			else
			{
				snprintf(ret, sizeof(ret)-1, "*ERROR* MOVEHAND: too few arguments. (eg.: MOVEHAND 8h,Tc,KD 1)\n\0");
				send(uidx->userid, ret, strlen(ret), 0);
			}
		}
		else if (*newcard!=0 && !strncmp("LEFTCARD", gameargs[0], strlen(gameargs[0])))
		{
			if (gameargs[1])
			{
				if (buraco_discard_to_garbage(room, uidx->userid, gameargs[1]) != -1)
				{
					snprintf(ret, sizeof(ret)-1, "*GAME ROOM* user *%s* move card(s) *%s* to garbage.\n\n", uidx->nick, gameargs[1]);
					buraco_message_to_room(uidx->roomid, ret);

					++*who;

					if (ridx->players==2 && *who==2)
						*who = 0;
					else if	(ridx->players==4 && *who==4)
						*who = 0;

					*newcard = 0; *garbage = 0;
				}
				else
				{
					snprintf(ret, sizeof(ret)-1, "*ERROR* you dont have this card *%s*\n", gameargs[1]);
					send(uidx->roomid, ret, strlen(ret), 0);
				}
			}
		}
		else
		{
			buraco_interpret(uidx->userid, buf);
		}
		fprintf(stdout, "*GAME ROOM* id *%d* user *%s* inGame:%d sends command *%s*\n\0", room->roomid, uidx->nick, uidx->inGame, buf);
	}
	else
		fprintf(stdout, "*GAME ROOM* id *%d* COMMAND ERROR..\n\0", room->roomid);

	return 0;
}

/*
void *
check_timeout()
{
	if (user_wait == 15)
	{
		x=0;

		uidx = buraco_mem_get_user(saveplayer);
		if (!uidx)
			x++;
		else
		{
			if (strncmp(uidx->nick, nick, strlen(uidx->nick)))
				x++;
		}

		if (x)
		{
			snprintf(ret, sizeof(ret)-1, "*GAME ERROR* The user *%s* doesn't back to the game, closing game...\n\0", nick);
			buraco_message_to_room(room->roomid, ret);
			buraco_close_game(room->roomid);

			return NULL;
		}
				
		uidx->inGame = 1;
		snprintf(ret, sizeof(ret)-1, "*GAME ROOM* user *%s* come back, restarting the game..\n\0", nick);
		buraco_message_to_room(room->roomid, ret);
		saveplayer = -1; user_wait = 0; 
		memset(&nick, 0, sizeof(nick));
	} 

} */

void *
buraco_loop_game(struct ROOM *room)
{
	int i, x, users, rbytes;
	struct ROOM_idx *ridx;
	struct USER_idx *uidx;
	char get[SZ_STRING_MAX], ret[SZ_STRING_MAX], nick[SZ_NICKNAME];
	struct pollfd gamepoll[4];
	int who_is, smax, newcard, garbage, saveplayer;
	time_t timeout;
	fd_set master, slave;

	users = 0; who_is = 0; smax = 0; newcard = 0; garbage = 0; saveplayer = -1;

	sleep(5);

	ridx = buraco_mem_get_room(room->roomid);

	if (!ridx)
		return NULL; 
	
	ridx->turn = -1;

	while (1)
	{
		FD_ZERO(&master); FD_ZERO(&slave);
		for (i=0; i<buraco_mem_count_room(ridx->id); i++)
		{
			FD_SET(ridx->users[i], &master);
			if (ridx->users[i] > smax)
				smax = ridx->users[i];
		}

		slave = master;
		select(smax+1, &slave, NULL, NULL, NULL);

		for (users=0; users<BURACO_USERS_PER_TABLE; users++)
		{
			if (!ridx->users[users])
				continue;

			if (ridx->users[users] == saveplayer)
				continue;

			if (ridx->turn != who_is)
			{
				//FIXME: Bug se o user sai antes de inciar o jogo
				fprintf(stdout, "buraco_loop_game() resumin init\n");
				ridx->turn = who_is;
				uidx = buraco_mem_get_user(ridx->users[ridx->turn]);
				snprintf(ret, sizeof(ret)-1, "*GAME ROOM* turn changes, its time to *%s* do an action...\n\0", uidx->nick);
				buraco_message_to_room(room->roomid, ret);
				buraco_resume_to_room(room->roomid, room);
				
				fprintf(stdout, "buraco_loop_game() resumin middle 1 fd0:%d fd1:%d\n", ridx->users[0], ridx->users[1]);
				for (x=0;x<BURACO_USERS_PER_TABLE;x++)
				{
					if (ridx->users[x]!=0)
						buraco_show_cards_to_user(ridx->users[x], room);
				}
				fprintf(stdout, "buraco_loop_game() resumin end\n");
			}

			if (FD_ISSET(ridx->users[users], &slave))
			{
				rbytes=0;
				rbytes=recv(ridx->users[users], get, sizeof(get)-1, 0);

				if (!rbytes)
				{
					uidx = buraco_mem_get_user(ridx->users[users]);
					if (!uidx)
						return NULL;

					strcpy(nick, uidx->nick); saveplayer = ridx->users[users];
					ridx->users[users] = 0; uidx->roomid = 0; uidx->inGame = 0;

	
					snprintf(get, sizeof(get)-1, "*GAME ERROR* User *%s* close his connection, waiting 30s for him back..\n\0", nick);
					buraco_message_to_room(room->roomid, get);
				} 
				else
				{
					get[rbytes-2] = '\0';

					if (get[0] == ':')
					{
						uidx = buraco_mem_get_user(ridx->users[users]);
						snprintf(ret, sizeof(ret)-1, "<%s> %s\n\0", uidx->nick, get);
						buraco_message_to_room(room->roomid, ret);
					}
					else if (ridx->users[ridx->turn] == ridx->users[users])
						buraco_interpret_game(room, get, &who_is, &newcard, &garbage);
					else
						buraco_interpret(ridx->users[users], get);

					memset(&get, 0, sizeof(get));
				}

			}
			usleep(5);
		}
		usleep(2);
	}
}

int
buraco_init_game(int players, struct ROOM *table)
{
	struct ROOM_idx *ridx;
	struct USER_idx *uidx;
	int i,j,z;
	int card;
	pthread_t tid;

	buraco_sort_cards(table->in_game.baralho);

	if (players == 2)
	{
		for (i=0;i<11;i++) 
		{
			table->in_game.cards_p1[i] = table->in_game.baralho[i];
			for (z=11;z<BURACO_TABLE_CARDS;z++)
				table->in_game.cards_p1[z] = -1;
		}
		for (;i<22;i++)
		{
			table->in_game.cards_p2[i-11] = table->in_game.baralho[i];
			for (z=11;z<BURACO_TABLE_CARDS;z++)
				table->in_game.cards_p2[z] = -1;
		}
		for (j=0;j<11;j++)
			table->in_game.cards_p3[i] = -1;
		for (j=0;j<11;j++)
			table->in_game.cards_p4[i] = -1;

		for (;i<33;i++)
			table->in_game.room_dead_one[i-22] = table->in_game.baralho[i];
		for (;i<44;i++)
			table->in_game.room_dead_two[i-33] = table->in_game.baralho[i];

		for (;i<=BURACO_CARDS;i++)
			table->in_game.table_cards[i-44] = table->in_game.baralho[i];
	}
	else if (players == 4)
	{
		for (i=0;i<11;i++)
		{
			table->in_game.cards_p1[i] = table->in_game.baralho[i];
			for (z=11;z<BURACO_TABLE_CARDS;z++)
				table->in_game.cards_p1[z] = -1;
		}
		for (;i<22;i++)
		{
			table->in_game.cards_p2[i-11] = table->in_game.baralho[i];
			for (z=11;z<BURACO_TABLE_CARDS;z++)
				table->in_game.cards_p2[z] = -1;
		}
		for (;i<33;i++)
		{
			table->in_game.cards_p3[i-22] = table->in_game.baralho[i];
			for (z=11;z<BURACO_TABLE_CARDS;z++)
				table->in_game.cards_p3[z] = -1;
		}
		for (;i<44;i++)
		{
			table->in_game.cards_p4[i-33] = table->in_game.baralho[i];
			for (z=11;z<BURACO_TABLE_CARDS;z++)
				table->in_game.cards_p4[z] = -1;
		}

		for (;i<55;i++)
			table->in_game.room_dead_one[i-44] = table->in_game.baralho[i];
		for (;i<66;i++)
			table->in_game.room_dead_two[i-55] = table->in_game.baralho[i];

		for (;i<=BURACO_CARDS;i++)
			table->in_game.table_cards[i-66] = table->in_game.baralho[i];
	}

	for (i=0;i<BURACO_DEAD_CARDS;i++)
		table->in_game.garbage[i] = -1;

	fprintf(stdout, "BARALHO ORIGINAL:");
	for (z=0; z<BURACO_CARDS; z++)
	{
		fprintf(stdout, "%.3d:%s ", z, BARALHO[z]);
	}
	fprintf(stdout, "\n\n");

	fprintf(stdout, "BARALHO DA VEZ:");
	for (z=0; z<BURACO_CARDS; z++)
	{
		card = 0;
		card = table->in_game.baralho[z];
		fprintf(stdout, "%.3d:%s:%.3d ", z, BARALHO[card], card);
	}
	fprintf(stdout, "\n\n");

	fprintf(stdout, "Player 1: "); 
	for (z=0; z<11; z++)
	{
		card = 0;
		card = table->in_game.cards_p1[z];
		fprintf(stdout, "%.2d:%s:%.3d ", z, BARALHO[card], card);
	}
	fprintf(stdout, "\n");
	fprintf(stdout, "Player 2: "); 
	for (z=0; z<11; z++)
	{
		card = 0;
		card = table->in_game.cards_p2[z];
		fprintf(stdout, "%.2d:%s:%.3d ", z, BARALHO[card], card);
	}
	if (players > 2)
	{
		fprintf(stdout, "\nPlayer 3: "); 
		for (z=0; z<11; z++)
		{
			card = 0;
			card = table->in_game.cards_p3[z];
			fprintf(stdout, "%.2d:%s:%.3d ", z, BARALHO[card], card);
		}
		fprintf(stdout, "\n");
		fprintf(stdout, "Player 4: "); 
		for (z=0; z<11; z++)
		{
			card = 0;
			card = table->in_game.cards_p4[z];
			fprintf(stdout, "%.2d:%s:%.3d ", z, BARALHO[card], card);
		}
	}
	fprintf(stdout, "\n\n");

	fprintf(stdout, "Morto 1: "); 
	for (z=0; z<11; z++)
	{
		card = 0;
		card = table->in_game.room_dead_one[z];
		fprintf(stdout, "%s ", BARALHO[card]);
	}
	fprintf(stdout, "\n");

	fprintf(stdout, "Morto 2: "); 
	for (z=0; z<11; z++)
	{
		card = 0;
		card = table->in_game.room_dead_two[z];
		fprintf(stdout, "%s ", BARALHO[card]);
	}
	fprintf(stdout, "\n\n");

	fprintf(stdout, "Table Cards: "); 
	for (z=0; z<(int)strlen(table->in_game.table_cards); z++)
	{
		card = 0;
		card = table->in_game.table_cards[z];
		fprintf(stdout, "%.2d:%s:%.3d ", z, BARALHO[card], card);
	}
	fprintf(stdout, "\n");

	ridx = buraco_mem_get_room(table->roomid);
	for (i=0;i<ridx->players;i++)
	{
		uidx = NULL;
		uidx = buraco_mem_get_user(ridx->users[i]);

		if (uidx)
			uidx->inGame = 1;
	}

	ridx->inGame = 1;
	pthread_create(&tid, NULL, (void*)buraco_loop_game, table);
}

