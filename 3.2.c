//-----------------------------------------------------------------------------
// Evelina Adomaityte PS 1 kursas 1 grupe 1 pogrupis 3 namu darbas 2 uzduotis
//-----------------------------------------------------------------------------
// 2. Parasyti funkcija, kuri duotame tekste perraso zodzius atvirscia tvarka
// (zodziu tvarka nesikeicia, bet keiciasi simboliu tvarka zodyje).
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

//-----------------------------------------------------------------------------
void getDataFileName(int argc, char *argv[]);
void getDataFromFile(int argc, char *argv[]);
void findWords(char *argv[], char *ptrToBuffer, char *ptrToWord, int bufferLength, int *wordLength, int *lineLength);
void reverseWords(char **ptrToWord, int wordLength);
void printResults(char *argv[], char *ptrToWord, int wordLength);
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    FILE *myFile = NULL;

    //checking how many arg have been entered through cmd line, what arg they are and so on
    if(argc == 3) //two file names were entered
    {
        if((myFile = fopen(argv[1], "r")) != 0) //data file exists
        {
            printf("The file exists!\nThe results will be printed into a file named %s!\n", argv[2]);
            fclose(myFile);
            getDataFromFile(argc, argv);
        }
        else //data file doesn't exist
        {
            printf("Failed to open the data file!\nThe results will be printed into a file named %s!\n", argv[2]);
            getDataFileName(argc, argv);
        }
    }
    else if(argc == 2) //one file name was entered
    {
        if((myFile = fopen(argv[1], "r")) == 0) //data file's name wasn't entered
        {
            argv[2] = argv[1];
            printf("You didn't enter the data file's name!\nThe results will be printed into a file named %s!\n", argv[2]);
            fclose(myFile);
            getDataFileName(argc, argv);
        }
        else //results file's name wasn't entered
        {
            argv[2] = "empty";
            printf("The entered file has the data!\nThe results will be printed in the terminal!\n");
            getDataFromFile(argc, argv);
        }
    }
    else if(argc < 2) //no file names were entered
    {
        printf("No file names have been entered!\nThe results will be printed in the terminal!\n\n");
        argv[2] = "empty";
        getDataFileName(argc, argv);
    }
    else //too many file names were entered
    {
        printf("There are too many arguments!\nThe results will be printed in the terminal!\n");
        argv[2] = "empty";
        getDataFileName(argc, argv);
    }
    return 0;
}
//-----------------------------------------------------------------------------
//function that gets data file's name if it wasn't entered
void getDataFileName(int argc, char *argv[])
{
    FILE *myFile = NULL;
    char fileName[1000]; //temporary data file name variable

    while(1)
    {
        printf("Please enter a file's name that has data in it!\n");
        scanf("%[^\n]s", fileName);
        getchar();
        argv[1] = fileName;

        if((myFile = fopen(argv[1], "r")) != 0) //the data file exists
        {
            printf("Success!\n");
            getDataFromFile(argc, argv); //moving on to getting data from a file
            fclose(myFile);
            break;
        }
        fclose(myFile);
    }
}
//-----------------------------------------------------------------------------
//function that gets data from a file
void getDataFromFile(int argc, char *argv[])
{
    FILE *myFile;

    char *ptrToBuffer = (char *)calloc(BUFFER_SIZE + 1, sizeof(char));
    int lineLength = 0;

    if(ptrToBuffer != NULL && (myFile = fopen(argv[1], "r")) != 0)
    {
        char *ptrToWord = (char *)calloc(255, sizeof(char));
        int wordLength = 0;

        if(ptrToWord != NULL)
        {
            while(1) //buffer reading from a file
            {
                int bufferLength = fread(ptrToBuffer, sizeof(char), BUFFER_SIZE, myFile);

                if(bufferLength != 0) //if the buffer isn't empty
                    findWords(argv, ptrToBuffer, ptrToWord, bufferLength, &wordLength, &lineLength);

                if(bufferLength < BUFFER_SIZE) //if the buffer isn't full, there are no data left in the file
                    break;
            }
        }
        else
            printf("Memory not allocated properly!\n");
        free(ptrToWord);
    }
    else
        printf("Memory not allocated properly!\n");
    free(ptrToBuffer);
}
//-----------------------------------------------------------------------------
//function that works with buffer
void findWords(char *argv[], char *ptrToBuffer, char *ptrToWord, int bufferLength, int *wordLength, int *lineLength)
{
    char character; //a symbol read from buffer

    for(int i = 0; i < bufferLength; ++i)
    {
        sscanf((ptrToBuffer + i), "%c", &character); //getting a char symbol from buffer

        if(character == '\n') //checking, if the line ended
            (*lineLength) = 0;
        if(character == ' ')
            ++(*lineLength);

        //checking, if the line from file is longer than 255
        if((*lineLength) >= 255)
        {
            reverseWords(&ptrToWord, *wordLength);
            ptrToWord[*wordLength] = '\n';
            ++(*wordLength);
            printResults(argv, ptrToWord, *wordLength);
            *wordLength = 0;

            while(1) //ignoring the rest of the line
            {
                sscanf((ptrToBuffer + i), "%c", &character);
                ++i;

                if(character == '\n' || i >= bufferLength)
                {
                    --i;
                    (*lineLength) = 0;
                    break;
                }
            }
        }
        else if(character == ' ' || character == '\n') //checking if the word has ended
        {
            reverseWords(&ptrToWord, *wordLength);
            ptrToWord[*wordLength] = character;
            ++(*wordLength);
            printResults(argv, ptrToWord, *wordLength);
            *wordLength = 0;
        }
        else //the character belongs to a word
        {
            ptrToWord[*wordLength] = character;
            ++(*wordLength);
            ++(*lineLength);
        }
    }
}
//-----------------------------------------------------------------------------
//function that works with calloc and reverses letter order in words
void reverseWords(char **ptrToWord, int wordLength)
{
    int j = wordLength - 1;

    for(int i = 0; i < j; ++i)
    {
        char copy = *((*ptrToWord) + i);
        *((*ptrToWord) + i) = *((*ptrToWord) + j);
        *((*ptrToWord) + j) = copy;
        --j;
    }
}
//-----------------------------------------------------------------------------
//function that prints out results into a file or command line
void printResults(char *argv[], char *ptrToWord, int wordLength)
{
    if(argv[2] == "empty") //the results will be printed to the terminal
        fwrite(ptrToWord, sizeof(char), wordLength, stdout);

    else //the results will be printed to the results file
    {
        FILE *myFile;
        myFile = fopen(argv[2], "a");
        fwrite(ptrToWord, sizeof(char), wordLength, myFile);
        fclose(myFile);
    }
}
