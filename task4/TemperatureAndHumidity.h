#ifndef TEMPERATUREANDHUMIDITY_H
#define TEMPERATUREANDHUMIDITY_H

/**
 * Struct to store all raw log data and aggregate data to be queried by the user
 */
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

/**
 * Initialise a clean TemperatureAndHumidityStore struct
 *
 * @return TemperatureAndHumidityStore
 */
TemperatureAndHumidityStore TemperatureAndHumidity_init();

/**
 * Free the dynamically allocated memory, call on exit
 */
void TemperatureAndHumidity_free(TemperatureAndHumidityStore *self);

/**
 * Extract and return the log increment from a received message
 */
int TemperatureAndHumidity_get_log_increment(int message);

/**
 * Updates the log increment used when the log gets full (stored in TemperatureAndHumidityStore struct)
 */
void TemperatureAndHumidity_set_log_increment(int newIncrement, TemperatureAndHumidityStore *self);

/**
 * Increment the log size by logIncrement when the log gets full
 */
void TemperatureAndHumidity_update_log_size(TemperatureAndHumidityStore *self);

/**
 * Update the TemperatureAndHumidityStore struct when receiving a temperature and humidity
 */
void TemperatureAndHumidity_update(int humidity, int temperature, TemperatureAndHumidityStore *self);

/**
 * Extract and return the reserved bits from a received message
 */
int TemperatureAndHumidity_get_reserved(int message);

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