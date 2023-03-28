//
// Created by Yurii Koval on 3/1/23.
//

//
// Created by Yurii Koval on 3/1/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


//Function that adds elements to the array
//Takes in a pointer to the array, a pointer to the new array, the max size of the array, the new string to be added, and the current size of the array
//If the array is full, it creates a new array with a size of 1 more than the old array
void add(char*** pArray, char*** pnew, int* maxSize, char* newString, int* currentSize)
{

  if (*currentSize == *maxSize) {
	*pnew = (char**) malloc(sizeof(char*) * (*maxSize + 1));
	for(int i=0; i<*maxSize; i++)
	  (*pnew)[i] = (*pArray)[i];

	free(*pArray);
	*pArray = *pnew;
	*maxSize += 1;
  }

  (*pArray)[*currentSize] = (char*) malloc(sizeof(char) * (strlen(newString) + 1));
  strcpy((*pArray)[*currentSize], newString);
  (*currentSize)++;
}
//Function that prints the array
//Takes in the array and the size of the array
void displayArray(char* theArray[], int size)
{
  printf("Array: ");
  for(int i=0; i<size; i++)
	printf("%s ", theArray[i]);
  printf("\n");
}

//Function that checks if a word is in the dictionary
//Takes in the array, the size of the array, and the word to be checked
//Loop through the array and check if the word is in the array
bool isInDictionary(char* theArray[], int size, char* word)
{
  for(int i=0; i<size; i++) {
	if (strcmp(theArray[i], word) == 0) {
	  return true;
	}
  }
  return false;
}

//Function that deletes the array
//Takes in the array and the size of the array
//Loop through the array and free each element
//Free the array
void deleteArray(char** pArray, int size)
{
  if (pArray) { // check if pArray is not NULL before freeing it
	for(int i=0; i<size; i++)
	  free(pArray[i]);
	free(pArray);
  }
}


int main() {
  srand(1);
  char filename[] = "words.txt";
  FILE *filePtr = fopen(filename, "r"); // "r" means we open the file for reading
  int numLettersInWord = 0;
  int currentSize = 0;
  int maxSize = 0;
  char **pArray = NULL;
  char **pNew = NULL;
  int steps = 1;
  int gameState = 2;

  pArray = (char **)malloc(sizeof(char *));
  maxSize = 1;

// Check that the file was able to be opened
  if (filePtr == NULL) {
	printf("Error: could not open %s for reading\n", filename);
	exit(-1);
  }

  printf("Weaver is a game where you try to find a way to get from the starting word to the ending word.\n");
  printf("You can change only one letter at a time, and each word along the way must be a valid word.\n");
  printf("Enjoy!\n\n");
  //Start the main game loop
  do {
	//Check whether its the first time the game is being played or user wants to play again
	//Read in the file and store the words of specified length in an array
	if(gameState == 2) {
	  rewind(filePtr);
	  currentSize = 0;
	  maxSize = 0;
	  steps = 1;
	  printf("How many letters do you want to have in the words? ");
	  scanf("%d", &numLettersInWord);
	  char inputString[81] = "";
	  while (fscanf(filePtr, "%s", inputString) != EOF) {

		if (strlen(inputString) == numLettersInWord) {
		  add(&pArray, &pNew, &maxSize, inputString, &currentSize);
		}
	  }
	  printf("Number of %d-letter words found: %d.\n\n", numLettersInWord, currentSize);
	}
	//Get the starting and ending words from the user
	char startWord[81];
	char endWord[81];
	while(true) {
	  printf("Enter starting and ending words, or 'r' for either for a random word: ");
	  scanf("%s", startWord);
	  scanf("%s", endWord);

	  //Check if the user wants a random word
	  //If so, generate a random number and use that as the index of the word in the array
	  if (strcmp(startWord, "r") == 0) {
		int random = rand() % currentSize;
		strcpy(startWord, pArray[random]);
	  }
	  if (strcmp(endWord, "r") == 0) {
		int random = rand() % currentSize;
		strcpy(endWord, pArray[random]);
	  }

	  //chek if the words are valid:
	  //Checking if the word are correct length
	  if (strlen(startWord) != numLettersInWord) {
		printf("Your word, '%s', is not a %d-letter word. Try again.\n", startWord, numLettersInWord);
		continue;
	  }
	  if (strlen(endWord) != numLettersInWord) {
		printf("Your word, '%s', is not a %d-letter word. Try again.\n", endWord, numLettersInWord);
		continue;
	  }

	  //Checking if the words are in the dictionary
	  if (!isInDictionary(pArray, currentSize, startWord)) {
		printf("Your word, '%s', is not a valid dictionary word. Try again.\n", startWord);
		continue;

	  }
	  if (!isInDictionary(pArray, currentSize, endWord)) {
		printf("Your word, '%s', is not a valid dictionary word. Try again.\n", endWord);
		continue;
	  }

	  break;
	}
	printf("Your starting word is: %s.\n", startWord);
	printf("Your ending word is: %s.\n\n", endWord);
	printf("On each move enter a word of the same length that is at most 1 character different and is also in the dictionary.\n");
	printf("You may also type in 'q' to quit guessing.\n\n");


	char guessWord[81];
	char previousWord[81];
	strcpy(previousWord, startWord);

	//Start the guessing loop
	while (true) {
	  printf("%d. Previous word is '%s'. Goal word is '%s'. Next word: ", steps, previousWord, endWord);

	  scanf("%s", guessWord);


	  if(strcmp(guessWord, "q") == 0) {
		break;
	  }

	  //Checking if the word is valid
	  if (strlen(guessWord) != numLettersInWord) {
		printf("Your word, '%s', is not a %d-letter word. Try again.\n", guessWord, numLettersInWord);
		continue;
	  }
	  if (!isInDictionary(pArray, currentSize, guessWord)) {
		printf("Your word, '%s', is not a valid dictionary word. Try again.\n", guessWord);
		continue;
	  }

	  //Checking if the word is 1 character different
	  int numDifferences = 0;
	  for (int i = 0; i < numLettersInWord; i++) {
		if (guessWord[i] != previousWord[i]) {
		  numDifferences++;
		}
	  }
	  //This also handles a case where the user enters the same word twice
	  if (numDifferences > 1 || numDifferences == 0) {
		printf("Your word, '%s', is not exactly 1 character different. Try again.\n", guessWord);
		continue;
	  }
	  //For formatting purposes
	  if(strcmp(guessWord, endWord) != 0)
	  {
		printf("\n");
	  }
	  //Check if the user has won
	  if (numDifferences == 1) {
		if (strcmp(guessWord, endWord) == 0) {
		  printf("Congratulations! You changed '%s' into '%s' in %d moves.\n", startWord, endWord, steps);
		  break;
		}
		strcpy(previousWord, guessWord);
		steps++;
		continue;

	  }

	}
	//Ask the user if they want to play again or change the number of letters in the words
	printf("\nEnter: \t1 to play again,\n");
	printf("\t2 to change the number of letters in the words and then play again, or\n");
	printf("\t3 to exit the program.\n");
	printf("Your choice --> ");
	scanf("%d", &gameState);
	if(gameState == 1){
	  steps = 1;
	  continue;
	}

	deleteArray(pArray, currentSize);
	pArray = NULL;

  }while (gameState != 3);

  printf("\nThanks for playing!\n"
		 "Exiting...\n");

  fclose(filePtr);

  return 0;


}
