#define MCB 16
#define PPV 12 // params per voice

#define MIDI_CC_ATK MCB+0
#define MIDI_CC_DEC MCB+1
#define MIDI_CC_SUS MCB+2
#define MIDI_CC_REL MCB+3
#define MIDI_CC_PW_HI MCB+4
#define MIDI_CC_PW_LO MCB+5
#define MIDI_CC_DETUNE MCB+6

#define MIDI_CC_SHAPE MCB+7

#define MIDI_CC_FILTER MCB+8


/*#define MIDI_CC_1_ATK MCB+PPV+0
#define MIDI_CC_1_DEC MCB+PPV+1
#define MIDI_CC_1_SUS MCB+PPV+2
#define MIDI_CC_1_REL MCB+PPV+3
#define MIDI_CC_1_PW_HI MCB+PPV+4
#define MIDI_CC_1_PW_LO MCB+PPV+5

#define MIDI_CC_2_ATK MCB+PPV+PPV+0
#define MIDI_CC_2_DEC MCB+PPV+PPV+1
#define MIDI_CC_2_SUS MCB+PPV+PPV+2
#define MIDI_CC_2_REL MCB+PPV+PPV+3
#define MIDI_CC_2_PW_HI MCB+PPV+PPV+4
#define MIDI_CC_2_PW_LO MCB+PPV+PPV+5*/

#define MIDI_CC_UNISON    MCB+PPV+0 /*+PPV+PPV+0*/

#define MIDI_CC_CUTOFF    MCB+PPV+1
#define MIDI_CC_RESONANCE MCB+PPV+2
#define MIDI_CC_FILTMODE  MCB+PPV+3

#define V_A 0
#define V_D 1
#define V_S 2
#define V_R 3

int voice_adsr[][5] = {
   { 0, 1, 15, 5 },
   { 0, 1, 15, 5 },
   { 0, 1, 15, 5 },
};

int voice_pw_hi[3] = {
  0, 0, 0
};
int voice_pw_lo[3] = {
  16, 16, 16
};

/*int voice_detune[3] = {
  64, 64 ,64
};*/


void decodeCC( int chan, byte controller, byte value ) {
  bool update_env = false;

  //chan-=1;

  switch (controller) {
    case MIDI_CC_ATK:
      voice_adsr[chan][V_A] = value;
      update_env = true;
      break;

   case MIDI_CC_DEC:
      voice_adsr[chan][V_D] = value;
      update_env = true;
      break;

   case MIDI_CC_SUS:
      voice_adsr[chan][V_S] = value;
      update_env = true;
      break;

   case MIDI_CC_REL:
      voice_adsr[chan][V_R] = value;
      update_env = true;
      break;
      
    case MIDI_CC_PW_LO:
      voice_pw_lo[chan] = value;
      SID.setPulseWidth(chan, voice_pw_lo[chan]<<4); // & voice_pw_hi[chan]<<12); //value<<4);
      break;

    /*case MIDI_CC_PW_HI:
      voice_pw_hi[chan] = value;
      SID.setPulseWidth(chan, voice_pw_lo[chan]<<4 & voice_pw_hi[chan]<<12); //value<<12);
      break;*/

    case MIDI_CC_DETUNE:

      //float range = sidinote[curNote[chan+1]] - sidinote[curNote[chan-1]];
      //float adjust = range/(value-64);

      voice_detune[chan] = value;
    
      SID.setFrequency(chan, sidinote[curNote[chan]] + voice_detune[chan]-64); //(int)adjust); //range/((127/64-value)));
      SID.updateVoiceFrequency(chan);

      break;
  
     case MIDI_CC_SHAPE:
      if (value<32) {
              SID.setShape (chan, SID6581_MASK_SQUARE);
      } else if (value < 64) {
            SID.setShape (chan, SID6581_MASK_SAWTOOTH);
      } else if (value < 96) {
            SID.setShape (chan, SID6581_MASK_SQUARE);
      } else {
            SID.setShape (chan, SID6581_MASK_NOISE);
      }
      break;

      case MIDI_CC_CUTOFF:
          SID.setCutoff(value);

      break;
      
      case MIDI_CC_RESONANCE:
          SID.setResonance(value);
          break;

      case MIDI_CC_FILTER:
          SID.setFilter(chan, value==127);
          break;

      case MIDI_CC_FILTMODE:
        if (value < 32) {
              SID.setFilterMode (16); //SID6581_MASK_FLT_LP);
        } else if (value < 64) {
              SID.setFilterMode (32); //SID6581_MASK_FLT_BP);
        } else if (value < 96) {
              SID.setFilterMode (64); //SID6581_MASK_FLT_HP);
        } else {
              SID.setFilterMode (128);//SID6581_MASK_FLT_MUTEV3);
        }
      break;

      break;
  }

    if (update_env) {
      SID.setEnvelope( chan, voice_adsr[chan][V_A], voice_adsr[chan][V_D], voice_adsr[chan][V_S], voice_adsr[chan][V_R]);
      SID.updateEnvelope(chan);
    }
  }
        /*if (chan==0 && controller==1) {  // evt 
              //value = forceRead();  // controller value

              SID.setEnvelope( chan, value, 1, value, 5 );
              SID.updateEnvelope(chan);
              
              //SID.setFrequency(chan, sidinote[value]);
              //SID.updateVoiceFrequency(chan);
              //delay(2);
              //test_voice();
        } else if (chan==1 && evt==1) { //evt==2) {
              //value = forceRead();  // controller value
         
              SID.setEnvelope( chan, 0, 1, 15, value );
              SID.updateEnvelope(chan);

              SID.setPulseWidth(chan, value<<4);
               
              //SID.setFrequency(chan, sidinote[value]);
              //SID.updateVoiceFrequency(chan);
        }
        //delay(2);
*/
