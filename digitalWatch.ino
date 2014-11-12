#include <util/delay.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, NULL, 11, 9,8,7,6);
volatile int secunda = 0;
volatile int minut = 0;
volatile int ora = 0;
volatile int zi = 1;
volatile int luna = 1;
volatile int an = 2013;
volatile int zile_luna [] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31,
30, 31};
const int backlight = 13;
volatile boolean backlight_on;
volatile byte set_mode;

void setup() {
   TIMSK1 = (1 << TOIE1); // activare timer overflow
   TCNT1=0x0BDC;
   TCCR1A = 0;
   TCCR1B = (1 << CS12); // timer start
   //activare int0 si int1
   EIMSK |= (1 << INT0);
   EIMSK |= (1 << INT1);
   //activare PCI2 (buton 3 - PCINT20 - PCI2)
   PCICR |= (1 << PCIE2);
   PCMSK2 |= (1<<PCINT20);
   //la pornire lumina de fundal a afisajului este oprita
   pinMode(backlight, OUTPUT);
   digitalWrite(backlight, LOW);
   backlight_on = false;
   //setare mod functionare normala
   set_mode = 0;
   //configurare LCD
   lcd.begin(16, 2);
   lcd.noCursor();
}

void loop(){
   lcd.setCursor(0, 0);
   if (zi<10) 
       lcd.print("0");
   lcd.print(zi);
   lcd.setCursor(2,0);
   lcd.print("/");
   if (luna<10) 
       lcd.print("0");
   lcd.print(luna);
   lcd.setCursor(5,0);
   lcd.print("/");
   lcd.print(an);
   lcd.setCursor(0, 1);
   if (ora<10) 
       lcd.print("0");
   lcd.print(ora);
   lcd.setCursor(2,1);
   lcd.print(":");
   if (minut<10) 
       lcd.print("0");
   lcd.print(minut);
   lcd.setCursor(5,1);
   lcd.print(":");
   if (secunda<10)
       lcd.print("0");
   lcd.print(secunda);
   lcd.print(" ");
 
   if (set_mode!=0)
   {
       lcd.setCursor(11, 1);
       lcd.print("SET ");
       switch (set_mode) {
           case 1:
               lcd.print("Z");
               break;
           case 2:
               lcd.print("L");
               break;
           case 3:
               lcd.print("A");
               break;
           case 4:
               lcd.print("O");
               break;
           case 5:
               lcd.print("M");
               break;
           case 6:
               lcd.print("S");
               break;
       }
   }
   delay(500);
   lcd.clear();
}

ISR(TIMER1_OVF_vect) {
   TCNT1=0x0BDC;
   secunda++;
   if (secunda == 60)
   { 
       secunda = 0;
       minut++;
       if (minut>=60)
       {
           minut = 0;
           ora ++;
           if (ora >= 24)
           {
               ora = 0;
               zi++;
           if (zi > zile_luna[luna-1])
           {
               zi = 1;
               luna++;
               if (luna > 12)
               {
                   luna = 1;
                   an++;
                   if (an%4 == 0) 
                       zile_luna[1] = 29;
                   else 
                       zile_luna[1] = 28;
               }
           }
       }
     }
   }
}

ISR(INT0_vect) {
   set_mode++;
   if (set_mode==7) 
       set_mode=0;
   _delay_ms(400);
}
 
ISR(INT1_vect){  
    if (set_mode!=0)
       switch (set_mode) {
           case 1:
               zi++;
               if (zi==zile_luna[luna-1])
                   zi=1;
               break;
           case 2:
               luna++;
               if (luna==12) 
                   luna=1;
               if (zi>zile_luna[luna-1]) 
                   zi=zile_luna[luna-1];
               break;
           case 3:
               an++;
               if (an==2020) 
                   an=2013;
               if (an%4 == 0)
                   zile_luna[1] = 29;
               else 
                   zile_luna[1] = 28;
               if (zi>zile_luna[luna-1]) 
                   zi=zile_luna[luna-1];
               break;
           case 4:
               ora++;
               if (ora==24) 
                   ora=0;
               break;
           case 5:
               minut++;
               if (minut==60) 
                   minut=0;
               break;
           case 6:
               secunda++;
               if (secunda==60) 
                   secunda=0;
               break;
           }
     _delay_ms(400);
}
 
ISR(PCINT2_vect) {
    _delay_ms(20);
    if (!(PIND & 0x10))
    {
       if (backlight_on) {
           digitalWrite(backlight, LOW);
           backlight_on = false;
         }
       else {
           digitalWrite(backlight, HIGH);
           backlight_on = true;
       }
    } 
}
