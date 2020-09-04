
#include <AccelStepper.h>
AccelStepper stepper(1, 6, 7); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5


#include <Nextion.h>
//Declare your Nextion objects , page id, component id, component name

NexButton b0 = NexButton(0, 1, "b0");
//NexButton b1 = NexButton(0, 2, "b1");
//NexText t0 = NexText(0,2,"t0");
NexNumber n0 = NexNumber(0, 2, "n0");


//Register a button object to the touch event list

NexTouch *nex_listen_list[] = {
  &b0,
  //&b1,

  NULL
};

// COUNTER
int ir_pin = 8;
int counter2 = 0;
bool hitObject = false;


//E3F-DS30C4 INFRARED SENSOR/PHOTOELETRIC SWITCH (DIGITAL_INPUT)
int infra = 4;


//SHARP INFRA SENSORE (ANALOG_INPUT)
int infra2 = A1;


//LED LAMP
int led13 = 13;


//RELAY (FAN 12V)
int relay = 52;
int devectoButton = 22;
int limitSwitch = 3;


//PRESENTATION
String presentation = "Presentation";


//LEDS/LAMPS
int yellow_led = 12;
int green_led = 11;
int blue_led = 10;
int red_led = 9;


//JOYSTICK-analog

int joystick_right = A2;
int joystick_left = A3;

//NEMA17 STEPPER
int SPEED1 = 800;
int STEP  = 6;
int DIREC  = 7;
int DISTANCE = 1;


//ACCEL-STEPPER-MODE
int AccelStepperButton = 5;

int Nema17matka   = 800;
int Nema17matka2  = 600;

int counter;
int task1Status    = 0;
int repeat = 1;



void setup() {
  Serial.begin(9600);
  Serial2.begin(115200);
  nexInit();
  //Register the pop event callback function of the components

  b0.attachPop(b0PushCallBack, &b0);


  pinMode(ir_pin, INPUT);

  pinMode (infra, INPUT);

  pinMode (infra2, INPUT);

  pinMode (relay, OUTPUT);

  pinMode (devectoButton, INPUT_PULLUP);
  pinMode (limitSwitch, INPUT_PULLUP);

  pinMode (yellow_led, OUTPUT);
  pinMode (green_led, OUTPUT);
  pinMode (blue_led, OUTPUT);
  pinMode (red_led, OUTPUT);

  pinMode (joystick_right, INPUT);
  pinMode (joystick_left, INPUT);

  pinMode(STEP, OUTPUT);
  pinMode(DIREC, OUTPUT);

  pinMode(led13, OUTPUT);
  pinMode(AccelStepperButton, INPUT_PULLUP);


}

void b0PushCallBack(void *ptr) {

  // t0.setText ("State:ON");
  counter2 = 0;
  Serial2.print("n0.val=");
  Serial2.print(counter2);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

}

void loop() {

  //waiting for touch
  nexLoop(nex_listen_list);

  //----------COUNTER-----------

  int val = digitalRead(ir_pin);

  if ( (val == 0) && (hitObject == false) ) {
    counter2++;
    hitObject = true;

    //Serial.print("Counter = ");
    //Serial.println( counter2);

    Serial2.print("n0.val=");
    Serial2.print((counter2) / 2);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);

  }
  else if ( (val == 1) && (hitObject == true) ) {
    hitObject = false;

    if ((hitObject == false) && (hitObject == true))
    {
      counter2++;
    }

  }
  //  ----------BUTTON------------
  //Serial.println( digitalRead(devectoButton));
  if (digitalRead( devectoButton) == LOW)
  {
    digitalWrite(led13, LOW);
    counter2 = 0;
    Serial2.print("n0.val=");
    Serial2.print(counter2);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }
  else {
    digitalWrite(led13, HIGH);

  }

  //----------INFRA-RELAY-FAN------------

  // Serial.println(analogRead(infra2));

  // delay(50);
  if (digitalRead(infra) == LOW)
  {
    //Serial.println(presentation);
    //digitalWrite(lampa,HIGH);
    digitalWrite(relay, LOW);
  }
  else if (digitalRead(infra) == HIGH)
  {
    //digitalWrite(lampa,LOW);
    digitalWrite(relay, HIGH);
  }

  //--------joystic/infraRed---------------

  int infra2Read = analogRead(infra2);
  int right = analogRead(joystick_right);
  int left = analogRead(joystick_left);

  //Serial.println(infra2Read);

  //
  int sensValYellow = constrain(infra2Read, 205, 220);
  int sensValGreen = constrain(infra2Read, 240, 280);
  int sensValBlue = constrain(infra2Read, 300, 340);
  int sensValRed = constrain(infra2Read, 400, 600);


  //---------infra2/yelow_led-------------

  if (infra2Read == sensValYellow)
  {

    digitalWrite(yellow_led, HIGH);
  }

  else

  {
    digitalWrite(yellow_led, LOW);
  }


  //---------infra2/green_led-------------

  if (infra2Read == sensValGreen)
  {

    digitalWrite(green_led, HIGH);
  }

  else

  {
    digitalWrite(green_led, LOW);
  }


  //--------infra2/blue_led--------------

  if (infra2Read == sensValBlue)
  {

    digitalWrite(blue_led, HIGH);
  }

  else

  {
    digitalWrite(blue_led, LOW);
  }


  //--------infra2/red_led--------------

  if (infra2Read == sensValRed)
  {

    digitalWrite(red_led, HIGH);
  }

  else

  {
    digitalWrite(red_led, LOW);
  }

  //----STEPER CONTROL-WITH-JOYSTIC-LIMIT-SWITCH-----

  int switchAction1 = (digitalRead(limitSwitch) == HIGH);

  {

    if ((left < 500)  && (switchAction1))
    {
      digitalWrite(DIREC, HIGH);
      for (int i = 0; i < DISTANCE; i++)
      {

        digitalWrite(STEP, HIGH);
        digitalWrite(STEP, LOW);
        delayMicroseconds(SPEED1);

      }
    }

    else {
      digitalWrite(STEP, LOW);
      digitalWrite(DIREC, LOW);
    }


    if ((right > 550) && (switchAction1))
    {
      digitalWrite(DIREC, LOW);
      for (int i = 0; i < DISTANCE; i++)
      {
        digitalWrite(STEP, HIGH);
        digitalWrite(STEP, LOW);
        delayMicroseconds(SPEED1);

      }
    }
    else
    {
      digitalWrite(STEP, LOW);
      digitalWrite(DIREC, LOW);
    }
  }

  //-----------ACCEL-MODE-PART-----------

  if (digitalRead(AccelStepperButton) == LOW) {

    if (counter < repeat) {
      switch (task1Status)

      {

        case 0:

          {


            stepper.setAcceleration(4000);
            stepper.runToNewPosition(-Nema17matka);
            digitalWrite(yellow_led, HIGH);
            digitalWrite(red_led, LOW);
            digitalWrite(green_led, LOW);
            digitalWrite(blue_led, LOW);
            counter++;
          }
        case 1:
          {

            delay(500);
            stepper.setAcceleration(4000);
            stepper.runToNewPosition(0);
            digitalWrite(yellow_led, LOW);
            digitalWrite(red_led, LOW);
            digitalWrite(green_led, HIGH);
            digitalWrite(blue_led, HIGH);
            counter++;
          }

        case 2:
          {

            delay(500);
            stepper.setAcceleration(4000);
            stepper.runToNewPosition(Nema17matka);
            digitalWrite(yellow_led, LOW);
            digitalWrite(red_led, HIGH);
            digitalWrite(green_led, LOW);
            digitalWrite(blue_led, LOW);
            counter++;
          }

        case 3:
          {

            delay(500);
            stepper.setAcceleration(4000);
            stepper.runToNewPosition(0);
            digitalWrite(yellow_led, LOW);
            digitalWrite(red_led, LOW);
            digitalWrite(green_led, HIGH);
            digitalWrite(blue_led, HIGH);
            counter++;
          }

        case 4:
          {

            delay(100);
            stepper.setAcceleration(4000);
            stepper.runToNewPosition(-Nema17matka);
            digitalWrite(yellow_led, HIGH);
            digitalWrite(red_led, LOW);
            digitalWrite(green_led, LOW);
            digitalWrite(blue_led, LOW);
            counter++;
          }

        case 5:
          {

            delay(100);
            stepper.setAcceleration(4000);
            stepper.runToNewPosition(Nema17matka);
            digitalWrite(yellow_led, LOW);
            digitalWrite(red_led, HIGH);
            digitalWrite(green_led, LOW);
            digitalWrite(blue_led, LOW);
            counter++;
          }

        case 6:
          {

            delay(100);
            stepper.setAcceleration(4000);
            stepper.runToNewPosition(-Nema17matka);
            digitalWrite(yellow_led, HIGH);
            digitalWrite(red_led, LOW);
            digitalWrite(green_led, LOW);
            digitalWrite(blue_led, LOW);
            counter++;
          }

        case 7:
          {

            delay(100);
            stepper.setAcceleration(50000);
            stepper.runToNewPosition(Nema17matka);
            digitalWrite(yellow_led, LOW);
            digitalWrite(red_led, HIGH);
            digitalWrite(green_led, LOW);
            digitalWrite(blue_led, LOW);
            counter++;
          }

        case 8:
          {

            delay(100);
            stepper.setAcceleration(50000);
            stepper.runToNewPosition(-Nema17matka);
            digitalWrite(yellow_led, HIGH);
            digitalWrite(red_led, LOW);
            digitalWrite(green_led, LOW);
            digitalWrite(blue_led, LOW);
            counter++;
          }

        case 9:
          {

            delay(100);
            stepper.setAcceleration(50000);
            stepper.runToNewPosition(0);
            digitalWrite(yellow_led, LOW);
            digitalWrite(red_led, LOW);
            digitalWrite(green_led, HIGH);
            digitalWrite(blue_led, HIGH);
            counter++;

          }

      }

    }

  }
  counter = 0;


}
