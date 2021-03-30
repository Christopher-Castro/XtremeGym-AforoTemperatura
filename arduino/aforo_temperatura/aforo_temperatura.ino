

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
float temperatura = 0; // variable para almacenar la temperatura 
int s_ingreso = 2; //sensor de entrada
int s_salida = 3; //sensor de salida
int buzzer = 12; //buzzer
int tiempo = 1000; //tiempo de retardo para la variación de aforo
bool aux_ingreso = false;
bool aux_salida = false;



Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  
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
  if (Serial.available() > 0){
     aforo = (Serial.read()- '0')*10;  
  }
  temperatura = mlx.readObjectTempC();
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

  Serial.println(aforo);
  
  if (temperatura>39) {
    lcd.setCursor(0,0);
    lcd.print("Paso Restringido");
    //lcd.print(aforo);
    
    lcd.setCursor(0,1);
    lcd.print("Temperatura Alta");
    digitalWrite(buzzer, HIGH);
    delay(3000);
    digitalWrite(buzzer, LOW);
    
  } else if(temperatura>29) {
    if (temperatura>35) {
      lcd.setCursor(0,0);
      lcd.print("  Bien Venido!!");
      //cd.print(aforo);
      lcd.setCursor(0,1);
      lcd.print("Temp: ");
      lcd.print(temperatura);
    } else {
      lcd.setCursor(0,0);
      lcd.print("Intente de nuevo");
      //cd.print(aforo);
      lcd.setCursor(0,1);
      lcd.print("Temp: ");
      lcd.print(temperatura);
    }
  } else {
    lcd.setCursor(0,0);
    lcd.print("Aforo: ");
    lcd.print(aforo);
    lcd.setCursor(0,1);
    lcd.print("Temp: ----");
  };
  Serial.println(mlx.readObjectTempC());
}
