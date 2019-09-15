/********************************************************
 *          ______                                      *
 *    _____|\     \       ____                 _____    * 
 *   /     / |     |  ____\_  \__         _____\    \   *
 *  |      |/     /| /     /     \       /    / \    |  *
 *  |      |\____/ |/     /\      |     |    |  /___/|  *
 *  |\     \    | /|     |  |     |  ____\    \ |   ||  *
 *  | \     \___|/ |     |  |     | /    /\    \|___|/  *
 *  |  \     \     |     | /     /||    |/ \    \       *
 *    \ |     |    | \_____\   | / | |   ||    | |      *
 *     \|_____|     \ |    |___|/   \|___||____|/       *
 *                   \|____|                            *
 *                                                      *
 ********************************************************
 *            SIDI: THE MIDI SID INTERFACE              *
 *            (c)2013-2015 Phar Out Studios             *
 *------------------------------------------------------*
 * This is SIDI's main Arduino sketch. It handles       *
 * including and configuring everything we're going to  *
 * need to drive the chip and receives MIDI events over *
 * the hardware serial port and converts them in to     *
 * things that the SID chip understands.                *
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

#include "SIDINotes.h"
#include "SID6581.h"

/** Current note for all 3 channels */
unsigned long curNote[3] = { 0, 0, 0 };

unsigned long lastNote[3] = { 0, 0, 0 };

unsigned long voice_porta[3] = {
  120, 120, 60
};

unsigned long voice_portamicros[3] = {
  0, 0, 0
};

int voice_octave[3] = {
  0,0,0
};

float voice_detune[3] = {
  1, 1, 1
};

bool poly = false;

/**
 * Set up the serial port and start the SID chip's
 * clock running. This will also reset the chip.
 */
void setup() {
  Serial.begin(38400);
  SID.startClock();
    
  for( int x=0;x<3;x++ ) {
    //SID.setEnvelope( x, 0, 0, 15, 0 );
    SID.setEnvelope( x, 1, 1, 15, 5 );
    SID.updateEnvelope(x);
    
    SID.setShape( x, SID6581_MASK_SQUARE );
    SID.setPulseWidth( x, 2048 );
  }

  //SID.setFilter(SID6581_MASK_FLT_EXT,false);  // 

  SID.setFilterMode(1);

  SID.resetFilter();
  

  test_voice();
}

/**
 * Spin continuously at Serial.read() until it returns something.
 * Potentially very dangerous.
 */
int forceRead() {
  int ret;
  
  do {
    ret = Serial.read();
  } while( ret == -1 );
  
  return ret;
}

/**
 * If there's data available on the serial port, read
 * it and act appropriately.
 *
 * If it's time to update the notes being played in an
 * arpeggio, do so.
 */

void test_voice() {
 int evt;
  /*while( Serial.available() > 0 ) {
        evt = forceRead();
  }*/


  //SID.resetFilter(); //resetChip();
  
  /*sidchip.filter.resfilt = B11111111; // set high bits
  
  // Update immediately
  setAddress( SID6581_REG_RFLT );
  setData( sidchip.filter.resfilt);
  writeData();*/

  for (int i = 0 ; i< 3 ; i++) {
              SID.setVolume(4);
              SID.setFrequency( i, sidinote[35] );
              SID.updateVoiceFrequency( i );
              
              //if( curNote[chan] == 0 )
                SID.voiceOn(i);
                delay(200);
                SID.voiceOff(i);
                //delay(200);

            SID.setVolume(15);
              SID.setFrequency( i, sidinote[60] );
              SID.updateVoiceFrequency( i );

              delay(500);

              SID.voiceOff(i);

              delay(500);
  }

              //SID.setFilterOn(SID6581_MASK_FLT_EXT , true);
                
}

#define MIDI_NOTE_OFF 0x80
#define MIDI_NOTE_ON 0x90
#define MIDI_AFTERTOUCH 0xA0
#define MIDI_CC 0xB0
#define MIDI_PATCH 0xC0 
#define MIDI_CHANNEL_PRESSURE 0xD0 
#define MIDI_PITCHBEND 0xE0
#define MIDI_SYSEX 0xF0 
#define MIDI_VEND_ARDSID 0x7D
#define MIDI_SYSEX_END 0xF7 

float getPortaAdjust(int chan, int note) {
  if (note==lastNote[chan]) 
    return 0;
    
  long freqdiff = 
    (long)(sidinote[voice_octave[chan] + lastNote[chan]]+voice_detune[chan]) -
    (long)(sidinote[voice_octave[chan] + note]+voice_detune[chan])  
    ;

    freqdiff = abs(freqdiff);
    
  //return freqdiff * (voice_porta[chan]/127) * (micros()-voice_portamicros[chan]);
  //return random(freqdiff);
  //return (freqdiff * ((micros()-voice_portamicros[chan]))) /freqdiff;
  //sysex(5,(int)freqdiff/10);
  if (voice_portamicros[chan]==0) 
    return 0;

  float porta_factor = (1000/127);
    
  long elapsed = ((micros()-voice_portamicros[chan])/1000);
  if (elapsed < (voice_porta[chan]*porta_factor) ) {
    //elapsed = (voice_porta[chan]*5)-elapsed;
    float a = ((float)elapsed/(float)(voice_porta[chan]*porta_factor)); // * freqdiff;// * (((micros()-voice_portamicros[chan])) / voice_porta[chan]); // (freqdiff/120);
    //if (a<=(-1*freqdiff)) a = -1;
    //if (a>=freqdiff) a = 1;
    //a = constrain(a,-1.0,1.0);
    //return (/*-1.0**/freqdiff) * ((float)elapsed/(float)(voice_porta[chan]*porta_factor));
    //if (note < lastNote[chan]) return freqdiff*-1;
    return freqdiff * a;///12;
    /*unsigned long elapsed = (micros()-voice_portamicros[chan]);
    //if (elapsed > voice_porta[chan]) {
      float a = (voice_porta[chan]/elapsed)*1000; //voice_porta[chan]*(voice_porta[chan]/elapsed);///voice_porta[chan];
      return freqdiff/a;
    //}
    //float a = freqdiff/;
    //return a;*/
  }
  return 0;
}

uint16_t getVoiceFrequency(int chan, int note) {
   int16_t f = (sidinote[voice_octave[chan] + note] + voice_detune[chan]);

   int16_t pa = getPortaAdjust(chan, note);
   if (pa>=0) {
    if (note < lastNote[chan]) {
      return f - (pa>>4);
    } else if (note > lastNote[chan]) {
      return f + (pa>>4);
    }
   }
   return f;
   /*if (pa>0) 
    return f + pa;
   else if (pa<0) {
    return f - pa;
   } else {
    return f;
   } */    
}


void updatePorta() {
  for (int chan = 0 ; chan < 3 ; chan++) {
    //int porta_amount = (micros() - voice_portamicros[chan]) / (voice_porta[chan]*8);
    if (/*porta_amount!=0 && */curNote[chan]!=0) {
      SID.setFrequency(chan, getVoiceFrequency(chan,curNote[chan]));
      SID.updateVoiceFrequency( chan );
    }
  }
}

void playNote(int chan, int note) {

  lastNote[chan] = curNote[chan];
  
  SID.setFrequency( 
    chan, getVoiceFrequency(chan, note)
  );
  SID.updateVoiceFrequency( chan );
  
  if( curNote[chan] == 0 ) {
    SID.voiceOn(chan);
    voice_portamicros[chan] = 0;
  } /*else {
    SID.voiceOff(chan);
    //SID.voiceOn(chan);
  }*/ else {
    voice_portamicros[chan] = micros();
  }

  //lastNote[chan] = curNote[chan];
  curNote[chan] = note;
}


void loop() {
  int evt, chan, note, vel, foo;

  //void LFOupdate(bool retrig, byte mode, float FILtop, float FILbottom);
  LFOupdate(false, 0/*LFOmodeSelect*/, SID.sidchip.filter.frequency); // LFOdepth);

  updatePorta();
  
  while( Serial.available() > 0 ) {
    evt = forceRead();
    
    chan = evt & 0x0f;
    evt &= 0xf0;
    
    switch( evt & 0xf0 ) {
      case MIDI_NOTE_OFF: //0x80:
      case MIDI_NOTE_ON: //0x90:
        note = forceRead();
        vel = forceRead();
        if( /*chan < 3 &&*/ sidinote[note] != 0 ) {
          if (poly) { 
            if (chan==0) { 
              // new roundrobin polyopt mode 
              static int instance;
              // ignore channel
              if( evt == MIDI_NOTE_OFF || vel == 0 ) {
                // turn off any channels that are playing this note
                for (int i = 0 ; i < 3 ; i++) {
                  if( curNote[i] == note ) {
                    SID.voiceOff(i);
                    curNote[i] = 0;
                    instance--;
                    //break;
                  }
                }
              } else {
                // pick a new channel to play this note on 
                //chan = instance%3;
                bool already_playing = false;
                for (int i = 0 ; i < 3 ; i++) {
                  if (curNote[i] == note) {
                    already_playing = true;
                  } else if (curNote[i] == 0) {
                    chan = i;
                    break;
                  }
                }
                if( !already_playing) { //curNote[chan] != note ) { // not already playing note 
                  playNote(chan, note);

                  //SID.setVolume(vel>>4);

                  LFOupdate(true, 0/*LFOmodeSelect*/, SID.sidchip.filter.frequency); //, LFOdepth);
                  
                  instance++;
                }
              }              
            } else {
              // poly mode - play voices individually by channel
              chan %= 3;
              if( evt == MIDI_NOTE_OFF || vel == 0 ) {
                if( curNote[chan] == note ) {
                  SID.voiceOff(chan);
                  curNote[chan] = 0;
                }
              } else {
                if( curNote[chan] != note ) {
                  playNote(chan, note);

                  LFOupdate(true, 0/*LFOmodeSelect*/, SID.sidchip.filter.frequency); //, LFOdepth);
                }
              }
            }
          } else {  
            chan %= 3;
            // mono mode - play all voices simultaneously
            if( evt == MIDI_NOTE_OFF || vel == 0 ) {  // note off - stop all voices
              for (chan = 0 ; chan < 3 ; chan++) {
                if( curNote[chan] == note ) {
                  SID.voiceOff(chan);
                  //SID.voiceOff(1);
                  //SID.voiceOff(2);
                  curNote[chan] = 0;
                  //curNote[1] = 0;
                  //curNote[2] = 0;
                }
              }
            } else {
              if( curNote[0] != note ) {
                LFOupdate(true, 0/*LFOmodeSelect*/, SID.sidchip.filter.frequency); //, LFOdepth);
                //curNote[chan] = note;

                for (chan = 0 ; chan < 3 ; chan++) {
                  playNote(chan, note);
                }
              }
            }
          } // todo: third mode where we re-use notes intelligently

          /*            if( evt == MIDI_NOTE_OFF || vel == 0 ) {
              if( curNote[chan] == note ) {
                SID.voiceOff(chan);
                curNote[chan] = 0;
              }
            } else {
              if( curNote[chan] != note ) {
                SID.setFrequency( chan, sidinote[note] + voice_detune[chan]-64);
                SID.updateVoiceFrequency( chan );
                //SID.setVolume(vel>>4);
                
                if( curNote[chan] == 0 )
                  SID.voiceOn(chan);
                  
                curNote[chan] = note;
              }
            }
          */
            
        }

        break;
        
      case MIDI_AFTERTOUCH: // 0xA0
        //todo: make assignable?  effect filter etc
        forceRead();  // key
        forceRead();  // touch
        
        break;
      case 0xB0: //MIDI_CC: // 0xB0
        evt = forceRead();  // controller #
        vel = forceRead();

        decodeCC(chan, evt, vel);
        
        break;
        
      case MIDI_PATCH: //0xC0:
        forceRead();
        break;
        
      case MIDI_CHANNEL_PRESSURE: //0xD0:
        forceRead();
        break;
        
      case MIDI_PITCHBEND: //0xE0:
        //todo: wobble global pitch
        forceRead();
        forceRead();
        break;
        
        
      /*case MIDI_SYSEX: //0xF0:
        chan = forceRead();
        
        //if( chan == 0x7D ) {
          //chan = forceRead();

        do {
          evt = Serial.read();
        } while( evt != MIDI_SYSEX_END ); // 0xF7 

        break;
        */
        
      default:
        break;
    }
  }
  
}
