// ux_manager.c


#include "project.h"
#include "ux_manager.h"
#include <stdio.h>

// ----------------------------
// private defines


// ----------------------------
// Global Constants


// ----------------------------
// Modular Constants


// ----------------------------
// global variables
// Screens
ui_screen currentScreen;
ui_screen lastScreen;

// Display-wrapped values
// format seq (numeric): {<format string>, <error message>, <Xpos>, <Ypos>, <valid?>, <init value>}
DWfloat counter = {"%5.2f", "---", 0, 0, true, 0};
DWfloat tempInF = {"%4.1f", "---", 0, 0, true, 72.2};
DWfloat humidity = {"%4.1f", "---", 0, 0, true, 40.1};
DWuint8_t setTempF = {"%d", "---", 0, 0, true, 72};

// format seq (string): {<format string>,  <error message>, <Xpos>, <Ypos>, <valid?>, "<init value>"
DWstring25 displayMessage = {"%s", "...", 0, 0, false, ""};

test2_t(8) myTest1;
// ----------------------------
// modular variables


// ----------------------------
// module prototypes
void SetupScreen(ui_screen _screenNo);



// ***************
// Start Of Code
// ***************
// Screen switching utility that manages pre-, post-, and screen switch conditions
void SwitchScreens(ui_screen screen_no)
{
  lastScreen = currentScreen;

  
//#pragma diag_suppress= Pa149
  // what must be done before current screen is switched out
  switch (lastScreen) {
  }
  
  
  // what must be done before screen is switched in
  switch (screen_no) {
  }
//#pragma diag_warning= Pa149
  
  // Switch the screens
  switch (screen_no) {
  case MAIN:
    // clear the screen from the previos dispayed data
    SSD1306_Clear();
    // Put up the "persistant" info (like data labels)
    SSD1306_GotoXY(0,0);
    SSD1306_Puts("Main Screen", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(0,30);
    SSD1306_Puts("Counter: ", &Font_11x18, SSD1306_COLOR_WHITE);
    // Set u X/Y coordinates for "live" data to be displayed on this screen
    counter.xPos = 55;
    counter.yPos = 30;
    break;
  }
  
  // Send a screen update (note this does not update the live data)
  SSD1306_UpdateScreen(); //display

  currentScreen = screen_no;
  
//#pragma diag_suppress= Pa149
  // what must be done after screen is switched in
  switch (currentScreen) {
  }
//#pragma diag_warning= Pa149
  
}



//#pragma diag_suppress= Pa149
void SetupScreen(ui_screen _screenNo)
{
  switch (_screenNo) {
  }
}
//#pragma diag_warning= Pa149



// ============= Begin kbd section =================
// *** UX Keycode processor
// *** This is a context sensitive keycode processor. There is a 
// *** non-context sensitive keycode processor in the keyboard handler file.
// *** 
// *** This one should be used if there are screens and the function
// *** of the keys changes based on the screen showing or system status.

// *** Typically only one of these two functions is used.
// ***
// =================================================

// Context-sensitive keyboard processor
uint8_t ProcessKeyCodeInContext (uint16_t key_code)
{
  switch (currentScreen) {
  case  MAIN:
    switch (key_code) {
    case 0:
//      SwitchScreens(SHOW_TEMP_HUM);
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    }
    break;
//  case  SHOW_TEMP_HUM:
//    switch (key_code) {
//    case 0:
//      SwitchScreens(SHOW_COUNT);
//      break;
//    case 1:
//      break;
//    case 2:
//      break;
//    case 3:
//      break;
//    }
//    break;
//  case  SHOW_COUNT:
//    switch (key_code) {
//    case 0:
//      SwitchScreens(SHOW_MESSAGE);
//      break;
//    case 1:
//      break;
//    case 2:
//      break;
//    case 3:
//      break;
//    }
//    break;
//  case  SHOW_MESSAGE:
//    switch (key_code) {
//    case 0:
//      SwitchScreens(SET_TEMP);
//      break;
//    case 1:
//      break;
//    case 2:
//      break;
//    case 3:
//      break;
//    }
//    break;
//  case  SET_TEMP:
//    switch (key_code) {
//    case 0:
//      SwitchScreens(BIG_TEMP);
//      break;
//    case 1:
//      if (setTempF.data < 90) {
//        setTempF.data++;
//      }
//      break;
//    case 2:
//      if (setTempF.data > 45) {
//        setTempF.data--;
//      }
//      break;
//    case 3:
//      break;
//    }
//    break;
//  case  SET_HUM:
//    switch (key_code) {
//    case 0:
//      break;
//    case 1:
//      break;
//    case 2:
//      break;
//    case 3:
//      break;
//    }
//    break;
//  case  SET_TIME:
//    switch (key_code) {
//    case 0:
//      break;
//    case 1:
//      break;
//    case 2:
//      break;
//    case 3:
//      break;
//    }
//    break;
//  case BIG_TEMP:
//    switch (key_code) {
//    case 0:
//      SwitchScreens(MAIN);
//      break;
//    case 1:
//      break;
//    case 2:
//      break;
//    case 3:
//      break;
//    }
//    break;
    
  }
  
  return true;
}

// ============== End kbd section ==================




// Live screen updates!!!
void UpdateScreenValues(void)
{
  char displayString[25];
  
  switch (currentScreen) {
  case MAIN:
      SSD1306_GotoXY(counter.xPos, tempInF.yPos);
    if (counter.valid) {
      sprintf(displayString, counter.format, counter.data);
      SSD1306_Puts(displayString, &Font_11x18, SSD1306_COLOR_WHITE);
    }
    else 
        SSD1306_Puts(counter.invalidMsg, &Font_11x18, SSD1306_COLOR_WHITE);
    break;
  }
  ssd1306_UpdateScreen(); //display
}


