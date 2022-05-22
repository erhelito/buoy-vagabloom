// for accelerometer
#include <Wire.h>
#include <EEPROM.h>

// for temperature sensors
#include <OneWire.h>
#include <DallasTemperature.h>



/*
##########
ACCELEROMETER
##########
*/
const char address_accel_3_axes = 0x53;

// registers addresses
const char data_format = 0x31;
const char power_control = 0x2D;

// measure modes addresses
const char precision_2G_accel  = 0x00;
const char mode_accel   = 0x08;

// axes addresses
const char x0_accel_address = 0x32;

// time variables
unsigned long time;
unsigned long time_1;
unsigned long period;

// saving values of axes
int y_accel;

// accelerations variables
// wave phase : true if wave is rising, false if wave is falling
bool phase = true;

// certification variable
int certification = 0;
int max_certification = 40;
int acc_max = 0;
int acc_min = 0;
unsigned int delta_acc = 0;

// variables for average calculation
int sum_values = 0;
int average = 0;
int previous_average = 0;
int nbr_values_in_average = 30;
int storage_50_values[50];

byte memory_accel[6];

// height variable
unsigned int height;



/*
##########
TEMPERATURE SENSORS
##########
*/

// initialise temperature sensors
OneWire oneWire(8);
DallasTemperature sensors(&oneWire);
DeviceAddress sensorDeviceAddress;

// initialise temperature variables
float surface_temp;
float depth_temp;


const int salinity_pin = A5;
const int impermeability_pin = A3;
const int battery_pin = A1;

// dataframes
float actual_data[6];
float past_data[6];

void setup(void) {
    Serial.begin(9600);

    // initialize accelerometer
    Wire.begin();
    accel_configuration(data_format, precision_2G_accel);
    accel_configuration(power_control, mode_accel);

    // initialize temperature sensors
    sensors.begin();
    sensors.setResolution(sensorDeviceAddress, 9);

    // initialize analogic inputs
    pinMode(salinity_pin, INPUT);
    pinMode(impermeability_pin, INPUT);
    pinMode(battery_pin, INPUT);
}

void loop(void) {
    backup_data();

    // get wave height
    get_height();

    // get temperatures
    // create surface_temp and depth_temp
    get_temp();

    // read analogic inputs
    int salinity = analogRead(salinity_pin);
    int impermeability = analogRead(impermeability_pin);
    int battery = analogRead(battery_pin);

    actual_data[0] = height;
    actual_data[1] = surface_temp;
    actual_data[2] = depth_temp;
    actual_data[3] = salinity;
    actual_data[4] = impermeability;
    actual_data[5] = battery;
}

void backup_data(void) {
    // backup actual_data in past_data
    for (int i = 0; i < 6; i++) {
        past_data[i] = actual_data[i];
    }
}




void get_height(void) {
    time = millis();

    // while we don't have 20 measurements
    for (int i = nbr_values_in_average; i >= 0; i -= 1) {
        // create the sum of the 20 values
        sum_values += storage_50_values[i];
        storage_50_values[i] = storage_50_values[i-1];
    }

    accel_reading();

    storage_50_values[0] = (y_accel + 255);

    average = sum_values / nbr_values_in_average;

    sum_values = 0;

    data_compute();
}

void accel_configuration(byte address, byte value) {
    Wire.beginTransmission(address_accel_3_axes);
    Wire.write(address);
    Wire.write(value);
    Wire.endTransmission();
}

void accel_reading() {
    Wire.beginTransmission(address_accel_3_axes);
    Wire.write(x0_accel_address);
    Wire.endTransmission();
    Wire.beginTransmission(address_accel_3_axes);
    Wire.requestFrom(address_accel_3_axes, 6);

    // write values while connection is active
    int i = 0;
    while (Wire.available()) {
        memory_accel[i] = Wire.read();
        i++;
    }

    Wire.endTransmission();

    y_accel = (((int) memory_accel[3]) << 8) | memory_accel[2];
}

void data_compute(void) {
    if (phase) {
        rising_phase();
    }

    else {
        falling_phase();
    }
}

void rising_phase() {
    if (average >= previous_average) {
        previous_average = average;
        certification = 0;
    }

    else {
        if (certification < max_certification) {
            certification += 1;
        }

        else {
            acc_max = previous_average;
            certification = 0;
            phase = false;
        }
    }
}

void falling_phase() {
    if (average <= previous_average) {
        previous_average = average;
        certification = 0;
    }

    else {
        if (certification < max_certification) {
            certification += 1;
        }

        else{
            acc_min = previous_average;
            period = (time - time_1);
            delta_acc = (acc_max - acc_min);

            // computing height
            // splitted in two parts to avoid overflow
            height = (delta_acc * period) / 100;
            height = (height * period) / 103000;

            time_1 = time;
            certification = 0;
            phase = true;
        }
    }
}

void get_temp(void) {
    // request temperatures
    sensors.requestTemperatures();

    // get temperatures
    surface_temp = (sensors.getTempCByIndex(0));
    depth_temp = (sensors.getTempCByIndex(1));
}