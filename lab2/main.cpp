/*
 * Copyright (c) 2014-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "PinNames.h"
#include "ThisThread.h"
#include "mbed.h"
#include "mbed_wait_api.h"

//  Notes      
#define A4      440        
#define B4b     446         
#define C4      261
#define C4_1    130 
#define C5      523      
#define D4      293   
#define D4b     277               
#define E4      329          
#define F4      349           
#define G4      392  
 
const int notes[] = {C4_1,C4,D4,C4,F4,E4,C4_1,C4,D4,C4,G4,F4,C4_1,C4,C5,A4,F4,E4,D4,B4b,B4b,A4,F4,G4,F4};
const int intervals[] = {4, 4, 8, 8, 8, 10, 4, 4, 8, 8, 8, 10, 4, 4, 8, 8, 8, 8, 8, 4, 4, 8, 8, 8, 12};     
 
// speaker sound effect demo using PWM hardware output
PwmOut speaker(PWM_OUT);

int main()
{
    int i;
    
    // Intro ( ta-dah @ 500Hz )
    speaker.period(1.0/500.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait_us(50);
    speaker=0.0; // turn off audio
    wait_us(50);
    speaker =0.5; //50% duty cycle - max volume
    wait_us(50);
    speaker=0.0; // turn off audio
    
    while(1) {
        wait_us(10);
        std::printf ("Happy birthday...\n");
        for (int i=0;i<=24;i++) {
            speaker.period(1.0/notes[i]);
            speaker=0.25;
            wait_us(0.8*intervals[i]*100);
        }
        speaker=0.0; // off
   }

    // for(int i = 0; i < 10000; ++i){
    //     speaker.period(0.1f);
    //     speaker = 0.25;
    //     wait_us(10);
    // }

    // speaker.period(0.05f);
    // speaker.pulsewidth(0.02f);
    // wait_us(10);

    // speaker.period(0.1f);
    // speaker.write(0.25f);
    // wait_us(10);
}