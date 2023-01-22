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
const char precision_2G_accel = 0x00;
const char mode_accel = 0x08;

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
float final_height;

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


const int salinity_pin = A2;
const int impermeability_pin = A3;
const int battery_pin = A1;

// ##########
// OTHERS SENSORS
// ##########
int salinity;
int impermeability;
int battery;

// ##########
// DATAFRAMES
// ##########
float actual_data[6];
float past_data[6];

void setup(void) {
    Serial.println("Starting program");

    Serial.begin(9600);

    // initialize accelerometer
    Serial.print("Accelerometer init");
    Wire.begin();
    accel_configuration(data_format, precision_2G_accel);
    accel_configuration(power_control, mode_accel);
    Serial.println("Done");

    // initialize temperature sensors
    Serial.print("Temperature init");
    sensors.begin();
    sensors.setResolution(sensorDeviceAddress, 9);
    Serial.println("Done");

    // initialize analogic inputs
    Serial.print("Analogic init");
    pinMode(salinity_pin, INPUT);
    pinMode(impermeability_pin, INPUT);
    pinMode(battery_pin, INPUT);
    Serial.println("Done");

    Serial.println("Wave height, Surface temp, Depth temp, Salinity, Impermeability, Battery");
}

void loop(void) {
    backup_data();

    //loop 17000 times
    for (int i = 0; i < 1250; i++) {
        if (i == 0) {
            // get temperatures
            // create surface_temp and depth_temp
            get_temp();

            // read analogic inputs[
            salinity = analogRead(salinity_pin);
            impermeability = analogRead(impermeability_pin);
            battery = analogRead(battery_pin);
            
        }

        // get wave height
        get_height();

        final_height += height;

        delay(5);
    }

    final_height = final_height / 1250;

    actual_data[0] = final_height;
    actual_data[1] = surface_temp;
    actual_data[2] = depth_temp;
    actual_data[3] = salinity;
    actual_data[4] = impermeability;
    actual_data[5] = battery;

    // send data
    Serial.print("I: ");
    for (int i = 0; i < 6; i++) {
        Serial.print(actual_data[i]);
        Serial.print(" , ");
    }

    // send past data
    Serial.print("P: ");
    for (int i = 0; i < 6; i++) {
        Serial.print(past_data[i]);
        Serial.print(" , ");
    }

    Serial.println("");

    delay(1000);
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

    storage_50_values[0] = (y_accel - 255);

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