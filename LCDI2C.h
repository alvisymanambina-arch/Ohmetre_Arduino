#ifndef LCDI2C_H
#define LCDI2C_H

#define SDA 4
#define SCL 5
#define LCD_ADDR 0x27
#define BACKLIGHT 0x08

void i2c_start() {
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, HIGH);
  delayMicroseconds(5);

  digitalWrite(SDA, LOW);
  delayMicroseconds(5);
  digitalWrite(SCL, LOW);
}

void i2c_stop() {
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  delayMicroseconds(5);

  digitalWrite(SDA, HIGH);
  delayMicroseconds(5);
}

void i2c_write(uint8_t data) {
  for (int i = 0; i < 8; i++) {

    if (data & 0x80)
      digitalWrite(SDA, HIGH);
    else
      digitalWrite(SDA, LOW);

    digitalWrite(SCL, HIGH);
    delayMicroseconds(5);
    digitalWrite(SCL, LOW);

    data <<= 1;
  }

  pinMode(SDA, INPUT);
  digitalWrite(SCL, HIGH);
  delayMicroseconds(5);
  digitalWrite(SCL, LOW);
  pinMode(SDA, OUTPUT);
}

void lcd_pulse(uint8_t data) {
  i2c_start();

  i2c_write(LCD_ADDR << 1);
  i2c_write(data | 0x04 | BACKLIGHT);
  i2c_write((data & ~0x04) | BACKLIGHT);

  i2c_stop();
}

void lcd_send(uint8_t data, uint8_t mode) {

  uint8_t high = (data & 0xF0) | mode;
  uint8_t low = ((data << 4) & 0xF0) | mode;

  lcd_pulse(high);
  lcd_pulse(low);
}

void lcd_command(uint8_t cmd) {
  lcd_send(cmd, 0);
  delay(2);
}

void lcd_write(char c) {
  lcd_send(c, 1);
}

void lcd_print(const char *str) {
  while (*str) {
    lcd_write(*str++);
  }
}

void lcd_init() {

  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);

  delay(50);

  lcd_command(0x02);
  lcd_command(0x28);
  lcd_command(0x0C);
  lcd_command(0x06);
  lcd_command(0x01);

  delay(5);
}

void lcd_setCursor(uint8_t col, uint8_t row) {

  uint8_t row_offset[] = {0x00, 0x40};

  lcd_command(0x80 | (col + row_offset[row]));
}

#endif