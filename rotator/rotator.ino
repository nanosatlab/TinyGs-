/*  AZ/EL Rotator controller for Arduino with servos

 */
#include <Wire.h>
#include <Servo.h>
Servo AzServo;  // create servo object to control the azimuth servo
Servo ElServo;  // create servo object to control the elevation servo

  String Azimuth = "";
  String Elevation = "";
  String ComputerRead = "";
  String ComputerWrite = "";
  int ComAzim = 0;
  int ComElev = 0;

void setup()   {
  Serial.begin(9600);
  Serial.setTimeout(50);           // miliseconds to wait for USB sata. Default 1000
 
  AzServo.attach(6);  // 
  ElServo.attach(5);  // 

}

void loop() {

  if (Serial.available()){
    SerComm();
    delay (100);
  }
  
}

void SerComm() {
  // initialize readings
  ComputerRead = "";
  Azimuth = "";
  Elevation = "";

  while(Serial.available()) {
    ComputerRead= Serial.readString();  // read the incoming data as string
    Serial.println(ComputerRead);     // echo the reception for testing purposes
  }
  
// looking for command <AZxxx.x>
    for (int i = 0; i <= ComputerRead.length(); i++) {
     if ((ComputerRead.charAt(i) == 'A')&&(ComputerRead.charAt(i+1) == 'Z')){ // if read AZ
      for (int j = i+2; j <= ComputerRead.length(); j++) {
        if (isDigit(ComputerRead.charAt(j))) {                                // if the character is number
          Azimuth = Azimuth + ComputerRead.charAt(j);
        }
        else {break;}
      }
     }
    }
    
// looking for command <ELxxx.x>
    for (int i = 0; i <= (ComputerRead.length()-2); i++) {
      if ((ComputerRead.charAt(i) == 'E')&&(ComputerRead.charAt(i+1) == 'L')){ // if read EL
        if ((ComputerRead.charAt(i+2)) == '-') {
          ComElev = 0;                  // if elevation negative
          break;
        }
        for (int j = i+2; j <= ComputerRead.length(); j++) {
          if (isDigit(ComputerRead.charAt(j))) {                               // if the character is number
            Elevation = Elevation + ComputerRead.charAt(j);
          }
          else {break;}
        }
      }
    }
    
// if <AZxx> received
    if (Azimuth != ""){
      ComAzim = Azimuth.toInt();          
      ComAzim= (ComAzim / 2) % 180;
       AzServo.write(ComAzim);
      }

// if <ELxx> received
    if (Elevation != ""){
      ComElev = Elevation.toInt();
      //Change reference
      ComElev = 130 - ComElev * (130 - 40) / 90;
      ElServo.write(ComElev);
    }

// looking for <AZ EL> interogation for antenna position
  for (int i = 0; i <= (ComputerRead.length()-4); i++) {
    if ((ComputerRead.charAt(i) == 'A')&&(ComputerRead.charAt(i+1) == 'Z')&&(ComputerRead.charAt(i+3) == 'E')&&(ComputerRead.charAt(i+4) == 'L')){
    // send back the antenna position <+xxx.x xx.x>
      ComAzim= (ComAzim / 2) % 180;
      ComElev = 130 - ComElev * (130 - 40) / 90;
      ComputerWrite = "+"+String(ComAzim)+".0 "+String(ComElev)+".0";
      Serial.println(ComputerWrite);
    }
  }
}
