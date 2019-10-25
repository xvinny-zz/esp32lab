/**
 * @file DMS.h
 * @author Vinicius de Sa (vinicius@vido-la.com)
 * @brief Define a estrutura de graus, minutos e segundos.
 * @version 0.1
 * @date 22/10/2019
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */
#pragma once

#include "NanoGPSConfig.h"
#include <stdint.h>

class Print;

/**
 * @brief Defines the hemisphere.
 * 
 */
enum Hemisphere_t
{
    /**
     * @brief North hemisphere.
     * 
     */
    NORTH_H = 0,
    /**
     * @brief South hemisphere.
     * 
     */
    SOUTH_H = 1,
    /**
     * @brief East hemisphere.
     * 
     */
    EAST_H = 0,
    /**
     * @brief West hemisphere.
     * 
     */
    WEST_H = 1
};

class DMS_t
{
public:
    uint8_t degrees;
    uint8_t minutes;         //NEOGPS_BF(6);
    Hemisphere_t hemisphere; //NEOGPS_BF(2); compiler bug!
    uint8_t seconds_whole NANOGPS_BF(6);
    uint16_t seconds_frac NANOGPS_BF(10); // 1000ths

    /**
     * @brief Initializes the DMS members.
     * 
     */
    void init()
    {
        degrees = minutes = seconds_whole = seconds_frac = 0;
        hemisphere = NORTH_H;
    }

    /**
     * @brief Returns the number of seconds in fraction.
     * 
     * @return float 
     */
    float secondsF() const { return seconds_whole + 0.001 * seconds_frac; };

    /**
     * @brief Returns the latitudinal hemisphere.
     * 
     * @return char 'S' for South and 'N' for North.
     */
    char NS() const { return (hemisphere == SOUTH_H) ? 'S' : 'N'; };
    /**
     * @brief Returns the longitudinal hemisphere.
     * 
     * @return char 'E' for East and 'W' for West.
     */
    char EW() const { return (hemisphere == WEST_H) ? 'W' : 'E'; };

    /**
     * @brief A utility function to convert from integer 'lat' or 'lon', scaled by 10^7.
     * 
     * @param deg_1E7 
     * @remarks No division is used, and shifts are on byte boundaries.  Fast!
     */
    void From(int32_t deg_1E7);

    /**
     * @brief Print DMS as the funky NMEA DDDMM.mmmm format
     * 
     * @param outs 
     */
    void printDDDMMmmmm(Print &outs) const;

} NANOGPS_PACKED;

/**
 * @brief Operator <<.
 * 
 * @param outs 
 * @return Print& 
 */
extern Print &operator<<(Print &outs, const DMS_t &);