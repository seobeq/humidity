#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#define MAXTIMINGS	85
#define DHTPIN		7
float hum;
float temp;
int dht22_dat[5] = {0, 0, 0, 0, 0};
//int wiringPiSetup();
void read_dht22_dat()
{
uint8_t laststate	= HIGH;
uint8_t counter		= 0;
uint8_t j		= 0, i;
float	f;
dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;

pinMode(DHTPIN, OUTPUT);
digitalWrite(DHTPIN, LOW);
delay(18);
digitalWrite(DHTPIN, HIGH);
delayMicroseconds(30);
pinMode(DHTPIN, INPUT);

for (i = 0; i < MAXTIMINGS; i++){
counter = 0;
while (digitalRead(DHTPIN) == laststate){
counter++;
delayMicroseconds(1);
if (counter == 255)
{
break;
}
}
laststate = digitalRead(DHTPIN);

if (counter == 255)
break;

if ((i >=4) && (i%2 == 0))
{
dht22_dat[j/8] <<= 1;
if (counter > 16)
dht22_dat[j/8] |= 1;
j++;
}
}

if ((j >= 40) && (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF)))
{
f = dht22_dat[2] * 9. / 5. + 32;
hum = dht22_dat[0] * 256 + dht22_dat[1];
hum /= 10.0;
temp = (dht22_dat[2] & 0x7F)* 256 + dht22_dat[3];
temp /= 10.0;
printf( "Relative Humidity = %2.f%  Temperature = %2.fC\n", hum, temp);
FILE *fpoint;
time_t timestamp;
time(&timestamp);
fpoint = fopen("Monitor.txt", "a");
//fprintf(fpoint, "Environmental data..\n");
fprintf(fpoint, "Humidity: %2.f Temp: %2.f %s\n\n",hum, temp, ctime(&timestamp)); // need a format specifier
//fprintf(fpoint, "Humidity: %2.f Temp: %2.f\n\n",hum, temp);
fclose(fpoint);

}
else{
printf( "Data not good, skip\n" );
}
}

int main(void){
printf( "Raspberry Pi wiringPi DHT22 Temperature test program\n" );

if (wiringPiSetup() == -1)
exit(1);

while (1)
{
read_dht22_dat();
delay(5000);
}
return(0);
}
