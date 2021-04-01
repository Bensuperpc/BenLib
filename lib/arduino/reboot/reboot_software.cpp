/**
 * @file reboot_software.c
 * @author Bensuperpc (bensuperpc@gmail.com)
 * @brief Servolent 2016 and Jeu de réflexe 2017
 *        BAC project 2016-2017
 * @version 1.0.0
 * @date 2021-04-01
 * 
 * MIT License
 * 
 */

#include "reboot_software.hpp"

void reboot_software(void)
{
    wdt_enable(WDTO_15MS);
}
