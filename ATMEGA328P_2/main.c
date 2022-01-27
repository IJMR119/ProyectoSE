#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#define SPEAKER_PORT PORTC
#define SPEAKER_DDR DDRC
#define SPEAKER_PIN 2


#define _BV(bit) (1 << (bit))
#define bit_is_clear(sfr, bit) (!(_SFR_BYTE(sfr) & _BV(bit)))
#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))

void Sound_play(float frequency,float duration)
{
	long int i,cycles;
	float half_period;
	float wavelength;
	wavelength=(1/frequency)*1000;
	cycles=duration/wavelength;
	half_period = wavelength/2;
	SPEAKER_DDR |= (1 << SPEAKER_PIN);
	for (i=0;i<cycles;i++)
	{
		_delay_ms(half_period);
		SPEAKER_PORT |= (1 << SPEAKER_PIN);
		_delay_ms(half_period);
		SPEAKER_PORT &= ~(1 << SPEAKER_PIN);
	}
}

int main()
{
    DDRC= 0b00000100;
   while(1)
   {
      if(bit_is_set(PINC,PC0))
      {
      Sound_play(200,100);
      Sound_play(200,100);
      Sound_play(200,100);
      Sound_play(0,200);
      Sound_play(200,100);
      Sound_play(200,100);
      Sound_play(0,200);
      }
   
}
   return 0;
}