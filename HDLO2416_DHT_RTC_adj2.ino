/*
 * // http://gorgusgfx.se/?page_id=62
 * kod för att styra en DLR3416-display från Siemens
 * Nicu FLORICA tests 5 module HDLO-2416 
 * for future projects see http://arduinotehniq.com
 * or http://www.tehnic.go.ro
 * ver.4 - use 74HC138 for control display module
 * ver.5 - use subroutines for display characters & clear
 * ver.6 - DHT termomether and higrometer
 * ver.6d - add RTC data and hour
 * ver.6e - add MENU and + button for change data and hour
 * ver.7 - change display style and use roumanian, english or german language
 */

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
//#include "RTClib.h"
//RTC_DS1307 rtc;
//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


#include "DHT.h"
#define DHTPIN 12     // what pin we're connected to
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);
// NOTE: For working with a faster chip, like an Arduino Due or Teensy, you
// might need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// Example to initialize DHT sensor for Arduino Due:
//DHT dht(DHTPIN, DHTTYPE, 30);


byte  a[2];      // Adresspinnar  
byte  d[7];      // Datapinnar
byte  bl;        // Blankingpinne
byte  wr;        // Writepinne   
byte  i;         // Temporär variabel
byte c[3];       // module selector
int timp1 = 150;
int timp2 = 30;  // x 0,1sec

float temperatura;
int umiditate;
int tzeci, tunit, tzecimi, trest;
int tsemn, ttot;
int hzeci, hunit;

byte hh, mm, ss;
byte yy, ll, dd, zz;
#define DS3231_I2C_ADDRESS 104
byte tMSB, tLSB;

unsigned long timpi1;
unsigned long timpi2 = 1000;

#define meniu 13    // button MENU 
#define plus 17     // buttom +
int nivel = 20;     // state level 
// 0 - normal clock
// 2 - chamge hour
// 4 - change minute
// 6 - chamge year
// 8 - change month
// 10 - change day
// 
// 20 - ready
int limba = 1;   // 1 for english, 2 for roumanian, 3 for german
int maxday;
 
void setup()
{
 dht.begin();
temperatura = 10*dht.readTemperature();
umiditate = dht.readHumidity();
delay(1500);
  Serial.begin(9600);
  Wire.begin();
/*
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
   if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
*/
// setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year);
//setDS3231time(0, 45, 13, 0, 25, 11, 2016;

  
  wr=9;                  //Write = pin 9 på Arduinon
  pinMode(wr,OUTPUT);
  digitalWrite(wr,1);
 
  bl=12;                  // Blanking = Pin 12 på Arduinon
  pinMode(bl,OUTPUT);    
  digitalWrite(bl,1);
 
  a[0]=10;                // Adresspinnarna, 10 och 11 på Arduinon
  a[1]=11;
  pinMode(a[0],OUTPUT);
  pinMode(a[1],OUTPUT);
 
  d[0]=2;                  // Datapinnar från pin 2 - pin 8 på Arduinon
  d[1]=3;
  d[2]=4;
  d[3]=5;
  d[4]=6;
  d[5]=7;
  d[6]=8;

  c[0]=14;
  c[1]=15;
  c[2]=16; 
 
  for(i=0;i<7;i++) {
    pinMode(d[i],OUTPUT);
  }
  for(i=0;i<3;i++) {
    pinMode(c[i],OUTPUT);
  }

 for(i=0;i<5;i++) {
    digitalWrite(c[i],0);
  }

pinMode(meniu, INPUT);   // MENU button
pinMode(plus, INPUT);   // + button
digitalWrite(meniu, HIGH);  
digitalWrite(plus, HIGH);  

mop(); 
timpi1 = millis();
}
 
void loop()
{


if (nivel == 0)  // clock
{

temperatura = 10*dht.readTemperature();
umiditate = dht.readHumidity();
//temperatura = random(-150, 350);
//temperatura = temperatura/10;
//umiditate = random (10, 90);
//temperatura = 10*dht.readTemperature();
delay(500);

if (temperatura >= 0) 
{
  tsemn = 1;
  ttot = temperatura;
}
if (temperatura < 0) 
{
  tsemn = -1;
  ttot = -temperatura;
}

tzeci = ttot/100;
trest = ttot%100;
tunit = trest/10;
tzecimi = trest%10;
hzeci = umiditate/10;
hunit = umiditate%10;

if (digitalRead(meniu) == LOW)
  {
  nivel = 1;
  delay(500);
  mop();
  }
    
mop();
 for (byte i=0; i < timp1; i++) 
{
/*DateTime now = rtc.now();  
hh = now.hour();
mm = now.minute();
ss = now.second();
yy = now.year()-2000;
ll = now.month();
dd = now.day();
zz = now.dayOfTheWeek();
*/

  readDS3231time(&ss, &mm, &hh, &zz, &dd, &ll,&yy);

if (digitalRead(meniu) == LOW)
  {
  nivel = 1;
  i = timp1;
  delay(500);
  mop();
  }

zona1(19);
numar(48+hh/10);  // hour tens
zona1(18);
numar(48+hh%10);  // hour units
zona1(17);
numar(45);  // -
zona1(16);
numar(48+mm/10);  // minute tens
zona1(15);
numar(48+mm%10);  // minute units
zona1(14);
numar(45);  // -
zona1(13);
numar(48+ss/10);  // second tens
zona1(12);
numar(48+ss%10);  // second units

if (tzeci == 0)
{
//zona1(10);
//numar(32);  //empty
zona1(10);
if (tsemn == 1) numar(43);  //+
else numar(45);  //-
}
else
{
//zona1(10);  
//if (tsemn == 1) numar(43);  //+
//else numar(45);  //-
zona1(10);
numar(tzeci+48);  //cifra
}
zona1(9);
numar(tunit+48);  //cifra
zona1(8);
numar(46);  //.
//numar(44);  //,
zona1(7);
numar(tzecimi+48);  //cifra
zona1(6);
numar(34);  //grad
zona1(5);
numar(67);  //C
//zona1(1);
//numar(27);  //grad Celsius
//}

zona1(3);
numar(hzeci+48);  //cifra
zona1(2);
numar(hunit+48);  //cifra
zona1(1);
numar(82);  //R
zona1(0);
numar(72);  //H
}

// name of day   
if (nivel == 0)
{ 
mop();  // clear the screen;

for(i=0;i<timp2;i++)
{
 if (digitalRead(meniu) == LOW)
  {
  nivel = 1;
  delay(500);
  mop();
  } 

if (limba ==1)   // english
{
if (zz == 1)
{
zona1(19);
numar(77);  //M   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(111);  //o
zona1(17);
numar(110);  //n
zona1(16);
numar(100);  //d
zona1(15);
numar(97);  //a
zona1(14);
numar(121);  //y
}
if (zz == 2)
{
zona1(19);
numar(84);  //T   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(104);  //h
zona1(17);
numar(117);  //u
zona1(16);
numar(114);  //r
zona1(15);
numar(115);  //s
zona1(14);
numar(100);  //d
zona1(13);
numar(97);  //a
zona1(12);
numar(121);  //y
}
if (zz == 3)
{
zona1(19);
numar(87);  //W   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(101);  //e
zona1(17);
numar(100);  //d
zona1(16);
numar(110);  //n
zona1(15);
numar(101);  //e
zona1(14);
numar(115);  //s
zona1(13);
numar(100);  //d
zona1(12);
numar(97);  //a
zona1(11);
numar(121);  //y
}
if (zz == 4)
{
zona1(19);
numar(84);  //T   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(104);  //h
zona1(17);
numar(117);  //u
zona1(16);
numar(114);  //s
zona1(15);
numar(100);  //d
zona1(14);
numar(97);  //a
zona1(13);
numar(121);  //y
}
if (zz == 5)
{
zona1(19);
numar(70);  //F   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(114);  //r
zona1(17);
numar(105);  //i
zona1(16);
numar(100);  //d
zona1(15);
numar(97);  //a
zona1(14);
numar(121);  //y
}
if (zz == 6)
{
zona1(19);
numar(83);  //S   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(97);  //a
zona1(17);
numar(116);  //t
zona1(16);
numar(117);  //u
zona1(15);
numar(114);  //r
zona1(14);
numar(100);  //d
zona1(13);
numar(97);  //a
zona1(12);
numar(121);  //y
}
if ((zz == 0) || (zz == 7))
{
zona1(19);
numar(83);  //S   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(117);  //u
zona1(17);
numar(110);  //n
zona1(16);
numar(100);  //d
zona1(15);
numar(97);  //a
zona1(14);
numar(121);  //y
}
}  //  english

if (limba ==3)
{
// german
if (zz == 1)
{
zona1(19);
numar(77);  //M   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(111);  //o
zona1(17);
numar(110);  //n
zona1(16);
numar(116);  //t
zona1(15);
numar(97);  //a
zona1(14);
numar(103);  //g
}
if (zz == 2)
{
zona1(19);
numar(68);  //D   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(105);  //i
zona1(17);
numar(101);  //e
zona1(16);
numar(110);  //n
zona1(15);
numar(115);  //s
zona1(14);
numar(116);  //t
zona1(13);
numar(97);  //a
zona1(12);
numar(103);  //g
}
if (zz == 3)
{
zona1(19);
numar(77);  //M   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(105);  //i
zona1(17);
numar(116);  //t
zona1(16);
numar(116);  //t
zona1(15);
numar(119);  //w
zona1(14);
numar(111);  //o
zona1(13);
numar(99);  //c
zona1(12);
numar(104);  //h
}
if (zz == 4)
{
zona1(19);
numar(68);  //D   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(111);  //o
zona1(17);
numar(110);  //n
zona1(16);
numar(110);  //n
zona1(15);
numar(101);  //e
zona1(14);
numar(114);  //r
zona1(13);
numar(115);  //s
zona1(12);
numar(116);  //t
zona1(11);
numar(97);  //a
zona1(10);
numar(103);  //g
}
if (zz == 5)
{
zona1(19);
numar(70);  //F   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(114);  //r
zona1(17);
numar(101);  //e
zona1(16);
numar(105);  //i
zona1(15);
numar(116);  //t
zona1(14);
numar(97);  //a
zona1(13);
numar(103);  //g
}
if (zz == 6)
{
zona1(19);
numar(83);  //S   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(97);  //a
zona1(17);
numar(109); //m
zona1(16);
numar(115);  //s
zona1(15);
numar(116);  //t
zona1(14);
numar(97);  //a
zona1(13);
numar(103);  //g
}
if ((zz == 0) || (zz == 7))
{
zona1(19);
numar(83);  //S   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(111);  //o
zona1(17);
numar(110);  //n
zona1(16);
numar(110);  //n
zona1(15);
numar(116);  //t
zona1(14);
numar(97);  //a
zona1(13);
numar(103);  //g
}
}  //  romana

if (limba ==2)
{
// romana
if (zz == 1)
{
zona1(19);
numar(76);  //L   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(117);  //u
zona1(17);
numar(110);  //n
zona1(16);
numar(105);  //i
}
if (zz == 2)
{
zona1(19);
numar(77);  //M   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(97);  //a
zona1(17);
numar(114);  //r
zona1(16);
numar(116);  //t
zona1(15);
numar(105);  //i
}
if (zz == 3)
{
zona1(19);
numar(77);  //M   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(105);  //i
zona1(17);
numar(101);  //e
zona1(16);
numar(114);  //r
zona1(15);
numar(99);  //t
zona1(14);
numar(105);  //i
}
if (zz == 4)
{
zona1(19);
numar(74);  //J   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(111);  //o
zona1(17);
numar(105);  //i
}
if (zz == 5)
{
zona1(19);
numar(86);  //V   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(105);  //i
zona1(17);
numar(110);  //n
zona1(16);
numar(101);  //e
zona1(15);
numar(114);  //r
zona1(14);
numar(105);  //i
}
if (zz == 6)
{
zona1(19);
numar(83);  //S   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(97);  //a
zona1(17);
numar(109);  //m
zona1(16);
numar(98);  //b
zona1(15);
numar(97);  //a
zona1(14);
numar(116);  //t
zona1(13);
numar(97);  //a
}
if ((zz == 0) || (zz == 7))
{
zona1(19);
numar(68);  //D   - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(117);  //u
zona1(17);
numar(109);  //m
zona1(16);
numar(105);  //i
zona1(15);
numar(110);  //n
zona1(14);
numar(105);  //i
zona1(13);
numar(99);  //c
zona1(12);
numar(97);  //a
}
}  //  german



zona1(7);
if (dd/10 == 0) 
{
numar(32);  //empty  
}
else
numar(48+dd/10);  // day tens
zona1(6);
numar(48+dd%10);  // day units


if (limba == 1)
{
if (ll == 1)   // January
{
zona1(3);
numar(74);  //J
zona1(2);
numar(65);  //A
zona1(1);
numar(78);  //N  
}
if (ll == 2)   // February
{
zona1(3);
numar(70);  //F
zona1(2);
numar(69);  //E
zona1(1);
numar(66);  //B
}
if (ll == 3)   // March
{
zona1(3);
numar(77);  //M
zona1(2);
numar(65);  //A
zona1(1);
numar(82);  //R
}
if (ll == 4)   // April
{
zona1(3);
numar(65);  //A
zona1(2);
numar(80);  //P
zona1(1);
numar(82);  //R
}
if (ll == 5)   // May
{
zona1(3);
numar(77);  //M
zona1(2);
numar(65);  //A
zona1(1);
numar(89);  //Y
}
if (ll == 6)   // June
{
zona1(3);
numar(74);  //J
zona1(2);
numar(85);  //U
zona1(1);
numar(78);  //N
}
if (ll == 7)   // July
{
zona1(3);
numar(74);  //J
zona1(2);
numar(85);  //U
zona1(1);
numar(76);  //L
}
if (ll == 8)   // August
{
zona1(3);
numar(65);  //A
zona1(2);
numar(85);  //U
zona1(1);
numar(71);  //G
}
if (ll == 9)   // Septembre
{
zona1(3);
numar(83);  //S
zona1(2);
numar(69);  //E
zona1(1);
numar(80);  //P
}
if (ll == 10)   // October
{
zona1(3);
numar(79);  //O
zona1(2);
numar(67);  //C
zona1(1);
numar(84);  //T
}
if (ll == 11)   // November
{
zona1(3);
numar(78);  //N
zona1(2);
numar(79);  //O
zona1(1);
numar(86);  //V
}
if (ll == 12)   // December
{
zona1(3);
numar(68);  //D
zona1(2);
numar(69);  //E
zona1(1);
numar(67);  //C
}
}  // end english

if (limba == 2)
{
if (ll == 1)   // ianuarie
{
zona1(3);
numar(73);  //I
zona1(2);
numar(65);  //A
zona1(1);
numar(78);  //N  
}
if (ll == 2)   // februarie
{
zona1(3);
numar(70);  //F
zona1(2);
numar(69);  //E
zona1(1);
numar(66);  //B
}
if (ll == 3)   // martie
{
zona1(3);
numar(77);  //M
zona1(2);
numar(65);  //A
zona1(1);
numar(82);  //R
}
if (ll == 4)   // aprilie
{
zona1(3);
numar(65);  //A
zona1(2);
numar(80);  //P
zona1(1);
numar(82);  //R
}
if (ll == 5)   // mai
{
zona1(3);
numar(77);  //M
zona1(2);
numar(65);  //A
zona1(1);
numar(73);  //I
}
if (ll == 6)   // iunie
{
zona1(3);
numar(73);  //I
zona1(2);
numar(85);  //U
zona1(1);
numar(78);  //N
}
if (ll == 7)   // iulie
{
zona1(3);
numar(73);  //I
zona1(2);
numar(85);  //U
zona1(1);
numar(76);  //L
}
if (ll == 8)   // august
{
zona1(3);
numar(65);  //A
zona1(2);
numar(85);  //U
zona1(1);
numar(71);  //G
}
if (ll == 9)   // septembrie
{
zona1(3);
numar(83);  //S
zona1(2);
numar(69);  //E
zona1(1);
numar(80);  //P
}
if (ll == 10)   // octombrie
{
zona1(3);
numar(79);  //O
zona1(2);
numar(67);  //C
zona1(1);
numar(84);  //T
}
if (ll == 11)   // noiembrie
{
zona1(3);
numar(78);  //N
zona1(2);
numar(79);  //O
zona1(1);
numar(73);  //I
}
if (ll == 12)   // decembrie
{
zona1(3);
numar(68);  //D
zona1(2);
numar(69);  //E
zona1(1);
numar(67);  //C
}
}  // end limba romana

if (limba == 3)  // german
{
if (ll == 1)   // JAN
{
zona1(3);
numar(74);  //J
zona1(2);
numar(65);  //A
zona1(1);
numar(78);  //N  
}
if (ll == 2)   // FEBR
{
zona1(3);
numar(70);  //F
zona1(2);
numar(69);  //E
zona1(1);
numar(66);  //B
zona1(0);
numar(82);  //R
}
if (ll == 3)   // Marz
{
zona1(3);
numar(77);  //M
zona1(2);
numar(21);  //A
zona1(1);
numar(82);  //R
zona1(1);
numar(90);  //Z
}
if (ll == 4)   // APR
{
zona1(3);
numar(65);  //A
zona1(2);
numar(80);  //P
zona1(1);
numar(82);  //R
}
if (ll == 5)   // MAI
{
zona1(3);
numar(77);  //M
zona1(2);
numar(65);  //A
zona1(1);
numar(83);  //I
}
if (ll == 6)   // JUNI
{
zona1(3);
numar(74);  //J
zona1(2);
numar(85);  //U
zona1(1);
numar(78);  //N
zona1(0);
numar(83);  //I
}
if (ll == 7)   // JULY
{
zona1(3);
numar(74);  //J
zona1(2);
numar(85);  //U
zona1(1);
numar(76);  //L
zona1(0);
numar(83);  //I
}
if (ll == 8)   // AUG
{
zona1(3);
numar(65);  //A
zona1(2);
numar(85);  //U
zona1(1);
numar(71);  //G
}
if (ll == 9)   // SEPT
{
zona1(3);
numar(83);  //S
zona1(2);
numar(69);  //E
zona1(1);
numar(80);  //P
zona1(0);
numar(84);  //T
}
if (ll == 10)   // OKT
{
zona1(3);
numar(79);  //O
zona1(2);
numar(75);  //K
zona1(1);
numar(84);  //T
}
if (ll == 11)   // NOV
{
zona1(3);
numar(78);  //N
zona1(2);
numar(79);  //O
zona1(1);
numar(86);  //V
}
if (ll == 12)   // DEZ
{
zona1(3);
numar(68);  //D
zona1(2);
numar(69);  //E
zona1(1);
numar(90);  //Z
}
}  // end german

delay(timp2);   
}  // end data part
}
}   // end nivel = 0

if (nivel == 1)  // clear
{
  mop();
  nivel = 2;
}

if (nivel == 2)  // hour adjust
{
if (digitalRead(meniu) == LOW)
  {
  nivel = 3;
  delay(500);
 // mop();
  }

if (limba ==2)
{
// romana
zona1(19);
numar(114);  //r - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(101);  //e
zona1(17);
numar(103);  //g
zona1(16);
numar(108);  //l
zona1(15);
numar(97);  //a
zona1(14);
numar(106);  //j
zona1(13);
numar(32);  //space
zona1(12);
numar(111);  //o
zona1(11);
numar(114);  //r
zona1(10);
numar(97);  //a
zona1(9);
numar(58);  //:
}
if (limba == 1)
{
// english
zona1(19);
numar(104);  //h - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(111);  //o - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(17);
numar(117);  //u
zona1(16);
numar(114);  //r
zona1(15);
numar(32);  //space
zona1(14);
numar(97);  //a
zona1(13);
numar(100);  //d
zona1(12);
numar(106);  //j
zona1(11);
numar(117);  //u
zona1(10);
numar(115);  //s
zona1(9);
numar(116);  //t
zona1(8);
numar(32);  //:
}
if (limba == 3)
{
// german
zona1(19);
numar(85);  //U - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(104);  //h - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(17);
numar(114);  //r
zona1(16);
numar(58);  //:
}

zona1(1);
numar(48+hh/10);  // hour tens
zona1(0);
numar(48+hh%10);  // hour units

if (digitalRead(plus) == LOW) 
  {
  hh = hh+1;
  delay(250);
  }
if (hh > 24) hh = 0;  
}  // end nivel = 2;   

if (nivel == 3)  // clear and jump to minute adjust
{
  mop();
  delay(500);
  nivel = 4;
}

if (nivel == 4)  // minute adjust
{
if (digitalRead(meniu) == LOW)
  {
  nivel = 5;
  delay(500);
  mop();
  }

if (limba ==2)
{
// romana
zona1(19);
numar(114);  //r - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(101);  //e
zona1(17);
numar(103);  //g
zona1(16);
numar(108);  //l
zona1(15);
numar(97);  //a
zona1(14);
numar(106);  //j
zona1(13);
numar(32);  //space
zona1(12);
numar(109);  //m
zona1(11);
numar(105);  //i
zona1(10);
numar(110);  //n
zona1(9);
numar(117);  //u
zona1(8);
numar(116);  //t
zona1(7);
numar(101);  //e
zona1(6);
numar(58);  //:
}
if (limba == 1)
{
// english
zona1(19);
numar(109);  //m - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(105);  //i - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(17);
numar(110);  //n
zona1(16);
numar(117);  //u
zona1(15);
numar(116);  //t
zona1(14);
numar(101);  //e
zona1(13);
numar(32);  //space
zona1(12);
numar(97);  //a
zona1(11);
numar(100);  //d
zona1(10);
numar(106);  //j
zona1(9);
numar(117);  //u
zona1(8);
numar(115);  //s
zona1(7);
numar(116);  //t
zona1(6);
numar(32);  //:
}
if (limba == 3)
{
// german
zona1(19);
numar(77);  //M - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(105);  //i - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(17);
numar(110);  //n
zona1(16);
numar(117);  //u
zona1(15);
numar(116);  //t
zona1(14);
numar(101);  //e
zona1(13);
numar(58);  //space
}

zona1(1);
numar(48+mm/10);  // hour tens
zona1(0);
numar(48+mm%10);  // hour units

if (digitalRead(plus) == LOW) 
  {
  mm = mm+1;
  delay(200);
  }
if (mm > 59) mm = 0;  
}  // end nivel = 4;   

if (nivel == 5)  // clear
{
  mop();
  nivel = 6;
}

if (nivel == 6)  // year adjust
{
if (digitalRead(meniu) == LOW)
  {
  nivel = 7;
  delay(500);
 // mop();
  }

if (limba ==2)
{
// romana
zona1(19);
numar(114);  //r - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(101);  //e
zona1(17);
numar(103);  //g
zona1(16);
numar(108);  //l
zona1(15);
numar(97);  //a
zona1(14);
numar(106);  //j
zona1(13);
numar(32);  //space
zona1(12);
numar(97);  //a
zona1(11);
numar(110);  //n
zona1(10);
numar(58);  //:
}
if (limba == 1)
{
// english
zona1(19);
numar(121);  //y - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(101);  //e - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(17);
numar(97);  //a
zona1(16);
numar(114);  //r
zona1(15);
numar(32);  //space
zona1(14);
numar(97);  //a
zona1(13);
numar(100);  //d
zona1(12);
numar(106);  //j
zona1(11);
numar(117);  //u
zona1(10);
numar(115);  //s
zona1(9);
numar(116);  //t
zona1(8);
numar(32);  //:
}
if (limba == 3)
{
// german
zona1(19);
numar(74);  //J - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(97);  //a - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(17);
numar(104);  //h
zona1(16);
numar(114);  //r
zona1(15);
numar(58);  //space
}

zona1(3);
numar(50);  //2 -> 48+2;
zona1(2);
numar(48);  //0
zona1(1);
numar(48+yy/10);  // year tens
zona1(0);
numar(48+yy%10);  // year units

if (digitalRead(plus) == LOW) 
  {
  yy = yy+1;
  delay(250);
  }
if (yy > 40) yy = 16;  
}  // end nivel = 6;   

if (nivel == 7)  // clear
{
  mop();
  nivel = 8;
}


if (nivel == 8)  // month adjust
{
if (digitalRead(meniu) == LOW)
  {
  nivel = 9;
  delay(500);
 // mop();
  }

if (limba ==2)
{
// romana
zona1(19);
numar(114);  //r - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(101);  //e
zona1(17);
numar(103);  //g
zona1(16);
numar(108);  //l
zona1(15);
numar(97);  //a
zona1(14);
numar(106);  //j
zona1(13);
numar(32);  //space
zona1(12);
numar(108);  //l
zona1(11);
numar(117);  //u
zona1(10);
numar(110);  //n
zona1(9);
numar(97);  //a
zona1(8);
numar(58);  //:
}
if (limba == 1)
{
// english
zona1(19);
numar(109);  //m - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(111);  //o - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(17);
numar(110);  //n
zona1(16);
numar(116);  //t
zona1(15);
numar(104);  //h
zona1(14);
numar(32);  //space
zona1(13);
numar(97);  //a
zona1(12);
numar(100);  //d
zona1(11);
numar(106);  //j
zona1(10);
numar(117);  //u
zona1(9);
numar(115);  //s
zona1(8);
numar(116);  //t
zona1(7);
numar(32);  //:
}
if (limba == 3)
{
// german
zona1(19);
numar(77);  //M - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(111);  //o - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(17);
numar(110);  //n
zona1(16);
numar(97);  //a
zona1(15);
numar(116);  //t
zona1(14);
numar(58);  //:
}

zona1(1);
numar(48+ll/10);  // month tens
zona1(0);
numar(48+ll%10);  // month units

if (digitalRead(plus) == LOW) 
  {
  ll = ll+1;
  delay(250);
  }
if (ll > 12) ll = 1;  
}  // end nivel = 8;   

if (nivel == 9)  // clear
{
  mop();
  nivel = 10;
}

if (nivel == 10)  // day adjust
{
if (digitalRead(meniu) == LOW)
  {
  nivel = 11;
  delay(500);
 // mop();
  }

if (limba ==2)
{
// romana
zona1(19);
numar(114);  //r - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(101);  //e
zona1(17);
numar(103);  //g
zona1(16);
numar(108);  //l
zona1(15);
numar(97);  //a
zona1(14);
numar(106);  //j
zona1(13);
numar(32);  //space
zona1(12);
numar(122);  //z
zona1(11);
numar(105);  //i
zona1(10);
zona1(10);
numar(58);  //:
}
if (limba == 1)
{
// english
zona1(19);
numar(100);  //d - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(97);  //a - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(17);
numar(121);  //y
zona1(16);
numar(32);  //space
zona1(15);
numar(97);  //a
zona1(14);
numar(100);  //d
zona1(13);
numar(106);  //j
zona1(12);
numar(117);  //u
zona1(11);
numar(115);  //s
zona1(10);
numar(116);  //t
zona1(9);
numar(32);  //:
}
if (limba == 3)
{
// german
zona1(19);
numar(84);  //T - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(97);  //a - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(17);
numar(103);  //g
zona1(16);
numar(32);  //space
}

zona1(1);
numar(48+dd/10);  // day tens
zona1(0);
numar(48+dd%10);  // day  units

if (digitalRead(plus) == LOW) 
  {
  dd = dd+1;
  delay(250);
  }

// Dawn & Dusk controller. http://andydoz.blogspot.ro/2014_08_01_archive.html
 if (ll == 4 || ll == 5 || ll == 9 || ll == 11) { //30 days hath September, April June and November
    maxday = 30;
  }
  else {
  maxday = 31; //... all the others have 31
  }
  if (ll ==2 && yy % 4 ==0) { //... Except February alone, and that has 28 days clear, and 29 in a leap year.
    maxday = 29;
  }
  if (ll ==2 && ll % 4 !=0) {
    maxday = 28;
  }

if (dd > maxday) dd = 1;   
 
}  // end nivel = 10;   


if (nivel == 11)  // day in week adjust
{
if (digitalRead(meniu) == LOW)
  {
  nivel = 12;
  delay(500);
 // mop();
  }

if (limba ==2)
{
// romana
zona1(19);
numar(122);  //z - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(105);  //i
zona1(17);
numar(32);  //space
zona1(16);
numar(105);  //i
zona1(15);
numar(110);  //n
zona1(14);
numar(32);  //space
zona1(13);
numar(115);  //s
zona1(12);
numar(97);  //a
zona1(11);
numar(112);  //p
zona1(10);
numar(116);  //t
zona1(9);
numar(97);  //a
zona1(8);
numar(109);  //m
zona1(7);
numar(97);  //a
zona1(6);
numar(110);  //n
zona1(5);
numar(97);  //a
zona1(4);
numar(58);  //:
}
if (limba == 1)
{
// english
zona1(19);
numar(100);  //d - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(97);  //a - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(17);
numar(121);  //y
zona1(16);
numar(32);  //space
zona1(15);
numar(105);  //i
zona1(14);
numar(110);  //n
zona1(13);
numar(32);  //space
zona1(12);
numar(119);  //w
zona1(11);
numar(101);  //e
zona1(10);
numar(101);  //e
zona1(9);
numar(107);  //k
}
if (limba == 3)
{
// german
zona1(19);
numar(84);  //T - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(18);
numar(97);  //a - http://www.binaryhexconverter.com/binary-to-decimal-converter
zona1(17);
numar(103);  //g
zona1(16);
numar(32);  //space
zona1(15);
numar(100);  //d
zona1(14);
numar(101);  //e
zona1(13);
numar(114);  //r
zona1(12);
numar(32);  //space
zona1(11);
numar(87);  //W
zona1(10);
numar(111);  //o
zona1(9);
numar(99);  //c
zona1(8);
numar(104);  //h
zona1(7);
numar(101);  //e
}

zona1(1);
numar(32);  // empty
zona1(0);
numar(48+zz);  // day in week

if (digitalRead(plus) == LOW) 
  {
  zz = zz+1;
  delay(250);
  }
if (zz > 7) zz = 1 ;
}



if (nivel == 12)
{ 
 // January 21, 2014 at 3am you would call:
 //rtc.adjust(DateTime(2000+yy, ll, dd, hh, mm, 0));
 setDS3231time(0, mm, hh, zz, dd, ll, yy);
 nivel = 20;   
}

if (nivel == 20)
{
mop();
temperatura = 10*dht.readTemperature();
umiditate = dht.readHumidity();
delay(1000);
nivel = 0;
}  // end last level & go to clock level

 
}  // end main program
 
void wrt()
{
  digitalWrite(wr,0);
  delay(5);
  digitalWrite(wr,1);
}


void mop()
{
  for (byte i=0; i < 5; i++)  // number of modules
  {
  byte c2 = i/4;
  byte restc = i%4;
  byte c1 = restc/2;
  byte c0 = restc%2;
  digitalWrite(c[0],c0);   //
  digitalWrite(c[1],c1);
  digitalWrite(c[2],c2);   
  
  for (byte j=0; j < 4; j++)  // character in module
  {
  byte a1 = j/2;
  byte a0 = j%2;
  digitalWrite(a[0],a0);    //adress =1
  digitalWrite(a[1],a1);
  
// empty character
  digitalWrite(d[0],0);  // empty
  digitalWrite(d[1],0); 
  digitalWrite(d[2],0); 
  digitalWrite(d[3],0);
  digitalWrite(d[4],0); 
  digitalWrite(d[5],1); 
  digitalWrite(d[6],0); 

  digitalWrite(wr,0);
  delay(1);
  digitalWrite(wr,1);
  } // end characters in module (0 to 3)
  } // end modules (0 to 5)
}  // end 'mop'subroutine     

void numar(byte nr)
{
  byte d6 = nr/64;
  byte restd = nr%64;
  byte d5 = restd/32;
  restd = restd%32;
  byte d4 = restd/16;
  restd = restd%16;
  byte d3 = restd/8;
  restd = restd%8;
  byte d2 = restd/4;
  restd = restd%4;
  byte d1 = restd/2;
  byte d0 = restd%2;

  digitalWrite(d[0],d0);  // empty
  digitalWrite(d[1],d1); 
  digitalWrite(d[2],d2); 
  digitalWrite(d[3],d3);
  digitalWrite(d[4],d4); 
  digitalWrite(d[5],d5); 
  digitalWrite(d[6],d6); 
   wrt();
}  

void zona(byte modul, byte cifra)  // 0..19
{
  byte c2 = modul/4;
  byte restc = modul%4;
  byte c1 = restc/2;
  byte c0 = restc%2;
  digitalWrite(c[0],c0);   //
  digitalWrite(c[1],c1);
  digitalWrite(c[2],c2);   
  
  byte a1 = cifra/2;
  byte a0 = cifra%2;
  digitalWrite(a[0],a0);    //adress =1
  digitalWrite(a[1],a1);
}

void zona1(byte pus)  // 0..19
{
  byte modul = pus/4;
  byte c2 = modul/4;
  byte restc = modul%4;
  byte c1 = restc/2;
  byte c0 = restc%2;
  digitalWrite(c[0],c0);   //
  digitalWrite(c[1],c1);
  digitalWrite(c[2],c2);   
  
  byte cifra = pus - 4*modul;
//  cifra = -cifra;
  byte a1 = cifra/2;
  byte a0 = cifra%2;
  digitalWrite(a[0],a0);    //adress =1
  digitalWrite(a[1],a1);
}


void mop1(byte timpi)
{
  for (byte i=0; i < 5; i++)  // number of modules
  {
  byte c2 = i/4;
  byte restc = i%4;
  byte c1 = restc/2;
  byte c0 = restc%2;
  digitalWrite(c[0],c0);   //
  digitalWrite(c[1],c1);
  digitalWrite(c[2],c2);   
  
  for (byte j=0; j < 4; j++)  // character in module
  {
  byte a1 = j/2;
  byte a0 = j%2;
  digitalWrite(a[0],a0);    //adress =1
  digitalWrite(a[1],a1);
  
// empty character
  digitalWrite(d[0],0);  // empty
  digitalWrite(d[1],0); 
  digitalWrite(d[2],0); 
  digitalWrite(d[3],0);
  digitalWrite(d[4],0); 
  digitalWrite(d[5],1); 
  digitalWrite(d[6],0); 

  digitalWrite(wr,0);
  delay(5);
  digitalWrite(wr,1);
  
  delay(timpi);
  } // end characters in module (0 to 3)
  } // end modules (0 to 5)
}  // end 'mop'subroutine     

void mop2(byte timpi)
{
  for (int i=4; i >= 0; i--)  // number of modules
  {
  byte c2 = i/4;
  byte restc = i%4;
  byte c1 = restc/2;
  byte c0 = restc%2;
  digitalWrite(c[0],c0);   //
  digitalWrite(c[1],c1);
  digitalWrite(c[2],c2);   
  
  for (int j=3; j >= 0; j--)  // character in module
  {
  byte a1 = j/2;
  byte a0 = j%2;
  digitalWrite(a[0],a0);    //adress =1
  digitalWrite(a[1],a1);
  
// empty character
  digitalWrite(d[0],0);  // empty
  digitalWrite(d[1],0); 
  digitalWrite(d[2],0); 
  digitalWrite(d[3],0);
  digitalWrite(d[4],0); 
  digitalWrite(d[5],1); 
  digitalWrite(d[6],0); 

  digitalWrite(wr,0);
  delay(2);
  digitalWrite(wr,1);
  
  delay(timpi);
  } // end characters in module (0 to 3)
  } // end modules (0 to 5)
}  // end 'mop'subroutine     

// http://forum.arduino.cc/index.php?topic=398891.0
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
                   dayOfMonth, byte month, byte year)
{
    // sets time and date data to DS3231
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set next input to start at the seconds register
    Wire.write(decToBcd(second)); // set seconds
    Wire.write(decToBcd(minute)); // set minutes
    Wire.write(decToBcd(hour)); // set hours
    Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
    Wire.write(decToBcd(month)); // set month
    Wire.write(decToBcd(year)); // set year (0 to 99)
    Wire.endTransmission();
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
    return ( (val / 16 * 10) + (val % 16) );
}

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
    return ( (val / 10 * 16) + (val % 10) );
}

void readDS3231time(byte *second,
                    byte *minute,
                    byte *hour,
                    byte *dayOfWeek,
                    byte *dayOfMonth,
                    byte *month,
                    byte *year)
{
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    // request seven bytes of data from DS3231 starting from register 00h
    *second = bcdToDec(Wire.read() & 0x7f);
    *minute = bcdToDec(Wire.read());
    *hour = bcdToDec(Wire.read() & 0x3f);
    *dayOfWeek = bcdToDec(Wire.read());
    *dayOfMonth = bcdToDec(Wire.read());
    *month = bcdToDec(Wire.read());
    *year = bcdToDec(Wire.read());
}
