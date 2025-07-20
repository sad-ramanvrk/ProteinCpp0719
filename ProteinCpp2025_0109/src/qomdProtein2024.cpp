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

// 全过程
void all_procedures_2024()
{

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// STEP : set & init base para                                                                                                     //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //================================================================================//
    // struct "QomdBasePara"       is defined  in aBase.h                             //
    // func   "qomd_strcopy_rtrim" is defined  in libQomd.a -> stringFunc.cpp         //
    // func   "qomd_strcopy_rtrim" is declared in aFumcListSim.h                      //
    //================================================================================//

	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("STEP : set & init base para\n");
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    // QomdBasePara处理文件路径
    // 分配QomdBasePara结构体内存（基础参数容器）
	int size_basePara = sizeof(QomdBasePara) * 1;
	QomdBasePara* basePara = (QomdBasePara*)malloc(size_basePara);

    // 初始化文件路径参数为"None"（去除右侧空格）
	char* string_of_None = (char*)"None    ";
	qomd_strcopy_rtrim(basePara->para_DATA_DIR, string_of_None);
    	// para_DATA_DIR为文件路径名
    	// 将 string_of_None 字符串复制到 basePara->para_DATA_DIR，并在复制过程中去除右侧的空格。


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// STEP : read base data from a text file <QOMD_run_para_linux_2024.txt>                                                           //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("STEP : read base data from a text file <QOMD_run_para_linux_2024.txt>\n");
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

	char fname_para_readin[FULL_NAME_MAX_LENGTH];
    	// FULL_NAME_MAX_LENGTH为文件名最大长度，193。在aBase.h中

	// 2024-07-10 revised
    #if PROTEIN_RUN_ON_LINUX
	    qomd_strmerge_3in1(fname_para_readin, DIR_HOME_QOMD_PROTEIN, DIR_SLASH, (char*)"QOMD_run_para_linux_2024.txt");
    	// qomd_strmerge_3in1 将DIR_HOME_QOMD_PROTEIN、DIR_SLASH、"QOMD_run_para_linux_2024.txt"，三个字符串合并成一个字符串，放在fname_para_readin中
    #endif  // match : #if PROTEIN_RUN_ON_LINUX
    
    #if PROTEIN_RUN_ON_WINDOWS
		qomd_strmerge_3in1(fname_para_readin, DIR_HOME_QOMD_PROTEIN, DIR_SLASH, (char*)"QOMD_run_para_win_2024.txt");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS


	// read para from text file
    read_RunPara(fname_para_readin, basePara);




	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // STEP : open output file : test_output_2024.txt                                                                                  //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //================================================================================//
    // para "DIR_OutputData"        is defined in aDir.h                              //
    //================================================================================//

	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("STEP : open output file : <test_output_2024.txt>\n");
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    // initial output file name
    // get output file name
    	// 初始化输出文件名
	char fname_writerecord_test_output[FULL_NAME_MAX_LENGTH];
    qomd_strmerge_3in1(fname_writerecord_test_output, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"test_output_2024.txt");
	printf("TEST INFO write to file :   %s---\n", fname_writerecord_test_output);


	// get output file ptr
    	// 打开文件
	FILE* fptr_writerecord_test_output;

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++ 
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    #if PROTEIN_RUN_ON_LINUX
        // for linux : fopen in <stdio.h>
	    fptr_writerecord_test_output = fopen(fname_writerecord_test_output, "w+");     // w+ : read/write, clean
    #endif  // match : #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
        // for win : fopen_s in <stdio.h>
        fopen_s(&fptr_writerecord_test_output, fname_writerecord_test_output, "w+");   // w+ : read/write, clean
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++ 
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	if (fptr_writerecord_test_output == NULL)
	{
		printf("failed to open output file < %s >\n", fname_writerecord_test_output);
		exit(1);
	}
	else
	{
		printf("successful open output file < %s >\n", fname_writerecord_test_output);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // open output file : test_output_2024.txt                                                                                         //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// STEP : get start time                                                                                                           //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("STEP : get start time\n");
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

	clock_t time_start = clock();




	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  TEST PROCEDURE                                                                                                                 //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 2024-07-10
	if (basePara->para_RUN_PROCEDURE == 101)
	{
		printf("TEST PROCEDURE 2024 : only a test : basePara->para_RUN_PROCEDURE == 101\n");
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  TEST PROCEDURE                                                                                                                 //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 2024-08-29
	/*
	if (basePara->para_RUN_PROCEDURE == 24102)
	{
		printf("TEST PROCEDURE 24102 : only a test : basePara->para_RUN_PROCEDURE == 24102\n");

		procedure_24102(fptr_writerecord_test_output);
	}
	*/

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  TEST PROCEDURE                                                                                                                 //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 2024-08-29
	if (basePara->para_RUN_PROCEDURE == 24103)
	{
		printf("TEST PROCEDURE 24103 : only a test : basePara->para_RUN_PROCEDURE == 24103\n");

		procedure_24103();
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  TEST PROCEDURE                                                                                                                 //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 2024-09-10
	if (basePara->para_RUN_PROCEDURE == 24104)
	{
		printf("TEST PROCEDURE 24103 : only a test : basePara->para_RUN_PROCEDURE == 24104\n");

		procedure_24104();
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  TEST PROCEDURE                                                                                                                 //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2024-11-13
	if (basePara->para_RUN_PROCEDURE == 24105)
	{
	    ///////////////////////////////
	    // 1VII
	    ///////////////////////////////
		printf("TEST PROCEDURE 24105 : only a test : basePara->para_RUN_PROCEDURE == 24105\n");
	    char * proteinBag_file_name_linux = (char*)"proteinBag_1VII_linux.byte";
	    char * proteinBag_file_name_win   = (char*)"proteinBag_1VII_win.byte"  ;
		procedure_24102(fptr_writerecord_test_output, proteinBag_file_name_linux, proteinBag_file_name_win);
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  TEST PROCEDURE                                                                                                                 //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2024-11-13
	if (basePara->para_RUN_PROCEDURE == 24106)
	{
	    ///////////////////////////////
	    // 4UZX
	    ///////////////////////////////
		printf("TEST PROCEDURE 24106 : only a test : basePara->para_RUN_PROCEDURE == 24106\n");
	    char * proteinBag_file_name_linux = (char*)"proteinBag_4UZX_linux.byte";
	    char * proteinBag_file_name_win   = (char*)"proteinBag_4UZX_win.byte"  ;
		procedure_24102(fptr_writerecord_test_output, proteinBag_file_name_linux, proteinBag_file_name_win);
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  TEST PROCEDURE                                                                                                                 //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2024-11-13
	if (basePara->para_RUN_PROCEDURE == 24107)
	{
	    ///////////////////////////////
	    // 2RRK
	    ///////////////////////////////
		printf("TEST PROCEDURE 24107 : only a test : basePara->para_RUN_PROCEDURE == 24107\n");
	    char * proteinBag_file_name_linux = (char*)"proteinBag_2RRK_linux.byte";
	    char * proteinBag_file_name_win   = (char*)"proteinBag_2RRK_win.byte"  ;
		procedure_24102(fptr_writerecord_test_output, proteinBag_file_name_linux, proteinBag_file_name_win);
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  TEST PROCEDURE                                                                                                                 //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2024-11-16
	if (basePara->para_RUN_PROCEDURE == 24115)
	{
	    ///////////////////////////////
	    // 1VII_rod
	    ///////////////////////////////
		printf("TEST PROCEDURE 24105 : only a test : basePara->para_RUN_PROCEDURE == 24105\n");
	    char * proteinBag_file_name_linux = (char*)"proteinBag_1VII_rod_linux.byte";
	    char * proteinBag_file_name_win   = (char*)"proteinBag_1VII_rod_win.byte"  ;
		procedure_24102(fptr_writerecord_test_output, proteinBag_file_name_linux, proteinBag_file_name_win);
	}






	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// STEP : get end time                                                                                                             //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("STEP : get end time\n");
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

	clock_t time_end = clock();



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// STEP : print time to output file                                                                                                //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("STEP : print time to output file\n");
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

	// fps : frame_per_second
    	// 计算总时间。CLOCKS_PER_SEC是一个常量，表示每秒钟的时钟周期数。
	//float total_seconds = ((float)(time_end - time_start - (basePara->para_WAIT_KEY * total_frames))) / ((float)CLOCKS_PER_SEC);
	float total_seconds = ((float)(time_end - time_start)) / ((float)CLOCKS_PER_SEC);
	//float frames_per_second = ((float)total_frames) / total_seconds;

	//printf("FRAMES PER SECOND = %10.3f  fps\n", frames_per_second);
	printf("total_seconds = %10.3f  (seconds) \n", total_seconds);

	// print time
    	// 将时间信息写入文件。将开始时间、结束时间和运行总时间写入到文件中。注意，这里的时间是以时钟周期为单位的，而不是微秒。
	fprintf(fptr_writerecord_test_output, "     time_start = %10d (microSeconds)   time_end = %10d (microSeconds)\n", (int)time_start, (int)time_end);
	fprintf(fptr_writerecord_test_output, "     running time = %10.3f (seconds) \n", (float)((time_end - time_start) / (float)CLOCKS_PER_SEC));
	// 



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// STEP : close output file                                                                                                        //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("STEP : close output file\n");
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

	//use fclose  // in <stdio.h>

	// close fptr_writerecord
	if (fclose(fptr_writerecord_test_output))
	{
		printf("failed to close file < %s >\n", fname_writerecord_test_output);
	}
	else
	{
		printf("successfully close file < %s >\n", fname_writerecord_test_output);
	}



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// STEP : free memory allocated by malloc                                                                                          //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("STEP : free memory allocated by malloc\n");
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

	free(basePara);


}// end of all_procedures_2024()



// 2024-08-29
// 整套蛋白质模拟计算过程，具体步骤包括读取二进制文件初始化数据，设置相关参数，执行迭代计算，并输出模拟结果。
void procedure_24102(FILE* fptr_writerecord_test_output, char * proteinBag_file_name_linux, char * proteinBag_file_name_win)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : proteinBag_1VII get from byte file <proteinBag_1VII_linux.byte>                                                //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 1. 【读取初始数据】从字节文件 proteinBag_1VII.byte 读取数据到 proteinBag_1VII 参数中
    //  proteinBag_1VII 是蛋白质模拟运行数据包的头地址，具体结构不可见，有函数接口用于修改其中的参数和读出数据

    //================================================================================//
    // para "DIR_InputData"        is defined in aDir.h                               //
    //================================================================================//

	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : proteinBag_1VII get from byte file <proteinBag_1VII_linux.byte>\n");
	printf("================================================================================================================\n");

	/*
	// file name
	char fname_input_proteinBag_1VII_byte[FULL_NAME_MAX_LENGTH];

    #if PROTEIN_RUN_ON_LINUX
    	qomd_strmerge_3in1(fname_input_proteinBag_1VII_byte, DIR_HOME_QOMD_PROTEIN, DIR_InputData, (char*)"proteinBag_2RRK_linux.byte");
    #endif  // match : #if PROTEIN_RUN_ON_LINUX
    
    #if PROTEIN_RUN_ON_WINDOWS
    	qomd_strmerge_3in1(fname_input_proteinBag_1VII_byte, DIR_HOME_QOMD_PROTEIN, DIR_InputData, (char*)"proteinBag_2RRK_win.byte");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
	*/

	/*
	// file name
	char fname_input_proteinBag_1VII_byte[FULL_NAME_MAX_LENGTH];

    #if PROTEIN_RUN_ON_LINUX
    	qomd_strmerge_3in1(fname_input_proteinBag_1VII_byte, DIR_HOME_QOMD_PROTEIN, DIR_InputData, (char*)"proteinBag_4UZX_linux.byte");
    #endif  // match : #if PROTEIN_RUN_ON_LINUX
    
    #if PROTEIN_RUN_ON_WINDOWS
    	qomd_strmerge_3in1(fname_input_proteinBag_1VII_byte, DIR_HOME_QOMD_PROTEIN, DIR_InputData, (char*)"proteinBag_4UZX_win.byte");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS

	*/

	/*
	// file name
	char fname_input_proteinBag_1VII_byte[FULL_NAME_MAX_LENGTH];

    #if PROTEIN_RUN_ON_LINUX
    	qomd_strmerge_3in1(fname_input_proteinBag_1VII_byte, DIR_HOME_QOMD_PROTEIN, DIR_InputData, (char*)"proteinBag_1VII_linux.byte");
    #endif  // match : #if PROTEIN_RUN_ON_LINUX
    
    #if PROTEIN_RUN_ON_WINDOWS
    	qomd_strmerge_3in1(fname_input_proteinBag_1VII_byte, DIR_HOME_QOMD_PROTEIN, DIR_InputData, (char*)"proteinBag_1VII_win.byte");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
	*/


	// file name
    // 新增一个数据包，1073个原子：proteinBag_4UZX_linux.zip。代码部分，用新的文件名替换就可以了，其它地方不用修改。
	char fname_input_proteinBag_1VII_byte[FULL_NAME_MAX_LENGTH];

    #if PROTEIN_RUN_ON_LINUX
    	qomd_strmerge_3in1(fname_input_proteinBag_1VII_byte, DIR_HOME_QOMD_PROTEIN, DIR_InputData, (char*)proteinBag_file_name_linux);
    #endif  // match : #if PROTEIN_RUN_ON_LINUX
    
    #if PROTEIN_RUN_ON_WINDOWS
    	qomd_strmerge_3in1(fname_input_proteinBag_1VII_byte, DIR_HOME_QOMD_PROTEIN, DIR_InputData, (char*)proteinBag_file_name_win);
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS



    //====================================================================================================================================//
    // func   "qomd_shell3_get_from_byte_file_for_ProteinPackage" is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdSaveGetByte.cpp     //
    // func   "qomd_shell3_get_from_byte_file_for_ProteinPackage" is declared in aFumcListSim.h                                           //
    //====================================================================================================================================//
    // func   "qomd_shell3_display_ProteinPackage_para"           is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp            //
    // func   "qomd_shell3_display_ProteinPackage_para"           is declared in aFumcListSim.h                                           //
    //====================================================================================================================================//

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // this (void*) ptr is only a (VPIC) for transfer and store                                      //
    // VPIC(void pointer info card) : this card is a void pointer,                                   //
    //                                it point to an address which stores some comprehensive data    //
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    // 加载蛋白质数据包到内存
	// get proteinBag_1VII
	void * proteinBag_1VII = qomd_shell3_get_from_byte_file_for_ProteinPackage(fname_input_proteinBag_1VII_byte);

	// display para
	qomd_shell3_display_ProteinPackage_para(proteinBag_1VII);



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : init pointer revised                                                                                           //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 初始化指针修正
    printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : init pointer revised\n");
	printf("================================================================================================================\n");

    //====================================================================================================================================//
    // func   "qomd_shell3_init_ProteinPackage_pointer_revise" is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp               //
    // func   "qomd_shell3_init_ProteinPackage_pointer_revise" is declared in aFumcListSim.h                                              //
    //====================================================================================================================================//

	/////////////////////////////////////////////////////////////////
	// write vertexList to test file
	/////////////////////////////////////////////////////////////////
	// get output file name
	char fname_writerecord_pointer_revise[FULL_NAME_MAX_LENGTH];
	qomd_strmerge_3in1(fname_writerecord_pointer_revise, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"pointer_revise.txt");

    // revise
	qomd_shell3_init_ProteinPackage_pointer_revise(proteinBag_1VII, fname_writerecord_pointer_revise);




	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : init dvnn sliderList                                                                                           //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	初始化并随机配置DVNN（动态变量神经网络）的滑动参数列表
        printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : init dvnn sliderList\n");
	printf("================================================================================================================\n");

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

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// STEP : init dvnn sliderList
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	// get dvnn sliderList
	/////////////////////////////////////////////////////////////////
	// init sliderList : malloc
	DvnnSliderList* dvnnSliderList = dvnn_init_dvnnSliderList_malloc();

    ///////////////////////////////////////////////////////////////////////////////////////
	// NOTE : this func is only a test example                                           //
	///////////////////////////////////////////////////////////////////////////////////////

	// reset dvnnSliderList by random
	dvnn_reset_dvnnSliderList_by_random(dvnnSliderList);



	//================================//
	//  PROCEDURE STEP                //
	//================================//
    // 加载 hhmo参数面板
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : get hhmoPanel from byte file                                                                                   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : get hhmoPanel\n");
	printf("================================================================================================================\n");
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// STEP ADD : get hhmoPanel from byte file
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //====================================================================================================================================//
    // func   "qomd_shell3_get_from_byte_file_for_HhmoPanel"      is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdSaveGetByte.cpp     //
    // func   "qomd_shell3_get_from_byte_file_for_HhmoPanel"      is declared in aFumcListSim.h                                           //
    //====================================================================================================================================//
    // func   "qomd_shell3_adjust_hhmoPanel_by_dvnnSliderList"    is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp            //
    // func   "qomd_shell3_adjust_hhmoPanel_by_dvnnSliderList"    is declared in aFumcListSim.h                                           //
    //====================================================================================================================================//

	// file name
	char fname_input_hhmoPanel_byte[FULL_NAME_MAX_LENGTH];
    
    #if PROTEIN_RUN_ON_LINUX
       	qomd_strmerge_3in1(fname_input_hhmoPanel_byte, DIR_HOME_QOMD_PROTEIN, DIR_dict, (char*)"hhmoPanel_linux.byte");
    #endif  // match : #if PROTEIN_RUN_ON_LINUX
        
    #if PROTEIN_RUN_ON_WINDOWS
       	qomd_strmerge_3in1(fname_input_hhmoPanel_byte, DIR_HOME_QOMD_PROTEIN, DIR_dict, (char*)"hhmoPanel_win.byte");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
    
	// get hhmoPanel
	void * hhmoPanel = qomd_shell3_get_from_byte_file_for_HhmoPanel(fname_input_hhmoPanel_byte);

	// reset hhmoPanel by dvnn sliderList
	qomd_shell3_adjust_hhmoPanel_by_dvnnSliderList((void *)hhmoPanel, (void *)dvnnSliderList);



    
	//================================//
	//  PROCEDURE STEP                //
	//================================//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : reset hhmo_para                                                                                                //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 重置hhmo_para
    printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : reset hhmo_para\n");
	printf("================================================================================================================\n");

    //====================================================================================================================================//
    // func   "qomd_shell3_adjust_ProteinPackage_hhmo_para_by_HhmoPanel"    is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp  //
    // func   "qomd_shell3_adjust_ProteinPackage_hhmo_para_by_HhmoPanel"    is declared in aFumcListSim.h                                 //
    //====================================================================================================================================//

	// reset hhmo_para
	qomd_shell3_adjust_ProteinPackage_hhmo_para_by_HhmoPanel((void *)proteinBag_1VII, (void *)hhmoPanel);



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : get qomdRunPara                                                                                                //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 获取qomd运行参数
	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : get qomdRunPara\n");
	printf("================================================================================================================\n");

	char fname_qomd_RunPara_readin[FULL_NAME_MAX_LENGTH];

	// 2024-07-10 revised
    #if PROTEIN_RUN_ON_LINUX
	    qomd_strmerge_3in1(fname_qomd_RunPara_readin, DIR_HOME_QOMD_PROTEIN, DIR_SLASH, (char*)"cppCuda_run_para_linux_2024.txt");
    #endif  // match : #if PROTEIN_RUN_ON_LINUX
    
    #if PROTEIN_RUN_ON_WINDOWS
		qomd_strmerge_3in1(fname_qomd_RunPara_readin, DIR_HOME_QOMD_PROTEIN, DIR_SLASH, (char*)"cppCuda_run_para_win_2024.txt");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS


    //====================================================================================================================================//
    // func   "qomd_shell3_get_qomd_RunPara_from_configFile" is defined  in libQomd.a -> qomdShell3.cpp  ->  readFile.cpp                 //
    // func   "qomd_shell3_get_qomd_RunPara_from_configFile" is declared in aFumcListSim.h                                                //
    //====================================================================================================================================//

	InputPara * qomdRunPara = qomd_shell3_get_qomd_RunPara_from_configFile(fname_qomd_RunPara_readin);

	printf("qomdRunPara ptr %p  \n", qomdRunPara);
	printf("get_qomdRunPara_from_configFile DONE !\n");

	///////////////////////////////////////////////////////////////////////////////
	// reset dir by predefined para
	///////////////////////////////////////////////////////////////////////////////
	// 通过预定义的 para 重置目录
	qomd_strcopy_rtrim(qomdRunPara->dir_home_qomd_protein, DIR_HOME_QOMD_PROTEIN);
	printf("qomdRunPara->dir_home_qomd_protein : reset done!\n");

	qomd_strcopy_rtrim(qomdRunPara->dir_dictionary, DIR_DICTIONARY);
	printf("qomdRunPara->dir_dictionary        : reset done!\n");


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
	// PROCEDURE_STEP : reset qomdRunPara in proteinBag                                                                                //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 在proteinBag中重置qomdRunPara
	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : reset qomdRunPara in proteinBag\n");
	printf("================================================================================================================\n");

    //====================================================================================================================================//
    // func   "qomd_shell3_init_ProteinPackage_qomdRunPara" is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp                  //
    // func   "qomd_shell3_init_ProteinPackage_qomdRunPara" is declared in aFumcListSim.h                                                 //
    //====================================================================================================================================//

	// init qomdInputPara in proteinBag_1VII
	qomd_shell3_init_ProteinPackage_qomdRunPara(proteinBag_1VII, qomdRunPara);



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : init qomdEvaluatePara                                                                                          //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : init qomdEvaluatePara\n");
	printf("================================================================================================================\n");


	////////////////////////////////////////
	// set & init qomdEvaluatePara
	////////////////////////////////////////

	int size_qomdEvaluatePara = sizeof(EvaluatePara) * 1;
	EvaluatePara* qomdEvaluatePara = (EvaluatePara*)malloc(size_qomdEvaluatePara);

	printf("qomdEvaluatePara ptr %p       ", qomdEvaluatePara     );
	printf("qomdEvaluatePara size %10d  \n", size_qomdEvaluatePara);





	//================================//
	//  PROCEDURE STEP                //
	//================================//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : switch_ProteinPackage_Osci_Harmonic_by_HHMO                                                                    //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : switch_ProteinPackage_Osci_Harmonic_by_HHMO\n");
	printf("================================================================================================================\n");

    //====================================================================================================================================//
    // func   "qomd_shell3_switch_ProteinPackage_Osci_Harmonic_by_QOMD"    is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp   //
    // func   "qomd_shell3_switch_ProteinPackage_Osci_Harmonic_by_QOMD"    is declared in aFumcListSim.h                                  //
    //====================================================================================================================================//
    //====================================================================================================================================//
    // func   "qomd_shell3_switch_ProteinPackage_Osci_Harmonic_by_HHMO"    is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp   //
    // func   "qomd_shell3_switch_ProteinPackage_Osci_Harmonic_by_HHMO"    is declared in aFumcListSim.h                                  //
    //====================================================================================================================================//

	// 1 = use qomd(old fixed para), 2 = use hhmo(new adjustable para), add 2024-10-70
	if (qomdRunPara->PARA_FREQ == 1)
	{
		qomd_shell3_switch_ProteinPackage_Osci_Harmonic_by_QOMD((void *)proteinBag_1VII);
	}
	else if (qomdRunPara->PARA_FREQ == 2)
	{
		qomd_shell3_switch_ProteinPackage_Osci_Harmonic_by_HHMO((void *)proteinBag_1VII);
	}
	else
	{
		printf("INVALID PARA_FREQ, PARA_FREQ must be 1 or 2. \n");
		exit(0);
	}
	



	//================================//
	//  PROCEDURE STEP                //
	//================================//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : switch_ProteinPackage_elccol_charge_by_vertex_hhmoCharge                                                       //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : switch_ProteinPackage_elccol_charge_by_vertex_hhmoCharge\n");
	printf("================================================================================================================\n");

    //=================================================================================================================================================//
    // func   "qomd_shell3_switch_ProteinPackage_elccol_charge_by_vertex_qomdCharge"    is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp   //
    // func   "qomd_shell3_switch_ProteinPackage_elccol_charge_by_vertex_qomdCharge"    is declared in aFumcListSim.h                                  //
    //=================================================================================================================================================//
    //=================================================================================================================================================//
    // func   "qomd_shell3_switch_ProteinPackage_elccol_charge_by_vertex_hhmoCharge"    is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp   //
    // func   "qomd_shell3_switch_ProteinPackage_elccol_charge_by_vertex_hhmoCharge"    is declared in aFumcListSim.h                                  //
    //=================================================================================================================================================//

	// 1 = use qomd(old fixed para), 2 = use hhmo(new adjustable para), add 2024-10-70
	if (qomdRunPara->PARA_ELEC == 1)
	{
		qomd_shell3_switch_ProteinPackage_elccol_charge_by_vertex_qomdCharge((void *)proteinBag_1VII);
	}
	else if (qomdRunPara->PARA_ELEC == 2)
	{
		qomd_shell3_switch_ProteinPackage_elccol_charge_by_vertex_hhmoCharge((void *)proteinBag_1VII);
	}
	else
	{
		printf("INVALID PARA_ELEC, PARA_ELEC must be 1 or 2. \n");
		exit(0);
	}



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : reset CudaGridBlock                                                                                            //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : reset CudaGridBlock\n");
	printf("================================================================================================================\n");

    //====================================================================================================================================//
    // func   "qomd_shell3_init_ProteinPackage_CudaGridBlock" is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp                //
    // func   "qomd_shell3_init_ProteinPackage_CudaGridBlock" is declared in aFumcListSim.h                                               //
    //====================================================================================================================================//

	// init CudaGridBlock
	qomd_shell3_init_ProteinPackage_CudaGridBlock(proteinBag_1VII);




	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : init osci data before run                                                                                      //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : init osci data before run\n");
	printf("================================================================================================================\n");

    //====================================================================================================================================//
    // func   "qomd_shell3_initOsci_normalState_limitDeviation"         is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp      //
    // func   "qomd_shell3_initOsci_normalState_limitDeviation"         is declared in aFumcListSim.h                                     //
    //====================================================================================================================================//
    // func   "qomd_shell3_init_osci_set_DeltaR_index_All"              is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdInit.cpp      //
    // func   "qomd_shell3_init_osci_set_DeltaR_index_All"              is declared in aFumcListSim.h                                     //
    //====================================================================================================================================//
    // func   "qomd_shell3_init_osci_set_DeltaR_to_zero_All"            is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdCompute.cpp   //
    // func   "qomd_shell3_init_osci_set_DeltaR_to_zero_All"            is declared in aFumcListSim.h                                     //
    //====================================================================================================================================//
    // func   "qomd_shell3_computeVertex_reset_Position_by_globalPoint" is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdCompute.cpp   //
    // func   "qomd_shell3_computeVertex_reset_Position_by_globalPoint" is declared in aFumcListSim.h                                     //
    //====================================================================================================================================//

    // init of normalState_limitDeviation
	qomd_shell3_initOsci_normalState_limitDeviation(proteinBag_1VII);

    // init DeltaR_index
	qomd_shell3_init_osci_set_DeltaR_index_All(proteinBag_1VII);

	// init all DeltaR_to zero
	qomd_shell3_init_osci_set_DeltaR_to_zero_All(proteinBag_1VII);

    // get position data by globalPoint : GvnnGraph globalPoint ----> GvnnForce position
	qomd_shell3_computeVertex_reset_Position_by_globalPoint(proteinBag_1VII);



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : init dvnn vertexList                                                                                           //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : init dvnn vertexList\n");
	printf("================================================================================================================\n");

    //================================================================================//
    // struct "DvnnAtomicVertexList"       is defined  in aDvnn.h                     //
    //================================================================================//

    //====================================================================================================================================//
    // func   "dvnn_init_malloc_vertexList"         is defined  in dvnnFunc.cpp                                                           //
    // func   "dvnn_init_malloc_vertexList"         is declared in aFumcListSim.h                                                         //
    //====================================================================================================================================//
    //====================================================================================================================================//
    // func   "qomd_shell3_copy_data_from_proteinBag_dvnnVertexList"      is defined  in libQomd.a -> qomdShell3.cpp  ->  stringFunc.cpp  //
    // func   "qomd_shell3_copy_data_from_proteinBag_dvnnVertexList"      is declared in aFumcListSim.h                                   //
    //====================================================================================================================================//

    //====================================================================================================================================//
    // func   "dvnn_write_dvnnVertexList_to_test_file"         is defined  in dvnnFunc.cpp                                                //
    // func   "dvnn_write_dvnnVertexList_to_test_file"         is declared in aFumcListSim.h                                              //
    //====================================================================================================================================//

	/////////////////////////////////////////////////////////////////
	// get dvnn vertexList
	/////////////////////////////////////////////////////////////////
	// init vertexList : malloc
	DvnnAtomicVertexList* dvnnVertexList = dvnn_init_malloc_vertexList(proteinBag_1VII);

	// init vertexList : copy init data
	qomd_shell3_copy_data_from_proteinBag_dvnnVertexList(dvnnVertexList, proteinBag_1VII);


	/////////////////////////////////////////////////////////////////
	// write vertexList to test file
	/////////////////////////////////////////////////////////////////
	// get output file name
	char fname_writerecord_dvnn_vertexList_1[FULL_NAME_MAX_LENGTH];
	qomd_strmerge_3in1(fname_writerecord_dvnn_vertexList_1, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"dvnn_vertexList_1.txt");

	// write to test file
	dvnn_write_dvnnVertexList_to_test_file(dvnnVertexList, fname_writerecord_dvnn_vertexList_1);


	/////////////////////////////////////////////////////////////////
	// write vertexList to pdb file
	/////////////////////////////////////////////////////////////////
	// get output file name
	char fname_writerecord_dvnn_vertexList_pdb_1[FULL_NAME_MAX_LENGTH];
	qomd_strmerge_3in1(fname_writerecord_dvnn_vertexList_pdb_1, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"dvnn_vertexList_pdb_1.txt");

    //====================================================================================================================================//
    // func   "qomd_shell3_write_dvnnVertexList_to_pdb_file"      is defined  in libQomd.a -> qomdShell3.cpp  ->  gvnnName.cpp            //
    // func   "qomd_shell3_write_dvnnVertexList_to_pdb_file"      is declared in aFumcListSim.h                                           //
    //====================================================================================================================================//

	// write to pdb file
	qomd_shell3_write_dvnnVertexList_to_pdb_file(dvnnVertexList, fname_writerecord_dvnn_vertexList_pdb_1);



	// ProteinCpp2024_1109，增加了输出四个列表。
    // 具体内容参见相关新增数据结构，在aDvnn.h。四种振子的所有列表
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : init dvnnBondList                                                                                              //
	// PROCEDURE_STEP : init dvnnAngleList                                                                                             //
	// PROCEDURE_STEP : init dvnnDihedralList                                                                                          //
	// PROCEDURE_STEP : init dvnnPlaneList                                                                                             //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : init dvnn bondList angleList dihedralList planeList\n");
	printf("================================================================================================================\n");

    //================================================================================//
    // struct "DvnnForceBondList    "       is defined  in aDvnn.h                    //
    // struct "DvnnForceAngleList   "       is defined  in aDvnn.h                    //
    // struct "DvnnForceDihedralList"       is defined  in aDvnn.h                    //
    // struct "DvnnForcePlaneList   "       is defined  in aDvnn.h                    //
    //================================================================================//
    //====================================================================================================================================//
    // func   "dvnn_init_malloc_dvnnBondList    "         is defined  in dvnnFunc.cpp, is declared in aFumcListSim.h                      //
    // func   "dvnn_init_malloc_dvnnAngleList   "         is defined  in dvnnFunc.cpp, is declared in aFumcListSim.h                      //
    // func   "dvnn_init_malloc_dvnnDihedralList"         is defined  in dvnnFunc.cpp, is declared in aFumcListSim.h                      //
    // func   "dvnn_init_malloc_dvnnPlaneList   "         is defined  in dvnnFunc.cpp, is declared in aFumcListSim.h                      //
    //====================================================================================================================================//

	/////////////////////////////////////////////////////////////////
	// get dvnn data
	/////////////////////////////////////////////////////////////////
	// init : malloc
	DvnnForceBondList    * dvnnBondList     = dvnn_init_malloc_dvnnBondList    (proteinBag_1VII);
	DvnnForceAngleList   * dvnnAngleList    = dvnn_init_malloc_dvnnAngleList   (proteinBag_1VII);
	DvnnForceDihedralList* dvnnDihedralList = dvnn_init_malloc_dvnnDihedralList(proteinBag_1VII);
	DvnnForcePlaneList   * dvnnPlaneList    = dvnn_init_malloc_dvnnPlaneList   (proteinBag_1VII);

    //==================================================================================================================================================//
    // func   "qomd_shell3_copy_data_from_proteinBag_dvnnBondList    "      is defined  in libQomd.a -> qomdShell3.cpp , is declared in aFumcListSim.h  //
    // func   "qomd_shell3_copy_data_from_proteinBag_dvnnAngleList   "      is defined  in libQomd.a -> qomdShell3.cpp , is declared in aFumcListSim.h  //
    // func   "qomd_shell3_copy_data_from_proteinBag_dvnnDihedralList"      is defined  in libQomd.a -> qomdShell3.cpp , is declared in aFumcListSim.h  //
    // func   "qomd_shell3_copy_data_from_proteinBag_dvnnPlaneList   "      is defined  in libQomd.a -> qomdShell3.cpp , is declared in aFumcListSim.h  //
    //==================================================================================================================================================//

	// copy init fixed data
	qomd_shell3_copy_data_from_proteinBag_dvnnBondList    (dvnnBondList    , (void*) proteinBag_1VII);
    qomd_shell3_copy_data_from_proteinBag_dvnnAngleList   (dvnnAngleList   , (void*) proteinBag_1VII);
    qomd_shell3_copy_data_from_proteinBag_dvnnDihedralList(dvnnDihedralList, (void*) proteinBag_1VII);
    qomd_shell3_copy_data_from_proteinBag_dvnnPlaneList   (dvnnPlaneList   , (void*) proteinBag_1VII);


	/////////////////////////////////////////////////////////////////
	// write all List to test file
	/////////////////////////////////////////////////////////////////
	// get output file name
	char fname_writerecord_dvnn_all_list_init[FULL_NAME_MAX_LENGTH];
	qomd_strmerge_3in1(fname_writerecord_dvnn_all_list_init, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"dvnn_all_list_init.txt");

	// write to test file
    dvnn_write_dvnnAllList_to_test_file(
                                         (DvnnAtomicVertexList  *) dvnnVertexList                 ,
                                         (DvnnForceBondList     *) dvnnBondList                   ,
                                         (DvnnForceAngleList    *) dvnnAngleList                  ,
                                         (DvnnForceDihedralList *) dvnnDihedralList               ,
                                         (DvnnForcePlaneList    *) dvnnPlaneList                  ,
	                                     (char                  *) fname_writerecord_dvnn_all_list_init );



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : init dvnn timePointList                                                                                        //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : init dvnn timePointList\n");
	printf("================================================================================================================\n");

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

	/////////////////////////////////////////////////////////////////
	// get dvnn timePointList
	/////////////////////////////////////////////////////////////////
	// init vertexList : malloc
	DvnnTimePointList* timePointList = dvnn_init_timePointList_malloc(dvnnVertexList);

	// output file name
	char fname_output_TIME_POINT_byte[FULL_NAME_MAX_LENGTH];

    #if PROTEIN_RUN_ON_LINUX
    	qomd_strmerge_3in1(fname_output_TIME_POINT_byte, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"time_point_linux.byte");
    #endif  // match : #if PROTEIN_RUN_ON_LINUX
    
    #if PROTEIN_RUN_ON_WINDOWS
    	qomd_strmerge_3in1(fname_output_TIME_POINT_byte, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"time_point_win.byte");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS

	//qomd_strmerge_3in1(fname_output_TIME_POINT_byte, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"time_point.byte");
	//qomd_strmerge_3in1(fname_output_TIME_POINT_byte, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"time_point.byte");

	//int totalStamps = 1001;                          // init : 0, run data :1~1000
	int totalStamps = qomdRunPara->RUN_ROUNDS;         // init : 0, run data :1 ~ RUN_ROUNDS
	int totalVeretx = dvnnVertexList->actItem;         // 596 : 1VII

	// create byte file & save para
	dvnn_save_timePointList_paraData_to_byte_file(fname_output_TIME_POINT_byte, totalStamps, totalVeretx);

	// append save init : timeStamp = 0
	dvnn_save_timePointList_listData_to_byte_file(fname_output_TIME_POINT_byte, timePointList);



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : qomd_femto_iterator set run method                                                                             //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : qomd_femto_iterator set run method\n");
	printf("================================================================================================================\n");

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

	// declare a pointer to function
	void (*qomd_femto_iterator)(void* proteinBag_vpic);

	// RUN_METHOD : 1 = COUVDW or 2 = ELCCOL or 3 = only_4_osci
	// RUN_MODE   : 1 = SER    or 2 = PAR
	// assign a function to the pointer
	     if ((qomdRunPara->RUN_METHOD == 1) && (qomdRunPara->RUN_MODE == 1)) { qomd_femto_iterator = &qomd_shell3_femto_iterator_ser_use_CouVdw; }
	else if ((qomdRunPara->RUN_METHOD == 1) && (qomdRunPara->RUN_MODE == 2)) { qomd_femto_iterator = &qomd_shell3_femto_iterator_per_use_CouVdw; }
	else if ((qomdRunPara->RUN_METHOD == 2) && (qomdRunPara->RUN_MODE == 1)) { qomd_femto_iterator = &qomd_shell3_femto_iterator_ser_use_ElcCol; }
	else if ((qomdRunPara->RUN_METHOD == 2) && (qomdRunPara->RUN_MODE == 2)) { qomd_femto_iterator = &qomd_shell3_femto_iterator_per_use_ElcCol; }
	else if ((qomdRunPara->RUN_METHOD == 3) && (qomdRunPara->RUN_MODE == 2)) { qomd_femto_iterator = &qomd_shell3_femto_iterator_per_use_4_osci; }
	else { printf("INVALID RUN_METHOD or INVALID RUN_MODE.\n");		exit(1); }


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : iterate                                                                                                        //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : iterate\n");
	printf("================================================================================================================\n");

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

	////////////////////////////////////////
	// iterate
	////////////////////////////////////////

	int count = -1;
	int write_freq = qomdRunPara->WRITE_FREQ;
	int max_round  = qomdRunPara->RUN_ROUNDS;               // example : RUN_ROUNDS = 1000

	for (int round = 0; round < max_round; round++)         // round = 0 ~ 999
	{
		// use the pointer function
		qomd_femto_iterator(proteinBag_1VII);


		////////////////////////////////////////////////////////////
		// count and temperature                                  //
		////////////////////////////////////////////////////////////
		count = count + 1;
		if (count == write_freq)
		{
			count = 0;

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


		/////////////////////////////////////////////////////////////////
		// get dvnn vertexList
		/////////////////////////////////////////////////////////////////
		/*


		// copy position data proteinBag_1VII ---> vertexList
		qomd_shell3_copy_current_from_proteinBag_dvnnVertexList(dvnnVertexList, proteinBag_1VII);

		// copy position data vertexList ---> timePointList
		int timeStamp = round + 1;                // timeStamp = 1 ~ RUN_ROUNDS, example 1~1000
		dvnn_copy_vertexList_to_timePointList_by_timeStamp(timePointList, dvnnVertexList, timeStamp);

		// append : with current timeStamp
		// append : copy position data timePointList ---> fname_output_TIME_POINT_byte
		dvnn_save_timePointList_listData_to_byte_file(fname_output_TIME_POINT_byte, timePointList);


		*/



		/////////////////////////////////////////////////////////////////
		// write proteinBag
		/////////////////////////////////////////////////////////////////

		if (round == 100)
		{
		    // test print
		    qomd_shell3_write_proteinBag_output_10_files((void *) proteinBag_1VII);

		}


	}// end for




	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : output dvnn vertexList                                                                                         //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : output dvnn vertexList\n");
	printf("================================================================================================================\n");


	/////////////////////////////////////////////////////////////////
	// get dvnn vertexList
	/////////////////////////////////////////////////////////////////
	// copy position data after qomd
	qomd_shell3_copy_current_from_proteinBag_dvnnVertexList(dvnnVertexList, proteinBag_1VII);

	/////////////////////////////////////////////////////////////////
	// write vertexList to test file
	/////////////////////////////////////////////////////////////////
	// get output file name
	char fname_writerecord_dvnn_vertexList_2[FULL_NAME_MAX_LENGTH];
	qomd_strmerge_3in1(fname_writerecord_dvnn_vertexList_2, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"dvnn_vertexList_2.txt");

	// write to test file
	dvnn_write_dvnnVertexList_to_test_file(dvnnVertexList, fname_writerecord_dvnn_vertexList_2);


	/////////////////////////////////////////////////////////////////
	// write vertexList to pdb file
	/////////////////////////////////////////////////////////////////
	// get output file name
	char fname_writerecord_dvnn_vertexList_pdb_2[FULL_NAME_MAX_LENGTH];
	qomd_strmerge_3in1(fname_writerecord_dvnn_vertexList_pdb_2, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"dvnn_vertexList_pdb_2.txt");

	// write to pdb file
	qomd_shell3_write_dvnnVertexList_to_pdb_file(dvnnVertexList, fname_writerecord_dvnn_vertexList_pdb_2);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCEDURE_STEP : output all List                                                                                                //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("================================================================================================================\n");
	printf("PROCEDURE_STEP : output all List\n");
	printf("================================================================================================================\n");
	
	// 当前运行数据，每一步运算后，都可以输出到四个列表
    //=====================================================================================================================================================//
    // func   "qomd_shell3_copy_current_from_proteinBag_dvnnBondList    "      is defined  in libQomd.a -> qomdShell3.cpp , is declared in aFumcListSim.h  //
    // func   "qomd_shell3_copy_current_from_proteinBag_dvnnAngleList   "      is defined  in libQomd.a -> qomdShell3.cpp , is declared in aFumcListSim.h  //
    // func   "qomd_shell3_copy_current_from_proteinBag_dvnnDihedralList"      is defined  in libQomd.a -> qomdShell3.cpp , is declared in aFumcListSim.h  //
    // func   "qomd_shell3_copy_current_from_proteinBag_dvnnPlaneList   "      is defined  in libQomd.a -> qomdShell3.cpp , is declared in aFumcListSim.h  //
    //=====================================================================================================================================================//

	// copy current data
	qomd_shell3_copy_current_from_proteinBag_dvnnBondList    (dvnnBondList    , (void*) proteinBag_1VII);
    qomd_shell3_copy_current_from_proteinBag_dvnnAngleList   (dvnnAngleList   , (void*) proteinBag_1VII);
    qomd_shell3_copy_current_from_proteinBag_dvnnDihedralList(dvnnDihedralList, (void*) proteinBag_1VII);
    qomd_shell3_copy_current_from_proteinBag_dvnnPlaneList   (dvnnPlaneList   , (void*) proteinBag_1VII);

	/////////////////////////////////////////////////////////////////
	// write all List to test file
	/////////////////////////////////////////////////////////////////
	// get output file name
	char fname_writerecord_dvnn_all_list_end[FULL_NAME_MAX_LENGTH];
	qomd_strmerge_3in1(fname_writerecord_dvnn_all_list_end, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"dvnn_all_list_end.txt");

	// write to test file
    dvnn_write_dvnnAllList_to_test_file(
                                         (DvnnAtomicVertexList  *) dvnnVertexList                 ,
                                         (DvnnForceBondList     *) dvnnBondList                   ,
                                         (DvnnForceAngleList    *) dvnnAngleList                  ,
                                         (DvnnForceDihedralList *) dvnnDihedralList               ,
                                         (DvnnForcePlaneList    *) dvnnPlaneList                  ,
	                                     (char                  *) fname_writerecord_dvnn_all_list_end );


}// end of void procedure_24102()




// 2024-08-29
void procedure_24103()
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// read in file "/home/m1/ProteinCpp/OutputData/time_point.byte"                                             //
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //====================================================================================================================================//
    // func   "dvnn_get_timePointList_from_byte_file"       is defined  in dvnnFunc.cpp                                                   //
    // func   "dvnn_get_timePointList_from_byte_file"       is declared in aFumcListSim.h                                                 //
    //====================================================================================================================================//

	// input file name
    char fname_input_TIME_POINT_byte[FULL_NAME_MAX_LENGTH];

	#if PROTEIN_RUN_ON_LINUX
	    qomd_strmerge_3in1(fname_input_TIME_POINT_byte, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"time_point_linux.byte");
	#endif  // match : #if PROTEIN_RUN_ON_LINUX

	#if PROTEIN_RUN_ON_WINDOWS
		qomd_strmerge_3in1(fname_input_TIME_POINT_byte, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"time_point_win.byte");
	#endif  // match : #if PROTEIN_RUN_ON_WINDOWS
		

	// read data
	DvnnTimePointList * timePointList = dvnn_get_timePointList_from_byte_file(fname_input_TIME_POINT_byte);

	// print
	for (int i = 0; i < timePointList->actItem; i++)
	{
		if ( (timePointList->List[i].t<5) || (timePointList->List[i].t>995) )
		{
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


	// print 
	printf(" sizeof(int)               =%10d \n", (int)sizeof(int)               );
	printf(" sizeof(double)            =%10d \n", (int)sizeof(double)            );
	printf(" sizeof(DvnnTimePoint)     =%10d \n", (int)sizeof(DvnnTimePoint)     );
	printf(" sizeof(DvnnTimePointList) =%10d \n", (int)sizeof(DvnnTimePointList) );

    ///////////////////////////////////////////////////////////////////////
	// linux :
	//sizeof(int)               =         4 
    //sizeof(double)            =         8 
    //sizeof(DvnnTimePoint)     =        40 
    //sizeof(DvnnTimePointList) =        16 
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// windows :
	//sizeof(int)               =         4 
	//sizeof(double)            =         8 
	//sizeof(DvnnTimePoint)     =        40 
	//sizeof(DvnnTimePointList) =        16 
	///////////////////////////////////////////////////////////////////////


}




// 2024-09-10
void procedure_24104()
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// read in file "/home/m1/ProteinCpp/OutputData/time_point.byte"                                             //
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// input file name
    char fname_input_TIME_POINT_byte[FULL_NAME_MAX_LENGTH];

	#if PROTEIN_RUN_ON_LINUX
	    qomd_strmerge_3in1(fname_input_TIME_POINT_byte, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"time_point_linux.byte");
	#endif  // match : #if PROTEIN_RUN_ON_LINUX

	#if PROTEIN_RUN_ON_WINDOWS
		qomd_strmerge_3in1(fname_input_TIME_POINT_byte, DIR_HOME_QOMD_PROTEIN, DIR_OutputData, (char*)"time_point_win.byte");
	#endif  // match : #if PROTEIN_RUN_ON_WINDOWS
		

	// read data
	DvnnTimePointList * timePointList = dvnn_get_timePointList_from_byte_file(fname_input_TIME_POINT_byte);


    //====================================================================================================================================//
    // func   "qomd_shell3_compute_rmsd_by_timePointList"                is defined  in libQomd.a -> qomdShell3.cpp  ->  qomdCompute.cpp  //
    // func   "qomd_shell3_compute_rmsd_by_timePointList"                is declared in aFumcListSim.h                                    //
    //====================================================================================================================================//

	int timeA      = 0  ; 
	int timeB      = 900;
	int maxAtomNum = 600;

    double rmsd_covMatNormL2_EPSILON = 0.0001      ;
    double rmsd_svd_EPSILON          = 0.0000000001;
    int    rmsd_svd_ROUND            = 100         ;

    double rmsd = qomd_shell3_compute_rmsd_by_timePointList(
	                                timePointList, 
	                                timeA, 
	                                timeB,
	                                maxAtomNum,
                                    rmsd_covMatNormL2_EPSILON,
                                    rmsd_svd_EPSILON,
                                    rmsd_svd_ROUND
	                                );

    // test print
	printf("######  RMSD = %20.10f  ######\n", rmsd);
    printf("\n");
}