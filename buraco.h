#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <poll.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

#define BURACO_NAME "OpenKanastra"
#define BURACO_VERSION "0.1"
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
#define SZ_NAIPE 14

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

	int inGame;

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

/* Other functions */
int buraco_num_get_users(int);

/* Memory functions */
struct USER_idx *buraco_mem_get_user(int);
struct ROOM_idx *buraco_mem_get_room(int);
int buraco_mem_update_user_idx(int, char *, int, int, int);
int buraco_mem_update_room_idx(int, int, char *);
