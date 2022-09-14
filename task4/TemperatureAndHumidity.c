#include <stdio.h>
#include <stdlib.h>

#include "TemperatureAndHumidity.h"

/**
 * Extract bits between start-end (inclusive) from value.
 * This is achieved by applying an AND mask (bitwise ANDing) - a simple way of extracting a subset of bits.
 * The idea is to create a mask (binary number) that has 1's at the indices we want to keep and 0's at the other indices.
 * This works since any bit X that is ANDed with 1 remain the same (X AND 1 = X) and any bit ANDed with 0 is 0 (X AND 0 = 0).
 */
unsigned extract_bits(unsigned value, unsigned start, unsigned end)
{
    unsigned mask;
    int numberOfBits = end - start + 1; // Number of bits to be extracted/set to 1 in the mask
    mask = (1 << numberOfBits) - 1;     // Set numberOfBits to 1 e.g. numberOfBits=8 => (1 << 8)-1=11111111 (255)
    mask <<= start;                     // Shift the 1's to correct position i.e. pad with [start] number of 0's on the right
    return (value & mask) >> start;     // Perform the bitwise AND and shift the isolated bits to LSB (to only get the isolated bits)
}

TemperatureAndHumidityStore TemperatureAndHumidity_init()
{
    TemperatureAndHumidityStore th = {.logIndex = 0, .logSize = 0, .avgTemperature = 0.0, .avgHumidity = 0.0};
    return th;
}

int TemperatureAndHumidity_get_log_increment(int message)
{
    int incrementStart = 0, incrementEnd = 25;
    return extract_bits(message, incrementStart, incrementEnd);
}

void TemperatureAndHumidity_set_log_increment(int newIncrement, TemperatureAndHumidityStore *self)
{
    self->logIncrement = newIncrement;
    TemperatureAndHumidity_update_log_size(self);
}

void TemperatureAndHumidity_update_log_size(TemperatureAndHumidityStore *self)
{
    if (self->temperatureLog == NULL)
    {
        self->temperatureLog = malloc(self->logIncrement * sizeof(int));
    }
    else
    {
        self->temperatureLog = realloc(self->temperatureLog, self->logIncrement * sizeof(int));
    }
    if (self->humidityLog == NULL)
    {
        self->humidityLog = malloc(self->logIncrement * 2 * sizeof(int));
    }
    else
    {
        self->humidityLog = realloc(self->humidityLog, self->logIncrement * sizeof(int));
    }
    self->logSize += self->logIncrement;
}

void TemperatureAndHumidity_update(int humidity, int temperature, TemperatureAndHumidityStore *self)
{
    // Set max temp
    if (self->maxTemperature < temperature || self->logIndex == 0)
        self->maxTemperature = temperature;
    // Set min temp
    if (self->minTemperature > temperature || self->logIndex == 0)
        self->minTemperature = temperature;

    // Set max humidity
    if (self->maxHumidity < humidity || self->logIndex == 0)
        self->maxHumidity = humidity;
    // Set min humidity
    if (self->minHumidity > humidity || self->logIndex == 0)
        self->minHumidity = humidity;

    // Calculate avg temp and humidity
    self->avgTemperature = self->avgTemperature + ((temperature - self->avgTemperature) / (self->logIndex + 1));
    self->avgHumidity = self->avgHumidity + ((humidity - self->avgHumidity) / (self->logIndex + 1));

    // Add received temperature and humidity to log array
    self->temperatureLog[self->logIndex] = temperature;
    self->humidityLog[self->logIndex] = humidity;

    // Increment currentLogIndex
    self->logIndex++;
}

int TemperatureAndHumidity_get_type(int message)
{
    int typeStart = 26, typeEnd = 28;
    return extract_bits(message, typeStart, typeEnd);
}

int TemperatureAndHumidity_get_humidity(int message)
{
    int humidityStart = 13, humidityEnd = 25;
    return extract_bits(message, humidityStart, humidityEnd);
}

int TemperatureAndHumidity_get_temperature(int message)
{
    int temperatureStart = 0, temperatureEnd = 12;
    return extract_bits(message, temperatureStart, temperatureEnd);
}