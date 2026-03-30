#include "LCDI2C.h"

#define mesurePin A0       
#define rfR 1000.0          
float r;                
char buffer[10];         

void setup() {
  // i2c_init();              
  lcd_init();              

  lcd_setCursor(0, 0);
  lcd_print(" Mesure Resistance ");
  lcd_setCursor(0, 1);
  lcd_print(" Initialisation...");
  delay(2000);
  lcd_command(0x01);       
}

void loop() {
  long somme = 0;
  for (int i = 0; i < 10; i++) {
    somme += analogRead(mesurePin);
    delay(10);
  }
  float val = somme / 10.0;

  float voltage = val * (5.0 / 1023.0);

  lcd_command(0x01); 

  if (voltage < 0.05) {
    lcd_setCursor(0, 0);
    lcd_print("Placez une resistance");
  } else {
    
    r = (rfR * (5.0 - voltage)) / voltage;

    lcd_setCursor(0, 0);
    lcd_print(" R =");

    if (r < 1000) {
      dtostrf(r, 6, 2, buffer);  
      lcd_print(buffer);
      lcd_print(" Ohm ");
    } else if (r < 1000000) {
      dtostrf(r / 1000.0, 6, 2, buffer);
      lcd_print(buffer);
      lcd_print(" kOhm");
    } else if (r < 10000000) {
      dtostrf(r / 1000000.0, 6, 2, buffer);
      lcd_print(buffer);
      lcd_print(" MOhm");
    } else {
      lcd_print("Trop grande R !");
    }
  }

  delay(3000); 
}
