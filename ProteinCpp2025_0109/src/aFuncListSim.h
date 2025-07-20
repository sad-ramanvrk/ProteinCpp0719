/**
 * @Version 1.0
 * @Date 2023-10-03
 * @Author Zhaohui Meng
 * @Affiliation Department of Computer Science, Hohai University, Nanjing, China.
 */



// c++
#include <stdio.h>

// base
#include "aBase.h"

// dvnn
#include "aDvnn.h"




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions in dvnnFunc.cpp
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 用于处理与顶点和时间点相关的数据结构，并提供了文件的读写功能。

// 生成0到1之间的新随机实数
double dvnn_getNew_Real_0_to_1();

// 初始化DvnnSliderList并分配内存
DvnnSliderList* dvnn_init_dvnnSliderList_malloc();

// 使用随机值重置DvnnSliderList
void dvnn_reset_dvnnSliderList_by_random(DvnnSliderList* sliderList);

// 创建空DvnnString对象
DvnnString dvnnString_new_null_string();

// 初始化顶点列表（原子顶点）
DvnnAtomicVertexList* dvnn_init_malloc_vertexList(void* proteinBag);

// 初始化键合力列表
DvnnForceBondList* dvnn_init_malloc_dvnnBondList(void* proteinBag);

// 初始化角度力列表
DvnnForceAngleList* dvnn_init_malloc_dvnnAngleList(void* proteinBag);

// 初始化二面角力列表
DvnnForceDihedralList* dvnn_init_malloc_dvnnDihedralList(void* proteinBag);

// 初始化平面力列表
DvnnForcePlaneList* dvnn_init_malloc_dvnnPlaneList(void* proteinBag);

// 将顶点列表写入测试文件
void dvnn_write_dvnnVertexList_to_test_file(DvnnAtomicVertexList* vertexList, char* fname_writerecord);

// 将所有力列表写入测试文件
void dvnn_write_dvnnAllList_to_test_file(
	DvnnAtomicVertexList* dvnnVertexList,
	DvnnForceBondList* dvnnBondList,
	DvnnForceAngleList* dvnnAngleList,
	DvnnForceDihedralList* dvnnDihedralList,
	DvnnForcePlaneList* dvnnPlaneList,
	char* fname_writerecord);

// 初始化时间点列表并分配内存
DvnnTimePointList* dvnn_init_timePointList_malloc(void* atomicVertexList);

// 将时间点参数数据保存到二进制文件
void dvnn_save_timePointList_paraData_to_byte_file(char* fname_output_TIME_POINT_byte, int totalStamps, int totalVeretx);

// 将时间点列表数据保存到二进制文件
void dvnn_save_timePointList_listData_to_byte_file(char* fname_output_TIME_POINT_byte, DvnnTimePointList* timePointList);

// 将顶点数据复制到指定时间戳的时间点列表
void dvnn_copy_vertexList_to_timePointList_by_timeStamp(DvnnTimePointList* timePointList, DvnnAtomicVertexList* vertexList, int timeStamp);

// 从二进制文件加载时间点列表（已注释旧版本）
// DvnnTimePointList* dvnn_get_timePointList_from_byte_file(char* fname_input_TIME_POINT_byte, int total_stamps, int total_veretx);
DvnnTimePointList* dvnn_get_timePointList_from_byte_file(char* fname_input_TIME_POINT_byte);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions in stringFunc.cpp
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 已弃用：带右截断的字符串复制
void qomd_rtrim_copy_backup(char* str_src, char* str_dst);    // deprecated

// 带右截断的字符串复制（去除尾部空格）
void qomd_strcopy_rtrim(char* str_dst, char* str_src);

// 无截断的字符串复制（保留尾部空格）
void qomd_strcopy_notrim(char* str_dst, char* str_src);

// 无截断且转小写的字符串复制
void qomd_strcopy_notrim_upper_to_lower(char* str_dst, char* str_src);

// 计算字符串长度
int qomd_strlen(char* str);

// 合并两个字符串到新字符串
void qomd_strmerge_2in1(char* str_new, char* str1, char* str2);

// 合并三个字符串到新字符串
void qomd_strmerge_3in1(char* str_new, char* str1, char* str2, char* str3);

// 合并四个字符串到新字符串
void qomd_strmerge_4in1(char* str_new, char* str1, char* str2, char* str3, char* str4);

// 提取子字符串
int qomd_fetch_substr(char* host_str, char* sub_str, int start, int end);

// 字符串比较
int qomd_strcmp(char* str1, char* str2);

// 合并两个字符串到新字符串（旧版函数）
void qomd_strmerge(char* str1, char* str2, char* str_new);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions in readFile.cpp
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 读取运行参数配置文件
void read_RunPara(char* fname_readin, QomdBasePara* basePara);

// 读取QOMD运行参数
void read_qomd_RunPara(char* fname_readin, InputPara* inputRunPara);

// 从配置文件获取QOMD运行参数对象
InputPara* get_qomd_RunPara_from_configFile(char* fname_readin); //2024-07-10

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions in qomdShell1.cpp
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 从proteinBag复制顶点位置数据到顶点列表
void qomd_shell1_copy_vertexList_by_proteinBag_position_data(DvnnAtomicVertexList* vertexList, void* proteinBag_vpic);

// 从proteinBag获取输入参数
InputPara* qomd_shell1_get_InputPara_from_ProteinPackage(void* proteinBag_vpic);

// 通过DeltaR计算顶点温度
double qomd_shell1_computeVertex_temperature_by_DeltaR(void* proteinBag_vpic);

// 序列化执行CouVdw计算（飞秒迭代）
void qomd_shell1_femto_iterator_ser_use_CouVdw(void* proteinBag_vpic);

// 并行执行CouVdw计算（飞秒迭代）
void qomd_shell1_femto_iterator_per_use_CouVdw(void* proteinBag_vpic);

// 序列化执行ElcCol计算（飞秒迭代）
void qomd_shell1_femto_iterator_ser_use_ElcCol(void* proteinBag_vpic);

// 并行执行ElcCol计算（飞秒迭代）
void qomd_shell1_femto_iterator_per_use_ElcCol(void* proteinBag_vpic);

// 从八面体字典文件加载数据
void* qomd_shell1_get_octa_dic_data_list_from_octa_dict_file(char* fname_input_QOMD_DICT_OCTA_byte);

// 从六面体字典文件加载数据
void* qomd_shell1_get_hexa_dic_data_list_from_hexa_dict_file(char* fname_input_QOMD_DICT_HEXA_byte);

// 从GVNN字典文件加载谐波数据
void* qomd_shell1_get_dicHarmonicList_hhmo_freq_from_gvnn_dict_file(char* fname_readin_GVNN_DICT_hhmoHarmonic);

// 构建1VII蛋白质包（示例）
void* qomd_shell1_get_ProteinPackage_1VII_rod(void* octaDicDataList, char* dir_home_qomd_protein, char* dir_dictionary);

// 通过GPDB文件构建蛋白质包
void* qomd_shell1_get_ProteinPackage_by_gpdbFile(
	void* octaDicDataList,
	void* qomdDicDataList,
	void* hhmoPanel,
	char* dir_home_qomd_protein,
	char* dir_dictionary,
	char* gpdb_file_full_name);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions in qomdShell3.cpp
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 获取顶点列表当前活动项数
int qomd_shell3_get_vertexList_actItem_by_proteinBag(void* proteinBag_vpic);

// 获取顶点列表最大项数
int qomd_shell3_get_vertexList_maxItem_by_proteinBag(void* proteinBag_vpic);

// 获取键合力列表当前活动项数
int qomd_shell3_get_dvnnBondList_actItem_by_proteinBag(void* proteinBag_vpic);

// 获取键合力列表最大项数
int qomd_shell3_get_dvnnBondList_maxItem_by_proteinBag(void* proteinBag_vpic);

// 获取角度力列表当前活动项数
int qomd_shell3_get_dvnnAngleList_actItem_by_proteinBag(void* proteinBag_vpic);

// 获取角度力列表最大项数
int qomd_shell3_get_dvnnAngleList_maxItem_by_proteinBag(void* proteinBag_vpic);

// 获取二面角力列表当前活动项数
int qomd_shell3_get_dvnnDihedralList_actItem_by_proteinBag(void* proteinBag_vpic);

// 获取二面角力列表最大项数
int qomd_shell3_get_dvnnDihedralList_maxItem_by_proteinBag(void* proteinBag_vpic);

// 获取平面力列表当前活动项数
int qomd_shell3_get_dvnnPlaneList_actItem_by_proteinBag(void* proteinBag_vpic);

// 获取平面力列表最大项数
int qomd_shell3_get_dvnnPlaneList_maxItem_by_proteinBag(void* proteinBag_vpic);

// 从proteinBag复制顶点数据到顶点列表
void qomd_shell3_copy_data_from_proteinBag_dvnnVertexList(DvnnAtomicVertexList* vertexList, void* proteinBag_vpic);

// 从proteinBag复制键合力数据
void qomd_shell3_copy_data_from_proteinBag_dvnnBondList(DvnnForceBondList* dvnnBondList, void* proteinBag_vpic);

// 从proteinBag复制角度力数据
void qomd_shell3_copy_data_from_proteinBag_dvnnAngleList(DvnnForceAngleList* dvnnAngleList, void* proteinBag_vpic);

// 从proteinBag复制二面角力数据
void qomd_shell3_copy_data_from_proteinBag_dvnnDihedralList(DvnnForceDihedralList* dvnnDihedralList, void* proteinBag_vpic);

// 从proteinBag复制平面力数据
void qomd_shell3_copy_data_from_proteinBag_dvnnPlaneList(DvnnForcePlaneList* dvnnPlaneList, void* proteinBag_vpic);

// 从proteinBag复制当前顶点数据（增量更新）
void qomd_shell3_copy_current_from_proteinBag_dvnnVertexList(DvnnAtomicVertexList* vertexList, void* proteinBag_vpic);

// 将顶点列表写入PDB格式文件
void qomd_shell3_write_dvnnVertexList_to_pdb_file(DvnnAtomicVertexList* vertexList, char* fname_writerecord);

// 从二进制文件加载蛋白质包
void* qomd_shell3_get_from_byte_file_for_ProteinPackage(char* fname_input_byte);

// 从二进制文件加载HhmoPanel
void* qomd_shell3_get_from_byte_file_for_HhmoPanel(char* fname_input_byte);

// 通过滑动条调整HhmoPanel参数
void qomd_shell3_adjust_hhmoPanel_by_dvnnSliderList(void* hhmoPanel_vpic, void* dvnnSliderList_vpic);

// 通过HhmoPanel调整蛋白质包参数
void qomd_shell3_adjust_ProteinPackage_hhmo_para_by_HhmoPanel(void* proteinBag_vpic, void* hhmoPanel_vpic);

// 切换蛋白质包的振荡谐波模式（HHMO）
void qomd_shell3_switch_ProteinPackage_Osci_Harmonic_by_HHMO(void* proteinBag_vpic);

// 切换蛋白质包的振荡谐波模式（QOMD）
void qomd_shell3_switch_ProteinPackage_Osci_Harmonic_by_QOMD(void* proteinBag_vpic);

// 切换电荷计算模式（HHMO电荷）
void qomd_shell3_switch_ProteinPackage_elccol_charge_by_vertex_hhmoCharge(void* proteinBag_vpic);

// 切换电荷计算模式（QOMD电荷）
void qomd_shell3_switch_ProteinPackage_elccol_charge_by_vertex_qomdCharge(void* proteinBag_vpic);

// 显示蛋白质包参数
void qomd_shell3_display_ProteinPackage_para(void* proteinBag_vpic);

// 初始化蛋白质包指针验证
void qomd_shell3_init_ProteinPackage_pointer_revise(void* proteinBag_vpic, char* pointer_verify_file_name);

// 初始化QOMD运行参数
void qomd_shell3_init_ProteinPackage_qomdRunPara(void* proteinBag_vpic, InputPara* qomdRunPara);

// 初始化CUDA网格块配置
void qomd_shell3_init_ProteinPackage_CudaGridBlock(void* proteinBag_vpic);

// 初始化振荡器正常状态限制
void qomd_shell3_initOsci_normalState_limitDeviation(void* proteinBag_vpic);

// 设置DeltaR索引
void qomd_shell3_init_osci_set_DeltaR_index_All(void* proteinBag_vpic);

// 重置所有DeltaR为零
void qomd_shell3_init_osci_set_DeltaR_to_zero_All(void* proteinBag_vpic);

// 通过全局点重置顶点位置
void qomd_shell3_computeVertex_reset_Position_by_globalPoint(void* proteinBag_vpic);

// 从配置文件获取QOMD运行参数
InputPara* qomd_shell3_get_qomd_RunPara_from_configFile(char* fname_readin);

// 序列化执行CouVdw计算（飞秒迭代）
void qomd_shell3_femto_iterator_ser_use_CouVdw(void* proteinBag_vpic);

// 并行执行CouVdw计算（飞秒迭代）
void qomd_shell3_femto_iterator_per_use_CouVdw(void* proteinBag_vpic);

void qomd_shell3_femto_iterator_ser_use_ElcCol(void* proteinBag_vpic);

void qomd_shell3_femto_iterator_per_use_ElcCol(void* proteinBag_vpic);

// 并行执行振荡器计算（飞秒迭代）
void qomd_shell3_femto_iterator_per_use_4_osci(void* proteinBag_vpic);

// 通过DeltaR计算温度
void qomd_shell3_computeVertex_temperature_by_DeltaR(void* proteinBag_vpic, EvaluatePara* qomdEvaluatePara);

// 通过DeltaR和ElcCol计算温度
void qomd_shell3_computeVertex_temperature_by_DeltaRElcCol(void* proteinBag_vpic, EvaluatePara* qomdEvaluatePara);

// 通过DeltaR和CouVdw计算温度
void qomd_shell3_computeVertex_temperature_by_DeltaRCouVdw(void* proteinBag_vpic, EvaluatePara* qomdEvaluatePara);

// 评估键合力列表
void qomd_shell3_computeEvaluate_BondList(void* proteinBag_vpic, EvaluatePara* qomdEvaluatePara);

// 评估角度力列表
void qomd_shell3_computeEvaluate_AngleList(void* proteinBag_vpic, EvaluatePara* qomdEvaluatePara);

// 评估平面力列表
void qomd_shell3_computeEvaluate_PlaneList(void* proteinBag_vpic, EvaluatePara* qomdEvaluatePara);

// 评估二面角力列表
void qomd_shell3_computeEvaluate_DihedralList(void* proteinBag_vpic, EvaluatePara* qomdEvaluatePara);

// 评估电荷碰撞列表
void qomd_shell3_computeEvaluate_ElcColList(void* proteinBag_vpic, EvaluatePara* qomdEvaluatePara);

// 计算两个时间点的RMSD（结构差异）
double qomd_shell3_compute_rmsd_by_timePointList(
    DvnnTimePointList* timePointList,
    int timeA,
    int timeB,
    int maxAtomNum,
    double rmsd_covMatNormL2_EPSILON,
    double rmsd_svd_EPSILON,
    int rmsd_svd_ROUND);

// 输出蛋白质包的10个结果文件
void qomd_shell3_write_proteinBag_output_10_files(void* proteinBag_vpic);

// 从proteinBag拷贝最终的键、角、二面角、平面列表的当前数据。
void qomd_shell3_copy_current_from_proteinBag_dvnnBondList(DvnnForceBondList*, void* );
void qomd_shell3_copy_current_from_proteinBag_dvnnAngleList(DvnnForceAngleList*, void* );
void qomd_shell3_copy_current_from_proteinBag_dvnnDihedralList(DvnnForceDihedralList*, void* );
void qomd_shell3_copy_current_from_proteinBag_dvnnPlaneList(DvnnForcePlaneList*, void* );