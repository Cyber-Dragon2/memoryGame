#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define WORD_FILENAME "assets\\words.txt"
#define FILENAME "assets\\info.txt"
#define MAX_LINES_IN_WORDS_FILE 546

int points = 0, chancesLeft = 3;
int wins, loses;
char* name[30];

void setData(char* n[], int ws, int ls){
	FILE *file;

	// Open the file in read mode
	file = fopen(FILENAME, "w+");
	if (file == NULL)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	// Write the updated values to the file
    fprintf(file, "%s\n%d %d\n", n, ws, ls);

	// Close the file
	fclose(file);
}

void readData(char* n[], int* ws, int* ls){
	FILE *file;

	// Open the file in read mode
	file = fopen(FILENAME, "r");
	if (file == NULL)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	// Read the counts from the file
	if (fscanf(file, "%s %d %d",n , ws, ls) != 3)
	{
		fprintf(stderr, "Error reading data from the file.\n");
		exit(EXIT_FAILURE);
	}
	// Close the file
	fclose(file);
}

int generateRandomNumber(int lower, int upper)
{
	return (rand() % (upper - lower + 1)) + lower;
}

void generateRandomWord(int lineNumber, char *arr[], int pos)
{
	FILE *file;
	char line[256]; // Adjust the string size as needed
	// Open the file
	file = fopen(WORD_FILENAME, "r");
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
	arr[pos] = strdup(line);
	if (arr[pos] == NULL)
	{
		perror("Memory allocation error");
		exit(EXIT_FAILURE);
	}
}

void delay(int seconds, char *wordsArr[], int numberOfWords)
{
	// Storing start time
	clock_t start_time = clock();
	// looping until required time is not achieved
	while (clock() / 1000 < start_time / 1000 + seconds)
	{
		// Display words list
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
	float difficulties[] = {1.5, 1, 0.8};
	int difficultyChoosed;
	printf("\nEnter number of words you want to guess: ");
	scanf("%d", &numberOfWords);

	printf("Select Game difficulty\n\t0 - Easy\n\t1 - Medium\n\t2 - Hard\n");
	do{
		printf("Select Difficulty: ");
		scanf("%d", &difficultyChoosed);
		if(difficultyChoosed<0 || difficultyChoosed>2)
			printf("ERROR: Invalid Difficulty Selected, Try Again\n");
	}while(difficultyChoosed<0 || difficultyChoosed>2);

	char *wordsArr[numberOfWords];
	for (int i = 0; i < numberOfWords; i++)
	{
		generateRandomWord(generateRandomNumber(0, MAX_LINES_IN_WORDS_FILE), wordsArr, i);
	}

	waitTime = numberOfWords * difficultyChoosed[difficulties];

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
		printf("\n\t\tGAME OVER!!\n\"Tough Game this time, No worries, that's the game!, shake it off\nand let's aim for an epic comeback in the next round. You've got this!\"\n\nYou earned %d Points\n", points);
		loses++;
	}
	else if (!numberOfWords)
	{
		printf("\n\t\tCongrats, you have Won!\n\"I'm truly honored to be in the company of a Memory Master.\"\nYou've mastered the memory game! Outstanding performance.\n");
		wins++;
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
	readData(name, &wins, &loses);
	printf("\t\tMEMORY MASTER GAME\n\t\tDeveloped by: Amritpal Singh(B.Tech Sem-1)\n\n");
	printf("Hello, %s!\nPrepare for an immersive experience where memory mastery is the ultimate goal.\n",name);

	int option;
	
	do{
		printf("\n\t\tChoose you Operation\n\t0 - Play Game\n\t1 - Display Game Stats\n\t2 - Change Username\n\t3 - Exit\n");
		printf("\nEnter command: ");
		scanf("%d", &option);

		if (option == 0){
			game();
			setData(name, wins, loses);
		}
		else if(option == 1){
			printf("\nSure, %s\nHere are your Game Stats\n\t%d WINS\n\t%d LOSES\nYou're doing well. Let's keep aiming for even greater heights in the memory challenges ahead.\n",name, wins, loses);
		}
		else if(option == 2){
			printf("\nPerforming Username updation\nNote: Entered name should be without spaces you can use Underscore(_) instead.\nEnter new Username: ");
			scanf("%s", name);
			setData(name, wins, loses);
			printf("Successfully updated you Username to %s\n", name);
		}
		else if(option == 3){
			printf("\"Goodbye for now, %s! Keep those neurons firing until our next showdown.\"\n",name);
		}
		else
			printf("ERROR: Enter valid Command\n");
	}while(option != 3);

	return 0;
}