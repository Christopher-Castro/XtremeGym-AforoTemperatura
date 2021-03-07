

/*
   Este código permite imprime un mensaje en el Puerto Serie a intérvalos de
   1 segundo que contiene un contador de personas
   Adicionalmente se incorpora una funcionalidad de medición de temperatura

   @author Christopher Alexis Castro Garcés, XtremeGym
*/
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>

int aforo = 0; //contador
int s_ingreso = 2; //sensor de entrada
int s_salida = 3; //sensor de salida
int tiempo = 1000; //tiempo de retardo para la variación de aforo
bool aux_ingreso = false;
bool aux_salida = false;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(s_ingreso, INPUT_PULLUP);
  pinMode(s_salida, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(s_ingreso),ingreso,FALLING);
  attachInterrupt(digitalPinToInterrupt(s_salida),salida,FALLING);
  
  Serial.begin(9600);
  mlx.begin();  
  lcd.begin();
  lcd.backlight();
  lcd.print("XtremeGym");
  delay(1000);
  lcd.clear();
}

void loop() {
  imprimir();
  
  if (aux_ingreso==true){
    delay(tiempo);
    aforo++;
    aux_ingreso=false;
  }
  if (aux_salida==true){
    delay(tiempo);
    aforo--;
    aux_salida=false;
  }
  
  delay(500);
}

void ingreso () {
  aux_ingreso = true;
}

void salida () {
  aux_salida = true;

}

void imprimir (){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Aforo: ");
  lcd.print(aforo);
  Serial.println(aforo);
  lcd.setCursor(0,1);
  lcd.print("Temp: ");
  lcd.print(mlx.readObjectTempC());
  Serial.println(mlx.readObjectTempC());
}
