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

int main(int argc, char const *argv[])
{
    char action[3];

    TemperatureStore tempStore = {
        .currentLogIndex = 0,
        .avgTemp = 0.0
    };

    while (1)
    {
        scanf("%s", action);
        int actionLenght = strlen(action);
        if (actionLenght > 1)
        {
            if (actionLenght == 3 && action[0] == 'T' && isdigit((unsigned char)action[1]) && isdigit((unsigned char)action[2]))
            {
                if (tempStore.currentLogIndex < 3)
                {
                    // Extract recieved temperature
                    char subString[3];
                    memcpy(subString, &action[1], 2);
                    subString[2] = '\0';
                    unsigned recievedTemp = atoi(subString);

                    // Set min and max temp
                    if (tempStore.maxTemp < recievedTemp || tempStore.currentLogIndex == 0) tempStore.maxTemp = recievedTemp;
                    if (tempStore.minTemp > recievedTemp || tempStore.currentLogIndex == 0) tempStore.minTemp = recievedTemp;

                    // Calculate avg temp
                    tempStore.avgTemp = tempStore.avgTemp + ((recievedTemp - tempStore.avgTemp) / (tempStore.currentLogIndex+1));

                    // Add recieved temp to log array
                    tempStore.tempLog[tempStore.currentLogIndex] = recievedTemp;

                    // Increment "pointer" to current index
                    tempStore.currentLogIndex++;

                    // Print temperature
                    printf("Received Temperature: %d\n", recievedTemp);
                } else
                {
                    printf("Log Full\n");
                }
            } else
            {
                printf("Input Error Length\n");
            }
        } else
        {
            switch (action[0])
            {
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
                    exit(1);
                    break;

                default:
                    printf("Input Error\n");
                    break;
            }
        }
    }

    return 0;
}
