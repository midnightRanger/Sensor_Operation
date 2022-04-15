#define temp_pin A2
#define light_pin A1
#define buzzer_pin 5

#define light_delay 2000
#define beep_delay 3000
#define temp_delay 1000

#include <Wire.h>
#include <Arduino.h>

float MAX_TEMP = -273;
float MIN_LIGHT = 900;

float current_temp;
float current_light;

bool is_first_launch = true;
bool is_beeping = false;
bool is_lighting = false;

long bepp_millis = 0;
void beep()
{
  if (is_beeping && (millis() - bepp_millis > beep_delay))
  {
    tone(buzzer_pin, 1000, 300); // пищать на пине 3, 1 кГц
    bepp_millis = millis();
  }
}

//звуковое оповещение
void handle_temp(float temp)
{
  if (temp > MAX_TEMP)
  {
    Serial.println("beep!");
    is_beeping = true;
  }
  else
  {
    is_beeping = false;
  }
}

void handle_light(float light)
{
  if (light > MIN_LIGHT)
  {
    Serial.println("Too dark!");
    is_lighting = true;
  }
  else
  {
    is_lighting = false;
  }
}

void max_temp_receive()
{
  while (MAX_TEMP == -273)
  {
    String input_command = Serial.readString();
    Serial.print("Введите максимальную температуру: ");
    MAX_TEMP = input_command.toFloat();
  }
}

void min_light_receive()
{
  while (MIN_LIGHT == 900)
  {
    String input_command = Serial.readString();
    Serial.print("Введите минимальную степень освещенности: ");
    MIN_LIGHT = input_command.toFloat();
  }
}

void command_handle()
{
  String input_command;
  while (input_command == "")
  {
    input_command = Serial.readString();
    MAX_TEMP = input_command.toFloat();
  }
}

//измерение температуры раз в секунду
void mensure_temp()
{
  int analogTemp = analogRead(temp_pin);
  current_temp = ((5 / 1023.0) * analogTemp) * 100;
  Serial.print("Температура: ");
  Serial.println(current_temp);
  handle_temp(current_temp);
}

//измерение освещенности раз в 2 секунды
void mensure_light()
{
  int analogLight = analogRead(light_pin);
  current_light = ((5 / 1023.0) * analogLight) * 100;
  Serial.print("Текущий уровень освещенности: ");
  Serial.println(analogLight);
  handle_light(current_light);
}

void setup()
{
  pinMode(temp_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);
  Serial.begin(9600);
}

long temp_millis = 0;
long light_millis = 0;

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

    else if (millis() - light_millis > light_delay)
    {
      mensure_light();
      light_millis = millis();
    }
    // beep();
  }
}