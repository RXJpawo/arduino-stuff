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
const byte length_of_text_string = 20; // just det.
const byte mrPin = 2;  // Master reset pinne, raderar skiftregistret om 30 bitar i displayen.
//const byte clockPin = 1; (Hårdvaru-kodade)
//const byte dataPin = 2;  (Hårdvaru-kodade)
byte pinscan[] = {3, 4, 5, 6, 7, 8, 9}; // Håller reda på vilken pinne som är vilken rad 0,1,2,3,4,5,6 i interuptrutinen.
byte row_mem = 0; // används i interruptrutinen för att komma ihåg vilken rad vi e på.

// Serieporten:
int incomingByte[length_of_text_string];// Inkommande tecken från serieporten
int antal_tecken = 0; // Antal tecken hämtade från serial in.

// Displayen:
byte display_matrix[7][4];// DISPLAYBUFFERTEN !!!
byte patterns[length_of_text_string][7];

// chr_array definerar alla tecken som bitmap
byte chr_array[81][7] ={A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,\
SPACE,NUM0,NUM1,NUM2,NUM3,NUM4,NUM5,NUM6,NUM7,NUM8,NUM9,DIVIDE,KOLON,PUNKT,\
KOMMA,MINUS,QUESTION,UTROP,LA,LB,LC,LD,LE,LF,LG,LH,LI,LJ,LK,LL,LM,LN,LO,LP,\
LQ,LR,LS,LT,LU,LV,LW,LX,LY,LZ,LAA,LAE,LOE,AA,AE,OE,PLUS,LIKAMED,MINDRE,STORRE,UNDERSCORE};

void setup() {

  Serial.begin(9600);  // Debug

  SPI.begin();        // Sätter upp hårdvaru-serie-data till displayen
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(0); // 0 = Mode för nivå och flank på clock/data
  SPI.setClockDivider(SPI_CLOCK_DIV16); // Hastigheten för clock-data

  pinMode(mrPin, OUTPUT);  // Reset-pinne är en utgång
  digitalWrite(mrPin, 0);  // Sätter MR pin låg = reset

  for (byte i=0; i<7; i++) pinMode(pinscan[i], OUTPUT);  // Definerar alla rad-pinnarna som utgångar
  for (byte i=0; i<7; i++) digitalWrite(pinscan[i], 0);  // Sätter alla rad-pinnar till 0, stänger av alla LED-raderna

  // Här är det viktiga:
  FlexiTimer2::set(2, 1.0/1000, LEDflash); // Sätter interrupt till var 2:e ms och kör då rutinen LEDflash
  FlexiTimer2::start(); // Startar interrupten

  Serial.println("LED-Display V0.7");
}

void loop() {
  // Väntar på att vi har en rad med txt från serieporten
  if(Serial.available() > 0){
    incomingByte[antal_tecken] = Serial.read();// Hämta tecknen och lägg i en array.
    antal_tecken++;// Räkna antalet tecken i buffert
  }
  else {
    // Finns det texken i bufferten är det nog dags att scrolla dessa på displayen...
    if(antal_tecken != 0){ 
    
      // Debug
      for (int i=0; i<antal_tecken; i++)
        Serial.print(incomingByte[i]); 
    
      Serial.println();
      // Fyller bufferten patterns[][] med data för textsträngen
      fyll_teckenbufferten(antal_tecken);
      // Visar bufferten patterns[][] 5 ggr med antalet tecken i bufferten och hastighet 10
      display_word(5,patterns,antal_tecken,10);
      // Nu är vi klara, sätt antal tecken till 0.
      antal_tecken = 0; 
    }
  } 
}

void fyll_teckenbufferten(int antal) {
  byte tecken;
  for(int az=0;az<antal;az++){// ...bokstäver A-Z,a-z
    if((incomingByte[az] > 64 && incomingByte[az] < 91) || (incomingByte[az] > 96 && incomingByte[az] < 123)){
      if(incomingByte[az] > 64 && incomingByte[az] < 91){
        for(int row=0;row<7;row++) patterns[az][row] = chr_array[incomingByte[az] - 65][row]; // 65=Ascii-Offset för "A"
      }
      else {
        for(int row=0;row<7;row++) patterns[az][row] = chr_array[incomingByte[az] - 53][row]; // 53=Ascii-Offset för "a"
      }
    }
    else { // ...övriga tecken
      switch(incomingByte[az]){
      case  32:        tecken=26;        break; //Mellanslag
      case  33:        tecken=43;        break; //Utropstecken
      case  43:        tecken=76;        break; // Plus
      case  44:        tecken=40;        break; // Komma
      case  45:        tecken=41;        break; // Minus                
      case  46:        tecken=39;        break; // Punkt
      case  47:        tecken=37;        break; // Divide
      case  48:        tecken=27;        break; // 0
      case  49:        tecken=28;        break; // 1
      case  50:        tecken=29;        break; // 2
      case  51:        tecken=30;        break; // 3
      case  52:        tecken=31;        break; // 4
      case  53:        tecken=32;        break; // 5
      case  54:        tecken=33;        break; // 6 
      case  55:        tecken=34;        break; // 7
      case  56:        tecken=35;        break; // 8
      case  57:        tecken=36;        break; // 9
      case  58:        tecken=38;        break; // Kolon
      case  60:        tecken=78;        break; // Mindre än
      case  61:        tecken=77;        break; // Lika med
      case  62:        tecken=79;        break; // Större än
      case  63:        tecken=42;        break; // Frågetecken
      case  95:        tecken=80;        break; // Underscore
      case 196:        tecken=74;        break; // Ä
      case 197:        tecken=73;        break; // Å
      case 214:        tecken=75;        break; // Ö
      case 228:        tecken=71;        break; // ä
      case 229:        tecken=70;        break; // å
      case 246:        tecken=72;        break; // ö
      default:        tecken=26;        break; // Allt annat blir mellanslag
      }
      for(int row=0;row<7;row++) patterns[az][row] = chr_array[tecken][row];
    }               
  }
}

void display_word(int loops,byte word_print[][7],int num_patterns,int scroll_delay){ 
  // the main funcion for displaying and scrolling the word
  for(int first=0;first<7;first++){ // Nolla display_matrix
    for(int second=0;second<4;second++)
      display_matrix[first][second] = 0;
  }   
  for ( ; loops>0; loops--) {  
    for(int x=0;x<num_patterns;x++){     // Antalet tecken som teckensträngen består av
      for(int scroll=0;scroll<7;scroll++){  // Varje tecken består av 5+1 column.
        for(int r=0;r<7;r++){ // Skifta ett steg vänster, upprepa för alla 7 raderna.
          display_matrix[r][3] = (display_matrix[r][3] << 1)+((display_matrix[r][2] & 0x80) >> 7); 
          display_matrix[r][2] = (display_matrix[r][2] << 1)+((display_matrix[r][1] & 0x80) >> 7); 
          display_matrix[r][1] = (display_matrix[r][1] << 1)+((display_matrix[r][0] & 0x80) >> 7); 
          display_matrix[r][0] = (display_matrix[r][0] << 1)+(((word_print[x][r]<<scroll)&0x80)>>7);
        }
        for(int t=0;t<scroll_delay;t++) delay(5);
      } 
    } 
  }

  for (int n=0;n<30;n++){ // Skifta ut 30 tecken och fyll på med nollor. Tömmer displayen
    for(int h=0;h<7;h++){ // Skifta ett steg vänster, upprepa för alla 7 raderna.
      display_matrix[h][3] = (display_matrix[h][3] << 1) | ((display_matrix[h][2] & 0x80) >> 7); 
      display_matrix[h][2] = (display_matrix[h][2] << 1) | ((display_matrix[h][1] & 0x80) >> 7); 
      display_matrix[h][1] = (display_matrix[h][1] << 1) | ((display_matrix[h][0] & 0x80) >> 7); 
      display_matrix[h][0] = (display_matrix[h][0] << 1) | 0 ;
    }
    for(int t=0;t<scroll_delay;t++) delay(5); 
  }
}

void LEDflash() {
  // interruptrutin. row_mem måste finnas def. utanför interrupten, den håller reda på vilken rad som just nu lyser.
  // denna rutin repeteras var tredje ms, vilket innebär att varje rad lyser i 3ms.
  // (har provat och sätta 4ms, men då flimrar displayen, ger för låg frame-rate)
  // display_matrix är en 7 x 8 byte matris som innehåller det som för närvarande ska visas på LED-displayen
  byte row_prev = row_mem;
  if (row_mem == 0) row_prev=6; 
  else row_prev--;
  digitalWrite(pinscan[row_prev], 0);
  // Gör reset på 32-bitarsbufferten
  digitalWrite(mrPin, 0);
  digitalWrite(mrPin, 1);
  // Kör ut 8 bitar * 4 = 32 bitar data för en rad, row_mem
  SPI.transfer(display_matrix[row_mem][3]);
  SPI.transfer(display_matrix[row_mem][2]);
  SPI.transfer(display_matrix[row_mem][1]);
  SPI.transfer(display_matrix[row_mem][0]);
  // Lys upp en av de 7 rader (row_mem)
  digitalWrite(pinscan[row_mem], 1);
  // Förbered inför nästa rad när vi återvänder hit efter 3ms.
  row_mem++;
  if (row_mem == 7) row_mem = 0;  
}
