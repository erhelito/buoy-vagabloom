const int salinity_pin = A5;
const int impermeability_pin = A3;
const int battery_pin = A1;

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

}