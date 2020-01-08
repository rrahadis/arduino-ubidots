#include <Servo.h>
#include <UbidotsESPMQTT.h>

float vref = 3.3;
float resolution = vref/1023;
int suhu = A0;
int kipas = D8;
float nilai_suhu;
Servo myservo;





/****************************************

 * Include Libraries

 ****************************************/


/****************************************

 * Define Constants

 ****************************************/

#define TOKEN "BBFF-wLG8jPnQg7Kgtqxc1eiqU0C2BxBCpZ" // Your Ubidots TOKEN

#define WIFINAME "A" //Your SSID

#define WIFIPASS "home1234" // Your Wifi Pass

#define DEVICE_LABEL "io"

#define VARIABLE_LABEL1  "leda"  // Put here your Ubidots variable label
#define VARIABLE_LABEL2  "ledb"  // Put here your Ubidots variable label
#define VARIABLE_LABEL3  "ledc"  // Put here your Ubidots variable label

int brillo=0;
int brillo2=0;
const int ERROR_VALUE = 65535;  // Set here an error value

const uint8_t NUMBER_OF_VARIABLES = 3; // Number of variable to subscribe to
char * variable_labels[NUMBER_OF_VARIABLES] = {"leda", "ledb", "ledc"}; // labels of the variable to subscribe to


#define out1 D0
#define out2 D5
#define out3 D6


int seguro=3;

float estadoout1; // Name of the variable to be used within the code.
float estadoout2; // Name of the variable to be used within the code.
float estadoout3; // Name of the variable to be used within the code.

float value; // To store incoming value.
uint8_t variable; // To keep track of the state machine and be able to use the switch case.

Ubidots ubiClient(TOKEN);

WiFiClient client;

/****************************************

 * Auxiliar Functions

 ****************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  char* variable_label = (char *) malloc(sizeof(char) * 30);;
  get_variable_label_topic(topic, variable_label);
  value = btof(payload, length);
  set_state(variable_label);
  execute_cases();
  free(variable_label);
  /////////////////////////////////////
  if(estadoout1==1){
    digitalWrite(out1, HIGH);
  }
  else
  {
    digitalWrite(out1, LOW);
  }
  /////////////////////////////////////
  if(estadoout2==1){
    digitalWrite(out2, HIGH);
  }
  else
  {
    digitalWrite(out2, LOW);
  }
  /////////////////////////////////////
  if(estadoout3==1){
    digitalWrite(out3, HIGH);
  }
  else
  {
    digitalWrite(out3, LOW);
  }
  /////////////////////////////////////
  
}

// Parse topic to extract the variable label which changed value
void get_variable_label_topic(char * topic, char * variable_label) {
  Serial.print("topic:");
  Serial.println(topic);
  sprintf(variable_label, "");
  for (int i = 0; i < NUMBER_OF_VARIABLES; i++) {
    char * result_lv = strstr(topic, variable_labels[i]);
    if (result_lv != NULL) {
      uint8_t len = strlen(result_lv);      
      char result[100];
      uint8_t i = 0;
      for (i = 0; i < len - 3; i++) { 
        result[i] = result_lv[i];
      }
      result[i] = '\0';
      Serial.print("Label is: ");
      Serial.println(result);
      sprintf(variable_label, "%s", result);
      break;
    }
  }
}

// cast from an array of chars to float value.
float btof(byte * payload, unsigned int length) {
  char * demo_ = (char *) malloc(sizeof(char) * 10);
  for (int i = 0; i < length; i++) {
    demo_[i] = payload[i];
  }
  return atof(demo_);
}

// State machine to use switch case
void set_state(char* variable_label) {
  variable = 0;
  for (uint8_t i = 0; i < NUMBER_OF_VARIABLES; i++) {
    if (strcmp(variable_label, variable_labels[i]) == 0) {
      break;
    }
    variable++;
  }
  if (variable >= NUMBER_OF_VARIABLES) variable = ERROR_VALUE; // Not valid
}

// Function with switch case to determine which variable changed and assigned the value accordingly to the code variable
void execute_cases() {  
  switch (variable) {
    case 0:
      estadoout1 = value;
      Serial.print("LED 1 en: ");
      Serial.println(estadoout1);
      Serial.println();
      break;
    case 1:
      estadoout2 = value;
      Serial.print("LED 2 en: ");
      Serial.println(estadoout2);
      Serial.println();
      break;
    case 2:
      estadoout3 = value;
      Serial.print("LED 3 en: ");
      Serial.println(estadoout3);
      Serial.println();
      break;
   
    case ERROR_VALUE:
      Serial.println("error");
      Serial.println();
      break;
    default:
      Serial.println("default");
      Serial.println();
  }

}
/****************************************

 * Main Functions

 ****************************************/

void setup() {
Serial.begin(115200);
myservo.attach(D7);
pinMode(suhu,INPUT);
pinMode(kipas,OUTPUT);
// put your setup code here, to run once:
  
  ubiClient.ubidotsSetBroker("industrial.api.ubidots.com"); // Sets the broker properly for the business account

  ubiClient.setDebug(true); // Pass a true or false bool value to activate debug messages

  Serial.begin(115200);

  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT);



  ubiClient.wifiConnection(WIFINAME, WIFIPASS);

  ubiClient.begin(callback);

  if(!ubiClient.connected()){

  ubiClient.reconnect();

  }

  char* deviceStatus = getUbidotsDevice(DEVICE_LABEL);

  if (strcmp(deviceStatus, "404") == 0) {

ubiClient.add("leda", 0); //Insert your variable Labels and the value to be sent
ubiClient.ubidotsPublish(DEVICE_LABEL);
ubiClient.add("ledb", 0); //Insert your variable Labels and the value to be sent
ubiClient.ubidotsPublish(DEVICE_LABEL);
ubiClient.add("ledc", 0); //Insert your variable Labels and the value to be sent
ubiClient.ubidotsPublish(DEVICE_LABEL);


ubiClient.loop();

  }

  ubiClient.ubidotsSubscribe(DEVICE_LABEL,VARIABLE_LABEL1); //Insert the Device and Variable's Labels
  ubiClient.ubidotsSubscribe(DEVICE_LABEL,VARIABLE_LABEL2); //Insert the Device and Variable's Labels
  ubiClient.ubidotsSubscribe(DEVICE_LABEL,VARIABLE_LABEL3); //Insert the Device and Variable's Labels

  Serial.println(variable_labels[1]);
}

void loop() {
 // put your main code here, to run repeatedly:

  if(!ubiClient.connected()){

  ubiClient.reconnect();

  ubiClient.ubidotsSubscribe(DEVICE_LABEL,VARIABLE_LABEL1); //Insert the Device and Variable's Labels
  ubiClient.ubidotsSubscribe(DEVICE_LABEL,VARIABLE_LABEL2); //Insert the Device and Variable's Labels
  ubiClient.ubidotsSubscribe(DEVICE_LABEL,VARIABLE_LABEL3); //Insert the Device and Variable's Labels


  }


  float luz = analogRead(A0);
  luz = (luz*resolution);
  luz = luz*100;

    //float value2 = analogRead(2) // uncomment this line to read the GPIO2 of the ESP8266
   ubiClient.add("luz", luz); //Insert your variable Labels and the value to be sent
    ubiClient.ubidotsPublish(DEVICE_LABEL);

    
    ubiClient.loop();
    
    delay(500);
int cm = sensor_ping(D2,D4);
if(cm<10){
  myservo.write(0);
}
else{
  myservo.write(90);
}

temperature();
}

int sensor_ping(const int trig, const int echo){
  long duration, cm;

  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  pinMode(echo, INPUT);
  duration = pulseIn(echo, HIGH);
  cm = microsecondsToCentimeters(duration);

  Serial.print(cm);
  Serial.print("cm");

  Serial.println();
  delay(100);
  return(cm);
}

//mengkonversi data sensor menjadi satuan detik
long microsecondsToCentimeters(long microseconds)
{
  return microseconds /29/2;

}

void temperature(){
 nilai_suhu = analogRead(suhu);
nilai_suhu = (nilai_suhu*resolution);
nilai_suhu = nilai_suhu*100;
Serial.println(nilai_suhu);

if(nilai_suhu >= 40){
  digitalWrite(kipas,HIGH);
  }
else {
  digitalWrite(kipas,LOW);
}
}

char* getUbidotsDevice(char* deviceLabel) {

  char* data = (char *) malloc(sizeof(char) * 700);

  char* response = (char *) malloc(sizeof(char) * 400);

  sprintf(data, "GET /api/v1.6/devices/%s/", deviceLabel);

  sprintf(data, "%s HTTP/1.1\r\n", data);

  sprintf(data, "%sHost: industrial.api.ubidots.com\r\nUser-Agent:io/1.0\r\n", data);

  sprintf(data, "%sX-Auth-Token: %s\r\nConnection: close\r\n\r\n", data, TOKEN);

  if (client.connect("industrial.api.ubidots.com", 80)) {

client.println(data);

  } else {

return "e";

  }

  free(data);

  int timeout = 0;

  while(!client.available() && timeout < 5000) {

timeout++;

if (timeout >= 4999){

    return "e";

}

delay(1);

  }

  int i = 0;

  while (client.available()) {

response[i++] = (char)client.read();

if (i >= 399){

  break;

}

  }

  char * pch;

  char * statusCode;

  int j = 0;

  pch = strtok (response, " ");

  while (pch != NULL) {

if (j == 1 ) {

  statusCode = pch;

}

pch = strtok (NULL, " ");

j++;

  }

  free(response);

  return statusCode;

}
