#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TemperatureAndHumidity.h"

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

void handle_valid_input(int type, int humidity, int temperature, TemperatureAndHumidityStore *thStore)
{
    // Handles all possible types
    switch (type)
    {
    case 0b000:
        if (thStore->currentLogIndex < 10)
        {
            TemperatureAndHumidity_update(humidity, temperature, thStore);
            printf("Received Temperature: %d\n", temperature);
            printf("Received Humidity: %d\n", humidity);
        }
        else
        {
            printf("Log Full\n");
        }
        break;

    case 0b010:
        if (thStore->currentLogIndex > 0)
        {
            printf("Average Temperature: %.2f\n", thStore->avgTemperature);
            printf("Average Humidity: %.2f\n", thStore->avgHumidity);
        }
        else
        {
            printf("Average Temperature: N/A\n");
            printf("Average Humidity: N/A\n");
        }
        break;

    case 0b011:
        if (thStore->currentLogIndex > 0)
        {
            printf("Minimum Temperature: %d\n", thStore->minTemperature);
            printf("Minimum Humidity: %d\n", thStore->minHumidity);
        }
        else
        {
            printf("Minimum Temperature: N/A\n");
            printf("Minimum Humidity: N/A\n");
        }
        break;

    case 0b100:
        if (thStore->currentLogIndex > 0)
        {
            printf("Maximum Temperature: %d\n", thStore->maxTemperature);
            printf("Maximum Humidity: %d\n", thStore->maxHumidity);
        }
        else
        {
            printf("Maximum Temperature: N/A\n");
            printf("Maximum Humidity: N/A\n");
        }
        break;

    case 0b101:
        printf("Log: %d entries\n", thStore->currentLogIndex);
        for (int i = 0; i < thStore->currentLogIndex; i++)
        {
            printf("Temperature: %d; Humidity: %d\n", thStore->temperatureLog[i], thStore->humidityLog[i]);
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

    int message, type, humidity, temperature;

    TemperatureAndHumidityStore temperatureAndHumidityStore = TemperatureAndHumidity_init();

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
            sscanf(logInput, "%x", &message);

            // Here message is a valid hexnumber
            type = TemperatureAndHumidity_get_type(message);
            humidity = TemperatureAndHumidity_get_humidity(message);
            temperature = TemperatureAndHumidity_get_temperature(message);
            handle_valid_input(type, humidity, temperature, &temperatureAndHumidityStore);
        }
    }
    return 0;
}
