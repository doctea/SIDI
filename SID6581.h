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
 * This header defines various pin locations and the    *
 * functionality we can expect from the SID6581 library *
 * that handles actual control of the chip.             *
 *                                                      *
 * For different wiring configurations than I used,     *
 * simply tweak the pin definitions in the top sections *
 * below. Note that if you change the CLOCK pin, you'll *
 * need to make sure it's assigned somewhere the Atmel  *
 * can output a clock, or you'll need to roll your own. *
 *                                                      *
 * This library will also define a global SID instance  *
 * as my current needs don't dictate more than one      *
 * SID chip per Arduino.                                *
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

#ifndef HAVE_SID6581_H
#define HAVE_SID6581_H

#include <Arduino.h>

#define CHECK_BIT(var,pos) ((var) & (pos))

//
// --------------------------------------------------------------------------
// SID6581 DATA PINS
// I have these connected to the Arduino's D0 ... D7
// --------------------------------------------------------------------------
// 
#ifndef SID6581_PIN_D0
#define SID6581_PIN_D0 A5
#endif
#ifndef SID6581_PIN_D1
#define SID6581_PIN_D1 13
#endif
#ifndef SID6581_PIN_D2
#define SID6581_PIN_D2 2
#endif
#ifndef SID6581_PIN_D3
#define SID6581_PIN_D3 3
#endif
#ifndef SID6581_PIN_D4
#define SID6581_PIN_D4 4
#endif
#ifndef SID6581_PIN_D5
#define SID6581_PIN_D5 5
#endif
#ifndef SID6581_PIN_D6
#define SID6581_PIN_D6 6
#endif
#ifndef SID6581_PIN_D7
#define SID6581_PIN_D7 7
#endif

//
// --------------------------------------------------------------------------
// SID6581 ADDRESS PINS
// I have these connected to the Arduino's A0 ... A4
// --------------------------------------------------------------------------
//
#ifndef SID6581_PIN_A0
#define SID6581_PIN_A0 A0
#endif
#ifndef SID6581_PIN_A1
#define SID6581_PIN_A1 A1
#endif
#ifndef SID6581_PIN_A2
#define SID6581_PIN_A2 A2
#endif
#ifndef SID6581_PIN_A3
#define SID6581_PIN_A3 A3
#endif
#ifndef SID6581_PIN_A4
#define SID6581_PIN_A4 A4
#endif

//
// --------------------------------------------------------------------------
// SID6581 CONTROL PINS
// Various pins that do useful things for us
// --------------------------------------------------------------------------
//
#ifndef SID6581_PIN_RESET
#define SID6581_PIN_RESET 8
#endif

#ifndef SID6581_PIN_CLOCK
#define SID6581_PIN_CLOCK 9
#endif

#ifndef SID6581_PIN_RW
#define SID6581_PIN_RW 10
#endif

#ifndef SID6581_PIN_SEL
#define SID6581_PIN_SEL 11
#endif

// This isn't really used anymore...
#ifndef SID6581_PIN_TIMER
#define SID6581_PIN_TIMER 12
#endif

// --------------------------------------------------------------------------
// SID6581 CONTROL REGISTERS 
// --------------------------------------------------------------------------
#define SID6581_REG_F1LO 0
#define SID6581_REG_F1HI 1
#define SID6581_REG_P1LO 2
#define SID6581_REG_P1HI 3
#define SID6581_REG_CTL1 4
#define SID6581_REG_AD1  5
#define SID6581_REG_SR1  6
#define SID6581_REG_F2LO 7
#define SID6581_REG_F2HI 8
#define SID6581_REG_P2LO 9
#define SID6581_REG_P2HI 10
#define SID6581_REG_CTL2 11
#define SID6581_REG_AD2  12
#define SID6581_REG_SR2  13
#define SID6581_REG_F3LO 14
#define SID6581_REG_F3HI 15
#define SID6581_REG_P3LO 16 
#define SID6581_REG_P3HI 17
#define SID6581_REG_CTL3 18
#define SID6581_REG_AD3  19
#define SID6581_REG_SR3  20
#define SID6581_REG_FCLO 21
#define SID6581_REG_FCHI 22
#define SID6581_REG_RFLT 23
#define SID6581_REG_MVOL 24
#define SID6581_REG_POTX 25
#define SID6581_REG_POTY 26
#define SID6581_REG_OSC3 27
#define SID6581_REG_ENV3 28

// --------------------------------------------------------------------------
// SID6581 VOICE CONTROL REGISTER BITMASK
// --------------------------------------------------------------------------
#define SID6581_MASK_OFF 0
#define SID6581_MASK_GATE 1
#define SID6581_MASK_SYNC 2
#define SID6581_MASK_RING 4
#define SID6581_MASK_TEST 8
#define SID6581_MASK_TRIANGLE 16
#define SID6581_MASK_SAWTOOTH 32
#define SID6581_MASK_SQUARE 64
#define SID6581_MASK_NOISE 128


#define SID6581_MASK_FLT_V1 1
#define SID6581_MASK_FLT_V2 2
#define SID6581_MASK_FLT_V3 4
#define SID6581_MASK_FLT_EXT 8

#define SID6581_MASK_FLT_MODE_MUTEV3 128
#define SID6581_MASK_FLT_MODE_HP 64
#define SID6581_MASK_FLT_MODE_BP 32
#define SID6581_MASK_FLT_MODE_LP 16

//
// --------------------------------------------------------------------------
// SID6581 DATA STRUCTURES
// How you'd describe the 6581 SID chip to C
// --------------------------------------------------------------------------
//
typedef struct {
  uint16_t frequency;
  uint16_t width;
  uint8_t control;
  uint8_t ad;
  uint8_t sr;
} sid6581_voice_t;

typedef struct {
  uint16_t frequency;
  uint8_t resfilt;
  uint8_t modevol;
} sid6581_filter_t;

typedef struct {
  uint8_t potx;
  uint8_t poty;
  uint8_t rnd;
  uint8_t env;
} sid6581_misc_t;

typedef struct {
  sid6581_voice_t voices[3];
  sid6581_filter_t filter;
  sid6581_misc_t misc;
} sid6581_chip_t;

//
// --------------------------------------------------------------------------
// C++ CLASS DEFINITION
// --------------------------------------------------------------------------
//

class SID6581 {
  public:
    sid6581_chip_t sidchip;
    
    SID6581();
    
    void startClock(void);
    void waitCycle(void);
    void writeData(void);
    void resetChip(void);
    
    // Voice manipulation
    void setFrequency( int voice, uint16_t frequency );
    void updateVoiceFrequency( int which );
    void setEnvelope( int voice, uint8_t atk, uint8_t sus, uint8_t dec, uint8_t rel );
    void updateEnvelope( int which );
    void setShape( int which, uint8_t shape );
    void updateControl( int which );
    void voiceOn( int which );
    void voiceOff( int which );
    void setPulseWidth( int voice, uint16_t width );
    void setSync(int which, int state);
    void setRing(int which, int state);
    
    void setVolume( uint8_t vol );
    void setCutoff ( uint8_t vol);
    void setResonance ( uint8_t vol);

    void resetFilter();
    void setFilter(int chan, bool status);

    
    // These are pretty dangerous
    void setAddress( uint16_t what );
    void setData( uint16_t what );
};

//
// --------------------------------------------------------------------------
// MIGHT AS WELL
// --------------------------------------------------------------------------
//
extern SID6581 SID;

#endif
