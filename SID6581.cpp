/********************************************************
 *          ______                                      *
 *    _____|\     \       ____                 _____    *
 *   /     / |     |  ____\_  \__         _____\    \   *
 *  |      |/     /| /     /     \       /    / \    |  *
 *  |      |\____/ |/     /\      |     |    |  /___/|  *
 *  |\     \    | /|     |  |     |  ____\    \ |   ||  *
 *  | \     \___|/ |     |  |     | /    /\    \|___|/  *
 *  |  \     \     |     | /     /||    |/ \    \       *
 *   \  \_____\    |\     \_____/ ||\____\ /____/|      *
 *    \ |     |    | \_____\   | / | |   ||    | |      *
 *     \|_____|     \ |    |___|/   \|___||____|/       *
 *                   \|____|                            *
 *                                                      *
 ********************************************************
 *            SIDI: THE MIDI SID INTERFACE              *
 *            (c)2013-2015 Phar Out Studios             *
 *------------------------------------------------------*
 * This is the implementation of the SID6581 class that *
 * is responsible for the actual driving of the chip.   *
 *                                                      *
 * It also defines and instantiates a global instance   *
 * named SID because my current needs don't dictate     *
 * multiple SID chips on one Arduino.                   *
 *------------------------------------------------------*
 * This file is part of SIDI.                           *
 *                                                      *
 * SIDI is free software: you can redistribute it       *
 * and/or modify it under the terms of the GNU General  *
 * Public License as published by the Free Software     *
 * Foundation, either version 3 of the License, or      *
 * (at your option) any later version.                  *
 *                                                      *
 * SIDI is distributed in the hope that it will be      *
 * useful, but WITHOUT ANY WARRANTY; without even the   *
 * implied warranty of MERCHANTABILITY or FITNESS FOR   *
 * A PARTICULAR PURPOSE. See the GNU General Public     *
 * License for more details.                            *
 *                                                      *
 * You should have received a copy of the GNU General   *
 * Public License along with SIDI.                      *
 *                                                      *
 * If not, see <http://www.gnu.org/licenses/>.          *
 ********************************************************/

#include <Arduino.h>
#include "SID6581.h"

SID6581::SID6581() {
  int x;
  
  // Make sure our "chip" is completely 0d
  memset( &sidchip, 0, sizeof(sid6581_chip_t) );
  
  // Set up our pins
  pinMode( SID6581_PIN_D0, OUTPUT );
  pinMode( SID6581_PIN_D1, OUTPUT );
  pinMode( SID6581_PIN_D2, OUTPUT );
  pinMode( SID6581_PIN_D3, OUTPUT );
  pinMode( SID6581_PIN_D4, OUTPUT );
  pinMode( SID6581_PIN_D5, OUTPUT );
  pinMode( SID6581_PIN_D6, OUTPUT );
  pinMode( SID6581_PIN_D7, OUTPUT );
  
  pinMode( SID6581_PIN_A0, OUTPUT );
  pinMode( SID6581_PIN_A1, OUTPUT );
  pinMode( SID6581_PIN_A2, OUTPUT );
  pinMode( SID6581_PIN_A3, OUTPUT );
  pinMode( SID6581_PIN_A4, OUTPUT );
  
  pinMode( SID6581_PIN_RESET, OUTPUT );
  pinMode( SID6581_PIN_RW, OUTPUT );
  pinMode( SID6581_PIN_SEL, OUTPUT );
  
  pinMode( SID6581_PIN_TIMER, INPUT );
}

void SID6581::startClock(void) {
  pinMode( SID6581_PIN_CLOCK, OUTPUT );
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 7;
  TCCR1A |= (1 << COM1A0);
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS10);
  
  delayMicroseconds(1);
  resetChip();
}

void SID6581::waitCycle(void) {
  /*uint8_t foo = digitalRead( SID6581_PIN_TIMER );

  while( foo != HIGH ) {
    Serial.println("Delay1");
    foo = digitalRead( SID6581_PIN_TIMER );
  }
  
  foo = digitalRead( SID6581_PIN_TIMER );
  while( foo == HIGH ) {
    Serial.println("Delay2");    
    foo = digitalRead( SID6581_PIN_TIMER );
  }*/
  delayMicroseconds(2);
}

/*********************************************************************
 Writes can only happen when SEL is low and a clock strobe occurs.
 We keep SEL high to avoid any unintended writes.
 *********************************************************************/
void SID6581::writeData(void) {
  waitCycle();
  
  // Ensure chip is in WRITE mode
  digitalWrite( SID6581_PIN_RW, LOW );
  
  // Select chip
  digitalWrite( SID6581_PIN_SEL, LOW );
  
  // Wait for a high to come and go
  waitCycle();
  
  // Deselect chip
  digitalWrite( SID6581_PIN_SEL, HIGH );
}

/*********************************************************************
 Reset the SID6581 chip
 *********************************************************************/
void SID6581::resetChip(void) {
  int x = 10;
  
  // Bring reset high
  digitalWrite( SID6581_PIN_RESET, HIGH );
  
  // HIGH for 2 cycles
  x = 2;
  while( x > 0 ) {
    waitCycle();
    x--;
  }
  
  digitalWrite( SID6581_PIN_RESET, LOW );
  
  // Bring reset LOW for 10 clock cycles
  x = 10;
  while( x > 0 ) {
    waitCycle();
    x--;
  }
  
  digitalWrite( SID6581_PIN_RESET, HIGH );
}


// doctea to REALLY reset parameters on the ARMSID to something that works
void SID6581::resetFilter() {
  sidchip.filter.resfilt = B00000000 | (SID6581_MASK_FLT_V3 | SID6581_MASK_FLT_V2 | SID6581_MASK_FLT_V1 ) ;
    
  // Update immediately
  setAddress( SID6581_REG_RFLT );
  setData( sidchip.filter.resfilt);
  writeData();

  sidchip.filter.modevol = (SID6581_MASK_FLT_BP | B00001111);
  setAddress( SID6581_REG_MVOL );
  setData( sidchip.filter.modevol );
  writeData();
  
}



/*********************************************************************
 Set a certain address on our A0 ... A4 pins
 *********************************************************************/
void SID6581::setAddress( uint16_t what ) {
  digitalWrite( SID6581_PIN_A0, CHECK_BIT(what,1)?HIGH:LOW );
  digitalWrite( SID6581_PIN_A1, CHECK_BIT(what,2)?HIGH:LOW );
  digitalWrite( SID6581_PIN_A2, CHECK_BIT(what,4)?HIGH:LOW );
  digitalWrite( SID6581_PIN_A3, CHECK_BIT(what,8)?HIGH:LOW );
  digitalWrite( SID6581_PIN_A4, CHECK_BIT(what,16)?HIGH:LOW );

}

/*********************************************************************
 Set a certain value on our D0 ... D7 pins
 *********************************************************************/
void SID6581::setData( uint16_t what ) {
  digitalWrite( SID6581_PIN_D0, CHECK_BIT(what,1)?HIGH:LOW );
  digitalWrite( SID6581_PIN_D1, CHECK_BIT(what,2)?HIGH:LOW );
  digitalWrite( SID6581_PIN_D2, CHECK_BIT(what,4)?HIGH:LOW );
  digitalWrite( SID6581_PIN_D3, CHECK_BIT(what,8)?HIGH:LOW );
  digitalWrite( SID6581_PIN_D4, CHECK_BIT(what,16)?HIGH:LOW );
  digitalWrite( SID6581_PIN_D5, CHECK_BIT(what,32)?HIGH:LOW );
  digitalWrite( SID6581_PIN_D6, CHECK_BIT(what,64)?HIGH:LOW );
  digitalWrite( SID6581_PIN_D7, CHECK_BIT(what,128)?HIGH:LOW );
 
}
 
void SID6581::setFrequency( int voice, uint16_t frequency ) {
  sidchip.voices[voice].frequency = frequency;
}

void SID6581::updateVoiceFrequency( int which ) {
  uint8_t hi, lo;
  uint16_t freq = sidchip.voices[which].frequency;
  
  switch( which ) {
    case 0:
      hi = SID6581_REG_F1HI;
      lo = SID6581_REG_F1LO;
      break;
      
     case 1:
       hi = SID6581_REG_F2HI;
       lo = SID6581_REG_F2LO;
       break;
       
     case 2:
       hi = SID6581_REG_F3HI;
       lo = SID6581_REG_F3LO;
       break;
       
    default:
      return;
  }
  
  // First, we set low
  setAddress( lo );
  setData( freq );
  writeData();
  
  // Now, we set high
  setAddress( hi );
  setData( freq>>8 );
  writeData();
}

void SID6581::setPulseWidth( int voice, uint16_t width ) {
  uint8_t hi, lo;
  
  sidchip.voices[voice].width = width;
  
  switch( voice ) {
    case 0:
      hi = SID6581_REG_P1HI;
      lo = SID6581_REG_P1LO;
      break;
      
    case 1:
      hi = SID6581_REG_P2HI;
      lo = SID6581_REG_P2LO;
      break;
      
    case 2:
      hi = SID6581_REG_P3HI;
      lo = SID6581_REG_P3LO;
      break;
      
    default:
      return;
  }
  
  setAddress( lo );
  setData( width );
  writeData();
  
  setAddress( hi );
  setData( (width >> 8) & B00001111 );
  writeData();
      
}

void SID6581::setEnvelope( int voice, uint8_t atk, uint8_t dec, uint8_t sus, uint8_t rel ) {
  sidchip.voices[voice].ad = 0;
  //release & B00001111) | (sustain << 4

  dec = dec / 8;
  atk = atk / 8;
  
  sidchip.voices[voice].ad = (dec & B00001111) | (atk << 4);

  rel = rel / 8;
  sus = sus / 8;
                             
  sidchip.voices[voice].sr = 0;
  sidchip.voices[voice].sr = (rel & B00001111) | (sus << 4);
}

void SID6581::updateEnvelope( int which ) {
  uint8_t ad,sr;
  
  switch( which ) {
    case 0:
      ad = SID6581_REG_AD1;
      sr = SID6581_REG_SR1;
      break;
      
    case 1:
      ad = SID6581_REG_AD2;
      sr = SID6581_REG_SR2;
      break;
      
    case 2:
      ad = SID6581_REG_AD3;
      sr = SID6581_REG_SR3;
      break;
  
    default:
      return;
  }

  // First, we write AD
  setAddress( ad );
  setData( sidchip.voices[which].ad );
  writeData();

  // Next, our SR
  setAddress( sr );
  setData( sidchip.voices[which].sr );  
  writeData();
}

void SID6581::voiceOn( int which ) {
  sidchip.voices[which].control |= B00000001;
  updateControl(which);
}

void SID6581::voiceOff( int which ) {
  sidchip.voices[which].control &= B11111110;
  updateControl(which);
}
  
void SID6581::setShape( int which, uint8_t shape ) {
  // Clear other shapes in high bits
  sidchip.voices[which].control &= B00001111;
  sidchip.voices[which].control |= shape;
  updateControl(which);
}

void SID6581::setShape2( int which, byte b, bool state) {
  if (state && !(b & sidchip.voices[which].control))
    sidchip.voices[which].control += b;
  else if (!state && (b & sidchip.voices[which].control))
    sidchip.voices[which].control -= b;  

  updateControl(which);

}

void SID6581::setSync( int which, int state ) {
  sidchip.voices[which].control &= B11111101;
  
  if( state )
    sidchip.voices[which].control  += 2;
    
  updateControl(which);
}

void SID6581::setRing( int which, int state ) {
  sidchip.voices[which].control &= B11111011;
  
  if( state )
    sidchip.voices[which].control += 4;

  updateControl(which);
}

void SID6581::updateControl( int which ) {
  uint8_t reg;
  
  switch( which ) {
    case 0:
      reg = SID6581_REG_CTL1;
      break;
      
    case 1:
      reg = SID6581_REG_CTL2;
      break;
      
    case 2:
      reg = SID6581_REG_CTL3;
      break;
      
    default:
      return;
  }
  
  // Set control register & data
  setAddress( reg );
  setData( sidchip.voices[which].control );
  writeData();
}

void SID6581::setVolume( uint8_t vol ) {
  sidchip.filter.modevol &= B11110000;
  sidchip.filter.modevol |= (vol & B00001111);
  
  // Update immediately
  setAddress( SID6581_REG_MVOL );
  setData( sidchip.filter.modevol );
  writeData();
}


void SID6581::setCutoff( uint8_t vol ) {
  //sidchip.filter.cutoff &= B11110000;
  //sidchip.filter. |= (vol & B00001111);

  sidchip.filter.frequency = vol * 16;
  
  // Update immediately
  setAddress( SID6581_REG_FCLO );
  setData( (sidchip.filter.frequency ) ); // & B00001111);
  writeData();

  setAddress( SID6581_REG_FCHI );
  setData( (sidchip.filter.frequency)>>4 ); // & B11111111);  //setData( (width >> 8) & B00001111 );
  writeData();
}

void SID6581::modulateCutoff( float mod ) {
  //mod = mod; //mod >> 4;
  // Update immediately
  setAddress( SID6581_REG_FCLO );
  setData( (sidchip.filter.frequency * mod ) ); // & B00001111);
  writeData();

  setAddress( SID6581_REG_FCHI );
  setData( (int)(sidchip.filter.frequency * mod)>>4 ); // & B11111111);  //setData( (width >> 8) & B00001111 );
  writeData();  
}

void SID6581::setFilterOn (int chan, bool status) {
  if (status) {
    sidchip.filter.resfilt |= 1<<chan;
  } else {
    sidchip.filter.resfilt &= ~(1<<chan);
  }

  setAddress ( SID6581_REG_RFLT );
  setData(sidchip.filter.resfilt);
  writeData();
}


void SID6581::setResonance( uint8_t vol ) {
  sidchip.filter.resfilt &= B00001111;  // clear high bits
  sidchip.filter.resfilt |= ((vol/8)<<4) & B11110000; // set high bits
  
  // Update immediately
  setAddress( SID6581_REG_RFLT );
  setData( sidchip.filter.resfilt);
  writeData();
}



void SID6581::setFilter( byte b, bool state) {
  if (state && !(b & sidchip.filter.modevol))
    sidchip.filter.modevol += b;
  else if (!state && (b & sidchip.filter.modevol))
    sidchip.filter.modevol -= b;  

  setAddress( SID6581_REG_MVOL );
  setData( sidchip.filter.modevol );
  writeData();  
}


// only sets one filter mode at a time, deprecated
void SID6581::setFilterMode(int mode) {
  sidchip.filter.modevol &= B00001111;
  sidchip.filter.modevol |= mode;

  setAddress( SID6581_REG_MVOL );
  setData( sidchip.filter.modevol );
  writeData();  
}

SID6581 SID = SID6581();
