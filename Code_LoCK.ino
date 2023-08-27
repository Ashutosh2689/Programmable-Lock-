#include <Wire.h>        
#include <RTClib.h> 
#include <LiquidCrystal_I2C.h>     
#include <Keypad.h> 
#include<SPI.h>
#include<SD.h>     
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;           
const byte ROWS = 4;      
const byte COLS = 4; 
const int chipSelect = 8;    
const int RELAY_PIN = A0;
int data[5];

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 10, 7, 6};     
byte colPins[COLS] = {5, 4, 3, 2};     

 Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

void setup() {
   pinMode(RELAY_PIN, OUTPUT); 
   pinMode(chipSelect, OUTPUT);
   Serial.begin(9600);
   while (!Serial) {
    ; 
  }
 if (!SD.begin(chipSelect)) {
    return;
  }
  lcd.init(); 
  lcd.backlight();     
  Wire.begin();            
  rtc.begin();  
}

void readFile() {
   
  File myFile = SD.open("data.txt", FILE_READ);

  if (myFile) {
    lcd.print("CRR SHD:");
    delay(1000);
    
    int i = 0;
    while (myFile.available() && i < 5) {
      data[i] = myFile.parseInt();
      i++;
      myFile.read(); 
    }
    myFile.close();
    
    lcd.setCursor(0, 1);    
    for (int j = 0; j < 5; j++) {
      lcd.print(data[j]);
      if(j<4){lcd.print(",");
      }
    }
    delay(1000);
  } 
  else {
    lcd.print("Error in data.txt");
  }
  delay(1000);
  lcd.clear(); 
}
 void loop() {
  digitalWrite(RELAY_PIN, HIGH);
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);   
  lcd.print("Cr DT: ");
  lcd.print(now.year(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.day(), DEC);
  lcd.print(' ');
  lcd.setCursor(0, 1);
  lcd.print("Cr TIM: ");
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.println(now.second(), DEC);
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);   
  lcd.print("Press A to SET");
  char key = keypad.getKey();
  lcd.setCursor(0, 1);  
  lcd.print("Press B to RESET");
  delay(2000); 
  lcd.clear(); 
  String str = "";
  int newDate[5];

  if (key != NO_KEY) {  
    lcd.setCursor(0, 0);       
    lcd.println(key);
    delay(3000);

    if (key == 'A') {    
      lcd.clear();
      lcd.setCursor(0, 0);     
     char x ;

      for (int i = 0; i < 4; i++) {
        lcd.print("Enter year:");
        lcd.setCursor(i+1, 1);
       char x = keypad.getKey();        
        while (!x) {
       x = keypad.getKey();
       }      
         lcd.print(x); 
         delay(1000);
          lcd.clear();         
         str = str + x;  
      }
      newDate[0] = str.toInt();   
       lcd.println(newDate[0]);
       delay(1000);
       lcd.clear();
       str = "";
      for (int i = 0; i < 2; i++) {
        lcd.print("Enter month:");
        lcd.setCursor(i+1, 1);
         char x = keypad.getKey();        
        while (!x) {
       x = keypad.getKey();
       }
        lcd.print(x);  
         delay(1000);
         lcd.clear();        
       str = str + x; 
      }
      newDate[1] = str.toInt();   
       lcd.println(newDate[1]);
       delay(1000);
       lcd.clear();
       str = "";
      for (int i = 0; i < 2; i++) {
         lcd.print("Enter day:");
         lcd.setCursor(i+1, 1);
         char x = keypad.getKey();        
        while (!x) {
       x = keypad.getKey();
       }
          lcd.print(x); 
          delay(1000);
          lcd.clear();          
         str = str + x;  
      }
      newDate[2] = str.toInt(); 
      lcd.println(newDate[2]);  
       delay(1000);
       lcd.clear();
       str = "";
      for (int i = 0; i < 2; i++) {
         lcd.print("Enter Hour:");
         lcd.setCursor(i+1, 1);
         char x = keypad.getKey();        
        while (!x) {
       x = keypad.getKey();
       }
          lcd.print(x); 
           delay(1000);
          lcd.clear();
        str = str + x;  
      }
      newDate[3] = str.toInt();
       lcd.println(newDate[3]);
       delay(1000);
       lcd.clear();
       str = "";
       for (int i = 0; i < 2; i++) {
       lcd.print("Enter minute:"); 
       lcd.setCursor(i+1, 1);        
         char x = keypad.getKey();        
        while (!x) {
       x = keypad.getKey();
       }
          lcd.print(x); 
          delay(1000);
          lcd.clear();
        str = str + x;  
      }
      newDate[4] = str.toInt();   
       lcd.println(newDate[4]);
       delay(1000);
       lcd.clear();
        File dataFile = SD.open("data.txt", FILE_WRITE);
        if (dataFile) {
              lcd.print("Writing data to file...");
              delay(1000);
              dataFile.println();
           for (int i = 0; i <5; i++) {
            dataFile.print(newDate[i]);
            if(i<4){ dataFile.print(",");
            }
          }
            dataFile.close();
            lcd.println("Data logged to file.");
            delay(1000);
            return;
       } 
       else {
         lcd.println("Error data.txt");
       }
       delay(1000);
       lcd.clear(); 
    } 
    if (key == 'B'){
       
     delay(1000); 
    lcd.print("Schd is RESET");
    delay(2000);  
    }
  }  
     readFile();
   if(now.year() == data[0] && now.month() == data[1] && now.day() == data[2]&& now.hour() == data[3] && now.minute()== data[4] && now.second() >= 0 && now.second() <= 10){
    digitalWrite(RELAY_PIN, LOW);
    lcd.print("Lock is opening..");
    delay(5000);
    digitalWrite(RELAY_PIN, HIGH);
    lcd.clear();
    lcd.print("Lock is opened");  
    delay(2000);
   }
}