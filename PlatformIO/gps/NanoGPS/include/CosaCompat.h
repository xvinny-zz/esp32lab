/**
 * @file CosaCompat.h
 * @author Vinicius de Sa (vinicius@vido-la.com)
 * @brief 
 * @version 0.1
 * @date 22/10/2019
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */
#pragma once

#ifdef __AVR__

  #include <avr/pgmspace.h>

#else

  #define PGM_P const char *

#endif

typedef PGM_P str_P;
#define __PROGMEM PROGMEM