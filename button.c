/* 
 * The MIT License (MIT)
 * 
 * ESP8266 Non-OS Firmware
 * Copyright (c) 2015 Michael Jacobsen (github.com/mikejac)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "button.h"
#include <github.com/mikejac/esp-open-rtos.gpio.esp8266-nonos.cpp/gpio/gpio.h>

#define DTXT(...)   os_printf(__VA_ARGS__)

/******************************************************************************************************************
 * public functions
 *
 */

/**
 * 
 * @param b
 * @param pin
 * @param no_state
 * @return 
 */
int ICACHE_FLASH_ATTR Button_Initialize(BUTTON* b, int pin, int no_state)
{
    b->m_Pin        = pin;
    b->m_NOState    = (no_state == 0) ? false : true;
    
    b->m_Debounce   = 100;
    b->m_ShortPress = -1;
    b->m_LongPress  = -1;
    b->m_State      = 0;
    b->m_Start      = 0;
    
    // disable timer
    countdown_ms(&b->m_DebounceTimer, 0);
    
    gpio_enable(pin, GPIO_INPUT_PULLUP);

    DTXT("Button_Initialize(): m_NOState = %s\n", b->m_NOState ? "True" : "False");
    
    return 0;
}
/**
 * 
 * @param b
 * @param ms
 * @return 
 */
int ICACHE_FLASH_ATTR Button_SetDebounce(BUTTON* b, unsigned long ms)
{
    b->m_Debounce = ms;
    
    return 0;
}
/**
 * 
 * @param b
 * @param ms
 * @return 
 */
int ICACHE_FLASH_ATTR Button_SetShortPress(BUTTON* b, unsigned long ms)
{
    b->m_ShortPress = ms;
    
    return 0;
}
/**
 * 
 * @param b
 * @param ms
 * @return 
 */
int ICACHE_FLASH_ATTR Button_SetLongPress(BUTTON* b, unsigned long ms)
{
    b->m_LongPress = ms;
    
    return 0;
}
/**
 * 
 * @param b
 * @return 
 */
BUTTON_STATE ICACHE_FLASH_ATTR Button_Run(BUTTON* b)
{
    BUTTON_STATE state = Button_None;

    bool bb = gpio_read(b->m_Pin);

    if(b->m_State == 0 && bb != b->m_NOState) {
        DTXT("Button_Run(): input changed to CLOSED, starting debounce timer\n");

        // start debounce timer
        countdown_ms(&b->m_DebounceTimer, b->m_Debounce);

        // record start time
        b->m_Start = millis();
        
        b->m_State = 1;
    }
    else if(b->m_State == 1 && bb != b->m_NOState) {
        // button still pressed
        if(expired(&b->m_DebounceTimer)) {
            DTXT("Button_Run(): debounce timer expired; bb != b->m_NOState\n");
            
            // disable debounce timer
            countdown_ms(&b->m_DebounceTimer, 0);
    
            b->m_State = 2;
        }
    }
    else if(b->m_State == 1 && bb == b->m_NOState) {
        // button released
        if(expired(&b->m_DebounceTimer)) {
            DTXT("Button_Run(): debounce timer expired; bb == b->m_NOState\n");

            // disable debounce timer
            countdown_ms(&b->m_DebounceTimer, 0);
    
            b->m_State = 3;
        }
    }
    else if(b->m_State == 2) {
        // debounce done, button was still pressed when debounce ended
        if(bb == b->m_NOState) {
            DTXT("Button_Run(): button released\n");

            b->m_State = 3;
        }
    }
    else if(b->m_State == 3) {
        // debounce done, button released
        int time_left_ms_longpress  = millis() - (b->m_Start + b->m_LongPress);
        
        if(time_left_ms_longpress <= 0) {
            DTXT("Button_Run(): Short press\n");

            b->m_State = 4;
            state      = Button_Short;
        }
        else {
            DTXT("Button_Run(): Long press\n");

            b->m_State = 4;
            state      = Button_Long;
        }
    }
    else if(b->m_State == 4) {
        DTXT("Button_Run(): start debouncing button release\n");

        // start debounce timer
        countdown_ms(&b->m_DebounceTimer, b->m_Debounce);

        b->m_State = 5;
    }
    else if(b->m_State == 5) {
        if(expired(&b->m_DebounceTimer)) {
            DTXT("Button_Run(): button released debounce timer expired\n");

            // disable debounce timer
            countdown_ms(&b->m_DebounceTimer, 0);
    
            b->m_State = 0;
        }
    }
    
    return state;
}
