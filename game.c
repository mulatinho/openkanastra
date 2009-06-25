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
	int i, pos;

//	fprintf(stdout , "buraco_get_card_value() init, ch:%s\n", ch);
	for (i=0;i<BURACO_CARDS;i++)
	{
		pos = table->in_game.baralho[i];
		if (!strncmp(BARALHO[pos], ch, strlen(ch)))
			return i;
	}

	return -1;
//	fprintf(stdout , "buraco_get_card_value() init\n");
}

int buraco_remove_card_from_user(struct ROOM *table, int userid, char *card)
{
	struct ROOM_idx *ridx;
	char ret[SZ_STRING_MID];
	int i, x, z, tcard;

	fprintf(stdout, "buraco_remove_card_from_user() init, %s\n", card);
	i = 0; x = 0; z = 0; tcard = 0;
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

		fprintf(stdout, "buraco_remove_cards_from_user() middle, x: %d\n", x);
		if (!x)
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
			//
		}
		
	}

	fprintf(stdout, "buraco_remove_cards_from_user() end\n");
	return 0;
}

int
buraco_move_cards_to_group(struct ROOM *table, int userid, char *rcards, int group, int game_cards)
{
	struct CARDS *cardsidx, *tmp, *t;
	int i, x, card, exists;
	char *cards[SZ_NAIPE];

	fprintf(stdout, "buraco_move_cards_to_group() init\n");
	exists = 0; card = 0; i = 0; x = 0;

	tmp = (struct CARDS *)buraco_mem_get_gcards(table->roomid, group, game_cards);
	if (!tmp)
		buraco_mem_update_gcards(table->roomid, group, game_cards);

	t = cfst;
	while (t)
	{
		fprintf(stdout, "cards--debug: roomid:%d group:%d id:%d\n", t->roomid, t->group, t->id);
		t = t->next;
	}
	// Algo
	//
	// Process cards
	//   Count one per one
	//     Verify if they had the same suit
	//   Update Points
	//   Move cards
	//
	// REMOVE CARD FROM USER

	fprintf(stdout, "buraco_move_cards_to_group() args init\n");
	cards[0] = (char *)strkey(rcards, 0, ','); cards[1] = (char *)strkey(rcards, 1, ','); cards[2] = (char *)strkey(rcards, 0, ','); 
	cards[3] = (char *)strkey(rcards, 3, ','); cards[4] = (char *)strkey(rcards, 4, ','); cards[5] = (char *)strkey(rcards, 5, ','); 
	cards[6] = (char *)strkey(rcards, 6, ','); cards[7] = (char *)strkey(rcards, 7, ','); cards[8] = (char *)strkey(rcards, 8, ','); 
	cards[9] = (char *)strkey(rcards, 9, ','); cards[10] = (char *)strkey(rcards, 10, ','); cards[11] = (char *)strkey(rcards, 11, ',');
	cards[12] = (char *)strkey(rcards, 12, ','); cards[13] = (char *)strkey(rcards, 13, ',');
	fprintf(stdout, "buraco_move_cards_to_group() args finish\n");
	
	if (!cards[2])
	{
		// FIXME
		// PELO O MENOS 3 cartas
		return -1;
	}

	cardsidx = (struct CARDS *)buraco_mem_get_gcards(table->roomid, group, game_cards);
	if (cardsidx) 
	{
		fprintf(stdout, "buraco_move_cards_to_group() -> cardsidx init\n");
		for (x=0; cardsidx->cards[x]!=-1; x++);

		i = 0;
		while (cards[i])
		{
			card = buraco_get_card_value(table, cards[i]);
			cardsidx->cards[x] = table->in_game.baralho[card];

			buraco_remove_card_from_user(table, userid, cards[i]);
			fprintf(stdout, "cards[%d] string:%s/value:%.3d  type:%c/suit:%c\n", x, cards[x], card, cards[i][0], cards[i][1]);
			i++; x++;
		}
		fprintf(stdout, "buraco_move_cards_to_group() -> cardsidx end\n");
	}
	fprintf(stdout, "buraco_move_cards_to_group() end\n");
	return 0;
}

//FIXME
// Ve se nao passa do range do array;
int
buraco_discard_to_garbage(struct ROOM *table, int userid, char *ch)
{
	int i, card, pos_g;
	char ret[SZ_STRING_MID];

	pos_g = 0; card = 0;
	fprintf(stdout, "buraco_discard_to_garbage() init ch:%s\n", ch);

	pos_g = buraco_count_garbage(table);
	if (pos_g == BURACO_DEAD_CARDS-1)
	{
		snprintf(ret, sizeof(ret)-1, "*ERROR* Too many discarded cards in the game.\n\0");
		send(userid, ret, strlen(ret), 0);

		return -1;
	}
	fprintf(stdout, "buraco_discard_to_garbage() pos_g:%d\n", pos_g);
	card = buraco_get_card_value(table, ch);
	table->in_game.garbage[pos_g] = table->in_game.baralho[card];
	buraco_remove_card_from_user(table, userid, ch);

	// REMOVE CARD FROM USER

	fprintf(stdout, "buraco_discard_to_garbage() emd\n");
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

	fprintf(stdout, "buraco_get_garbage_card() init\n");
	ridx = buraco_mem_get_room(table->roomid);

	fprintf(stdout, "buraco_get_garbage_card() ... 1\n");
	if (ridx->users[0] == userid)
		for (x=0;table->in_game.cards_p1[x]!=-1;x++);
	else if (ridx->users[1] == userid)
		for (x=0;table->in_game.cards_p2[x]!=-1;x++);
	else if (ridx->users[2] == userid)
		for (x=0;table->in_game.cards_p3[x]!=-1;x++);
	else if (ridx->users[3] == userid)
		for (x=0;table->in_game.cards_p4[x]!=-1;x++);
	fprintf(stdout, "buraco_get_garbage_card() ... 2 -> xvalue: %d\n", x);

	z = buraco_count_garbage(table);
	if (ridx->users[0] == userid)
		for (i=0; i<z; table->in_game.cards_p1[x+i]=table->in_game.garbage[i],i++);
	else if (ridx->users[1] == userid)
		for (i=0; i<z; table->in_game.cards_p2[x+i]=table->in_game.garbage[i],i++);
	else if (ridx->users[2] == userid)
		for (i=0; i<z; table->in_game.cards_p3[x+i]=table->in_game.garbage[i],i++);
	else if (ridx->users[3] == userid)
		for (i=0; i<z; table->in_game.cards_p4[x+i]=table->in_game.garbage[i],i++);
	fprintf(stdout, "buraco_get_garbage_card() ... end\n");

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
	int card=0,pos=0,loop=0,i=0,x=0;

	snprintf(ret, sizeof(ret)-1, "\n\n*POINTS OF BLUE: %.5d*   *POINTS OF RED: %.5d*\n\0", table->points_red, table->points_blue);
	buraco_message_to_room(roomid, ret);

	aux = buraco_mem_get_room(roomid);
	if (aux)
	{
		cidx = cfst;
		if (cidx) {
			snprintf(ret, sizeof(ret)-1, "*GAMES OF GROUP 1*\n  \0");
			buraco_message_to_room(roomid, ret);

			while (cidx)
			{
				if (cidx->roomid==table->roomid && cidx->group==0)
				{
					//FIXME
					//Show Groups Cards
					snprintf(ret, sizeof(ret)-1, "id:%d[\0", cidx->id);
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
					
					snprintf(ret, sizeof(ret)-1, "]\n  \0");
					buraco_message_to_room(roomid, ret);
				}
		
				cidx = cidx->next;
			}	

			snprintf(ret, sizeof(ret)-1, "\n\0");
			buraco_message_to_room(roomid, ret);

			snprintf(ret, sizeof(ret)-1, "*GAMES OF GROUP 2*\n  \0");
			buraco_message_to_room(roomid, ret);

			cidx = cfst;
			while (cidx)
			{
				if (cidx->roomid == table->roomid && cidx->group==1)
				{
					snprintf(ret, sizeof(ret)-1, "id:%d[\0", cidx->id);
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

					snprintf(ret, sizeof(ret)-1, "]\n  \0");
					buraco_message_to_room(roomid, ret);
				}

				cidx = cidx->next;	
			}

			snprintf(ret, sizeof(ret)-1, "] \n\n\0");
			buraco_message_to_room(roomid, ret);
		}
		else
		{
			snprintf(ret, sizeof(ret)-1, "*NO GROUPS OF CARDS YET*\n\n\0");
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
}

int
buraco_show_cards_to_user(int userid, struct ROOM *table)
{
	struct USER_idx *uidx;
	struct ROOM_idx *ridx;
	char ret[SZ_STRING_MIN];
	int j, z, card;

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
}

int
buraco_close_game(int roomid)
{
	struct ROOM_idx *ridx;
	int users;

	ridx = buraco_mem_get_room(roomid);
	if (ridx)
	{
		ridx->inGame = 0;

		for (users=0; users<BURACO_USERS_PER_TABLE; users++)
			ridx->users[users] = 0;
	}

	buraco_mem_del_game(roomid);

	return 0;
}

void *
buraco_loop_game(struct ROOM *room)
{
	struct ROOM_idx *ridx;
	struct USER_idx *uidx;
	struct CARDS *cidx;
	int i, x, z, newcard, garbage, who_is, bytes, card, players_now;
	char get[SZ_STRING_MAX], ret[SZ_STRING_MAX];
	char *gameargs[4];

	who_is = 0; 
	ridx = buraco_mem_get_room(room->roomid);

	if (!ridx)
		return NULL; //FIXME: Can't create this game;
	
	while (1)
	{
		if (ridx->players==2 && who_is==2)
			who_is=0;
		else if	(ridx->players==4 && who_is==4)
			who_is=0;

		uidx = NULL;
		uidx = buraco_mem_get_user(ridx->users[who_is]);
		if (uidx)
		{
			snprintf(ret, sizeof(ret)-1, "*GAME ROOM* turn changes, its time to *%s* do an action...\n\0", uidx->nick);
			buraco_message_to_room(room->roomid, ret);
			buraco_show_cards_to_user(uidx->userid, room);
		}
		else
		{
			buraco_user_part(ridx->users[who_is]);

			snprintf(ret, sizeof(ret)-1, "*GAME ERROR* User goes out, waiting for him back..\n\0");
			buraco_message_to_room(room->roomid, ret);

			sleep(30); uidx = NULL;
			uidx = buraco_mem_get_user(ridx->users[who_is]);
			if (!uidx)
			{
				snprintf(ret, sizeof(ret)-1, "*GAME ERROR* User dont back, closing this game..\n\0");
				buraco_message_to_room(room->roomid, ret);
				buraco_close_game(room->roomid);

				return NULL;
			}
			//FIXME
			// USER GONE, WAIT FOR HIM BACK OR DIE
		}

		buraco_resume_to_room(room->roomid, room);
		newcard = 0; garbage = 0;
		while (1)
		{
			// DEBUG
			/*
			fprintf(stdout, "\n\nTable Cards: "); 
			for (z=0; z<BURACO_TABLE_CARDS; z++)
			{
				card = 0;
				card = room->in_game.table_cards[z];
				fprintf(stdout, "%.2d:%s:%.3d ", z, BARALHO[card], card);
			}
			fprintf(stdout, "\n");
			fprintf(stdout, "Garbage[sz:%d]: ", buraco_count_garbage(room));
			for (z=0; z<BURACO_DEAD_CARDS; z++)
			{
				card = 0;
				card = room->in_game.garbage[z];
				fprintf(stdout, "%.2d:%s:%.3d ", z, BARALHO[card], card);
			}
			fprintf(stdout, "\n\n"); */

			players_now = 0;
			players_now = buraco_mem_count_room(room->roomid);
			if (players_now != ridx->players)
			{
				snprintf(ret, sizeof(ret)-1, "*GAME ERROR* One player left of the table. Waiting him back...\n\0");
				buraco_message_to_room(room->roomid, ret);

				players_now = 0; sleep(60); // WAIT 60 Seconds to player back
				players_now = buraco_mem_count_room(room->roomid);
				if (players_now != ridx->players)
				{
					snprintf(ret, sizeof(ret)-1, "*GAME ERROR* User dont back, closing game..\n\0");
					buraco_message_to_room(room->roomid, ret);
					buraco_close_game(room->roomid);
	
					return NULL;
				}
			}

			snprintf(ret, sizeof(ret)-1, "COMMANDS are: USERCARD, MOVEHAND, GETNCARD or LEFTCARD\n$ \0");
			send(uidx->userid, ret, strlen(ret), 0);
			
			memset(get, '\0', sizeof(get)); bytes = 0;
			bytes=read(uidx->userid, get, sizeof(ret)-1);
			if (!bytes)
			{
				ridx->users[who_is] = 0; players_now = 0;
				snprintf(ret, sizeof(ret)-1, "*GAME ERROR* User *%s* got connection closed, waiting for him back..\n\0", uidx->nick);
				buraco_message_to_room(room->roomid, ret);
					
				buraco_user_part(uidx->userid);
				buraco_mem_del_user(uidx->userid);

				sleep(30); players_now = buraco_mem_count_room(room->roomid);
				if (players_now != ridx->players)
				{
					snprintf(ret, sizeof(ret)-1, "*GAME ERROR* User dont back, closing game..\n\0");
					buraco_message_to_room(room->roomid, ret);
					buraco_close_game(room->roomid);
				}
			}
			get[bytes-2] = '\0';
	
			gameargs[0] = (char *)strkey(get, 0, ' '); gameargs[1] = (char *)strkey(get, 1, ' ');
			gameargs[2] = (char *)strkey(get, 2, ' '); gameargs[3] = (char *)strkey(get, 3, ' ');

			if (gameargs[0])
			{
				if (!strncmp("USERCARD", gameargs[0], strlen(gameargs[0])))
				{
					buraco_show_cards_to_user(uidx->userid, room);
				}
				else if (!newcard && !strncmp("GETNCARD", gameargs[0], strlen(gameargs[0])))
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
							garbage++;
						}

						send(uidx->userid, ret, strlen(ret), 0);
						newcard++;
					} 
					else 
					{
						snprintf(ret, sizeof(ret)-1, "*ERROR* command *GETNCARD* had too few arguments.\neg.: GETNCARD 0\n\0");
						send(uidx->userid, ret, strlen(ret), 0);
					}
				}
				else if (!strncmp("MOVEHAND", gameargs[0], strlen(gameargs[0])))
				{
					//FIXME
					// Create new group of cards
					// Move them
					// Count cards
					if (gameargs[2])
					{
						x = atoi(gameargs[2]);
						//ve_cards_to_group(struct ROOM *table, int userid, char *cards, int group)
						
						if (ridx->users[0]==uidx->userid && ridx->users[2]==uidx->userid)
							z=0;
						else
							z=1;

						buraco_move_cards_to_group(room, uidx->userid, gameargs[1], z, x);
						garbage = 0;
					}
					else
					{
						snprintf(ret, sizeof(ret)-1, "*ERROR* MOVEHAND: too few arguments.\neg.: MOVEHAND 8h,Tc,KD 1\n\0");
						send(uidx->userid, ret, strlen(ret), 0);
					}
				}
				else if (garbage==0 && newcard!=0 && !strncmp("LEFTCARD", gameargs[0], strlen(gameargs[0])))
				{
					//FIXME
					// Move discard card to garbage
					if (gameargs[1])
					{
						if (buraco_discard_to_garbage(room, uidx->userid, gameargs[1]) != -1)
						{
							snprintf(ret, sizeof(ret)-1, "*GAME ROOM* user *%s* move card(s) *%s* to garbage.\n", uidx->nick, gameargs[1]);
							buraco_message_to_room(uidx->roomid, ret);
							fprintf(stderr, "in *room %d* command is *%s*\n", room->roomid, gameargs[0]);
							goto nextplayer;
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
					buraco_interpret(uidx->userid, get);
				}
			}
		}
		nextplayer:
		who_is++;
	}

	return 0;
}

int
buraco_init_game(int players, struct ROOM *table)
{
	struct ROOM_idx *ridx;
	struct USER_idx *uidx;
	char ret[SZ_STRING_MAX];
	int i,j,z;
	int card;
	pthread_t tid;
	void *args[4];

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
	for (z=0; z<strlen(table->in_game.table_cards); z++)
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

