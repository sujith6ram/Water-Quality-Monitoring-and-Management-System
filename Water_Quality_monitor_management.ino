const int RELAY_PIN = 6;

float vol1 = 0.0, vol2 = 0.0; // Volume for sensor 1 and sensor 2
float l_minute1, l_minute2; // Flow rate for sensor 1 and sensor 2
unsigned char flowsensor1 = 2; // Sensor 1 input
unsigned char flowsensor2 = 3; // Sensor 2 input
unsigned long flow_frequency1, flow_frequency2; // Flow frequency for sensor 1 and sensor 2
unsigned long currentTime1, currentTime2; // Current time for sensor 1 and sensor 2
unsigned long cloopTime1, cloopTime2; // Loop time for sensor 1 and sensor 2

float cost;

void flow1() {
  flow_frequency1++;
}

void flow2() {
  flow_frequency2++;
}


void setup() {
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN,HIGH);
    Serial.begin(9600);
    pinMode(flowsensor1, INPUT);
    digitalWrite(flowsensor1, HIGH); // Optional Internal Pull-Up
    attachInterrupt(digitalPinToInterrupt(flowsensor1), flow1, RISING); // Setup Interrupt for sensor 1
  
    pinMode(flowsensor2, INPUT);
    digitalWrite(flowsensor2, HIGH); // Optional Internal Pull-Up
    attachInterrupt(digitalPinToInterrupt(flowsensor2), flow2, RISING); // Setup Interrupt for sensor 2
  
    delay(500);
    currentTime1 = millis();
    currentTime2 = millis();
    cloopTime1 = currentTime1;
    cloopTime2 = currentTime2;
}

void loop() {
    for(int i=1; i<=10; i++) {
        String phdata = Serial.readStringUntil(':');
        //Serial.println(phdata);
        if(phdata != ""){
            String ph = Serial.readStringUntil('$');
           
            float phvalue=ph.toFloat();
            Serial.println();
            Serial.println("PH Value");
            Serial.println(phvalue); // to display the ph value 
            if(phvalue<15.00){
              digitalWrite(RELAY_PIN, LOW);
              Serial.println("motor is on");
            }
            else{
              digitalWrite(RELAY_PIN, HIGH);
            }
        }
    }


  currentTime1 = millis();
  if (currentTime1 >= (cloopTime1 + 1000)) {
    cloopTime1 = currentTime1;
    if (flow_frequency1 != 0) {
      l_minute1 = (flow_frequency1 / 7.5); // Calculate flow rate for sensor 1
      l_minute1 = l_minute1 / 60; // Convert to liters per second
      vol1 += l_minute1; // Update volume for sensor 1
      flow_frequency1 = 0; // Reset counter for sensor 1
    }
    Serial.print("Sensor 1 - Flow Rate: ");
    Serial.print(l_minute1);
    Serial.print(" mL/s, Total Volume: ");
    Serial.print(vol1);
    Serial.println(" mL");
  }

  // Sensor 2 calculations
  currentTime2 = millis();
  if (currentTime2 >= (cloopTime2 + 1000)) {
    cloopTime2 = currentTime2;
    if (flow_frequency2 != 0) {
      l_minute2 = (flow_frequency2 / 7.5); // Calculate flow rate for sensor 2
      l_minute2 = l_minute2 / 60; // Convert to liters per second
      vol2 += l_minute2; // Update volume for sensor 2
      flow_frequency2 = 0; // Reset counter for sensor 2
    }
    Serial.print("Sensor 2 - Flow Rate: ");
    Serial.print(l_minute2);
    Serial.print(" mL/s, Total Volume: ");
    Serial.print(vol2);
    Serial.println(" mL");
    if(abs(l_minute1-l_minute2)>=17){
    Serial.println(abs(l_minute1-l_minute2));
    Serial.println("Leakage is detected");
    }

    if(vol2>1){
      cost=vol2*0.03;
      Serial.println("you've exceeded your daily water usage!!");
      Serial.print("cost: Rs. ");
      Serial.println(cost);
    }
  }
}
