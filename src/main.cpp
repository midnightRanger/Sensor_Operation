// pinout
#define sensor_temp_pin A2
#define sensor_light_pin A1
#define sensor_co2_pin A3

#define led1_pin 13
#define led2_pin 12
#define buzzer_pin 5

#define light_delay 2000
#define beep_delay 3000
#define temp_delay 1000
#define co2_delay 5000
#define blink_delay 200

#include <MQ135.h>
#include <Wire.h>
#include <Arduino.h>

float MAX_TEMP = -273;
float MIN_LIGHT = -1;
float MIN_CO2 = -1;
float MAX_CO2 = 9999;

float current_temp;
float current_light;

bool is_first_launch = true;
bool is_beeping = false;
bool is_blinking = false;

long bepp_millis = 0;
void beep()
{
  if (is_beeping && (millis() - bepp_millis > beep_delay))
  {
    tone(buzzer_pin, 1000, 300); // пищать на пине 3, 1 кГц
    bepp_millis = millis();
  }
}

long blink_millis = 0;
bool led1_on = false;
void blink()
{
  if (is_blinking && (millis() - blink_millis > blink_delay))
  {
    digitalWrite(led1_pin, led1_on ? HIGH : LOW);
    led1_on = !led1_on;
    blink_millis = millis();
  }
}

//звуковое оповещение
void handle_temp(float temp)
{
  if (temp > MAX_TEMP)
  {
    Serial.println("! Высокий уровень температуры !");
    is_beeping = true;
  }
  else
  {
    is_beeping = false;
  }
}
//включение светодиода если уровень освещенности меньше MIN LIGHT
void handle_light(float light)
{
  if (light < MIN_LIGHT)
  {
    Serial.println("! Низкий уровень освещённости !");
    digitalWrite(led2_pin, HIGH);
  }
  else
  {
    digitalWrite(led2_pin, LOW);
  }
}

void handle_co2(float co2_level)
{
  if (co2_level < MIN_CO2)
  {
    Serial.println("! Маленькая концентрация CO2 ! ");
    digitalWrite(led1_pin, HIGH);
  }
  else if (co2_level > MAX_CO2)
  {
    Serial.println("!Высокая концентрация CO2! ");
    is_blinking = true;
  }
  else
  {
    is_blinking = false;
    digitalWrite(led1_pin, LOW);
  }
}

void command_handle()
{
  String input_command;
  Serial.print("Введите максимальное значение температуры: ");
  while (input_command == "")
  {
    input_command = Serial.readString();
    MAX_TEMP = input_command.toFloat();
    Serial.print(input_command);
  }
  Serial.println();

  input_command = "";
  Serial.print("Введите минимальное значение освещенности: ");
  while (input_command == "")
  {
    input_command = Serial.readString();
    MIN_LIGHT = input_command.toFloat();
    Serial.print(input_command);
  }
  Serial.println();

  input_command = "";
  Serial.print("Введите минимальное значение углекислого газа: ");
  while (input_command == "")
  {
    input_command = Serial.readString();
    MIN_CO2 = input_command.toFloat();
    Serial.print(input_command);
  }
  Serial.println();

  input_command = "";
  Serial.print("Введите максимальное значение углекислого газа: ");
  while (input_command == "")
  {
    input_command = Serial.readString();
    MAX_CO2 = input_command.toFloat();
    Serial.print(input_command);
  }
  Serial.println();
  Serial.println();
}

//измерение температуры раз в секунду
void mensure_temp()
{
  int analogTemp = analogRead(sensor_temp_pin);
  current_temp = ((5 / 1023.0) * analogTemp) * 100;
  Serial.print("Температура: ");
  Serial.println(current_temp);
  handle_temp(current_temp);
}

//измерение освещенности раз в 2 секунды
void mensure_light()
{
  int analogLight = analogRead(sensor_light_pin);
  current_light = ((5 / 1023.0) * analogLight) * 100;
  Serial.print("Текущий уровень освещенности: ");
  Serial.println(analogLight);
  handle_light(current_light);
}

void mensure_carbon_dioxide()
{
  MQ135 gasSensor = MQ135(sensor_co2_pin); // Attach sensor to pin A5
  Serial.print("Текущее значение углекислого газа: ");
  float rzero = gasSensor.getRZero();
  Serial.println(rzero);
  handle_co2(rzero);
}

void setup()
{
  pinMode(sensor_temp_pin, INPUT);
  // pinMode(sensor_co2_pin, INPUT); не нужно
  pinMode(buzzer_pin, OUTPUT);
  pinMode(led1_pin, INPUT);
  pinMode(led2_pin, INPUT);
  Serial.begin(9600);
}

long temp_millis = 0;
long light_millis = 0;
long co2_millis = 0;
void loop()
{

  if (is_first_launch)
  {
    is_first_launch = false;
    command_handle();
  }
  else
  {
    if (millis() - temp_millis > temp_delay)
    {
      mensure_temp();
      temp_millis = millis();
    }

    if (millis() - light_millis > light_delay)
    {
      mensure_light();
      light_millis = millis();
    }

    if (millis() - co2_millis > co2_delay)
    {
      mensure_carbon_dioxide();
      co2_millis = millis();
    }
    beep();
    blink();
  }
}