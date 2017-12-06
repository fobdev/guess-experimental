#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <algorithm>
#include <cctype>
#include <conio.h>

std::vector<std::string> words;
int word_size_config = 0;
int game_diff = 0;
int main();
enum difficulty { EASY, MEDIUM, HARD, LUCKY };


bool vector_contains_word(const std::vector<std::string>& vec, const std::string& word)
{
	for (const auto& w : vec)
	{
		if (w == word)
		{
			return true;
		}
	}
	return false;
}

std::vector<int> bucket_filler(const std::string& word)
{
	std::vector<int> buckets(26, 0);
	for (char c : word)
	{
		/*
		Get a character between A to Z and put a index in it.

		Works like this:
		If letter A is found = c becames the inputted 'a' and get in buckets['a' - 'a'] = 0
		If letter B is found = c becames the inputted 'b' and get in buckets['b' - 'a'] = 1
		If letter C is found = c becames the inputted 'c' and get in buckets['c' - 'a'] = 2
		and so on between every value of the vector.
		*/

		buckets[c - 'a']++;
	}
	return buckets;
}

int score_match(const std::string& word1, const std::string& word2, int w_size)
{
	const auto buckets1 = bucket_filler(word1);
	const auto buckets2 = bucket_filler(word2);

	int score = 0;
	for (int i = 0; i < 26; i++)
	{
		score += std::min(buckets1[i], buckets2[i]);
	}
	for (int i = 0; i < w_size; i++)
	{
		if (word1[i] == word2[i])
		{
			score++;
		}
	}

	return score;
}

void Generate(int w_size, int game_diff)
{
	/*
	Line 1 - Creates a random number generator
	Line 2 - Set the limits of the generator
	Line 3 - Randomize a word to be guessed by the user.
	*/


	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, words.size() - 1);
	std::string target = words[dist(rng)];

	int guess_total = 0;
	while (true)
	{
		std::cout << "Guess a " << w_size << " letters word [ " << guess_total << " / " << game_diff << " ]: ";
		std::string guess;
		std::getline(std::cin, guess);

		if (guess_total != game_diff)
		{

			for (auto& c : guess)
			{
				c = std::tolower(c);
			}

			if (guess == "restart")
			{
				system("cls");
				word_size_config = 0;
				main();
			}
			else
			{
				if (guess.size() != w_size)
				{
					if (guess == "")
					{
						system("cls");
						continue;
					}
					else
					{
						std::cout << "\"" << guess << "\" does not have " << w_size << " letters.\n" << std::endl;
						guess_total++;
						continue;
					}
				}

				if (!vector_contains_word(words, guess))
				{
					std::cout << "\"" << guess << "\" does not exists in the dictionary.\n" << std::endl;
					guess_total++;
					continue;
				}

				const int score = score_match(guess, target, w_size);
				if (score == w_size)
				{
					std::cout << "You did it! \"" << guess << "\" is the correct word!\n" << std::endl;
					break;
				}
				else
				{

					if (score < w_size && score > 1)
					{
						std::cout << "Wrong! But there is " << score << " correct letters in \"" << guess << "\", keep trying!\n" << std::endl;
						guess_total++;
					}
					else if (score == 1)
					{
						std::cout << "Wrong! But there is " << score << " correct letter in \"" << guess << "\", keep trying!\n" << std::endl;
						guess_total++;
					}
					else
					{
						std::cout << "Wrong! There is no correct letters in \"" << guess << "\", try again.\n" << std::endl;
						guess_total++;
					}

					continue;
				}
			}
		}
		else
		{
			system("cls");
			std::cout << "Game Over!" << std::endl;
			std::cout << "\n\nPress any key to restart the game." << std::endl;
			std::cin.get();
			system("cls");
			main();
		}

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.get();
	}
}

int amount_verifier()
{
	std::cout << "Amount of characters in a word (3 to 16): ";
	std::cin >> word_size_config;

	// Amount that does not belong to the limits.
	if (word_size_config < 3)
	{
		std::cout << "This amount of characters is too low, please set another." << std::endl;
		system("pause");
		system("cls");
		word_size_config = 0;
		amount_verifier();
	}
	if (word_size_config > 16)
	{
		std::cout << "Maybe the words can became a bit too large, please set other amount of characters." << std::endl;
		system("pause");
		system("cls");
		word_size_config = 0;
		amount_verifier();
	}

	// Small word warning
	if (word_size_config >= 3 && word_size_config < 5)
	{
		std::cout << "Note that small words sometimes can be as hard as long words to guess, they sometimes looks random..."
			"\n(Y to proceed | N to change): ";
		char select = _getch();
		if (select == 'Y' || select == 'y')
		{
			return word_size_config;
		}
		else if (select == 'N' || select == 'n')
		{
			system("cls");
			word_size_config = 0;
			amount_verifier();
		}
	}

	// Long word warning
	if (word_size_config >= 10)
	{
		std::cout << "Note that long words will make the game way harder and longer to finish..."
			"\n(Y to proceed | N to change): ";
		char select = _getch();
		if (select == 'Y' || select == 'y')
		{
			return word_size_config;
		}
		else if (select == 'N' || select == 'n')
		{
			system("cls");
			word_size_config = 0;
			amount_verifier();
		}
	}

	return word_size_config;
}

int difficulty()
{
	system("cls");
	std::cout << "Select the game difficulty. "
		"\n(1) > Easy   [50 lifes]"
		"\n(2) > Normal [25 lifes]"
		"\n(3) > Hard   [10 lifes]"
		"\n(4) > Lucky  [5  lifes]"
		"\n-> ";
	char diff = _getch();

	if (diff < '1' || diff > '4')
	{
		std::cout << "Please select a valid difficulty. " << std::endl;
		system("pause");
		system("cls");
		difficulty();
	}
	else
	{
		return diff;
	}
}

void init()
{
	system("color f");
	std::ifstream words_data("common_words_data.txt");

	int word_selected_size = amount_verifier();
	int curr_dif = difficulty();

	std::cout << "\nYou can restart the game writing \"restart\" at any moment." << std::endl;
	system("pause");

	// Verify if current line matches with word size configurations.
	for (std::string line; std::getline(words_data, line);)
	{
		// Clear the line if the current line isn't equal to the amount of characters selected.
		if (line.size() != word_selected_size)
		{
			line.clear();
		}

		// Saves the space of the cleared line above.
		if (line.empty())
		{
			continue;
		}
		words.push_back(line);
	}

	system("cls");

	Generate(word_selected_size, curr_dif);

	// Tests if the words are being loaded properly

	// for (const auto& w : words)
	// {
	// 	std::cout << w << std::endl;
	// }

	std::cout << std::endl;
}

int main()
{
	init();

	system("pause");
	return 0;
}