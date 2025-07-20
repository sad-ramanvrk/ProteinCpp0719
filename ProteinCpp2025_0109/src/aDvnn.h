/**
 * @Version 1.0
 * @Date 2023-12-19
 * @Author Zhaohui Meng
 * @Affiliation Department of Computer Science, Hohai University, Nanjing, China.
 */




/////////////////////////////////////////////////////////////
//  Dvnn : dynamic valued neural networks                  //
//  2023-10-08                                             //
/////////////////////////////////////////////////////////////


#ifndef _DVNN_PI_DEFINED
#define DVNN_PI 3.14159265358979323846
#define _DVNN_PI_DEFINED
#endif /* DVNN_PI */



/* 2023-10-09 */
#ifndef _DvnnPara_DEFINED

// MAX string LENGTH
constexpr int DVNN_NAME_STRING_ARRAY_MAX_LENGTH = 21;
// 20 + 1, add '\0'。表示字符串数组的最大长度，其中20是实际可用的字符数，额外的1是为了存储字符串结束符\0。
// same as NAME_STRING_ARRAY_MAX_LENGTH。说明这个常量的值与另一个可能存在的常量NAME_STRING_ARRAY_MAX_LENGTH相同。
#define _DvnnPara_DEFINED
#endif


/////////////////////////////////////////////////////////////
//  DvnnString                                             //
/////////////////////////////////////////////////////////////
/* 2023-10-09 */
#ifndef _DvnnString_DEFINED
// 用于表示一个具有最大长度限制的字符串。包含一个字符数组用于存储字符串，以及两个整数成员分别用于存储字符串的实际长度和最大长度。

struct DvnnString {

	char string[DVNN_NAME_STRING_ARRAY_MAX_LENGTH];

	int actLength;                         // actLength == 0, iff charList[0] = 0
	// 用于存储字符串的实际长度。注释说明了当actLength为0时，表示字符串数组的第一个元素（charList[0]）是空字符（\0），即字符串为空。

	// maxLength = DVNN_NAME_STRING_ARRAY_MAX_LENGTH - 1;
	int maxLength;
	// 用于存储字符串的最大长度。注释中提到的maxLength = DVNN_NAME_STRING_ARRAY_MAX_LENGTH - 1; 表示最大长度是20，因为数组的最后一个位置保留给空字符。

};
#define _DvnnString_DEFINED
#endif




// Date 2023-10-08
// point data structure : same as MdPoint
#ifndef _DvnnPoint_DEFINED
struct DvnnPoint{
	int valid;         // = 1 : is valid
	                   // = 0 : not valid
					   // =-1 : not assigned, before assigned by gpdb data, 2022-07-05
	double x;
	double y;
	double z;
};
#define _DvnnPoint_DEFINED
#endif



/////////////////////////////////////////////////////////////
//  DvnnRecord                                             //
/////////////////////////////////////////////////////////////
// 动态变量神经网络记录结构体 (用于存储计算过程中的中间状态数据)
// 此数据结构用于记录当前振子（适用于bond,angle,plane）的数据

// Date 2025-01-08
// data record, same as MdRecord
// 数据记录结构，与 MdRecord 结构一致
#ifndef _DvnnRecord_DEFINED

struct DvnnRecord{
	// 运行模式标识
	int    Mode;           // 0 ; init
	                       // 1 : computeHarmonic_get_normal_delta_d_regressive_enforce_constraint
	                       // 2 : computeHarmonic_get_normal_delta_d_syntonic
	// 0 ; 初始值init
	// 1 : 强制拉回computeHarmonic_get_normal_delta_d_regressive_enforce_constraint
	// 2 : 一般震荡computeHarmonic_get_normal_delta_d_syntonic
	// Mode记录当前振子的操作模式，

	// 以下四个值为振子的状态变化值，嵌入到三种数据结构情况：
	// （bond：两原子间距，单位，埃10-10米）
	// （angle：三原子夹角，单位，弧度角）
	// （plane：星型四原子平面角度，单位，弧度角）

	// 一个计算步骤，每个振子的状态变化由两种因素构成，
	// 1来自于振子内部的弹性震荡，（强制拉回和一般震荡）
	// 2来自于振子外部的相互作用，（各个振子之间的力作用）

	// 开始值
	// 初始值 r(n) - 当前迭代周期的起始值
	double Start   ;       // r(n)

	// 内部目标值，内部的弹性震荡的计算预测值
	// 目标值 r(n+½) - 内层迭代的理论目标值（r(n) + Δr(n,内层)）
	double Target  ;       // target : r(n+(1/2)) = r(n) + delta_r(n,inner)

	// 内部实际达到值，内部的弹性震荡的实际计算值
	// 实际到达值 r(n+½) - 内层迭代的实际计算结果
    double Arrive  ;       // actual : r(n+(1/2)) = r(n) + delta_r(n,inner)

	// 最终状态值，外部扰动效果叠加后的最终值
	// 终止值 r(n+1) - 外层迭代的最终结果（r(n+½) + Δr(n,外层)）// 2024-11-12新增
    double Terminal;       // r(n+1) = r(n+(1/2)) + delta_r(n,outer)               // add 2024-11-12
};
#define _DvnnRecord_DEFINED
#endif



/////////////////////////////////////////////////////////////
//  DvnnRecordDihedral                                     //
/////////////////////////////////////////////////////////////
// 这专门用于dihedral的数据记录
// 对于二面角结构，实际上是0~360度旋转，形成两个或者三个Valley（波峰波谷结构），从一个Valley变化到另一个Valley，既完成一次角度相变，这个相变是蛋白质构象变化的关键。
// Start   ; // 开始值
// Target  ; // 内部目标值，内部的弹性震荡的计算预测值
// Arrive  ; // 内部实际达到值，内部的弹性震荡的实际计算值
// Terminal; // 最终状态值，外部扰动效果叠加后的最终值

// Date 2025-01-08
// data record, same as MdRecordDihedral
// 数据记录结构，与 MdRecordDihedral 结构一致
#ifndef _DvnnRecordDihedral_DEFINED
struct DvnnRecordDihedral{
//	能量状态维度
//	Valley系列成员：描述系统在势能曲面上的能量状态变迁
//	采用能谷编号机制跟踪系统在不同能量洼地（energy basin）之间的跃迁过程
	int ValleyStart;     // Valley编号值.起始能谷编号 - 当前迭代周期开始的能量状态标识
	int ValleyTarget;    // 目标能谷编号 - 内层迭代期望达到的能量状态标识
	int ValleyArrive;    // 实际到达能谷 - 内层迭代实际达到的能量状态标识
	int ValleyTerminal;  // 最终能谷编号 - 外层迭代结束后的能量状态标识

//	角度演化维度
//	Phi系列成员：精确记录二面角在迭代过程中的演化路径
//	通过Start->Target->Arrive->Terminal完整呈现角度值的预测-修正过程
	// 起始二面角值 φ(n) - 当前时间步的初始角度值
	double PhiStart   ;       // Phi二面角值（注意这里单位是弧度）r(n)
	// 目标二面角值 φ(n+½) - 内层迭代的理论目标角度（φ(n) + Δφ(n,内层)）
	double PhiTarget  ;       // target : r(n+(1/2)) = r(n) + delta_r(n,inner)
	// 实际到达角度 φ(n+½) - 内层迭代实际计算得到的角度
    double PhiArrive  ;       // actual : r(n+(1/2)) = r(n) + delta_r(n,inner)
	// 最终二面角值 φ(n+1) - 外层迭代后的最终角度（φ(n+½) + Δφ(n,外层)）// 2024-11-12新增
	double PhiTerminal;       // r(n+1) = r(n+(1/2)) + delta_r(n,outer)               // add 2024-11-12

//	时间步对应关系
//	遵循 n → n+½ → n+1 的层级迭代模式
// 	内层迭代（n+½）处理局部约束，外层迭代（n+1）处理全局收敛
};
#define _DvnnRecordDihedral_DEFINED
#endif



/////////////////////////////////////////////////////////////
//  DvnnAtomicVertex                                       //
//  extract from GvnnAtomicVertex                          //
/////////////////////////////////////////////////////////////
/* 2023-10-08 */
#ifndef _DvnnAtomicVertex_DEFINED
// 示一个原子顶点，并且从GvnnAtomicVertex中提取了一些基本信息。包含了一些基本信息，如残基ID、顶点名称序列号、顶点全名ID、顶点全名和原子位置。

struct DvnnAtomicVertex {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // basic informations
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 2023.09.02 same as ResidueID of ForceVertex of dataStructure.h of qomd2022
	int residueID;          // from 0 to N-1
	// 表示残基ID，范围从0到N-1

	// nameSN
	int vertexNameSN;                // serial number : from 0 to 1034, total 
	                                 // not include : R_PRE_0CA0, R_PRE_0C00, R_PRE_0O00, R_SUC_0N00, R_SUC_0CA0, R_SUC_0H00
	// 表示顶点名称的序列号，范围从0到1034，不包括某些特定的值。

	// nameID
	// 2023.09.02 same as NameID of ForceVertex of dataStructure.h of qomd2022
	int vertexNameID;                // full nameID : use NAME_full_name.dict, 
	// 表示顶点的全名ID，使用NAME_full_name.dict字典。

    // full name
	//char vertexName[DVNN_NAME_STRING_ARRAY_MAX_LENGTH];
	DvnnString vertexName;				// 表示顶点的全名。
    //GvnnString vertexName;           // full name : use NAME_full_name.dict, 

	// PARA charge
	double qomdCharge;                       // fixed para : rename 2024-10-05
	double hhmoCharge;                       // adjustable para : add 2024-10-05

	DvnnPoint Position;				// 表示原子的当前位置。注释中提到了另一种可能的类型MdPoint，但实际使用的是DvnnPoint。
	//MdPoint Position;             // current position of this atom
	

};
#define _DvnnAtomicVertex_DEFINED
#endif



/////////////////////////////////////////////////////////////
//  DvnnAtomicVertexList                                   //
/////////////////////////////////////////////////////////////
/* 2023-10-08 */
#ifndef _DvnnAtomicVertexList_DEFINED
// 用于表示一个原子顶点列表。这段代码定义了一个结构体DvnnAtomicVertexList，它包含了一个指向DvnnAtomicVertex结构体的指针、列表中允许的最大项数和当前的实际项数。

struct DvnnAtomicVertexList {

    // list 
    DvnnAtomicVertex * List;
	// 表示顶点列表。

    // max items
    int maxItem;

    // number of items
    int actItem;

};
#define _DvnnAtomicVertexList_DEFINED
#endif




/////////////////////////////////////////////////////////////
//  DvnnForceBond                                          //
//  extract from GvnnForceBond                             //
/////////////////////////////////////////////////////////////
/* 2024-11-07 */
#ifndef _DvnnForceBond_DEFINED
struct DvnnForceBond {

	/*************************************/
	/** fixed data                       */
	/*************************************/
	/*************************************/
	/** 固定数据 (模拟过程中不变)        */
	/*************************************/
	//////  links
	//	原子索引
	// 原子I在顶点列表中的数组索引（DvnnAtomicVertexList中的位置）
 	int VertexI;              // array index of VertexI, index of the data in List of DvnnAtomicVertexList
	// 原子J在顶点列表中的数组索引（DvnnAtomicVertexList中的位置）
	int VertexJ;              // array index of VertexJ, index of the data in List of DvnnAtomicVertexList
	
	//////  fixed parameters
	//	键参数
	// 平衡键长 (r_equilibrium)，势能面谷底对应的键长值
	//	区分 qomdFreq（量子力学）和 hhmoFreq（经典谐振）的模型差异
	double req;               // 两原子距离的平衡值r_equilibrium, bottom of the valley
	// 量子振荡频率 (QM-based)，单位：波数 (1/cm)，用于量子力学计算
	double qomdFreq;          // 旧频谱数据frequency : wave number (1/cm), 2024-10-02
	// 经典谐振频率 (HM-based)，单位：波数 (1/cm)，用于经典谐振模型
	double hhmoFreq;          // 新频谱数据frequency : wave number (1/cm), 2024-10-02

	//////  fixed parameters
	//	偏差约束
	// 最大允许偏差 (固定参数)，键长变化的上限阈值
    double MaxDeviation;      // 最大偏移量 add 2025-01-08, maximum deviation, fixed para
	// 最小允许偏差 (固定参数)，键长变化的下限阈值
    double MinDeviation;      // 最小偏移量 add 2025-01-08, minimum deviation, fixed para

	/*************************************/
	/** 可变数据 (模拟过程中动态更新)    */
	/*************************************/
	// 当前允许的动态偏差范围，可能根据算法阶段调整
	//////  variant parameters
	double LimitDeviation;    // 极限偏移量 add 2025-01-08, current deviation, variant para

	// “最大偏移量”和“最小偏移量”初始设定后不变。
	// 前面提到的满足条件：“最大偏移量”>“极限偏移量”>“最小偏移量”
	// 每个振子的“极限偏移量”在最大和最小之间不断变化的，表示该振子的能量变化。


    // 键状态记录器，跟踪键长在时间步中的变化（Start→Arrive→Terminal）
	//////  variant parameters
	DvnnRecord Record;        // add 2025-01-08, current deviation, variant para

};
#define _DvnnForceBond_DEFINED
#endif



/////////////////////////////////////////////////////////////
//  DvnnForceBondList                                      //
/////////////////////////////////////////////////////////////
/* 2024-11-07 */
#ifndef _DvnnForceBondList_DEFINED
// 存储所有化学键的动态数组
struct DvnnForceBondList {

    // list 
	DvnnForceBond * List;// 指向 DvnnForceBond 数组的指针

	// 容量管理
	// 列表最大容量（预分配空间）
    // max items
    int maxItem;
	// 当前实际存储的键数量
    // number of items
    int actItem;

};
#define _DvnnForceBondList_DEFINED
#endif




/////////////////////////////////////////////////////////////
//  DvnnForceAngle                                         //
//  extract from GvnnForceAngle                            //
/////////////////////////////////////////////////////////////
// 这里的解释类似于bond
/* 2024-11-07 */
#ifndef _DvnnForceAngle_DEFINED
struct DvnnForceAngle {

	/*************************************/
	/** fixed data                       */
	/*************************************/
	/*************************************/
	/** 固定数据 (模拟过程中不变)        */
	/*************************************/
	//////  links
	//	原子索引
	// 键角顶点I在顶点列表中的数组索引（DvnnAtomicVertexList中的位置）
 	int VertexI;              // array index of VertexI, index of the data in List of DvnnAtomicVertexList
	// 键角中心顶点J的数组索引（键角定义为 I-J-K 的夹角）
	int VertexJ;              // array index of VertexJ, index of the data in List of DvnnAtomicVertexList, center point
	// 键角顶点K在顶点列表中的数组索引
	int VertexK;              // array index of VertexK, index of the data in List of DvnnAtomicVertexList
	
	//////  parameters
	//	键角参数
	// 平衡键角 (θ_equilibrium)，势能面谷底对应的角度值，单位：度 (Degree)
	double thetaeq;           // 三原子夹角平衡值，注意这里单位是角度 theta_equilibrium, bottom of the valley, Degree
	// 量子振荡频率 (QM-based)，单位：波数 (1/cm)，用于量子力学模型
	double qomdFreq;          // frequency : wave number (1/cm), 2024-10-02
	// 经典谐振频率 (HM-based)，单位：波数 (1/cm)，用于经典谐振模型
	double hhmoFreq;          // frequency : wave number (1/cm), 2024-10-02

	//////  fixed parameters
//	偏差约束
	// 最大允许偏差 (固定参数)，键角变化的上限阈值（相对于 thetaeq）
    double MaxDeviation;      // add 2025-01-08, maximum deviation, fixed para
	// 最小允许偏差 (固定参数)，键角变化的下限阈值（相对于 thetaeq）
    double MinDeviation;      // add 2025-01-08, minimum deviation, fixed para

	/*************************************/
	/** 可变数据 (模拟过程中动态更新)    */
	/*************************************/
	//////  variant parameters
	// 当前动态允许的偏差范围，可能根据算法阶段调整
	double LimitDeviation;    // add 2025-01-08, current deviation, variant para

	//////  variant parameters
	// 键角状态记录器，跟踪角度在时间步中的变化（Start→Arrive→Terminal）
	// 注意这里单位是弧度
	DvnnRecord Record;        // add 2025-01-08, current deviation, variant para


};
#define _DvnnForceAngle_DEFINED
#endif



/////////////////////////////////////////////////////////////
//  DvnnForceAngleList                                     //
/////////////////////////////////////////////////////////////
/* 2024-11-07 */
#ifndef _DvnnForceAngleList_DEFINED
// 存储所有键角的动态数组
struct DvnnForceAngleList {

    // list
	// 指向 DvnnForceAngle 数组的指针
    DvnnForceAngle * List;

	// 容量管理
    // max items
	// 列表最大容量（预分配空间）
    int maxItem;

	// 当前实际存储的键角数量
    // number of items
    int actItem;

};
#define _DvnnForceAngleList_DEFINED
#endif




/////////////////////////////////////////////////////////////
//  DvnnForceDihedral                                      //
//  extract from GvnnForceDihedral                         //
/////////////////////////////////////////////////////////////
// 这里的解释类似于bond
/* 2024-11-07 */
#ifndef _DvnnForceDihedral_DEFINED
struct DvnnForceDihedral {

	/*************************************/
	/** fixed data                       */
	/*************************************/
	/*************************************/
	/** 固定数据 (模拟过程中不变)        */
	/*************************************/
	//////  links
//	原子索引
	// 二面角顶点I在顶点列表中的数组索引（DvnnAtomicVertexList中的位置）
 	int VertexI;              // array index of VertexI, index of the data in List of DvnnAtomicVertexList
	// 二面角顶点J的数组索引（定义旋转轴 J-K）
	int VertexJ;              // array index of VertexJ, index of the data in List of DvnnAtomicVertexList
	// 二面角顶点K的数组索引（定义旋转轴 J-K）
	int VertexK;              // array index of VertexK, index of the data in List of DvnnAtomicVertexList
	// 二面角顶点L在顶点列表中的数组索引（与I分别位于旋转轴两侧）
	int VertexL;              // array index of VertexL, index of the data in List of DvnnAtomicVertexList
	
	//////  parameters
//	二面角参数
	// 量子振荡频率 (QM-based)，单位：波数 (1/cm)，用于量子力学模型
	double qomdFreq;          // frequency : wave number (1/cm), 2024-10-02
	// 经典谐振频率 (HM-based)，单位：波数 (1/cm)，用于经典谐振模型
	double hhmoFreq;          // frequency : wave number (1/cm), 2024-10-02

	/*************************************/
	/** 可变数据 (模拟过程中动态更新)    */
	/*************************************/
	//////  variant parameters
	// 二面角状态记录器，跟踪能量谷状态（Valley）和扭转角Φ的变化
	DvnnRecordDihedral Record;        // add 2025-01-08, current deviation, variant para

};
#define _DvnnForceDihedral_DEFINED
#endif



/////////////////////////////////////////////////////////////
//  DvnnForceDihedralList                                  //
/////////////////////////////////////////////////////////////
/* 2024-11-07 */
#ifndef _DvnnForceDihedralList_DEFINED
// 存储所有二面角的动态数组
struct DvnnForceDihedralList {
	// 存储所有二面角的动态数组
	DvnnForceDihedral * List; // 指向 DvnnForceDihedral 数组的指针

	// 容量管理
	int maxItem;              // 列表最大容量（预分配空间）
	int actItem;              // 当前实际存储的二面角数量
};
#define _DvnnForceDihedralList_DEFINED
#endif




/////////////////////////////////////////////////////////////
//  DvnnForcePlane                                         //
//  extract from GvnnForcePlane                            //
/////////////////////////////////////////////////////////////
// 这里的解释类似于bond
/* 2024-11-07 */
#ifndef _DvnnForcePlane_DEFINED
struct DvnnForcePlane {

	/*************************************/
	/** fixed data                       */
	/*************************************/
	/*************************************/
	/** 固定数据 (模拟过程中不变)        */
	/*************************************/
	//////  links
// 	int VertexI;              // array index of VertexI, index of the data in List of DvnnAtomicVertexList
//	int VertexJ;              // array index of VertexJ, index of the data in List of DvnnAtomicVertexList
//	int VertexK;              // array index of VertexK, index of the data in List of DvnnAtomicVertexList
//	int VertexL;              // array index of VertexL, index of the data in List of DvnnAtomicVertexList
	//////  原子索引
	int VertexI;              // 平面参考原子I的数组索引（DvnnAtomicVertexList中的位置）
	int VertexJ;              // 平面定义原子J的数组索引（J-K-L 构成基础平面）
	int VertexK;              // 平面定义原子K的数组索引（J-K-L 构成基础平面）
	int VertexL;              // 平面定义原子L的数组索引（J-K-L 构成基础平面）

	//////  parameters
//	double phieq;             // phi_equilibrium, same as gamma2 in GvnnForcePlane;
	                          // Degree : Gamma2 is equal to 180 DEG always.
	//////  平面约束参数
	double phieq;             // 星型四原子平面角度平衡值，注意这里单位是角度（并且固定180度）（平面完全展开状态）
							  // 对应 GvnnForcePlane 中的 gamma2 参数

	// 量子振荡频率 (QM-based)，单位：波数 (1/cm)，用于量子力学模型
	double qomdFreq;          // frequency : wave number (1/cm), 2024-10-02
	// 经典谐振频率 (HM-based)，单位：波数 (1/cm)，用于经典谐振模型
	double hhmoFreq;          // frequency : wave number (1/cm), 2024-10-02

	//////  fixed parameters
//	偏差约束
	// 最大允许平面弯曲角度（相对于 phieq 的正偏差阈值）
    double MaxDeviation;      // add 2025-01-08, maximum deviation, fixed para
	// 最小允许平面弯曲角度（相对于 phieq 的负偏差阈值）
    double MinDeviation;      // add 2025-01-08, minimum deviation, fixed para

	/*************************************/
	/** 可变数据 (模拟过程中动态更新)    */
	/*************************************/
	//////  variant parameters
	// 当前动态允许的弯曲范围，可能根据算法阶段调整
	double LimitDeviation;    // add 2025-01-08, current deviation, variant para

	//////  variant parameters
	// 平面状态记录器，跟踪角度在时间步中的变化（Start→Arrive→Terminal）
	DvnnRecord Record;        // 注意这里单位是弧度 add 2025-01-08, current deviation, variant para
};
#define _DvnnForcePlane_DEFINED
#endif



/////////////////////////////////////////////////////////////
//  DvnnForcePlaneList                                     //
/////////////////////////////////////////////////////////////
/* 2024-11-07 */
#ifndef _DvnnForcePlaneList_DEFINED
struct DvnnForcePlaneList {
	// 存储所有平面约束的动态数组
	DvnnForcePlane * List;    // 指向 DvnnForcePlane 数组的指针

	// 容量管理
	int maxItem;              // 列表最大容量（预分配空间）
	int actItem;              // 当前实际存储的平面约束数量
};
#define _DvnnForcePlaneList_DEFINED
#endif





/////////////////////////////////////////////////////////////
//  DvnnTimePoint                                          //
/////////////////////////////////////////////////////////////
/* 2024-03-03 */
#ifndef _DvnnTimePoint_DEFINED
struct DvnnTimePoint {

	// index
	int vertexIdx;          // index in one protein

	// nameSN
	int vertexNameSN;                // serial number : from 0 to 1034, total 
	                                 // not include : R_PRE_0CA0, R_PRE_0C00, R_PRE_0O00, R_SUC_0N00, R_SUC_0CA0, R_SUC_0H00

    // point data
	int valid;         // = 1 : is valid
	                   // = 0 : not valid
					   // =-1 : not assigned, before assigned by gpdb data, 2022-07-05
	int    t;
	double x;
	double y;
	double z;

};
#define _DvnnTimePoint_DEFINED
#endif



/////////////////////////////////////////////////////////////
//  DvnnTimePointList                                      //
/////////////////////////////////////////////////////////////
/* 2024-03-03 */
#ifndef _DvnnTimePointList_DEFINED
struct DvnnTimePointList {

    // list 
    DvnnTimePoint * List;

    // max items
    int maxItem;

    // number of items
    int actItem;

};
#define _DvnnTimePointList_DEFINED
#endif




/////////////////////////////////////////////////////////////
//  DvnnSlider                                             //
/////////////////////////////////////////////////////////////
/* 2024-10-07 */
#ifndef _DvnnSlider_DEFINED
struct DvnnSlider {
	//滑动条（Slider）的状态

	// 表示滑动条在其最小值和最大值之间的当前状态。
	// DvnnSlider：表示一个滑动条，有最小值、当前值和最大值。
	////////////////////////
	// slider value
	////////////////////////

	double min;	// 滑动条的最小值。
	double act;	// 滑动条的当前值。
	double max;	// 滑动条的最大值。


};
#define _DvnnSlider_DEFINED
#endif


/////////////////////////////////////////////////////////////
//  DvnnSliderList                                         //
/////////////////////////////////////////////////////////////
/* 2024-10-07 */
#ifndef _DvnnSliderList_DEFINED
struct DvnnSliderList {
	// 通过 List 指针，该结构体能够动态管理滑动条的数组，使其大小可以在运行时决定。
	// DvnnSliderList：表示一个滑动条的列表，用于存储和管理多个滑动条，并且可以动态调整滑动条的数量。

	// list 
	DvnnSlider* List;

	// max items
	int maxItem;	// 表示列表中可以容纳的最大滑动条数。

	// number of items
	int actItem;	// 表示列表中当前有效的滑动条数。

};
#define _DvnnSliderList_DEFINED
#endif