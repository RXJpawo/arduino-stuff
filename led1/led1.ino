#include <SPI.h>
#include <FlexiTimer2.h>

// Hårdvara:
// Teensy Pin 14 till LED-display _MR pin 3
// Teensy Pin 15-21 till LED-display Rad 0-6 pin 2,4,6,8,10,12,14
// Teensy Pin 1 till LED-display CLOCK pin 1
// Teensy Pin 2 till LEd-display I/O pin 11
// Teense Pin GND till LED-display GND pin 13

// Arduino Pin 2 till LED-display _MR pin 3
// Arduino Pin 3-9 till LED-display Rad 0-6 pin 2,4,6,8,10,12,14
// Arduino Pin 11 (MOSI)till LED-display CLOCK pin 1
// Arduino Pin 13 (SCL) till LEd-display I/O pin 11
// Arduino Pin GND till LED-display GND pin 13

// LED-display pin 5,7,9 används ej.

const int mrPin = 2;

byte pinscan[] = {9,8,7,6,5,4,3}; // Håller reda på vilken pinne som är vilken rad 0,1,2,3,4,5,6.
byte row_mem = 0; // används i interruptrutinen för att komma ihåg vilken rad vi e på.
byte LEDdata1[7]; byte LEDdata2[7]; byte LEDdata3[7]; byte LEDdata4[7]; // 7 raders display i interruprtutinen

byte dataArray1[20]; // en "bitmapbild" av vad som ska animeras på led.
byte dataArray2[20]; // 1-4 är bittarna 0-32 (de två msb i dataAarray1 är inte använda då displayen endast har 30 bitar
byte dataArray3[20];
byte dataArray4[20];


void setup() {

   //Serial.begin(9600);  // Debug

   SPI.begin();        // Sätter upp hårdvaru-serie-data till displayen
   SPI.setBitOrder(MSBFIRST);
   SPI.setDataMode(0); // 0 = Mode för nivå och flank på clock/data
   SPI.setClockDivider(SPI_CLOCK_DIV16); // Hastigheten för clock-data

   pinMode(mrPin, OUTPUT);  // Reset-pinne är en utgång
   digitalWrite(mrPin, 0);  // Sätter MR pin låg = reset

   for (byte i=0; i<7 ;i++) {
     pinMode(pinscan[i], OUTPUT);  // Definerar alla rad-pinnarna som utgångar
   }

   for(byte i=0;i<7;i++) {
     digitalWrite(pinscan[i], 0);  // Sätter alla rad-pinnar till 0, stänger av alla LED-raderna
   }

// Testdata för att animera displayen, består av 20 olika bilder (alla raderna får samma utseende)
   dataArray1[0] = B00111111;
   dataArray2[0] = B11111111;
   dataArray3[0] = B11111111;
   dataArray4[0] = B11111111;
   
   dataArray1[1] = B00001111;
   dataArray2[1] = B11111111;
   dataArray3[1] = B11111111;
   dataArray4[1] = B11111100;
   
   dataArray1[2] = B00000001;
   dataArray2[2] = B11111111;
   dataArray3[2] = B11111111;
   dataArray4[2] = B11110000;
   
   dataArray1[3] = B00000000;
   dataArray2[3] = B11111111;
   dataArray3[3] = B11111111;
   dataArray4[3] = B11000000;
   
   dataArray1[4] = B00000000;
   dataArray2[4] = B00111111;
   dataArray3[4] = B11111111;
   dataArray4[4] = B00000000;
   
   dataArray1[5] = B00000000;
   dataArray2[5] = B00001111;
   dataArray3[5] = B11111100;
   dataArray4[5] = B00000000;
   
   dataArray1[6] = B00000000;
   dataArray2[6] = B00000011;
   dataArray3[6] = B11110000;
   dataArray4[6] = B00000000;
   
   dataArray1[7] = B00000000;
   dataArray2[7] = B00000001;
   dataArray3[7] = B11100000;
   dataArray4[7] = B00000000;
   
   dataArray1[8] = B00000000;
   dataArray2[8] = B00000000;
   dataArray3[8] = B11000000;
   dataArray4[8] = B00000000;
   
   dataArray1[9] = B00000000;
   dataArray2[9] = B00000000;
   dataArray3[9] = B00000000;
   dataArray4[9] = B00000000;
   
   dataArray1[10] = B00000000;
   dataArray2[10] = B00000000;
   dataArray3[10] = B11000000;
   dataArray4[10] = B00000000;
   
   dataArray1[11] = B00000000;
   dataArray2[11] = B00000011;
   dataArray3[11] = B11110000;
   dataArray4[11] = B00000000;
   
   dataArray1[12] = B00000000;
   dataArray2[12] = B00011111;
   dataArray3[12] = B11111100;
   dataArray4[12] = B00000000;
   
   dataArray1[13] = B00000000;
   dataArray2[13] = B01111111;
   dataArray3[13] = B11111111;
   dataArray4[13] = B00000000;
   
   dataArray1[14] = B00000001;
   dataArray2[14] = B11111111;
   dataArray3[14] = B11111111;
   dataArray4[14] = B11000000;
   
   dataArray1[15] = B00000111;
   dataArray2[15] = B11111111;
   dataArray3[15] = B11111111;
   dataArray4[15] = B11110000;
   
   dataArray1[16] = B00011111;
   dataArray2[16] = B11111111;
   dataArray3[16] = B11111111;
   dataArray4[16] = B11111100;

// Här är det viktiga:
   FlexiTimer2::set(2, 1.0/1000, LEDflash); // Sätter interrupt till var 3:e ms och kör då rutinen LEDflash
   FlexiTimer2::start(); // Startar interrupten
}

void loop() {

   // Animerar frame 0 till 16
   for (byte j=0; j<17; j++) {
    
     // Ville egentligen rita en springande gubbe, men...
     // Sätter alla LED-rader till samma värde, det blev enklast så.
     
     LEDdata1[0] = dataArray1[j]; LEDdata2[0] = dataArray2[j];
     LEDdata3[0] = dataArray3[j]; LEDdata4[0] = dataArray4[j];
     
     LEDdata1[1] = dataArray1[j]; LEDdata2[1] = dataArray2[j];
     LEDdata3[1] = dataArray3[j]; LEDdata4[1] = dataArray4[j];
     
     LEDdata1[2] = dataArray1[j]; LEDdata2[2] = dataArray2[j];
     LEDdata3[2] = dataArray3[j]; LEDdata4[2] = dataArray4[j];
     
     LEDdata1[3] = dataArray1[j]; LEDdata2[3] = dataArray2[j];
     LEDdata3[3] = dataArray3[j]; LEDdata4[3] = dataArray4[j];
     
     LEDdata1[4] = dataArray1[j]; LEDdata2[4] = dataArray2[j];
     LEDdata3[4] = dataArray3[j]; LEDdata4[4] = dataArray4[j];
     
     LEDdata1[5] = dataArray1[j]; LEDdata2[5] = dataArray2[j];
     LEDdata3[5] = dataArray3[j]; LEDdata4[5] = dataArray4[j];
     
     LEDdata1[6] = dataArray1[j]; LEDdata2[6] = dataArray2[j];
     LEDdata3[6] = dataArray3[j]; LEDdata4[6] = dataArray4[j];
     
     delay(75); // Bestämmer hastigheten mellan varje hel frame
   }
}

void LEDflash()
// interruptrutin. row_mem måste finnas def. utanför interrupten, den håller reda på vilken rad som just nu lyser.
// denna rutin repeteras var tredje ms, vilket innebär att varje rad lyser i 3ms.
// (har provat och sätta 4ms, men då flimrar displayen, ger för låg frame-rate)
{
   //Släck alla raderna (vi behöver egentligen barar släcka den rad som lyser, fast det går kanske
   // fortare att släcka alla på en gång...?
   digitalWrite(pinscan[0], 0);
   digitalWrite(pinscan[1], 0);
   digitalWrite(pinscan[2], 0);
   digitalWrite(pinscan[3], 0);
   digitalWrite(pinscan[4], 0);
   digitalWrite(pinscan[5], 0);
   digitalWrite(pinscan[6], 0);
   // Gör reset på 32-bitarsbufferten
   digitalWrite(mrPin, 0);
   digitalWrite(mrPin, 1);
   // Kör ut 8 bitar * 4 = 32 bitar data för en rad, row_mem
   SPI.transfer(LEDdata1[row_mem]);
   SPI.transfer(LEDdata2[row_mem]);
   SPI.transfer(LEDdata3[row_mem]);
   SPI.transfer(LEDdata4[row_mem]);
   // Lys upp en av de 7 rader (row_mem)
   digitalWrite(pinscan[row_mem], 1);
   // Förbered inför nästa rad när vi återvänder hit efter 3ms.
   row_mem++;
   if (row_mem == 7) row_mem = 0;
}
