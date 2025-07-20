/**
 * @Version 1.0
 * @Date 2021-06-29
 * @Author Zhaohui Meng
 * @Affiliation Department of Computer Science, Hohai University, Nanjing, China.
 */

/*
这个头文件的主要作用是提供一个统一的接口，使得程序可以在不同的操作系统上编译和运行，而不需要修改核心代码。具体功能包括：
条件编译：根据不同的操作系统定义不同的宏和常量。
数学函数兼容性：确保在不同操作系统上使用正确的数学函数。
路径分隔符兼容性：确保在不同操作系统上使用正确的路径分隔符。
路径常量定义：提供常用的文件路径常量，便于程序中使用。
通过这种方式，开发者可以编写一次代码，并在不同的操作系统上进行编译和部署，大大提高了代码的可移植性和维护性。
*/



// data structure
//#include "gvnnDsCore.h"
//#include "pdbCore.h"
//#include "mdDsCore.h"



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IN USE
//
// home directory : all data files in this dir
//
// 2021-06-29
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SELECT WINDOWS OR LINUX
// add 2024-07-06
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _PROTEIN_RUN_ON_WINDOWS_or_LINUX_DEFINED

#define PROTEIN_RUN_ON_WINDOWS 0              // 1 : run on windows, 0 : not run on windows
#define PROTEIN_RUN_ON_LINUX 1                // 1 : run on linux,   0 : not run on linux

//#define PROTEIN_RUN_ON_WINDOWS 1              // 1 : run on windows, 0 : not run on windows
//#define PROTEIN_RUN_ON_LINUX 0                // 1 : run on linux,   0 : not run on linux

//constexpr int PROTEIN_RUN_ON_WINDOWS = 0;              // 1 : run on windows, 0 : not run on windows
//constexpr int PROTEIN_RUN_ON_LINUX   = 1;              // 1 : run on linux,   0 : not run on linux

#define _PROTEIN_RUN_ON_WINDOWS_or_LINUX_DEFINED
#endif


/*
#if PROTEIN_RUN_ON_LINUX
#endif  // match : #if PROTEIN_RUN_ON_LINUX

#if PROTEIN_RUN_ON_WINDOWS
#endif  // match : #if PROTEIN_RUN_ON_WINDOWS
*/


// 2021-06-29
#ifndef _QomdProteinMainDirName_DEFINED


    #if PROTEIN_RUN_ON_LINUX

        #define qomdFunc_abs abs                  // func of linux
        #define qomdFunc_isnan(a) isnan(a)        // func of linux
        #define qomdFunc_isinf(a) isinf(a)        // func of linux
        // isinf(t);      // #include <math.h>
        // isnan(t);      // #include <math.h>
        // 在使用这些函数时，应确保包含正确的头文件（<cstdlib>用于abs，<cmath>用于isnan和isinf）。
        // 对于abs函数，如果处理的是浮点数，应使用std::abs（在<cmath>头文件中定义），而不是abs，因为后者可能不会正确处理浮点数。

        
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        //  DIR_SLASH use by                                                                             //
        //  src/dicOctaHexa.cpp                                                                          //
        //  src/gvnnGpdbProcess.cpp                                                                      //
        //  src/pdbFunc.cpp                                                                              //
        //  carefully verify 2024-07-08                                                                  //
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        
        constexpr char* DIR_SLASH      = (char*)"/"           ;
        constexpr char* DIR_dict       = (char*)"/dict/"      ;
        constexpr char* DIR_dict_test  = (char*)"/dict_test/" ;
        constexpr char* DIR_dict_hhmo  = (char*)"/dict_hhmo/" ;
        constexpr char* DIR_dict_qomd  = (char*)"/dict_qomd/" ;
        constexpr char* DIR_InputData  = (char*)"/InputData/" ;
        constexpr char* DIR_OutputData = (char*)"/OutputData/";
    
        ///////////////////////////
        // xian                  //
        ///////////////////////////
        // 这边需要改成目标文件夹
        constexpr char* DIR_HOME_QOMD_PROTEIN = (char*)"/home/m1/mtf/ProteinCpp2025_0109";
        constexpr char* DIR_DICTIONARY        = (char*)"/home/m1/mtf/ProteinCpp2025_0109/dict";
        
        
        ///////////////////////////
        // qing                  //
        ///////////////////////////
        //constexpr char* DIR_HOME_QOMD_PROTEIN = (char*)"/home/hhcv/ProteinCpp";
        //constexpr char* DIR_DICTIONARY        = (char*)"/home/hhcv/ProteinCpp/dict";
    
    
    #endif  // match : #if PROTEIN_RUN_ON_LINUX



    #if PROTEIN_RUN_ON_WINDOWS

        #define _CRT_SECURE_NO_WARNINGS
        //用于关闭某些过时的CRT（C运行时库）函数的警告信息。


        #define qomdFunc_abs fabs                          // func of windows
        #define qomdFunc_isnan(a) _isnan(a)                // func of windows
        #define qomdFunc_isinf(a) !_finite(a)              // func of windows
        // _finite(cos_phi_ijkl);     // #include <float.h>
        // _isnan(cos_phi_ijkl);      // #include <float.h>

        constexpr char* DIR_SLASH      = (char*)"\\"            ;
        constexpr char* DIR_dict       = (char*)"\\dict\\"      ;
        constexpr char* DIR_dict_test  = (char*)"\\dict_test\\" ;
        constexpr char* DIR_dict_hhmo  = (char*)"\\dict_hhmo\\" ;
        constexpr char* DIR_dict_qomd  = (char*)"\\dict_qomd\\" ;
        constexpr char* DIR_InputData  = (char*)"\\InputData\\" ;
        constexpr char* DIR_OutputData = (char*)"\\OutputData\\";

        ///////////////////////////
        // matebook              //
        ///////////////////////////
        constexpr char* DIR_HOME_QOMD_PROTEIN = (char*)"D:\\home\\m2\\qomd_protein";
        constexpr char* DIR_DICTIONARY        = (char*)"D:\\home\\m2\\qomd_protein\\dict";
        
        
        ///////////////////////////
        // qing                  //
        ///////////////////////////
        //constexpr char* DIR_HOME_QOMD_PROTEIN = (char*)"D:\\home\\hhcv\\ProteinCpp";
        //constexpr char* DIR_DICTIONARY        = (char*)"D:\\home\\hhcv\\ProteinCpp\\dict";
    
    
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS




    ///////////////////////////
    // xian                  //
    ///////////////////////////
    
    // pdb files download from https://www.wwpdb.org/ftp/pdb-ftp-sites
    constexpr char* DIR_DOWNLOAD_PDB_FILES = (char*)"/home/m1/mengsdb2/pdb";
    
    // gvnnpdb files, that contains only one chain one model
    constexpr char* DIR_GVNN_PDB_FILES = (char *)"/home/m1/mengsdb2/gvnnPdb";


#define _QomdProteinMainDirName_DEFINED
#endif

/*
    // WINDOWS or LINUX ///////////////////////////////////////////////////////////////////////////////////////////////
    // 2024-07-10 revised
    if ( !( qomdFunc_isnan(Lambda1) || qomdFunc_isinf(Lambda1) ) )
    {
        return Lambda1;
    }
    else if ( !( qomdFunc_isnan(Lambda2) || qomdFunc_isinf(Lambda2) ) )
    {
        return Lambda2;
    }
    else
    {
        return Lambda3;
    }

    // WINDOWS or LINUX ///////////////////////////////////////////////////////////////////////////////////////////////

*/
/*
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++ 
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    #if PROTEIN_RUN_ON_LINUX
        // for linux : fopen in <stdio.h>
	    fptr_writerecord = fopen(fname_writerecord, "w+");
    #endif  // match : #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
        // for win : fopen_s in <stdio.h>
        fopen_s(&fptr_writerecord, fname_writerecord, "w+");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++ 
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/*
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    #if PROTEIN_RUN_ON_LINUX
        // for linux : fopen in <stdio.h>
        fptr_readin = fopen(fname_readin, "r");
    #endif  // match : #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
        // for win : fopen_s in <stdio.h>
        fopen_s(&fptr_readin, fname_readin, "r");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

*/

/*
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    #if PROTEIN_RUN_ON_LINUX
    // for linux : fopen in <stdio.h>
    fptr_output_QOMD_DICT_OCTA_byte = fopen(fname_output_QOMD_DICT_OCTA_byte, "wb");     // write/binary/create/clean
    #endif  // match : #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
        // for win : fopen_s in <stdio.h>
        fopen_s(&fptr_output_QOMD_DICT_OCTA_byte, fname_output_QOMD_DICT_OCTA_byte, "wb");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/


/*
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // BLOCK of <fopen> & <fopen_s>                                  ++
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        #if PROTEIN_RUN_ON_LINUX
            // for linux : fopen in <stdio.h>
            fptr_writerecord = fopen(fname_output_dicBond_byteFile, "wb");     // write/binary
        #endif  // match : #if PROTEIN_RUN_ON_LINUX

        #if PROTEIN_RUN_ON_WINDOWS
            // for win : fopen_s in <stdio.h>
            fopen_s(&fptr_writerecord, fname_output_dicBond_byteFile, "wb");
        #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // BLOCK of <fopen> & <fopen_s>                                  ++
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

*/

/*
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // BLOCK of <fopen> & <fopen_s>                                  ++
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        #if PROTEIN_RUN_ON_LINUX
            // for linux : fopen in <stdio.h>
            fptr_input_dicBond_byteFile = fopen(fname_input_dicBond_byteFile, "rb");     // read/binary
        #endif  // match : #if PROTEIN_RUN_ON_LINUX

        #if PROTEIN_RUN_ON_WINDOWS
            // for win : fopen_s in <stdio.h>
            fopen_s(&fptr_input_dicBond_byteFile, fname_input_dicBond_byteFile, "rb");
        #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // BLOCK of <fopen> & <fopen_s>                                  ++
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

*/

/*
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // BLOCK of <fopen> & <fopen_s>                                  ++
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        #if PROTEIN_RUN_ON_LINUX
            // for linux : fopen in <stdio.h>
            fptr_input_QOMD_DICT_OCTA_byte = fopen(fname_input_QOMD_DICT_OCTA_byte, "rb");     // read/binary
        #endif  // match : #if PROTEIN_RUN_ON_LINUX

        #if PROTEIN_RUN_ON_WINDOWS
            // for win : fopen_s in <stdio.h>
            fopen_s(&fptr_input_QOMD_DICT_OCTA_byte, fname_input_QOMD_DICT_OCTA_byte, "rb");
        #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // BLOCK of <fopen> & <fopen_s>                                  ++
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

*/


/*
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    #if PROTEIN_RUN_ON_LINUX
        // for linux : fopen in <stdio.h>
        fptr_writerecord = fopen(gvnn_file_full_name, "w+");
    #endif  // match : #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
        // for win : fopen_s in <stdio.h>
        fopen_s(&fptr_writerecord, gvnn_file_full_name, "w+");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS

*/

/*
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    #if PROTEIN_RUN_ON_LINUX
        // for linux : fopen in <stdio.h>
    fptr_output_TIME_POINT_byte = fopen(fname_output_TIME_POINT_byte, "wb");
    #endif  // match : #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
        // for win : fopen_s in <stdio.h>
        fopen_s(&fptr_output_TIME_POINT_byte, fname_output_TIME_POINT_byte, "wb");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

*/

/*
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    #if PROTEIN_RUN_ON_LINUX
        // for linux : fopen in <stdio.h>
    fptr_output_TIME_POINT_byte = fopen(fname_output_TIME_POINT_byte, "ab");
    #endif  // match : #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
        // for win : fopen_s in <stdio.h>
        fopen_s(&fptr_output_TIME_POINT_byte, fname_output_TIME_POINT_byte, "ab");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

*/

/*
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    #if PROTEIN_RUN_ON_LINUX
        // for linux : fopen in <stdio.h>
        fptr_output_QOMD_DICT_OCTA_byte = fopen(fname_output_QOMD_DICT_OCTA_byte, "wb");
    #endif  // match : #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
        // for win : fopen_s in <stdio.h>
        fopen_s(&fptr_output_QOMD_DICT_OCTA_byte, fname_output_QOMD_DICT_OCTA_byte, "wb");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/*
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    #if PROTEIN_RUN_ON_LINUX
        // for linux : fopen in <stdio.h>
        fptr_input_QOMD_DICT_OCTA_byte = fopen(fname_input_QOMD_DICT_OCTA_byte, "rb");
    #endif  // match : #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
        // for win : fopen_s in <stdio.h>
        fopen_s(&fptr_input_QOMD_DICT_OCTA_byte, fname_input_QOMD_DICT_OCTA_byte, "rb");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/


/*
    struct tm tm_data;
    struct tm * tm_ptr = &tm_data;
    time_t the_time;

    (void) time(&the_time);

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <gmtime> & <gmtime_s>                                ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    #if PROTEIN_RUN_ON_LINUX
        // for linux : gmtime in <time.h>
        tm_ptr = gmtime(&the_time);
    #endif  // match : #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
        // for win : gmtime_s in <time.h>
        gmtime_s(&tm_data, &the_time);
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <gmtime> & <gmtime_s>                                ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

*/

/*
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <sprintf> & <sprintf_s>                              ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    #if PROTEIN_RUN_ON_LINUX
        // for linux : sprintf in <stdio.h>
        sprintf(modelID_print, "%05d", previous_modelID);
    #endif  // match : #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
        // for win : sprintf_s in <stdio.h>
        sprintf_s(modelID_print, sizeof(modelID_print), "%05d", previous_modelID);
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <sprintf> & <sprintf_s>                              ++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //sprintf(modelID_print, "%05d", previous_modelID);    // Beginning Linux Programming 4th Edition : page96
                                                         // APUE page160 : flag 0, prefix with leading zeros instead of padding with spaces

*/





/*
2024-09-09 total code lines : 

run command (include blank lines):
hhcv@hhcv:~/ProteinCpp$ find ./src -name "*.*" |xargs cat|wc -l
cat: ./src/.vscode: Is a directory
129865

run command (exclude blank lines):
hhcv@hhcv:~/ProteinCpp$ find ./src -name "*.*" |xargs cat|grep -v ^$|wc -l
cat: ./src/.vscode: Is a directory
107918

*/