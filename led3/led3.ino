#include <SPI.h> // För att skicka seriedata med hårdvaran i Teensy'n.
#include <FlexiTimer2.h>  // För att skapa en interupt för att skifta ut 30 bitar data i 7 rader.

// 7 Byte, där den första är den översta pixelraden och den sista är den understa
// Varje tecken består av 5 punkter + en släckt mellanrumspunkt bit 7,6,5,4,3,2. Bit 1 och 0 används ej.
#define A { B01110000,B10001000,B10001000,B11111000,B10001000,B10001000,B10001000} //
#define B {B11110000,B10001000,B10001000,B11110000,B10001000,B10001000,B11110000} //
#define C {B01110000,B10001000,B10000000,B10000000,B10000000,B10001000,B01110000} //
#define D {B11100000,B10010000,B10001000,B10001000,B10001000,B10010000,B11100000} //
#define E {B11111000,B10000000,B10000000,B11110000,B10000000,B10000000,B11111000} //
#define F {B11111000,B10000000,B10000000,B11110000,B10000000,B10000000,B10000000} //
#define G {B01110000,B10001000,B10000000,B10111000,B10001000,B10001000,B01110000} //
#define H {B10001000,B10001000,B10001000,B11111000,B10001000,B10001000,B10001000} //
#define I {B01110000,B00100000,B00100000,B00100000,B00100000,B00100000,B01110000} //
#define J {B00111000,B00010000,B00010000,B00010000,B00010000,B10010000,B01100000} //
#define M {B10001000,B11011000,B10101000,B10101000,B10001000,B10001000,B10001000} //
#define N {B10001000,B10001000,B11001000,B10101000,B10011000,B10001000,B10001000} //
#define L {B10000000,B10000000,B10000000,B10000000,B10000000,B10000000,B11111000} //
#define O {B01110000,B10001000,B10001000,B10001000,B10001000,B10001000,B01110000} //
#define P {B11110000,B10001000,B10001000,B11110000,B10000000,B10000000,B10000000} //
#define Q {B01110000,B10001000,B10001000,B10001000,B10101000,B10010000,B01101000} //
#define R {B11110000,B10001000,B10001000,B11110000,B10100000,B10010000,B10001000} //
#define S {B01111000,B10000000,B10000000,B01110000,B00001000,B00001000,B11110000} //
#define K {B10001000,B10010000,B10100000,B11000000,B10100000,B10010000,B10001000} //
#define T {B11111000,B00100000,B00100000,B00100000,B00100000,B00100000,B00100000} //
#define U {B10001000,B10001000,B10001000,B10001000,B10001000,B10001000,B01110000} //
#define V {B10001000,B10001000,B10001000,B10001000,B10001000,B01010000,B00100000} //
#define W {B10001000,B10001000,B10001000,B10101000,B10101000,B10101000,B01010000} //
#define X {B10001000,B10001000,B01010000,B00100000,B01010000,B10001000,B10001000} //
#define Y {B10001000,B10001000,B10001000,B01010000,B00100000,B00100000,B00100000} //
#define Z {B11111000,B00001000,B00010000,B00100000,B01000000,B10000000,B11111000} //
#define LA{B00000000,B00000000,B01110000,B00001000,B01111000,B10001000,B01111000} //
#define LB{B10000000,B10000000,B10110000,B11001000,B10001000,B10001000,B11110000} //
#define LC{B00000000,B00000000,B01110000,B10000000,B10000000,B10001000,B01110000} //
#define LD{B00001000,B00001000,B01101000,B10011000,B10001000,B10001000,B01111000} //
#define LE{B00000000,B00000000,B01110000,B10001000,B11111000,B10000000,B01110000} //
#define LF{B00110000,B01001000,B01000000,B11100000,B01000000,B01000000,B01000000} //
#define LG{B00000000,B01111000,B10001000,B10001000,B01111000,B00001000,B01110000} //
#define LH{B10000000,B10000000,B10110000,B11001000,B10001000,B10001000,B10001000} //
#define LI{B01000000,B00000000,B11000000,B01000000,B01000000,B01000000,B11100000} //
#define LJ{B00010000,B00000000,B00110000,B00010000,B00010000,B10010000,B01100000} //
#define LK{B10000000,B10000000,B10010000,B10100000,B11000000,B10100000,B10010000} //
#define LL{B01100000,B00100000,B00100000,B00100000,B00100000,B00100000,B01110000} //
#define LM{B00000000,B00000000,B11010000,B10101000,B10101000,B10001000,B10001000} //
#define LN{B00000000,B00000000,B10110000,B11001000,B10001000,B10001000,B10001000} //
#define LO{B00000000,B00000000,B01110000,B10001000,B10001000,B10001000,B01110000} //
#define LP{B00000000,B00000000,B11110000,B10001000,B11110000,B10000000,B10000000} //
#define LQ{B00000000,B00000000,B01101000,B10011000,B01111000,B00001000,B00001000} //
#define LR{B00000000,B00000000,B10110000,B11001000,B10000000,B10000000,B10000000} //
#define LS{B00000000,B00000000,B01110000,B10000000,B01110000,B00001000,B11110000} //
#define LT{B01000000,B01000000,B11100000,B01000000,B01000000,B01001000,B00110000} //
#define LU{B00000000,B00000000,B10001000,B10001000,B10001000,B10011000,B01101000} //
#define LV{B00000000,B00000000,B10001000,B10001000,B10001000,B01010000,B00100000} //
#define LW{B00000000,B00000000,B10001000,B10001000,B10101000,B10101000,B01010000} //
#define LX{B00000000,B00000000,B10001000,B01010000,B00100000,B01010000,B10001000} //
#define LY{B00000000,B00000000,B10001000,B10001000,B01111000,B00001000,B01110000} //
#define LZ{B00000000,B00000000,B11111000,B00010000,B00100000,B01000000,B11111000} //
#define SPACE{B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000} //
#define NUM0{B01110000,B10001000,B10011000,B10101000,B11001000,B10001000,B01110000} //
#define NUM1{B00100000,B01100000,B00100000,B00100000,B00100000,B00100000,B01110000} //
#define NUM2{B01110000,B10001000,B00001000,B00010000,B00100000,B01000000,B11111000} //
#define NUM3{B01110000,B10001000,B00001000,B00110000,B00001000,B10001000,B01110000} //
#define NUM4{B00010000,B00110000,B01010000,B10010000,B11111000,B00010000,B00010000} //
#define NUM5{B11111000,B10000000,B01110000,B00001000,B00001000,B10001000,B01110000} //
#define NUM6{B00110000,B01000000,B10000000,B11110000,B10001000,B10001000,B01110000} //
#define NUM7{B11111000,B00001000,B00010000,B00100000,B01000000,B01000000,B01000000} //
#define NUM8{B01110000,B10001000,B10001000,B01110000,B10001000,B10001000,B01110000} //
#define NUM9{B01110000,B10001000,B10001000,B01111000,B00001000,B00010000,B01100000} //
#define DIVIDE{B00000000,B00001000,B00010000,B00100000,B01000000,B10000000,B00000000} //
#define KOLON{B00000000,B01100000,B01100000,B00000000,B00000000,B01100000,B01100000}
#define PUNKT{B00000000,B00000000,B00000000,B00000000,B00000000,B01100000,B01100000} //
#define KOMMA{B00000000,B00000000,B00000000,B00000000,B01100000,B00100000,B01000000} //
#define MINUS{B00000000,B00000000,B00000000,B11111000,B00000000,B00000000,B00000000} //
#define QUESTION{B01110000,B10001000,B00001000,B00010000,B00100000,B00000000,B00100000} //
#define UTROP{B00100000,B01110000,B01110000,B01110000,B00100000,B00000000,B00100000} //
#define PLUS{B00000000,B00100000,B00100000,B11111000,B00100000,B00100000,B00000000} //
#define LIKAMED{B00000000,B00000000,B11111000,B00000000,B11111000,B00000000,B00000000} //
#define MINDRE{B00010000,B00100000,B01000000,B10000000,B01000000,B00100000,B00010000} //
#define STORRE{B01000000,B00100000,B00010000,B00001000,B00010000,B00100000,B01000000} //
#define UNDERSCORE{B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11111000} //

#define LAA{B00100000,B00000000,B01110000,B00001000,B01111000,B10001000,B01111000} //
#define LAE{B01010000,B00000000,B01110000,B00001000,B01111000,B10001000,B01111000} //
#define LOE{B01010000,B00000000,B01110000,B10001000,B10001000,B10001000,B01110000} //
#define AA{B00100000,B00000000,B01110000,B10001000,B11111000,B10001000,B10001000} //
#define AE{B01010000,B00000000,B01110000,B10001000,B11111000,B10001000,B10001000} //
#define OE{B01010000,B00000000,B01110000,B10001000,B10001000,B10001000,B01110000} //

// Hårdvara:
// Teensy Pin 14 till LED-display _MR pin 3
// Teensy Pin 15-21 till LED-display Rad 0-7 pin 2,4,6,8,10,12,14
// Teensy Pin 1 till LED-display CLOCK pin 1
// Teensy Pin 2 till LEd-display I/O pin 11
// Teense Pin GND till LED-display GND pin 13

// Arduino Pin 2 till LED-display _MR pin 3
// Arduino Pin 3-9 till LED-display Rad 0-6 pin 2,4,6,8,10,12,14
// Arduino Pin 11 (MOSI)till LED-display CLOCK pin 1
// Arduino Pin 13 (SCL) till LEd-display I/O pin 11
// Arduino Pin GND till LED-display GND pin 13

// LED-display 5,7,9 används ej.
const byte length_of_text_string = 7; // just det.
const byte mrPin = 2;  // Master reset pinne, raderar skiftregistret om 30 bitar i displayen.
//const byte clockPin = 1; (Hårdvaru-kodade)
//const byte dataPin = 2;  (Hårdvaru-kodade)
byte pinscan[] = {3, 4, 5, 6, 7, 8, 9}; // Håller reda på vilken pinne som är vilken rad 0,1,2,3,4,5,6 i interuptrutinen.
byte row_mem = 0; // används i interruptrutinen för att komma ihåg vilken rad vi e på.

// Serieporten:
char incomingByte[length_of_text_string];// Inkommande tecken från serieporten
byte antal_tecken = 0; // Antal tecken hämtade från serial in.

// Displayen:
byte display_matrix[7][4];// DISPLAYBUFFERTEN !!!
unsigned long long_row[7];
unsigned long LEDtecken[length_of_text_string][7];

// chr_array definerar alla tecken som bitmap
byte chr_array[81][7] = {A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, \
                         SPACE, NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, DIVIDE, KOLON, PUNKT, \
                         KOMMA, MINUS, QUESTION, UTROP, LA, LB, LC, LD, LE, LF, LG, LH, LI, LJ, LK, LL, LM, LN, LO, LP, \
                         LQ, LR, LS, LT, LU, LV, LW, LX, LY, LZ, LAA, LAE, LOE, AA, AE, OE, PLUS, LIKAMED, MINDRE, STORRE, UNDERSCORE
                        };

void setup() {

  Serial.begin(9600);  // Debug

  SPI.begin();        // Sätter upp hårdvaru-serie-data till displayen
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(0); // 0 = Mode för nivå och flank på clock/data
  SPI.setClockDivider(SPI_CLOCK_DIV16); // Hastigheten för clock-data

  pinMode(mrPin, OUTPUT);  // Reset-pinne är en utgång
  digitalWrite(mrPin, 0);  // Sätter MR pin låg = reset

  for (byte i = 0; i < 7; i++) pinMode(pinscan[i], OUTPUT); // Definerar alla rad-pinnarna som utgångar
  for (byte i = 0; i < 7; i++) digitalWrite(pinscan[i], 0); // Sätter alla rad-pinnar till 0, stänger av alla LED-raderna

  // Här är det viktiga:
  FlexiTimer2::set(2, 1.0 / 1000, LEDflash); // Sätter interrupt till var 2:e ms och kör då rutinen LEDflash
  FlexiTimer2::start(); // Startar interrupten

  Serial.println("LED-Display V0.7");
}

void loop() {
  if (Serial.available()) {
    antal_tecken = Serial.readBytesUntil('\10', incomingByte, 7);
    Serial.flush();
  }
  // Fyller bufferten patterns[][] med data för textsträngen
  visa_text(antal_tecken);
}

// Fyller arrayen LEDtecken med bitmönster för varje tecken som ska displayas
void visa_text(int antal) {

  byte tecken;
  byte stecken;

  for (int tecken = 0; tecken < antal; tecken++) {
    if ((incomingByte[tecken] > 64 && incomingByte[tecken] < 91)
        || (incomingByte[tecken] > 96 && incomingByte[tecken] < 123)) {
      if (incomingByte[tecken] > 64 && incomingByte[tecken] < 91) { // Stora bokstäver
        for (int row = 0; row < 7; row++) {
          LEDtecken[tecken][row] = chr_array[incomingByte[tecken] - 65][row]; // 65=Ascii-Offset för "A"
        }
      }
      else {  // Små bokstäver
        for (int row = 0; row < 7; row++) {
          LEDtecken[tecken][row] = chr_array[incomingByte[tecken] - 53][row]; // 53=Ascii-Offset för "a"
        }
      }

    }
    else { // ...övriga tecken
      switch (incomingByte[tecken]) {
        case  32:        stecken = 26;        break; //Mellanslag
        case  33:        stecken = 43;        break; //Utropstecken
        case  43:        stecken = 76;        break; // Plus
        case  44:        stecken = 40;        break; // Komma
        case  45:        stecken = 41;        break; // Minus
        case  46:        stecken = 39;        break; // Punkt
        case  47:        stecken = 37;        break; // Divide
        case  48:        stecken = 27;        break; // 0
        case  49:        stecken = 28;        break; // 1
        case  50:        stecken = 29;        break; // 2
        case  51:        stecken = 30;        break; // 3
        case  52:        stecken = 31;        break; // 4
        case  53:        stecken = 32;        break; // 5
        case  54:        stecken = 33;        break; // 6
        case  55:        stecken = 34;        break; // 7
        case  56:        stecken = 35;        break; // 8
        case  57:        stecken = 36;        break; // 9
        case  58:        stecken = 38;        break; // Kolon
        case  60:        stecken = 78;        break; // Mindre än
        case  61:        stecken = 77;        break; // Lika med
        case  62:        stecken = 79;        break; // Större än
        case  63:        stecken = 42;        break; // Frågetecken
        case  95:        stecken = 80;        break; // Underscore
        case 196:        stecken = 74;        break; // Ä
        case 197:        stecken = 73;        break; // Å
        case 214:        stecken = 75;        break; // Ö
        case 228:        stecken = 71;        break; // ä
        case 229:        stecken = 70;        break; // å
        case 246:        stecken = 72;        break; // ö
        default:        stecken = 26;        break; // Allt annat blir mellanslag
      }

      for (int row = 0; row < 7; row++) {
        LEDtecken[tecken][row] = chr_array[stecken][row];
      }
    }
  }

  for (int r = 0; r < 7; r++) {     // Vi har sju rader
    long_row[r] = ((LEDtecken[0][r] << 22) + (LEDtecken[1][r] << 16) + (LEDtecken[2][r] << 10) + (LEDtecken[3][r] << 4) + (LEDtecken[4][r] >> 2));
  }
}

void LEDflash() {
  // interruptrutin. row_mem måste finnas def. utanför interrupten, den håller reda på vilken rad som just nu lyser.
  // denna rutin repeteras var tredje ms, vilket innebär att varje rad lyser i 2ms.
  // (har provat och sätta 3ms, men då flimrar displayen, ger för låg frame-rate)
  // display_matrix är en 7 x 8 byte matris som innehåller det som för närvarande ska visas på LED-displayen
  
  byte row_prev = row_mem;
  if (row_mem == 0) row_prev = 6;
  else row_prev--;
  digitalWrite(pinscan[row_prev], 0);
  
  // Gör reset på 32-bitarsbufferten
  digitalWrite(mrPin, 0);
  digitalWrite(mrPin, 1);
  
  // Kör ut 8 bitar * 4 = 32 bitar data för en rad, row_mem
  SPI.transfer(byte(long_row[row_mem] >> 24));
  SPI.transfer(byte(long_row[row_mem] >> 16));
  SPI.transfer(byte(long_row[row_mem] >> 8));
  SPI.transfer(byte(long_row[row_mem] >> 0));
  
  // Lys upp en av de 7 rader (row_mem)
  digitalWrite(pinscan[row_mem], 1);
  
  // Förbered inför nästa rad när vi återvänder hit nästa interrupt
  row_mem++;
  if (row_mem == 7) row_mem = 0;
}
