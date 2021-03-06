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
//  Created: 06, March, 2021                                //
//  Modified: 06, March, 2021                               //
//  file: findStringInv.cl                                  //
//  Crypto                                                  //
//  Source: https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/scalarDataTypes.html                                                //
//  OS: ALL                                                 //
//  CPU: ALL                                                //
//                                                          //
//////////////////////////////////////////////////////////////

#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define alphabetSize 26

/**
 * \brief Generate Alphabetic sequence from size_t value, A=1, Z=27, AA = 28, AB = 29
 * \param A Alphabetic sequence index
 * \param B return array (char*)
 */


kernel void findStringInv(__global ulong* A, __global char* B)
{
    __private char alpha[26] = {ALPHABET};
    // If *A < 27
    if ((*A) < 27) {
        B[0] = alpha[(*A) - 1];
        return;
    }
    
    // If *A > 27
    __private ulong i = 0;
    
    while ((*A) > 0) {
        B[i] = alpha[(--(*A)) % alphabetSize];
        (*A) /= alphabetSize;
        i++;
    }
}

/**
 * \brief Generate Alphabetic sequence from size_t value, A=1, Z=27, AA = 28, AB = 29... With multi-threading (WIP)
 * \param A Alphabetic sequence index
 * \param B return array (char*)
 */


kernel void findStringInv_MT(__global ulong* A, __global char* B)
{
    __private char alpha[26] = {ALPHABET};
    // If *A < 27
    if (*A < 27) {
        B[0] = alpha[*A - 1];
        return;
    }
    
    // If *A > 27
    __private ulong i = get_global_id(0);
    
    while (*A > 0) {
        B[i] = alpha[(--*A) % alphabetSize];
        *A /= alphabetSize;
        i++;
    }
    B[i] = 0;
}