# SIM7000-NBIoT-StarterKit
SIM7000 NBIoT Starter kit MQTT demo with Thingspeak Borker

 * This sketch design for demo SIMCOM 's SIM7000E NBIoT mdoule;
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
