#ifndef TEMPERATUREANDHUMIDITY_H
#define TEMPERATUREANDHUMIDITY_H

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

TemperatureAndHumidityStore TemperatureAndHumidity_init();

void TemperatureAndHumidity_update(int humidity, int temperature, TemperatureAndHumidityStore *self);

int TemperatureAndHumidity_get_type(int message);

int TemperatureAndHumidity_get_humidity(int message);

int TemperatureAndHumidity_get_temperature(int message);

#endif // TEMPERATUREANDHUMIDITY_H