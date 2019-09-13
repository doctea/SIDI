// Teensy-Synth Part 8
// LFO Test
// By Notes and Volts
// www.notesandvolts.com

const float DIV127 = (1.0 / 127.0);

unsigned int LFOspeed = 2000;
float LFOpitch = 1;
float LFOdepth = 1;
float LFOattack = 0;
byte LFOmodeSelect = 1;

int FILfreq =  10000;
float FILfactor = 1;

// main loop
//  LFOupdate(false, LFOmodeSelect, FILfactor, LFOdepth);

// note on
//     LFOupdate(true, LFOmodeSelect, FILfactor, LFOdepth);

// pitch lfo
//waveform1.frequency(noteFreqs[note] * bendFactor * LFOpitch);

void sysex(byte v1, byte v2) {
  Serial.write(MIDI_SYSEX);
  Serial.write(v1); //(int)(127 * (LFOrange * LFO) + LFOdepth));
  Serial.write(v2);
  Serial.write(MIDI_SYSEX_END);
}

void decodeCC_lfo(int chan, int controller, int value) {
  switch(controller) {
    // change lfo speed
      case MIDI_CC_LFO_SPEED:
        {
          float xSpeed = value * DIV127;
          xSpeed = pow(100, (xSpeed - 1));
          LFOspeed = (70000 * xSpeed);
          break;
        }
          
    // change lfo depth
        case MIDI_CC_LFO_DEPTH:
          LFOdepth = 2* (value * DIV127);
          break;
    
    // change lfo mode
        case MIDI_CC_LFO_MODE:
          LFOmodeSelect = value;
          break;

        case MIDI_CC_LFO_ATTACK:
          LFOattack = 1-(value * DIV127);
          break;


      case MIDI_CC_PW_HI: // actually pw lfo amt
        //voice_pw_lo[chan] = value;
        SID.voice_pulfactor[chan] = value * DIV127;
        //SID.setPulseWidth(chan, voice_pw_lo[chan]<<4); // & voice_pw_hi[chan]<<12); //value<<4);
        break;

          
  }
}

// change freq by midi cc
  /*case MIDI_CC_:
      FILfactor = value * DIV127;
      FILfreq = 10000 * (value * DIV127);
      if (LFOmodeSelect < 1 || LFOmodeSelect > 5)filter1.frequency(FILfreq);*/

#define oscSet(); /* oscSet(); */


void LFOupdate(bool retrig, byte mode, float FILtop){ //, float FILbottom) {
  static float LFO = 0;
  static unsigned long LFOtime = 0;
  static bool LFOdirection = false;
  unsigned long currentMicros = micros();
  //uint32_t 
  
  static unsigned long retrigMicros = 0;
  static bool LFOstop = false;
  static float LFOrange = 0;
  static byte oldMode = 0;
  static bool retriggered = false;
  
  float FILbottom = LFOdepth;

  if (retrig == true) retriggered = true;

  mode = LFOmodeSelect;

  if (currentMicros - LFOtime >= LFOspeed) {
    //int delta = 1; 
    float delta = (currentMicros - LFOtime)/(LFOspeed);
    LFOtime = currentMicros;

    if (mode != oldMode) {
      if (mode == 0 || mode == 8) {
        LFOpitch = 1;
        //oscSet();
        //filter1.setFrequency(FILfreq);
        SID.modulateCutoff(1);
      }
      else if (mode >= 1 || mode <= 7) {
        LFOpitch = 1;
        //oscSet();
      }
      else if (mode >= 9 || mode <= 13) {
        //filter1.setFrequency(FILfreq);
        SID.modulateCutoff(1);
      }
      oldMode = mode;
    }

    LFOrange = (FILtop/16384) - FILbottom;
    //if (LFOrange < 0) LFOrange = 0;

#define bias 1
    //float bias = 1;

    

    // between 0 and 1, how much of the attack period has elapsed?
    // time since retrigMicros s=d/t t=d/s d=t*s
    float t = ((currentMicros-retrigMicros)/(LFOspeed*1000)) ;
    float atkfactor = t * LFOattack;
    if (atkfactor>1) atkfactor = 1;
    if (atkfactor<0) atkfactor = 0;

        // update pulse widths by lfo too..?
    for (int i = 0 ; i < 3 ; i++) {
      //float pulfactor = t * SID.voice_pulfactor[i];// LFO; //constrain(((currentMicros%LFOspeed)/(LFOspeed*1000)),0,1); // * SID.voice_pulfactor[i]);
      float pulfactor = (LFO * SID.voice_pulfactor[i]) * (atkfactor || 1);

      //SID.setPulseWidth(i, ((int)(((currentMicros-retrigMicros)/(LFOspeed*1000))*SID.voice_pulfactor[i] + SID.sidchip.voices[i].width)<<4));
      //SID.setPulseWidth(i,(SID.sidchip.voices[i].width + (int)(LFO*pulfactor*100*SID.voice_pulfactor[i]))); //(LFOtime * SID.voice_pulfactor[i]))<<4);
      SID.setPulseWidth(i,
        SID.sidchip.voices[i].width + 
        (pulfactor*100) //SID.sidchip.voices[i].width))
      ); //+ (uint8_t)(200*(atkfactor*pulfactor))))<<4); //(int)(SID.voice_pulfactor[i]*pulfactor)) << 4); // (1+(pulfactor * 127*SID.voice_pulfactor[i]))); //+ (100 * pulfactor * SID.voice_pulfactor[i]));
    }
    
    // LFO Modes
    switch (mode) {

      case 0: //Filter OFF
        SID.modulateCutoff(1);
        return;
        break;
      case 1: //Filter FREE
        SID.modulateCutoff(bias+((LFOrange * LFO) + LFOdepth));
        break;
      case 2: //Filter DOWN
        if (retriggered == true) {
          retrigMicros = currentMicros;

          LFOdirection = true;
          LFO = 1.0;
        }

        SID.modulateCutoff(atkfactor * (bias+((LFOrange * LFO) + (LFOdepth))));
        //sysex(2,(byte)(int)(LFO*10));
        break;
      case 3: //Filter UP
        if (retriggered == true) {
          retrigMicros = currentMicros;

          LFOdirection = false;
          LFO = 0;
        }
        SID.modulateCutoff(atkfactor * (bias+((LFOrange * LFO) + LFOdepth)));
        //sysex(3,(byte)((int)LFO*10));
        break;
      case 4: //Filter 1-DN
        if (retriggered == true) {
          retrigMicros = currentMicros;
          LFOstop = false;
          LFOdirection = true;
          LFO = 1.0;
        }
        if (LFOstop == false) SID.modulateCutoff(atkfactor * bias+((LFOrange * LFO) + LFOdepth));
        break;
      case 5: //Filter 1-UP
        if (retriggered == true) {
          retrigMicros = currentMicros;
          LFOstop = false;
          LFOdirection = false;
          LFO = 0;
        }
        if (LFOstop == false) SID.modulateCutoff(atkfactor * bias+((LFOrange * LFO) + LFOdepth));
        break;
    }

    retriggered = false;

    // Update LFO
    if (LFOdirection == false) { //UP
      //LFO = (LFO + 0.01);
      LFO = (LFO + (0.01*(float)delta));
      if (LFO >= 1) {
        LFOdirection = true;
        LFOstop = true;
        //sysex(1,0);
      }
    }

    if (LFOdirection == true) { //Down
      //LFO = (LFO - 0.01);
      LFO = (LFO - (0.01*(float)delta));
      if (LFO <= 0) {
        LFOdirection = false;
        LFOstop = true;
        //Serial.write(3);
        //sysex(1,1);
      }
    }
  }
}

  
