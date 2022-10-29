#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// Enumeration of codes for errors that the program provides:
// Program returns "1" if user uses main search request and tries to add more than 1 argument
// Program returns "2" if user's argument for main search request is not a number
// Program returns "3" if contact database "seznam.txt" contains an empty line
// Program returns "4" if contact database "seznam.txt" contains two lines with same data type in row
// Program returns "5" if user's argument for implicit search request is not a number
// Program returns "6" if user haven't add argumet for Levenshtein Distance
// Program returns "7" if user's argument for Levenshtein Distance is not a number

// Declares a data structure that will contain information about the name and phone number of the contact
typedef struct contacts {
    char name[101];
    char number[101];
} Contact;

// Finds string length
int lng(char str[]) {
    int length = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        length++;
    }

    return length;
}

// Converts characters to digits according to the rule: 2(abc), 3(def), 4(ghi), 5(jkl), 6(mno), 7(pqrs), 8(tuv), 9(wxyz), 0(+)
char *charToDigit(char *str){

    for(int i = 0; i < lng(str); i++){
        if(str[i] == 'a' || str[i] == 'b' || str[i] == 'c'){
            str[i] = '2';
        } else if(str[i] == 'd' || str[i] == 'e' || str[i] == 'f'){
            str[i] = '3';
        } else if(str[i] == 'g' || str[i] == 'h' || str[i] == 'i'){
            str[i] = '4';
        } else if(str[i] == 'j' || str[i] == 'k' || str[i] == 'l'){
            str[i] = '5';
        } else if(str[i] == 'm' || str[i] == 'n' || str[i] == 'o'){
            str[i] = '6';
        } else if(str[i] == 'p' || str[i] == 'q' || str[i] == 'r' || str[i] == 's'){
            str[i] = '7';
        } else if(str[i] == 't' || str[i] == 'u' || str[i] == 'v'){
            str[i] = '8';
        } else if(str[i] == 'w' || str[i] == 'x' || str[i] == 'y' || str[i] == 'z'){
            str[i] = '9';
        } else if(str[i] == '+'){
            str[i] = '0';
        }
    }

    return str;
}

// Converts upper case characters to lower case
char *toLowerCase(char *str){

    for(int i = 0; i < lng(str); i++){
        if(str[i] >= 'A' && str[i] <= 'Z'){
            str[i] += 'a' - 'A';
        }
    }
    
    return str;
} 

// Checks whether the string is a name (contains only letters, space and dot, ex. Robert Downey Jr.)
// in the case of the "name" modifier, 
// or whether the string is a number (contains only numbers and "+")
// in the case of the "number" modifier
bool symbolChecker(char *str){

        for(int i = 0; i < lng(str) - 1; i++){
            if(!(str[i] >= '0' && str[i] <= '9') && str[i] != '+'){
                return false;
            }
        }

    return true;
}

// Не дублирует symbolChecker, потому что тут нет плюса
// Checks if a string contains only digits
bool isDigit(char *str){

    for(int i = 0; i < lng(str); i++){
        if(str[i] < '0' || str[i] > '9'){
            return false;
        }
    }

    return true;
}

// Compares a string with the request, where each subsequent character from the request in the string 
// being checked does not have to be the next one, there may be more characters between them
bool implicitComparison(char *str, char *request){
    int flag = 0;

    // Additional check for an empty request, so that the function ends immediately and does not go into a loop

    if(strstr("", request)){
        return true;
    }

    for (int i = 0; i < lng(str); i++){
        if(str[i] == request[0] && lng(request) <= lng(str) - i){
            for (int j = i; j < lng(str); j++){
                if(str[j] == request[flag]){
                    flag++;
                }

                if(flag == lng(request)){
                    return true;
                }
            }

            return false;
        }
    }

    return false;
}

// Finds the minimum value among three given numbers (used to calculate in the levenshteinComparison function)
int minimumOfThree(int d1, int d2, int d3){
    int buffer = d1 < d2 ? d1 : d2;

    buffer = buffer < d3 ? buffer : d3;

    return buffer;
}

// Finds the number of actions to perform (delete, add, replace) to get from string 1 to string 2 or vice versa
int levenshteinComparison(char *str1, char *str2){
    // Distance matrix initialization
    int distance[101][101];

    // Initial filling of the first row and first column of the matrix
    for(int i = 0; i < lng(str1) + 1; i++){
        for (int j = 0; j < lng(str2) + 1; j++){
            distance[i][j] = (i * j == 0 ? (i + j) : 0); 
        }
    }

    // Filling the rest of the matrix by calculating minimum distance between removing, adding, and replacing an element
    for(int i = 1; i < lng(str1) + 1; i++){
        for (int j = 1; j < lng(str2) + 1; j++){
            if(str1[i-1] == str2[j - 1]){
                distance[i][j] = distance[i-1][j-1];
            } else {
                distance[i][j] = 1 + minimumOfThree(distance[i-1][j], distance[i][j-1], distance[i-1][j-1]);
            }
        }
    }

    return distance[lng(str1)][lng(str2)];
}

// Prints the search result to the terminal, depending on the type of search (main, implicit, Levenshtein) 
void printResults(Contact contactsArr[], char *request, int contactsSize, int modifier, int levenshteinDistance){
    int counter = 0;

    // Creating buffer strings that will be validated so as not to modify database elements
    char nameBuffer[101], numberBuffer[101];

    for(int i = 0; i < contactsSize; i++){
        strcpy(nameBuffer, contactsArr[i].name);
        strcpy(numberBuffer, contactsArr[i].number);

        // Condition for choosing which type of search to perform
        switch (modifier){
            case 1:
                if((strstr(charToDigit(toLowerCase(nameBuffer)), request) || strstr(charToDigit(toLowerCase(numberBuffer)), request)) && 
                strcmp(nameBuffer, "") && strcmp(numberBuffer, "")){
                    printf("%s, %s\n", contactsArr[i].name, contactsArr[i].number);
                    counter++;
                }
                break;
            case 2:
                if((implicitComparison(charToDigit(toLowerCase(nameBuffer)), request) || implicitComparison(charToDigit(contactsArr[i].number), request)) && 
                strcmp(contactsArr[i].name, "") && strcmp(contactsArr[i].number, "")){
                    printf("%s, %s\n", contactsArr[i].name, contactsArr[i].number);
                    counter++;
                }
                break;
            case 3:
                if(((levenshteinComparison(charToDigit(numberBuffer), request) <= levenshteinDistance) ||
                (levenshteinComparison(charToDigit(toLowerCase(nameBuffer)), request) <= levenshteinDistance)) &&
                strcmp(contactsArr[i].name, "") && strcmp(contactsArr[i].number, "")){
                    printf("%s, %s\n", contactsArr[i].name, contactsArr[i].number);
                    counter++;
                }
                break;
        }
    }

    if(!counter){
        printf("Not found\n");
    }
}


int main(int argc, char *argv[]){

    int modifier = 0;
    int levenshteinDistance = 0;

    // Conditions for defining the search type (main, implicit, Levenshtein) 
    // and error print in case of incorrectly passed arguments
    if(argc > 1 && strstr("-s", argv[1])){
        modifier = 2;

        if(argc == 2){
            argv[2] = "";
        } else if (argc == 3 && !(isDigit(argv[2]))){
            fprintf(stderr, "Invalid search request. Type some digits.\n");
            return 5;
        }
    } else if(argc > 1 && strstr("-l", argv[1])){
        modifier = 3;

        if(argc < 3){
            fprintf(stderr, "Invalid search request. You forgot to type levenstein distance as an argument.\n");
            return 6;
        } else if(!(isDigit(argv[2]))){
            fprintf(stderr, "Invalid search request. Levenshtein distance must be a number.\n");
            return 7;
        } else {
            levenshteinDistance = atoi(argv[2]);
        }

        if(argc == 3){
            argv[3] = "";
        }
    } else {
        modifier = 1;

        if(argc > 2){
            fprintf(stderr, "You must type in only 1 argument for simple search.\n");
            return 1;
        } else if (argc == 2 && !(isDigit(argv[1]))){
            fprintf(stderr, "Invalid search request. Type some digits.\n");
            return 2;
        } else if (argc == 1){
            argv[1] = "";
        } 
    }
    
    // Initializing an array of 100 contacts and assigning the variables "name" and "number" to empty values
    Contact contactsArr[100] = {{"", ""}};

    // Calculation of array size (this formula is more applicable to dynamic arrays, 
    // but in case of static allocation of array size it would still 
    // be more flexible to calculate the length of the array and set it as a variable)
    int contactsSize = sizeof(contactsArr) / sizeof(Contact);
    
    // Creation of a 100-character string, which will store the strings from the file with contacts
    // And creation of a buffer string, which will store the values of the string under the index i-1 for future checks
    char str[101], str2uffer[101];
    str[100] = '\0';
    str2uffer[100] = '\0';

    // ТУТ Х2
    //FILE *f;
    //f = fopen("seznam.txt", "r");
 
    int index = 0;
    // ТУТ
    // A loop for distributing contact data from a file to the Contacts structure
    while(fgets(str, 101, stdin) != NULL){

        index++;
        // Calculation of indixes for distribution to the Contacts array
        int del = (index % 2) ? index / 2 : index / 2 - 1;

        if(lng(str) < 100) {
            str[lng(str) - 1] = '\0';
        }

        if(!(symbolChecker(str) || symbolChecker(str2uffer)) || 
        (symbolChecker(str) && symbolChecker(str2uffer))){
            fprintf(stderr, "Somewhere around line '%d', two lines in a row are either a name or a number, or line is bigger than 100 symbols.\n", index);
            fprintf(stderr, "Please correct the data.\n");
            return 4;
        }

        if(strstr("\n", str)){
            fprintf(stderr, "You have an empty line by index '%d'.", index);
            return 3;
        } else if (symbolChecker(str)){
            strcpy(contactsArr[del].number, str);
        } else {
            strcpy(contactsArr[del].name, toLowerCase(str));
        }

        // Copy of last string in buffer to compare with a new string in next iteration
        strcpy(str2uffer, str);
    }

    // ТУТ
    //fclose(f);

    printResults(contactsArr, argv[modifier], contactsSize, modifier, levenshteinDistance);

    return 0;
}