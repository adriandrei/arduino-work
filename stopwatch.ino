// stopwatch

#include <util/delay.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, NULL, 11, 9,8,7,6);
volatile int nr_secunde = 0;

void setup() {
   TIMSK1 = (1 << TOIE1); // activare timer overflow
   TCCR1A = 0;
   TCCR1B = 0; // timer stop
   //activare INT0, INT1, PCINT20
   EIMSK = (1 << INT0);
   EIMSK |= (1 << INT1);
   //activare PCI2 (buton 3 - PCINT20 - PCI2)
   PCICR |= (1 << PCIE2);
   PCMSK2 |= (1<<PCINT20);
   //configurare LCD
     lcd.begin(16, 2);
   lcd.noCursor();
}

void loop(){
   lcd.setCursor(0, 1);
   if (nr_secunde<10) 
       lcd.print("0");
   lcd.print(nr_secunde);
   lcd.print(" ");
}

ISR(TIMER1_OVF_vect) {
   TCNT1=0x0BDC;
   nr_secunde++;
   if (nr_secunde == 60) 
       nr_secunde = 0;
}

ISR(INT0_vect) {
   TCNT1=0x0BDC;
   TCCR1A = 0;
   TCCR1B = (1 << CS12); // timer start
   _delay_ms(400);
}

ISR(INT1_vect){
   TCCR1A = 0;
   TCCR1B = 0; // timer stop
   _delay_ms(400);
}

ISR(PCINT2_vect) {
   nr_secunde = 0; //reset cronometru
   _delay_ms(400);
}
