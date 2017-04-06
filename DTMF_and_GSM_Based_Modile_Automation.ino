/**************************************************************
 DTMF (mobile) based Home automation

 Author : Jithin T; DEBUT LABS (jithin.1005@gmail.com)
 Authored on : 01 March 2017

 This Arduino Sketch is part of DTMF and GSM Based Home Automation tutorial.

 this sketch will do all what is necessary to answer an incomming call (from any number), receive keypad number dials (from remote caller) and operate relays as per instruction.
 Here we have intrerfaced only two relays. First keytone will deceive the relay ('1' for Relay 1 -connected to D8- and '2' for Relay 2 -connected to D9-)
 Second key tone will decide the status of the Relay ('1' for relay 'ON' and '2' for Relay 'OFF') any other key strokes will be neglected.

 Circuit Necessary
 > Arduino Uno
 > GSM module - with SIM800
 >Relays

 
**************************************************************/


#include <SoftwareSerial.h>
// defining communicationion pins for Software Serial
# define GSM_RX 10 // Connect TX of GSM module
# define GSM_TX 11 // Connect RX of GSM module


// Defining interfacing pins for Relay board
# define RELAY_1 8 // Connect the Control of Relay 1 toD8
# define RELAY_2 9 // Connect the Control of Relay 2 toD9

SoftwareSerial gsm_board(GSM_RX,GSM_TX);

boolean call_flag=0, relay_flag=0;
int i=0,j=0,x=-1;
char n[3];
void gsm_initilaize();// used to inilitize the gsm and chk all its parameters
void relay(); // used to control relay outputs.

void setup() {
  // put your setup code here, to run once:
  gsm_board.begin(9600);
  Serial.begin(9600);
  pinMode(RELAY_1,OUTPUT);
  pinMode(RELAY_2,OUTPUT);
  digitalWrite(RELAY_1,LOW);
  digitalWrite(RELAY_2,LOW);
  gsm_initilaize();

}
//////////////////////////////setup ends/////////////////////////////

/////////////////////loop begins///////////////////////////
void loop() {

  String gsm_data; // to hold incomming communication from GSM module
   
  while(gsm_board.available())
  {
    char c=gsm_board.read();
    gsm_data+=c;
    delay(10);
  }  //read serial data and store it to gsm_data STRING instance;
  
  if(!call_flag) // if call is not in connected, checking for ring
  {
    x=gsm_data.indexOf("RING");
    if(x>-1)
    {
      delay(5000);
      gsm_board.println("ATA");
      call_flag=1;
    }
  }  // ring test over, call flag high if sim rings
  if(call_flag) // if call is connected
  {
    x=gsm_data.indexOf("DTMF"); //checkinh dtmf and storing approprietly
    if(x>-1)
    {
      n[j]=gsm_data[x+6];
      Serial.println(n[j]);
      j++;
    }
    x=gsm_data.indexOf("NO CARRIER"); // Checking whether call is still connected or not
    if(x>-1)
    {
      gsm_board.println("ATH");
      relay_flag=1;
      call_flag=0;
      j=0;
    }
  }

  if(relay_flag) // If a call was resently disconnected, changing relay states accordingly
  {
    relay();
  }
  
}
//////////////////////////////loop ends/////////////////////////////

/////////////////////gsm inilitize begins///////////////////////////

void gsm_initilaize()
{
  boolean gsm_Ready=1;
  Serial.println("initializing GSM module");
  while(gsm_Ready>0)
  {
   gsm_board.println("AT");
   Serial.println("AT"); 
   while(gsm_board.available())
   {
     if(gsm_board.find("OK")>0)
       gsm_Ready=0;
   }
   delay(2000);
  }
  Serial.println("AT READY");
  // GSM MODULE REPLIED 'OK' TO 'AT' INPUT, INDICAING THE MODULE IS OK AND FUNCTIONING
  
  boolean ntw_Ready=1;
  Serial.println("finding network");
  while(ntw_Ready>0)
  {
   gsm_board.println("AT+CPIN?");
   Serial.println("AT+CPIN?"); 
   while(gsm_board.available())
   {
     if(gsm_board.find("+CPIN: READY")>0)
       ntw_Ready=0;
   }
   delay(2000);
  }
  Serial.println("NTW READY");

// GSM MODULE REPLIED '+CPIN:READY' TO 'AT+CPIN?' INPUT, INDICAING THE NETWORK IS OK AND FUNCTIONING
  
  boolean DTMF_Ready=1;
  Serial.println("turning DTMF ON");
  while(DTMF_Ready>0)
  {
   gsm_board.println("AT+DDET=1");
   Serial.println("AT+DDET=1"); 
   while(gsm_board.available())
   {
     if(gsm_board.find("OK")>0)
       DTMF_Ready=0;
   }
   delay(2000);
  }
  Serial.println("DTMF READY");// GSM MODULE REPLIED '+OK' TO 'AT+DDET=1?' INPUT, INDICAING THE DTMF IS ON;
}

//////////////////////////////gsm inilitization ends/////////////////////////////

/////////////////////relay begins///////////////////////////

void relay()
{
  if(n[0]=='1') // if RELAY_1  was selected
    {
      if(n[1]=='1')
      {
        digitalWrite(RELAY_1,HIGH); //relay 1 on
        Serial.println("RELAY 1 ON");
      }
      else if(n[1]=='2')
      {
        digitalWrite(RELAY_1,LOW);// relay 1 off
        Serial.println("RELAY 1 OFF");
      }
    }
    else if(n[0]=='2') // if RELAY_2  was selected
    {
      if(n[1]=='1')
      {
        digitalWrite(RELAY_2,HIGH); // realy 2 on
        Serial.println("RELAY 2 ON");
      }
      else if(n[1]=='2')
      {
        digitalWrite(RELAY_2,LOW); // relay 2 off
        Serial.println("RELAY 2 OFF");
      }
    }
    relay_flag=0;
}

/////////////////////relay ends///////////////////////////


