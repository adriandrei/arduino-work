#include <LiquidCrystal.h>

LiquidCrystal lcd(12, NULL, 11, 9,8,7,6);
const int voltmetrupin = 0;
volatile int val=0; // valoare achizitionata
// tensiunea citita vout = (val * 5.0) / 1024.0;
volatile float vout = 0.0;
const float R1 = 220.0; // valori rezistente divizor tensiune
const float R2 = 220.0;
// tensiune baterie vin = vout / (R2/(R1+R2))
volatile float vin = 0.0;
// valoarea maxima a tensiunii bateriei
volatile float initialvin = 0.0;
//intesitate curent in mA i = (vin * 1000) / (R1+R2)
volatile float i = 0.0;
//procent dupa care consideram ca bateria e descarcata 60%
const float lb = 0.6;
volatile float sumvin = 0.0; // suma tensiunilor bateriei
// capacitate consumata cap = (sumvin * 10) / (R1+R2)
volatile float cap = 0.0;
volatile int sec = 0;
volatile float timp = 0; // timp in minute (sec/60)

byte lowbat[8] = { // caracter pentru baterie descarcata
 B01110,
 B11111,
 B10001,
 B10001,
 B10001,
 B10001,
 B11111,
};

void setup() {
 TIMSK1 = (1 << TOIE1); // activare timer overflow
 TCCR1A = 0;
 TCCR1B = 0; // timer stop
 TCNT1 = 0x0BDC; // setarea valorii initiale
 TCCR1B = (1 << CS12); // timer start
 lcd.createChar(0, lowbat);
 lcd.begin(16, 2);
 lcd.noCursor();
}

void loop(){
 if ((val<10)&(sumvin==0)) 
 {
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Conectati");
     lcd.setCursor(0,1);
     lcd.print("bateria...");
 }
 else 
 {
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print(vin);
     lcd.setCursor(4,0);
     lcd.print("V");
     lcd.setCursor(6,0);
     i = (vin * 1000) / (R1+R2);
     lcd.print(i);
     lcd.setCursor(10,0);
     lcd.print("mA");
     lcd.setCursor(15,0);
     if ((vin/initialvin)<lb) 
         lcd.write((byte)0);
     lcd.setCursor(0,1);
     cap = (sumvin * 10.0) / (R1+R2);
     lcd.print(cap);
     lcd.setCursor(5,1);
     lcd.print("AH");
     lcd.setCursor(8,1);
     lcd.print("/");
     lcd.setCursor(10,1);
     timp = float(sec) / 60.0;
     lcd.print(timp);
 }
 delay(1000);
}

ISR(TIMER1_OVF_vect) {
 TCNT1=0x0BDC;
 sec++;
 val = analogRead(voltmetrupin);
 vout = (val * 5.0) / 1024.0;
 vin = vout / (R2/(R1+R2));
 if (initialvin<vin) 
     initialvin = vin;
 if (!(sec%10)) 
     sumvin+=vin; 
}





 
