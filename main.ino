#include <OneWire.h>
#include <DallasTemperature.h>

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

    // initialise temperature sensors
    sensors.begin();
    sensors.setResolution(sensorDeviceAddress, 9);

    // initialize analogic inputs
    pinMode(salinity_pin, INPUT);
    pinMode(impermeability_pin, INPUT);
    pinMode(battery_pin, INPUT);
}

void loop(void) {
    backup_data();

    // get temperatures
    // create surface_temp and depth_temp
    get_temp();


    // read analogic inputs
    int salinity = analogRead(salinity_pin);
    int impermeability = analogRead(impermeability_pin);
    int battery = analogRead(battery_pin);

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

void get_temp(void){
    // request temperatures
    sensors.requestTemperatures();

    // get temperatures
    surface_temp = (sensors.getTempCByIndex(0));
    depth_temp = (sensors.getTempCByIndex(1));
}