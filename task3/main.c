#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct TemperatureAndHumidityStore
{
    int currentLogIndex;
    unsigned maxTemperature;
    unsigned minTemperature;
    float avgTemperature;
    unsigned temperatureLog[10];
    unsigned maxHumidity;
    unsigned minHumidity;
    float avgHumidity;
    unsigned humidityLog[10];
} TemperatureAndHumidityStore;

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

void handle_temp_and_humidity_log(int humidity, int temperature, TemperatureAndHumidityStore *tnhStore)
{
    // Set max temp
    if (tnhStore->maxTemperature < temperature || tnhStore->currentLogIndex == 0)
        tnhStore->maxTemperature = temperature;
    // Set min temp
    if (tnhStore->minTemperature > temperature || tnhStore->currentLogIndex == 0)
        tnhStore->minTemperature = temperature;

    // Set max humidity
    if (tnhStore->maxHumidity < humidity || tnhStore->currentLogIndex == 0)
        tnhStore->maxHumidity = humidity;
    // Set min humidity
    if (tnhStore->minHumidity > humidity || tnhStore->currentLogIndex == 0)
        tnhStore->minHumidity = humidity;

    // Calculate avg temp and humidity
    tnhStore->avgTemperature = tnhStore->avgTemperature + ((temperature - tnhStore->avgTemperature) / (tnhStore->currentLogIndex + 1));
    tnhStore->avgHumidity = tnhStore->avgHumidity + ((humidity - tnhStore->avgHumidity) / (tnhStore->currentLogIndex + 1));

    // Add received temperature and humidity to log array
    tnhStore->temperatureLog[tnhStore->currentLogIndex] = temperature;
    tnhStore->humidityLog[tnhStore->currentLogIndex] = humidity;

    // Increment currentLogIndex
    tnhStore->currentLogIndex++;

    // Print temperature and humidity
    printf("Received Temperature: %d\n", temperature);
    printf("Received Humidity: %d\n", humidity);
}

void handle_valid_input(int type, int humidity, int temperature, TemperatureAndHumidityStore *tnhStore)
{
    // Handles all possible types
    switch (type)
    {
    case 0b000:
        if (tnhStore->currentLogIndex < 10)
        {
            handle_temp_and_humidity_log(humidity, temperature, tnhStore);
        }
        else
        {
            printf("Log Full\n");
        }
        break;

    case 0b010:
        if (tnhStore->currentLogIndex > 0)
        {
            printf("Average Temperature: %.2f\n", tnhStore->avgTemperature);
            printf("Average Humidity: %.2f\n", tnhStore->avgHumidity);
        }
        else
        {
            printf("Average Temperature: N/A\n");
            printf("Average Humidity: N/A\n");
        }
        break;

    case 0b011:
        if (tnhStore->currentLogIndex > 0)
        {
            printf("Minimum Temperature: %d\n", tnhStore->minTemperature);
            printf("Minimum Humidity: %d\n", tnhStore->minHumidity);
        }
        else
        {
            printf("Minimum Temperature: N/A\n");
            printf("Minimum Humidity: N/A\n");
        }
        break;

    case 0b100:
        if (tnhStore->currentLogIndex > 0)
        {
            printf("Maximum Temperature: %d\n", tnhStore->maxTemperature);
            printf("Maximum Humidity: %d\n", tnhStore->maxHumidity);
        }
        else
        {
            printf("Maximum Temperature: N/A\n");
            printf("Maximum Humidity: N/A\n");
        }
        break;

    case 0b101:
        printf("Log: %d entries\n", tnhStore->currentLogIndex);
        for (int i = 0; i < tnhStore->currentLogIndex; i++)
        {
            printf("Temperature: %d; Humidity: %d\n", tnhStore->temperatureLog[i], tnhStore->humidityLog[i]);
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

    int message, type, humidity, temperature;
    int typeStart = 26, typeEnd = 28;
    int humidityStart = 13, humidityEnd = 25;
    int temperatureStart = 0, temperatureEnd = 12;

    TemperatureAndHumidityStore temperatureAndHumidityStore = {
        .currentLogIndex = 0,
        .avgTemperature = 0.0,
        .avgHumidity = 0.0
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
        }
        else
        {
            hexConvertRes = sscanf(logInput, "%x", &message);
            if (hexConvertRes != 1)
            {
                printf("Huh?\n");
            }
            else
            {
                // Here message is a valid hexnumber
                type = extractBits(message, typeStart, typeEnd);
                humidity = extractBits(message, humidityStart, humidityEnd);
                temperature = extractBits(message, temperatureStart, temperatureEnd);
                handle_valid_input(type, humidity, temperature, &temperatureAndHumidityStore);
            }
        }
    }
    return 0;
}
