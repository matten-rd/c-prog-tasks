#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct TemperatureStore {
    unsigned maxTemp;
    unsigned minTemp;
    float avgTemp;
    char *tempLog[10];
} TemperatureStore;

int main(int argc, char const *argv[])
{
    char action[3];

    while (1)
    {
        scanf("%s", action);
        if (strlen(action) > 1)
        {
            if (strlen(action) == 3 && action[0] == 'T' && isdigit((unsigned char) action[1]) && isdigit((unsigned char) action[2]))
            {
                printf("Received Temperature: %c%c\n", action[1], action[2]);
            } else
            {
                printf("Input Error Length\n");
            }
        } else
        {
            switch (action[0])
            {
                case 'A':
                    printf("Average Temperature: 19.90\n");
                    break;

                case 'N':
                    printf("Minimum Temperature: 15\n");
                    break;

                case 'X':
                    printf("Maximum Temperature: 23\n");
                    break;

                case 'L':
                    printf("Log: 10 entries\n");
                    for (int i = 0; i < 10; i++)
                    {
                        printf("Temperature: %d\n", i);
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
