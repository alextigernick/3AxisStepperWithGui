#include <AccelStepper.h>
AccelStepper supply(AccelStepper::DRIVER,3,4);
AccelStepper build(AccelStepper::DRIVER,11,12);
AccelStepper coat(AccelStepper::DRIVER,5,6);
AccelStepper motors[] = {supply,build,coat};
long int distancePerStep[] = {2,2,90};//microns
int const homePin = 8;
int const pushbuttonPin = 10; //reads high until pressed
#define BUFSIZE 64
char buf [BUFSIZE] = "";
char buf2 [BUFSIZE] = "";
long int parms[10];
char dexs [10];
char c = 0;
int i,j,k;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  pinMode(homePin, INPUT);//20
  pinMode(pushbuttonPin, INPUT);
  for(i = 0; i < 3; i++){

    motors[i].setAcceleration(1000);
  }
  while (!Serial);
  i = 0;
  Serial.println("USAGE: motor# distance rpm");
  Serial.println("Motors are: 0 - Supply, 1 - Build, 2 - Coat");
}
void loop() {
  while(Serial.available()){
    c = Serial.read();
    if (c != '\n') {
      buf[i] = c;
      i++;
    }
    else{
      buf[i] = ' ';
      command(buf,i);
      i = 0;
    }
  }
}
void command(char * buf, int len){
  dexs[0] = 0;
  for (j=0,i=1; j <= len; j++) {
    if (buf[j] == ' ') {
      dexs[i] = j+1;
      i++;
    }
  }
  c = i;
  for(i = 0; i < c-1; i++){
    for(j = dexs[i], k = 0; j < dexs[i+1]; j++, k++)
      buf2[k] = buf[j];
    buf2[k] = 0;
    parms[i] = atol(buf2);
  }
  Serial.print("Motor ");
  Serial.print(parms[0]);
  Serial.print(" moving ");
  Serial.print(parms[1]);
  Serial.print(" um @ ");
  Serial.print(parms[2]);
  Serial.print(" um/s: ");
  Serial.print(parms[1]/distancePerStep[parms[0]]);
  Serial.print(" steps @ ");
  Serial.print(parms[2]/distancePerStep[parms[0]]);
  Serial.println(" steps/s");
  motors[parms[0]].move(-parms[1]/distancePerStep[parms[0]]);
  motors[parms[0]].setMaxSpeed(parms[2]/distancePerStep[parms[0]]);
  motors[parms[0]].runToPosition();
  Serial.println(" Done!");
}
