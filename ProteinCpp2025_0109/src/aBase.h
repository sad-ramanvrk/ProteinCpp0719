/**
 * @Version 1.0
 * @Date 2021-06-29
 * @Author Zhaohui Meng
 * @Affiliation Department of Computer Science, Hohai University, Nanjing, China.
 */

/*
设定了QOMD中重要的常量和基础参数结构体，详细定义了一个全面的输入参数结构体
*/




///////////////////////////////////////////////////////////////////////////////////
// struct for video name list
///////////////////////////////////////////////////////////////////////////////////

// 2021-06-29
// 预处理器宏#ifndef和#define来确保这段代码只被编译一次，防止重复定义。
#ifndef _QomdParaLength_DEFINED

// capacity parameters
constexpr int FULL_NAME_MAX_LENGTH = 193;        // 130 + 61 + 1 + 1, add '/' & '\0'
// 定义了文件名的最大长度。130（假设的文件名长度）+ 61（路径长度）+ 1（斜杠/）+ 1（字符串结束符\0）。

// EXAMPLE : a long name in hmdb51, length = 153
// "/home/m1/meng/hmdb51/climb_stairs/Increase_stride_length_by_running_stairs__working_step_over_action_speed_development_climb_stairs_f_cm_np1_le_med_1.avi"

#define _QomdParaLength_DEFINED
#endif




 /* 2021-06-29 */
#ifndef _QomdBasePara_DEFINED
// 存储程序运行相关参数的结构
struct QomdBasePara {
	char para_DATA_DIR[51];			// 用于存储数据目录的路径。数组长度为51，可以存储最多50个字符加上一个空字符（\0）作为字符串结束标志。

	// 用于存储运行程序的相关参数。
	int para_RUN_PROCEDURE;			// para_RUN_PROCEDURE 表示将要运行的程序的编号，对应qomdProtein2024中的24102、24103、24104

	int para_WAIT_KEY;				// 目前程序没用到

	// 这四行定义了四个整型变量，分别用于存储CUDA并行计算的网格维度和块维度。这些参数通常用于配置CUDA内核的执行配置。
	int para_CUDA_GRID_DIM_X;		// 网格维度（para_CUDA_GRID_DIM_X和para_CUDA_GRID_DIM_Y）
	int para_CUDA_GRID_DIM_Y;

	int para_CUDA_BLOCK_DIM_X;		// 块维度（para_CUDA_BLOCK_DIM_X和para_CUDA_BLOCK_DIM_Y）
	int para_CUDA_BLOCK_DIM_Y;

	//int CUDA_GRID_DIM_X_1D;
	//int CUDA_BLOCK_DIM_X_1D;

};
#define _QomdBasePara_DEFINED
#endif




 /* 2024-12-16 */
 // not used 
#ifndef _EvaluatePara1_DEFINED
// 存储基础评估参数，参数较简单，未使用
struct EvaluatePara1 {
	double LimitDeviationLevel;  // 允许的最大偏差水平
	double Mode_1_Level;         // 模式1的触发阈值
};
#define _EvaluatePara1_DEFINED
#endif




 /* 2024-12-27 */
#ifndef _EvaluatePara_DEFINED
// 系统评估相关的参数
struct EvaluatePara {

    //////////////////////////////////////////////////////////////////
	// temprature
    //////////////////////////////////////////////////////////////////

	// 温度相关参数
	double t_DeltaR;         // 温度变化量（总）[总温度]
	// 总温度，应该控制在实验室温度300k左右（摄氏27度），或者生物体温度308k左右（摄氏35度）

	double t_DeltaRElcCol;   // 与静电/碰撞相关的温度变化
	// 由ElcCol计算产生的温度

	double t_DeltaRCouVdw;   // 与库仑力/范德华力相关的温度变化
	// 由CouVdw计算产生的温度
	
    //////////////////////////////////////////////////////////////////
	// Bond
    //////////////////////////////////////////////////////////////////

	// 化学键（Bond）相关参数
	double LimitDeviationLevel_Bond;  	// 键偏差阈值
	// 前面提到条件公式：“最大偏移量”>“极限偏移量”>“最小偏移量”，每个振子的“极限偏移量”是不断变化的，
	// 为了综合掌握这个信息，计算一个实时评估参数“极限偏移量平均水平LimitDeviationLevel_Bond”，
	// 将所有的Bond振子的“极限偏移量”在最大最小值之间的平均位置统计一下

	double Mode_1_Level_Bond;         	// 键模式1阈值
	// 所有的Bond振子中，执行“强制回位操作”的统计比例。每一步的振子位移计算方法分为两种：“强制回位操作”和“一般震荡操作”

	// 对于每一个Bond振子，完成一步计算后，其当前的实际状态（两原子距离）与平衡位置（固定值，来源于统计）的比值，
	// 记为ratio_terminal_Bond，然后统计所有的Bond振子，
	// 找到最大的（Max_ratio_terminal_Bond），并记下其Bond振子编号（Max_ratio_terminal_Bond_idx），
	// 找到最小的（Min_ratio_terminal_Bond），也记下其Bond振子编号（Min_ratio_terminal_Bond_idx）
	double Max_ratio_terminal_Bond;   	// 键终端比例最大值
	int    Max_ratio_terminal_Bond_idx; // 最大值对应的索引
	double Min_ratio_terminal_Bond;   	// 键终端比例最小值
	int    Min_ratio_terminal_Bond_idx; // 最小值对应的索引

    //////////////////////////////////////////////////////////////////
	// Angle
    //////////////////////////////////////////////////////////////////

	// 键角（Angle）相关参数（与Bond类似）
	double LimitDeviationLevel_Angle;
	double Mode_1_Level_Angle;
	double Max_ratio_terminal_Angle;
	int    Max_ratio_terminal_Angle_idx;
	double Min_ratio_terminal_Angle;
	int    Min_ratio_terminal_Angle_idx;

    //////////////////////////////////////////////////////////////////
	// Plane
    //////////////////////////////////////////////////////////////////

	// 平面（Plane）相关参数（与Bond类似）
	double LimitDeviationLevel_Plane;
	double Mode_1_Level_Plane;
	double Max_ratio_terminal_Plane;
	int    Max_ratio_terminal_Plane_idx;
	double Min_ratio_terminal_Plane;
	int    Min_ratio_terminal_Plane_idx;

    //////////////////////////////////////////////////////////////////
	// ElcCol
    //////////////////////////////////////////////////////////////////

	// 静电/碰撞（ElcCol）统计
	// ElcCol三种操作（COL1、COL2、ELC）的数量统计
	int Num_elccol_move_ALL;   // 所有移动类型计数
	int Num_elccol_move_NONE;  // 无移动类型计数
	int Num_elccol_move_COL1;  // 碰撞类型1计数
	int Num_elccol_move_COL2;  // 碰撞类型2计数
	int Num_elccol_move_ELC;   // 静电类型计数
	int Num_elccol_move_OTHE;  // 其他类型计数

    //////////////////////////////////////////////////////////////////
	// Dihedral
    //////////////////////////////////////////////////////////////////

	// 二面角（Dihedral）统计
	// “dihedral相变”的数量统计
	int Num_Dihedral_ALL;      // 所有二面角数量
	int Num_valley_change;     // 二面角谷值变化次数
};
#define _EvaluatePara_DEFINED
#endif





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     qomd run para
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 2023-09-09 
// copy from /home/m1/qomd2022/src/dataStructure.h in qomd2022
#ifndef _InputPara_DEFINED
// 用于控制蛋白质模拟的各类输入参数，对应cppCuda_run_para_linux_2024.txt 里的内容
struct InputPara{

    char PROTEIN_NAME[10];             // 1VII, 2DX2, etc.
	// 蛋白质名称，例如 "1VII", "2DX2" 等，最多包含 10 个字符（包括终止符 '\0'）

	char GPU_DEVICE_NAME[10];          // 970, c1060, 2080I
	// GPU 设备名称，例如 "970", "c1060", "2080I"，最多存储 10 个字符

	char DATA_DIR[51];
	// 数据目录路径，最多存储 51 个字符（包括 '\0'）

    char dir_dictionary[FULL_NAME_MAX_LENGTH];               // add 2023-10-01
	// Dictionary directory path, FULL_NAME_MAX_LENGTH is a macro constant representing the maximum length of the path, added on 2023-10-01
	// 字典目录路径，FULL_NAME_MAX_LENGTH 是宏定义的常量，表示路径的最大长度，添加于 2023-10-01
    char dir_home_qomd_protein[FULL_NAME_MAX_LENGTH];        // add 2023-10-01
	// QOMD protein directory path, added on 2023-10-01
	// QOMD 蛋白质目录的路径，添加于 2023-10-01

	// 输入文件名相关字段，暂时注释掉，未使用
	//char INPUTFILE_vertex  [51];
	//char INPUTFILE_bond    [51];
	//char INPUTFILE_angle   [51];
	//char INPUTFILE_plane   [51];
	//char INPUTFILE_harmonic[51];
	//char INPUTFILE_couvdw  [51];
	//char INPUTFILE_dihedral[51];

	// 输出文件名相关字段，暂时注释掉，未使用
	//char OUTPUTFILE_run_ser[51];
	//char OUTPUTFILE_run_par[51];

	int PARA_ELEC;          // 1 = use qomd(old fixed para), 2 = use hhmo(new adjustable para), add 2024-10-70
	// 频率计算参数选择，1 = 使用 QOMD 旧固定参数，2 = 使用 HHMO 新可调参数，添加于 2024-10-70

	int PARA_FREQ;          // 1 = use qomd(old fixed para), 2 = use hhmo(new adjustable para), add 2024-10-70
	// 频率计算参数选择，1 = 使用 QOMD 旧固定参数，2 = 使用 HHMO 新可调参数，添加于 2024-10-70

	int RUN_START;          // 1 = START or 0 = CONTINUE
	// 运行开始模式，1 = START（开始新的运行），0 = CONTINUE（继续先前的运行）


	int RUN_MODE;           // 1 = SER or 2 = PAR or 3 = CUDA
	// 运行模式选择，1 = 串行（SER），2 = 并行（PAR），3 = 使用 CUDA（GPU 加速）


	int RUN_METHOD;         // 1 = COUVDW or 2 = ELCCOL    // add 2023-10-02
	// 运行方法选择，1 = 使用 COUVDW，2 = 使用 ELCCOL，添加于 2023-10-02
	// 1：使用 COUVDW 库伦力范德华力模型
	// 2：使用 ELCCOL 电荷力碰撞力模型
	// GPU 设备数量选择，1 = 使用一个 GPU 设备，2 = 使用两个 GPU 设备

	int RUN_GPU;            // 1 : use one device, 2 : use two devices
	// GPU 设备数量选择，1 = 使用一个 GPU 设备，2 = 使用两个 GPU 设备

	int RUN_ROUNDS;         // one round for 1 fs compute in QOMD, int is 4 bytes in windows, same as long, (-2147483648, +2147483647)
	                        // RUN_ROUNDS = form 1 to 2147483647 , limited by <int> type 
	// 运行轮数，每轮可能对应于 1 fs 的模拟计算，取值范围是 1 到 2147483647（正整数）

	int WRITE_FREQ;         // write run-round and temperature to console and output file
	// 控制每隔多少轮次将当前的运行轮次和温度写到控制台和输出文件

	int WRITE_MEDIUM_FREQ;  // write medium data files and pdb file, to protect run data when power off for servers.
	// 写入中间数据文件和 PDB 文件的频率，以便在服务器断电的情况下保护运行数据

	// 与分子结构模拟中的几何约束相关的参数，用于控制键长、键角、平面和二面角的允许偏差范围。
	double BOND_DEVI_RATIO;  // Bond length deviation ratio, default value is 0.0435
	// 键长偏差的比例，默认值为 0.0435
	double ANGLE_DEVI_RATIO;  // Bond angle deviation ratio, default value is 0.0435
	// 键角偏差的比例，默认值为 0.0435
	double PLANE_DEVI_RATIO;  // Plane deviation ratio, default value is 0.0435
	// 分子平面偏差的比例，默认值为 0.0435
	double DIHE_LIMIT_RATIO;  // Dihedral angle limit deviation ratio, default value is 0.0435
	// 二面角偏差的限制比例，默认值为 0.0435
	double DIHE_FLATTOP_RATIO;  // Dihedral angle flattop deviation ratio, default value is 0.9
	// 二面角平顶偏差的比例，默认值为 0.9

	// 增强约束参数（2024-12-12新增，默认0.3）
	// 键长约束强化系数
	double BOND_ENFORCE_RATIO ;  // bond NORMAL_INNER_REGRESSIVE_ENFORCE_RATIO = 0.3;    add 2024-12-12
	// 键角约束强化系数
	double ANGLE_ENFORCE_RATIO;  // angle NORMAL_INNER_REGRESSIVE_ENFORCE_RATIO = 0.3;   add 2024-12-12
	// 平面约束强化系数
	double PLANE_ENFORCE_RATIO;  // plane NORMAL_INNER_REGRESSIVE_ENFORCE_RATIO = 0.3;   add 2024-12-12

	// 偏差修正参数（2024-12-12新增，默认0.5）
	// 键长偏差修正速率
	double BOND_DEVIMODI_RATE ;  // bond LIMIT_DEVIATION_MODIFY_RATE_BOND = 0.5;     add 2024-12-12
	// 键角偏差修正速率
	double ANGLE_DEVIMODI_RATE;  // angle LIMIT_DEVIATION_MODIFY_RATE_ANGLE = 0.5;   add 2024-12-12
	// 平面偏差修正速率
	double PLANE_DEVIMODI_RATE;  // plane LIMIT_DEVIATION_MODIFY_RATE_PLANE = 0.5;   add 2024-12-12

    // ELCCOL模型参数（2024-12-23新增）
    double RATIO_ELCCOL_R_WALL;  // 碰撞半径比例（默认0.7）
    double RATIO_ELCCOL_COL1;    // 一级碰撞响应比例（默认0.5）
    double RATIO_ELCCOL_COL2;    // 二级碰撞响应比例（默认0.3）
    double RATIO_ELCCOL_ELC;     // 电荷相互作用比例（默认0.05）

};
#define _InputPara_DEFINED
#endif