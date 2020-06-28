
//1. leggere il dato da monitor seriale arduino. es. m1180

//2. quando ho ricevuto il dato , devo spezzarlo in 2 . es datototale = "m1180", 
    // nomeMotore da caratteri 1 e 2 , posizione dal 3 carattere in poi
    // es. nomeMotore="m1" posizioneMotore="180"

//3. inviare la posizione al motore 

//4. impostare finecorsa software per impedire al servo di raggiungere posizioni che lo
  // porterebbero ad un surriscaldamento
  
//5. leggere un dato da bluetooth e proseguire la logica come in punto 2  

////

//1. la logica riceve input da monitor seriale e invoca 2 funzioni per ricevere i dati con 
// inizio trasmissione "<" e fine ">" , 
// es <m1180> , es <m1180><m290>




//serial region
const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;
//serial region


//servo region
#include <Servo.h>

Servo servo01;
Servo servo02;
Servo servo03;
Servo servo04;
Servo servo05;
Servo servo06;




int gradiMotore1Min=1;int gradiMotore1Max=179;
int gradiMotore2Min=10;int gradiMotore2Max=120;
int gradiMotore3Min=10;int gradiMotore3Max=90;
int gradiMotore4Min=10;int gradiMotore4Max=90;
int gradiMotore5Min=10;int gradiMotore5Max=90;
int gradiMotore6Min=20;int gradiMotore6Max=90;

int servo1Pos, servo2Pos, servo3Pos, servo4Pos, servo5Pos, servo6Pos; // current position
int servo1PPos, servo2PPos, servo3PPos, servo4PPos, servo5PPos, servo6PPos; // previous position


//servo region

//bt region
#include <SoftwareSerial.h>
SoftwareSerial bluetooth(3, 4);
String message; //string that stores the incoming message

//bt region



void setup() {
    Serial.begin(9600);
    Serial.println("<Arduino is ready>");
    Serial.println("<M4100>");


    //servo region
    servo01.attach(5);
    servo02.attach(6);
    servo03.attach(7);
    servo04.attach(8);
    servo05.attach(9);
    servo06.attach(10);  
    
    servo1Pos = servo1PPos = servoMotion(90, 500 , 1);
    servo02.write(90);
    servo03.write(90);
    servo04.write(90);
    servo05.write(90);
    servo06.write(45);



    //servo region



    
    //bt region
    bluetooth.begin(9600); //set baud rate
    //bt region
}

void loop() {
  
    recvWithStartEndMarkers();
    
    showNewData("");
    
    mybluetooth();
    
  }

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}



void showNewData(String message) {
  
    //Serial.print("showNewData ");Serial.print(message);
  
    if (newData == true || !message.equals("")) {
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
  String datiInArrivo;
  if (newData == true) {datiInArrivo = receivedChars; } 
  if (!message.equals("")) {datiInArrivo = message;} 
   
  
  String nomeMotore = datiInArrivo.substring(1, 2);
  Serial.println("il motore è: " + nomeMotore);


  int gradiMotore = (datiInArrivo.substring(2,datiInArrivo.length())).toInt();
  Serial.print("comandato ad andare sul grado: ");Serial.print(gradiMotore);
  Serial.println("");
  //3 

  switch (nomeMotore.toInt()) {
    case 1:    
      Serial.print("servo1PPos ");Serial.println(servo1PPos); 
      Serial.print("gradiMotore1Min ");Serial.println(gradiMotore1Min); 
      Serial.print("gradiMotore1Max ");Serial.println(gradiMotore1Max); 
      
      
      servo1PPos = servoMotion(gradiMotore,500 , 1); 
      
      
      
      
      break;
    
    case 2:    
      //muovi servo
      if (gradiMotore<gradiMotore2Min) {gradiMotore=gradiMotore2Min;}   
      if (gradiMotore>gradiMotore2Max) {gradiMotore=gradiMotore2Max;} 
      servo2PPos = servoMotion(gradiMotore,1000 , 2); 
      break;
      
     case 3:    
      //muovi servo
      Serial.print("motore 3");
      Serial.print("gradiMotore ");  Serial.println(gradiMotore);
      Serial.print("gradiMotore3Min ");  Serial.println(gradiMotore3Min);
      
      if (gradiMotore<gradiMotore3Min) {gradiMotore=gradiMotore3Min;  
      if (gradiMotore>gradiMotore3Max) {gradiMotore=gradiMotore3Max;} 
  
      Serial.print("comando finecorsa grado:");Serial.println(gradiMotore3Min);}
      
      servo3PPos = servoMotion(gradiMotore,1000 , 3); 

      break;
   
    
     case 4:    
      //<m490>muovi servo
      //int servoMotion(int posS, int dlyTime, int servoNbr ){
      Serial.print("servo4PPos  ");  Serial.println(servo4PPos);
      servo4PPos = servoMotion(gradiMotore,500 , 4);
      Serial.print("servo4PPos  ");  Serial.println(servo4PPos);
     
      break; 
      
     case 5:    
           servo5PPos = servoMotion(gradiMotore,500 , 5);

      
      break; 
      
     case 6:
      Serial.print("servo6PPos ");Serial.println(servo6PPos); 
      Serial.print("gradiMotore6Min ");Serial.println(gradiMotore6Min); 
      Serial.print("gradiMotore6Max ");Serial.println(gradiMotore6Max); 
      
      if (gradiMotore<gradiMotore6Min) {gradiMotore=gradiMotore6Min;}   
      if (gradiMotore>gradiMotore6Max) {gradiMotore=gradiMotore6Max;} 
      servo6PPos = servoMotion(gradiMotore,500 , 6); 
      break;
      
     
    //aggiungere altri motori

    default:
      // eventuale default
      Serial.println("default ");
      }

  //3
        newData = false;
        message = "";
    }
}

//bluetooth
void mybluetooth()
{
  while(bluetooth.available()){
    message+=char(bluetooth.read());
    Serial.println("connected");
  
  }
  if(!bluetooth.available())
  {
    if(message!="")
    {//if data is available
      
     //servo1.write(message.toInt());

      Serial.print("mybluetooth "); Serial.println(message); //show the data
      
     showNewData(message);
     
     message=""; //clear the data     
      
             
    }
  }
}

//dalla turca
int servoMotion(int posS, int dlyTime, int servoNbr ){
  int prevPos;
  Servo servo00;
  if (servoNbr == 1){
    prevPos = servo1PPos;
    servo00 = servo01;
  }
  if (servoNbr == 2){
    prevPos = servo2PPos;
    servo00 = servo02;
  }
  if (servoNbr == 3){
    prevPos = servo3PPos;
    servo00 = servo03;
  }
  if (servoNbr == 4){
    prevPos = servo4PPos;
    servo00 = servo04;
  }
  if (servoNbr == 5){
    prevPos = servo5PPos;
    servo00 = servo05;
  }
  if (servoNbr == 6){
    prevPos = servo6PPos;
    servo00 = servo06;
    //if (servo6PPos<gradiMotore6Min) {servo6PPos=gradiMotore6Min;} 
    //if (servo6PPos>gradiMotore6Max) {servo6PPos=gradiMotore6Max;} 

  // finire finecorsa software

  }
  if (prevPos > posS){
    for ( int i = prevPos; i >= posS; i--){ // Run servo down
      servo00.write(i);
      delay(20);// defines the speed at which the servo rotates
    }   
    // If previous position is smaller then current position
  }
    
    Serial.print("prevPos");      Serial.println(prevPos);
    Serial.print("posS");    Serial.println(posS);


  
  if (prevPos < posS){
    for ( int i = prevPos; i <= posS; i++){ // Run servo up
      servo00.write(i);
      delay(20);
      Serial.println(i);

    }
  }
  prevPos = posS; // set current position as previous position
  return prevPos;
}
