/** @file route_manager.c
 *  @brief A pipes & filters program that uses conditionals, loops, and string processing tools in C to process airline routes.
 *  @author Felipe R.
 *  @author Hausi M.
 *  @author Jose O.
 *  @author Saasha J.
 *  @author Victoria L.
 *  @author Chaiitanyaa C.
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Function: main
 * --------------
 * @brief The main function and entry point of the program.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv The list of arguments passed to the program.
 * @return int 0: No errors; 1: Errors produced.
 *
 */

char line[256];
char fileToRead[20];

char airline[20];
char to_country[20];

char from_country[20];
char to_city[20];
char to_country[20];

char from_city[20];

// Function Prototypes
void get_arguments(int a, char *arguments[]);

void two_arguments();
void three_arguments();
void four_arguments();

// Main function
int main(int argc, char *argv[])
{
    get_arguments(argc, argv);

    // Use Case 1: There are 2 user arguments to filter
    if (argc == 4)
    {
        two_arguments();
        return 1;
    }

    // Use Case 2: there are 3 arguments to filter
    if (argc == 5)
    {
        three_arguments();
        return 1;
    }

    // Use Case 3: there are 4 arguments to filter
    else if (argc == 6)
    {
        four_arguments();
        return 1;
    }

    return 0;
    exit(0);
}

/*Function to get arguments from the command line and stores it into variables*/
void get_arguments(int no_of_args, char *argv[])
{
    sscanf(argv[1], "--DATA=%s", fileToRead);

    // Prints Out an error message if no file is given to access the data
    if (no_of_args < 2)
    {
        printf("Error: No file specified\n");
    }

    // Prints Out an error if the file has just 1 argument
    if (no_of_args < 3)
    {
        printf("Not enough arguments\n");
    }

    // Takes input from command line of airline, Dest_country to search for.
    if (no_of_args == 4)
    {
        sscanf(argv[2], "--AIRLINE=%[^\n]", airline);
        sscanf(argv[3], "--DEST_COUNTRY=%[^\n]", to_country);
    }

    // Takes input from command line of Src_country, Dest_city and Dest_country to search for.
    if (no_of_args == 5)
    {
        sscanf(argv[2], "--SRC_COUNTRY=%[^\n]", from_country);
        sscanf(argv[3], "--DEST_CITY=%[^\n]", to_city);
        sscanf(argv[4], "--DEST_COUNTRY=%[^\n]", to_country);
    }

    // Takes input from command line of Src_city, Src_country, Dest_city and Dest_country to search for
    if (no_of_args == 6)
    {
        sscanf(argv[2], "--SRC_CITY=%[^\n]", from_city);
        sscanf(argv[3], "--SRC_COUNTRY=%[^\n]", from_country);
        sscanf(argv[4], "--DEST_CITY=%[^\n]", to_city);
        sscanf(argv[5], "--DEST_COUNTRY=%[^\n]", to_country);
    }
}

/*
 * This function reads the .csv file given as the first argument in command line
 * and then reads the file and outputs the filtered output as per the 2 arguments
 * given in command line after the file name in output.txt
 */
void two_arguments()
{

    FILE *fw;
    fw = fopen("output.txt", "w");

    FILE *file = fopen(fileToRead, "r");
    if (file == NULL)
    {
        printf("Error: Unable to open file\n");
        return; // exit the program
    }

    int test = 0;
    int print_count = 0;
    char routes_found[14][256];
    int count_routes_found = 0;

    // loop to read the csv file line by line
    while (fgets(line, sizeof(line), file))
    {
        int counter = 0;
        char routes[14][256];
        char *token = strtok(line, ",");

        // loop seperate the data in to an array
        while (token != NULL)
        {
            strcpy(routes[counter], token);
            counter++;
            token = strtok(NULL, ",");
        }

        // Comparing the inputted data with the array of data
        if (strcmp(airline, routes[1]) == 0)
        {
            if (strcmp(to_country, routes[10]) == 0)
            {
                if (print_count == 0)
                {
                    fprintf(fw, "FLIGHTS TO %s BY %s (%s):\n", to_country, routes[0], airline);
                    print_count = 1;
                }

                fprintf(fw, "FROM: %s, %s, %s TO: %s (%s), %s\n", routes[6], routes[4], routes[5], routes[8], routes[11], routes[9]);

                test = 1;
            }
        }
    }
    if (test == 0)
    {
        fputs("NO RESULTS FOUND.\n", fw);
    }

    // closing the files after use
    fclose(file);
    fclose(fw);
}

/*
 * This function reads the .csv file given as the first argument in command line
 * and then reads the file and outputs the filtered output as per the 3 arguments
 * given in command line after the file name in output.txt
 */
void three_arguments()
{

    FILE *fw;
    fw = fopen("output.txt", "w");

    FILE *file = fopen(fileToRead, "r");
    if (file == NULL)
    {
        printf("Error: Unable to open file\n");
    }

    int test = 0;
    int count_routes_found = 0;

    // loop to read the csv file line by line
    while (fgets(line, sizeof(line), file))
    {

        int counter = 0;
        char routes[14][256];
        char *token = strtok(line, ",");

        // loop seperate the data in to an array
        while (token != NULL)
        {

            strcpy(routes[counter], token);
            counter++;
            token = strtok(NULL, ",");
        }

        // Comparing the inputted data with the array of data
        if (strcmp(from_country, routes[5]) == 0)
        {
            if (strcmp(to_city, routes[9]) == 0)
            {
                if (strcmp(to_country, routes[10]) == 0)
                {
                    if (count_routes_found == 0)
                    {
                        fprintf(fw, "FLIGHTS FROM %s TO %s, %s:\n", from_country, to_city, to_country);
                        count_routes_found = 1;
                    }
                    fprintf(fw, "AIRLINE: %s (%s) ORIGIN: %s (%s), %s\n", routes[0], routes[1], routes[3], routes[6], routes[4]);
                    test = 1;
                }
            }
        }
    }
    if (test == 0)
    {
        fprintf(fw, "NO RESULTS FOUND.\n");
    }

    // closing the files after use
    fclose(file);
    fclose(fw);
}

/*
 * This function reads the .csv file given as the first argument in command line
 * and then reads the file and outputs the filtered output as per the 4 arguments
 * given in command line after the file name in output.txt
 */
void four_arguments()
{

    FILE *fw;
    fw = fopen("output.txt", "w");

    int test = 0;
    int count_routes_found = 0;

    FILE *file = fopen(fileToRead, "r");
    if (file == NULL)
    {
        printf("Error: Unable to open file\n");
    }

    // loop to read the csv file line by line
    while (fgets(line, sizeof(line), file))
    {
        int counter = 0;
        char routes[14][256];
        char *token = strtok(line, ",");

        // loop seperate the data in to an array
        while (token != NULL)
        {
            strcpy(routes[counter], token);
            counter++;
            token = strtok(NULL, ",");
        }

        // Comparing the inputted data with the array of data
        if (strcmp(from_city, routes[4]) == 0)
        {
            if (strcmp(from_country, routes[5]) == 0)
            {
                if (strcmp(to_city, routes[9]) == 0)
                {
                    if (strcmp(to_country, routes[10]) == 0)
                    {
                        if (count_routes_found == 0)
                        {
                            fprintf(fw, "FLIGHTS FROM %s, %s TO %s, %s:\n", from_city, from_country, to_city, to_country);
                            count_routes_found = 1;
                        }
                        fprintf(fw, "AIRLINE: %s (%s) ROUTE: %s-%s\n", routes[0], routes[1], routes[6], routes[11]);
                        test = 1;
                    }
                }
            }
        }
    }
    if (test == 0)
    {
        fprintf(fw, "NO RESULTS FOUND.\n");
    }

    // closing the files after use
    fclose(file);
    fclose(fw);
}
