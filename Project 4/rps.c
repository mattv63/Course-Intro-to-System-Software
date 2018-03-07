
/*****************************************************************************************************
* RockPaperScissors -- Program to simulate a best of five game of the classic rock, paper scissors   *
*                                                                                                    *
* Author: Matt Viego                                                                                 *
*                                                                                                    *
* Usage: After running RPS, the program will display a welcome message and ask the user if they'd    *
* like to play. Upon answering "yes", the user will be asked to choose between input "rock", "paper",*
* or "scissors". The program will then convert the inputted string into a number between 0 or 2 for  *
* easier comparison to the computer's selection made by a random number generator. The comparison    *
* will either result in a win, loss or tie for the user. User wins and computer wins are totaled     *
* until one of them equals 3. At this time we will have a winner and the program will ask the user if*
* they would like to play again.                                                                     *
*****************************************************************************************************/

#include <stdio.h>
#include <string.h>

      /****************************
      *    Prototype Functions    *
      ****************************/

int checkwinner(int *userwins, int *compwins);
int play(int *userwins, int *compwins);
int start();

/*
 *                                       Check Winner Function
 *^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 * Parameters: 1. Pointer to where the number of user wins is being stored.
 *             2. Pointer to where the number of computer wins is being stored.
 *
 * This function will first print out a statement that shows what the current score is between the
 * user and the computer by dereferencing pointers. Then by using an if statement, it will check to
 * see if either the user or computer has accumulated enough match wins to win the game.
 *
 */

int checkwinner(int *userwins, int *compwins){

        printf("\n\nThe score is now you: %d computer: %d", *userwins, *compwins);

        if (*userwins == 3){
                printf("\nCongratulations! You win!");
        }
        else if(*compwins == 3){
                printf("\nComputer wins.");
        }
        return(0);
}

/*
 *                                          Play Function
 *^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 * Parameters: 1. Pointer to where the number of user wins is being stored.
 *             2. Pointer to where the number of computer wins is being stored.
 *
 * This function will do most of the work in the program. We start out with asking for the user's choice and recording that.
 * The function will hold the player choice as a number between 0 and 2. The program will then compare the user selection to
 * the random number generated for the computer. A win will be added to the location pointed to by the parameters if applicable
 *
 */


int play(int *userwins, int *compwins, int driver){

        char userplay[10];
        char compplay[10];
        char result[10];
        int c;
        int _userplay = 0;
        char _compplay = getRandom(driver);

        CANNOTSPELL:

        printf("\nWhat is your choice? ");

        fgets(userplay, sizeof(userplay), stdin);

        if (strcmp(userplay, "rock\n") == 0){
                _userplay = 0;
        }

        else if(strcmp(userplay, "paper\n") == 0){
                _userplay = 1;
        }

        else if(strcmp(userplay, "scissors\n") == 0){
                _userplay = 2;
        }

        else{
                printf("\nNot a valid choice. Choose rock, paper or scissors. It is literally the name of the game. I can't make this any easier for you. C'mon man.");
                goto CANNOTSPELL;
        }


        if (_compplay == '0'){
                strcpy(compplay, "rock");
                if (_userplay == 0){
                        strcpy(result, "tied");
                }
                else if(_userplay == 1){
                        strcpy(result, "win");
                        *userwins += 1;
                }
                else if(_userplay == 2){
                        strcpy(result, "lose");
                        *compwins += 1;
                }
                else{
                        printf("Error!");
                }
        }

         else if (_compplay == '1'){
                strcpy(compplay, "paper");
                if (_userplay == 0){
                        strcpy(result, "lose");
                        *compwins += 1;
                }
                else if(_userplay == 1){
                        strcpy(result, "tied");
                }
                else if(_userplay == 2){
                        strcpy(result, "win");
                        *userwins += 1;
                }
                else{
                        printf("Error!");
                }
        }
         else if (_compplay == '2'){
                strcpy(compplay, "scissors");
                if (_userplay == 0){
                        strcpy(result, "win");
                        *userwins += 1;
                }
                else if(_userplay == 1){
                        strcpy(result, "lose");
                        *compwins += 1;
                }
                else if(_userplay == 2){
                        strcpy(result, "tied");
                }
                else{
                        printf("Error!");
                }
        }

        printf("The computer chooses %s. You %s this game.", compplay, result);



        return (0);
}

char getRandom(int driver){
		char x;
		read(driver, &x, 1);
		return x;
}

/*
 * Start() is simply a function that will begin the out to the program and ask the user if he/she would like to play.
 * If the user types "yes" then the program will continue by returning a 0 to main. If an input other then "yes" is
 * input the function will return 1 and the program will exit.
 *
 */

int start(){

        int x, c;

        char start[4];

        printf("Welcome to Rock, Paper, Scissors\n\n");
        printf("Would you like to play? ");

        fgets(start, sizeof(start), stdin);
        while ((c = getchar()) != '\n' && c != EOF);
        x = strcmp(start, "yes");

        if (x != 0){
                return (1);
        }

        return (0);
}

int main(){

        int userwins = 0, compwins = 0;  /* Holds number of wins for user and computer */
        char playagain[5];               /* char array that holds user input from start() */

        srand((unsigned int)time(NULL)); /* RANDOM */
		int driver = open("/dev/rps", O_RDONLY);
		if (driver == -1){
			printf("An error occured while opening the driver");
			return -1;
		}

        if (start() == 0){
                KEEPPLAYING:
                        play(&userwins, &compwins, driver);
                        checkwinner(&userwins, &compwins);
                        if (userwins == 3 || compwins == 3){
                                printf("\n\nWould you like to play again? ");
                                fgets(playagain, sizeof(playagain), stdin);
                                if(strcmp(playagain, "yes\n") == 0){
                                        userwins = 0;
                                        compwins = 0;
                                        goto KEEPPLAYING;
                                }
                                else{
                                        printf("Come back again soon!\n");
                                        return (0);
                                }
                        }
                        goto KEEPPLAYING;
        }

        return(0);

}
