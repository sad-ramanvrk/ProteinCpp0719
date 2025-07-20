
/**
 * @Version 1.0
 * @Date 2023-12-19
 * @Author Zhaohui Meng
 * @Affiliation Department of Computer Science, Hohai University, Nanjing, China.
 */





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
//#include "aBase.h"



// all functions
#include "aFuncListSim.h"

///////////////////////////
// functions declare
///////////////////////////



// 2024-10-07
// get a pseudo_random value in [-1,+1]
double dvnn_getNew_Real_neg1_to_pos1()
// 伪随机数生成器来生成一个在区间 [-1, +1] 内的双精度浮点数。
// 这个函数通过一个简单的线性同余生成器 (LCG) 来产生伪随机数，初始范围在 [0, 1]，然后将其变换为 [-1, +1]，并返回。
// 每次调用时，随机数生成器都会基于前一次的结果更新，从而生成一个新的伪随机数。
{
	// 确保每次函数调用后的 Rand 值都会基于上一次的值进行更新
	static int Rand = 1;

	// 随机数生成器中的“种子”值
	int seed1 = 123;
	int seed0 = 59;
	// module 是模运算的值，确保生成的随机数始终在 [0, module] 之间，
	// 这里 module = 65536，也就是 2^16。
	int module = 65536;

	// 使用 Rand 的前一个值与种子 seed1 和 seed0 进行线性同余生成运算，并对 module 取模，保证新的 Rand 值在 [0, module] 之间
	Rand = (seed1 * Rand + seed0) % module;

	// random_x : [0,1]
	// 生成的 Rand 值是 [0, module] 之间的整数，除以 module 后，random_x 被标准化为 [0, 1] 之间的浮点数
	double random_x = ((double)Rand) / ((double)module);

	// random_x : [-1,+1]
	// 接着通过减去 0.5 再乘以 2，将 random_x 从 [0, 1] 映射到 [-1, 1]。这个步骤是为了将伪随机数范围从 [0, 1] 变换到 [-1, 1]
	double random_y = ( (random_x - 0.5) * 2.0 );

	return random_y;

}



// 2024-10-07
DvnnSliderList* dvnn_init_dvnnSliderList_malloc()
// 通过动态分配内存，初始化了一个 DvnnSliderList 结构体，该结构体包含 1600 个 DvnnSlider 元素。
// 它的主要作用是创建一个包含滑动条的列表，并对列表中的每个滑动条进行初始值设定。
// min=-1，act=0，maxa=1
{
	//================================================================================//
	// struct "DvnnSliderList"       is defined  in aDvnn.h                           //
	//================================================================================//
	// struct "DvnnSlider"           is defined  in aDvnn.h                           //
	//================================================================================//

	// new an empty sliderList
	DvnnSliderList* sliderList = (DvnnSliderList*)malloc(sizeof(DvnnSliderList));

	sliderList->actItem = 1600;  // 当前有效的滑动条数量
	sliderList->maxItem = 1600;  // 滑动条的最大数量

	sliderList->List = (DvnnSlider*)malloc((sliderList->maxItem) * sizeof(DvnnSlider));


	// set data to zero
	for (int i = 0; i < sliderList->maxItem; i++)
	{
		sliderList->List[i].min = -1.0;  // 设置滑动条的最小值为 -1.0
		sliderList->List[i].act =  0.0;  // 设置滑动条的当前值为 0.0
		sliderList->List[i].max = +1.0;  // 设置滑动条的最大值为 +1.0
	}

	// return
	return sliderList;
}


// 2024-10-07
void dvnn_reset_dvnnSliderList_by_random(DvnnSliderList* sliderList)
// 将滑动条列表中的每个滑动条的当前值（act）随机化为一个在 [-1, +1] 之间的值，同时确保随机值不会超出该范围。
{
	// 遍历 sliderList 中的每个 DvnnSlider 元素
	for (int i = 0; i < sliderList->actItem; i++)
	{
		// 生成一个随机数，范围为 [-1, 1]
		double random_neg1_to_pos1 = dvnn_getNew_Real_neg1_to_pos1();

		// 限制随机数的范围，确保不会超过 [-1, 1] 的边界
		if (random_neg1_to_pos1 >  1.0) { random_neg1_to_pos1 =  1.0; }  // 如果大于 1，强制设为 1
		if (random_neg1_to_pos1 < -1.0) { random_neg1_to_pos1 = -1.0; }  // 如果小于 -1，强制设为 -1

		// 将生成的随机数赋值给当前 DvnnSlider 的 act 成员
		sliderList->List[i].act = random_neg1_to_pos1;

		// 测试打印，打印滑动条的索引、最小值、当前值和最大值
		// printf("i= %6d   ", i);
		// printf("slider_min= %12.8f   ", sliderList->List[i].min);
		// printf("slider_act= %12.8f   ", sliderList->List[i].act);
		// printf("slider_max= %12.8f   ", sliderList->List[i].max);
		// printf("\n");
	}
}



// 2023-10-09
// 初始化一个DvnnAtomicVertexList结构体，并为其分配内存。
DvnnAtomicVertexList * dvnn_init_malloc_vertexList(void * proteinBag)
{
    //================================================================================//
    // struct "DvnnAtomicVertexList"       is defined  in aDvnn.h                     //
    //================================================================================//
    // struct "DvnnAtomicVertex"           is defined  in aDvnn.h                     //
    //================================================================================//

    //====================================================================================================================================//
    // func   "qomd_shell3_get_vertexList_actItem_by_proteinBag"         is defined  in libQomd.a -> qomdShell3.cpp                       //
    // func   "qomd_shell3_get_vertexList_actItem_by_proteinBag"         is declared in aFumcListSim.h                                    //
    //====================================================================================================================================//
    // func   "qomd_shell3_get_vertexList_maxItem_by_proteinBag"         is defined  in libQomd.a -> qomdShell3.cpp                       //
    // func   "qomd_shell3_get_vertexList_maxItem_by_proteinBag"         is declared in aFumcListSim.h                                    //
    //====================================================================================================================================//

	// new an empty vertexList
	DvnnAtomicVertexList * vertexList = (DvnnAtomicVertexList*)malloc(sizeof(DvnnAtomicVertexList));	// 原子顶点列表


	vertexList->actItem = qomd_shell3_get_vertexList_actItem_by_proteinBag(proteinBag);		// 从蛋白质数据中获取实际顶点的数量。
	vertexList->maxItem = qomd_shell3_get_vertexList_maxItem_by_proteinBag(proteinBag);		// 从蛋白质数据中获取顶点的最大数量。
	// 原子链
    vertexList->List = (DvnnAtomicVertex*) malloc( (vertexList->maxItem) * sizeof(DvnnAtomicVertex) );

    // set data to zero and null
	// set data to zero and null。初始化数据为null
	for (int i = 0; i < vertexList->maxItem; i++)
	{
		vertexList->List[i].residueID      = -1;			// 残基ID
		vertexList->List[i].vertexNameID   = -1;			// 原子名称ID
		vertexList->List[i].vertexNameSN   = -1;			// 原子名称编码

		vertexList->List[i].Position.valid = -1;             // not assigned, before assigned by gpdb data, 2022-07-05
		vertexList->List[i].Position.x     = 0.0;
		vertexList->List[i].Position.y     = 0.0;
		vertexList->List[i].Position.z     = 0.0;

		vertexList->List[i].vertexName     = dvnnString_new_null_string();	// 原子全名

		vertexList->List[i].qomdCharge     = 0.0;            // add 2024-11-09
		vertexList->List[i].hhmoCharge     = 0.0;            // add 2024-11-09

	}


    // return
	return vertexList;

}



// 2024-11-07
/**
 * 初始化并分配DvnnForceBondList内存
 *
 * 功能描述：
 * 1. 为键BondList列表分配内存空间
 * 2. 通过proteinBag获取活动项数和最大项数
 * 3. 初始化所有键BondList项的默认参数
 *
 * @param proteinBag 蛋白质包指针（包含分子结构数据）
 * @return 初始化完成的DvnnForceBondList指针
 */
DvnnForceBondList* dvnn_init_malloc_dvnnBondList(void* proteinBag)
{
    //================================================================================//
    // 结构体定义参考：
    // - DvnnForceBondList 定义在 aDvnn.h
    // - DvnnForceBond 定义在 aDvnn.h
    //================================================================================//

    //====================================================================================================================================//
    // 依赖函数说明：
    // - qomd_shell3_get_dvnnBondList_actItem_by_proteinBag: 获取当前活动键合项数量（在qomdShell3.cpp定义）
    // - qomd_shell3_get_dvnnBondList_maxItem_by_proteinBag: 获取最大键合项容量（在qomdShell3.cpp定义）
    //====================================================================================================================================//

    // 分配主结构体内存
    DvnnForceBondList* dvnnBondList = (DvnnForceBondList*)malloc(sizeof(DvnnForceBondList));

    // 从蛋白质包获取列表参数
    dvnnBondList->actItem = qomd_shell3_get_dvnnBondList_actItem_by_proteinBag(proteinBag); // 当前有效项数
    dvnnBondList->maxItem = qomd_shell3_get_dvnnBondList_maxItem_by_proteinBag(proteinBag); // 最大容量

    // 为键合项数组分配内存
    dvnnBondList->List = (DvnnForceBond*)malloc(dvnnBondList->maxItem * sizeof(DvnnForceBond));

    // 初始化所有键合项参数（默认值）
    for (int i = 0; i < dvnnBondList->maxItem; i++)
    {
        // 原子索引初始化（-1表示未分配）
        dvnnBondList->List[i].VertexI  = -1;
        dvnnBondList->List[i].VertexJ  = -1;

        // 键合参数初始化
        dvnnBondList->List[i].req      = 0.0;  // 键长平衡值
        dvnnBondList->List[i].qomdFreq = 0.0;  // QOMD频率
        dvnnBondList->List[i].hhmoFreq = 0.0;  // HHMO频率

        // 偏差限制参数
        dvnnBondList->List[i].MaxDeviation   = 0.0;  // 最大允许偏差
        dvnnBondList->List[i].MinDeviation   = 0.0;  // 最小允许偏差
        dvnnBondList->List[i].LimitDeviation = 0.0;  // 限制偏差阈值

        // 记录参数初始化
        dvnnBondList->List[i].Record.Mode     = 0;    // 模式标识
        dvnnBondList->List[i].Record.Start    = 0.0;  // 起始值
        dvnnBondList->List[i].Record.Target   = 0.0;  // 目标值
        dvnnBondList->List[i].Record.Arrive   = 0.0;  // 到达值
        dvnnBondList->List[i].Record.Terminal = 0.0;  // 终止值
    }

    return dvnnBondList;
}



// 2024-11-07
/**
 * 初始化并分配DvnnForceAngleList内存
 *
 * 功能描述：
 * 1. 为AngleList列表分配内存空间
 * 2. 通过proteinBag获取活动项数和最大项数
 * 3. 初始化所有AngleList项的默认参数
 *
 * @param proteinBag 蛋白质包指针（包含分子结构数据）
 * @return 初始化完成的DvnnForceAngleList指针
 */
DvnnForceAngleList* dvnn_init_malloc_dvnnAngleList(void* proteinBag)
{
    //================================================================================//
    // 结构体定义参考：
    // - DvnnForceAngleList 定义在 aDvnn.h
    // - DvnnForceAngle 定义在 aDvnn.h
    //================================================================================//

    //====================================================================================================================================//
    // 依赖函数说明：
    // - qomd_shell3_get_dvnnAngleList_actItem_by_proteinBag: 获取当前活动角度项数量（在qomdShell3.cpp定义）
    // - qomd_shell3_get_dvnnAngleList_maxItem_by_proteinBag: 获取最大角度项容量（在qomdShell3.cpp定义）
    //====================================================================================================================================//

    // 分配主结构体内存
    DvnnForceAngleList* dvnnAngleList = (DvnnForceAngleList*)malloc(sizeof(DvnnForceAngleList));

    // 从蛋白质包获取列表参数
    dvnnAngleList->actItem = qomd_shell3_get_dvnnAngleList_actItem_by_proteinBag(proteinBag); // 当前有效项数
    dvnnAngleList->maxItem = qomd_shell3_get_dvnnAngleList_maxItem_by_proteinBag(proteinBag); // 最大容量

    // 为角度项数组分配内存
    dvnnAngleList->List = (DvnnForceAngle*)malloc(dvnnAngleList->maxItem * sizeof(DvnnForceAngle));

    // 初始化所有角度项参数（默认值）
    for (int i = 0; i < dvnnAngleList->maxItem; i++)
    {
        // 原子索引初始化（-1表示未分配）
        dvnnAngleList->List[i].VertexI  = -1;  // 角度顶点I
        dvnnAngleList->List[i].VertexJ  = -1;  // 角度顶点J（中心原子）
        dvnnAngleList->List[i].VertexK  = -1;  // 角度顶点K

        // 角度参数初始化
        dvnnAngleList->List[i].thetaeq  = 0.0;  // 平衡角度值（弧度）
        dvnnAngleList->List[i].qomdFreq = 0.0;  // QOMD频率
        dvnnAngleList->List[i].hhmoFreq = 0.0;  // HHMO频率

        // 偏差限制参数
        dvnnAngleList->List[i].MaxDeviation   = 0.0;  // 最大允许偏差（弧度）
        dvnnAngleList->List[i].MinDeviation   = 0.0;  // 最小允许偏差（弧度）
        dvnnAngleList->List[i].LimitDeviation = 0.0;  // 限制偏差阈值（弧度）

        // 记录参数初始化
        dvnnAngleList->List[i].Record.Mode     = 0;    // 模式标识
        dvnnAngleList->List[i].Record.Start    = 0.0;  // 起始角度值
        dvnnAngleList->List[i].Record.Target   = 0.0;  // 目标角度值
        dvnnAngleList->List[i].Record.Arrive   = 0.0;  // 到达角度值
        dvnnAngleList->List[i].Record.Terminal = 0.0;  // 终止角度值
    }

    return dvnnAngleList;
}



// 2024-11-07
/**
 * 初始化并分配DvnnForceDihedralList内存
 *
 * 功能描述：
 * 1. 为DihedralList列表分配内存空间
 * 2. 通过proteinBag获取活动项数和最大项数
 * 3. 初始化所有DihedralList项的默认参数
 *
 * @param proteinBag 蛋白质包指针（包含分子结构数据）
 * @return 初始化完成的DvnnForceDihedralList指针
 */
DvnnForceDihedralList* dvnn_init_malloc_dvnnDihedralList(void* proteinBag)
{
    //================================================================================//
    // 结构体定义参考：
    // - DvnnForceDihedralList 定义在 aDvnn.h
    // - DvnnForceDihedral 定义在 aDvnn.h
    //================================================================================//

    //====================================================================================================================================//
    // 依赖函数说明：
    // - qomd_shell3_get_dvnnDihedralList_actItem_by_proteinBag: 获取当前活动二面角项数量（在qomdShell3.cpp定义）
    // - qomd_shell3_get_dvnnDihedralList_maxItem_by_proteinBag: 获取最大二面角项容量（在qomdShell3.cpp定义）
    //====================================================================================================================================//

    // 分配主结构体内存
    DvnnForceDihedralList* dvnnDihedralList = (DvnnForceDihedralList*)malloc(sizeof(DvnnForceDihedralList));

    // 从蛋白质包获取列表参数
    dvnnDihedralList->actItem = qomd_shell3_get_dvnnDihedralList_actItem_by_proteinBag(proteinBag); // 当前有效项数
    dvnnDihedralList->maxItem = qomd_shell3_get_dvnnDihedralList_maxItem_by_proteinBag(proteinBag); // 最大容量

    // 为二面角项数组分配内存
    dvnnDihedralList->List = (DvnnForceDihedral*)malloc(dvnnDihedralList->maxItem * sizeof(DvnnForceDihedral));

    // 初始化所有二面角项参数（默认值）
    for (int i = 0; i < dvnnDihedralList->maxItem; i++)
    {
        // 原子索引初始化（-1表示未分配）
        dvnnDihedralList->List[i].VertexI  = -1;  // 二面角顶点I
        dvnnDihedralList->List[i].VertexJ  = -1;  // 二面角顶点J
        dvnnDihedralList->List[i].VertexK  = -1;  // 二面角顶点K
        dvnnDihedralList->List[i].VertexL  = -1;  // 二面角顶点L

        // 频率参数
        dvnnDihedralList->List[i].qomdFreq = 0.0;  // QOMD频率（扭转振动）
        dvnnDihedralList->List[i].hhmoFreq = 0.0;  // HHMO频率（谐波振动）

        // 二面角谷值记录（-1表示未定义）
        dvnnDihedralList->List[i].Record.ValleyStart    = -1;  // 起始谷值位置
        dvnnDihedralList->List[i].Record.ValleyTarget   = -1;  // 目标谷值位置
        dvnnDihedralList->List[i].Record.ValleyArrive   = -1;  // 到达谷值位置
        dvnnDihedralList->List[i].Record.ValleyTerminal = -1;  // 终止谷值位置

        // 二面角相位记录（弧度）
        dvnnDihedralList->List[i].Record.PhiStart    = 0.0;  // 起始相位角
        dvnnDihedralList->List[i].Record.PhiTarget   = 0.0;  // 目标相位角
        dvnnDihedralList->List[i].Record.PhiArrive   = 0.0;  // 到达相位角
        dvnnDihedralList->List[i].Record.PhiTerminal = 0.0;  // 终止相位角
    }

    return dvnnDihedralList;
}

/**
 * 初始化并分配DvnnForcePlaneList内存
 *
 * 功能描述：
 * 1. 为PlaneList列表分配内存空间
 * 2. 通过proteinBag获取活动项数和最大项数
 * 3. 初始化所有PlaneList项的默认参数
 *
 * @param proteinBag 蛋白质包指针（包含分子结构数据）
 * @return 初始化完成的DvnnForcePlaneList指针
 */
DvnnForcePlaneList* dvnn_init_malloc_dvnnPlaneList(void* proteinBag)
{
    //================================================================================//
    // 结构体定义参考：
    // - DvnnForcePlaneList 定义在 aDvnn.h
    // - DvnnForcePlane 定义在 aDvnn.h
    //================================================================================//

    //====================================================================================================================================//
    // 依赖函数说明：
    // - qomd_shell3_get_dvnnPlaneList_actItem_by_proteinBag: 获取当前活动平面项数量（在qomdShell3.cpp定义）
    // - qomd_shell3_get_dvnnPlaneList_maxItem_by_proteinBag: 获取最大平面项容量（在qomdShell3.cpp定义）
    //====================================================================================================================================//

    // 分配主结构体内存
    DvnnForcePlaneList* dvnnPlaneList = (DvnnForcePlaneList*)malloc(sizeof(DvnnForcePlaneList));

    // 从蛋白质包获取列表参数
    dvnnPlaneList->actItem = qomd_shell3_get_dvnnPlaneList_actItem_by_proteinBag(proteinBag); // 当前有效项数
    dvnnPlaneList->maxItem = qomd_shell3_get_dvnnPlaneList_maxItem_by_proteinBag(proteinBag); // 最大容量

    // 为平面项数组分配内存
    dvnnPlaneList->List = (DvnnForcePlane*)malloc(dvnnPlaneList->maxItem * sizeof(DvnnForcePlane));

    // 初始化所有平面项参数（默认值）
    for (int i = 0; i < dvnnPlaneList->maxItem; i++)
    {
        // 原子索引初始化（-1表示未分配）
        dvnnPlaneList->List[i].VertexI  = -1;  // 平面顶点I
        dvnnPlaneList->List[i].VertexJ  = -1;  // 平面顶点J
        dvnnPlaneList->List[i].VertexK  = -1;  // 平面顶点K
        dvnnPlaneList->List[i].VertexL  = -1;  // 平面顶点L（参考点）

        // 平面参数初始化
        dvnnPlaneList->List[i].phieq    = 0.0;  // 平面平衡角度（弧度）
        dvnnPlaneList->List[i].qomdFreq = 0.0;  // QOMD频率
        dvnnPlaneList->List[i].hhmoFreq = 0.0;  // HHMO频率

        // 偏差限制参数
        dvnnPlaneList->List[i].MaxDeviation   = 0.0;  // 最大允许偏差（弧度）
        dvnnPlaneList->List[i].MinDeviation   = 0.0;  // 最小允许偏差（弧度）
        dvnnPlaneList->List[i].LimitDeviation = 0.0;  // 限制偏差阈值（弧度）

        // 记录参数初始化
        dvnnPlaneList->List[i].Record.Mode     = 0;    // 模式标识
        dvnnPlaneList->List[i].Record.Start    = 0.0;  // 起始平面角度
        dvnnPlaneList->List[i].Record.Target   = 0.0;  // 目标平面角度
        dvnnPlaneList->List[i].Record.Arrive   = 0.0;  // 到达平面角度
        dvnnPlaneList->List[i].Record.Terminal = 0.0;  // 终止平面角度
    }

    return dvnnPlaneList;
}





// 2023-10-09
// 创建并初始化一个新的 DvnnString 类型的空的字符串。
DvnnString dvnnString_new_null_string()
{
	// new str_dst
	DvnnString str_dst;		// DvnnString表示一个有最大长度限制的字符串

	// reset
	str_dst.maxLength = DVNN_NAME_STRING_ARRAY_MAX_LENGTH -1; // DVNN_NAME_STRING_ARRAY_MAX_LENGTH=21，在aDvnn中定义

	// reset a null string : actLength is 0
	str_dst.actLength = 0;

	// reset a null string : all chars are \0
	// 初始化字符数组为空字符串：将数组中的所有字符都设置为 0（即 \0），确保整个字符串都是空的。
	for (int i = 0; i <= str_dst.maxLength; i++){
		str_dst.string[i] = 0;
	}

	return str_dst;
}



// 2023-10-09
void dvnn_write_dvnnVertexList_to_test_file(DvnnAtomicVertexList * vertexList, char * fname_writerecord)
// 将 vertexList 中的内容写入 fname_writerecord 文件中
{

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // open output file : test_output.txt
    ///////////////////////////////////////////////////////////////////////////////////////////////

	// get output file ptr
	FILE* fptr_writerecord;

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
	//use fopen : for linux
	//fptr_writerecord = fopen(fname_writerecord, "w+");     // open mode : if not exist then create, if exist then clean, read/write 


	if (fptr_writerecord == NULL)
	{
		printf("failed to open output file < %s >\n", fname_writerecord);
		exit(1);
	}
	else
	{
		printf("successful open output file < %s >\n", fname_writerecord);
	}

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // end open output file : test_output.txt
    ///////////////////////////////////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // write VertexList
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fprintf(fptr_writerecord, "======= VertexList =======   \n");
	// 向文件 fptr_writerecord 中写入======= VertexList =======   \n

    for (int i = 0; i < vertexList->actItem; i++)
    {
    	// 写入数据：函数遍历vertexList->actItem中的每个顶点，并将其详细信息写入fptr_writerecord文件中。
    	// 对应输出的dvnn_vertexList_1.txt和dvnn_vertexList_2.txt文件
    	// 写入的信息包括顶点索引、指针地址、残基ID、顶点名称序列号、顶点名称ID、顶点名称字符串及其长度、位置信息的有效标志和坐标值。

        ////////////////////////
        // basic info
        ////////////////////////
        fprintf(fptr_writerecord, "vIdx=%4d "  , i                                       );
        fprintf(fptr_writerecord, "vPtr=%p  "  , &(vertexList->List[i])                  );
        fprintf(fptr_writerecord, "resID=%6d " , vertexList->List[i].residueID           );
        fprintf(fptr_writerecord, "vNmSN=%6d " , vertexList->List[i].vertexNameSN        );
        fprintf(fptr_writerecord, "vNmID=%6d " , vertexList->List[i].vertexNameID        );
        fprintf(fptr_writerecord, "vNm=%12s "  , vertexList->List[i].vertexName.string   );
        fprintf(fptr_writerecord, "actL=%2d "  , vertexList->List[i].vertexName.actLength);
        fprintf(fptr_writerecord, "maxL=%2d "  , vertexList->List[i].vertexName.maxLength);

        fprintf(fptr_writerecord, "qomdCharge=%10.6f " , vertexList->List[i].qomdCharge);
        fprintf(fptr_writerecord, "hhmoCharge=%10.6f " , vertexList->List[i].hhmoCharge);

        fprintf(fptr_writerecord, "posV=%2d "    , vertexList->List[i].Position.valid);
        fprintf(fptr_writerecord, "posX=%12.6f " , vertexList->List[i].Position.x);
        fprintf(fptr_writerecord, "posY=%12.6f " , vertexList->List[i].Position.y);
        fprintf(fptr_writerecord, "posZ=%12.6f " , vertexList->List[i].Position.z);

        fprintf(fptr_writerecord, "\n");

    }




	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// close output file
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//use fclose  // in <stdio.h>

	// close fptr_writerecord
	if (fclose(fptr_writerecord))
	{
		printf("failed to close < %s >\n", fname_writerecord);
	}
	else
	{
		printf("successfully close < %s >\n", fname_writerecord);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// end close output file
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}



// 2024-11-09
/**
 * 将所有DVNN数据列表写入测试文件
 *
 * 功能描述：
 * 1. 将顶点列表、键合/角度/二面角/平面力列表统一写入测试文件
 * 2. 包含详细的数值记录和关联信息输出
 * 3. 支持跨平台文件操作（Linux/Windows）
 *
 * @param dvnnVertexList 	顶点列表指针
 * @param dvnnBondList 		BondList列表指针
 * @param dvnnAngleList 	AngleList列表指针
 * @param dvnnDihedralList 	DihedralList列表指针
 * @param dvnnPlaneList 	PlaneList列表指针
 * @param fname_writerecord 输出文件名
 */
void dvnn_write_dvnnAllList_to_test_file(
    DvnnAtomicVertexList  * dvnnVertexList  ,
    DvnnForceBondList     * dvnnBondList    ,
    DvnnForceAngleList    * dvnnAngleList   ,
    DvnnForceDihedralList * dvnnDihedralList,
    DvnnForcePlaneList    * dvnnPlaneList   ,
	char                  * fname_writerecord)
{

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // open output file : test_output.txt
    ///////////////////////////////////////////////////////////////////////////////////////////////

	// get output file ptr
	FILE* fptr_writerecord;

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
	//use fopen : for linux
	//fptr_writerecord = fopen(fname_writerecord, "w+");     // open mode : if not exist then create, if exist then clean, read/write


	if (fptr_writerecord == NULL)
	{
		printf("failed to open output file < %s >\n", fname_writerecord);
		exit(1);
	}
	else
	{
		printf("successful open output file < %s >\n", fname_writerecord);
	}

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // end open output file : test_output.txt
    ///////////////////////////////////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // write dvnnVertexList，写入顶点列表数据
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fprintf(fptr_writerecord, "======= dvnnVertexList =======   \n");

    fprintf(fptr_writerecord, "dvnnVertexList->actItem = %12d " , dvnnVertexList->actItem          );
    fprintf(fptr_writerecord, "dvnnVertexList->maxItem = %12d " , dvnnVertexList->maxItem          );

    fprintf(fptr_writerecord, "\n\n");

    for (int i = 0; i < dvnnVertexList->actItem; i++)
    {
    	// 顶点基本信息
        fprintf(fptr_writerecord, "vIdx=%6d "    , i                                           );
        fprintf(fptr_writerecord, "resID=%6d "   , dvnnVertexList->List[i].residueID           );
        fprintf(fptr_writerecord, "vNmSN=%6d "   , dvnnVertexList->List[i].vertexNameSN        );
        fprintf(fptr_writerecord, "vNmID=%6d "   , dvnnVertexList->List[i].vertexNameID        );
        fprintf(fptr_writerecord, "vNm=%12s "    , dvnnVertexList->List[i].vertexName.string   );
    	// 电荷信息
        fprintf(fptr_writerecord, "qCha=%10.6f " , dvnnVertexList->List[i].qomdCharge          );
        fprintf(fptr_writerecord, "hCha=%10.6f " , dvnnVertexList->List[i].hhmoCharge          );
    	// 位置信息
        fprintf(fptr_writerecord, "posV=%2d "    , dvnnVertexList->List[i].Position.valid      );
        fprintf(fptr_writerecord, "posX=%12.6f " , dvnnVertexList->List[i].Position.x          );
        fprintf(fptr_writerecord, "posY=%12.6f " , dvnnVertexList->List[i].Position.y          );
        fprintf(fptr_writerecord, "posZ=%12.6f " , dvnnVertexList->List[i].Position.z          );

        fprintf(fptr_writerecord, "\n");
    }

    fprintf(fptr_writerecord, "\n\n");




    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // write dvnnBondList，写入bond列表数据
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fprintf(fptr_writerecord, "======= dvnnBondList =======   \n");

    fprintf(fptr_writerecord, "dvnnBondList->actItem = %12d " , dvnnBondList->actItem          );
    fprintf(fptr_writerecord, "dvnnBondList->maxItem = %12d " , dvnnBondList->maxItem          );

    fprintf(fptr_writerecord, "\n\n");

    for (int i = 0; i < dvnnBondList->actItem; i++)
    {
    	// 键合原子索引
        fprintf(fptr_writerecord, "bondIdx=%6d "     , i                                                                         );
        fprintf(fptr_writerecord, "vI=%6d "          , dvnnBondList->List[i].VertexI                                             );
        fprintf(fptr_writerecord, "vJ=%6d "          , dvnnBondList->List[i].VertexJ                                             );
        fprintf(fptr_writerecord, "vInm=%12s "       , dvnnVertexList->List[(dvnnBondList->List[i].VertexI)].vertexName.string   );
        fprintf(fptr_writerecord, "vJnm=%12s "       , dvnnVertexList->List[(dvnnBondList->List[i].VertexJ)].vertexName.string   );
    	// 键合参数
        fprintf(fptr_writerecord, "req     =%12.6f " , dvnnBondList->List[i].req                                                 );
        fprintf(fptr_writerecord, "BqomdFreq=%12.6f ", dvnnBondList->List[i].qomdFreq                                            );
        fprintf(fptr_writerecord, "BhhmoFreq=%12.6f ", dvnnBondList->List[i].hhmoFreq                                            );
    	// 偏差控制
        fprintf(fptr_writerecord, "MaxDev=%12.6f "   , dvnnBondList->List[i].MaxDeviation                                        );
        fprintf(fptr_writerecord, "MinDev=%12.6f "   , dvnnBondList->List[i].MinDeviation                                        );
        fprintf(fptr_writerecord, "LimDev=%12.6f "   , dvnnBondList->List[i].LimitDeviation                                      );
    	// 记录参数
        fprintf(fptr_writerecord, "Mode=%6d "        , dvnnBondList->List[i].Record.Mode                                         );
        fprintf(fptr_writerecord, "Start=%12.6f "    , dvnnBondList->List[i].Record.Start                                        );
        fprintf(fptr_writerecord, "Target=%12.6f "   , dvnnBondList->List[i].Record.Target                                       );
        fprintf(fptr_writerecord, "Arrive=%12.6f "   , dvnnBondList->List[i].Record.Arrive                                       );
        fprintf(fptr_writerecord, "Terminal=%12.6f " , dvnnBondList->List[i].Record.Terminal                                     );

        fprintf(fptr_writerecord, "\n");
    }

    fprintf(fptr_writerecord, "\n\n");



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // write dvnnAngleList，写入Angle列表数据
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fprintf(fptr_writerecord, "======= dvnnAngleList =======   \n");

    fprintf(fptr_writerecord, "dvnnAngleList->actItem = %12d " , dvnnAngleList->actItem          );
    fprintf(fptr_writerecord, "dvnnAngleList->maxItem = %12d " , dvnnAngleList->maxItem          );

    fprintf(fptr_writerecord, "\n\n");

    for (int i = 0; i < dvnnAngleList->actItem; i++)
    {
        fprintf(fptr_writerecord, "angleIdx=%6d "      , i                                                                          );
        fprintf(fptr_writerecord, "vI=%6d "            , dvnnAngleList->List[i].VertexI                                             );
        fprintf(fptr_writerecord, "vJ=%6d "            , dvnnAngleList->List[i].VertexJ                                             );
        fprintf(fptr_writerecord, "vK=%6d "            , dvnnAngleList->List[i].VertexK                                             );
        fprintf(fptr_writerecord, "vInm=%12s "         , dvnnVertexList->List[(dvnnAngleList->List[i].VertexI)].vertexName.string   );
        fprintf(fptr_writerecord, "vJnm=%12s "         , dvnnVertexList->List[(dvnnAngleList->List[i].VertexJ)].vertexName.string   );
        fprintf(fptr_writerecord, "vKnm=%12s "         , dvnnVertexList->List[(dvnnAngleList->List[i].VertexK)].vertexName.string   );
        fprintf(fptr_writerecord, "AqomdFreq=%12.6f "  , dvnnAngleList->List[i].qomdFreq                                            );
        fprintf(fptr_writerecord, "AhhmoFreq=%12.6f "  , dvnnAngleList->List[i].hhmoFreq                                            );
        fprintf(fptr_writerecord, "thetaeqDEG=%12.6f " , dvnnAngleList->List[i].thetaeq                                             );
        fprintf(fptr_writerecord, "thetaeqRAD=%12.6f " , ((dvnnAngleList->List[i].thetaeq)/180.0) * ((double)(DVNN_PI))             );
        fprintf(fptr_writerecord, "MaxDev=%12.6f "     , dvnnAngleList->List[i].MaxDeviation                                        );
        fprintf(fptr_writerecord, "MinDev=%12.6f "     , dvnnAngleList->List[i].MinDeviation                                        );
        fprintf(fptr_writerecord, "LimDev=%12.6f "     , dvnnAngleList->List[i].LimitDeviation                                      );
        fprintf(fptr_writerecord, "Mode=%6d "          , dvnnAngleList->List[i].Record.Mode                                         );
        fprintf(fptr_writerecord, "Start=%12.6f "      , dvnnAngleList->List[i].Record.Start                                        );
        fprintf(fptr_writerecord, "Target=%12.6f "     , dvnnAngleList->List[i].Record.Target                                       );
        fprintf(fptr_writerecord, "Arrive=%12.6f "     , dvnnAngleList->List[i].Record.Arrive                                       );
        fprintf(fptr_writerecord, "Terminal=%12.6f "   , dvnnAngleList->List[i].Record.Terminal                                     );

        fprintf(fptr_writerecord, "\n");
    }

    fprintf(fptr_writerecord, "\n\n");



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // write dvnnDihedralList，写入二面角列表数据
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fprintf(fptr_writerecord, "======= dvnnDihedralList =======   \n");

    fprintf(fptr_writerecord, "dvnnDihedralList->actItem = %12d " , dvnnDihedralList->actItem          );
    fprintf(fptr_writerecord, "dvnnDihedralList->maxItem = %12d " , dvnnDihedralList->maxItem          );

    fprintf(fptr_writerecord, "\n\n");

    for (int i = 0; i < dvnnDihedralList->actItem; i++)
    {
        fprintf(fptr_writerecord, "dihedralIdx=%6d "     , i                                                                             );
        fprintf(fptr_writerecord, "vI=%6d "              , dvnnDihedralList->List[i].VertexI                                             );
        fprintf(fptr_writerecord, "vJ=%6d "              , dvnnDihedralList->List[i].VertexJ                                             );
        fprintf(fptr_writerecord, "vK=%6d "              , dvnnDihedralList->List[i].VertexK                                             );
        fprintf(fptr_writerecord, "vL=%6d "              , dvnnDihedralList->List[i].VertexL                                             );
        fprintf(fptr_writerecord, "vInm=%12s "           , dvnnVertexList->List[(dvnnDihedralList->List[i].VertexI)].vertexName.string   );
        fprintf(fptr_writerecord, "vJnm=%12s "           , dvnnVertexList->List[(dvnnDihedralList->List[i].VertexJ)].vertexName.string   );
        fprintf(fptr_writerecord, "vKnm=%12s "           , dvnnVertexList->List[(dvnnDihedralList->List[i].VertexK)].vertexName.string   );
        fprintf(fptr_writerecord, "vLnm=%12s "           , dvnnVertexList->List[(dvnnDihedralList->List[i].VertexL)].vertexName.string   );
        fprintf(fptr_writerecord, "DqomdFreq=%12.6f "    , dvnnDihedralList->List[i].qomdFreq                                            );
        fprintf(fptr_writerecord, "DhhmoFreq=%12.6f "    , dvnnDihedralList->List[i].hhmoFreq                                            );

        fprintf(fptr_writerecord, "ValleyStart=%6d "     , dvnnDihedralList->List[i].Record.ValleyStart                                  );
        fprintf(fptr_writerecord, "ValleyTarget=%6d "    , dvnnDihedralList->List[i].Record.ValleyTarget                                 );
        fprintf(fptr_writerecord, "ValleyArrive=%6d "    , dvnnDihedralList->List[i].Record.ValleyArrive                                 );
        fprintf(fptr_writerecord, "ValleyTerminal=%6d "  , dvnnDihedralList->List[i].Record.ValleyTerminal                               );
        fprintf(fptr_writerecord, "PhiStart=%12.6f "     , dvnnDihedralList->List[i].Record.PhiStart                                     );
        fprintf(fptr_writerecord, "PhiTarget=%12.6f "    , dvnnDihedralList->List[i].Record.PhiTarget                                    );
        fprintf(fptr_writerecord, "PhiArrive=%12.6f "    , dvnnDihedralList->List[i].Record.PhiArrive                                    );
        fprintf(fptr_writerecord, "PhiTerminal=%12.6f "  , dvnnDihedralList->List[i].Record.PhiTerminal                                  );

        fprintf(fptr_writerecord, "\n");
    }

    fprintf(fptr_writerecord, "\n\n");



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // write dvnnPlaneList，写入平面力列表数据
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fprintf(fptr_writerecord, "======= dvnnPlaneList =======   \n");

    fprintf(fptr_writerecord, "dvnnPlaneList->actItem = %12d " , dvnnPlaneList->actItem          );
    fprintf(fptr_writerecord, "dvnnPlaneList->maxItem = %12d " , dvnnPlaneList->maxItem          );

    fprintf(fptr_writerecord, "\n\n");

    for (int i = 0; i < dvnnPlaneList->actItem; i++)
    {
        fprintf(fptr_writerecord, "planeIdx=%6d "    , i                                                                          );
        fprintf(fptr_writerecord, "vI=%6d "          , dvnnPlaneList->List[i].VertexI                                             );
        fprintf(fptr_writerecord, "vJ=%6d "          , dvnnPlaneList->List[i].VertexJ                                             );
        fprintf(fptr_writerecord, "vK=%6d "          , dvnnPlaneList->List[i].VertexK                                             );
        fprintf(fptr_writerecord, "vL=%6d "          , dvnnPlaneList->List[i].VertexL                                             );
        fprintf(fptr_writerecord, "vInm=%12s "       , dvnnVertexList->List[(dvnnPlaneList->List[i].VertexI)].vertexName.string   );
        fprintf(fptr_writerecord, "vJnm=%12s "       , dvnnVertexList->List[(dvnnPlaneList->List[i].VertexJ)].vertexName.string   );
        fprintf(fptr_writerecord, "vKnm=%12s "       , dvnnVertexList->List[(dvnnPlaneList->List[i].VertexK)].vertexName.string   );
        fprintf(fptr_writerecord, "vLnm=%12s "       , dvnnVertexList->List[(dvnnPlaneList->List[i].VertexL)].vertexName.string   );
        fprintf(fptr_writerecord, "PqomdFreq=%12.6f ", dvnnPlaneList->List[i].qomdFreq                                            );
        fprintf(fptr_writerecord, "PhhmoFreq=%12.6f ", dvnnPlaneList->List[i].hhmoFreq                                            );
        fprintf(fptr_writerecord, "phieqDEG=%12.6f " , dvnnPlaneList->List[i].phieq                                               );
        fprintf(fptr_writerecord, "phieqRAD=%12.6f " , ((dvnnPlaneList->List[i].phieq)/180.0) * ((double)(DVNN_PI))               );
        fprintf(fptr_writerecord, "MaxDev=%12.6f "   , dvnnPlaneList->List[i].MaxDeviation                                        );
        fprintf(fptr_writerecord, "MinDev=%12.6f "   , dvnnPlaneList->List[i].MinDeviation                                        );
        fprintf(fptr_writerecord, "LimDev=%12.6f "   , dvnnPlaneList->List[i].LimitDeviation                                      );
        fprintf(fptr_writerecord, "Mode=%6d "        , dvnnPlaneList->List[i].Record.Mode                                         );
        fprintf(fptr_writerecord, "Start=%12.6f "    , dvnnPlaneList->List[i].Record.Start                                        );
        fprintf(fptr_writerecord, "Target=%12.6f "   , dvnnPlaneList->List[i].Record.Target                                       );
        fprintf(fptr_writerecord, "Arrive=%12.6f "   , dvnnPlaneList->List[i].Record.Arrive                                       );
        fprintf(fptr_writerecord, "Terminal=%12.6f " , dvnnPlaneList->List[i].Record.Terminal                                     );

        fprintf(fptr_writerecord, "\n");
    }

    fprintf(fptr_writerecord, "\n\n");




	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// close output file
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//use fclose  // in <stdio.h>

	// close fptr_writerecord
	if (fclose(fptr_writerecord))
	{
		printf("failed to close < %s >\n", fname_writerecord);
	}
	else
	{
		printf("successfully close < %s >\n", fname_writerecord);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// end close output file
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}




// 2024-03-04
DvnnTimePointList * dvnn_init_timePointList_malloc(void * atomicVertexList)
// 初始化一个 DvnnTimePointList 结构体，并为其分配内存。该函数接受一个指向 DvnnAtomicVertexList 结构体的指针作为参数，并基于该顶点列表创建一个新的时间点列表。
{
	DvnnAtomicVertexList * vertexList = ((DvnnAtomicVertexList*)atomicVertexList);

	// new an empty timePointList
	// 分配内存：使用 malloc 为 DvnnTimePointList 结构体本身分配内存。根据 vertexList->maxItem 的值，为 List 数组中的每个 DvnnTimePoint 元素分配内存。
    DvnnTimePointList * timePointList = (DvnnTimePointList*)malloc(sizeof(DvnnTimePointList));

	timePointList->actItem = vertexList->actItem;
	timePointList->maxItem = vertexList->maxItem;

    timePointList->List = (DvnnTimePoint*) malloc( (timePointList->maxItem) * sizeof(DvnnTimePoint) );

    // copy data from vertexList
	for (int i = 0; i < vertexList->maxItem; i++)
	{
		timePointList->List[i].vertexIdx    = i;
		timePointList->List[i].t            = 0;

		timePointList->List[i].vertexNameSN = vertexList->List[i].vertexNameSN;
		timePointList->List[i].valid        = vertexList->List[i].Position.valid;
		timePointList->List[i].x            = vertexList->List[i].Position.x;
		timePointList->List[i].y            = vertexList->List[i].Position.y;
		timePointList->List[i].z            = vertexList->List[i].Position.z;

	}


    // return 
	return timePointList;

}





// 2024-03-04
// 将顶点坐标列表按时间戳复制到时间点列表（用于轨迹记录）
void dvnn_copy_vertexList_to_timePointList_by_timeStamp(
    DvnnTimePointList * timePointList,  // [输出] 目标时间点列表（需预分配内存）
    DvnnAtomicVertexList * vertexList,  // [输入] 原子顶点坐标列表
    int timeStamp                        // [输入] 当前时间戳（如模拟步数）
) {
    // 遍历所有有效原子顶点
    for (int i = 0; i < vertexList->actItem; i++) {
        // 设置时间戳
        timePointList->List[i].t = timeStamp;

        // 复制顶点有效性标志（用于过滤无效原子）
        timePointList->List[i].valid = vertexList->List[i].Position.valid;

        // 复制三维坐标（保持与顶点列表数据同步）
        timePointList->List[i].x = vertexList->List[i].Position.x;
        timePointList->List[i].y = vertexList->List[i].Position.y;
        timePointList->List[i].z = vertexList->List[i].Position.z;
    }
}
// 2024-03-05
void dvnn_save_timePointList_paraData_to_byte_file(char * fname_output_TIME_POINT_byte, int totalStamps, int totalVeretx)
// 写入固定大小的参数数据（totalStamps、totalVeretx）到文件，并且每次写入时会清空文件内容，适合用于保存初始参数。
// 使用 "wb" 写入模式，每次打开文件时都会清空文件的内容，然后从头开始写入数据。适合用来写入一些新的数据（如参数数据）到文件中，但不保留以前的内容。
{

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// open file fname_output_TIME_POINT_byte
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// get output file ptr
	FILE* fptr_output_TIME_POINT_byte;
    
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
	//use fopen : for linux
	//fptr_output_TIME_POINT_byte = fopen(fname_output_TIME_POINT_byte, "wb");     // write/binary/create/clean
	//fptr_output_TIME_POINT_byte = fopen(fname_output_TIME_POINT_byte, "ab");     // append write/binary/create/not clean
   
	if (fptr_output_TIME_POINT_byte == NULL)
	{
	   	printf("failed to open output file  < %s >\n", fname_output_TIME_POINT_byte);
	   	exit(1);
	}
	else
	{
	   	printf("successful open output file  < %s >\n", fname_output_TIME_POINT_byte);
	}



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// copy data
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    

	/////////////////////////////////////////////////////////////
    // step 1 : totalStamps
	/////////////////////////////////////////////////////////////

	////////////////////////////
    // totalStamps : byte_size 
	////////////////////////////
	int byteSize_totalStamps = sizeof(int);


	////////////////////////////
    // totalStamps : copy 
	////////////////////////////
    // cast data to char* list
	char * byteDataBlock_totalStamps = (char*)(&(totalStamps));

    // write char* list to byte file
	for (int i = 0; i < byteSize_totalStamps; i++)
	{
		fputc((int)(byteDataBlock_totalStamps[i]), fptr_output_TIME_POINT_byte);
	}
    printf("SAVE DONE : totalStamps ! totalStamps =%10d \n", totalStamps);


	/////////////////////////////////////////////////////////////
    // step 2 : totalVeretx
	/////////////////////////////////////////////////////////////

	////////////////////////////
    // totalVeretx : byte_size 
	////////////////////////////
	int byteSize_totalVeretx = sizeof(int);


	////////////////////////////
    // totalVeretx : copy 
	////////////////////////////
    // cast data to char* list
	char * byteDataBlock_totalVeretx = (char*)(&(totalVeretx));

    // write char* list to byte file
	for (int i = 0; i < byteSize_totalVeretx; i++)
	{
		fputc((int)(byteDataBlock_totalVeretx[i]), fptr_output_TIME_POINT_byte);
	}
    printf("SAVE DONE : totalVeretx ! totalVeretx =%10d \n", totalVeretx);




	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// close file fptr_output_TIME_POINT_byte
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
	//use fclose  // in <stdio.h>
    
	// close fptr_output_TIME_POINT_byte
	if (fclose(fptr_output_TIME_POINT_byte))
	{
	  	printf("failed to close  < %s >\n", fname_output_TIME_POINT_byte);
	}
	else
	{
	   	printf("successfully close  < %s >\n", fname_output_TIME_POINT_byte);
	}
	


}



// 2024-03-04
void dvnn_save_timePointList_listData_to_byte_file(char * fname_output_TIME_POINT_byte, DvnnTimePointList * timePointList)
// 写入动态大小的结构体数据（DvnnTimePointList 的列表），数据会追加到已有的文件内容之后，适合用于在文件中不断追加新的列表数据。
// 使用 "ab" 追加模式，每次打开文件时，数据会追加到文件的末尾，而不会清空文件。适合用于需要保留现有数据并在其后追加新的数据（如将新的时间点列表追加到文件中）。

{


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// open file fname_output_TIME_POINT_byte
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// get output file ptr
	FILE* fptr_output_TIME_POINT_byte;
    
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
	//use fopen : for linux
	//fptr_output_TIME_POINT_byte = fopen(fname_output_TIME_POINT_byte, "ab");     // append write/binary/create/not clean


	if (fptr_output_TIME_POINT_byte == NULL)
	{
	   	printf("failed to open output file  < %s >\n", fname_output_TIME_POINT_byte);
	   	exit(1);
	}
	else
	{
	   	printf("successful open output file  < %s >\n", fname_output_TIME_POINT_byte);
	}



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// copy data
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////
    // byte_size 
	////////////////////////////
	int byteSize_timePointList = sizeof(DvnnTimePoint) * (timePointList->actItem);


    ////////////////////////////
    // copy 
	////////////////////////////
    // cast data to char* list
	char * byteDataBlock_timePointList = (char*)(timePointList->List);

    // write char* list to byte file
	for (int i = 0; i < byteSize_timePointList; i++)
	{
		fputc((int)(byteDataBlock_timePointList[i]), fptr_output_TIME_POINT_byte);
	}
    printf("SAVE DONE : timePointList ! \n");


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// close file fptr_output_TIME_POINT_byte
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
	//use fclose  // in <stdio.h>
    
	// close fptr_output_TIME_POINT_byte
	if (fclose(fptr_output_TIME_POINT_byte))
	{
	  	printf("failed to close  < %s >\n", fname_output_TIME_POINT_byte);
	}
	else
	{
	   	printf("successfully close  < %s >\n", fname_output_TIME_POINT_byte);
	}
	

}



// 2024-03-04
DvnnTimePointList * dvnn_get_timePointList_from_byte_file(char * fname_input_TIME_POINT_byte)
// 将数据文件 fname_input_TIME_POINT_byte 中的数据读入数据列表 timePointList，
// 先读入两个整数，整数 totalStamps（总时刻数）和整数 totalVeretx（总原子数），
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// open file fname_input_TIME_POINT_byte
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// get input file ptr
	FILE* fptr_input_TIME_POINT_byte;
    
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++ 
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    #if PROTEIN_RUN_ON_LINUX
        // for linux : fopen in <stdio.h>
	fptr_input_TIME_POINT_byte = fopen(fname_input_TIME_POINT_byte, "rb");
    #endif  // match : #if PROTEIN_RUN_ON_LINUX

    #if PROTEIN_RUN_ON_WINDOWS
        // for win : fopen_s in <stdio.h>
        fopen_s(&fptr_input_TIME_POINT_byte, fname_input_TIME_POINT_byte, "rb");
    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BLOCK of <fopen> & <fopen_s>                                  ++ 
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//use fopen : for linux
	//fptr_input_TIME_POINT_byte = fopen(fname_input_TIME_POINT_byte, "rb");     // read/binary


	if (fptr_input_TIME_POINT_byte == NULL)
	{
	 	printf("failed to open output file  < %s >\n", fname_input_TIME_POINT_byte);
	   	exit(1);
	}
	else
	{
	   	printf("successful open output file  < %s >\n", fname_input_TIME_POINT_byte);
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // read para data from header of byte file
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
    // step 1 : totalStamps
	/////////////////////////////////////////////////////////////

    // byte_size 
	int byteSize_totalStamps = sizeof(int);

    // malloc char* byteDataBlock
    char * byteDataBlock_totalStamps = (char *)malloc( sizeof(char) * byteSize_totalStamps );

    // read char* list from byte file
	for (int i = 0; i < byteSize_totalStamps; i++)
	{	
		byteDataBlock_totalStamps[i] = (char)fgetc(fptr_input_TIME_POINT_byte);		
	}

    // malloc new para
    int * totalStamps = (int *)malloc( sizeof(int) );
    
    // recast
	totalStamps = (int*)byteDataBlock_totalStamps;

	// print
	printf(" totalStamps =%10d ", (*totalStamps));
	printf("\n");



	/////////////////////////////////////////////////////////////
    // step 2 : totalVeretx
	/////////////////////////////////////////////////////////////

    // byte_size 
	int byteSize_totalVeretx = sizeof(int);

    // malloc char* byteDataBlock
    char * byteDataBlock_totalVeretx = (char *)malloc( sizeof(char) * byteSize_totalVeretx );

    // read char* list from byte file
	for (int i = 0; i < byteSize_totalVeretx; i++)
	{	
		byteDataBlock_totalVeretx[i] = (char)fgetc(fptr_input_TIME_POINT_byte);		
	}

    // malloc new para
    int * totalVeretx = (int *)malloc( sizeof(int) );
    
    // recast
	totalVeretx = (int*)byteDataBlock_totalVeretx;

	// print
	printf(" totalVeretx =%10d ", (*totalVeretx));
	printf("\n");





	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// reset a new timePointList and malloc
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// new an empty timePointList
    DvnnTimePointList * timePointList = (DvnnTimePointList*)malloc(sizeof(DvnnTimePointList));

	//timePointList->actItem = total_stamps * total_veretx;
	//timePointList->maxItem = total_stamps * total_veretx;

	timePointList->actItem = (*totalStamps) * (*totalVeretx);
	timePointList->maxItem = (*totalStamps) * (*totalVeretx);

    // only malloc
	timePointList->List = (DvnnTimePoint*) malloc( (timePointList->maxItem) * sizeof(DvnnTimePoint) );


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // read list data to byteDataBlock 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // byte_size of byteDataBlock_timePointList
	int byteSize_timePointList = sizeof(DvnnTimePoint) * (timePointList->actItem);

    // malloc char* byteDataBlock
    char * byteDataBlock_timePointList = (char *)malloc( sizeof(char) * byteSize_timePointList );

    // read char* list from byte file
	for (int i = 0; i < byteSize_timePointList; i++)
	{	
		byteDataBlock_timePointList[i] = (char)fgetc(fptr_input_TIME_POINT_byte);		
	}


 	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // recast timePointList->List
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // recast
    timePointList->List = (DvnnTimePoint*)byteDataBlock_timePointList;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// close file fname_input_TIME_POINT_byte
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
	//use fclose  // in <stdio.h>
    
	// close fname_input_TIME_POINT_byte
	if (fclose(fptr_input_TIME_POINT_byte))
	{
	   	printf("failed to close  < %s >\n", fname_input_TIME_POINT_byte);
	}
	else
	{
	   	printf("successfully close  < %s >\n", fname_input_TIME_POINT_byte);
	}


	////////////////////
	///**
	// * @Version 1.0
	// * @Date 2023-12-19
	// * @Author Zhaohui Meng
	// * @Affiliation Department of Computer Science, Hohai University, Nanjing, China.
	// */
	//
	//
	//
	//
	//
	// ///////////////////////////
	// // include
	// ///////////////////////////
	//
	//
	//// C lib
	//#include <iostream>
	//#include <string>
	//
	//#include <stdio.h>
	//#include <stdlib.h>
	//#include <time.h>
	//
	//using namespace std;
	//
	//
	//
	//
	// ///////////////////////////
	// // include 
	// ///////////////////////////
	//
	//// home dir
	//#include "aDir.h"
	//
	//// dvnn
	//#include "aDvnn.h"
	//
	//// base para
	////#include "aBase.h"
	//
	//
	//
	//// all functions
	//#include "aFuncListSim.h"
	//
	/////////////////////////////
	//// functions declare
	/////////////////////////////
	//
	//
	//
	//// 2024-10-07
	//// get a pseudo_random value in [-1,+1]
	//double dvnn_getNew_Real_neg1_to_pos1()
	//// 伪随机数生成器来生成一个在区间 [-1, +1] 内的双精度浮点数。
	//// 这个函数通过一个简单的线性同余生成器 (LCG) 来产生伪随机数，初始范围在 [0, 1]，然后将其变换为 [-1, +1]，并返回。
	//// 每次调用时，随机数生成器都会基于前一次的结果更新，从而生成一个新的伪随机数。
	//{
	//	// 确保每次函数调用后的 Rand 值都会基于上一次的值进行更新
	//	static int Rand = 1;
	//
	//	// 随机数生成器中的“种子”值
	//	int seed1 = 123;
	//	int seed0 = 59;
	//	// module 是模运算的值，确保生成的随机数始终在 [0, module] 之间，
	//	// 这里 module = 65536，也就是 2^16。
	//	int module = 65536;
	//
	//	// 使用 Rand 的前一个值与种子 seed1 和 seed0 进行线性同余生成运算，并对 module 取模，保证新的 Rand 值在 [0, module] 之间
	//	Rand = (seed1 * Rand + seed0) % module;
	//
	//	// random_x : [0,1]
	//	// 生成的 Rand 值是 [0, module] 之间的整数，除以 module 后，random_x 被标准化为 [0, 1] 之间的浮点数
	//	double random_x = ((double)Rand) / ((double)module);
	//
	//	// random_x : [-1,+1]
	//	// 接着通过减去 0.5 再乘以 2，将 random_x 从 [0, 1] 映射到 [-1, 1]。这个步骤是为了将伪随机数范围从 [0, 1] 变换到 [-1, 1]
	//	double random_y = ( (random_x - 0.5) * 2.0 );
	//
	//	return random_y;
	//
	//}
	//
	//
	//
	//// 2024-10-07
	//DvnnSliderList* dvnn_init_dvnnSliderList_malloc()
	//// 通过动态分配内存，初始化了一个 DvnnSliderList 结构体，该结构体包含 1600 个 DvnnSlider 元素。
	//// 它的主要作用是创建一个包含滑动条的列表，并对列表中的每个滑动条进行初始值设定。
	//// min=-1，act=0，maxa=1
	//{
	//	//================================================================================//
	//	// struct "DvnnSliderList"       is defined  in aDvnn.h                           //
	//	//================================================================================//
	//	// struct "DvnnSlider"           is defined  in aDvnn.h                           //
	//	//================================================================================//
	//
	//	// new an empty sliderList
	//	DvnnSliderList* sliderList = (DvnnSliderList*)malloc(sizeof(DvnnSliderList));
	//
	//	sliderList->actItem = 1600;  // 当前有效的滑动条数量
	//	sliderList->maxItem = 1600;  // 滑动条的最大数量
	//
	//	sliderList->List = (DvnnSlider*)malloc((sliderList->maxItem) * sizeof(DvnnSlider));
	//
	//
	//	// set data to zero
	//	for (int i = 0; i < sliderList->maxItem; i++)
	//	{
	//		sliderList->List[i].min = -1.0;  // 设置滑动条的最小值为 -1.0
	//		sliderList->List[i].act =  0.0;  // 设置滑动条的当前值为 0.0
	//		sliderList->List[i].max = +1.0;  // 设置滑动条的最大值为 +1.0
	//	}
	//
	//	// return
	//	return sliderList;
	//}
	//
	//
	//// 2024-10-07
	//void dvnn_reset_dvnnSliderList_by_random(DvnnSliderList* sliderList)
	//// 将滑动条列表中的每个滑动条的当前值（act）随机化为一个在 [-1, +1] 之间的值，同时确保随机值不会超出该范围。
	//{
	//	// 遍历 sliderList 中的每个 DvnnSlider 元素
	//	for (int i = 0; i < sliderList->actItem; i++)
	//	{
	//		// 生成一个随机数，范围为 [-1, 1]
	//		double random_neg1_to_pos1 = dvnn_getNew_Real_neg1_to_pos1();
	//
	//		// 限制随机数的范围，确保不会超过 [-1, 1] 的边界
	//		if (random_neg1_to_pos1 >  1.0) { random_neg1_to_pos1 =  1.0; }  // 如果大于 1，强制设为 1
	//		if (random_neg1_to_pos1 < -1.0) { random_neg1_to_pos1 = -1.0; }  // 如果小于 -1，强制设为 -1
	//
	//		// 将生成的随机数赋值给当前 DvnnSlider 的 act 成员
	//		sliderList->List[i].act = random_neg1_to_pos1;
	//
	//		// 测试打印，打印滑动条的索引、最小值、当前值和最大值
	//		// printf("i= %6d   ", i);
	//		// printf("slider_min= %12.8f   ", sliderList->List[i].min);
	//		// printf("slider_act= %12.8f   ", sliderList->List[i].act);
	//		// printf("slider_max= %12.8f   ", sliderList->List[i].max);
	//		// printf("\n");
	//	}
	//}
	//
	//
	//
	//// 2023-10-09
	//// 初始化一个DvnnAtomicVertexList结构体，并为其分配内存。
	//DvnnAtomicVertexList * dvnn_init_malloc_vertexList(void * proteinBag)
	//{
	//    //================================================================================//
	//    // struct "DvnnAtomicVertexList"       is defined  in aDvnn.h                     //
	//    //================================================================================//
	//    // struct "DvnnAtomicVertex"           is defined  in aDvnn.h                     //
	//    //================================================================================//
	//
	//    //====================================================================================================================================//
	//    // func   "qomd_shell3_get_vertexList_actItem_by_proteinBag"         is defined  in libQomd.a -> qomdShell3.cpp                       //
	//    // func   "qomd_shell3_get_vertexList_actItem_by_proteinBag"         is declared in aFumcListSim.h                                    //
	//    //====================================================================================================================================//
	//    // func   "qomd_shell3_get_vertexList_maxItem_by_proteinBag"         is defined  in libQomd.a -> qomdShell3.cpp                       //
	//    // func   "qomd_shell3_get_vertexList_maxItem_by_proteinBag"         is declared in aFumcListSim.h                                    //
	//    //====================================================================================================================================//
	//
	//	// new an empty vertexList
	//	DvnnAtomicVertexList * vertexList = (DvnnAtomicVertexList*)malloc(sizeof(DvnnAtomicVertexList));	// 原子顶点列表
	//
	//
	//	vertexList->actItem = qomd_shell3_get_vertexList_actItem_by_proteinBag(proteinBag);		// 从蛋白质数据中获取实际顶点的数量。
	//	vertexList->maxItem = qomd_shell3_get_vertexList_maxItem_by_proteinBag(proteinBag);		// 从蛋白质数据中获取顶点的最大数量。
	//	// 原子链
	//    vertexList->List = (DvnnAtomicVertex*) malloc( (vertexList->maxItem) * sizeof(DvnnAtomicVertex) );
	//
	//    // set data to zero and null
	//	// set data to zero and null。初始化数据为null
	//	for (int i = 0; i < vertexList->maxItem; i++)
	//	{
	//		vertexList->List[i].residueID      = -1;			// 残基ID
	//		vertexList->List[i].vertexNameID   = -1;			// 原子名称ID
	//		vertexList->List[i].vertexNameSN   = -1;			// 原子名称编码
	//
	//		vertexList->List[i].Position.valid = -1;             // not assigned, before assigned by gpdb data, 2022-07-05
	//		vertexList->List[i].Position.x     = 0.0;
	//		vertexList->List[i].Position.y     = 0.0;
	//		vertexList->List[i].Position.z     = 0.0;
	//
	//		vertexList->List[i].vertexName     = dvnnString_new_null_string();	// 原子全名
	//
	//		vertexList->List[i].qomdCharge     = 0.0;            // add 2024-11-09
	//		vertexList->List[i].hhmoCharge     = 0.0;            // add 2024-11-09
	//
	//	}
	//
	//
	//    // return
	//	return vertexList;
	//
	//}
	//
	//
	//
	//// 2024-11-07
	///**
	// * 初始化并分配DvnnForceBondList内存
	// *
	// * 功能描述：
	// * 1. 为键BondList列表分配内存空间
	// * 2. 通过proteinBag获取活动项数和最大项数
	// * 3. 初始化所有键BondList项的默认参数
	// *
	// * @param proteinBag 蛋白质包指针（包含分子结构数据）
	// * @return 初始化完成的DvnnForceBondList指针
	// */
	//DvnnForceBondList* dvnn_init_malloc_dvnnBondList(void* proteinBag)
	//{
	//    //================================================================================//
	//    // 结构体定义参考：
	//    // - DvnnForceBondList 定义在 aDvnn.h
	//    // - DvnnForceBond 定义在 aDvnn.h
	//    //================================================================================//
	//
	//    //====================================================================================================================================//
	//    // 依赖函数说明：
	//    // - qomd_shell3_get_dvnnBondList_actItem_by_proteinBag: 获取当前活动键合项数量（在qomdShell3.cpp定义）
	//    // - qomd_shell3_get_dvnnBondList_maxItem_by_proteinBag: 获取最大键合项容量（在qomdShell3.cpp定义）
	//    //====================================================================================================================================//
	//
	//    // 分配主结构体内存
	//    DvnnForceBondList* dvnnBondList = (DvnnForceBondList*)malloc(sizeof(DvnnForceBondList));
	//
	//    // 从蛋白质包获取列表参数
	//    dvnnBondList->actItem = qomd_shell3_get_dvnnBondList_actItem_by_proteinBag(proteinBag); // 当前有效项数
	//    dvnnBondList->maxItem = qomd_shell3_get_dvnnBondList_maxItem_by_proteinBag(proteinBag); // 最大容量
	//
	//    // 为键合项数组分配内存
	//    dvnnBondList->List = (DvnnForceBond*)malloc(dvnnBondList->maxItem * sizeof(DvnnForceBond));
	//
	//    // 初始化所有键合项参数（默认值）
	//    for (int i = 0; i < dvnnBondList->maxItem; i++)
	//    {
	//        // 原子索引初始化（-1表示未分配）
	//        dvnnBondList->List[i].VertexI  = -1;
	//        dvnnBondList->List[i].VertexJ  = -1;
	//
	//        // 键合参数初始化
	//        dvnnBondList->List[i].req      = 0.0;  // 键长平衡值
	//        dvnnBondList->List[i].qomdFreq = 0.0;  // QOMD频率
	//        dvnnBondList->List[i].hhmoFreq = 0.0;  // HHMO频率
	//
	//        // 偏差限制参数
	//        dvnnBondList->List[i].MaxDeviation   = 0.0;  // 最大允许偏差
	//        dvnnBondList->List[i].MinDeviation   = 0.0;  // 最小允许偏差
	//        dvnnBondList->List[i].LimitDeviation = 0.0;  // 限制偏差阈值
	//
	//        // 记录参数初始化
	//        dvnnBondList->List[i].Record.Mode     = 0;    // 模式标识
	//        dvnnBondList->List[i].Record.Start    = 0.0;  // 起始值
	//        dvnnBondList->List[i].Record.Target   = 0.0;  // 目标值
	//        dvnnBondList->List[i].Record.Arrive   = 0.0;  // 到达值
	//        dvnnBondList->List[i].Record.Terminal = 0.0;  // 终止值
	//    }
	//
	//    return dvnnBondList;
	//}
	//
	//
	//
	//// 2024-11-07
	///**
	// * 初始化并分配DvnnForceAngleList内存
	// *
	// * 功能描述：
	// * 1. 为AngleList列表分配内存空间
	// * 2. 通过proteinBag获取活动项数和最大项数
	// * 3. 初始化所有AngleList项的默认参数
	// *
	// * @param proteinBag 蛋白质包指针（包含分子结构数据）
	// * @return 初始化完成的DvnnForceAngleList指针
	// */
	//DvnnForceAngleList* dvnn_init_malloc_dvnnAngleList(void* proteinBag)
	//{
	//    //================================================================================//
	//    // 结构体定义参考：
	//    // - DvnnForceAngleList 定义在 aDvnn.h
	//    // - DvnnForceAngle 定义在 aDvnn.h
	//    //================================================================================//
	//
	//    //====================================================================================================================================//
	//    // 依赖函数说明：
	//    // - qomd_shell3_get_dvnnAngleList_actItem_by_proteinBag: 获取当前活动角度项数量（在qomdShell3.cpp定义）
	//    // - qomd_shell3_get_dvnnAngleList_maxItem_by_proteinBag: 获取最大角度项容量（在qomdShell3.cpp定义）
	//    //====================================================================================================================================//
	//
	//    // 分配主结构体内存
	//    DvnnForceAngleList* dvnnAngleList = (DvnnForceAngleList*)malloc(sizeof(DvnnForceAngleList));
	//
	//    // 从蛋白质包获取列表参数
	//    dvnnAngleList->actItem = qomd_shell3_get_dvnnAngleList_actItem_by_proteinBag(proteinBag); // 当前有效项数
	//    dvnnAngleList->maxItem = qomd_shell3_get_dvnnAngleList_maxItem_by_proteinBag(proteinBag); // 最大容量
	//
	//    // 为角度项数组分配内存
	//    dvnnAngleList->List = (DvnnForceAngle*)malloc(dvnnAngleList->maxItem * sizeof(DvnnForceAngle));
	//
	//    // 初始化所有角度项参数（默认值）
	//    for (int i = 0; i < dvnnAngleList->maxItem; i++)
	//    {
	//        // 原子索引初始化（-1表示未分配）
	//        dvnnAngleList->List[i].VertexI  = -1;  // 角度顶点I
	//        dvnnAngleList->List[i].VertexJ  = -1;  // 角度顶点J（中心原子）
	//        dvnnAngleList->List[i].VertexK  = -1;  // 角度顶点K
	//
	//        // 角度参数初始化
	//        dvnnAngleList->List[i].thetaeq  = 0.0;  // 平衡角度值（弧度）
	//        dvnnAngleList->List[i].qomdFreq = 0.0;  // QOMD频率
	//        dvnnAngleList->List[i].hhmoFreq = 0.0;  // HHMO频率
	//
	//        // 偏差限制参数
	//        dvnnAngleList->List[i].MaxDeviation   = 0.0;  // 最大允许偏差（弧度）
	//        dvnnAngleList->List[i].MinDeviation   = 0.0;  // 最小允许偏差（弧度）
	//        dvnnAngleList->List[i].LimitDeviation = 0.0;  // 限制偏差阈值（弧度）
	//
	//        // 记录参数初始化
	//        dvnnAngleList->List[i].Record.Mode     = 0;    // 模式标识
	//        dvnnAngleList->List[i].Record.Start    = 0.0;  // 起始角度值
	//        dvnnAngleList->List[i].Record.Target   = 0.0;  // 目标角度值
	//        dvnnAngleList->List[i].Record.Arrive   = 0.0;  // 到达角度值
	//        dvnnAngleList->List[i].Record.Terminal = 0.0;  // 终止角度值
	//    }
	//
	//    return dvnnAngleList;
	//}
	//
	//
	//
	//// 2024-11-07
	///**
	// * 初始化并分配DvnnForceDihedralList内存
	// *
	// * 功能描述：
	// * 1. 为DihedralList列表分配内存空间
	// * 2. 通过proteinBag获取活动项数和最大项数
	// * 3. 初始化所有DihedralList项的默认参数
	// *
	// * @param proteinBag 蛋白质包指针（包含分子结构数据）
	// * @return 初始化完成的DvnnForceDihedralList指针
	// */
	//DvnnForceDihedralList* dvnn_init_malloc_dvnnDihedralList(void* proteinBag)
	//{
	//    //================================================================================//
	//    // 结构体定义参考：
	//    // - DvnnForceDihedralList 定义在 aDvnn.h
	//    // - DvnnForceDihedral 定义在 aDvnn.h
	//    //================================================================================//
	//
	//    //====================================================================================================================================//
	//    // 依赖函数说明：
	//    // - qomd_shell3_get_dvnnDihedralList_actItem_by_proteinBag: 获取当前活动二面角项数量（在qomdShell3.cpp定义）
	//    // - qomd_shell3_get_dvnnDihedralList_maxItem_by_proteinBag: 获取最大二面角项容量（在qomdShell3.cpp定义）
	//    //====================================================================================================================================//
	//
	//    // 分配主结构体内存
	//    DvnnForceDihedralList* dvnnDihedralList = (DvnnForceDihedralList*)malloc(sizeof(DvnnForceDihedralList));
	//
	//    // 从蛋白质包获取列表参数
	//    dvnnDihedralList->actItem = qomd_shell3_get_dvnnDihedralList_actItem_by_proteinBag(proteinBag); // 当前有效项数
	//    dvnnDihedralList->maxItem = qomd_shell3_get_dvnnDihedralList_maxItem_by_proteinBag(proteinBag); // 最大容量
	//
	//    // 为二面角项数组分配内存
	//    dvnnDihedralList->List = (DvnnForceDihedral*)malloc(dvnnDihedralList->maxItem * sizeof(DvnnForceDihedral));
	//
	//    // 初始化所有二面角项参数（默认值）
	//    for (int i = 0; i < dvnnDihedralList->maxItem; i++)
	//    {
	//        // 原子索引初始化（-1表示未分配）
	//        dvnnDihedralList->List[i].VertexI  = -1;  // 二面角顶点I
	//        dvnnDihedralList->List[i].VertexJ  = -1;  // 二面角顶点J
	//        dvnnDihedralList->List[i].VertexK  = -1;  // 二面角顶点K
	//        dvnnDihedralList->List[i].VertexL  = -1;  // 二面角顶点L
	//
	//        // 频率参数
	//        dvnnDihedralList->List[i].qomdFreq = 0.0;  // QOMD频率（扭转振动）
	//        dvnnDihedralList->List[i].hhmoFreq = 0.0;  // HHMO频率（谐波振动）
	//
	//        // 二面角谷值记录（-1表示未定义）
	//        dvnnDihedralList->List[i].Record.ValleyStart    = -1;  // 起始谷值位置
	//        dvnnDihedralList->List[i].Record.ValleyTarget   = -1;  // 目标谷值位置
	//        dvnnDihedralList->List[i].Record.ValleyArrive   = -1;  // 到达谷值位置
	//        dvnnDihedralList->List[i].Record.ValleyTerminal = -1;  // 终止谷值位置
	//
	//        // 二面角相位记录（弧度）
	//        dvnnDihedralList->List[i].Record.PhiStart    = 0.0;  // 起始相位角
	//        dvnnDihedralList->List[i].Record.PhiTarget   = 0.0;  // 目标相位角
	//        dvnnDihedralList->List[i].Record.PhiArrive   = 0.0;  // 到达相位角
	//        dvnnDihedralList->List[i].Record.PhiTerminal = 0.0;  // 终止相位角
	//    }
	//
	//    return dvnnDihedralList;
	//}
	//
	///**
	// * 初始化并分配DvnnForcePlaneList内存
	// *
	// * 功能描述：
	// * 1. 为PlaneList列表分配内存空间
	// * 2. 通过proteinBag获取活动项数和最大项数
	// * 3. 初始化所有PlaneList项的默认参数
	// *
	// * @param proteinBag 蛋白质包指针（包含分子结构数据）
	// * @return 初始化完成的DvnnForcePlaneList指针
	// */
	//DvnnForcePlaneList* dvnn_init_malloc_dvnnPlaneList(void* proteinBag)
	//{
	//    //================================================================================//
	//    // 结构体定义参考：
	//    // - DvnnForcePlaneList 定义在 aDvnn.h
	//    // - DvnnForcePlane 定义在 aDvnn.h
	//    //================================================================================//
	//
	//    //====================================================================================================================================//
	//    // 依赖函数说明：
	//    // - qomd_shell3_get_dvnnPlaneList_actItem_by_proteinBag: 获取当前活动平面项数量（在qomdShell3.cpp定义）
	//    // - qomd_shell3_get_dvnnPlaneList_maxItem_by_proteinBag: 获取最大平面项容量（在qomdShell3.cpp定义）
	//    //====================================================================================================================================//
	//
	//    // 分配主结构体内存
	//    DvnnForcePlaneList* dvnnPlaneList = (DvnnForcePlaneList*)malloc(sizeof(DvnnForcePlaneList));
	//
	//    // 从蛋白质包获取列表参数
	//    dvnnPlaneList->actItem = qomd_shell3_get_dvnnPlaneList_actItem_by_proteinBag(proteinBag); // 当前有效项数
	//    dvnnPlaneList->maxItem = qomd_shell3_get_dvnnPlaneList_maxItem_by_proteinBag(proteinBag); // 最大容量
	//
	//    // 为平面项数组分配内存
	//    dvnnPlaneList->List = (DvnnForcePlane*)malloc(dvnnPlaneList->maxItem * sizeof(DvnnForcePlane));
	//
	//    // 初始化所有平面项参数（默认值）
	//    for (int i = 0; i < dvnnPlaneList->maxItem; i++)
	//    {
	//        // 原子索引初始化（-1表示未分配）
	//        dvnnPlaneList->List[i].VertexI  = -1;  // 平面顶点I
	//        dvnnPlaneList->List[i].VertexJ  = -1;  // 平面顶点J
	//        dvnnPlaneList->List[i].VertexK  = -1;  // 平面顶点K
	//        dvnnPlaneList->List[i].VertexL  = -1;  // 平面顶点L（参考点）
	//
	//        // 平面参数初始化
	//        dvnnPlaneList->List[i].phieq    = 0.0;  // 平面平衡角度（弧度）
	//        dvnnPlaneList->List[i].qomdFreq = 0.0;  // QOMD频率
	//        dvnnPlaneList->List[i].hhmoFreq = 0.0;  // HHMO频率
	//
	//        // 偏差限制参数
	//        dvnnPlaneList->List[i].MaxDeviation   = 0.0;  // 最大允许偏差（弧度）
	//        dvnnPlaneList->List[i].MinDeviation   = 0.0;  // 最小允许偏差（弧度）
	//        dvnnPlaneList->List[i].LimitDeviation = 0.0;  // 限制偏差阈值（弧度）
	//
	//        // 记录参数初始化
	//        dvnnPlaneList->List[i].Record.Mode     = 0;    // 模式标识
	//        dvnnPlaneList->List[i].Record.Start    = 0.0;  // 起始平面角度
	//        dvnnPlaneList->List[i].Record.Target   = 0.0;  // 目标平面角度
	//        dvnnPlaneList->List[i].Record.Arrive   = 0.0;  // 到达平面角度
	//        dvnnPlaneList->List[i].Record.Terminal = 0.0;  // 终止平面角度
	//    }
	//
	//    return dvnnPlaneList;
	//}
	//
	//
	//
	//
	//
	//// 2023-10-09
	//// 创建并初始化一个新的 DvnnString 类型的空的字符串。
	//DvnnString dvnnString_new_null_string()
	//{
	//	// new str_dst
	//	DvnnString str_dst;		// DvnnString表示一个有最大长度限制的字符串
	//
	//	// reset
	//	str_dst.maxLength = DVNN_NAME_STRING_ARRAY_MAX_LENGTH -1; // DVNN_NAME_STRING_ARRAY_MAX_LENGTH=21，在aDvnn中定义
	//
	//	// reset a null string : actLength is 0
	//	str_dst.actLength = 0;
	//
	//	// reset a null string : all chars are \0
	//	// 初始化字符数组为空字符串：将数组中的所有字符都设置为 0（即 \0），确保整个字符串都是空的。
	//	for (int i = 0; i <= str_dst.maxLength; i++){
	//		str_dst.string[i] = 0;
	//	}
	//
	//	return str_dst;
	//}
	//
	//
	//
	//// 2023-10-09
	//void dvnn_write_dvnnVertexList_to_test_file(DvnnAtomicVertexList * vertexList, char * fname_writerecord)
	//// 将 vertexList 中的内容写入 fname_writerecord 文件中
	//{
	//
	//    ///////////////////////////////////////////////////////////////////////////////////////////////
	//    // open output file : test_output.txt
	//    ///////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	// get output file ptr
	//	FILE* fptr_writerecord;
	//
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//    // BLOCK of <fopen> & <fopen_s>                                  ++ 
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//    #if PROTEIN_RUN_ON_LINUX
	//        // for linux : fopen in <stdio.h>
	//	    fptr_writerecord = fopen(fname_writerecord, "w+");
	//    #endif  // match : #if PROTEIN_RUN_ON_LINUX
	//
	//    #if PROTEIN_RUN_ON_WINDOWS
	//        // for win : fopen_s in <stdio.h>
	//        fopen_s(&fptr_writerecord, fname_writerecord, "w+");
	//    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//    // BLOCK of <fopen> & <fopen_s>                                  ++ 
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//	//use fopen : for linux
	//	//fptr_writerecord = fopen(fname_writerecord, "w+");     // open mode : if not exist then create, if exist then clean, read/write 
	//
	//
	//	if (fptr_writerecord == NULL)
	//	{
	//		printf("failed to open output file < %s >\n", fname_writerecord);
	//		exit(1);
	//	}
	//	else
	//	{
	//		printf("successful open output file < %s >\n", fname_writerecord);
	//	}
	//
	//    ///////////////////////////////////////////////////////////////////////////////////////////////
	//    // end open output file : test_output.txt
	//    ///////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    // write VertexList
	//    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    fprintf(fptr_writerecord, "======= VertexList =======   \n");
	//	// 向文件 fptr_writerecord 中写入======= VertexList =======   \n
	//
	//    for (int i = 0; i < vertexList->actItem; i++)
	//    {
	//    	// 写入数据：函数遍历vertexList->actItem中的每个顶点，并将其详细信息写入fptr_writerecord文件中。
	//    	// 对应输出的dvnn_vertexList_1.txt和dvnn_vertexList_2.txt文件
	//    	// 写入的信息包括顶点索引、指针地址、残基ID、顶点名称序列号、顶点名称ID、顶点名称字符串及其长度、位置信息的有效标志和坐标值。
	//
	//        ////////////////////////
	//        // basic info
	//        ////////////////////////
	//        fprintf(fptr_writerecord, "vIdx=%4d "  , i                                       );
	//        fprintf(fptr_writerecord, "vPtr=%p  "  , &(vertexList->List[i])                  );
	//        fprintf(fptr_writerecord, "resID=%6d " , vertexList->List[i].residueID           );
	//        fprintf(fptr_writerecord, "vNmSN=%6d " , vertexList->List[i].vertexNameSN        );
	//        fprintf(fptr_writerecord, "vNmID=%6d " , vertexList->List[i].vertexNameID        );
	//        fprintf(fptr_writerecord, "vNm=%12s "  , vertexList->List[i].vertexName.string   );
	//        fprintf(fptr_writerecord, "actL=%2d "  , vertexList->List[i].vertexName.actLength);
	//        fprintf(fptr_writerecord, "maxL=%2d "  , vertexList->List[i].vertexName.maxLength);
	//
	//        fprintf(fptr_writerecord, "qomdCharge=%10.6f " , vertexList->List[i].qomdCharge);
	//        fprintf(fptr_writerecord, "hhmoCharge=%10.6f " , vertexList->List[i].hhmoCharge);
	//
	//        fprintf(fptr_writerecord, "posV=%2d "    , vertexList->List[i].Position.valid);
	//        fprintf(fptr_writerecord, "posX=%12.6f " , vertexList->List[i].Position.x);
	//        fprintf(fptr_writerecord, "posY=%12.6f " , vertexList->List[i].Position.y);
	//        fprintf(fptr_writerecord, "posZ=%12.6f " , vertexList->List[i].Position.z);
	//
	//        fprintf(fptr_writerecord, "\n");
	//
	//    }
	//
	//
	//
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// close output file
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	//use fclose  // in <stdio.h>
	//
	//	// close fptr_writerecord
	//	if (fclose(fptr_writerecord))
	//	{
	//		printf("failed to close < %s >\n", fname_writerecord);
	//	}
	//	else
	//	{
	//		printf("successfully close < %s >\n", fname_writerecord);
	//	}
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// end close output file
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//}
	//
	//
	//
	//// 2024-11-09
	///**
	// * 将所有DVNN数据列表写入测试文件
	// *
	// * 功能描述：
	// * 1. 将顶点列表、键合/角度/二面角/平面力列表统一写入测试文件
	// * 2. 包含详细的数值记录和关联信息输出
	// * 3. 支持跨平台文件操作（Linux/Windows）
	// *
	// * @param dvnnVertexList 	顶点列表指针
	// * @param dvnnBondList 		BondList列表指针
	// * @param dvnnAngleList 	AngleList列表指针
	// * @param dvnnDihedralList 	DihedralList列表指针
	// * @param dvnnPlaneList 	PlaneList列表指针
	// * @param fname_writerecord 输出文件名
	// */
	//void dvnn_write_dvnnAllList_to_test_file(
	//    DvnnAtomicVertexList  * dvnnVertexList  ,
	//    DvnnForceBondList     * dvnnBondList    ,
	//    DvnnForceAngleList    * dvnnAngleList   ,
	//    DvnnForceDihedralList * dvnnDihedralList,
	//    DvnnForcePlaneList    * dvnnPlaneList   ,
	//	char                  * fname_writerecord)
	//{
	//
	//    ///////////////////////////////////////////////////////////////////////////////////////////////
	//    // open output file : test_output.txt
	//    ///////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	// get output file ptr
	//	FILE* fptr_writerecord;
	//
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//    // BLOCK of <fopen> & <fopen_s>                                  ++
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//    #if PROTEIN_RUN_ON_LINUX
	//        // for linux : fopen in <stdio.h>
	//	    fptr_writerecord = fopen(fname_writerecord, "w+");
	//    #endif  // match : #if PROTEIN_RUN_ON_LINUX
	//
	//    #if PROTEIN_RUN_ON_WINDOWS
	//        // for win : fopen_s in <stdio.h>
	//        fopen_s(&fptr_writerecord, fname_writerecord, "w+");
	//    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//    // BLOCK of <fopen> & <fopen_s>                                  ++
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//	//use fopen : for linux
	//	//fptr_writerecord = fopen(fname_writerecord, "w+");     // open mode : if not exist then create, if exist then clean, read/write
	//
	//
	//	if (fptr_writerecord == NULL)
	//	{
	//		printf("failed to open output file < %s >\n", fname_writerecord);
	//		exit(1);
	//	}
	//	else
	//	{
	//		printf("successful open output file < %s >\n", fname_writerecord);
	//	}
	//
	//    ///////////////////////////////////////////////////////////////////////////////////////////////
	//    // end open output file : test_output.txt
	//    ///////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    // write dvnnVertexList，写入顶点列表数据
	//    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    fprintf(fptr_writerecord, "======= dvnnVertexList =======   \n");
	//
	//    fprintf(fptr_writerecord, "dvnnVertexList->actItem = %12d " , dvnnVertexList->actItem          );
	//    fprintf(fptr_writerecord, "dvnnVertexList->maxItem = %12d " , dvnnVertexList->maxItem          );
	//
	//    fprintf(fptr_writerecord, "\n\n");
	//
	//    for (int i = 0; i < dvnnVertexList->actItem; i++)
	//    {
	//    	// 顶点基本信息
	//        fprintf(fptr_writerecord, "vIdx=%6d "    , i                                           );
	//        fprintf(fptr_writerecord, "resID=%6d "   , dvnnVertexList->List[i].residueID           );
	//        fprintf(fptr_writerecord, "vNmSN=%6d "   , dvnnVertexList->List[i].vertexNameSN        );
	//        fprintf(fptr_writerecord, "vNmID=%6d "   , dvnnVertexList->List[i].vertexNameID        );
	//        fprintf(fptr_writerecord, "vNm=%12s "    , dvnnVertexList->List[i].vertexName.string   );
	//    	// 电荷信息
	//        fprintf(fptr_writerecord, "qCha=%10.6f " , dvnnVertexList->List[i].qomdCharge          );
	//        fprintf(fptr_writerecord, "hCha=%10.6f " , dvnnVertexList->List[i].hhmoCharge          );
	//    	// 位置信息
	//        fprintf(fptr_writerecord, "posV=%2d "    , dvnnVertexList->List[i].Position.valid      );
	//        fprintf(fptr_writerecord, "posX=%12.6f " , dvnnVertexList->List[i].Position.x          );
	//        fprintf(fptr_writerecord, "posY=%12.6f " , dvnnVertexList->List[i].Position.y          );
	//        fprintf(fptr_writerecord, "posZ=%12.6f " , dvnnVertexList->List[i].Position.z          );
	//
	//        fprintf(fptr_writerecord, "\n");
	//    }
	//
	//    fprintf(fptr_writerecord, "\n\n");
	//
	//
	//
	//
	//    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    // write dvnnBondList，写入bond列表数据
	//    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    fprintf(fptr_writerecord, "======= dvnnBondList =======   \n");
	//
	//    fprintf(fptr_writerecord, "dvnnBondList->actItem = %12d " , dvnnBondList->actItem          );
	//    fprintf(fptr_writerecord, "dvnnBondList->maxItem = %12d " , dvnnBondList->maxItem          );
	//
	//    fprintf(fptr_writerecord, "\n\n");
	//
	//    for (int i = 0; i < dvnnBondList->actItem; i++)
	//    {
	//    	// 键合原子索引
	//        fprintf(fptr_writerecord, "bondIdx=%6d "     , i                                                                         );
	//        fprintf(fptr_writerecord, "vI=%6d "          , dvnnBondList->List[i].VertexI                                             );
	//        fprintf(fptr_writerecord, "vJ=%6d "          , dvnnBondList->List[i].VertexJ                                             );
	//        fprintf(fptr_writerecord, "vInm=%12s "       , dvnnVertexList->List[(dvnnBondList->List[i].VertexI)].vertexName.string   );
	//        fprintf(fptr_writerecord, "vJnm=%12s "       , dvnnVertexList->List[(dvnnBondList->List[i].VertexJ)].vertexName.string   );
	//    	// 键合参数
	//        fprintf(fptr_writerecord, "req     =%12.6f " , dvnnBondList->List[i].req                                                 );
	//        fprintf(fptr_writerecord, "BqomdFreq=%12.6f ", dvnnBondList->List[i].qomdFreq                                            );
	//        fprintf(fptr_writerecord, "BhhmoFreq=%12.6f ", dvnnBondList->List[i].hhmoFreq                                            );
	//    	// 偏差控制
	//        fprintf(fptr_writerecord, "MaxDev=%12.6f "   , dvnnBondList->List[i].MaxDeviation                                        );
	//        fprintf(fptr_writerecord, "MinDev=%12.6f "   , dvnnBondList->List[i].MinDeviation                                        );
	//        fprintf(fptr_writerecord, "LimDev=%12.6f "   , dvnnBondList->List[i].LimitDeviation                                      );
	//    	// 记录参数
	//        fprintf(fptr_writerecord, "Mode=%6d "        , dvnnBondList->List[i].Record.Mode                                         );
	//        fprintf(fptr_writerecord, "Start=%12.6f "    , dvnnBondList->List[i].Record.Start                                        );
	//        fprintf(fptr_writerecord, "Target=%12.6f "   , dvnnBondList->List[i].Record.Target                                       );
	//        fprintf(fptr_writerecord, "Arrive=%12.6f "   , dvnnBondList->List[i].Record.Arrive                                       );
	//        fprintf(fptr_writerecord, "Terminal=%12.6f " , dvnnBondList->List[i].Record.Terminal                                     );
	//
	//        fprintf(fptr_writerecord, "\n");
	//    }
	//
	//    fprintf(fptr_writerecord, "\n\n");
	//
	//
	//
	//    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    // write dvnnAngleList，写入Angle列表数据
	//    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    fprintf(fptr_writerecord, "======= dvnnAngleList =======   \n");
	//
	//    fprintf(fptr_writerecord, "dvnnAngleList->actItem = %12d " , dvnnAngleList->actItem          );
	//    fprintf(fptr_writerecord, "dvnnAngleList->maxItem = %12d " , dvnnAngleList->maxItem          );
	//
	//    fprintf(fptr_writerecord, "\n\n");
	//
	//    for (int i = 0; i < dvnnAngleList->actItem; i++)
	//    {
	//        fprintf(fptr_writerecord, "angleIdx=%6d "      , i                                                                          );
	//        fprintf(fptr_writerecord, "vI=%6d "            , dvnnAngleList->List[i].VertexI                                             );
	//        fprintf(fptr_writerecord, "vJ=%6d "            , dvnnAngleList->List[i].VertexJ                                             );
	//        fprintf(fptr_writerecord, "vK=%6d "            , dvnnAngleList->List[i].VertexK                                             );
	//        fprintf(fptr_writerecord, "vInm=%12s "         , dvnnVertexList->List[(dvnnAngleList->List[i].VertexI)].vertexName.string   );
	//        fprintf(fptr_writerecord, "vJnm=%12s "         , dvnnVertexList->List[(dvnnAngleList->List[i].VertexJ)].vertexName.string   );
	//        fprintf(fptr_writerecord, "vKnm=%12s "         , dvnnVertexList->List[(dvnnAngleList->List[i].VertexK)].vertexName.string   );
	//        fprintf(fptr_writerecord, "AqomdFreq=%12.6f "  , dvnnAngleList->List[i].qomdFreq                                            );
	//        fprintf(fptr_writerecord, "AhhmoFreq=%12.6f "  , dvnnAngleList->List[i].hhmoFreq                                            );
	//        fprintf(fptr_writerecord, "thetaeqDEG=%12.6f " , dvnnAngleList->List[i].thetaeq                                             );
	//        fprintf(fptr_writerecord, "thetaeqRAD=%12.6f " , ((dvnnAngleList->List[i].thetaeq)/180.0) * ((double)(DVNN_PI))             );
	//        fprintf(fptr_writerecord, "MaxDev=%12.6f "     , dvnnAngleList->List[i].MaxDeviation                                        );
	//        fprintf(fptr_writerecord, "MinDev=%12.6f "     , dvnnAngleList->List[i].MinDeviation                                        );
	//        fprintf(fptr_writerecord, "LimDev=%12.6f "     , dvnnAngleList->List[i].LimitDeviation                                      );
	//        fprintf(fptr_writerecord, "Mode=%6d "          , dvnnAngleList->List[i].Record.Mode                                         );
	//        fprintf(fptr_writerecord, "Start=%12.6f "      , dvnnAngleList->List[i].Record.Start                                        );
	//        fprintf(fptr_writerecord, "Target=%12.6f "     , dvnnAngleList->List[i].Record.Target                                       );
	//        fprintf(fptr_writerecord, "Arrive=%12.6f "     , dvnnAngleList->List[i].Record.Arrive                                       );
	//        fprintf(fptr_writerecord, "Terminal=%12.6f "   , dvnnAngleList->List[i].Record.Terminal                                     );
	//
	//        fprintf(fptr_writerecord, "\n");
	//    }
	//
	//    fprintf(fptr_writerecord, "\n\n");
	//
	//
	//
	//    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    // write dvnnDihedralList，写入二面角列表数据
	//    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    fprintf(fptr_writerecord, "======= dvnnDihedralList =======   \n");
	//
	//    fprintf(fptr_writerecord, "dvnnDihedralList->actItem = %12d " , dvnnDihedralList->actItem          );
	//    fprintf(fptr_writerecord, "dvnnDihedralList->maxItem = %12d " , dvnnDihedralList->maxItem          );
	//
	//    fprintf(fptr_writerecord, "\n\n");
	//
	//    for (int i = 0; i < dvnnDihedralList->actItem; i++)
	//    {
	//        fprintf(fptr_writerecord, "dihedralIdx=%6d "     , i                                                                             );
	//        fprintf(fptr_writerecord, "vI=%6d "              , dvnnDihedralList->List[i].VertexI                                             );
	//        fprintf(fptr_writerecord, "vJ=%6d "              , dvnnDihedralList->List[i].VertexJ                                             );
	//        fprintf(fptr_writerecord, "vK=%6d "              , dvnnDihedralList->List[i].VertexK                                             );
	//        fprintf(fptr_writerecord, "vL=%6d "              , dvnnDihedralList->List[i].VertexL                                             );
	//        fprintf(fptr_writerecord, "vInm=%12s "           , dvnnVertexList->List[(dvnnDihedralList->List[i].VertexI)].vertexName.string   );
	//        fprintf(fptr_writerecord, "vJnm=%12s "           , dvnnVertexList->List[(dvnnDihedralList->List[i].VertexJ)].vertexName.string   );
	//        fprintf(fptr_writerecord, "vKnm=%12s "           , dvnnVertexList->List[(dvnnDihedralList->List[i].VertexK)].vertexName.string   );
	//        fprintf(fptr_writerecord, "vLnm=%12s "           , dvnnVertexList->List[(dvnnDihedralList->List[i].VertexL)].vertexName.string   );
	//        fprintf(fptr_writerecord, "DqomdFreq=%12.6f "    , dvnnDihedralList->List[i].qomdFreq                                            );
	//        fprintf(fptr_writerecord, "DhhmoFreq=%12.6f "    , dvnnDihedralList->List[i].hhmoFreq                                            );
	//
	//        fprintf(fptr_writerecord, "ValleyStart=%6d "     , dvnnDihedralList->List[i].Record.ValleyStart                                  );
	//        fprintf(fptr_writerecord, "ValleyTarget=%6d "    , dvnnDihedralList->List[i].Record.ValleyTarget                                 );
	//        fprintf(fptr_writerecord, "ValleyArrive=%6d "    , dvnnDihedralList->List[i].Record.ValleyArrive                                 );
	//        fprintf(fptr_writerecord, "ValleyTerminal=%6d "  , dvnnDihedralList->List[i].Record.ValleyTerminal                               );
	//        fprintf(fptr_writerecord, "PhiStart=%12.6f "     , dvnnDihedralList->List[i].Record.PhiStart                                     );
	//        fprintf(fptr_writerecord, "PhiTarget=%12.6f "    , dvnnDihedralList->List[i].Record.PhiTarget                                    );
	//        fprintf(fptr_writerecord, "PhiArrive=%12.6f "    , dvnnDihedralList->List[i].Record.PhiArrive                                    );
	//        fprintf(fptr_writerecord, "PhiTerminal=%12.6f "  , dvnnDihedralList->List[i].Record.PhiTerminal                                  );
	//
	//        fprintf(fptr_writerecord, "\n");
	//    }
	//
	//    fprintf(fptr_writerecord, "\n\n");
	//
	//
	//
	//    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    // write dvnnPlaneList，写入平面力列表数据
	//    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    fprintf(fptr_writerecord, "======= dvnnPlaneList =======   \n");
	//
	//    fprintf(fptr_writerecord, "dvnnPlaneList->actItem = %12d " , dvnnPlaneList->actItem          );
	//    fprintf(fptr_writerecord, "dvnnPlaneList->maxItem = %12d " , dvnnPlaneList->maxItem          );
	//
	//    fprintf(fptr_writerecord, "\n\n");
	//
	//    for (int i = 0; i < dvnnPlaneList->actItem; i++)
	//    {
	//        fprintf(fptr_writerecord, "planeIdx=%6d "    , i                                                                          );
	//        fprintf(fptr_writerecord, "vI=%6d "          , dvnnPlaneList->List[i].VertexI                                             );
	//        fprintf(fptr_writerecord, "vJ=%6d "          , dvnnPlaneList->List[i].VertexJ                                             );
	//        fprintf(fptr_writerecord, "vK=%6d "          , dvnnPlaneList->List[i].VertexK                                             );
	//        fprintf(fptr_writerecord, "vL=%6d "          , dvnnPlaneList->List[i].VertexL                                             );
	//        fprintf(fptr_writerecord, "vInm=%12s "       , dvnnVertexList->List[(dvnnPlaneList->List[i].VertexI)].vertexName.string   );
	//        fprintf(fptr_writerecord, "vJnm=%12s "       , dvnnVertexList->List[(dvnnPlaneList->List[i].VertexJ)].vertexName.string   );
	//        fprintf(fptr_writerecord, "vKnm=%12s "       , dvnnVertexList->List[(dvnnPlaneList->List[i].VertexK)].vertexName.string   );
	//        fprintf(fptr_writerecord, "vLnm=%12s "       , dvnnVertexList->List[(dvnnPlaneList->List[i].VertexL)].vertexName.string   );
	//        fprintf(fptr_writerecord, "PqomdFreq=%12.6f ", dvnnPlaneList->List[i].qomdFreq                                            );
	//        fprintf(fptr_writerecord, "PhhmoFreq=%12.6f ", dvnnPlaneList->List[i].hhmoFreq                                            );
	//        fprintf(fptr_writerecord, "phieqDEG=%12.6f " , dvnnPlaneList->List[i].phieq                                               );
	//        fprintf(fptr_writerecord, "phieqRAD=%12.6f " , ((dvnnPlaneList->List[i].phieq)/180.0) * ((double)(DVNN_PI))               );
	//        fprintf(fptr_writerecord, "MaxDev=%12.6f "   , dvnnPlaneList->List[i].MaxDeviation                                        );
	//        fprintf(fptr_writerecord, "MinDev=%12.6f "   , dvnnPlaneList->List[i].MinDeviation                                        );
	//        fprintf(fptr_writerecord, "LimDev=%12.6f "   , dvnnPlaneList->List[i].LimitDeviation                                      );
	//        fprintf(fptr_writerecord, "Mode=%6d "        , dvnnPlaneList->List[i].Record.Mode                                         );
	//        fprintf(fptr_writerecord, "Start=%12.6f "    , dvnnPlaneList->List[i].Record.Start                                        );
	//        fprintf(fptr_writerecord, "Target=%12.6f "   , dvnnPlaneList->List[i].Record.Target                                       );
	//        fprintf(fptr_writerecord, "Arrive=%12.6f "   , dvnnPlaneList->List[i].Record.Arrive                                       );
	//        fprintf(fptr_writerecord, "Terminal=%12.6f " , dvnnPlaneList->List[i].Record.Terminal                                     );
	//
	//        fprintf(fptr_writerecord, "\n");
	//    }
	//
	//    fprintf(fptr_writerecord, "\n\n");
	//
	//
	//
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// close output file
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	//use fclose  // in <stdio.h>
	//
	//	// close fptr_writerecord
	//	if (fclose(fptr_writerecord))
	//	{
	//		printf("failed to close < %s >\n", fname_writerecord);
	//	}
	//	else
	//	{
	//		printf("successfully close < %s >\n", fname_writerecord);
	//	}
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// end close output file
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//}
	//
	//
	//
	//
	//// 2024-03-04
	//DvnnTimePointList * dvnn_init_timePointList_malloc(void * atomicVertexList)
	//// 初始化一个 DvnnTimePointList 结构体，并为其分配内存。该函数接受一个指向 DvnnAtomicVertexList 结构体的指针作为参数，并基于该顶点列表创建一个新的时间点列表。
	//{
	//	DvnnAtomicVertexList * vertexList = ((DvnnAtomicVertexList*)atomicVertexList);
	//
	//	// new an empty timePointList
	//	// 分配内存：使用 malloc 为 DvnnTimePointList 结构体本身分配内存。根据 vertexList->maxItem 的值，为 List 数组中的每个 DvnnTimePoint 元素分配内存。
	//    DvnnTimePointList * timePointList = (DvnnTimePointList*)malloc(sizeof(DvnnTimePointList));
	//
	//	timePointList->actItem = vertexList->actItem;
	//	timePointList->maxItem = vertexList->maxItem;
	//
	//    timePointList->List = (DvnnTimePoint*) malloc( (timePointList->maxItem) * sizeof(DvnnTimePoint) );
	//
	//    // copy data from vertexList
	//	for (int i = 0; i < vertexList->maxItem; i++)
	//	{
	//		timePointList->List[i].vertexIdx    = i;
	//		timePointList->List[i].t            = 0;
	//
	//		timePointList->List[i].vertexNameSN = vertexList->List[i].vertexNameSN;
	//		timePointList->List[i].valid        = vertexList->List[i].Position.valid;
	//		timePointList->List[i].x            = vertexList->List[i].Position.x;
	//		timePointList->List[i].y            = vertexList->List[i].Position.y;
	//		timePointList->List[i].z            = vertexList->List[i].Position.z;
	//
	//	}
	//
	//
	//    // return 
	//	return timePointList;
	//
	//}
	//
	//
	//
	//
	//
	//// 2024-03-04
	//// 将顶点坐标列表按时间戳复制到时间点列表（用于轨迹记录）
	//void dvnn_copy_vertexList_to_timePointList_by_timeStamp(
	//    DvnnTimePointList * timePointList,  // [输出] 目标时间点列表（需预分配内存）
	//    DvnnAtomicVertexList * vertexList,  // [输入] 原子顶点坐标列表
	//    int timeStamp                        // [输入] 当前时间戳（如模拟步数）
	//) {
	//    // 遍历所有有效原子顶点
	//    for (int i = 0; i < vertexList->actItem; i++) {
	//        // 设置时间戳
	//        timePointList->List[i].t = timeStamp;
	//
	//        // 复制顶点有效性标志（用于过滤无效原子）
	//        timePointList->List[i].valid = vertexList->List[i].Position.valid;
	//
	//        // 复制三维坐标（保持与顶点列表数据同步）
	//        timePointList->List[i].x = vertexList->List[i].Position.x;
	//        timePointList->List[i].y = vertexList->List[i].Position.y;
	//        timePointList->List[i].z = vertexList->List[i].Position.z;
	//    }
	//}
	//// 2024-03-05
	//void dvnn_save_timePointList_paraData_to_byte_file(char * fname_output_TIME_POINT_byte, int totalStamps, int totalVeretx)
	//// 写入固定大小的参数数据（totalStamps、totalVeretx）到文件，并且每次写入时会清空文件内容，适合用于保存初始参数。
	//// 使用 "wb" 写入模式，每次打开文件时都会清空文件的内容，然后从头开始写入数据。适合用来写入一些新的数据（如参数数据）到文件中，但不保留以前的内容。
	//{
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// open file fname_output_TIME_POINT_byte
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	// get output file ptr
	//	FILE* fptr_output_TIME_POINT_byte;
	//    
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//    // BLOCK of <fopen> & <fopen_s>                                  ++ 
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//    #if PROTEIN_RUN_ON_LINUX
	//        // for linux : fopen in <stdio.h>
	//	fptr_output_TIME_POINT_byte = fopen(fname_output_TIME_POINT_byte, "wb");
	//    #endif  // match : #if PROTEIN_RUN_ON_LINUX
	//
	//    #if PROTEIN_RUN_ON_WINDOWS
	//        // for win : fopen_s in <stdio.h>
	//        fopen_s(&fptr_output_TIME_POINT_byte, fname_output_TIME_POINT_byte, "wb");
	//    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//    // BLOCK of <fopen> & <fopen_s>                                  ++ 
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//	//use fopen : for linux
	//	//fptr_output_TIME_POINT_byte = fopen(fname_output_TIME_POINT_byte, "wb");     // write/binary/create/clean
	//	//fptr_output_TIME_POINT_byte = fopen(fname_output_TIME_POINT_byte, "ab");     // append write/binary/create/not clean
	//   
	//	if (fptr_output_TIME_POINT_byte == NULL)
	//	{
	//	   	printf("failed to open output file  < %s >\n", fname_output_TIME_POINT_byte);
	//	   	exit(1);
	//	}
	//	else
	//	{
	//	   	printf("successful open output file  < %s >\n", fname_output_TIME_POINT_byte);
	//	}
	//
	//
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// copy data
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    
	//
	//	/////////////////////////////////////////////////////////////
	//    // step 1 : totalStamps
	//	/////////////////////////////////////////////////////////////
	//
	//	////////////////////////////
	//    // totalStamps : byte_size 
	//	////////////////////////////
	//	int byteSize_totalStamps = sizeof(int);
	//
	//
	//	////////////////////////////
	//    // totalStamps : copy 
	//	////////////////////////////
	//    // cast data to char* list
	//	char * byteDataBlock_totalStamps = (char*)(&(totalStamps));
	//
	//    // write char* list to byte file
	//	for (int i = 0; i < byteSize_totalStamps; i++)
	//	{
	//		fputc((int)(byteDataBlock_totalStamps[i]), fptr_output_TIME_POINT_byte);
	//	}
	//    printf("SAVE DONE : totalStamps ! totalStamps =%10d \n", totalStamps);
	//
	//
	//	/////////////////////////////////////////////////////////////
	//    // step 2 : totalVeretx
	//	/////////////////////////////////////////////////////////////
	//
	//	////////////////////////////
	//    // totalVeretx : byte_size 
	//	////////////////////////////
	//	int byteSize_totalVeretx = sizeof(int);
	//
	//
	//	////////////////////////////
	//    // totalVeretx : copy 
	//	////////////////////////////
	//    // cast data to char* list
	//	char * byteDataBlock_totalVeretx = (char*)(&(totalVeretx));
	//
	//    // write char* list to byte file
	//	for (int i = 0; i < byteSize_totalVeretx; i++)
	//	{
	//		fputc((int)(byteDataBlock_totalVeretx[i]), fptr_output_TIME_POINT_byte);
	//	}
	//    printf("SAVE DONE : totalVeretx ! totalVeretx =%10d \n", totalVeretx);
	//
	//
	//
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// close file fptr_output_TIME_POINT_byte
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    
	//	//use fclose  // in <stdio.h>
	//    
	//	// close fptr_output_TIME_POINT_byte
	//	if (fclose(fptr_output_TIME_POINT_byte))
	//	{
	//	  	printf("failed to close  < %s >\n", fname_output_TIME_POINT_byte);
	//	}
	//	else
	//	{
	//	   	printf("successfully close  < %s >\n", fname_output_TIME_POINT_byte);
	//	}
	//	
	//
	//
	//}
	//
	//
	//
	//// 2024-03-04
	//void dvnn_save_timePointList_listData_to_byte_file(char * fname_output_TIME_POINT_byte, DvnnTimePointList * timePointList)
	//// 写入动态大小的结构体数据（DvnnTimePointList 的列表），数据会追加到已有的文件内容之后，适合用于在文件中不断追加新的列表数据。
	//// 使用 "ab" 追加模式，每次打开文件时，数据会追加到文件的末尾，而不会清空文件。适合用于需要保留现有数据并在其后追加新的数据（如将新的时间点列表追加到文件中）。
	//
	//{
	//
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// open file fname_output_TIME_POINT_byte
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	// get output file ptr
	//	FILE* fptr_output_TIME_POINT_byte;
	//    
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//    // BLOCK of <fopen> & <fopen_s>                                  ++ 
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//    #if PROTEIN_RUN_ON_LINUX
	//        // for linux : fopen in <stdio.h>
	//	fptr_output_TIME_POINT_byte = fopen(fname_output_TIME_POINT_byte, "ab");
	//    #endif  // match : #if PROTEIN_RUN_ON_LINUX
	//
	//    #if PROTEIN_RUN_ON_WINDOWS
	//        // for win : fopen_s in <stdio.h>
	//        fopen_s(&fptr_output_TIME_POINT_byte, fname_output_TIME_POINT_byte, "ab");
	//    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//    // BLOCK of <fopen> & <fopen_s>                                  ++ 
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//	//use fopen : for linux
	//	//fptr_output_TIME_POINT_byte = fopen(fname_output_TIME_POINT_byte, "ab");     // append write/binary/create/not clean
	//
	//
	//	if (fptr_output_TIME_POINT_byte == NULL)
	//	{
	//	   	printf("failed to open output file  < %s >\n", fname_output_TIME_POINT_byte);
	//	   	exit(1);
	//	}
	//	else
	//	{
	//	   	printf("successful open output file  < %s >\n", fname_output_TIME_POINT_byte);
	//	}
	//
	//
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// copy data
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    ////////////////////////////
	//    // byte_size 
	//	////////////////////////////
	//	int byteSize_timePointList = sizeof(DvnnTimePoint) * (timePointList->actItem);
	//
	//
	//    ////////////////////////////
	//    // copy 
	//	////////////////////////////
	//    // cast data to char* list
	//	char * byteDataBlock_timePointList = (char*)(timePointList->List);
	//
	//    // write char* list to byte file
	//	for (int i = 0; i < byteSize_timePointList; i++)
	//	{
	//		fputc((int)(byteDataBlock_timePointList[i]), fptr_output_TIME_POINT_byte);
	//	}
	//    printf("SAVE DONE : timePointList ! \n");
	//
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// close file fptr_output_TIME_POINT_byte
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    
	//	//use fclose  // in <stdio.h>
	//    
	//	// close fptr_output_TIME_POINT_byte
	//	if (fclose(fptr_output_TIME_POINT_byte))
	//	{
	//	  	printf("failed to close  < %s >\n", fname_output_TIME_POINT_byte);
	//	}
	//	else
	//	{
	//	   	printf("successfully close  < %s >\n", fname_output_TIME_POINT_byte);
	//	}
	//	
	//
	//}
	//
	//
	//
	//// 2024-03-04
	//DvnnTimePointList * dvnn_get_timePointList_from_byte_file(char * fname_input_TIME_POINT_byte)
	//// 将数据文件 fname_input_TIME_POINT_byte 中的数据读入数据列表 timePointList，
	//// 先读入两个整数，整数 totalStamps（总时刻数）和整数 totalVeretx（总原子数），
	//{
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// open file fname_input_TIME_POINT_byte
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	// get input file ptr
	//	FILE* fptr_input_TIME_POINT_byte;
	//    
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//    // BLOCK of <fopen> & <fopen_s>                                  ++ 
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//    #if PROTEIN_RUN_ON_LINUX
	//        // for linux : fopen in <stdio.h>
	//	fptr_input_TIME_POINT_byte = fopen(fname_input_TIME_POINT_byte, "rb");
	//    #endif  // match : #if PROTEIN_RUN_ON_LINUX
	//
	//    #if PROTEIN_RUN_ON_WINDOWS
	//        // for win : fopen_s in <stdio.h>
	//        fopen_s(&fptr_input_TIME_POINT_byte, fname_input_TIME_POINT_byte, "rb");
	//    #endif  // match : #if PROTEIN_RUN_ON_WINDOWS
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//    // BLOCK of <fopen> & <fopen_s>                                  ++ 
	//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//	//use fopen : for linux
	//	//fptr_input_TIME_POINT_byte = fopen(fname_input_TIME_POINT_byte, "rb");     // read/binary
	//
	//
	//	if (fptr_input_TIME_POINT_byte == NULL)
	//	{
	//	 	printf("failed to open output file  < %s >\n", fname_input_TIME_POINT_byte);
	//	   	exit(1);
	//	}
	//	else
	//	{
	//	   	printf("successful open output file  < %s >\n", fname_input_TIME_POINT_byte);
	//	}
	//
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    // read para data from header of byte file
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	/////////////////////////////////////////////////////////////
	//    // step 1 : totalStamps
	//	/////////////////////////////////////////////////////////////
	//
	//    // byte_size 
	//	int byteSize_totalStamps = sizeof(int);
	//
	//    // malloc char* byteDataBlock
	//    char * byteDataBlock_totalStamps = (char *)malloc( sizeof(char) * byteSize_totalStamps );
	//
	//    // read char* list from byte file
	//	for (int i = 0; i < byteSize_totalStamps; i++)
	//	{	
	//		byteDataBlock_totalStamps[i] = (char)fgetc(fptr_input_TIME_POINT_byte);		
	//	}
	//
	//    // malloc new para
	//    int * totalStamps = (int *)malloc( sizeof(int) );
	//    
	//    // recast
	//	totalStamps = (int*)byteDataBlock_totalStamps;
	//
	//	// print
	//	printf(" totalStamps =%10d ", (*totalStamps));
	//	printf("\n");
	//
	//
	//
	//	/////////////////////////////////////////////////////////////
	//    // step 2 : totalVeretx
	//	/////////////////////////////////////////////////////////////
	//
	//    // byte_size 
	//	int byteSize_totalVeretx = sizeof(int);
	//
	//    // malloc char* byteDataBlock
	//    char * byteDataBlock_totalVeretx = (char *)malloc( sizeof(char) * byteSize_totalVeretx );
	//
	//    // read char* list from byte file
	//	for (int i = 0; i < byteSize_totalVeretx; i++)
	//	{	
	//		byteDataBlock_totalVeretx[i] = (char)fgetc(fptr_input_TIME_POINT_byte);		
	//	}
	//
	//    // malloc new para
	//    int * totalVeretx = (int *)malloc( sizeof(int) );
	//    
	//    // recast
	//	totalVeretx = (int*)byteDataBlock_totalVeretx;
	//
	//	// print
	//	printf(" totalVeretx =%10d ", (*totalVeretx));
	//	printf("\n");
	//
	//
	//
	//
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// reset a new timePointList and malloc
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// new an empty timePointList
	//    DvnnTimePointList * timePointList = (DvnnTimePointList*)malloc(sizeof(DvnnTimePointList));
	//
	//	//timePointList->actItem = total_stamps * total_veretx;
	//	//timePointList->maxItem = total_stamps * total_veretx;
	//
	//	timePointList->actItem = (*totalStamps) * (*totalVeretx);
	//	timePointList->maxItem = (*totalStamps) * (*totalVeretx);
	//
	//    // only malloc
	//	timePointList->List = (DvnnTimePoint*) malloc( (timePointList->maxItem) * sizeof(DvnnTimePoint) );
	//
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    // read list data to byteDataBlock 
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//    // byte_size of byteDataBlock_timePointList
	//	int byteSize_timePointList = sizeof(DvnnTimePoint) * (timePointList->actItem);
	//
	//    // malloc char* byteDataBlock
	//    char * byteDataBlock_timePointList = (char *)malloc( sizeof(char) * byteSize_timePointList );
	//
	//    // read char* list from byte file
	//	for (int i = 0; i < byteSize_timePointList; i++)
	//	{	
	//		byteDataBlock_timePointList[i] = (char)fgetc(fptr_input_TIME_POINT_byte);		
	//	}
	//
	//
	// 	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    // recast timePointList->List
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//   // recast
	//    timePointList->List = (DvnnTimePoint*)byteDataBlock_timePointList;
	//
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// close file fname_input_TIME_POINT_byte
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    
	//	//use fclose  // in <stdio.h>
	//    
	//	// close fname_input_TIME_POINT_byte
	//	if (fclose(fptr_input_TIME_POINT_byte))
	//	{
	//	   	printf("failed to close  < %s >\n", fname_input_TIME_POINT_byte);
	//	}
	//	else
	//	{
	//	   	printf("successfully close  < %s >\n", fname_input_TIME_POINT_byte);
	//	}
	//
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// return
	//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	return timePointList;
	//
	//}////////////////////////////////////////////////////////////////////////////////////////////////////////
	// return
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return timePointList;

}