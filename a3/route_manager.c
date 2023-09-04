/** @file route_manager.c
 *  @brief A small program to analyze airline routes data.
 *  @author Mike Z.
 *  @author Felipe R.
 *  @author Hausi M.
 *  @author Jose O.
 *  @author Saasha J.
 *  @author Victoria L.
 *  @author Chaiitanyaa C.
 *
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

char fileToRead[40];
char question[5];
char N[5];

// TODO: Make sure to adjust this based on the input files given
#define MAX_LINE_LENGTH 80
typedef struct
{
    char *airline_name;
    char *airline_icao_unique_code;
    char *airline_country;
    char *from_airport_name;
    char *from_airport_city;
    char *from_airport_country;
    char *from_airport_icao_unique_code;
    char *from_airport_altitude;
    char *to_airport_name;
    char *to_airport_city;
    char *to_airport_country;
    char *to_airport_icao_unique_code;
    char *to_airport_altitude;
} Route;

/**
 * @brief Serves as an incremental counter for navigating the list.
 *
 * @param p The pointer of the node to print.
 * @param arg The pointer of the index.
 *
 */
void inccounter(node_t *p, void *arg)
{
    int *ip = (int *)arg;
    (*ip)++;
}

/**
 * @brief Allows to print out the content of a node.
 *
 * @param p The pointer of the node to print.
 * @param arg The format of the string.
 *
 */
void print_node(node_t *p, void *arg)
{
    char *fmt = (char *)arg;
    printf(fmt, p->word);
}

/**
 * @brief Allows to print each node in the list.
 *
 * @param l The first node in the list
 *
 */
void analysis(node_t *l)
{
    int len = 0;

    apply(l, inccounter, &len);
    printf("Number of words: %d\n", len);

    apply(l, print_node, "%s\n");
}

/**
 * @brief Gets the Arguments that the user passes from the command-line
 *
 * @param no_of_args the count of the arguments passed
 * @param argv[] The array of arguments passed
 *
 */
void get_arguments(int no_of_args, char *argv[])
{
    // Prints Out an error message if no file is given to access the data
    if (no_of_args < 2)
    {
        printf("Error: No file specified\n");
    }

    // Prints Out an error if the file has just 1 argument
    else if (no_of_args < 3)
    {
        printf("Not enough arguments\n");
    }

    else
    {
        sscanf(argv[1], "--DATA=%s", fileToRead);
        sscanf(argv[2], "--QUESTION=%[^\n]", question);
        sscanf(argv[3], "--N=%[^\n]", N);
    }
}

/**
 * @brief removes the white space from the front while parsing
 *
 * @param value the value to remove the whitespace from
 *
 */
void removeWhitespace(char *value)
{
    int i, j;

    // loop through the array starting from the third character
    for (i = 1, j = 0; value[i] != '\0'; i++, j++)
    {
        value[j] = value[i]; // shift the characters two positions to the left
    }

    value[j] = '\0'; // add null character to terminate the stri
}

/**
 * @brief Prints the data obtained into a CSV file according to the given user size
 *
 * @param data the filtered data from the program
 * @param N the size of the data wanted passed by the user
 *
 */
void exportCSV(node_t *data, int N)
{

    FILE *output = fopen("output.csv", "w+"); // opens a file in write mode
    // grabs the first (head) node of the final linked list
    node_t *curr;
    int count = 0; // counter for comparing with display variable
    fprintf(output, "subject,statistic\n");
    for (curr = data; curr != NULL; curr = curr->next)
    {
        if (count < N)
        {
            fprintf(output, "%s\n", curr->word);
        }
        count++;
    }
}

/**
 * @brief Question 1 handler(produces the correct output to answer q1)
 *
 * @return int 0: No errors; 1: Errors produced.
 *
 */
int questionOne()
{
    int dataReq = atoi(N);

    // the name of the .yaml file
    node_t *airlines = NULL;
    FILE *fp = fopen(fileToRead, "r"); // open the file for reading

    if (!fp)
    {
        fprintf(stderr, "Failed to open file: %s\n", fileToRead);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    // read and ignore the first line
    fgets(line, MAX_LINE_LENGTH, fp);

    char *key = NULL;
    char *value = NULL;

    Route myroute;
    myroute.airline_name = malloc(250 * sizeof(char));
    myroute.airline_icao_unique_code = malloc(10 * sizeof(char));
    myroute.to_airport_country = malloc(250 * sizeof(char));

    while (fgets(line, MAX_LINE_LENGTH, fp))
    {
        // skip lines that contain only whitespace
        if (strspn(line, " \t\n\r\v\f") == strlen(line))
        {
            continue;
        }
        key = strtok(line, ":");
        value = strtok(NULL, "\n");

        // Reads all the data that will be relevent to us for the question
        if (line[0] == '-')
        {
            removeWhitespace(value);
            strcpy(myroute.airline_name, value);
        }
        else if (strcmp(key, "  airline_icao_unique_code") == 0)
        {
            removeWhitespace(value);
            strcpy(myroute.airline_icao_unique_code, value);
        }
        else if (strcmp(key, "  to_airport_country") == 0)
        {
            removeWhitespace(value);
            strcpy(myroute.to_airport_country, value);
        }

        if (strcmp(value, "Canada") == 0)
        {
            char *required = malloc(500 * sizeof(char));
            strcpy(required, myroute.airline_name);
            strcat(required, " (");
            strcat(required, myroute.airline_icao_unique_code);
            strcat(required, "),");
            node_t *temp_node = new_node(required, 0);
            // airlines = add_front(airlines, temp_node);
            airlines = add_inorder(airlines, temp_node);
            free(required);
        }
    }
    node_t *curr;
    int count = 1;
    node_t *airlines_final = NULL;
    node_t *temp2 = NULL;
    char *val = malloc(500 * sizeof(char));
    temp2 = new_node(val, 0);

    for (curr = airlines; curr != NULL; curr = curr->next)
    {
        if (curr->next != NULL && strcmp(curr->word, curr->next->word) == 0)
        {
            count += 1;
        }
        else
        {
            char str[20];
            char *word_1 = malloc(500 * sizeof(char));
            strcpy(word_1, curr->word);
            sprintf(str, "%d", count);
            strcat(word_1, str);
            temp2 = new_node(word_1, count);
            free(word_1);
            if (airlines_final == NULL)
            {
                airlines_final = add_front(airlines_final, temp2);
            }
            else
            {
                airlines_final = sortDecending(airlines_final, temp2);
            }

            count = 1;
        }
    }

    exportCSV(airlines_final, dataReq);

    fclose(fp);
    node_t *tempNode1 = NULL;
    node_t *tempNode2 = NULL;

    free(myroute.to_airport_country);
    free(myroute.airline_name);
    free(myroute.airline_icao_unique_code);
    free(val);

    tempNode1 = airlines;
    while (tempNode1 != NULL)
    {
        assert(tempNode1 != NULL);
        airlines = remove_front(airlines);
        free(tempNode1);
        tempNode1 = airlines;
    }

    while (tempNode2 != NULL)
    {
        assert(tempNode2 != NULL);
        airlines_final = remove_front(airlines_final);
        free(tempNode2);
        tempNode2 = airlines_final;
    }
    return 0;
}

/**
 * @brief Question 2 handler (produces the correct output to answer q2).
 *
 * @return int 0: No errors; 1: Errors produced.
 *
 */
int questionTwo()
{
    int dataReq = atoi(N);

    // the name of the .yaml file
    node_t *airlines = NULL;
    FILE *fp = fopen(fileToRead, "r"); // open the file for reading

    if (!fp)
    {
        fprintf(stderr, "Failed to open file: %s\n", fileToRead);
        return 1;
    }

    char line[MAX_LINE_LENGTH];

    // read and ignore the first line
    fgets(line, MAX_LINE_LENGTH, fp);

    char *key = NULL;
    char *value = NULL;

    Route myroute;
    myroute.airline_name = malloc(250 * sizeof(char));
    myroute.airline_icao_unique_code = malloc(10 * sizeof(char));
    myroute.to_airport_country = malloc(250 * sizeof(char));

    while (fgets(line, MAX_LINE_LENGTH, fp))
    {
        // skip lines that contain only whitespace
        if (strspn(line, " \t\n\r\v\f") == strlen(line))
        {
            continue;
        }
        key = strtok(line, ":");
        value = strtok(NULL, "\n");

        // reads the first data set which begins with "-"
        if (line[0] == '-')
        {
            removeWhitespace(value);
            strcpy(myroute.airline_name, value);
        }
        else if (strcmp(key, "  to_airport_country") == 0)
        {
            removeWhitespace(value);
            // To handle exceptions
            if (value[0] == '\'')
            {
                value[0] = ' ';
                removeWhitespace(value);
                removeWhitespace(value);
            }
            if (value[strlen(value) - 1] == '\'')
            {
                value[strlen(value) - 1] = ' ';
                int i = strlen(value) - 1;
                while (i >= 0 && value[i] == ' ')
                {
                    i--;
                }

                // Terminate the string at the last non-whitespace character
                value[i + 1] = '\0';
            }
            strcpy(myroute.to_airport_country, value);

            char *required = malloc(500 * sizeof(char));

            strcpy(required, myroute.to_airport_country);
            strcat(required, ",");
            node_t *temp_node = new_node(required, 0);
            free(required);
            airlines = add_inorder(airlines, temp_node);
        }
    }

    node_t *curr1;

    int count = 1;
    node_t *airlines_final = NULL;
    node_t *temp2 = NULL;

    char *val = malloc(500 * sizeof(char));
    temp2 = new_node(val, 0);

    for (curr1 = airlines; curr1 != NULL; curr1 = curr1->next)
    {
        if (curr1->next != NULL && strcmp(curr1->word, curr1->next->word) == 0)
        {
            count += 1;
        }
        else
        {
            char str[20];
            char *word_1 = malloc(500 * sizeof(char));
            strcpy(word_1, curr1->word);
            sprintf(str, "%d", count);
            strcat(word_1, str);
            temp2 = new_node(word_1, count);
            free(word_1);
            if (airlines_final == NULL)
            {
                airlines_final = add_front(airlines_final, temp2);
            }
            else
            {
                airlines_final = sortAscending(airlines_final, temp2);
            }
            count = 1;
        }
    }

    exportCSV(airlines_final, dataReq);
    fclose(fp);

    node_t *tempNode1 = NULL;
    node_t *tempNode2 = NULL;

    free(myroute.to_airport_country);
    free(myroute.airline_name);
    free(myroute.airline_icao_unique_code);
    free(val);

    tempNode1 = airlines;
    while (tempNode1 != NULL)
    {
        assert(tempNode1 != NULL);
        airlines = remove_front(airlines);
        free(tempNode1);
        tempNode1 = airlines;
    }

    while (tempNode2 != NULL)
    {
        assert(tempNode2 != NULL);
        airlines_final = remove_front(airlines_final);
        free(tempNode2);
        tempNode2 = airlines_final;
    }
    return 0;
}

/**
 * @brief Question 3 handler (produces the correct output to answer q3).
 *
 * @return int 0: No errors; 1: Errors produced.
 */
int questionThree()
{
    int x = atoi(N);

    // the name of the .yaml file
    node_t *airlines = NULL;
    FILE *fp = fopen(fileToRead, "r"); // open the file for reading

    if (!fp)
    {
        fprintf(stderr, "Failed to open file: %s\n", fileToRead);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    // read and ignore the first line
    fgets(line, MAX_LINE_LENGTH, fp);

    char *key = NULL;
    char *value = NULL;
    Route myroute;
    myroute.airline_name = malloc(50 * sizeof(char));
    myroute.to_airport_name = malloc(50 * sizeof(char));
    myroute.to_airport_icao_unique_code = malloc(10 * sizeof(char));
    myroute.to_airport_city = malloc(50 * sizeof(char));
    myroute.to_airport_country = malloc(50 * sizeof(char));

    while (fgets(line, MAX_LINE_LENGTH, fp))
    {
        // skip lines that contain only whitespace
        if (strspn(line, " \t\n\r\v\f") == strlen(line))
        {
            continue;
        }
        key = strtok(line, ":");
        value = strtok(NULL, "\n");

        if (line[0] == '-')
        {
            removeWhitespace(value);
            strcpy(myroute.airline_name, value);
        }
        else if (strcmp(key, "  to_airport_name") == 0)
        {
            removeWhitespace(value);
            strcpy(myroute.to_airport_name, value);
        }
        else if (strcmp(key, "  to_airport_city") == 0)
        {
            removeWhitespace(value);
            strcpy(myroute.to_airport_city, value);
        }

        else if (strcmp(key, "  to_airport_country") == 0)
        {
            removeWhitespace(value);
            strcpy(myroute.to_airport_country, value);
        }
        else if (strcmp(key, "  to_airport_icao_unique_code") == 0)
        {
            removeWhitespace(value);
            strcpy(myroute.to_airport_icao_unique_code, value);

            // Making the string into what we want as output
            char *required = malloc(200 * sizeof(char));
            strcpy(required, "\"");
            strcat(required, myroute.to_airport_name);
            strcat(required, " (");
            strcat(required, myroute.to_airport_icao_unique_code);
            strcat(required, "), ");
            strcat(required, myroute.to_airport_city);
            strcat(required, ", ");
            strcat(required, myroute.to_airport_country);
            strcat(required, "\",");

            node_t *temp_node = new_node(required, 0);
            free(required);

            airlines = add_inorder(airlines, temp_node);
        }
    }

    node_t *curr1;
    int count = 1;
    node_t *airlines_final = NULL;
    node_t *temp2 = NULL;
    char *val = malloc(500 * sizeof(char));
    temp2 = new_node(val, 0);

    for (curr1 = airlines; curr1 != NULL; curr1 = curr1->next)
    {
        if (curr1->next != NULL && strcmp(curr1->word, curr1->next->word) == 0)
        {
            count += 1;
        }
        else
        {
            char str[20];
            char *word_1 = malloc(2000 * sizeof(char));
            strcpy(word_1, curr1->word);
            sprintf(str, "%d", count);
            strcat(word_1, str);
            temp2 = new_node(word_1, count);
            free(word_1);
            if (airlines_final == NULL)
            {
                airlines_final = add_front(airlines_final, temp2);
            }
            else
            {
                airlines_final = sortDecending(airlines_final, temp2);
            }

            count = 1;
        }
    }
    exportCSV(airlines_final, x);
    fclose(fp);

    node_t *tempNode1 = NULL;
    node_t *tempNode2 = NULL;

    free(myroute.airline_name);
    free(myroute.to_airport_name);
    free(myroute.to_airport_icao_unique_code);
    free(myroute.to_airport_city);
    free(myroute.to_airport_country);
    free(val);

    tempNode1 = airlines;
    while (tempNode1 != NULL)
    {
        assert(tempNode1 != NULL);
        airlines = remove_front(airlines);
        free(tempNode1);
        tempNode1 = airlines;
    }

    while (tempNode2 != NULL)
    {
        assert(tempNode2 != NULL);
        airlines_final = remove_front(airlines_final);
        free(tempNode2);
        tempNode2 = airlines_final;
    }
    return 0;
}

/**
 * @brief The main function and entry point of the program.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv The list of arguments passed to the program.
 * @return int 0: No errors; 1: Errors produced.
 *
 */

int main(int argc, char *argv[])
{
    get_arguments(argc, argv);

    if (strcmp(question, "1") == 0)
    {
        questionOne();
    }
    else if (strcmp(question, "2") == 0)
    {
        questionTwo();
    }
    else if (strcmp(question, "3") == 0)
    {
        questionThree();
    }
    else
    {
        return 1;
    }
    return 0;
}