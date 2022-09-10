#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Struct to store the log data in
typedef struct TemperatureStore {
    unsigned maxTemp;
    unsigned minTemp;
    float avgTemp;
    int currentLogIndex;
    unsigned tempLog[10];
} TemperatureStore;

int is_valid_number(char *ch)
{
    // To catch when input is just 'T'
    int length = strlen(ch);
    if (length < 1) return 0;
    
    // Checks if any of the chars are NOT digits and returns 0 in that case
    for (int i = 0; i < length; i++)
    {
        if (!isdigit((unsigned char)ch[i])) return 0;
    }
    // Return 1 if all chars are digits
    return 1;
}

void handle_user_request(char action, TemperatureStore *tempStore)
{
    // Handles all possible user actions
    switch (action)
    {
        case 'A':
            if (tempStore->currentLogIndex > 0)
            {
                printf("Average Temperature: %.2f\n", tempStore->avgTemp);
            }
            else
            {
                printf("Average Temperature: N/A\n");
            }
            break;

        case 'N':
            if (tempStore->currentLogIndex > 0)
            {
                printf("Minimum Temperature: %d\n", tempStore->minTemp);
            }
            else
            {
                printf("Minimum Temperature: N/A\n");
            }
            break;

        case 'X':
            if (tempStore->currentLogIndex > 0)
            {
                printf("Maximum Temperature: %d\n", tempStore->maxTemp);
            }
            else
            {
                printf("Maximum Temperature: N/A\n");
            }
            break;

        case 'L':
            printf("Log: %d entries\n", tempStore->currentLogIndex);
            for (int i = 0; i < tempStore->currentLogIndex; i++)
            {
                printf("Temperature: %d\n", tempStore->tempLog[i]);
            }
            break;

        case 'Q':
            printf("Exiting...\n");
            exit(0);
            break;

        default:
            printf("Input Error\n");
            break;
    }
}

void handle_temp_log(char *temperature, TemperatureStore *tempStore) 
{
    // Convert to int (checked previously that this is safe)
    unsigned receivedTemp = atoi(temperature);
    // Set max temp
    if (tempStore->maxTemp < receivedTemp || tempStore->currentLogIndex == 0)
        tempStore->maxTemp = receivedTemp;
    // Set min temp
    if (tempStore->minTemp > receivedTemp || tempStore->currentLogIndex == 0)
        tempStore->minTemp = receivedTemp;

    // Calculate avg temp
    tempStore->avgTemp = tempStore->avgTemp + ((receivedTemp - tempStore->avgTemp) / (tempStore->currentLogIndex + 1));

    // Add received temp to log array
    tempStore->tempLog[tempStore->currentLogIndex] = receivedTemp;

    // Increment currentLogIndex
    tempStore->currentLogIndex++;

    // Print temperature
    printf("Received Temperature: %d\n", receivedTemp);
}

int main(int argc, char const *argv[])
{
    // Initialise a pointer for the log input
    char arr[10];
    char *logInput = arr;

    int sc;

    TemperatureStore tempStore = {
        .avgTemp = 0.0,
        .currentLogIndex = 0
    };

    while (1)
    {
        // read log input
        sc = scanf("%s", logInput);
        if (sc != 1) printf("Epic fail");

        if (strlen(logInput) == 1) // User commands are always 1 char
        {
            handle_user_request(logInput[0], &tempStore);
        }
        else if (logInput[0] == 'T')
        {
            // Increment pointer past 'T' and check if the rest is a valid number (T20 -> 20)
            logInput++;
            if (!is_valid_number(logInput) && atoi(logInput) >= 0)
            {
                printf("Input Error\n");
            } else if (tempStore.currentLogIndex < 10) // Check that log is not full
            {
                handle_temp_log(logInput, &tempStore);
            }
            else
            {
                printf("Log Full\n");
            }
        } else // All other input is not allowed
        {
            printf("Input Error\n");
        }
    }
    return 0;
}
