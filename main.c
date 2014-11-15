#include "buraco.h"

char *
strkey(char *buf, int who, short del)
{
    char *str = (char*)malloc(strlen(buf));
    short ch, i, f=0, x=0, loop=0;

    for (i=0;i<(int)strlen(buf);i++) {
        ch = *(buf+i);

        if (ch==del && !i && !who)
            return NULL;

        if (ch==del) 
            loop++; 

        if (loop==who) {
            if (i==0 || f!=0) {
                *(str+x) = *(buf+i); x++;
            }
            f++;
        }
    }
    *(str+x)='\0';
    str=(char*)realloc(str,x);
    return (char*)str;
}

int
buraco_message_to_room(int roomid, char *message)
{
	struct ROOM_idx *aux;
	int loop;

	usleep(7); aux = buraco_mem_get_room(roomid);
	if (aux)
	{
		for (loop=0;loop<BURACO_USERS_PER_TABLE;loop++)
		{
			if (aux->users[loop] != 0)
				send(aux->users[loop], message, strlen(message), 0);
		}
	}

	return 0;
}

void *
buraco_user_list(int fd, char **args)
{
	struct USER_idx *uidx;
	char ret[SZ_STRING_MID];
	int users;

	users = 0;

	uidx = uidx_fst;
	while (uidx)
	{
		fprintf(stdout, "ID:%d NICKNAME:%s InGAME:%d ROOMID:%d\n\0", uidx->userid, uidx->nick, uidx->inGame, uidx->roomid);

		uidx = uidx->next;
		users++;
	}

	snprintf(ret, sizeof(ret)-1, "Total users: %d\n\0", users);
	send(fd, ret, strlen(ret), 0);

	return 0;
}

void *
buraco_room_list(int fd, char **args)
{
	char ret[SZ_STRING_MID];
	struct ROOM_idx *aux;
	int room;

	if (!args[1])
		room = 0;
	else
		room = atoi(args[1]);

	if (!room)
	{
		aux = ridx_fst;
		if (!aux)
		{
			snprintf(ret, sizeof(ret)-1, "There are no rooms in this moment\0");
			send(fd, ret, strlen(ret), 0);
		}

		while (aux != NULL)
		{
			snprintf(ret, sizeof(ret)-1, "%d:%d,%d,%d,%d \0",
				aux->id, aux->users[0], aux->users[1], aux->users[2], aux->users[3]);

			send(fd, ret, strlen(ret), 0);
			memset(&ret, 0, sizeof(ret));
			aux = aux->next;
		}
		
		snprintf(ret, sizeof(ret)-1, "\n\0");
		send(fd, ret, strlen(ret), 0);
	}
	else
	{
		aux = buraco_mem_get_room(room);
		if (aux)
		{
			snprintf(ret, sizeof(ret)-1, "%d:%d,%d,%d,%d\n\0",
				aux->id, aux->users[0], aux->users[1], aux->users[2], aux->users[3]);
			send(fd, ret, strlen(ret), 0);
		}
		else
		{
			snprintf(ret, sizeof(ret)-1, "There are no such *room %d* in this moment\n\0", room);
			send(fd, ret, strlen(ret), 0);
		}

	}

}

int 
buraco_user_part(int userid, char **args)
{
	struct ROOM_idx *aux;
	struct USER_idx *usr;
	int loop=0;

	usr = NULL;
	usr = buraco_mem_get_user(userid);
	if (usr)
	{
		usr->roomid = 0; 
		usr->inGame = 0;
	}
	else
		return -1;

	aux = NULL;
	aux = buraco_mem_get_room(usr->roomid);
	if (aux)
	{
		while (loop < BURACO_USERS_PER_TABLE)
		{
			if (aux->users[loop] == userid)
			{
				aux->users[loop] = 0;
				fprintf(stdout, "FIND HIM AND CLOSE HIM..\n");
			}

			loop++;
		}
	}
	else
		return -1;

	return 0;
}

void *
buraco_user_join(int fd, char **args)
{
	struct USER_idx *uidx;
	struct ROOM_idx *ridx;
	int nplayers, roomid;
	char ret[SZ_STRING_MAX];

	if (!args[1])
	{
		snprintf(ret, sizeof(ret)-1, "*USERJOIN* error, too few arguments.\n");
		send(fd, ret, strlen(ret), 0);
	}
	else
	{

		roomid = atoi(args[1]);
		if (!(uidx = buraco_mem_get_user(fd)))
		{
			snprintf(ret, sizeof(ret)-1, "*AUTH* error, you need login in the system\n");
			send(fd, ret, strlen(ret), 0);
		}
		else
		{
			if (!uidx->roomid)
			{
				if (!(ridx = buraco_mem_get_room(roomid)))
				{
					if (!args[2])
						args[2] = NULL;

					buraco_mem_create_room_idx(roomid, TWO_PLAYERS, args[2]);
					buraco_mem_user_join_room(roomid, fd, args[2]);
				}
				else
				{
					if (buraco_mem_count_room(roomid) < ridx->players)
					{
						if (!args[2])
							args[2] = NULL;

						buraco_mem_user_join_room(roomid, fd, args[2]);
					
						nplayers = buraco_mem_count_room(roomid);
						if (ridx->players == nplayers)
						{
							if (!ridx->inGame)
							{
								snprintf(ret, sizeof(ret)-1, "*GAME ROOM* the game will begin in five seconds..\n\0");
								buraco_mem_init_game(roomid, ridx->players);
							}
							else
								snprintf(ret, sizeof(ret)-1, "*GAME ROOM* the user *%s* try to join in the game..\n\0", uidx->nick);

							buraco_message_to_room(roomid, ret);
							fprintf(stdout, "%s", ret);
						}
					}
				}
			}
			else
			{
				snprintf(ret, sizeof(ret)-1, "*ERROR* You are already in a room, can't join in another until game ends.\n\0");
				send(fd, ret, strlen(ret), 0);
			}
		}	
	}

}

int
buraco_user_auth(int userfd, char *user, char *pass)
{
	int auth=0;

	//FIXME: DO LOGIN HERE
	if (!strncmp(user, "crudo", strlen(user)) && !strncmp(pass, "lindju", strlen(pass)))
		auth++;
	else if (!strcmp(user, "esc2") && !strcmp(pass, "feio"))
		auth++;
	else if (!strcmp(user, "yzak") && !strcmp(pass, "ramps"))
		auth++;
	else if (!strcmp(user, "borracho") && !strcmp(pass, "pinguim"))
		auth++;
	else if (!strcmp(user, "user1") && !strcmp(pass, "user1"))
		auth++;
	else if (!strcmp(user, "user2") && !strcmp(pass, "user2"))
		auth++;
	else if (!strcmp(user, "user3") && !strcmp(pass, "user3"))
		auth++;
	else if (!strcmp(user, "user4") && !strcmp(pass, "user4"))
		auth++;
	else if (!strcmp(user, "user5") && !strcmp(pass, "user5"))
		auth++;
	else if (!strcmp(user, "user6") && !strcmp(pass, "user6"))
		auth++;

	if (!auth)
		return 0;
	else
	{
		buraco_mem_update_user_idx(userfd, user, 1, 0, 0);
		return 1;
	}

}

int
buraco_check_auth(int userfd, char **args)
{
	struct USER_idx *aux;
	char ret[SZ_STRING_MAX];

	aux = uidx_fst;
	while (aux != NULL)
	{
		if (aux->userid == userfd && aux->auth == 1)
			return 1;

		aux = aux->next;
	}

	if (!args[2])
		return 0;
	else
	{
		if (!strncmp(args[0], "AUTHUSER", strlen("AUTHUSER")))
		{
			if (buraco_user_auth(userfd, args[1], args[2]))
			{
				snprintf(ret, sizeof(ret)-1, "*AUTH* You are successfully identified! Welcome %s!\n\0", args[1]);
				send(userfd, ret, strlen(ret), 0);

				return 1;
			}
			else
			{
				//FIXME
				// Cuidado com o BRUTE FORCE
				snprintf(ret, sizeof(ret)-1, "*AUTH* Username or password is wrong!\n\0");
				send(userfd, ret, strlen(ret), 0);
			}
		}
	
	}

	return 0;
}

int
buraco_interpret(int userfd, char *buf)
{
	char *args[4];
	int i, z, total;
	char ret[SZ_STRING_MAX];

	struct command commands[] = {
		{ "AUTHUSER", (void*)&buraco_check_auth },
		{ "ROOMLIST", (void*)&buraco_room_list },
		{ "USERJOIN", (void*)&buraco_user_join },
		{ "USERLIST", (void*)&buraco_user_list },
		{ "USERPART", (void*)&buraco_user_part }
//		{ "GAMECARD", (void*)&buraco_room_cards }
//		{ "GMPOINTS", (void*)&buraco_room_points }
	};

	if (!buf || !strlen(buf))
		return -1;

	z = 0;

	args[0] = strkey(buf, 0, ' '); args[1] = strkey(buf, 1, ' ');
	args[2] = strkey(buf, 2, ' '); args[3] = strkey(buf, 3, ' ');

	if (!buraco_check_auth(userfd, args))
		return -1;

	if (!args[0])
		return -1;

	total = sizeof(commands) / sizeof(commands[0]);
	for (i=0;i<total;i++)
	{
		if (!strncmp(commands[i].name, args[0], strlen(commands[i].name)))
		{
			fprintf(stderr, "*COMMAND* user id *%d* type *%s* command.\n", userfd, buf);
			commands[i].func(userfd, args); z++;
		}
	}

	if (!z)
	{
		fprintf(stdout, "*UNKNOWN COMMAND* user id *%d* type *%s* command.\n", userfd, buf);
		snprintf(ret, sizeof(ret)-1, "*UNKNOWN COMMAND*\n");
		send(userfd, ret, strlen(ret), 0);
	}

	return 0;
}

char *
buraco_strings_clean(char *string)
{
	int i, x;
	char *ret = malloc(strlen(string));

	x = 0;
	for (i=0; i<(int)strlen(string);i++)
	{
		if ((string[i] >= 48 && string[i] <= 57) || (string[i] >= 65 && string[i] <= 90) || (string[i] >= 97 && string[i] <= 122) || string[i]==32)
		{
			*(ret+x) = *(string+i);
			x++;
		}
	}

	*(ret+x) = '\0';
	ret=(char*)realloc(ret,x);

	return ret;
}

int 
main(void)
{
	int vrf, ret, soq, newsoq;
	int rbytes, users, smax;
	char buf[SZ_STRING_MAX];
	struct sockaddr_in server, client;
	struct pollfd serverpoll[BURACO_USERS];
	struct USER_idx *uidx;
	char *ptrbuf;
	fd_set slave, master;
		
	ret = 0; users = 0; smax = 0; vrf = 0;

	server.sin_port = htons(BURACO_PORT);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	
	
	if ((soq = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
		return -1;

	if (bind(soq, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
		return -2;

	listen(soq, BURACO_USERS);


	//serverpoll[0].fd = soq;
	//serverpoll[0].events = POLLIN;

	FD_SET(soq, &master);
	smax = soq;

	fprintf(stderr, "*SERVER* waiting for events..\n");
	while (1) {
		slave = master;
		//ret = poll(serverpoll, smax+1, 0);
		ret = select(smax+1, &slave, NULL, NULL, NULL);
		if (ret < 0) {
			fprintf(stderr, "select: requested event failed!\n");
			return -2;
		}

		for (users=0; users<=smax; users++) {
			uidx = NULL;
			uidx = buraco_mem_get_user(users);
			if (uidx)
			{
				if (uidx->inGame)
					continue;
			}

			//fprintf(stdout, "\033[1muserid:%d loop:%d\033[m\n", serverpoll[users].fd, users);
			//sleep(1);
			if (FD_ISSET(users, &slave)) {
				if (users == soq) {
					socklen_t sizestr = sizeof(struct sockaddr);
					if ((newsoq = accept(soq, (struct sockaddr *)&client, &sizestr)) == -1) {
						fprintf(stderr, "accept: fuckin error!\n");
						return -3;
					}

					FD_SET(newsoq, &master);
					if (newsoq > soq)
						smax = newsoq+1;

					fprintf(stdout, "*SERVER* the ip *%s* connect in the server with fd *%d* as anonymous..\n",
						inet_ntoa(client.sin_addr), serverpoll[smax].fd);

					buraco_mem_update_user_idx(newsoq, NULL, 0, 0, 0);
					snprintf(buf, sizeof(buf)-1, "Welcome to %s %s\n\0", BURACO_NAME, BURACO_VERSION);
					send(newsoq, buf, strlen(buf), 0);
				} else {
					rbytes = recv(users, buf, sizeof(buf)-1, 0);
					if (!rbytes) {
						uidx = buraco_mem_get_user(users);
						if (uidx)
						{
							fprintf(stdout, "*QUIT* user *%s* goes offline\n", uidx->nick);
							//FIXME
							//Verificar se ele criou algum canal, se criou remover.
						}
						buraco_mem_del_user(uidx->userid);

						FD_CLR(users, &master);
						close(users);
						
						/*
						for(vrf=0;vrf<=smax;vrf++)
						{
							if (serverpoll[vrf].fd == -1)
							{
								serverpoll[vrf].fd = serverpoll[vrf+1].fd;
								serverpoll[vrf].revents = serverpoll[vrf+1].revents;
								//users--;
							}
						}

						smax--; */
						fprintf(stdout, "*SERVER* the ip *%s* is closed the connection..\n", inet_ntoa(client.sin_addr));
					}

					if (rbytes) {
						buf[rbytes - 2] = '\0';
						//ptrbuf = buraco_strings_clean(buf);
						buraco_interpret(users, buf);
					}
					
					memset(&buf, 0, rbytes);
					rbytes = 0;
				}
			}
			usleep(3);
		}
		usleep(9);
	}
	
	return 0;
}
