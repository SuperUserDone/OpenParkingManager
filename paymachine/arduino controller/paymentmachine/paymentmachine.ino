/*
  OpenParkingManager - An open source parking manager and parking finder.
    Copyright (C) 2019 Louis van der Walt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <LiquidCrystal.h>
#include <SD.h>

LiquidCrystal lcd(3, 2, 4, 5, 6, 7);

int x = 0;

File myFile;

bool demo = false;

void setup() {

  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Hello, world");
  pinMode(8, INPUT_PULLUP);
  delay(200);
  if (!digitalRead(8))
  {
    while (!digitalRead(8)) {}
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DEMO");
    delay(2000);
    demo = true;
  }
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Insert Ticket");
  while (digitalRead(8)) {}

  while (!digitalRead(8)) {}

  if (!demo)
  {
    SD.begin(10);
    myFile = SD.open("ticket.txt");
    if (myFile) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Reading...");
      while (myFile.available()) {
        Serial.write(myFile.read());
      }
      Serial.println("");
      // close the file:
      myFile.close();
      while (!Serial.available() > 0) {}
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You parked at:");
      lcd.setCursor(0, 1);
      delay(500);
      while (Serial.available() > 0)
      {
        lcd.write(Serial.read());
      }
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Failed to read");
      lcd.setCursor(0, 1);
      lcd.print("Ticket");
      delay(3000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please try again");
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reading...");
    delay(2000);
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("You parked at:");
    lcd.setCursor(0, 1);
    lcd.print("Floor 4 pos A6");
  }
  delay(5000);

}
