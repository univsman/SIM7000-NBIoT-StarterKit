/* This sketch design for demo SIMCOM 's SIM7000E NBIoT mdoule;
 * The hardware is SIM7000 NBIoT Starter kit;
 * The kit combine Arduino Leonardo and SIM7000 Sheild together,compatible with Arduino IED;
 * You can see the update from https://github.com/univsman/NBIOT-Starter-kit
 * The starter kit also integrated HTU21DF Temperature and Humidity sensor;
 * The SIM7000 support NBIoT , LTE Cat M1, GNSS and GSM;
 * AT+CMNB=2 was setted NBIoT, AT+CMNB=1 was setted LTE CAT M1;
 * The Char Telcom_APN[8] is the APN Varible,
 * You need set the correct APN and the Number, the number is length of APN +1;
 * This sketch is very sample version, just for Evaluate Starter kit purpose.
 * 
 * Sketch Function:
 * MCU collect Temperature and Humidity from HTU21DF 
 * send the datas to MQTT Broker of Thingspeak: https://thingspeak.com/channels/544857
 *  
 * Github: https://github.com/univsman/NBIOT-Starter-kit
 * MQTT Broker : https://thingspeak.com/channels/544857
 *   
 * Author: Steven Li , Email : univsman@gmail.com
 * 
 * Last Updated: 18/Sept/2018
 * Version: V1.0 , we will keep upgrade
 * if you have any question , please email to me.
 */
//#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include <Arduino.h>
//#include <SoftwareSerial.h>

#define Button_Pin 07
#define LED_Indicator 13
#define Power_Key 04
#define Power_En 05
#define Reset_Pin 12

//const byte Button_Pin = 07;
//const byte LED_Indicator = 13;
//const byte Power_Key = 04;
//const byte Power_En = 05;
//const byte Reset_Pin = 12;

//Variable Defing
char Telcom_APN[8] = "E-IDEAS";
String comdata = "12345678901234567890123456789012345678901234567890";
String Message = "status=Location: 1.274217, 103.799758; &field1=39&field2=42";
short int Message_Length = 0;
short int Cnt = 0;
short int Cnt1 = 0;
float Temperature = 0;
float Humidity = 0;

Adafruit_HTU21DF htu = Adafruit_HTU21DF();
/*========================================================================*/
//setup routing
void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(Power_Key, OUTPUT) ; // SIM7000 Power Key
  pinMode(Button_Pin, INPUT_PULLUP) ; // Button Pin
  pinMode(Reset_Pin, OUTPUT); // SIM7000 Reset Pin
  pinMode(LED_Indicator, OUTPUT); // LED Indicator
  pinMode(Power_En, OUTPUT); // LED Indicator
  digitalWrite(Power_Key, LOW);// SIM7000 Power Key
  digitalWrite(Reset_Pin, HIGH);// SIM7000 Reset Pin
  digitalWrite(Power_En, HIGH);// SIM7000 Reset Pin
  digitalWrite(LED_Indicator, LOW);// LED
  delay(5000);
  //while (!Serial) { } // Debugging porint
  Serial.println("Sim7000 Starter kit initial success");
  Serial.println("");
  if (!htu.begin()) {Serial.println("Couldn't find sensor!");}
}
/*========================================================================*/
//Main Routing
void loop()
{
  SIM7000boot();
  SIM7000_Initial();
  SIM7000_MQTT_Disconnect();
  digitalWrite(Button_Pin, HIGH);//Intial completed LED indicator set on*/
  while(1)
  {
    delay(10);
    Sensor_HTU21DF();
    delay(10);
    Message = "field1="+String(Temperature)+"&field2="+String(Humidity);
    Serial.println( Message );
    Message_Length = Message.length() + 1;
    Serial.println( "" );
    SIM7000_MQTT_Connect();
    delay(10);
    SIM7000_MQTT_Pub_Message();
    delay(10);
    SIM7000_MQTT_Disconnect();

    int i=0;
    for(i=0;i<100;i++)
    {
      delay(1000);
    }
  }
}
/*========================================================================*/
void SIM7000boot()  //SIM7000 Reboot routing
{
  Serial.println( "Waiting Module Ready" );
  Serial.println( "" );
  digitalWrite(Power_Key, LOW);// Power On
  comdata = "";
  while ( comdata.indexOf("OK") < 0 )
  {
    Serial.println( "loop" );
    Serial1.println("AT");
    comdata = Serial1.readString();
    comdata.trim();
    Serial.println( comdata );
    delay(1000);
  }
  Serial.println("");
  Serial.println("Sim7000 Start Succeed! \n");
}

/*========================================================================*/
void Sensor_HTU21DF()
{
  Serial.println( "HTU21DF Sensor Value" );
  Temperature = htu.readTemperature();
  Humidity = htu.readHumidity();
  Serial.print( "Temperature: ");
  Serial.print( htu.readTemperature());
  Serial.println("C");
  Serial.print( "Humidity: ");
  Serial.print( htu.readHumidity());
  Serial.println("%RH");
  delay(100);
}
/*========================================================================*/
void SIM7000_Initial() //SIM7000 Module Initialization, attach to network and register in telcom.
{
  digitalWrite(Power_Key, HIGH);
  Serial.println( "Check Model Version...\n" );
  Serial1.println("AT+SIMCOMATI");
  comdata = "";
  while ( comdata.indexOf("OK") < 0  )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  /*------------------------------------------------------------------------*/
  Serial.println( "" );
  Serial.println( "Check SIM Card...\n" );
  Serial1.println("AT+CPIN?");
  comdata = "";
  while ( comdata.indexOf("OK") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
    if (comdata.indexOf("ERROR") > 0)
    {
      Serial.println( "Please insert sim card then reboot...\n" );
      comdata = "";
    }
  }
  Serial.println("");
  /*------------------------------------------------------------------------*/
  Serial.println("Set LTE Mode");
  Serial1.println("AT+CNMP=38");
  comdata = "";
  while ( comdata.indexOf("OK") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");
  /*------------------------------------------------------------------------*/
  Serial.println("Set NBIOT Mode");
  Serial1.println("AT+CMNB=2");
  comdata = "";
  while ( comdata.indexOf("OK") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");
  /*------------------------------------------------------------------------*/
  Serial.println("Set APN!\n");
  Serial1.println("AT+CFUN=0");
  comdata = "";
  while ( comdata.indexOf("OK") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }

  Serial1.println(String("") + "AT+CGDCONT=1,\"IP\",\"" + Telcom_APN + "\"");
  comdata = "";
  while ( comdata.indexOf("OK") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");

  Serial1.println("AT+CFUN=1");
  comdata = "";
  while ( comdata.indexOf("+CPIN: READY") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");
  /*------------------------------------------------------------------------*/
  Serial.println("Set Phone Functionality!\n");
  Serial1.println("AT+CFUN?");
  comdata = "";
  while ( comdata.indexOf("+CFUN: 1") < 0   )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  /*------------------------------------------------------------------------*/
  Serial.println("Check Signal of NBIoT!\n");
  comdata = "99";
  while ( (comdata.toInt() > 32) || (comdata.toInt() == 0) )
  {
    Serial1.println("AT+CSQ");
    while (!Serial1.available()) {}
    comdata = Serial1.readString();
    comdata.trim();
    Serial.println( comdata );

    Cnt = comdata.indexOf(": ");
    Cnt1 = comdata.indexOf(",");
    comdata = comdata.substring(Cnt + 2, Cnt1);
    //comdata =1 ;
    delay(10);
  }
  Serial.print("Signal Strength is approximately ");
  Serial.print( comdata.toInt() * 2 - 113);
  Serial.println(" dbm");
  /*------------------------------------------------------------------------*/
  Serial.println("Network Registration Status");
  comdata = "";
  while ( comdata.indexOf("+CGREG: 0,1") < 0 )
  {
    Serial1.println("AT+CGREG?");
    delay(10);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("Network Registered");
  /*------------------------------------------------------------------------*/
  Serial.println("Inquiring UE system information");
  Serial1.println("AT+CPSI?");
  comdata = "";
  while ( comdata.indexOf("+CPSI:") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("Network Registered");
  /*------------------------------------------------------------------------*/
  Serial.println("Operator Selection information");
  Serial1.println("AT+COPS?");
  comdata = "";
  while ( comdata.indexOf("+COPS: 0,0,") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");
  /*------------------------------------------------------------------------*/
  Serial.println("Show PDP Address");
  Serial1.println("AT+CGPADDR");
  comdata = "";
  while ( comdata.indexOf("OK") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");
}
/*========================================================================*/
void SIM7000_MQTT_Connect()
{
  Serial1.println(String("") + "AT+CNACT=2,\"" + Telcom_APN + "\"");
  comdata = "";
  while ( comdata.indexOf("PDP: ACTIVE") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");

  Serial1.println("AT+CNACT?");
  comdata = "";
  while ( comdata.indexOf("CNACT: 1") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");

  Serial1.println("AT+SMCONF=\"CLIENTID\",ID");
  comdata = "";
  while ( comdata.indexOf("OK") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");

  Serial1.println("AT+SMCONF=\"KEEPTIME\",60");
  comdata = "";
  while ( comdata.indexOf("OK") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");

  Serial1.println("AT+SMCONF=\"CLEANSS\",1");
  comdata = "";
  while ( comdata.indexOf("OK") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");

  Serial1.println("AT+SMCONF=\"URL\",\"mqtt.thingspeak.com:1883\"");
  comdata = "";
  while ( comdata.indexOf("OK") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");

  Serial1.println("AT+SMCONN");
  comdata = "";
  while ( comdata.indexOf("OK") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");

  Serial1.println("AT+SMSUB=\"channels/544857/subscribe/XML/OXPTE9Z6L32TANZ6\",1");
  comdata = "";
  while ( comdata.indexOf("OK") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");
}
/*========================================================================*/
void SIM7000_MQTT_Pub_Message()
{
  Serial1.println(String("") + "AT+SMPUB=\"channels/544857/publish/YR2Y8P8K8B4ASYTQ\"," + Message_Length + ",0,0");
  comdata = "";
  while ( comdata.indexOf(">") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }

  Serial1.println(Message);
  comdata = "";
  while ( comdata.indexOf("OK") < 0 )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
}
/*========================================================================*/
void SIM7000_MQTT_Disconnect()
{

  Serial1.println("AT+SMSTATE?");
  comdata = "";
  while ( (comdata.indexOf("OK") < 0 ) )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");

  if (comdata.indexOf("+SMSTATE: 1") > 0)
  {
  Serial1.println("AT+SMDISC");
  comdata = "";
  while ( (comdata.indexOf("OK") < 0 ) && (comdata.indexOf("ERROR") < 0 ) )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");

  Serial1.println("AT+CNACT=0");
  comdata = "";
  while ( (comdata.indexOf("+APP PDP: DEACTIVE") < 0) && (comdata.indexOf("ERROR") < 0 ) )
  {
    delay(2);
    while (Serial1.available())
    {
      comdata = Serial1.readString();
      comdata.trim();
      Serial.println( comdata );
    }
  }
  Serial.println("");
  comdata = "";
  }
}
/*========================================================================*/
