#ifdef WIN32
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock.h>
#include <malloc.h>
#else
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <pthread.h>
#endif
#include <time.h>

#define BURACO_NAME "OpenKanastra"
#define BURACO_VERSION "0.5"
#define BURACO_PORT 16666
#define SZ_COMMAND 8
#define SZ_ARGUMENT 20

#define SZ_STRING_MIN 32
#define SZ_STRING_MIN 32
#define SZ_STRING_MID 128
#define SZ_STRING_MED 256
#define SZ_STRING_MAX 512

#define BURACO_CARDS 108
#define BURACO_USERS 300
#define BURACO_USERS_PER_TABLE 4
#define BURACO_GAMES 100
#define BURACO_TWO_PLAYERS 50
#define BURACO_FOUR_PLAYERS 50

#define BURACO_TABLE_CARDS 64
#define BURACO_DEAD_CARDS 11

#define TWO_PLAYERS 2
#define FOUR_PLAYERS 4

#define SZ_NICKNAME 20
#define SZ_PASSWORD 20
#define SZ_NAIPE 16

struct ROOM
{
	int roomid;

	int points_red, points_blue;

	struct
	{
		char baralho[BURACO_CARDS];

		char table_cards[BURACO_TABLE_CARDS], garbage[BURACO_TABLE_CARDS];

		char room_dead_one[BURACO_DEAD_CARDS], room_dead_two[BURACO_DEAD_CARDS];
		
		char cards_p1[BURACO_TABLE_CARDS], cards_p2[BURACO_TABLE_CARDS],
		     cards_p3[BURACO_TABLE_CARDS], cards_p4[BURACO_TABLE_CARDS];
	} in_game;

	struct ROOM *next;
};
struct ROOM *rfst;

struct ROOM_idx
{
	int id;

	int turn, inGame, dead_cards;

	int players;

	int users[BURACO_USERS_PER_TABLE];

	char passphrase[SZ_STRING_MIN];

	struct ROOM_idx *next;
};
struct ROOM_idx *ridx_fst;

struct CARDS
{
	int roomid;

	int group;

	int id;
	
	int suit;

	char cards[SZ_NAIPE];

	struct CARDS *next;
};
struct CARDS *cfst;

struct USER_idx
{
	int userid;

	time_t timeout;

	int inGame;

	int auth, roomid;

	char nick[SZ_NICKNAME];

	struct USER_idx *next;
};
struct USER_idx *uidx_fst;

struct command
{
	char name[SZ_STRING_MIN];

	void (*func)(int, char**);
};

/* Main Functions */
int buraco_init_game(int, struct ROOM *);
char *buraco_strings_clean(char *);
int buraco_interpret(int, char *);
int buraco_check_auth(int, char **);
int buraco_user_auth(int, char *, char *);
void *buraco_user_join(int, char **);
int buraco_user_part(int, char **);
void *buraco_user_list(int, char **);
void *buraco_room_list(int, char **);
int buraco_num_get_users(int);

/* Other functions */
char *strkey(char *, int, short);
int buraco_message_to_room(int, char *);
void *buraco_user_list(int fd, char **args);

/* Memory functions */
struct USER_idx *buraco_mem_get_user(int);
struct ROOM_idx *buraco_mem_get_room(int);
struct CARDS *buraco_mem_get_gcards(int, int, int);
struct ROOM *buraco_mem_get_table_cards(int);
int buraco_mem_del_user(int);
int buraco_mem_del_room(int);
int buraco_mem_del_game(int);
int buraco_mem_update_user_idx(int, char *, int, int, int);
int buraco_mem_update_room_idx(int, int, char *);
int buraco_mem_add_gcards(int, int, int);
int buraco_mem_create_room_idx(int, int, char *);
int buraco_mem_count_room(int);
int buraco_mem_user_join_room(int, int, char *);
struct ROOM *buraco_mem_init_game(int, int);


/* Game functions */
int buraco_sort_cards(char []);
int buraco_count_user_cards(struct ROOM *, int);
int buraco_verify_user_card(struct ROOM *, int, char *);
int buraco_count_garbage(struct ROOM *);
int buraco_get_card_value(struct ROOM *, char *);
int buraco_remove_card_from_user(struct ROOM *, int, char *);
int buraco_move_cards_to_group(struct ROOM *, int, char *, int, int);
int buraco_discard_to_garbage(struct ROOM *, int, char *);
int buraco_get_table_card(struct ROOM *, int);
int buraco_get_garbage_card(struct ROOM *, int);
int buraco_resume_to_room(int, struct ROOM *);
int buraco_show_cards_to_user(int, struct ROOM *);
int buraco_close_game(int);
int buraco_interpret_game(struct ROOM *, char *, int *, int *, int *);
void *buraco_loop_game(struct ROOM *);
int buraco_init_game(int, struct ROOM *);


