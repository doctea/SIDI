// Offcuts from SIDI to start cleaning it up, non-working Arpeggiator

// dont try and compile this file
#ifdef ENABLE_OFFCUTS

/** Arpeggio data struture. Currently broken. */
byte arpeg[3][7] = {
  { 0, -1, -1, -1, -1, -1, -1 },
  { 0, -1, -1, -1, -1, -1, -1 },
  { 0, -1, -1, -1, -1, -1, -1 }
};

/** Milliseconds between notes in an arpeggio */
byte arate = 15;

/** Timestamp of last arpeggio */
unsigned long lastarp = 0;

/** Timestamp last heartbeat sent (disabled) */
unsigned long lastbeat = 0;

/**
 * Sort the notes in the arpeggio structure 
 *
 * @param chan The channel to sort
 */
void sort_arpeggio(int chan) {
  int tmp, done = 0,x;
  
  while( done != 1 ) {
    done = 1;
    for( x=2;x<5;x++ ) {
      if( arpeg[chan][x] > arpeg[chan][x+1] ) {
        tmp = arpeg[chan][x+1];
        arpeg[chan][x+1] = arpeg[chan][x];
        arpeg[chan][x] = tmp;
        done = 0;
      }
    }
  }
}

/**
 * Update the arpeggio's current note for
 * whatever channels it's needed.
 */
void update_arpeggios() {
  int chan,note;
  
  for( chan = 0; chan < 3; chan++ ) {
    if( arpeg[chan][0] == 1 && arpeg[chan][1] != -1 ) {
      while( arpeg[chan][ arpeg[chan][1] ] == -1 && arpeg[chan][1] < 6 ) {
        arpeg[chan][1]++;
      }
      
      if( arpeg[chan][1] != 6 ) {
        note = arpeg[chan][arpeg[chan][1]];
        if( SID.curNote[chan] != note ) {
          SID.setFrequency( chan, sidinote[note] );
          SID.updateVoiceFrequency( chan );
          curNote[chan] = note;
        }
      }
      
      arpeg[chan][1]++;
      if( arpeg[chan][1] >= 6 )
        arpeg[chan][1] = 2;
    }
  }
}



        /*if( arpeg[chan][0] == 1 ) {
          if( evt == 0x90 && vel != 0 ) {
            for(vel=6;vel>1;vel--) {
              if( arpeg[chan][vel] == -1 ) {
                arpeg[chan][vel] = note;
                break;
              }
            }
            
            if( vel != 1 ) {
              sort_arpeggio(chan);
              
              if( vel == 6 ) {
                arpeg[chan][1] = 2;
                curNote[chan] = note;
                SID.setFrequency(chan,sidinote[note]);
                SID.updateVoiceFrequency(chan);
                SID.voiceOn(chan);
              }
            }
          } else {
            for(vel=5;vel>1;vel--) {
              if( arpeg[chan][vel] == note ) {
                break;
              }
            }
            
            if( vel != 1 ) {
              arpeg[chan][vel] = -1;
              sort_arpeggio(chan);
              
              if( vel == 5 ) {
                arpeg[chan][1] = -1;
                curNote[chan] = 0;
                SID.voiceOff(chan);
              }
            }
          }
        } else*/


/*                 // Arpeggio
      case 9:
        chan = forceRead();
        evt = forceRead();
        
        arpeg[chan][0] = evt;
        arpeg[chan][1] = 2;
        
        if( evt == 0 ) {
          for( int pos=2;pos<6;pos++ ) {
            arpeg[chan][pos] = -1;
          }
        } else
          curNote[chan] = 0;
             
          }
          do {
            evt = Serial.read();
          } while( evt != MIDI_SYSEX_END ); // 0xF7 
        } else {
          do {
            evt = Serial.read();
          } while( evt != MIDI_SYSEX_END ); // 0xF7 
        }
        break;*/


  // ARPEGGIOS
  /*
  if( millis() - lastarp > arate ) {
    update_arpeggios();
    lastarp = millis();
  }
  */


  /*
      case MIDI_SYSEX: //0xF0:
        chan = forceRead();
        
        if( chan == 0x7D ) {
          chan = forceRead();
          
          switch( chan ) {
            // Stop a or all voices
            case 0:
              chan = forceRead();
              
              if( chan == 4 ) {
                for( chan=0;chan<3;chan++ ) {
                  SID.voiceOff(chan);
                  curNote[chan] = 0;
                  //arpeg[chan][1] = -1;
                  for( evt=2;evt<6;evt++ )
                    arpeg[chan][evt] = -1;
                    
                }
              } else {
                SID.voiceOff(chan);
                curNote[chan] = 0;
                arpeg[chan][1] = -1;
                for( evt=2;evt<6;evt++ )
                    arpeg[chan][evt] = -1;
              }
              break;
              
            // Update ADSR envelope
            case 1:
              chan = forceRead();
              
              SID.setEnvelope( chan, forceRead(), forceRead(), forceRead(), forceRead() );
              SID.updateEnvelope(chan);
              break;
              
            // Update pulse width
            case 2:
              chan = forceRead();
              evt = forceRead();
              note = forceRead();
              vel = forceRead();
              
              SID.setPulseWidth( chan, (evt << 8) | (note << 4) | vel );
              
              break;
             
            // Update shape 
            case 3:
              chan = forceRead();
              evt = forceRead();
              SID.setShape(chan,evt);
              break;
              
           // Set test
           case 4:
             forceRead();
             forceRead();
             break;
             
           // Set sync
           case 5:
             chan = forceRead();
             evt = forceRead();
             SID.setSync(chan,evt);
             break;
            
           // Update ring
           case 6:
             chan = forceRead();
             evt = forceRead();
             SID.setRing(chan,evt);
             break;
            
           case 7:
             chan = forceRead();
             evt = forceRead();
             
             if( evt ) {
               SID.voiceOn(chan);
             } else {
               SID.voiceOff(chan);              
             }
             break;
            
           // Volume
           case 8:
             chan = forceRead();
             SID.setVolume(chan);
             break;
                  

        break;*/




        /*case 8: //Pitch OFF
        return;
        break;
      case 9: //Pitch FREE
        LFOpitch = (LFO * LFOdepth) + 1;
        oscSet();
        break;
      case 10: //Pitch DOWN
        if (retriggered == true) {
          LFOdirection = true;
          LFO = 1.0;
        }
        LFOpitch = (LFO * LFOdepth) + 1;
        oscSet();
        break;
      case 11: //Pitch UP
        if (retriggered == true) {
          LFOdirection = false;
          LFO = 0;
        }
        LFOpitch = (LFO * LFOdepth) + 1;
        oscSet();
        break;
      case 12: //Pitch 1-DN
        if (retriggered == true) {
          LFOstop = false;
          LFOdirection = true;
          LFO = 1.0;
        }
        if (LFOstop == false) {
          LFOpitch = (LFO * LFOdepth) + 1;
          oscSet();
        }
        break;
      case 13: //Pitch 1-UP
        if (retriggered == true) {
          LFOstop = false;
          LFOdirection = false;
          LFO = 0;
        }
        if (LFOstop == false) {
          LFOpitch = (LFO * LFOdepth) + 1;
          oscSet();
        }
        break;*/



#endif
