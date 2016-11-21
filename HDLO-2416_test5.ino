/*
 * // http://gorgusgfx.se/?page_id=62
 * kod för att styra en DLR3416-display från Siemens
 * Nicu FLORICA tests 5 module HDLO-2416 
 * for future projects see http://arduinotehniq.com
 * or http://www.tehnic.go.ro
 * ver.4 - use 74HC138 for control display module
 * ver.5 - use subroutines for display characters & clear
 */
 
byte  a[2];      // Adresspinnar  
byte  d[7];      // Datapinnar
byte  bl;        // Blankingpinne
byte  wr;        // Writepinne   
byte  i;         // Temporär variabel
byte c[3];       // module selector
int timp = 250;
int timp2 = 2000;
 
void setup()
{
 
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

mop(); 
}
 
void loop()
{


for (byte x=0; x < 20; x++)  // number of modules
{
zona1(x);
numar(48+x);
delay(timp);
}
mop1(250);

for (int x=19; x >= 0; x--)  // number of modules
{
zona1(x);
numar(67-x);
delay(timp);
}
mop2(250);

for (byte h=0; h < 10; h++)
{
for (byte x=0; x < 20; x++)  // number of modules
{
zona1(x);
numar(48+x);
//delay(timp);
}
delay(timp);
mop();
delay(timp);
}
  
  delay(timp2);  

//stergere();
 
}
 
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
