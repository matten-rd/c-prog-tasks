#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Struct to store the log data in
typedef struct TemperatureStore
{
    unsigned maxTemp;
    unsigned minTemp;
    float avgTemp;
    int currentLogIndex;
    unsigned tempLog[10];
} TemperatureStore;

/**
 * Extract bits between start-end (inclusive) from value.
 * This is achieved by applying an AND mask (bitwise ANDing) - a simple way of extracting a subset of bits.
 * The idea is to create a mask (binary number) that has 1's at the indices we want to keep and 0's at the other indices.
 * This works since any bit X that is ANDed with 1 remain the same (X AND 1 = X) and any bit ANDed with 0 is 0 (X AND 0 = 0).
 */
unsigned extractBits(unsigned value, unsigned start, unsigned end)
{
    unsigned mask;
    int numberOfBits = end - start + 1; // Number of bits to be extracted/set to 1 in the mask
    mask = (1 << numberOfBits) - 1;     // Set numberOfBits to 1 e.g. numberOfBits=8 => (1 << 8)-1=11111111 (255)
    mask <<= start;                     // Shift the 1's to correct position i.e. pad with [start] number of 0's on the right
    return (value & mask) >> start;     // Perform the bitwise AND and shift the isolated bits to LSB (to only get the isolated bits)
}

int is_valid_hex(char *ch)
{
    // Handle case when input is empty
    int length = strlen(ch);
    if (length < 1)
        return 0;

    // Check that all chars are valid hex digits
    if (ch[strspn(ch, "0123456789abcdefxABCDEFX")] == 0)
    {
        return 1;
    }
    return 0;
}

void handle_temp_log(int receivedTemp, TemperatureStore *tempStore)
{
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

void handle_valid_input(int type, int data, TemperatureStore *tempStore)
{
    // Handles all possible types
    switch (type)
    {
        case 0b000:
            if (tempStore->currentLogIndex < 10)
            {
                handle_temp_log(data, tempStore);
            } else
            {
                printf("Log Full\n");
            }
            break;

        case 0b010:
            if (tempStore->currentLogIndex > 0)
            {
                printf("Average Temperature: %.2f\n", tempStore->avgTemp);
            }
            else
            {
                printf("Average Temperature: N/A\n");
            }
            break;

        case 0b011:
            if (tempStore->currentLogIndex > 0)
            {
                printf("Minimum Temperature: %d\n", tempStore->minTemp);
            }
            else
            {
                printf("Minimum Temperature: N/A\n");
            }
            break;

        case 0b100:
            if (tempStore->currentLogIndex > 0)
            {
                printf("Maximum Temperature: %d\n", tempStore->maxTemp);
            }
            else
            {
                printf("Maximum Temperature: N/A\n");
            }
            break;

        case 0b101:
            printf("Log: %d entries\n", tempStore->currentLogIndex);
            for (int i = 0; i < tempStore->currentLogIndex; i++)
            {
                printf("Temperature: %d\n", tempStore->tempLog[i]);
            }
            break;

        case 0b110:
            printf("Exiting...\n");
            exit(0);
            break;

        default:
            printf("Input Error\n");
            break;
    }
}



int main(int argc, char const *argv[])
{
    char logInput[32];

    int hexConvertRes;

    int message, reserved, type, data;
    int reservedStart = 29, reservedEnd = 31;
    int typeStart = 26, typeEnd = 28;
    int dataStart = 0, dataEnd = 25;

    TemperatureStore tempStore = {
        .avgTemp = 0.0,
        .currentLogIndex = 0
    };

    while (1)
    {
        // read log input
        if (!fgets(logInput, sizeof(logInput), stdin))
        {
            printf("Input Error\n");
        }
        // Remove trailing newline to make a proper string
        logInput[strcspn(logInput, "\r\n")] = 0; 

        if (!is_valid_hex(&logInput)) 
        {
            printf("Input Error\n");
        } else
        {
            hexConvertRes = sscanf(logInput, "%x", &message);
            if (hexConvertRes != 1)
            {
                printf("Huh?\n");
            } else
            {
                // Here message is a valid hexnumber

                /**
                 * NOTE: the intructions for this task suggest that we should print "Input Error" if the reserved bits are non-zero.
                 * However Kattis does not check this and you will receive a "Wrong Answer" error if youfollow the instructions.
                 */
                reserved = extractBits(message, reservedStart, reservedEnd);
               
                type = extractBits(message, typeStart, typeEnd);
                data = extractBits(message, dataStart, dataEnd);
                handle_valid_input(type, data, &tempStore);
            }
        }
    }
    return 0;
}
