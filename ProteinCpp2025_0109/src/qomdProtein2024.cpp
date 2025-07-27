/////////////////////////////////////////////////////////////////////////////////////////
// qomdProtein2024.cpp : this file contains "main" function.
// qomdProtein2024.cpp : Defines the entry point for the console application.
/////////////////////////////////////////////////////////////////////////////////////////


/**
 * @Version 1.0
 * @Date 2024-08-28
 * @Author Zhaohui Meng
 * @Affiliation Department of Computer Science, Hohai University, Nanjing, China.
 */



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                       //
//       Architecture of Protein2023                                                                                                                     //
//                                                                                                                                                       //
//                                                                                                                                                       //
//                                                                                                                                                       //
//                                       ┌────────────────────────────┐                                                                                  //
//                                       │ static protein             │                                                                                  //
//                                       │ x-y-z coordinates of atoms │························································                          //
//                                       │ (1VII, from .gpdb file)    │                                                       ·                          //
//                                       └────────────┬───────────────┘                                                       ·                          //
//                                                    │                                                                       ·                          //
//                                                    │↓                                                                      ·                          //
//                                                    │                                                                       ·                          //
//    ┌──────────────────────────┐        ┌───────────┴─────────────┐        ┌─────────────────────────┐        ┌────────────────────────────┐           //
//    │ QOMD PARA                │    →   │ QOMD SIMULATOR          │    →   │ QOMD DVNN               │   ←→   │ static protein             │           //
//    │ initial parameters       ├────────┤ a protein simulator     ├────────┤ an observer/viewer      ├────────┤ x-y-z coordinates of atoms │           //
//    │ QOMD_DICT_OCTA.byte      │        │ output snapshoot per fs │        │ a neural networks       │        │ (1VII, from .gpdb file)    │           //
//    │ QOMD_DICT_HEXA_STAT.byte │        └───────────┬─────────────┘        └───────────┬─────────────┘        └─────────────┬──────────────┘           //
//    └──────────────────────────┘                    │                                  │                                    │                          //
//                                                    │↑                                 │↓                                  ↓│                          //
//                                                    │                                  └─────────────────┬──────────────────┘                          //
//                                        ┌───────────┴─────────────┐                                      │                                             //
//                                        │ QOMD REGULATOR          │                ←                    ↓│                                             //
//                                        │ a learning machine      ├──────────────────────────────────────┘                                             //
//                                        │ for parameters of QOMD  │                                                                                    //
//                                        └─────────────────────────┘                                                                                    //
//                                                                                                                                                       //
//                                                                                                                                                       //
//       QOMD : quantitative oscillator molecular dynamics                                                                                               //
//       DVNN : dynamic valued neural networks                                                                                                           //
//                                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                       //
//       Architecture of Protein2024                                                                                                                     //
//                                                                                                                                                       //
//                                                                                                                                                       //
//                                                                                                                                                       //
//    ┌──────────────────────────┐       ┌────────────────────────────┐      ┌────────────────────────────┐                                              //
//    │ QOMD PARA                │       │ static protein             │      │ output snapshoot initial   │                                              //
//    │ initial parameters       │       │ x-y-z coordinates of atoms │      │ x-y-z coordinates of atoms │····················                          //
//    │ QOMD_DICT_OCTA.byte      │       │ (1VII, from .gpdb file)    │      │ (1)dvnn_vertexList_1.txt   │                   ·                          //
//    │ QOMD_DICT_HEXA_STAT.byte │       └────────────┬───────────────┘      │ (2)time_point_linux.byte   │                   ·                          //
//    └───────────┬──────────────┘          ←         │                   →  └───────────┬────────────────┘                   ·                          //
//                └─┬─────────────────────────────────┘        ┌─────────────────────────┘                                    ·                          //
//                 ↓│                                          │↑                                                             ·                          //
//    ┌─────────────┴─────────────┐        ┌───────────────────┴─────┐        ┌─────────────────────────┐        ┌────────────────────────────┐          //
//    │ QOMD RUN DATA BAG         │    →   │ QOMD SIMULATOR          │    →   │ QOMD DVNN               │   ←→   │ output snapshoot per fs    │          //
//    │ proteinBag_1VII_linux.byte├─┬──────┤ a protein simulator     ├────────┤ an observer/viewer      ├────────┤ x-y-z coordinates of atoms │          //
//    │ proteinBag_1VII_linux.byte│ │      │ output snapshoot per fs │        │ a neural networks       │        │ (1)dvnn_vertexList_1.txt   │          //
//    └───────────────────────────┘ │      └───────────┬─────────────┘        └───────────┬─────────────┘        │ (2)time_point_linux.byte   │          //
//                                  │↑                 │                                  │                      └────────────┬───────────────┘          //
//                                  │                  │↑                                 │↓                                 ↓│                          //
//    ┌─────────────────────────────┴──┐               │                                  └────────────────┬──────────────────┘                          //
//    │ QOMD RUN CONFIG FILE           │   ┌───────────┴─────────────┐                                     │                                             //
//    │ QOMD_run_para_linux_2024.txt   │   │ QOMD REGULATOR          │         ┌────────────────┐         ↓│                                             //
//    │ cppCuda_run_para_linux_2024.txt│   │ a learning machine      ├─────────┤ dvnnSliderList ├──────────┘                                             //
//    └────────────────────────────────┘   │ for parameters of HHMO  │    ←    │ adjustable     │                                                        //
//                                         │ hhmoPanel_linux.byte    │         └────────────────┘                                                        //
//                                         └─────────────────────────┘                                                                                   //
//                                                                                                                                                       //
//       QOMD : quantitative oscillator molecular dynamics                                                                                               //
//       DVNN : dynamic valued neural networks                                                                                                           //
//       HHMO : Hohai molecular orbit                                                                                                                    //
//                                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// libQomd.a依赖于stringFunc.cpp、readFile.cpp、qomdShell3.cpp
// libQomd.a -> stringFunc.cpp
// libQomd.a -> readFile.cpp

// qomdShell3.cpp依赖于下面的（同时包括readFile.cpp和stringFunc.cpp）
// libQomd.a -> qomdShell3.cpp  ->  qomdSaveGetByte.cpp
// libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp
// libQomd.a -> qomdShell3.cpp  ->  readFile.cpp
// libQomd.a -> qomdShell3.cpp  ->  qomdCompute.cpp -> mdCompute.cpp
// libQomd.a -> qomdShell3.cpp  ->  stringFunc.cpp
// libQomd.a -> qomdShell3.cpp  ->  gvnnName.cpp


    //================================================================================//
    // para "PROTEIN_RUN_ON_LINUX" is defined in aDir.h                               //
    //================================================================================//
    //================================================================================//
    // struct "QomdBasePara"       is defined  in aBase.h                             //
    // func   "qomd_strcopy_rtrim" is defined  in libQomd.a -> stringFunc.cpp         //
    // func   "qomd_strcopy_rtrim" is declared in aFumcListSim.h                      //
    //================================================================================//
    //================================================================================//
    // para "FULL_NAME_MAX_LENGTH"  is defined in aBase.h                             //
    // para "DIR_HOME_QOMD_PROTEIN" is defined in aDir.h                              //
    // para "DIR_SLASH"             is defined in aDir.h                              //
    //================================================================================//

    //================================================================================//
    // func   "qomd_strmerge_3in1" is defined  in libQomd.a -> stringFunc.cpp         //
    // func   "qomd_strmerge_3in1" is declared in aFumcListSim.h                      //
    // func   "read_RunPara"       is defined  in libQomd.a -> readFile.cpp           //
    // func   "read_RunPara"       is declared in aFumcListSim.h                      //
    //================================================================================//
    //================================================================================//
    // para "DIR_OutputData"        is defined in aDir.h                              //
    //================================================================================//
    //================================================================================//
    // para "DIR_InputData"        is defined in aDir.h                               //
    //================================================================================//
    //====================================================================================================================================//
    // func   "qomd_shell3_get_from_byte_file_for_ProteinPackage" is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdSaveGetByte.cpp     //
    // func   "qomd_shell3_get_from_byte_file_for_ProteinPackage" is declared in aFumcListSim.h                                           //
    //====================================================================================================================================//
    // func   "qomd_shell3_display_ProteinPackage_para"           is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp            //
    // func   "qomd_shell3_display_ProteinPackage_para"           is declared in aFumcListSim.h                                           //
    //====================================================================================================================================//

    //================================================================================//
    // struct "DvnnSliderList"       is defined  in aDvnn.h                           //
    //================================================================================//
    //====================================================================================================================================//
    // func   "dvnn_init_dvnnSliderList_malloc"             is defined  in dvnnFunc.cpp                                                   //
    // func   "dvnn_init_dvnnSliderList_malloc"             is declared in aFumcListSim.h                                                 //
    //====================================================================================================================================//
    //====================================================================================================================================//
    // func   "dvnn_reset_dvnnSliderList_by_random"         is defined  in dvnnFunc.cpp                                                   //
    // func   "dvnn_reset_dvnnSliderList_by_random"         is declared in aFumcListSim.h                                                 //
    //====================================================================================================================================//

    //====================================================================================================================================//
    // func   "qomd_shell3_get_from_byte_file_for_HhmoPanel"      is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdSaveGetByte.cpp     //
    // func   "qomd_shell3_get_from_byte_file_for_HhmoPanel"      is declared in aFumcListSim.h                                           //
    //====================================================================================================================================//
    // func   "qomd_shell3_adjust_hhmoPanel_by_dvnnSliderList"    is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp            //
    // func   "qomd_shell3_adjust_hhmoPanel_by_dvnnSliderList"    is declared in aFumcListSim.h                                           //
    //====================================================================================================================================//

    //====================================================================================================================================//
    // func   "qomd_shell3_adjust_ProteinPackage_hhmo_para_by_HhmoPanel"    is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp  //
    // func   "qomd_shell3_adjust_ProteinPackage_hhmo_para_by_HhmoPanel"    is declared in aFumcListSim.h                                 //
    //====================================================================================================================================//
    //====================================================================================================================================//
    // func   "qomd_shell3_switch_ProteinPackage_Osci_Harmonic_by_QOMD"    is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp   //
    // func   "qomd_shell3_switch_ProteinPackage_Osci_Harmonic_by_QOMD"    is declared in aFumcListSim.h                                  //
    //====================================================================================================================================//
    //====================================================================================================================================//
    // func   "qomd_shell3_switch_ProteinPackage_Osci_Harmonic_by_HHMO"    is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp   //
    // func   "qomd_shell3_switch_ProteinPackage_Osci_Harmonic_by_HHMO"    is declared in aFumcListSim.h                                  //
    //====================================================================================================================================//
    //=================================================================================================================================================//
    // func   "qomd_shell3_switch_ProteinPackage_elccol_charge_by_vertex_qomdCharge"    is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp   //
    // func   "qomd_shell3_switch_ProteinPackage_elccol_charge_by_vertex_qomdCharge"    is declared in aFumcListSim.h                                  //
    //=================================================================================================================================================//
    //=================================================================================================================================================//
    // func   "qomd_shell3_switch_ProteinPackage_elccol_charge_by_vertex_hhmoCharge"    is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp   //
    // func   "qomd_shell3_switch_ProteinPackage_elccol_charge_by_vertex_hhmoCharge"    is declared in aFumcListSim.h                                  //
    //=================================================================================================================================================//

    //====================================================================================================================================//
    // func   "qomd_shell3_get_qomd_RunPara_from_configFile" is defined  in libQomd.a -> qomdShell3.cpp  ->  readFile.cpp                 //
    // func   "qomd_shell3_get_qomd_RunPara_from_configFile" is declared in aFumcListSim.h                                                //
    //====================================================================================================================================//
    //====================================================================================================================================//
    // func   "qomd_shell3_init_ProteinPackage_qomdRunPara" is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp                  //
    // func   "qomd_shell3_init_ProteinPackage_qomdRunPara" is declared in aFumcListSim.h                                                 //
    //====================================================================================================================================//
    //====================================================================================================================================//
    // func   "qomd_shell3_init_ProteinPackage_CudaGridBlock" is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp                //
    // func   "qomd_shell3_init_ProteinPackage_CudaGridBlock" is declared in aFumcListSim.h                                               //
    //====================================================================================================================================//
    //====================================================================================================================================//
    // func   "qomd_shell3_initOsci_normalState_limitDeviation"         is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp      //
    // func   "qomd_shell3_initOsci_normalState_limitDeviation"         is declared in aFumcListSim.h                                     //
    //====================================================================================================================================//
    // func   "qomd_shell3_init_osci_set_DeltaR_index_All"              is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp      //
    // func   "qomd_shell3_init_osci_set_DeltaR_index_All"              is declared in aFumcListSim.h                                     //
    //====================================================================================================================================//
    // func   "qomd_shell3_computeVertex_reset_Position_by_globalPoint" is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdCompute.cpp   //
    // func   "qomd_shell3_computeVertex_reset_Position_by_globalPoint" is declared in aFumcListSim.h                                     //
    //====================================================================================================================================//
    //================================================================================//
    // struct "DvnnAtomicVertexList"       is defined  in aDvnn.h                     //
    //================================================================================//

    //====================================================================================================================================//
    // func   "dvnn_init_malloc_vertexList"         is defined  in dvnnFunc.cpp                                                           //
    // func   "dvnn_init_malloc_vertexList"         is declared in aFumcListSim.h                                                         //
    //====================================================================================================================================//
    //====================================================================================================================================//
    // func   "qomd_shell3_copy_data_from_proteinBag_dvnnVertexList"      is defined  in libQomd.a -> qomdShell3.cpp  ->  stringFunc.cpp    //
    // func   "qomd_shell3_copy_data_from_proteinBag_dvnnVertexList"      is declared in aFumcListSim.h                                     //
    //====================================================================================================================================//

    //====================================================================================================================================//
    // func   "dvnn_write_dvnnVertexList_to_test_file"         is defined  in dvnnFunc.cpp                                                //
    // func   "dvnn_write_dvnnVertexList_to_test_file"         is declared in aFumcListSim.h                                              //
    //====================================================================================================================================//
    //====================================================================================================================================//
    // func   "qomd_shell3_write_dvnnVertexList_to_pdb_file"      is defined  in libQomd.a -> qomdShell3.cpp  ->  gvnnName.cpp            //
    // func   "qomd_shell3_write_dvnnVertexList_to_pdb_file"      is declared in aFumcListSim.h                                           //
    //====================================================================================================================================//
    //================================================================================//
    // struct "DvnnTimePointList"       is defined  in aDvnn.h                        //
    //================================================================================//
    //====================================================================================================================================//
    // func   "dvnn_init_timePointList_malloc"                        is defined  in dvnnFunc.cpp                                         //
    // func   "dvnn_init_timePointList_malloc"                        is declared in aFumcListSim.h                                       //
    //====================================================================================================================================//
    // func   "dvnn_save_timePointList_paraData_to_byte_file"         is defined  in dvnnFunc.cpp                                         //
    // func   "dvnn_save_timePointList_paraData_to_byte_file"         is declared in aFumcListSim.h                                       //
    //====================================================================================================================================//
    // func   "dvnn_save_timePointList_listData_to_byte_file"         is defined  in dvnnFunc.cpp                                         //
    // func   "dvnn_save_timePointList_listData_to_byte_file"         is declared in aFumcListSim.h                                       //
    //====================================================================================================================================//
    //====================================================================================================================================//
    // func   "qomd_shell3_femto_iterator_ser_use_CouVdw"      is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdCompute.cpp            //
    // func   "qomd_shell3_femto_iterator_ser_use_CouVdw"      is declared in aFumcListSim.h                                              //
    //====================================================================================================================================//
    // func   "qomd_shell3_femto_iterator_per_use_CouVdw"      is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdCompute.cpp            //
    // func   "qomd_shell3_femto_iterator_per_use_CouVdw"      is declared in aFumcListSim.h                                              //
    //====================================================================================================================================//
    // func   "qomd_shell3_femto_iterator_ser_use_ElcCol"      is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdCompute.cpp            //
    // func   "qomd_shell3_femto_iterator_ser_use_ElcCol"      is declared in aFumcListSim.h                                              //
    //====================================================================================================================================//
    // func   "qomd_shell3_femto_iterator_per_use_ElcCol"      is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdCompute.cpp            //
    // func   "qomd_shell3_femto_iterator_per_use_ElcCol"      is declared in aFumcListSim.h                                              //
    //====================================================================================================================================//
    //====================================================================================================================================//
    // func   "qomd_shell3_computeVertex_temperature_by_DeltaR"          is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdCompute.cpp  //
    // func   "qomd_shell3_computeVertex_temperature_by_DeltaR"          is declared in aFumcListSim.h                                    //
    //====================================================================================================================================//
    // func   "qomd_shell3_copy_current_from_proteinBag_dvnnVertexList"  is defined  in libQomd.a -> qomdShell3.cpp                       //
    // func   "qomd_shell3_copy_current_from_proteinBag_dvnnVertexList"  is declared in aFumcListSim.h                                    //
    //====================================================================================================================================//
    //====================================================================================================================================//
    // func   "dvnn_copy_vertexList_to_timePointList_by_timeStamp"       is defined  in dvnnFunc.cpp                                      //
    // func   "dvnn_copy_vertexList_to_timePointList_by_timeStamp"       is declared in aFumcListSim.h                                    //
    //====================================================================================================================================//
    //====================================================================================================================================//
    // func   "dvnn_get_timePointList_from_byte_file"       is defined  in dvnnFunc.cpp                                                   //
    // func   "dvnn_get_timePointList_from_byte_file"       is declared in aFumcListSim.h                                                 //
    //====================================================================================================================================//
    //====================================================================================================================================//
    // func   "qomd_shell3_compute_rmsd_by_timePointList"                is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdCompute.cpp  //
    // func   "qomd_shell3_compute_rmsd_by_timePointList"                is declared in aFumcListSim.h                                    //
    //====================================================================================================================================//



 ///////////////////////////
 // include
 ///////////////////////////


// C lib
#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;




 ///////////////////////////
 // include 
 ///////////////////////////

// home dir
#include "aDir.h"

// dvnn
#include "aDvnn.h"

// base para
#include "aBase.h"

// all functions
#include "aFuncListSim.h"



///////////////////////////
// functions declare
///////////////////////////

void all_procedures_2024();

//void procedure_24102(FILE* fptr_writerecord_test_output);
//void procedure_24102(FILE* fptr_writerecord_test_output, char * proteinBag_file_name);
void procedure_24102(FILE* fptr_writerecord_test_output, char * proteinBag_file_name_linux, char * proteinBag_file_name_win);

void procedure_24103();

void procedure_24104();



///////////////////////////
// main
///////////////////////////

// 2024-07-19 add
#if PROTEIN_RUN_ON_LINUX
    
    //================================================================================//
    // para "PROTEIN_RUN_ON_LINUX" is defined in aDir.h                               //
    //================================================================================//

    int main()
    {
    	all_procedures_2024();
    	return 0;
    }

#endif  // match : #if PROTEIN_RUN_ON_LINUX


// 2024-10-02
int qomd_protein_2024()
{

	///////////////////////////////////////////
	// entry
	///////////////////////////////////////////
	all_procedures_2024();

	return 0;

}



///////////////////////////
// other func
///////////////////////////

/**
 * @brief all_procedures_2024
 * @details
 * 这是程序的主控制函数，负责协调整个模拟或测试流程。
 * 它按顺序执行以下步骤：
 * 1. 初始化基础参数结构体。
 * 2. 从配置文件读取运行参数。
 * 3. 创建并打开一个用于记录结果的输出文件。
 * 4. 记录程序开始时间。
 * 5. 根据读取到的参数，选择并执行特定的测试流程 (procedure)。
 * 6. 记录程序结束时间。
 * 7. 计算总运行时间并将其写入输出文件。
 * 8. 关闭输出文件。
 * 9. 释放动态分配的内存。
 */
void all_procedures_2024()
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 步骤一: 设置并初始化基础参数
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //================================================================================//
    // 相关定义说明:
    // 结构体 "QomdBasePara"       在 aBase.h 中定义。
    // 函数   "qomd_strcopy_rtrim" 在 libQomd.a -> stringFunc.cpp 中定义，
    //                              并在 aFumcListSim.h 中声明。
    //================================================================================//

    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("STEP : set & init base para\n");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    // 为 QomdBasePara 结构体分配内存。这个结构体用于存放程序运行所需的所有基本参数。
    int size_basePara = sizeof(QomdBasePara); // 计算结构体大小
    QomdBasePara* basePara = (QomdBasePara*)malloc(size_basePara); // 动态分配内存

    // 初始化文件路径参数为一个安全的默认值 "None"。
    // 这样做可以防止在参数文件读取失败时，程序使用未初始化的垃圾值作为路径。
    char* string_of_None = (char*)"None    ";
    // 调用 qomd_strcopy_rtrim 函数，将 "None" 字符串复制到 basePara->para_DATA_DIR 成员中。
    // 这个函数同时会移除源字符串尾部的所有空格。
    qomd_strcopy_rtrim(basePara->para_DATA_DIR, string_of_None);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 步骤二: 从文本文件 <QOMD_run_para_linux_2024.txt> 或 <QOMD_run_para_win_2024.txt> 读取基础数据
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //================================================================================//
    // 相关定义说明:
    // 宏 "FULL_NAME_MAX_LENGTH"  在 aBase.h 中定义 (值为 193)。
    // 宏 "DIR_HOME_QOMD_PROTEIN" 在 aDir.h 中定义 (程序主目录)。
    // 宏 "DIR_SLASH"             在 aDir.h 中定义 (路径分隔符, "\" 或 "/")。
    //================================================================================//
    // 相关函数说明:
    // 函数 "qomd_strmerge_3in1" 在 libQomd.a -> stringFunc.cpp 中定义，用于合并三个字符串。
    // 函数 "read_RunPara"       在 libQomd.a -> readFile.cpp 中定义，用于从文件读取参数。
    //================================================================================//

    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("STEP : read base data from a text file <QOMD_run_para_linux_2024.txt>\n");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    // 定义一个字符数组，用于存放参数文件的完整路径和名称。
    char fname_para_readin[FULL_NAME_MAX_LENGTH];

    // 使用预处理器指令，根据编译时定义的宏 (PROTEIN_RUN_ON_LINUX 或 PROTEIN_RUN_ON_WINDOWS)
    // 来决定使用哪个操作系统对应的参数文件名。
    // 这种方式使得代码可以跨平台编译和运行。
    #if PROTEIN_RUN_ON_LINUX
        // 如果在 Linux 环境下编译，则拼接生成 Linux 的参数文件路径。
        // 例如: "/home/user/qomd/protein/" + "/" + "QOMD_run_para_linux_2024.txt"
        qomd_strmerge_3in1(fname_para_readin, DIR_HOME_QOMD_PROTEIN, DIR_SLASH, (char*)"QOMD_run_para_linux_2024.txt");
    #endif  // 匹配: #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
       // 如果在 Windows 环境下编译，则拼接生成 Windows 的参数文件路径。
       // 例如: "C:\\Users\\user\\qomd\\protein" + "\\" + "QOMD_run_para_win_2024.txt"
       qomd_strmerge_3in1(fname_para_readin, DIR_HOME_QOMD_PROTEIN, DIR_SLASH, (char*)"QOMD_run_para_win_2024.txt");
    #endif  // 匹配: #if PROTEIN_RUN_ON_WINDOWS

    // 调用 read_RunPara 函数，从上面确定的参数文件中读取配置，并填充到 basePara 结构体中。
    read_RunPara(fname_para_readin, basePara);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 步骤三: 打开空白文件 test_output_2024.txt
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //================================================================================//
    // 相关定义说明:
    // 宏 "DIR_OutputData" 在 aDir.h 中定义 (输出数据子目录)。
    //================================================================================//

    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("STEP : 打开输出文件: <test_output_2024.txt>\n");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    // 初始化并构建输出文件的完整路径。
    char fname_writerecord_test_output[FULL_NAME_MAX_LENGTH];
    // 拼接文件路径：主目录 + 输出数据目录 + 文件名 "test_output_2024.txt"
    qomd_strmerge_3in1(fname_writerecord_test_output, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"test_output_2024.txt");
    printf("TEST INFO write to file :   %s---\n", fname_writerecord_test_output);

    // 定义文件指针，用于后续的文件写操作。
    FILE* fptr_writerecord_test_output;

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // 跨平台文件打开处理块
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // 同样使用预处理器指令来处理不同操作系统下文件打开函数的差异。
    // Linux/macOS 使用标准 `fopen`，而 Windows 推荐使用更安全的 `fopen_s`。
    #if PROTEIN_RUN_ON_LINUX
        // 在 Linux 系统上，使用 fopen 函数。
        // "w+" 模式: 以读/写方式打开文件。如果文件已存在，其内容被清空；如果文件不存在，则创建新文件。
        fptr_writerecord_test_output = fopen(fname_writerecord_test_output, "w+");
    #endif  // 匹配: #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
        // 在 Windows 系统上，使用 fopen_s 函数，这是 C11 标准推荐的安全版本。
        // 它需要传递文件指针的地址。
        fopen_s(&fptr_writerecord_test_output, fname_writerecord_test_output, "w+");
    #endif  // 匹配: #if PROTEIN_RUN_ON_WINDOWS
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // 跨平台文件打开处理块结束
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // 检查文件是否成功打开。如果 fptr 为 NULL，说明打开失败。
    if (fptr_writerecord_test_output == NULL)
    {
       printf("failed to open output file < %s >\n", fname_writerecord_test_output);
       exit(1); // 打开失败则打印错误信息并退出程序。
    }
    else
    {
       printf("successful open output file < %s >\n", fname_writerecord_test_output);
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 步骤四: 获取程序开始执行的时间戳
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("STEP : get start time\n");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    // 调用 clock() 函数 (在 <time.h> 中) 获取当前处理器时间（时钟周期数）。
    clock_t time_start = clock();


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 步骤五: 根据参数选择并执行具体的测试流程 (TEST PROCEDURE)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // 这是一个分发逻辑，根据从参数文件中读取的 `para_RUN_PROCEDURE` 的值，
    // 来决定调用哪个具体的测试函数。
    // 这种设计使得程序具有很好的扩展性，可以方便地添加新的测试流程。

    // 2024-07-10 添加的测试流程 101
    if (basePara->para_RUN_PROCEDURE == 101)
    {
       printf("TEST PROCEDURE 2024 : only a test : basePara->para_RUN_PROCEDURE == 101\n");
       // 此处可以添加 procedure_101() 的调用
    }

    // 2024-08-29 添加的测试流程 24102 (当前被注释掉了)
    /*
    if (basePara->para_RUN_PROCEDURE == 24102)
    {
       printf("TEST PROCEDURE 24102 : only a test : basePara->para_RUN_PROCEDURE == 24102\n");
       procedure_24102(fptr_writerecord_test_output);
    }
    */

    // 2024-08-29 添加的测试流程 24103
    if (basePara->para_RUN_PROCEDURE == 24103)
    {
       printf("TEST PROCEDURE 24103 : only a test : basePara->para_RUN_PROCEDURE == 24103\n");
       procedure_24103();
    }

    // 2024-09-10 添加的测试流程 24104
    if (basePara->para_RUN_PROCEDURE == 24104)
    {
       printf("TEST PROCEDURE 24104 : only a test : basePara->para_RUN_PROCEDURE == 24104\n");
       procedure_24104();
    }

    // 2024-11-13 添加的测试流程 24105，用于处理蛋白 1VII
    if (basePara->para_RUN_PROCEDURE == 24105)
    {
       printf("TEST PROCEDURE 24105 : only a test : basePara->para_RUN_PROCEDURE == 24105\n");
       // 定义针对不同操作系统的二进制文件名
       char * proteinBag_file_name_linux = (char*)"proteinBag_1VII_linux.byte";
       char * proteinBag_file_name_win   = (char*)"proteinBag_1VII_win.byte"  ;
       // 调用一个通用的处理函数，并传入输出文件指针和平台相关的文件名
       procedure_24102(fptr_writerecord_test_output, proteinBag_file_name_linux, proteinBag_file_name_win);
    }

    // 2024-11-13 添加的测试流程 24106，用于处理蛋白 4UZX
    if (basePara->para_RUN_PROCEDURE == 24106)
    {
       printf("TEST PROCEDURE 24106 : only a test : basePara->para_RUN_PROCEDURE == 24106\n");
       char * proteinBag_file_name_linux = (char*)"proteinBag_4UZX_linux.byte";
       char * proteinBag_file_name_win   = (char*)"proteinBag_4UZX_win.byte"  ;
       procedure_24102(fptr_writerecord_test_output, proteinBag_file_name_linux, proteinBag_file_name_win);
    }

    // 2024-11-13 添加的测试流程 24107，用于处理蛋白 2RRK
    if (basePara->para_RUN_PROCEDURE == 24107)
    {
       printf("TEST PROCEDURE 24107 : only a test : basePara->para_RUN_PROCEDURE == 24107\n");
       char * proteinBag_file_name_linux = (char*)"proteinBag_2RRK_linux.byte";
       char * proteinBag_file_name_win   = (char*)"proteinBag_2RRK_win.byte"  ;
       procedure_24102(fptr_writerecord_test_output, proteinBag_file_name_linux, proteinBag_file_name_win);
    }

    // 2024-11-16 添加的测试流程 24115，用于处理蛋白 1VII_rod
    if (basePara->para_RUN_PROCEDURE == 24115)
    {
       printf("TEST PROCEDURE 24115 : only a test : basePara->para_RUN_PROCEDURE == 24115\n");
       char * proteinBag_file_name_linux = (char*)"proteinBag_1VII_rod_linux.byte";
       char * proteinBag_file_name_win   = (char*)"proteinBag_1VII_rod_win.byte"  ;
       procedure_24102(fptr_writerecord_test_output, proteinBag_file_name_linux, proteinBag_file_name_win);
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 步骤六: 获取程序结束时的时间戳
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("STEP : get end time\n");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    // 再次调用 clock() 函数获取当前处理器时间。
    clock_t time_end = clock();


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 步骤七: 计算并打印总运行时间到输出文件
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("STEP : print time to output file\n");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    // 计算总运行时间（秒）。
    // (time_end - time_start) 得到的是时钟周期数。
    // CLOCKS_PER_SEC 是一个宏，表示每秒钟的时钟周期数。二者相除得到秒数。
    float total_seconds = ((float)(time_end - time_start)) / ((float)CLOCKS_PER_SEC);

    // 打印总运行时间到控制台。
    printf("total_seconds = %10.3f  (seconds) \n", total_seconds);

    // 将详细的时间信息写入之前打开的输出文件。
    // `(int)time_start` 和 `(int)time_end` 记录的是原始的时钟周期数。
    fprintf(fptr_writerecord_test_output, "     time_start = %10d (clock_ticks)   time_end = %10d (clock_ticks)\n", (int)time_start, (int)time_end);
    fprintf(fptr_writerecord_test_output, "     running time = %10.3f (seconds) \n", total_seconds);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 步骤八: 关闭输出文件
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("STEP : close output file\n");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    // 调用 fclose 关闭文件，释放与文件指针相关的资源。
    // 这是一个良好的编程习惯，可以防止数据丢失或资源泄露。
    if (fclose(fptr_writerecord_test_output) != 0) // fclose 成功时返回 0
    {
       printf("failed to close file < %s >\n", fname_writerecord_test_output);
    }
    else
    {
       printf("successfully close file < %s >\n", fname_writerecord_test_output);
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 步骤九: 释放由 malloc 分配的内存
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("STEP : free memory allocated by malloc\n");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    // 调用 free 函数，将之前通过 malloc 分配给 basePara 的内存返还给系统。
    // 这可以防止内存泄漏。
    free(basePara);
    basePara = NULL; // 将指针设为 NULL 是一个好习惯，防止其成为“悬挂指针”。

}// 函数 all_procedures_2024() 结束



// 。

// 2024-08-29
/**
 * @brief procedure_24102
 * @details
 * 整套蛋白质模拟计算过程，具体步骤包括读取二进制文件初始化数据，设置相关参数，执行迭代计算，并输出模拟结果
 * 这是一个核心的蛋白质模拟流程函数。它接收一个文件指针用于记录日志，
 * 以及两个分别对应Linux和Windows系统的蛋白质数据包文件名。
 * 该函数负责加载蛋白质数据，初始化各种模拟参数（包括力场、运行模式、CUDA设置等），
 * 然后进入主循环进行迭代计算，并最终输出结果。
 *
 * @param fptr_writerecord_test_output 用于写入日志和结果的输出文件指针。
 * @param proteinBag_file_name_linux 在Linux系统上使用的蛋白质数据包（.byte）文件名。
 * @param proteinBag_file_name_win   在Windows系统上使用的蛋白质数据包（.byte）文件名。
 */
void procedure_24102(FILE* fptr_writerecord_test_output, char * proteinBag_file_name_linux, char * proteinBag_file_name_win)
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 从字节文件加载蛋白质数据包 (proteinBag)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 这是流程的第一步，从一个二进制字节文件（.byte）中读取预先处理好的蛋白质数据。
    //       `proteinBag_1VII` 是一个void指针，指向一个包含所有蛋白质模拟所需数据的复杂结构体。
    //       其内部结构对上层函数是隐藏的，只能通过特定的API函数进行访问和操作。

    //================================================================================//
    // 相关定义:
    // "DIR_InputData" 在 aDir.h 中定义，指向输入数据所在的目录。
    //================================================================================//

    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : proteinBag_1VII get from byte file <proteinBag_1VII_linux.byte>\n");
    printf("================================================================================================================\n");

    // 构造输入数据包文件的完整路径。
    // 函数的参数 `proteinBag_file_name_linux` 和 `proteinBag_file_name_win` 允许
    // `all_procedures_2024` 函数动态地决定加载哪个蛋白质数据。
    char fname_input_proteinBag_1VII_byte[FULL_NAME_MAX_LENGTH];

    #if PROTEIN_RUN_ON_LINUX
        // 在Linux环境下，拼接Linux版本的文件名。
        qomd_strmerge_3in1(fname_input_proteinBag_1VII_byte, DIR_HOME_QOMD_PROTEIN, DIR_InputData, (char*)proteinBag_file_name_linux);
    #endif  // 匹配: #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
        // 在Windows环境下，拼接Windows版本的文件名。
        qomd_strmerge_3in1(fname_input_proteinBag_1VII_byte, DIR_HOME_QOMD_PROTEIN, DIR_InputData, (char*)proteinBag_file_name_win);
    #endif  // 匹配: #if PROTEIN_RUN_ON_WINDOWS

    //====================================================================================================================================//
    // 相关函数:
    // "qomd_shell3_get_from_byte_file_for_ProteinPackage" 从指定的字节文件反序列化数据，并返回一个指向蛋白质数据包的void指针。
    // "qomd_shell3_display_ProteinPackage_para"           打印出刚加载的数据包中的一些基本参数，用于调试和验证。
    //====================================================================================================================================//

    // 调用函数从文件加载蛋白质数据包到内存。
    // `proteinBag_1VII` 是一个“void指针信息卡”(VPIC)，它指向一个包含复杂数据的内存地址。
    void * proteinBag_1VII = qomd_shell3_get_from_byte_file_for_ProteinPackage(fname_input_proteinBag_1VII_byte);

    // 显示加载后的数据包参数。
    qomd_shell3_display_ProteinPackage_para(proteinBag_1VII);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 修正初始化后的指针
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 从文件加载数据后，数据结构内部的指针会失效（因为内存地址变了）。
    //       这一步调用专门的函数来修正这些指针，使其指向新分配内存中的正确位置。
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : init pointer revised\n");
    printf("================================================================================================================\n");

    //====================================================================================================================================//
    // 相关函数:
    // "qomd_shell3_init_ProteinPackage_pointer_revise" 遍历蛋白质数据包内部的结构，修正所有需要重新链接的指针。
    //====================================================================================================================================//

    // 构造一个用于写入指针修正信息的日志文件名。
    char fname_writerecord_pointer_revise[FULL_NAME_MAX_LENGTH];
    qomd_strmerge_3in1(fname_writerecord_pointer_revise, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"pointer_revise.txt");

    // 执行指针修正操作。
    qomd_shell3_init_ProteinPackage_pointer_revise(proteinBag_1VII, fname_writerecord_pointer_revise);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 初始化DVNN滑动参数列表 (dvnn sliderList)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 初始化一个与DVNN（动态变量神经网络）相关的参数列表。这些参数可能用于在模拟中动态调整力场。
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : init dvnn sliderList\n");
    printf("================================================================================================================\n");

    //====================================================================================================================================//
    // 相关函数:
    // "dvnn_init_dvnnSliderList_malloc"       为DvnnSliderList结构体分配内存。
    // "dvnn_reset_dvnnSliderList_by_random"   用随机值填充DvnnSliderList，用于测试或初始化。
    //====================================================================================================================================//

    // 分配内存并获取dvnn sliderList的指针。
    DvnnSliderList* dvnnSliderList = dvnn_init_dvnnSliderList_malloc();

    // 使用随机值重置/初始化sliderList。
    dvnn_reset_dvnnSliderList_by_random(dvnnSliderList);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 从字节文件加载HHMO参数面板 (hhmoPanel)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 加载另一个重要的数据结构 `hhmoPanel`，它包含了HHMO（一种力场模型）的参数。
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : get hhmoPanel\n");
    printf("================================================================================================================\n");

    //====================================================================================================================================//
    // 相关函数:
    // "qomd_shell3_get_from_byte_file_for_HhmoPanel"       从字节文件加载HHMO参数面板。
    // "qomd_shell3_adjust_hhmoPanel_by_dvnnSliderList"     使用前面生成的dvnnSliderList来调整/修改hhmoPanel中的参数。
    //====================================================================================================================================//

    // 构造hhmoPanel字节文件的完整路径。
    char fname_input_hhmoPanel_byte[FULL_NAME_MAX_LENGTH];
    #if PROTEIN_RUN_ON_LINUX
        qomd_strmerge_3in1(fname_input_hhmoPanel_byte, DIR_HOME_QOMD_PROTEIN, DIR_dict, (char*)"hhmoPanel_linux.byte");
    #endif
    #if PROTEIN_RUN_ON_WINDOWS
        qomd_strmerge_3in1(fname_input_hhmoPanel_byte, DIR_HOME_QOMD_PROTEIN, DIR_dict, (char*)"hhmoPanel_win.byte");
    #endif

    // 从文件加载hhmoPanel。
    void * hhmoPanel = qomd_shell3_get_from_byte_file_for_HhmoPanel(fname_input_hhmoPanel_byte);

    // 根据dvnn sliderList中的值来调整hhmoPanel的参数。
    qomd_shell3_adjust_hhmoPanel_by_dvnnSliderList((void *)hhmoPanel, (void *)dvnnSliderList);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 使用HHMO参数面板重置蛋白质数据包中的HHMO参数
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 将上一步调整好的 `hhmoPanel` 中的参数，应用到主蛋白质数据包 `proteinBag_1VII` 中。
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : reset hhmo_para\n");
    printf("================================================================================================================\n");

    //====================================================================================================================================//
    // 相关函数:
    // "qomd_shell3_adjust_ProteinPackage_hhmo_para_by_HhmoPanel" 将hhmoPanel中的参数更新到proteinBag中。
    //====================================================================================================================================//

    qomd_shell3_adjust_ProteinPackage_hhmo_para_by_HhmoPanel((void *)proteinBag_1VII, (void *)hhmoPanel);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 获取QOMD运行参数 (qomdRunPara)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 从一个文本配置文件中读取模拟的控制参数，如运行模式、方法、总步数等。
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : get qomdRunPara\n");
    printf("================================================================================================================\n");

    // 构造运行参数配置文件的路径。
    char fname_qomd_RunPara_readin[FULL_NAME_MAX_LENGTH];
    #if PROTEIN_RUN_ON_LINUX
        qomd_strmerge_3in1(fname_qomd_RunPara_readin, DIR_HOME_QOMD_PROTEIN, DIR_SLASH, (char*)"cppCuda_run_para_linux_2024.txt");
    #endif
    #if PROTEIN_RUN_ON_WINDOWS
       qomd_strmerge_3in1(fname_qomd_RunPara_readin, DIR_HOME_QOMD_PROTEIN, DIR_SLASH, (char*)"cppCuda_run_para_win_2024.txt");
    #endif

    //====================================================================================================================================//
    // 相关函数:
    // "qomd_shell3_get_qomd_RunPara_from_configFile" 从指定的文本文件读取参数并填充到InputPara结构体中。
    //====================================================================================================================================//

    // 从配置文件读取参数。
    InputPara * qomdRunPara = qomd_shell3_get_qomd_RunPara_from_configFile(fname_qomd_RunPara_readin);
    printf("get_qomdRunPara_from_configFile DONE !\n");

    // 使用代码中预定义的宏来覆盖或重置从文件中读取的目录路径，确保路径的正确性。
    qomd_strcopy_rtrim(qomdRunPara->dir_home_qomd_protein, DIR_HOME_QOMD_PROTEIN);
    qomd_strcopy_rtrim(qomdRunPara->dir_dictionary, DIR_DICTIONARY);

    // 打印所有读取到的运行参数，用于确认。
	printf("qomdRunPara : PROTEIN_NAME           %s      \n", qomdRunPara->PROTEIN_NAME         );
	printf("qomdRunPara : GPU_DEVICE_NAME        %s      \n", qomdRunPara->GPU_DEVICE_NAME      );
	printf("qomdRunPara : DATA_DIR               %s      \n", qomdRunPara->DATA_DIR             );
	printf("qomdRunPara : dir_home_qomd_protein  %s      \n", qomdRunPara->dir_home_qomd_protein);
	printf("qomdRunPara : dir_dictionary         %s      \n", qomdRunPara->dir_dictionary       );
	printf("qomdRunPara : RUN_START              %10d    \n", qomdRunPara->RUN_START            );
	printf("qomdRunPara : PARA_ELEC              %10d    \n", qomdRunPara->PARA_ELEC            );
	printf("qomdRunPara : PARA_FREQ              %10d    \n", qomdRunPara->PARA_FREQ            );
	printf("qomdRunPara : RUN_MODE               %10d    \n", qomdRunPara->RUN_MODE             );
	printf("qomdRunPara : RUN_METHOD             %10d    \n", qomdRunPara->RUN_METHOD           );
	printf("qomdRunPara : RUN_GPU                %10d    \n", qomdRunPara->RUN_GPU              );
	printf("qomdRunPara : RUN_ROUNDS             %20d    \n", qomdRunPara->RUN_ROUNDS           );
	printf("qomdRunPara : WRITE_FREQ             %20d    \n", qomdRunPara->WRITE_FREQ           );
	printf("qomdRunPara : WRITE_MEDIUM_FREQ      %20d    \n", qomdRunPara->WRITE_MEDIUM_FREQ    );
	printf("qomdRunPara : BOND_DEVI_RATIO        %20.12f \n", qomdRunPara->BOND_DEVI_RATIO      );
	printf("qomdRunPara : ANGLE_DEVI_RATIO       %20.12f \n", qomdRunPara->ANGLE_DEVI_RATIO     );
	printf("qomdRunPara : PLANE_DEVI_RATIO       %20.12f \n", qomdRunPara->PLANE_DEVI_RATIO     );
	printf("qomdRunPara : DIHE_LIMIT_RATIO       %20.12f \n", qomdRunPara->DIHE_LIMIT_RATIO     );
	printf("qomdRunPara : DIHE_FLATTOP_RATIO     %20.12f \n", qomdRunPara->DIHE_FLATTOP_RATIO   );
	printf("qomdRunPara : BOND_ENFORCE_RATIO     %20.12f \n", qomdRunPara->BOND_ENFORCE_RATIO   );     // add 2024-12-12
	printf("qomdRunPara : ANGLE_ENFORCE_RATIO    %20.12f \n", qomdRunPara->ANGLE_ENFORCE_RATIO  );     // add 2024-12-12
	printf("qomdRunPara : PLANE_ENFORCE_RATIO    %20.12f \n", qomdRunPara->PLANE_ENFORCE_RATIO  );     // add 2024-12-12
	printf("qomdRunPara : BOND_DEVIMODI_RATE     %20.12f \n", qomdRunPara->BOND_DEVIMODI_RATE   );     // add 2024-12-12
	printf("qomdRunPara : ANGLE_DEVIMODI_RATE    %20.12f \n", qomdRunPara->ANGLE_DEVIMODI_RATE  );     // add 2024-12-12
	printf("qomdRunPara : PLANE_DEVIMODI_RATE    %20.12f \n", qomdRunPara->PLANE_DEVIMODI_RATE  );     // add 2024-12-12

	printf("qomdRunPara : RATIO_ELCCOL_R_WALL    %20.12f \n", qomdRunPara->RATIO_ELCCOL_R_WALL  );     // add 2024-12-23
	printf("qomdRunPara : RATIO_ELCCOL_COL1      %20.12f \n", qomdRunPara->RATIO_ELCCOL_COL1    );     // add 2024-12-23
	printf("qomdRunPara : RATIO_ELCCOL_COL2      %20.12f \n", qomdRunPara->RATIO_ELCCOL_COL2    );     // add 2024-12-23
	printf("qomdRunPara : RATIO_ELCCOL_ELC       %20.12f \n", qomdRunPara->RATIO_ELCCOL_ELC     );     // add 2024-12-23



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 在蛋白质数据包中重置QOMD运行参数
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 将上一步读取到的 `qomdRunPara` 应用到主蛋白质数据包 `proteinBag_1VII` 中。
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : reset qomdRunPara in proteinBag\n");
    printf("================================================================================================================\n");

    //====================================================================================================================================//
    // 相关函数:
    // "qomd_shell3_init_ProteinPackage_qomdRunPara" 将qomdRunPara结构体的内容复制到proteinBag的相应位置。
    //====================================================================================================================================//
    qomd_shell3_init_ProteinPackage_qomdRunPara(proteinBag_1VII, qomdRunPara);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 初始化评估参数结构体 (qomdEvaluatePara)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 分配内存用于存放模拟过程中的评估指标，如温度、能量、键长偏离度等。
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : init qomdEvaluatePara\n");
    printf("================================================================================================================\n");
    int size_qomdEvaluatePara = sizeof(EvaluatePara);
    EvaluatePara* qomdEvaluatePara = (EvaluatePara*)malloc(size_qomdEvaluatePara);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 根据参数切换振子和谐振模型 (Osci_Harmonic)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 根据运行参数 `PARA_FREQ` 的值，选择使用旧的QOMD固定参数模型还是新的HHMO可调参数模型。
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : switch_ProteinPackage_Osci_Harmonic_by_HHMO\n");
    printf("================================================================================================================\n");
    // PARA_FREQ: 1 = 使用qomd (旧的固定参数), 2 = 使用hhmo (新的可调参数)
    if (qomdRunPara->PARA_FREQ == 1) {
       qomd_shell3_switch_ProteinPackage_Osci_Harmonic_by_QOMD((void *)proteinBag_1VII);
    } else if (qomdRunPara->PARA_FREQ == 2) {
       qomd_shell3_switch_ProteinPackage_Osci_Harmonic_by_HHMO((void *)proteinBag_1VII);
    } else {
       printf("INVALID PARA_FREQ, PARA_FREQ must be 1 or 2. \n");
       exit(0);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 根据参数切换电荷和库仑力计算模型 (elccol_charge)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 根据运行参数 `PARA_ELEC` 的值，选择使用旧的QOMD固定电荷模型还是新的HHMO可调电荷模型。
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : switch_ProteinPackage_elccol_charge_by_vertex_hhmoCharge\n");
    printf("================================================================================================================\n");
    // PARA_ELEC: 1 = 使用qomd (旧的固定参数), 2 = 使用hhmo (新的可调参数)
    if (qomdRunPara->PARA_ELEC == 1) {
       qomd_shell3_switch_ProteinPackage_elccol_charge_by_vertex_qomdCharge((void *)proteinBag_1VII);
    } else if (qomdRunPara->PARA_ELEC == 2) {
       qomd_shell3_switch_ProteinPackage_elccol_charge_by_vertex_hhmoCharge((void *)proteinBag_1VII);
    } else {
       printf("INVALID PARA_ELEC, PARA_ELEC must be 1 or 2. \n");
       exit(0);
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 重置CUDA网格和块大小 (CudaGridBlock)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 根据蛋白质的大小等信息，在数据包内部设置合适的CUDA执行配置（Grid和Block维度），为GPU并行计算做准备。
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : reset CudaGridBlock\n");
    printf("================================================================================================================\n");
    qomd_shell3_init_ProteinPackage_CudaGridBlock(proteinBag_1VII);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 运行前初始化振子数据
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 在主循环开始前，执行一系列与振子相关的初始化操作。
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : init osci data before run\n");
    printf("================================================================================================================\n");
    // 初始化振子的正常状态和偏离限制。
    qomd_shell3_initOsci_normalState_limitDeviation(proteinBag_1VII);
    // 初始化DeltaR（位置变化量）的索引。
    qomd_shell3_init_osci_set_DeltaR_index_All(proteinBag_1VII);
    // 将所有DeltaR设置为零。
    qomd_shell3_init_osci_set_DeltaR_to_zero_All(proteinBag_1VII);
    // 根据全局坐标重置计算用的局部坐标。
    qomd_shell3_computeVertex_reset_Position_by_globalPoint(proteinBag_1VII);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 初始化DVNN原子、键、角、二面角、平面等列表
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 为DVNN模块创建一系列数据列表，用于存储和操作原子坐标、键连接、键角、二面角和平面约束等信息。
    //       这些列表从主数据包 `proteinBag_1VII` 中拷贝初始数据，并可以独立于主数据包进行分析和输出。
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : init dvnn bondList angleList dihedralList planeList\n");
    printf("================================================================================================================\n");

    // 为原子列表分配内存并从proteinBag拷贝初始数据。
    DvnnAtomicVertexList* dvnnVertexList = dvnn_init_malloc_vertexList(proteinBag_1VII);
    qomd_shell3_copy_data_from_proteinBag_dvnnVertexList(dvnnVertexList, proteinBag_1VII);

    // 为键、角、二面角、平面列表分配内存并拷贝初始固定数据。
    DvnnForceBondList* dvnnBondList     = dvnn_init_malloc_dvnnBondList(proteinBag_1VII);
    DvnnForceAngleList* dvnnAngleList    = dvnn_init_malloc_dvnnAngleList(proteinBag_1VII);
    DvnnForceDihedralList* dvnnDihedralList = dvnn_init_malloc_dvnnDihedralList(proteinBag_1VII);
    DvnnForcePlaneList* dvnnPlaneList    = dvnn_init_malloc_dvnnPlaneList(proteinBag_1VII);
    qomd_shell3_copy_data_from_proteinBag_dvnnBondList(dvnnBondList, (void*) proteinBag_1VII);
    qomd_shell3_copy_data_from_proteinBag_dvnnAngleList(dvnnAngleList, (void*) proteinBag_1VII);
    qomd_shell3_copy_data_from_proteinBag_dvnnDihedralList(dvnnDihedralList, (void*) proteinBag_1VII);
    qomd_shell3_copy_data_from_proteinBag_dvnnPlaneList(dvnnPlaneList, (void*) proteinBag_1VII);

    // 将所有这些列表的初始状态写入文本文件和PDB文件，用于调试和验证。
	/////////////////////////////////////////////////////////////////
	// write all List to test file
	/////////////////////////////////////////////////////////////////
	// get output file name
	char fname_writerecord_dvnn_all_list_init[FULL_NAME_MAX_LENGTH];
	qomd_strmerge_3in1(fname_writerecord_dvnn_all_list_init, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"dvnn_all_list_init.txt");

	// write to test file
    // 将所有这些列表的初始状态写入文本文件和PDB文件，用于调试和验证。
    /////////////////////////////////////////////////////////////////
    // 将所有列表写入测试文件
    /////////////////////////////////////////////////////////////////
    // 获取输出文件名
    char fname_writerecord_dvnn_all_list_init[FULL_NAME_MAX_LENGTH];
    // 拼接文件路径：主目录 + 输出数据目录 + 文件名 "dvnn_all_list_init.txt"
    qomd_strmerge_3in1(fname_writerecord_dvnn_all_list_init, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"dvnn_all_list_init.txt");

    // 写入测试文件
>>>>>>> ad94389 (add: Data processing)
    dvnn_write_dvnnAllList_to_test_file(
                                         (DvnnAtomicVertexList  *) dvnnVertexList,        // 原子顶点列表
                                         (DvnnForceBondList     *) dvnnBondList,          // 键合力列表
                                         (DvnnForceAngleList    *) dvnnAngleList,         // 角度力列表
                                         (DvnnForceDihedralList *) dvnnDihedralList,      // 二面角力列表
                                         (DvnnForcePlaneList    *) dvnnPlaneList,         // 平面力列表
                                         (char                  *) fname_writerecord_dvnn_all_list_init ); // 输出文件名



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 初始化DVNN时间点列表 (timePointList)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 创建一个用于记录轨迹的列表。这个列表将在每个时间步（或每隔几个时间步）保存所有原子的坐标。
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : init dvnn timePointList\n");
    printf("================================================================================================================\n");
    // 为时间点列表分配内存。
    DvnnTimePointList* timePointList = dvnn_init_timePointList_malloc(dvnnVertexList);
    // 构造轨迹输出文件的路径。
    char fname_output_TIME_POINT_byte[FULL_NAME_MAX_LENGTH];
    #if PROTEIN_RUN_ON_LINUX
        qomd_strmerge_3in1(fname_output_TIME_POINT_byte, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"time_point_linux.byte");
    #endif
    #if PROTEIN_RUN_ON_WINDOWS
        qomd_strmerge_3in1(fname_output_TIME_POINT_byte, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"time_point_win.byte");
    #endif

    // 创建轨迹字节文件，并首先写入头部信息（总帧数、总原子数）。
    int totalStamps = qomdRunPara->RUN_ROUNDS;
    int totalVeretx = dvnnVertexList->actItem;
    dvnn_save_timePointList_paraData_to_byte_file(fname_output_TIME_POINT_byte, totalStamps, totalVeretx);
    // 将初始状态（第0帧）追加写入轨迹文件。
    dvnn_save_timePointList_listData_to_byte_file(fname_output_TIME_POINT_byte, timePointList);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 设置迭代器函数指针 (qomd_femto_iterator)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 这是程序设计的关键部分。通过使用函数指针，可以根据运行参数动态地选择核心计算函数。
    //       这避免了在主循环中使用大量的if-else分支，使代码更清晰、更高效。
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : qomd_femto_iterator set run method\n");
    printf("================================================================================================================\n");
    // 声明一个函数指针，它指向一个接收void*并返回void的函数。
    void (*qomd_femto_iterator)(void* proteinBag_vpic);

    // 根据运行参数 RUN_METHOD (计算方法) 和 RUN_MODE (串行/并行) 来给函数指针赋值。
    // RUN_METHOD: 1=COUVDW, 2=ELCCOL, 3=仅振子
    // RUN_MODE:   1=串行(SER), 2=并行(PAR)
    if      ((qomdRunPara->RUN_METHOD == 1) && (qomdRunPara->RUN_MODE == 1)) { qomd_femto_iterator = &qomd_shell3_femto_iterator_ser_use_CouVdw; }
    else if ((qomdRunPara->RUN_METHOD == 1) && (qomdRunPara->RUN_MODE == 2)) { qomd_femto_iterator = &qomd_shell3_femto_iterator_per_use_CouVdw; }
    else if ((qomdRunPara->RUN_METHOD == 2) && (qomdRunPara->RUN_MODE == 1)) { qomd_femto_iterator = &qomd_shell3_femto_iterator_ser_use_ElcCol; }
    else if ((qomdRunPara->RUN_METHOD == 2) && (qomdRunPara->RUN_MODE == 2)) { qomd_femto_iterator = &qomd_shell3_femto_iterator_per_use_ElcCol; }
    else if ((qomdRunPara->RUN_METHOD == 3) && (qomdRunPara->RUN_MODE == 2)) { qomd_femto_iterator = &qomd_shell3_femto_iterator_per_use_4_osci; }
    else { printf("INVALID RUN_METHOD or INVALID RUN_MODE.\n"); exit(1); }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 主循环 (iterate)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : iterate\n");
    printf("================================================================================================================\n");
    int count = -1;
    int write_freq = qomdRunPara->WRITE_FREQ; // 输出频率
    int max_round  = qomdRunPara->RUN_ROUNDS; // 总运行步数

    for (int round = 0; round < max_round; round++)
    {
       // 调用由函数指针指向的核心计算函数，执行一步模拟。
       qomd_femto_iterator(proteinBag_1VII);

       // 计数器，用于控制输出频率。
       count = count + 1;
       if (count == write_freq)
       {
          count = 0; // 重置计数器

          // 计算各种评估指标，如温度、键长/键角偏离度等。
			// compute temperature
			qomd_shell3_computeVertex_temperature_by_DeltaR      (proteinBag_1VII, qomdEvaluatePara);
			qomd_shell3_computeVertex_temperature_by_DeltaRElcCol(proteinBag_1VII, qomdEvaluatePara);
			qomd_shell3_computeVertex_temperature_by_DeltaRCouVdw(proteinBag_1VII, qomdEvaluatePara);

			// compute limitDeviation_level
            qomd_shell3_computeEvaluate_BondList (proteinBag_1VII, qomdEvaluatePara);
            qomd_shell3_computeEvaluate_AngleList(proteinBag_1VII, qomdEvaluatePara);
            qomd_shell3_computeEvaluate_PlaneList(proteinBag_1VII, qomdEvaluatePara);
            qomd_shell3_computeEvaluate_DihedralList(proteinBag_1VII, qomdEvaluatePara);
            qomd_shell3_computeEvaluate_ElcColList(proteinBag_1VII, qomdEvaluatePara);

          // 将计算出的指标打印到控制台和日志文件。
			// print temperature on console
			printf("round=%10d"                     , round                );
			printf("   t_DeltaR=%12.6f"             , qomdEvaluatePara->t_DeltaR                 );
			printf("   t_DeltaRElcCol=%12.6f"       , qomdEvaluatePara->t_DeltaRElcCol           );
			printf("   t_DeltaRCouVdw=%12.6f"       , qomdEvaluatePara->t_DeltaRCouVdw           );
			printf("\n");
			printf("   Num_elccol_move_ALL =%8d"         , qomdEvaluatePara->Num_elccol_move_ALL         );
			printf("   Num_elccol_move_NONE=%8d"         , qomdEvaluatePara->Num_elccol_move_NONE        );
			printf("   Num_elccol_move_COL1=%8d"         , qomdEvaluatePara->Num_elccol_move_COL1        );
			printf("   Num_elccol_move_COL2=%8d"         , qomdEvaluatePara->Num_elccol_move_COL2        );
			printf("   Num_elccol_move_ELC =%8d"         , qomdEvaluatePara->Num_elccol_move_ELC         );
			printf("   Num_elccol_move_OTHE=%8d"         , qomdEvaluatePara->Num_elccol_move_OTHE        );
			printf("\n");
			printf("   limitDevi_level_Bond =%10.6f"     , qomdEvaluatePara->LimitDeviationLevel_Bond    );
			printf("   Mode_1_Level_Bond =%10.6f"        , qomdEvaluatePara->Mode_1_Level_Bond           );
			printf("   Max_ratio_Bond =%10.6f"           , qomdEvaluatePara->Max_ratio_terminal_Bond     );
			printf("   Max_ratio_Bond_idx =%6d"          , qomdEvaluatePara->Max_ratio_terminal_Bond_idx );
			printf("   Min_ratio_Bond =%10.6f"           , qomdEvaluatePara->Min_ratio_terminal_Bond     );
			printf("   Min_ratio_Bond_idx =%6d"          , qomdEvaluatePara->Min_ratio_terminal_Bond_idx );
			printf("\n");
			printf("   limitDevi_level_Angle=%10.6f"     , qomdEvaluatePara->LimitDeviationLevel_Angle   );
			printf("   Mode_1_Level_Angle=%10.6f"        , qomdEvaluatePara->Mode_1_Level_Angle          );
			printf("   Max_ratio_Angle=%10.6f"           , qomdEvaluatePara->Max_ratio_terminal_Angle    );
			printf("   Max_ratio_Angle_idx=%6d"          , qomdEvaluatePara->Max_ratio_terminal_Angle_idx);
			printf("   Min_ratio_Angle=%10.6f"           , qomdEvaluatePara->Min_ratio_terminal_Angle    );
			printf("   Min_ratio_Angle_idx=%6d"          , qomdEvaluatePara->Min_ratio_terminal_Angle_idx);
			printf("\n");
			printf("   limitDevi_level_Plane=%10.6f"     , qomdEvaluatePara->LimitDeviationLevel_Plane   );
			printf("   Mode_1_Level_Plane=%10.6f"        , qomdEvaluatePara->Mode_1_Level_Plane          );
			printf("   Max_ratio_Plane=%10.6f"           , qomdEvaluatePara->Max_ratio_terminal_Plane    );
			printf("   Max_ratio_Plane_idx=%6d"          , qomdEvaluatePara->Max_ratio_terminal_Plane_idx);
			printf("   Min_ratio_Plane=%10.6f"           , qomdEvaluatePara->Min_ratio_terminal_Plane    );
			printf("   Min_ratio_Plane_idx=%6d"          , qomdEvaluatePara->Min_ratio_terminal_Plane_idx);
			printf("\n");
			printf("   Num_Dihedral_ALL =%6d"            , qomdEvaluatePara->Num_Dihedral_ALL            );
			printf("   Num_valley_change=%6d"            , qomdEvaluatePara->Num_valley_change           );
			printf("\n");

			// print temperature to file
			fprintf(fptr_writerecord_test_output, "round=%10d"                     , round                );
			fprintf(fptr_writerecord_test_output, "   t_DeltaR=%12.6f"             , qomdEvaluatePara->t_DeltaR                 );
			fprintf(fptr_writerecord_test_output, "   t_DeltaRElcCol=%12.6f"       , qomdEvaluatePara->t_DeltaRElcCol           );
			fprintf(fptr_writerecord_test_output, "   t_DeltaRCouVdw=%12.6f"       , qomdEvaluatePara->t_DeltaRCouVdw           );
			fprintf(fptr_writerecord_test_output, "\n");
			fprintf(fptr_writerecord_test_output, "   Num_elccol_move_ALL =%8d"         , qomdEvaluatePara->Num_elccol_move_ALL         );
			fprintf(fptr_writerecord_test_output, "   Num_elccol_move_NONE=%8d"         , qomdEvaluatePara->Num_elccol_move_NONE        );
			fprintf(fptr_writerecord_test_output, "   Num_elccol_move_COL1=%8d"         , qomdEvaluatePara->Num_elccol_move_COL1        );
			fprintf(fptr_writerecord_test_output, "   Num_elccol_move_COL2=%8d"         , qomdEvaluatePara->Num_elccol_move_COL2        );
			fprintf(fptr_writerecord_test_output, "   Num_elccol_move_ELC =%8d"         , qomdEvaluatePara->Num_elccol_move_ELC         );
			fprintf(fptr_writerecord_test_output, "   Num_elccol_move_OTHE=%8d"         , qomdEvaluatePara->Num_elccol_move_OTHE        );
			fprintf(fptr_writerecord_test_output, "\n");
			fprintf(fptr_writerecord_test_output, "   limitDevi_level_Bond =%10.6f"     , qomdEvaluatePara->LimitDeviationLevel_Bond    );
			fprintf(fptr_writerecord_test_output, "   Mode_1_Level_Bond =%10.6f"        , qomdEvaluatePara->Mode_1_Level_Bond           );
			fprintf(fptr_writerecord_test_output, "   Max_ratio_Bond =%10.6f"           , qomdEvaluatePara->Max_ratio_terminal_Bond     );
			fprintf(fptr_writerecord_test_output, "   Max_ratio_Bond_idx =%6d"          , qomdEvaluatePara->Max_ratio_terminal_Bond_idx );
			fprintf(fptr_writerecord_test_output, "   Min_ratio_Bond =%10.6f"           , qomdEvaluatePara->Min_ratio_terminal_Bond     );
			fprintf(fptr_writerecord_test_output, "   Min_ratio_Bond_idx =%6d"          , qomdEvaluatePara->Min_ratio_terminal_Bond_idx );
			fprintf(fptr_writerecord_test_output, "\n");
			fprintf(fptr_writerecord_test_output, "   limitDevi_level_Angle=%10.6f"     , qomdEvaluatePara->LimitDeviationLevel_Angle   );
			fprintf(fptr_writerecord_test_output, "   Mode_1_Level_Angle=%10.6f"        , qomdEvaluatePara->Mode_1_Level_Angle          );
			fprintf(fptr_writerecord_test_output, "   Max_ratio_Angle=%10.6f"           , qomdEvaluatePara->Max_ratio_terminal_Angle    );
			fprintf(fptr_writerecord_test_output, "   Max_ratio_Angle_idx=%6d"          , qomdEvaluatePara->Max_ratio_terminal_Angle_idx);
			fprintf(fptr_writerecord_test_output, "   Min_ratio_Angle=%10.6f"           , qomdEvaluatePara->Min_ratio_terminal_Angle    );
			fprintf(fptr_writerecord_test_output, "   Min_ratio_Angle_idx=%6d"          , qomdEvaluatePara->Min_ratio_terminal_Angle_idx);
			fprintf(fptr_writerecord_test_output, "\n");
			fprintf(fptr_writerecord_test_output, "   limitDevi_level_Plane=%10.6f"     , qomdEvaluatePara->LimitDeviationLevel_Plane   );
			fprintf(fptr_writerecord_test_output, "   Mode_1_Level_Plane=%10.6f"        , qomdEvaluatePara->Mode_1_Level_Plane          );
			fprintf(fptr_writerecord_test_output, "   Max_ratio_Plane=%10.6f"           , qomdEvaluatePara->Max_ratio_terminal_Plane    );
			fprintf(fptr_writerecord_test_output, "   Max_ratio_Plane_idx=%6d"          , qomdEvaluatePara->Max_ratio_terminal_Plane_idx);
			fprintf(fptr_writerecord_test_output, "   Min_ratio_Plane=%10.6f"           , qomdEvaluatePara->Min_ratio_terminal_Plane    );
			fprintf(fptr_writerecord_test_output, "   Min_ratio_Plane_idx=%6d"          , qomdEvaluatePara->Min_ratio_terminal_Plane_idx);
			fprintf(fptr_writerecord_test_output, "\n");
			fprintf(fptr_writerecord_test_output, "   Num_Dihedral_ALL =%6d"            , qomdEvaluatePara->Num_Dihedral_ALL            );
			fprintf(fptr_writerecord_test_output, "   Num_valley_change=%6d"            , qomdEvaluatePara->Num_valley_change           );
			fprintf(fptr_writerecord_test_output, "\n");

		}

       // 这部分被注释掉了，如果取消注释，它将在每一步都保存轨迹。
       /*
       // 从proteinBag拷贝当前坐标到dvnnVertexList
       qomd_shell3_copy_current_from_proteinBag_dvnnVertexList(dvnnVertexList, proteinBag_1VII);
       // 将dvnnVertexList的坐标拷贝到timePointList的当前时间戳位置
       int timeStamp = round + 1;
       dvnn_copy_vertexList_to_timePointList_by_timeStamp(timePointList, dvnnVertexList, timeStamp);
       // 将当前时间戳的数据追加写入轨迹文件
       dvnn_save_timePointList_listData_to_byte_file(fname_output_TIME_POINT_byte, timePointList);
       */

       // 按指定频率保存轨迹数据到time_point.byte文件
       // 修改：取消注释并添加频率控制，避免每步都输出造成文件过大

       // WRITE_FREQ 在 cppCuda_run_para_linux_2024.txt 设置，目前设置的是1
       // 相当于每一时间步都输出结果
     if (round % qomdRunPara->WRITE_FREQ == 0)
       {
          // 从proteinBag拷贝当前坐标到dvnnVertexList
          qomd_shell3_copy_current_from_proteinBag_dvnnVertexList(dvnnVertexList, proteinBag_1VII);
          // 将dvnnVertexList的坐标拷贝到timePointList的当前时间戳位置
           int timeStamp = round;  // 使用当前round作为时间戳
          dvnn_copy_vertexList_to_timePointList_by_timeStamp(timePointList, dvnnVertexList, timeStamp);
           // 将当前时间戳的数据追加写入轨迹文件
          dvnn_save_timePointList_listData_to_byte_file(fname_output_TIME_POINT_byte, timePointList);
           printf("轨迹数据已保存: 时间步 %d\n", timeStamp);
       }

//       // 在第100步时，输出一个快照，用于调试。
//       if (round == 100)
//       {
//           qomd_shell3_write_proteinBag_output_10_files((void *) proteinBag_1VII);
//       }
    }// 主循环结束


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 流程步骤: 输出最终的DVNN列表
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 模拟结束后，将最终的原子坐标和各种力场项的当前状态从主数据包中拷贝出来，并保存到文件中。
    printf("================================================================================================================\n");
    printf("PROCEDURE_STEP : output all List\n");
    printf("================================================================================================================\n");

    // 从proteinBag拷贝最终的原子坐标。
    qomd_shell3_copy_current_from_proteinBag_dvnnVertexList(dvnnVertexList, proteinBag_1VII);

    // 从proteinBag拷贝最终的键、角、二面角、平面列表的当前数据。
    qomd_shell3_copy_current_from_proteinBag_dvnnBondList(dvnnBondList, (void*) proteinBag_1VII);
    qomd_shell3_copy_current_from_proteinBag_dvnnAngleList(dvnnAngleList, (void*) proteinBag_1VII);
    qomd_shell3_copy_current_from_proteinBag_dvnnDihedralList(dvnnDihedralList, (void*) proteinBag_1VII);
    qomd_shell3_copy_current_from_proteinBag_dvnnPlaneList(dvnnPlaneList, (void*) proteinBag_1VII);

    // 将所有这些最终状态的列表写入一个汇总的文本文件。
    char fname_writerecord_dvnn_all_list_end[FULL_NAME_MAX_LENGTH];
    qomd_strmerge_3in1(fname_writerecord_dvnn_all_list_end, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"dvnn_all_list_end.txt");
    dvnn_write_dvnnAllList_to_test_file(
                                         (DvnnAtomicVertexList*)  dvnnVertexList,
                                         (DvnnForceBondList*)     dvnnBondList,
                                         (DvnnForceAngleList*)    dvnnAngleList,
                                         (DvnnForceDihedralList*) dvnnDihedralList,
                                         (DvnnForcePlaneList*)    dvnnPlaneList,
                                         (char*)                  fname_writerecord_dvnn_all_list_end );

    // 此处可以添加释放本函数内部分配的内存的代码，如 free(qomdEvaluatePara), free(dvnnVertexList) 等。

}// 函数 procedure_24102() 结束




// 2024-08-29
/**
 * @brief procedure_24103
 * @details
 * 这是一个用于后处理和分析的函数。它的主要任务是读取在先前模拟
 * (例如 procedure_24102) 中生成的二进制轨迹文件 (`time_point.byte`)，
 * 将数据加载到内存中，并打印出轨迹的起始和结束部分的数据点，
 * 以便快速验证轨迹是否被正确记录。最后，它会打印一些关键数据类型的大小，
 * 用于调试和确保跨平台数据一致性。
 */
void procedure_24103()
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 步骤一: 从二进制文件 "time_point.byte" 中读取轨迹数据
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //====================================================================================================================================//
    // 相关函数:
    // "dvnn_get_timePointList_from_byte_file" 在 dvnnFunc.cpp 中定义。
    //   此函数负责打开指定的二进制文件，读取头部信息（如总帧数、总原子数），
    //   然后分配足够的内存给 DvnnTimePointList 结构体，并从文件中读取所有
    //   轨迹数据点填充到该结构体中。
    //====================================================================================================================================//

    // 构造包含轨迹数据的输入文件名。
    char fname_input_TIME_POINT_byte[FULL_NAME_MAX_LENGTH];

    // 根据编译时定义的宏，选择适用于当前操作系统（Linux 或 Windows）的文件名。
    #if PROTEIN_RUN_ON_LINUX
        qomd_strmerge_3in1(fname_input_TIME_POINT_byte, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"time_point_linux.byte");
    #endif  // 匹配: #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
       qomd_strmerge_3in1(fname_input_TIME_POINT_byte, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"time_point_win.byte");
    #endif  // 匹配: #if PROTEIN_RUN_ON_WINDOWS

    // 调用函数，从文件中读取数据并返回一个指向已填充数据的 DvnnTimePointList 结构体的指针。
    DvnnTimePointList * timePointList = dvnn_get_timePointList_from_byte_file(fname_input_TIME_POINT_byte);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 步骤二: 打印部分轨迹数据以供查验
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // 遍历从文件中读取到的所有数据点。
    for (int i = 0; i < timePointList->actItem; i++)
    {
       // 设置一个条件，只打印轨迹的开头（时间步 < 5）和结尾（时间步 > 995）部分的数据。
       // 这样做可以避免在控制台输出海量数据，同时又能有效地检查轨迹的初始和最终状态。
       if ( (timePointList->List[i].t < 5) || (timePointList->List[i].t > 995) )
       {
           // 打印每个数据点的详细信息，包括：
           // i:           数据点在列表中的索引
           // t:           时间步 (timeStamp)
           // vertexIdx:   原子在原子列表中的索引
           // vertexSN:    原子的序列号
           // valid:       有效位 (通常为1)
           // x, y, z:     该时间步下该原子的三维坐标
           printf(" i =%6d "         , i                                   );
           printf(" t =%6d "         , timePointList->List[i].t            );
           printf(" vertexIdx =%6d " , timePointList->List[i].vertexIdx    );
           printf(" vertexSN =%6d "  , timePointList->List[i].vertexNameSN );
           printf(" valid =%4d "     , timePointList->List[i].valid        );
           printf(" x =%12.4f "      , timePointList->List[i].x            );
           printf(" y =%12.4f "      , timePointList->List[i].y            );
           printf(" z =%12.4f "      , timePointList->List[i].z            );
           printf("\n");
       }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 步骤三: 打印关键数据类型的大小
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 打印一些基本数据类型和自定义结构体的大小（以字节为单位）。
    //       这是一个非常重要的调试步骤，用于确保在不同的操作系统（如Linux和Windows）
    //       或不同的编译器设置下，数据结构的大小保持一致。如果不一致，
    //       直接读写二进制文件可能会导致数据错乱。
    printf(" sizeof(int)               =%10d \n", (int)sizeof(int)               );
    printf(" sizeof(double)            =%10d \n", (int)sizeof(double)            );
    printf(" sizeof(DvnnTimePoint)     =%10d \n", (int)sizeof(DvnnTimePoint)     );
    printf(" sizeof(DvnnTimePointList) =%10d \n", (int)sizeof(DvnnTimePointList) );

    ///////////////////////////////////////////////////////////////////////
    // 示例输出 (Linux):
    // sizeof(int)               =         4
    // sizeof(double)            =         8
    // sizeof(DvnnTimePoint)     =        40
    // sizeof(DvnnTimePointList) =        16
    ///////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////
    // 示例输出 (Windows):
    // sizeof(int)               =         4
    // sizeof(double)            =         8
    // sizeof(DvnnTimePoint)     =        40
    // sizeof(DvnnTimePointList) =        16
    ///////////////////////////////////////////////////////////////////////

    // 此处可以添加释放 timePointList 内存的代码，例如 free(timePointList->List); free(timePointList);
}




// 2024-09-10
/**
 * @brief procedure_24104
 * @details
 * 这是一个用于结构分析的函数。它的核心任务是计算两个不同构象之间的
 * 均方根偏差 (Root-Mean-Square Deviation, RMSD)。
 * 函数首先从之前模拟生成的二进制轨迹文件 (`time_point.byte`) 中加载所有原子坐标，
 * 然后调用一个专门的计算函数，找出将两个指定时间点的构象进行最佳叠合
 * (平移和旋转) 后的RMSD值。这个值可以用来衡量两个分子结构之间的相似度。
 */
void procedure_24104()
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 步骤一: 从二进制文件 "time_point.byte" 中读取轨迹数据
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 描述: 与 procedure_24103 类似，首先需要将完整的轨迹数据加载到内存中，
    //       以便后续可以访问任意时间点的原子坐标。

    // 构造包含轨迹数据的输入文件名。
    char fname_input_TIME_POINT_byte[FULL_NAME_MAX_LENGTH];

    // 根据编译时定义的宏，选择适用于当前操作系统（Linux 或 Windows）的文件名。
    #if PROTEIN_RUN_ON_LINUX
        qomd_strmerge_3in1(fname_input_TIME_POINT_byte, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"time_point_linux.byte");
    #endif  // 匹配: #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
       qomd_strmerge_3in1(fname_input_TIME_POINT_byte, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"time_point_win.byte");
    #endif  // 匹配: #if PROTEIN_RUN_ON_WINDOWS

    // 调用函数，从文件中读取数据并返回一个指向已填充数据的 DvnnTimePointList 结构体的指针。
    DvnnTimePointList * timePointList = dvnn_get_timePointList_from_byte_file(fname_input_TIME_POINT_byte);


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 步骤二: 设置参数并计算RMSD
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //====================================================================================================================================//
    // 相关函数:
    // "qomd_shell3_compute_rmsd_by_timePointList" 在 libQomd.a -> qomdShell3.cpp -> qomdCompute.cpp 中定义。
    //   此函数实现了计算RMSD的核心算法，可能包括：
    //   1. 提取指定时间点 timeA 和 timeB 的原子坐标。
    //   2. 计算两个构象的质心并将它们对齐。
    //   3. 构建协方差矩阵。
    //   4. 使用奇异值分解 (SVD) 等数值方法找到最佳旋转矩阵。
    //   5. 应用旋转矩阵并计算最终的RMSD值。
    //====================================================================================================================================//

    // --- RMSD 计算参数定义 ---
    int timeA      = 0;      // 第一个构象的时间戳（例如，初始结构）。
    int timeB      = 900;    // 第二个构象的时间戳。
    int maxAtomNum = 600;    // 用于计算RMSD的最大原子数。可能用于只考虑主链原子等情况。

    // --- 数值算法的精度和迭代控制参数 ---
    // 这些参数用于控制内部数值计算（如SVD）的收敛条件和精度，以平衡计算效率和准确性。
    double rmsd_covMatNormL2_EPSILON = 0.0001;        // 协方差矩阵L2范数的容差。
    double rmsd_svd_EPSILON          = 0.0000000001;  // SVD算法的收敛阈值。
    int    rmsd_svd_ROUND            = 100;           // SVD算法的最大迭代次数。

    // 调用RMSD计算函数。
    double rmsd = qomd_shell3_compute_rmsd_by_timePointList(
                                    timePointList,              // 包含所有轨迹数据的列表
                                    timeA,                      // 第一个时间点
                                    timeB,                      // 第二个时间点
                                    maxAtomNum,                 // 参与计算的原子数
                                    rmsd_covMatNormL2_EPSILON,  // 算法参数
                                    rmsd_svd_EPSILON,           // 算法参数
                                    rmsd_svd_ROUND              // 算法参数
                                    );

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 步骤三: 打印计算结果
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 将计算出的RMSD值打印到控制台。
    printf("######  RMSD = %20.10f  ######\n", rmsd);
    printf("\n");

    // 此处可以添加释放 timePointList 内存的代码。
}