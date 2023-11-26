#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_LINES_FILE 591

int points = 0, chancesLeft = 3;

int generateRandomNumber(int lower, int upper)
{
	return (rand() % (upper - lower + 1)) + lower;
}

void generateRandomWord(int lineNumber, char *arr[], int i)
{
	FILE *file;
	char filename[100] = "assets\\words.txt";
	char line[256]; // Adjust the string size as needed

	// Open the file
	file = fopen(filename, "r");
	if (file == NULL)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	// Read lines until the desired line
	for (int currentLine = 1; currentLine <= lineNumber; ++currentLine)
	{
		if (fgets(line, sizeof(line), file) == NULL)
		{
			// Handle the case where the file has fewer lines than lineNumber
			fprintf(stderr, "File has fewer lines than %d\n", lineNumber);
			fclose(file);
			exit(EXIT_FAILURE);
		}
	}
	// Close the file
	fclose(file);

	// Remove the newline character if present
	line[strcspn(line, "\n")] = '\0';
	arr[i] = strdup(line);
	if (arr[i] == NULL)
	{
		perror("Memory allocation error");
		exit(EXIT_FAILURE);
	}
}

void delay(int seconds, char *wordsArr[], int numberOfWords)
{
	// int milli_seconds = 1000 * seconds;

	// Storing start time
	clock_t start_time = clock();
	// looping until required time is not achieved
	while (clock() / 1000 < start_time / 1000 + seconds)
	{
		for (int i = 0; i < numberOfWords; i++)
		{
			printf("%s\t", wordsArr[i]);
			if (i > 0 && (i + 1) % 5 == 0)
				printf("\n");
		}
		printf("\nGame begins in %d seconds...\n", seconds - ((clock() - start_time) / 1000));
		system("cls");
	}
}

void game()
{
	int numberOfWords, waitTime;
	printf("Enter number of words you want to guess: ");
	scanf("%d", &numberOfWords);

	char *wordsArr[numberOfWords];
	for (int i = 0; i < numberOfWords; i++)
	{
		generateRandomWord(generateRandomNumber(0, MAX_LINES_FILE), wordsArr, i);
	}

	waitTime = numberOfWords * 0.8;

	printf("Wait time is: %d secs\nMemorise every Word faster.\nPress ANY key to Begin Timer.", waitTime);
	_getch();
	delay(waitTime, wordsArr, numberOfWords);

	printf("\t\tGame Begins!!\nYou will get 1 point for every correct guess.\nAnd you will get %d chances for Wrong guesses\n", chancesLeft);

	char *guessedWord = (char *)malloc(256 * sizeof(char));
	if (guessedWord == NULL)
	{
		perror("Memory allocation error");
		exit(EXIT_FAILURE);
	}

	int tempNumberOfWords = numberOfWords;
	while (numberOfWords != 0 && chancesLeft != 0)
	{
		printf("\nEnter your guess: ");
		scanf("%s", guessedWord);

		// Checking if guessed word was correct
		int found = 0;
		for (int i = 0; i < numberOfWords; i++)
		{
			if (stricmp(guessedWord, wordsArr[i]) == 0)
			{
				for (int j = i; j < numberOfWords - 1; ++j)
				{
					wordsArr[j] = strdup(wordsArr[j + 1]);
				}
				numberOfWords--;
				found = 1;
				points++;
				printf("\nGreat, your guess was right.\nChances left: %d\nWords left to guess: %d\n", chancesLeft, numberOfWords);
			}
		}

		if (!found)
		{
			chancesLeft--;
			printf("\nOops, wrong guess\nChances left: %d\nWords left to guess: %d\n", chancesLeft, numberOfWords);
		}
	}

	// Check game results
	if (!chancesLeft)
	{
		printf("\n\t\tGAME OVER!!\n\"Tough Game this time, No worries, that's the game!, shake it off\nand let's aim for an epic comeback in the next round. You've got this!\"\n\nYou earned %d Points", points);
	}
	else if (!numberOfWords)
	{
		printf("\n\t\tCongrats, you have Won!\n\"I'm truly honored to be in the company of a Memory Master.\"\nYou've mastered the memory game! Outstanding performance.\n");
	}
	else
	{
		printf("ERROR\n");
	}

	// Free allocated memory
	free(guessedWord);
	for (int i = 0; i < tempNumberOfWords; i++)
	{
		free(wordsArr[i]);
	}
}

int main(int argc, char const *argv[])
{
	srand(time(NULL));
	printf("\t\tMEMORY MASTER GAME\n\t\tDeveloped by: Amritpal Singh(B.Tech Sem-1)\n");
	game();

	return 0;
}