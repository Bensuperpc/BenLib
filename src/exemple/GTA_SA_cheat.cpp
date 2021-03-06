//////////////////////////////////////////////////////////////
//   ____                                                   //
//  | __ )  ___ _ __  ___ _   _ _ __   ___ _ __ _ __   ___  //
//  |  _ \ / _ \ '_ \/ __| | | | '_ \ / _ \ '__| '_ \ / __| //
//  | |_) |  __/ | | \__ \ |_| | |_) |  __/ |  | |_) | (__  //
//  |____/ \___|_| |_|___/\__,_| .__/ \___|_|  | .__/ \___| //
//                             |_|             |_|          //
//////////////////////////////////////////////////////////////
//                                                          //
//  BenLib, 2021                                            //
//  Created: 26, February, 2021                             //
//  Modified: 03, March, 2021                               //
//  file: crypto.cpp                                        //
//  Crypto                                                  //
//  Source: http://stackoverflow.com/questions/8710719/generating-an-alphabetic-sequence-in-java                                                //
//          https://github.com/theo546/stuff                                                //
//          https://stackoverflow.com/a/19299611/10152334                                                //
//          https://gms.tf/stdfind-and-memchr-optimizations.html                                                //
//          https://medium.com/applied/applied-c-align-array-elements-32af40a768ee                                                //
//          https://create.stephan-brumme.com/crc32/                                                //
//          https://rosettacode.org/wiki/Generate_lower_case_ASCII_alphabet                                                //
//          https://web.archive.org/web/20090204140550/http://www.maxbot.com/gta/3wordcheatsdumpsorted.txt                                                //
//          https://www.codeproject.com/Articles/663443/Cplusplus-is-Fun-Optimal-Alphabetical-Order                                                //
//          https://cppsecrets.com/users/960210997110103971089710711510497116484964103109971051084699111109/Given-integer-n-find-the-nth-string-in-this-sequence-A-B-C-Z-AA-AB-AC-ZZ-AAA-AAB-AAZ-ABA-.php
//          https://www.careercup.com/question?id=14276663                                                //
//          https://stackoverflow.com/a/55074804/10152334                                                //
//          https://web.archive.org/web/20090204140550/http://www.maxbot.com/gta/3wordcheatsdumpsorted.txt                                                //
//          https://stackoverflow.com/questions/26429360/crc32-vs-crc32c                                                //
//  OS: ALL                                                 //
//  CPU: ALL                                                //
//                                                          //
//////////////////////////////////////////////////////////////

/** @defgroup GTA_SA GTA SA software
 *  @brief The main GTA_SA group who contain all software to brute force GTA SA password
 */
/** @defgroup GTA_SA_CPU CPU version
 *  @ingroup GTA_SA
 *  @sa @link GTA_SA The first group GTA_SA@endlink
 */
/** @defgroup GTA_SA_GPU_CUDA GPU CUDA version
 *  @ingroup GTA_SA
 *  @sa @link GTA_SA The first group GTA_SA@endlink
 */
/** @defgroup GTA_SA_GPU_OPENCL GPU OpenCL version
 *  @ingroup GTA_SA
 *  @sa @link GTA_SA The first group GTA_SA@endlink
 */

/** @defgroup Crypto Cryptography
 *  @brief The main cryptography group
 *  The main group
 */
/** @defgroup Crypto_RSA Crypto RSA
 *  @brief The cryptography RSA group
 *  @ingroup Crypto
 *  @sa @link Crypto The main group Crypto@endlink
 */
/** @defgroup Crypto_AES Crypto AES
 *  @brief The cryptography AES group
 *  @ingroup Crypto
 *  @sa @link Crypto The main group Crypto@endlink
 */
/** @defgroup Crypto_CRC Crypto CRC
 *  @brief The cryptography CRC group (With CRC32 and JAMCRC)
 *  @ingroup Crypto
 *  @sa @link Crypto The main group Crypto@endlink
 */
/** @defgroup Crypto_CRC32 Crypto CRC32
 *  @ingroup Crypto_CRC
 *  @brief Calculate CRC32
 *  @sa @link Crypto The main group Crypto@endlink
 */
/** @defgroup Crypto_JAMCRC Crypto JAMCRC
 *  @ingroup Crypto_CRC
 *  @brief Calculate JAMCRC
 *  @sa @link Crypto The main group Crypto@endlink
 */

/**
 *  @addtogroup GTA_SA_CPU
 *  @brief GTA SA Alternate cheat with CPU
 *  @author Bensuperpc 
 *
 * @{
 */

#include <algorithm> // for std::find

#if __has_include("boost/crc.hpp")
#include <boost/crc.hpp>
#endif

#include <cmath> // pow
#include <cstring>
#include <iomanip>  // setw
#include <iostream> // cout
#include <mutex>    // Mutex
#include <string>
#include <string_view> // string_view
#include <tuple>
#include <utility> // std::make_pair
#include <vector>
//#include "thread/Pool.hpp"        // Threadpool
#include "time/chrono/chrono.hpp" // Chrono

/** @brief If you want display less informations, comment it */
#define MORE_INFO

/** @brief For debug mode */
//#define DNDEBUG

/** @brief Define alphabetic seq with upercase */
#define alphabetUp "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

/** @brief Size of alphabet */
constexpr std::uint32_t alphabetSize {26};

/** @brief List of CRC32/JAMCRC hash of cheats codes */
const std::array<unsigned int, 87> cheat_list {0xDE4B237D, 0xB22A28D1, 0x5A783FAE, 0xEECCEA2B, 0x42AF1E28, 0x555FC201, 0x2A845345, 0xE1EF01EA, 0x771B83FC,
    0x5BF12848, 0x44453A17, 0xFCFF1D08, 0xB69E8532, 0x8B828076, 0xDD6ED9E9, 0xA290FD8C, 0x3484B5A7, 0x43DB914E, 0xDBC0DD65, 0xD08A30FE, 0x37BF1B4E, 0xB5D40866,
    0xE63B0D99, 0x675B8945, 0x4987D5EE, 0x2E8F84E8, 0x1A9AA3D6, 0xE842F3BC, 0x0D5C6A4E, 0x74D4FCB1, 0xB01D13B8, 0x66516EBC, 0x4B137E45, 0x78520E33, 0x3A577325,
    0xD4966D59, 0x5FD1B49D, 0xA7613F99, 0x1792D871, 0xCBC579DF, 0x4FEDCCFF, 0x44B34866, 0x2EF877DB, 0x2781E797, 0x2BC1A045, 0xB2AFE368, 0xFA8DD45B, 0x8DED75BD,
    0x1A5526BC, 0xA48A770B, 0xB07D3B32, 0x80C1E54B, 0x5DAD0087, 0x7F80B950, 0x6C0FA650, 0xF46F2FA4, 0x70164385, 0x885D0B50, 0x151BDCB3, 0xADFA640A, 0xE57F96CE,
    0x040CF761, 0xE1B33EB9, 0xFEDA77F7, 0x8CA870DD, 0x9A629401, 0xF53EF5A5, 0xF2AA0C1D, 0xF36345A8, 0x8990D5E1, 0xB7013B1B, 0xCAEC94EE, 0x31F0C3CC, 0xB3B3E72A,
    0xC25CDBFF, 0xD5CF4EFF, 0x680416B1, 0xCF5FDA18, 0xF01286E9, 0xA841CC0A, 0x31EA09CF, 0xE958788A, 0x02C83A7C, 0xE49C3ED4, 0x171BA8CC, 0x86988DAE, 0x2BDD2FA1};

#ifdef MORE_INFO
/// List of cheats codes names
const std::array<const std::string, 87> cheat_list_name {"Weapon Set 1", "Weapon Set 2", "Weapon Set 3", "Health, Armor, $250k, Repairs car",
    "Increase Wanted Level +2", "Clear Wanted Level", "Sunny Weather", "Very Sunny Weather", "Overcast Weather", "Rainy Weather", "Foggy Weather",
    "Faster Clock", "N°12", "N°13", "People attack each other with golf clubs", "Have a bounty on your head", "Everyone is armed", "Spawn Rhino",
    "Spawn Bloodring Banger", "Spawn Rancher", "Spawn Racecar", "Spawn Racecar", "Spawn Romero", "Spawn Stretch", "Spawn Trashmaster", "Spawn Caddy",
    "Blow Up All Cars", "Invisible car", "All green lights", "Aggressive Drivers", "Pink CArs", "Black Cars", "Fat Body", "Muscular Body", "Skinny Body",
    "People attack with Rocket Launchers", "N°41", "N°42", "Gangs Control the Streets", "N°44", "Slut Magnet", "N°46", "N°47", "Cars Fly", "N°49", "N°50",
    "Spawn Vortex Hovercraft", "Smash n' Boom", "N°53", "N°54", "N°55", "Orange Sky", "Thunderstorm", "Sandstorm", "N°59", "N°60", "Infinite Health",
    "Infinite Oxygen", "Have Parachute", "N°64", "Never Wanted", "N°66", "Mega Punch", "Never Get Hungry", "N°69", "N°70", "N°71", "N°72",
    "Full Weapon Aiming While Driving", "N°74", "Traffic is Country Vehicles", "Recruit Anyone (9mm)", "Get Born 2 Truck Outfit", "N°78", "N°79", "N°80",
    "L3 Bunny Hop", "N°82", "N°83", "N°84", "Spawn Quad", "Spawn Tanker Truck", "Spawn Dozer", "pawn Stunt Plane", "Spawn Monster"};
#endif

std::vector<std::tuple<std::size_t, std::string, unsigned int>> results = {}; // Stock results after calculations

/**
 * @brief To get CRC32 with boost libs
 * @param my_string String input
 * @return uint32_t with CRC32 value
 */
unsigned int GetCrc32(const std::string_view my_string);
unsigned int GetCrc32(const std::string_view my_string)
{
    boost::crc_32_type result;
    result.process_bytes(my_string.data(), my_string.length());
    return result.checksum();
}

/**
 * \brief Generate Alphabetic sequence from size_t value, A=1, Z=27, AA = 28, AB = 29
 * \tparam T
 * \param n index in base 26
 * \param array return array
 */
template <class T> void findStringInv(T n, char *array);
template <class T> void findStringInv(T n, char *array)
{
    constexpr std::uint32_t stringSizeAlphabet {alphabetSize + 1};
    constexpr std::array<char, stringSizeAlphabet> alpha {alphabetUp};
    // If n < 27
    if (n < stringSizeAlphabet) {
        array[0] = alpha[n - 1];
        return;
    }
    // If n > 27
    std::size_t i = 0;
    while (n > 0) {
        array[i] = alpha[(--n) % alphabetSize];
        n /= alphabetSize;
        ++i;
    }
}

int main(int arc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);

    const size_t from_range = 1; // Alphabetic sequence range min, change it only if you want begin on higer range, 1 = A
    // 141167095653376 = ~17 days on I7 9750H
    // 5429503678976 = ~14h on I7 9750H
    // 208827064576 = ~28 min on I7 9750H
    // 8031810176 = ~1 min on I7 9750H
    // 1544578880 = ~11 sec on I7 9750H
    // 308915776 = 2 sec on I7 9750H
    const size_t to_range = 1544578880; // Alphabetic sequence range max, must be > from_range !

#ifdef DNDEBUG
    assert(from_range < to_range); // Test if begining value is highter than end value
    assert(from_range > 0);        // Test forbiden value
#endif

#ifdef MORE_INFO
    std::cout << "Number of calculations: " << (to_range - from_range) << std::endl;
    std::cout << "" << std::endl;
    // Display Alphabetic sequence range
    char tmp1[29] = {0};
    char tmp2[29] = {0};
    findStringInv<size_t>(from_range, tmp1);
    findStringInv<size_t>(to_range, tmp2);
    std::cout << "From: " << tmp1 << " to: " << tmp2 << " Alphabetic sequence" << std::endl;
    std::cout << "" << std::endl;
#endif

    char tmp[29] = {0}; // Temp array
    uint32_t crc = 0;   // CRC value
    auto &&t1 = my::chrono::now();
#pragma omp parallel for schedule(auto) shared(results) firstprivate(tmp, crc)
    for (std::size_t i = from_range; i < to_range; i++) {
        // for (size_t j = i; j < i; j++) {
        findStringInv<size_t>(i, tmp); // Generate Alphabetic sequence from size_t value, A=1, Z=27, AA = 28, AB = 29
        crc = ~(GetCrc32(tmp));        // Get CRC32 and apply bitwise not, to convert CRC32 to JAMCRC
        if (std::find(std::begin(cheat_list), std::end(cheat_list), crc) != std::end(cheat_list)) { // If crc is present in Array
            std::reverse(tmp, tmp + strlen(tmp));                                                   // Invert char array
            results.emplace_back(std::make_tuple(i, std::string(tmp), crc)); // Save result: calculation position, Alphabetic sequence, CRC
        }
    }
    auto &&t2 = my::chrono::now();
    sort(results.begin(), results.end());
    for (auto &&result : results) {
        std::cout << std::left << std::setw(14) << std::dec << std::get<0>(result) << std::left << std::setw(12) << std::get<1>(result) << "0x" << std::hex
                  << std::left << std::setw(12) << std::get<2>(result);
        std::cout << std::endl;
    }
    std::cout << "Time: " << my::chrono::duration(t1, t2).count() << " sec" << std::endl;
    std::cout << "This program execute: " << std::fixed << ((to_range - from_range) / my::chrono::duration(t1, t2).count()) / 1000000 << " MOps/sec"
              << std::endl;

    return EXIT_SUCCESS;
}
/** @} */ // end of group2