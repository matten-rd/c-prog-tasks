#ifndef TEMPERATUREANDHUMIDITY_H
#define TEMPERATUREANDHUMIDITY_H

typedef struct TemperatureAndHumidityStore
{
    int logIndex;
    int logSize;
    int logIncrement;
    unsigned maxTemperature;
    unsigned minTemperature;
    float avgTemperature;
    unsigned *temperatureLog;
    unsigned maxHumidity;
    unsigned minHumidity;
    float avgHumidity;
    unsigned *humidityLog;
} TemperatureAndHumidityStore;

TemperatureAndHumidityStore TemperatureAndHumidity_init();

int TemperatureAndHumidity_get_log_increment(int message);

void TemperatureAndHumidity_set_log_increment(int newIncrement, TemperatureAndHumidityStore *self);

void TemperatureAndHumidity_update_log_size(TemperatureAndHumidityStore *self);

void TemperatureAndHumidity_update(int humidity, int temperature, TemperatureAndHumidityStore *self);

int TemperatureAndHumidity_get_reserved(int message);

int TemperatureAndHumidity_get_type(int message);

int TemperatureAndHumidity_get_humidity(int message);

int TemperatureAndHumidity_get_temperature(int message);

#endif // TEMPERATUREANDHUMIDITY_H