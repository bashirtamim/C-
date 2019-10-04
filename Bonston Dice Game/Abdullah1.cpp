//Bashir Abdullah
#include <iostream>
#include <string>
#include <algorithm>
#include <stdlib.h>  
#include <time.h>  

using namespace std;

int main()
{
	int amount1 = 1000, amount2 = 1000;
	srand(time(NULL));
	int round = 1;
	while (amount1 && amount2)
	{
		int a1, a2, a3;
		int b1, b2, b3;
		a1 = rand() % 6 + 1;
		a2 = rand() % 6 + 1;
		a3 = rand() % 6 + 1;
		b1 = rand() % 6 + 1;
		b2 = rand() % 6 + 1;
		b3 = rand() % 6 + 1;
		printf("[Round %d] You rolled (%d, %d, ?), machine rolled (%d, %d, ?)... \n", round, a1, a2, b1, b2);
		int bid = 0;
		cout << "How much are you going to bet :";
		cin >> bid;
		bool f1 = false, f2 = false;
		while (bid > 10 || bid < 0)
		{
			cout << "- your bet must between $1 and $10, type again: ";
			cin >> bid;
		}
		printf("You had (%d, %d, %d)\n", a1, a2, a3);
		if (a1 == a2 && a2 == a3)
		{
			cout << "... Big Fish!!!\n";
			f1 = true;
		}
		else
		{
			a1 = max(max(a1, a2), a3);
			a2 = rand() % 6 + 1;
			a3 = rand() % 6 + 1;
			printf("-> You rolled (%d, %d) and then rolled ", a2, a3);
			a2 = max(a2, a3);
			a3 = rand() % 6 + 1;
			printf("(%d) -> You scored %d.\n", a3, a1 + a2 + a3);
		}
		printf("Machine had (%d, %d, %d)\n", b1, b2, b3);
		if (b1 == b2 && b2 == b3)
		{
			cout << "... Big Fish!!!\n";
			f2 = true;
		}
		else
		{
			b1 = max(max(a1, a2), a3);
			b2 = rand() % 6 + 1;
			b3 = rand() % 6 + 1;
			printf("-> Machine  rolled (%d, %d) and then rolled ", b2, b3);
			b2 = max(a2, a3);
			b3 = rand() % 6 + 1;
			printf("(%d) -> Machine scored %d.\n", b3, b1 + b2 + b3);
		}
		if (f1 && f2)
		{
			cout << "Tie\n";
		}
		else if (f1)
		{
			amount1 = min(bid * 50 + amount1, 2000);
			amount2 = max(amount2 - bid * 50, 0);
			printf("You won :) You have $%d, machine has $%d...\n", amount1, amount2);
		}
		else if (f2)
		{
			amount2 = min(bid * 50 + amount2, 2000);
			amount1 = max(amount1 - bid * 50, 0);
			printf("You lost :( You have $%d, machine has $%d...\n", amount1, amount2);
		}
		else
		{
			int a = a1 + a2 + a3;
			int b = b1 + b2 + b3;
			if (a > b)
			{
				amount1 = min(bid * (a - b) + amount1, 2000);
				amount2 = max(amount2 - bid * (a - b), 0);
				printf("You won :) You have $%d, machine has $%d...\n", amount1, amount2);
			} else 
			if (b > a)
			{
				amount2 = min(bid * (b - a) + amount2, 2000);
				amount1 = max(amount1 - bid * (b - a), 0);
				printf("You lost :( You have $%d, machine has $%d...\n", amount1, amount2);
			}
		}
		cout << endl;
		round++;
	}
	if (amount1 == 0) cout << "You lost :(\n";
	else cout << "You won :D\n";
}