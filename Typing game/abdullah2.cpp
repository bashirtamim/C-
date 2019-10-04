//Bashir Abdullah
//typing game
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<chrono>
#include<ctime>

typedef struct str{
	char string[100];
};


str generateString()
{
	srand(time(NULL));
	str string;
	//strcpy_s(string.string, "Hi");
	static const char alphanum[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < 7; ++i) {
		int random_chance = rand() % 10;

		//20% chance of wild card
		if (random_chance == 0)
		{
			string.string[i] = '0';
		}
		else if (random_chance == 1)
		{
			string.string[i] = '*';
		}
		else
		{
			string.string[i] = alphanum[rand() % (sizeof(alphanum)-1)];
		}
	}

	string.string[7] = 0;

	//strcpy_s(string.string, "Game");
	return string;
}

int get_offset(str string1, str string2)
{
	//check max length
	int max_length = 0;
	if (sizeof(string1.string) < sizeof(string2.string))
		max_length = sizeof(string1.string);
	else
		max_length = sizeof(string2.string);

	int offset = 0;
	for (int i = 0; i < sizeof(max_length); i++)
	{
		//pad with space
		if (string1.string[i] == 0) string1.string[i] = ' ';
		if (string2.string[i] == 0) string2.string[i] = ' ';

		//calculate offset
		if (string1.string[i] == '0')
		{
			if (!isdigit(string2.string[i]))
				offset += abs(string1.string[i] - string2.string[i]);

		}
		else if (string1.string[i] == '*')
		{
			if (isalnum(string2.string[i]))
				offset += abs(string1.string[i] - string2.string[i]);
		}
		else
			offset += abs(string1.string[i] - string2.string[i]);
	}

	return offset;
}

str translate_wildcard(str string)
{
	str new_string;
	int j = 0;
	for (int i = 0; i < 7; i++)
	{
		if (string.string[i] == '0')
		{
			new_string.string[j] = '[';
			new_string.string[++j] = '0';
			new_string.string[++j] = '-';
			new_string.string[++j] = '9';
			new_string.string[++j] = ']';
		}
		else if (string.string[i] == '*')
		{
			new_string.string[j] = '[';
			new_string.string[++j] = '%';
			new_string.string[++j] = '-';
			new_string.string[++j] = '&';
			new_string.string[++j] = ']';
		}
		else
			new_string.string[j] = string.string[i];

		j++;

	}
	new_string.string[j] = 0;

	return new_string;
}

int main()
{
	str string, question;
	int score = 1000;
	int time_limit = 7000;
	while (true)
	{
		//save current time
		std::chrono::steady_clock::time_point c_start = std::chrono::steady_clock::now();
		question = generateString();
		std::cout << "Your current points " << score << ", just type -> " << translate_wildcard(question).string << ": ";

		std::cin >> (string.string);

		//save end time
		std::chrono::steady_clock::time_point c_end = std::chrono::steady_clock::now();

		int offset = get_offset(question, string);

        //calculate time
		int elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(c_end - c_start).count()/1000;
		std::cout << elapsed_time << " milliseconds";

		//if the string is correct
		if (offset == 0)
		{
			//if within time frame
			if (elapsed_time <= time_limit)
			{
				std::cout << ", you made it within the interval of 7000...\n";
				score += 500;
			}
			else
			{
				std::cout << ", you *failed* it within the interval of 7000...\n" << "Your total penalty is " << elapsed_time - time_limit << "...\n";
				score -= (elapsed_time - time_limit);
			}
		}
		else
		{
			int time_penalty = 0;
			int offset_penalty = 0;

			//if within time frame
			if (elapsed_time <= time_limit)
			{
				std::cout << ", you made it within the interval of 7000...\n";
				offset_penalty = offset;
			}
			else
			{
				std::cout << ", you *failed* it within the interval of 7000...\n";
				time_penalty = (elapsed_time - time_limit);
				offset_penalty = offset * 2;
			}
			score -= (time_penalty + offset_penalty);
			std::cout << "String offset is " << offset << ", your total penalty is " << time_penalty + offset_penalty << "...\n";
		}

		//calculate end game
		if (score <= 0)
		{
			std::cout << "Bye...\n";
			std::cin >> (string.string);
			break;
		}
		else if (score >= 5000)
		{
			std::cout << "You Win! Bye...\n";
			std::cin >> (string.string);
			break;
		}
	}
	return 0;
}

