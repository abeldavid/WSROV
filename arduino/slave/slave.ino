#include <Adafruit_Sensor.h>

#include <DHT.h>

#include <Wire.h>
#include <Servo.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

Servo T1, T2;
Servo Thrusters[] = {T1, T2};
const int tPins[] = {5, 6}; // digital pins used to communicate with ESCs
int tForce[2]; // stores force values for each thruster
byte reading[6]; // stores raw readings from Master

float hum = 0;
float temp = 0;
byte first = 0;
byte second = 0;


void setup() {
    Serial.begin(9600);
    Wire.begin(8);
    Wire.onReceive(receiveEvent);
    for (int n = 0; n++; n < 2){
        Thrusters[n].attach(tPins[n]);
    }
}

void loop() {

}

void receiveEvent(int howMany){
    int i = 0;
    while ( Wire.available() > 0){
        reading[i] = Wire.read();
        i++;
    }
    if (reading[0] == 'T'){
        tForce[0] = reading[1] * 100;
        tForce[0] += reading[2];
        tForce[1] = reading[3] * 100;
        tForce[1] += reading[4]; 

        Thrusters[0].writeMicroseconds(tForce[0]);
        Thrusters[1].writeMicroseconds(tForce[1]); 
    }

}  

void requestEvent(int howMuch){
    if(reading[0] == 'S'){
        if (reading[1] == 'h'){
            hum = dht.readHumidity();
            first = hum;
            Wire.write(first);
            second = hum - first;
            second *= 100;
            Wire.write (second);
        }else if (reading[1] == 't'){
            temp = dht.readTemperature();
            byte wholePart = temp;
            Wire.write(first);
            byte decimalPart = (temp - wholePart) * 100;
            Wire.write (second);
        } 
    }else if(reading[0] == 'A'){
        Wire.write('s');
    }
}
