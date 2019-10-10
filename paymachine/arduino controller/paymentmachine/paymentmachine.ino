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

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <SD.h>

LiquidCrystal lcd(3, 2, 4, 5, 6, 7);

File myFile;

SoftwareSerial wifi_board(A4, A5);

void send_bytes(const char* bytes)
{
  wifi_board.println(bytes);
}

void parse_message(const char* bytes)
{
  switch(bytes[0])
  {
    case 'p':
    
    break;
    
  };
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.print("Hello, world");

  wifi_board.begin(38400);
  while(!wifi_board)
  {
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0 ,1);
  lcd.print(millis() / 1000); 
  if (wifi_board.available() > 0)
  {
    char buffer[64];
    wifi_board.readBytesUntil('\n', &buffer, 64);
    parse_message();
  }
  
}
