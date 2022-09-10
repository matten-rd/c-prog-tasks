#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct TemperatureStore {
    unsigned maxTemp;
    unsigned minTemp;
    float avgTemp;
    int currentLogIndex;
    unsigned tempLog[10];
} TemperatureStore;

int is_valid_temperature(char *ch)
{
    int length = strlen(ch);
    if (length < 1) return 0;
    
    for (int i = 0; i < length; i++)
    {
        if (!isdigit((unsigned char)ch[i]))
        {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char const *argv[])
{
    char test[32];
    char *action = test;
    int sc;
    TemperatureStore tempStore = {
        .maxTemp = 0,
        .minTemp = 0,
        .avgTemp = 0.0,
        .currentLogIndex = 0,
        .tempLog = {0}
    };

    while (1)
    {
        sc = scanf("%s", action);
        if (sc != 1) printf("Epic fail");
        
        switch (action[0])
        {
            case 'T':
                if (tempStore.currentLogIndex < 10)
                {
                    // Increment pointer past 'T' and check if the rest is a valid number
                    action++; 
                    if (!is_valid_temperature(action) && atoi(action) >= 0)
                    {
                        printf("Input Error\n");
                    } 
                    else 
                    {
                        unsigned recievedTemp = atoi(action);
                        // Set max temp
                        if (tempStore.maxTemp < recievedTemp || tempStore.currentLogIndex == 0)
                            tempStore.maxTemp = recievedTemp;
                        // Set min temp
                        if (tempStore.minTemp > recievedTemp || tempStore.currentLogIndex == 0)
                            tempStore.minTemp = recievedTemp;

                        // Calculate avg temp
                        tempStore.avgTemp = tempStore.avgTemp+((recievedTemp-tempStore.avgTemp) / (tempStore.currentLogIndex+1));

                        // Add recieved temp to log array
                        tempStore.tempLog[tempStore.currentLogIndex] = recievedTemp;

                        // Increment currentLogIndex
                        tempStore.currentLogIndex++;

                        // Print temperature
                        printf("Received Temperature: %d\n", recievedTemp);
                    }
                } 
                else
                {
                    printf("Log Full\n");
                }
                break;

            case 'A':
                if (tempStore.currentLogIndex > 0) {
                    printf("Average Temperature: %.2f\n", tempStore.avgTemp);
                } else {
                    printf("Average Temperature: N/A\n");
                }
                break;

            case 'N':
                if (tempStore.currentLogIndex > 0) {
                    printf("Minimum Temperature: %d\n", tempStore.minTemp);
                } else {
                    printf("Minimum Temperature: N/A\n");
                }
                break;

            case 'X':
                if (tempStore.currentLogIndex > 0) {
                    printf("Maximum Temperature: %d\n", tempStore.maxTemp);
                } else {
                    printf("Maximum Temperature: N/A\n");
                }
                break;

            case 'L':
                printf("Log: %d entries\n", tempStore.currentLogIndex);
                for (int i = 0; i < tempStore.currentLogIndex; i++)
                {
                    printf("Temperature: %d\n", tempStore.tempLog[i]);
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
    

    return 0;
}
