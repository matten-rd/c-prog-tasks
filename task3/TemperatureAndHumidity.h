#ifndef TEMPERATUREANDHUMIDITY_H
#define TEMPERATUREANDHUMIDITY_H

/**
 * Struct to store all raw log data and aggregate data to be queried by the user
 */
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
 * Initialise a clean TemperatureAndHumidityStore struct
 *
 * @return TemperatureAndHumidityStore
 */
TemperatureAndHumidityStore TemperatureAndHumidity_init();

/**
 * Update the TemperatureAndHumidityStore struct when receiving a temperature and humidity
 */
void TemperatureAndHumidity_update(int humidity, int temperature, TemperatureAndHumidityStore *self);

/**
 * Extract and return the type from a received message
 */
int TemperatureAndHumidity_get_type(int message);

/**
 * Extract and return the humidity from a received message
 */
int TemperatureAndHumidity_get_humidity(int message);

/**
 * Extract and return the temperature from a received message
 */
int TemperatureAndHumidity_get_temperature(int message);

#endif // TEMPERATUREANDHUMIDITY_H