/*************************************************************************
* FIREKEY-PROJECT
* 
* TODO
*************************************************************************/

#ifndef DEBUG_H
  #define DEBUG_H

  #ifdef DEBUG
    #define DEBUG_BEGIN() Serial.begin(9600);
    #define DEBUG_PRINT(str) Serial.print(str)
    #define DEBUG_PRINTLN(str) Serial.println(str)
  #else
    #define DEBUG_BEGIN()
    #define DEBUG_PRINT(str)
    #define DEBUG_PRINTLN(str)
  #endif

#endif