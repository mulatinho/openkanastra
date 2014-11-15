#include "buraco.h"

struct CARDS *
buraco_mem_get_gcards(int roomid, int group, int games)
{
	struct CARDS *aux;

	aux = cfst;
	while (aux!=NULL)
	{
		if (aux->roomid!=roomid || aux->group!=group || aux->id!=games)
			aux = aux->next;
		else
			break;
	}

	return aux;
}

struct ROOM * 
buraco_mem_get_table_cards(int roomid)
{
	struct ROOM *aux;

	aux = rfst;
	while (aux != NULL && aux->roomid != roomid)
		aux = aux->next;

	return aux;
}

struct ROOM_idx * 
buraco_mem_get_room(int roomid)
{
	struct ROOM_idx *aux;

	aux = ridx_fst;
	while (aux != NULL && aux->id != roomid)
		aux = aux->next;

	return aux;
}

struct USER_idx * 
buraco_mem_get_user(int userid)
{
	struct USER_idx *aux;

	aux = uidx_fst;
	while (aux != NULL && aux->userid != userid)
		aux = aux->next;

	return aux;
}

int 
buraco_mem_del_user(int userid)
{
	struct USER_idx *temp=NULL, *tmp=NULL, *aux=NULL;
	
	if (!uidx_fst)
		return 0;
		
	if (uidx_fst->userid == userid) {
		temp = uidx_fst->next;
		memset(uidx_fst->nick, 0, sizeof(uidx_fst->nick));
		free(uidx_fst);
		uidx_fst = NULL;
		uidx_fst = temp;
		return 0;
	}
		
	aux = uidx_fst;
	while (aux && (aux->userid != userid)) {
		tmp = aux;
		aux = aux->next;			
	}

	if (aux) {
		temp = aux;
		tmp->next = aux->next;
		memset(temp->nick, 0, sizeof(temp->nick));
		free(temp);			
		temp = NULL;
		return 0;
	}

	return -1;
} 

int 
buraco_mem_del_gcards(int roomid, int group, int id)
{
	struct CARDS *temp=NULL, *tmp=NULL, *aux=NULL;
	
	if (!cfst)
		return 0;

	if (cfst->roomid == roomid && cfst->group == group && cfst->id == id) 
	{	
		temp = cfst->next;
		free(cfst);
		cfst = NULL;
		cfst = temp;
		return 0;
	}
		
	aux = cfst;
	while (aux!=NULL && (aux->roomid != roomid || aux->group == group || aux->id == id)) 
	{
		tmp = aux;
		aux = aux->next;			
	}

	if (aux) {
		temp = aux;
		tmp->next = aux->next;

		free(temp);
		temp = NULL;
		return 0;
	}

	return -1;
}

int 
buraco_mem_del_room(int roomid)
{
	struct ROOM_idx *temp=NULL, *tmp=NULL, *aux=NULL;
	
	if (!ridx_fst)
		return 0;

	if (ridx_fst->id == roomid) {
		temp = ridx_fst->next;
		free(ridx_fst);
		ridx_fst = NULL;
		ridx_fst = temp;
		return 0;
	}
		
	aux = ridx_fst;
	while (aux && (aux->id != roomid)) {
		tmp = aux;
		aux = aux->next;			
	}

	if (aux) {
		temp = aux;
		tmp->next = aux->next;

		free(temp);
		temp = NULL;
		return 0;
	}

	return -1;
} 

int 
buraco_mem_del_game(int roomid)
{
	struct ROOM *temp=NULL, *tmp=NULL, *aux=NULL;
	
	if (!rfst)
		return 0;

	if (rfst->roomid == roomid) {
		temp = rfst->next;
		free(rfst);
		rfst = NULL;
		rfst = temp;
		return 0;
	}
		
	aux = rfst;
	while (aux && (aux->roomid != roomid)) {
		tmp = aux;
		aux = aux->next;			
	}

	if (aux) {
		temp = aux;
		tmp->next = aux->next;

		free(temp);
		temp = NULL;		
		return 0;
	}

	return -1;
}

int 
buraco_mem_update_user_idx(int fd, char *nick, int auth, int roomid, int in_game)
{
	struct USER_idx *new = malloc(sizeof(struct USER_idx));
	struct USER_idx *aux, *tmp, *vrf;
	int is_online;

	is_online = 0;

	vrf = uidx_fst;
	while (vrf != NULL)
	{
		if (vrf->userid == fd)
		{
			is_online++;
			
			if (nick)
			{
				if (!strlen(vrf->nick))
					strcpy(vrf->nick, nick);
			}

			break;
		}

		vrf = vrf->next;
	}

	if (!is_online)
	{
		new->auth = auth;
		new->userid = fd;
		new->roomid = roomid;
		new->inGame = 0;
		new->timeout = 0;

		new->nick[0] = '\0';


		new->next = NULL;

		if (!uidx_fst) {
			uidx_fst = new;
		} else {
			tmp = NULL;
			aux = uidx_fst;

			while (aux) { tmp = aux; aux = aux->next; }

			if (!aux) {
				tmp->next = new;
				new->next = aux;
			}
		}
	}
	else
	{
		vrf->roomid = roomid;
		vrf->auth = auth;
		vrf->inGame = in_game;
	}

	return 0;
}

int
buraco_mem_add_gcards(int roomid, int group, int subgroup)
{
	struct CARDS *new = malloc(sizeof(struct CARDS));
	struct CARDS *aux, *tmp;
	int i;

	new->roomid = roomid;
	new->group = group;
	new->id = subgroup;
	new->suit = 0;

	for (i=0;i<SZ_NAIPE;i++)
		new->cards[i] = -1;

	new->next = NULL;

	if (!cfst) {
		cfst = new;
	} else {
		tmp = NULL;
		aux = cfst;

		while (aux) { tmp = aux; aux = aux->next; }

		if (!aux) {
			tmp->next = new;
			new->next = aux;
		}
	}

	return 0;
}

int 
buraco_mem_create_room_idx(int roomid, int players, char *pass)
{
	struct ROOM_idx *new = malloc(sizeof(struct ROOM_idx));
	struct ROOM_idx *aux, *tmp;

	new->id = roomid;
	new->players = players;
	new->inGame = 0; new->turn = 0; new->dead_cards = 0;
	new->users[0] = 0; new->users[1] = 0; new->users[2] = 0; new->users[3] = 0;

	if (pass)
		strcpy(new->passphrase, pass);
	else
		new->passphrase[0] = '\0';

	new->next = NULL;

	if (!ridx_fst) {
		ridx_fst = new;
	} else {
		tmp = NULL;
		aux = ridx_fst;

		while (aux) { tmp = aux; aux = aux->next; }

		if (!aux) {
			tmp->next = new;
			new->next = aux;
		}
	}

	return 0;
}

int 
buraco_mem_count_room(int roomid)
{
	struct ROOM_idx *aux;
	int loop=0;

	aux = buraco_mem_get_room(roomid);
	while (aux!=NULL && aux->users[loop]!=0)
		loop++;

	return loop;
}

int 
buraco_mem_user_join_room(int roomid, int userid, char *pass)
{
	struct USER_idx *tmp;
	struct ROOM_idx *aux;
	int loop=0,nplayers=0,exists=0,test=0;
	char ret[SZ_STRING_MAX];

	aux = buraco_mem_get_room(roomid);
	if (!aux)
		return -1;

	if (aux)
	{
		nplayers = buraco_mem_count_room(roomid);
		if (nplayers >= aux->players)
		{
			snprintf(ret, sizeof(ret)-1, "*error* the room is full and the game is already running..\n\0");
			send(userid, ret, strlen(ret), 0);
			return -1;
		}
			
		while (loop < aux->players)
		{
			if (aux->users[loop] == userid)
				exists++;

			if (!aux->users[loop])
				break;

			loop++; 
		}
	}

	tmp = buraco_mem_get_user(userid);
	if (tmp)
	{
		if (tmp->roomid != 0)
			exists++;
	}

	if (exists)
	{
		snprintf(ret, sizeof(ret)-1, "*error* you're already enter in a room\n");
		send(userid, ret, strlen(ret), 0);
		return -1;
	}
	else
	{
		if (pass!=NULL && strlen(aux->passphrase)>0)
		{
			if (!strncmp(aux->passphrase, pass, strlen(aux->passphrase)))
				test++;
			else
			{
				snprintf(ret, sizeof(ret)-1, "*AUTH* wrong username or password!\n");
				send(userid, ret, strlen(ret), 0);

				return -1;
			}
		}
		else if (!pass && !strlen(aux->passphrase))
			test++;
		else
		{
			snprintf(ret, sizeof(ret)-1, "*AUTH* Error, wrong username or password!\n");
			send(userid, ret, strlen(ret), 0);

			return -1;
		}
			
		if (test)
		{
			tmp->roomid = roomid;
			aux->users[loop] = userid;
			
			if (!loop)
			{
				snprintf(ret, sizeof(ret)-1, "*GAME ROOM* user *%s* create *room %d* with password *%s*\n\0", tmp->nick, roomid, pass);
				send(tmp->userid, ret, strlen(ret), 0);
			}
			else
			{
				snprintf(ret, sizeof(ret)-1, "*GAME ROOM* user *%s* join in *room %d* with password *%s*\n\0", tmp->nick, roomid, pass);
				buraco_message_to_room(tmp->roomid, ret);
			}
		}
		
		return 0;
	}

	return 0;
}

struct ROOM * 
buraco_mem_init_game(int roomid, int players)
{
	struct ROOM *new = malloc(sizeof(struct ROOM));
	struct ROOM *aux, *tmp;
	struct ROOM_idx *ridx;

	ridx = buraco_mem_get_room(roomid);
	if (ridx)
	{
		new->roomid = roomid;
		new->points_red = 0; new->points_blue = 0;

		buraco_init_game(ridx->players, new);

		new->next = NULL;

		if (!rfst) {
			rfst = new;
		} else {
			tmp = NULL;
			aux = rfst;

			while (aux) { tmp = aux; aux = aux->next; }

			if (!aux) {
				tmp->next = new;
				new->next = aux;
			}
		}
	}

	return new;
}
