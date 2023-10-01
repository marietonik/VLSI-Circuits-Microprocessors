/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    LCD_4bit.c
 *      Purpose: LCD functions for 2 line 16 characters Text LCD 
 *               (4-bit interface) connected on Freescale Freedom KL25Z evaluation board
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2012 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

/*********************** Hardware specific configuration **********************/
#include <MKL25Z4.H>

/*------------------------- Speed dependent settings -------------------------*/

/* If processor works on high frequency delay has to be increased, it can be 
   increased by factor 2^N by this constant                                   */
#define DELAY_2N     4

/*------------------------- Text LCD size definitions ------------------------*/

#define LineLen     24                  /* Width (in characters)              */
#define NumLines     2                  /* Hight (in lines)                   */

#define LCD_USER_FONT_ENABLE (1)

/*-------------------- LCD interface hardware definitions --------------------*/

/* PINS: 
 DB4 through DB8 are contiguous, starting with LSB at bit position PIN_DATA_SHIFT
 
 E, RW and RS are contiguous, starting with E at bit position PIN_E_SHIFT
 
 For example:
   - DB4 = PTC3
   - DB5 = PTC4
   - DB6 = PTC5
   - DB7 = PTC6
   - E   = PTC7
   - RW  = PTC8
   - RS  = PTC9                                                               */

#define PIN_DATA_SHIFT				( 3 )

#define PIN_E_SHIFT						( 7 )
#define PIN_E                 ( 1 << PIN_E_SHIFT)

#define PIN_RW_SHIFT					( PIN_E_SHIFT + 1 )
#define PIN_RW                ( 1 << PIN_RW_SHIFT)

#define PIN_RS_SHIFT	        ( PIN_E_SHIFT + 2)
#define PIN_RS                ( 1 << PIN_RS_SHIFT)

#define PINS_CTRL             (0x07 << PIN_E_SHIFT)

#define PINS_DATA             (0x0F << PIN_DATA_SHIFT)
#define PINS_ALL              (PINS_CTRL | PINS_DATA)

/* Enable Clock for peripheral driving LCD pins                               */
#define LCD_CLOCK_EN         	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;	

/* pin E  setting to 0 or 1                                                   */
#define LCD_E(x)              PTC->PDOR = (PTC->PDOR & ~PIN_E)  | ((x) ? PIN_E : 0);

/* pin RW setting to 0 or 1                                                   */
#define LCD_RW(x)             PTC->PDOR = (PTC->PDOR & ~PIN_RW) | ((x) ? PIN_RW : 0);

/* pin RS setting to 0 or 1                                                   */
#define LCD_RS(x)             PTC->PDOR = (PTC->PDOR & ~PIN_RS) | ((x) ? PIN_RS : 0);

/* Reading DATA pins                                                          */
#define LCD_DATA_IN           (((PTC->PDIR & PINS_DATA) >> PIN_DATA_SHIFT) & 0x0F)

/* Writing value to DATA pins                                                 */
#define LCD_DATA_OUT(x)       PTC->PDOR = (PTC->PDOR & ~PINS_DATA) | ((x) << PIN_DATA_SHIFT);

/* Setting all pins to output mode                                            */
#define LCD_ALL_DIR_OUT       PTC->PDDR = PTC->PDDR | PINS_ALL; 

/* Setting DATA pins to input mode                                            */
#define LCD_DATA_DIR_IN       PTC->PDDR = PTC->PDDR & ~PINS_DATA;

/* Setting DATA pins to output mode                                           */
#define LCD_DATA_DIR_OUT      PTC->PDDR = PTC->PDDR | PINS_DATA;

/******************************************************************************/

/*******************************************************************************
* Initialize the LCD controller                                                *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/
void lcd_init (void);

/*******************************************************************************
* Set cursor position on LCD display                                           *
*   Parameter:    column: column position                                      *
*                 line:   line position                                        *
*   Return:                                                                    *
*******************************************************************************/
void set_cursor (int column, int line);

/*******************************************************************************
* Clear the LCD display                                                        *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/
void lcd_clear (void);


/*******************************************************************************
* Print sting to LCD display                                                   *
*   Parameter:    string: pointer to output string                             *
*   Return:                                                                    *
*******************************************************************************/
void lcd_print (char *string);

/*******************************************************************************
* Print a bargraph to LCD display                                              *
*   Parameter:     val:  value 0..100 %                                        *
*                  size: size of bargraph 1..16                                *
*   Return:                                                                    *
*******************************************************************************/
void lcd_bargraph (int value, int size); 

/*******************************************************************************
* Display bargraph on LCD display                                              *
*   Parameter:     pos_x: horizontal position of bargraph start                *
*                  pos_y: vertical position of bargraph                        *
*                  value: size of bargraph active field (in pixels)            *
*   Return:                                                                    *
*******************************************************************************/
void lcd_bargraphXY (int pos_x, int pos_y, int value);
