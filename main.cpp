#include <Arduino.h>
#include "stdio.h"
#include "string.h"
#include "esp_adc_cal.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include <esp_task_wdt.h>
#include "MapFloat.h"
#include "Nextion.h"


#define RXD2 16
#define TXD2 17
#define RXD1 9
#define TXD1 10
#define DE_RE 18  

uint16_t length = 0x0001;
uint8_t xmit_byte; 
char array[200];
char buffer1[10];
char buffer2[10];
char buffer3[10];
char buffer4[10];
char buffer5[10];
char buffer6[10];
char buffer7[10];
char buffer8[10];
char buffer9[10];
char buffer10[10];
char buffer11[10];
char buffer12[10];
char buffer13[10];
char *ptr = array;
int number1 = 7;
int number2 = 11;
int number3 = 15;
int number4 = 19;
int number5 = 23;
int number6 = 27;
int number7 = 31;
int number8 = 35;
int number9 = 39;
int number10 = 43;
int number11 = 47;
int number12 = 51;
int number13 = 55;
float float_number1;
float float_number2;
float float_number3;
float float_number4;
float float_number5;
float float_number6;
float float_number7;
float float_number8;
float float_number9;
float float_number10;
float float_number11;
float float_number12;
float float_number13;
int i;

//prototypes declaration
void parse_data (int index1, int index2, int index3, int index4,
                 int index5, int index6, int index7, int index8,
                 int index9, int index10, int index11, int index12,
                 int index13);
float bytes_to_float ( char *s);
void send_command (void);
 
void setup() {
  Serial.begin(19200);
  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);
  Serial2.begin(19200, SERIAL_8N1, RXD2, TXD2);
  pinMode (DE_RE, OUTPUT);
  //nexInit();
}

void loop()
{
  delay(1000);
  digitalWrite(DE_RE, HIGH); // set DE and RE to active HIGH TX mode
  send_command(); 
  delay(5);
  parse_data(number1, number2, number3, number4,
             number5, number6, number7, number8,
             number9, number10, number11, number12,
             number13);
  float_number1 = bytes_to_float(buffer1);
  Serial1.print("t1.txt=\"");
  Serial1.print(float_number1,2);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  float_number2 = bytes_to_float(buffer2);
  Serial1.print("t4.txt=\"");
  Serial1.print(float_number2,2);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  float_number3 = bytes_to_float(buffer3);
  Serial1.print("t7.txt=\"");
  Serial1.print(float_number3,2);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  float_number4 = bytes_to_float(buffer4);
  Serial1.print("t10.txt=\"");
  Serial1.print(float_number4,2);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  float_number5 = bytes_to_float(buffer5);
  Serial1.print("t13.txt=\"");
  Serial1.print(float_number5,2);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  float_number6 = bytes_to_float(buffer6);
  Serial1.print("t23.txt=\"");
  Serial1.print(float_number6,2);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  float_number7 = bytes_to_float(buffer7);
  Serial1.print("t24.txt=\"");
  Serial1.print(float_number7,2);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  float_number8 = bytes_to_float(buffer8);
  Serial1.print("t25.txt=\"");
  Serial1.print(float_number8,2);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  float_number9 = bytes_to_float(buffer9);
  Serial1.print("t26.txt=\"");
  Serial1.print(float_number9,2);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  float_number10 = bytes_to_float(buffer10);
  Serial1.print("t27.txt=\"");
  Serial1.print(float_number10,2);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  float_number11 = bytes_to_float(buffer11);
  Serial1.print("t28.txt=\"");
  Serial1.print(float_number11,2);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  float_number12 = bytes_to_float(buffer12);
  Serial1.print("t29.txt=\"");
  Serial1.print(float_number12,2);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  float_number13 = bytes_to_float(buffer13);
  Serial1.print("t30.txt=\"");
  Serial1.print(float_number13,2);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  
}
//parse data based on the indexes num in the function
void parse_data (int index1, int index2, int index3, int index4,
                 int index5, int index6, int index7, int index8,
                 int index9, int index10, int index11, int index12,
                 int index13)
{
  if(Serial2.available() > 2) 
  {
    for (i = 0; i<sizeof(array); i++)
    {
    array[i] = Serial2.read();
    }
    if ((array[0] == 0xFE) && (array[2] == 0x38))
     { 
      strlen (ptr = ptr+=index1);//advance pointer by index1 value in array
      memcpy (buffer1, ptr, 4);//copy 4 bytes of array to buffer1
      strlen (ptr = ptr-=index1);//subtract pointer by index1 value in array
      strlen (ptr = ptr+=index2);//advance pointer by index2 value in array
      memcpy (buffer2, ptr, 4);//copy 4 bytes of array to buffer2
      strlen (ptr = ptr-=index2);//subtract pointer by index2 value in array
      strlen (ptr = ptr+=index3);//advance pointer by index3 value in array
      memcpy (buffer3, ptr, 4);//copy 4 bytes of array to buffer3
      strlen (ptr = ptr-=index3);//subtract pointer by index3 value in array
      strlen (ptr = ptr+=index4);//advance pointer by index4 value in array
      memcpy (buffer4, ptr, 4);//copy 4 bytes of array to buffer4
      strlen (ptr = ptr-=index4);//subtract pointer by index4 value in array
      strlen (ptr = ptr+=index5);//advance pointer by index5 value in array
      memcpy (buffer5, ptr, 4);//copy 4 bytes of array to buffer5
      strlen (ptr = ptr-=index5);//subtract pointer by index5 value in array
      strlen (ptr = ptr+=index6);//advance pointer by index6 value in array
      memcpy (buffer6, ptr, 4);//copy 4 bytes of array to buffer6
      strlen (ptr = ptr-=index6);//subtract pointer by index6 value in array
      strlen (ptr = ptr+=index7);//advance pointer by index7 value in array
      memcpy (buffer7, ptr, 4);//copy 4 bytes of array to buffer7
      strlen (ptr = ptr-=index7);//subtract pointer by index7 value in array
      strlen (ptr = ptr+=index8);//advance pointer by index8 value in array
      memcpy (buffer8, ptr, 4);//copy 4 bytes of array to buffer8
      strlen (ptr = ptr-=index8);//subtract pointer by index8 value in array
      strlen (ptr = ptr+=index9);//advance pointer by index9 value in array
      memcpy (buffer9, ptr, 4);//copy 4 bytes of array to buffer9
      strlen (ptr = ptr-=index9);//subtract pointer by index9 value in array
      strlen (ptr = ptr+=index10);//advance pointer by index10 value in array
      memcpy (buffer10, ptr, 4);//copy 4 bytes of array to buffer10
      strlen (ptr = ptr-=index10);//subtract pointer by index10 value in array
      strlen (ptr = ptr+=index11);//advance pointer by index11 value in array
      memcpy (buffer11, ptr, 4);//copy 4 bytes of array to buffer11
      strlen (ptr = ptr-=index11);//subtract pointer by index11 value in array
      strlen (ptr = ptr+=index12);//advance pointer by index12 value in array
      memcpy (buffer12, ptr, 4);//copy 4 bytes of array to buffer12
      strlen (ptr = ptr-=index12);//subtract pointer by index12 value in array
      strlen (ptr = ptr+=index13);//advance pointer by index13 value in array
      memcpy (buffer13, ptr, 4);//copy 4 bytes of array to buffer13
      strlen (ptr = ptr-=index13);//subtract pointer by index13 value in array
      }
    }     
  }
/*This function saves the buffer contain inside the union
//with the respective float representaion of 4 x bytes of the buffer
and then return float_Value back to the main loop*/
float bytes_to_float ( char *s)
{
   union {
        char temp_array[4];
        float float_variable;
    } float_Value;
    float_Value.temp_array[3] = s[0];
    float_Value.temp_array[2] = s[1];
    float_Value.temp_array[1] = s[2];
    float_Value.temp_array[0] = s[3];
    return float_Value.float_variable; 
}
/*Send command to the charger controller to take and then retrieve data 
from it*/ 
void send_command (void)
{
 Serial2.write(0x00); // slave address
 xmit_byte = length>>8; // retrieve the first byte of the length
 Serial2.write(xmit_byte);
 xmit_byte = length; // retrieve the second byte of the length
 Serial2.write(xmit_byte);
 Serial2.write(0x01); // send a command to the charger controller
 Serial2.flush(); // wait the TX buffer is empty, transmission is complete
 delay(1);
 digitalWrite(DE_RE, LOW); // set DE and RE to active LOW RX mode
}