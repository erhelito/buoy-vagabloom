const int salinity_pin = A5;
const int impermeability_pin = A3;
const int battery_pin = A1;

// dataframes
float actual_data[5];
float past_data[5];

void setup() {
    Serial.begin(9600);

    // initialize analogic inputs
    pinMode(salinity_pin, INPUT);
    pinMode(impermeability_pin, INPUT);
    pinMode(battery_pin, INPUT);
}

void loop() {
    // read analogic inputs
    int salinity = analogRead(salinity_pin);
    int impermeability = analogRead(impermeability_pin);
    int battery = analogRead(battery_pin);

    past_data = actual_data;

    actual_data[2] = salinity;
    actual_data[3] = impermeability;
    actual_data[4] = battery;
}