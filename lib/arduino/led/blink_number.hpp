/**
 * @file blink_number.h
 * @author Bensuperpc (bensuperpc@gmail.com)
 * @brief Servolent 2016 and Jeu de réflexe 2017
 *        BAC project 2016-2017
 * @version 1.0.0
 * @date 2021-04-01
 * 
 * MIT License
 * 
 */

#ifndef BLINK_NUMBER_H
#define BLINK_NUMBER_H

#include "arduino_compatibility.hpp"

/**
 * @brief 
 * 
 * @param targetPin 
 * @param numBlinks 
 * @param blinkRate 
 */
void blink_led(byte targetPin, int numBlinks, int blinkRate);

#endif
