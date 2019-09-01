// Offcuts from SIDI to start cleaning it up, non-working Arpeggiator

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
        if( curNote[chan] != note ) {
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



  // ARPEGGIOS
  /*
  if( millis() - lastarp > arate ) {
    update_arpeggios();
    lastarp = millis();
  }
  */
