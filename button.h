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

#ifndef BUTTON_H
#define	BUTTON_H

#include <github.com/mikejac/timer.esp8266-nonos.cpp/timer.h>
#include <github.com/mikejac/misc.esp8266-nonos.cpp/espmissingincludes.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    Button_None,
    Button_Short,
    Button_Long
} BUTTON_STATE;


typedef struct {
    int             m_Pin;
    bool            m_NOState;
    unsigned long   m_Debounce;
    unsigned long   m_ShortPress;
    unsigned long   m_LongPress;
    
    uint32_t        m_Start;
    
    Timer           m_DebounceTimer;

    int             m_State;
} BUTTON;

/******************************************************************************************************************
 * prototypes
 *
 */

/**
 * 
 * @param b
 * @param pin
 * @param no_state
 * @return 
 */
int Button_Initialize(BUTTON* b, int pin, int no_state);
/**
 * 
 * @param b
 * @param ms
 * @return 
 */
int Button_SetDebounce(BUTTON* b, unsigned long ms);
/**
 * 
 * @param b
 * @param ms
 * @return 
 */
int Button_SetShortPress(BUTTON* b, unsigned long ms);
/**
 * 
 * @param b
 * @param ms
 * @return 
 */
int Button_SetLongPress(BUTTON* b, unsigned long ms);
/**
 * 
 * @param b
 * @return 
 */
BUTTON_STATE Button_Run(BUTTON* b);

#ifdef	__cplusplus
}
#endif

#endif	/* BUTTON_H */

