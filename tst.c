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

		fprintf(stderr, "baralho[%.3d]:%s ==> %.3d\n", i, BARALHO[x], x);
		//fprintf(stderr, "[%s] ", BARALHO[x]);
		baralho[i] = x;
		brl[i] = baralho[i];
	}

	return 0;
}

int 
main(void)
{
	char cards[BURACO_CARDS];
	int i, card;

	buraco_sort_cards(cards);
	for (i=0;i<BURACO_CARDS;i++)
	{
		card = 0; card = cards[i];
		fprintf(stderr, "[%s:%d -> number:%c type:%c] ", BARALHO[card], strlen(BARALHO[card]), BARALHO[card][0], BARALHO[card][1]);

	}

	return 0;
}
