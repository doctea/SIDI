// Teensy-Synth Part 8
// LFO Test
// By Notes and Volts
// www.notesandvolts.com

const float DIV127 = (1.0 / 127.0);

unsigned int LFOspeed = 2000;
float LFOpitch = 1;
float LFOdepth = 1;
byte LFOmodeSelect = 1;

int FILfreq =  10000;
float FILfactor = 1;

// main loop
//  LFOupdate(false, LFOmodeSelect, FILfactor, LFOdepth);

// note on
//     LFOupdate(true, LFOmodeSelect, FILfactor, LFOdepth);

// pitch lfo
//waveform1.frequency(noteFreqs[note] * bendFactor * LFOpitch);


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
          LFOdepth = value * DIV127;
          break;
    
    // change lfo mode
        case MIDI_CC_LFO_MODE:
          LFOmodeSelect = value;
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
  static bool LFOstop = false;
  static float LFOrange = 0;
  static byte oldMode = 0;
  static bool retriggered = false;
  
  float FILbottom = LFOdepth;

  if (retrig == true) retriggered = true;

  mode = LFOmodeSelect;

  if (currentMicros - LFOtime >= LFOspeed) {
    int delta = (currentMicros - LFOtime)/LFOspeed;
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

    //LFOrange = 0.5-LFOdepth; //DIV127 * (64 - LFOdepth);
    //LFOdepth = 1;

    // LFO Modes
    switch (mode) {

      case 0: //Filter OFF
        SID.modulateCutoff(1);
        return;
        break;
      case 1: //Filter FREE
        SID.modulateCutoff(0.5+((LFOrange * LFO) + LFOdepth));
        break;
      case 2: //Filter DOWN
        if (retriggered == true) {
          LFOdirection = true;
          LFO = 1.0;
        }
        SID.modulateCutoff(0.5+((LFOrange * LFO) + LFOdepth));
        break;
      case 3: //Filter UP
        if (retriggered == true) {
          LFOdirection = false;
          LFO = 0;
        }
        SID.modulateCutoff(0.5+((LFOrange * LFO) + LFOdepth));
        break;
      case 4: //Filter 1-DN
        if (retriggered == true) {
          LFOstop = false;
          LFOdirection = true;
          LFO = 1.0;
        }
        if (LFOstop == false) SID.modulateCutoff(0.5+((LFOrange * LFO) + LFOdepth));
        break;
      case 5: //Filter 1-UP
        if (retriggered == true) {
          LFOstop = false;
          LFOdirection = false;
          LFO = 0;
        }
        if (LFOstop == false) SID.modulateCutoff(0.5+((LFOrange * LFO) + LFOdepth));
        break;
      case 8: //Pitch OFF
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
        break;
    }

    retriggered = false;

    // Update LFO
    if (LFOdirection == false) { //UP
      //LFO = (LFO + 0.01);
      LFO = (LFO + (0.01*delta));
      if (LFO >= 1) {
        LFOdirection = true;
        LFOstop = true;
      }
    }

    if (LFOdirection == true) { //Down
      //LFO = (LFO - 0.01);
      LFO = (LFO - (0.01*delta));
      if (LFO <= 0) {
        LFOdirection = false;
        LFOstop = true;
      }
    }
  }
}

  
