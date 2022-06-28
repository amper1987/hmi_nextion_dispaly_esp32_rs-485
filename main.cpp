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
#define CHARGER_SIZE 256
#define DATA_SIZE 32 

uint16_t i;
uint32_t status_register = 0;
uint8_t status_value;
uint32_t ten_second_timeout_flag = 0;
hw_timer_t * timer = NULL;
uint16_t length = 0x0001;
uint8_t xmit_byte; 
char measurement_buff[CHARGER_SIZE];
char rssi_buff[DATA_SIZE];
char status_buff[DATA_SIZE];
char power_board_output_current_avg_buff[DATA_SIZE];
char power_board_output_current_max_buff[DATA_SIZE];
char power_board_output_current_min_buff[DATA_SIZE];
char power_board_output_voltage_buff[DATA_SIZE];
char output_current_avg_buff[DATA_SIZE];
char output_current_max_buff[DATA_SIZE];
char output_current_min_buff[DATA_SIZE];
char output_voltage_buff[DATA_SIZE];
char generator_battery_voltage_buff[DATA_SIZE];
char tempearure_sensor_0_buff[DATA_SIZE];
char temperature_sensor_1_buff[DATA_SIZE];
char temperature_sensor_2_buff[DATA_SIZE];
char temperature_sensor_3_buff[DATA_SIZE];
char *ptr = measurement_buff;
uint8_t rssi_index = 3;
uint8_t status_index = 7;
uint8_t power_board_output_current_avg_index =11;
uint8_t power_board_output_current_max_index = 15;
uint8_t power_board_output_current_min_index = 19;
uint8_t power_board_output_voltage_index = 23;
uint8_t output_current_avg_index = 27;
uint8_t output_current_max_index = 31;
uint8_t output_current_min_index = 35;
uint8_t output_voltage_index = 39;
uint8_t generator_battery_voltage_index = 43;
uint8_t tempearature_sensor_0_index = 47;
uint8_t temperature_sensor_1_index = 51;
uint8_t temperature_sensor_2_index = 55;
uint8_t temperature_sensor_3_index = 59;
float rssi;
float status;
float power_board_output_current_avg;
float power_board_output_current_max;
float power_board_output_current_min;
float power_board_output_voltage;
float output_current_avg;
float output_current_max;
float output_current_min;
float output_voltage;
float generator_battery_voltage;
float tempearature_sensor_0;
float temperature_sensor_1;
float temperature_sensor_2;
float temperature_sensor_3;


void IRAM_ATTR onTimer()
{
  ten_second_timeout_flag = 1;
}

//prototypes declaration
void parse_data (uint8_t rssi_index,
                 uint8_t status_index,
                 uint8_t power_board_output_current_avg_index,
                 uint8_t power_board_output_current_max_index,
                 uint8_t power_board_output_current_min_index,
                 uint8_t power_board_output_voltage_index,
                 uint8_t output_current_avg_index,
                 uint8_t output_current_max_index, 
                 uint8_t output_current_min_index,
                 uint8_t output_voltage_index,
                 uint8_t generator_battery_voltage_index,
                 uint8_t tempearature_sensor_0_index,
                 uint8_t temperature_sensor_1_index,
                 uint8_t temperature_sensor_2_index,
                 uint8_t temperature_sensor_3_index);
float bytes_to_float (char *s);
void send_command (void);
void hmi_display_data (void);
void receive_data (void);
 
void setup() 
{
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);
  Serial2.begin(19200, SERIAL_8N1, RXD2, TXD2);
  pinMode (DE_RE, OUTPUT);
  nexInit();
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 5000000, true);
  timerAlarmEnable(timer);
}

void loop()
{
  if(ten_second_timeout_flag)
  {
  digitalWrite(DE_RE, HIGH); // set DE and RE to active HIGH TX mode
  send_command();
  digitalWrite(DE_RE, LOW); // set DE and RE to active LOW RX mode 
  delay(5);
  receive_data();
  parse_data(rssi_index,
             status_index,
             power_board_output_current_avg_index,
             power_board_output_current_max_index,
             power_board_output_current_min_index,
             power_board_output_voltage_index,
             output_current_avg_index,
             output_current_max_index, 
             output_current_min_index,
             output_voltage_index,
             generator_battery_voltage_index,
             tempearature_sensor_0_index,
             temperature_sensor_1_index,
             temperature_sensor_2_index,
             temperature_sensor_3_index);
  hmi_display_data();
  memset(measurement_buff,0,sizeof(measurement_buff));
  ten_second_timeout_flag = 0;
  }
} 
//receive data and store in measurement_buff
void receive_data (void)
{
  if(Serial2.available())
  {
    for (i = 0; i<sizeof(measurement_buff); i++)
    {
    measurement_buff[i] = Serial2.read();
    }
  }
}
//parse data based on the indexes num in the function
void parse_data (uint8_t index0, uint8_t index1, uint8_t index2, uint8_t index3,
                 uint8_t index4, uint8_t index5, uint8_t index6, uint8_t index7,
                 uint8_t index8, uint8_t index9, uint8_t index10, uint8_t index11,
                 uint8_t index12, uint8_t index13, uint8_t index14)
{
  if ((measurement_buff[0] == 0xFE) && (measurement_buff[2] == 0x3C))
  {
  strlen (ptr = ptr+=index0);//advance pointer by index1 value in array
  memcpy (rssi_buff, ptr, 4);//copy 4 bytes of array to rssi_buff
  strlen (ptr = ptr-=index0);//subtract pointer by index1 value in array 
  strlen (ptr = ptr+=index1);//advance pointer by index1 value in array
  memcpy (status_buff, ptr, 4);//copy 4 bytes of array to status_buff
  strlen (ptr = ptr-=index1);//subtract pointer by index1 value in array 
  strlen (ptr = ptr+=index2);//advance pointer by index1 value in array
  memcpy (power_board_output_current_avg_buff, ptr, 4);//copy 4 bytes of array power_board_output_current_avg_buff
  strlen (ptr = ptr-=index2);//subtract pointer by index1 value in array
  strlen (ptr = ptr+=index5);//advance pointer by index4 value in array
  memcpy (power_board_output_voltage_buff, ptr, 4);//copy 4 bytes of array to power_board_output_voltage_buff
  strlen (ptr = ptr-=index5);//subtract pointer by index4 value in array
  strlen (ptr = ptr+=index6);//advance pointer by index5 value in array
  memcpy (output_current_avg_buff, ptr, 4);//copy 4 bytes of array to output_current_avg_buff
  strlen (ptr = ptr-=index6);//subtract pointer by index5 value in array
  strlen (ptr = ptr+=index9);//advance pointer by index8 value in array
  memcpy (output_voltage_buff, ptr, 4);//copy 4 bytes of array to output_voltage_buff
  strlen (ptr = ptr-=index9);//subtract pointer by index8 value in array
  strlen (ptr = ptr+=index10);//advance pointer by index9 value in array
  memcpy (generator_battery_voltage_buff, ptr, 4);//copy 4 bytes of array to generator_battery_voltage_buff
  strlen (ptr = ptr-=index10);//subtract pointer by index9 value in array
  strlen (ptr = ptr+=index11);//advance pointer by index10 value in array
  memcpy (tempearure_sensor_0_buff, ptr, 4);//copy 4 bytes of array to tempearure_sensor_0_buff
  strlen (ptr = ptr-=index11);//subtract pointer by index10 value in array
  strlen (ptr = ptr+=index12);//advance pointer by index11 value in array
  memcpy (temperature_sensor_1_buff, ptr, 4);//copy 4 bytes of array to tempearure_sensor_1_buff
  strlen (ptr = ptr-=index12);//subtract pointer by index11 value in array
  strlen (ptr = ptr+=index13);//advance pointer by index12 value in array
  memcpy (temperature_sensor_2_buff, ptr, 4);//copy 4 bytes of array to temperature_sensor_2_buff
  strlen (ptr = ptr-=index13);//subtract pointer by index12 value in array
  strlen (ptr = ptr+=index14);//advance pointer by index13 value in array
  memcpy (temperature_sensor_3_buff, ptr, 4);//copy 4 bytes of array to temperature_sensor_3_buff
  strlen (ptr = ptr-=index14);//subtract pointer by index13 value in array
  }
} 

/*This function saves the buffer contain inside the union
//with the respective float representaion of 4 x bytes of the buffer
and then return float_Value back to the main loop*/
float bytes_to_float ( char *s)
{
  union
  {
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
}

// Send command to the HMI with all the buffers corresponding to
// the float values
void hmi_display_data (void)
{
power_board_output_current_avg = bytes_to_float(power_board_output_current_avg_buff);
Serial1.print("t0.txt=\"");
Serial1.print(power_board_output_current_avg,1);
Serial1.print("\"");
Serial1.print("\xFF\xFF\xFF");
power_board_output_voltage = bytes_to_float(power_board_output_voltage_buff);
Serial1.print("t1.txt=\"");
Serial1.print(power_board_output_voltage,1);
Serial1.print("\"");
Serial1.print("\xFF\xFF\xFF");
output_current_avg = bytes_to_float(output_current_avg_buff);
Serial1.print("t2.txt=\"");
Serial1.print(output_current_avg,1);
Serial1.print("\"");
Serial1.print("\xFF\xFF\xFF");
output_voltage = bytes_to_float(output_voltage_buff);
Serial1.print("t3.txt=\"");
Serial1.print(output_voltage,1);
Serial1.print("\"");
Serial1.print("\xFF\xFF\xFF");
generator_battery_voltage = bytes_to_float(generator_battery_voltage_buff);
Serial1.print("t4.txt=\"");
Serial1.print(generator_battery_voltage,1);
Serial1.print("\"");
Serial1.print("\xFF\xFF\xFF");
tempearature_sensor_0 = bytes_to_float(tempearure_sensor_0_buff);
tempearature_sensor_0 = tempearature_sensor_0/10.0;
  if (tempearature_sensor_0 != -100.0)
  {
  Serial1.print("t5.txt=\""); 
  Serial1.print(tempearature_sensor_0,1);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
  else
  {
  Serial1.print("t5.txt=\"");
  Serial1.print("");
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
temperature_sensor_1 = bytes_to_float(temperature_sensor_1_buff);
temperature_sensor_1 = temperature_sensor_1/10.0;
  if (temperature_sensor_1 != -100.0)
  {
  Serial1.print("t6.txt=\"");
  Serial1.print(temperature_sensor_1,1);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
  else
  {
  Serial1.print("t6.txt=\"");
  Serial1.print("");
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
temperature_sensor_2 = bytes_to_float(temperature_sensor_2_buff);
temperature_sensor_2 = temperature_sensor_2/10.0;
  if (temperature_sensor_2 != -100.0)
  {
  Serial1.print("t7.txt=\"");
  Serial1.print(temperature_sensor_2,1);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
  else
  {
  Serial1.print("t7.txt=\"");
  Serial1.print("");
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
temperature_sensor_3 = bytes_to_float(temperature_sensor_3_buff);
temperature_sensor_3 = temperature_sensor_3/10.0;
  if (temperature_sensor_3 != -100.0)
  {
  Serial1.print("t8.txt=\"");
  Serial1.print(temperature_sensor_3,1);
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
  else
  {
  Serial1.print("t8.txt=\"");
  Serial1.print("");
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }

status = bytes_to_float(status_buff);
  if (status == 0.0)
  {
  Serial1.print("t9.txt=\"");
  Serial1.print("GENERATOR OFF");
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
  if (status == 1.0)
  {
  Serial1.print("t9.txt=\"");
  Serial1.print("GENERATOR RUNNING");
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
  else if (status == 2.0)
  {
  Serial1.print("t9.txt=\"");
  Serial1.print("WARMUP SYSTEM IS ACTIVE");
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
  else if (status == 4.0)
  {
  Serial1.print("t9.txt=\"");
  Serial1.print("GENERATOR FAILED TO START");
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
  else if (status == 8.0)
  {
  Serial1.print("t9.txt=\"");
  Serial1.print("GENERATOR PREMATURELY STOPPED");
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
  else if (status == 16.0)
  {
  Serial1.print("t9.txt=\"");
  Serial1.print("GENERATOR FAILED TO STOP");
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
  else if (status == 32.0)
  {
  Serial1.print("t9.txt=\"");
  Serial1.print("GENERATOR FAILED TO CHARGE");
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
  else if (status == 64.0)
  {
  Serial1.print("t9.txt=\"");
  Serial1.print("AC INPUT FAULT");
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
  else if (status == 128.0)
  {
  Serial1.print("t9.txt=\"");
  Serial1.print("UNDERVOLTAGE LOCKOUT CONDITION");
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
  else if (status == 256.0)
  {
  Serial1.print("t9.txt=\"");
  Serial1.print("POWER OUTPUT FAULT");
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
  else if (status == 512.0)
  {
  Serial1.print("t9.txt=\"");
  Serial1.print("EMERGENCY SHUTDOWN SWITCH PRESSED");
  Serial1.print("\"");
  Serial1.print("\xFF\xFF\xFF");
  }
rssi = bytes_to_float(rssi_buff);
Serial1.print("t10.txt=\"");
Serial1.print(rssi,1);
Serial1.print("\"");
Serial1.print("\xFF\xFF\xFF");
}

  
