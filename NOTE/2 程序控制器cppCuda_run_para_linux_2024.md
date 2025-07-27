# 2 程序控制器cppCuda_run_para_linux_2024.txt

| 程序  | 说明                                                         | 备注                                                         |
| ----- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 101   | 测试                                                         | 已经废弃                                                     |
| 24102 | 整套蛋白质模拟计算过程，具体步骤包括读取二进制文件初始化数据，设置相关参数，执行迭代计算，并输出模拟结果 |                                                              |
| 24103 | 读取点云数据并显示                                           |                                                              |
| 24104 | 计算两个指定时间点之间的均方根偏差（RMSD），目前设置的`timeA=0;timeB=900 | 在运行24104之前要先运行24102                                 |
| 24105 | 1VII 的PDB蛋白质数据，模拟运动                               |                                                              |
| 24106 | 用于处理蛋白 4UZX                                            |                                                              |
| 24107 | 用于处理蛋白 2RRK                                            |                                                              |
| 24115 | 1VII 从长杆模型开始（长链状态），模拟折叠过程<br />处理蛋白 1VII_rod | 长杆模型的数据是手工设置扭转角，然后反向计算出立体结构<br />立体结构三维数据，与扭转角是互相确定的 |

## 文件概述

**文件名**: `QOMD_run_para_linux_2024.txt`

**作用**: 控制QOMD主程序的执行流程选择

**位置**: 项目根目录

## 文件结构

该文件通过 `RUN_PROCEDURE` 参数控制程序执行的具体流程。程序会从上到下读取此文件，并使用它遇到的第一个未被注释的 `RUN_PROCEDURE` 块。

### 101-基础程序测试

```
RUN_PROCEDURE                |          101       |  基础连通性测试
END
```

**功能说明**:

-   最基本的程序连通性测试
-   验证程序可以正常启动并读取参数
-   适用于系统部署验证

## 24102-核心

处理蛋白质结构，读取数据文件、初始化结构、执行计算并输出结果。

先将`dvnnSliderList`列表中的每个`DvnnSlider`的当前值（`act`）随机化为一个在 [-1, +1] 之间的值，模拟网络中神经元参数的初始状态。

然后利用`dvnnSliderList`去调整`hhmoPanel`，然后根据`hhmoPanel`的数据去调整参数`hhmo_para`。

```bash
void procedure_24102(FILE* fptr_writerecord_test_output)简介

PROCEDURE_STEP : proteinBag_1VII get from byte file <proteinBag_1VII_linux.byte>
void * proteinBag_1VII是蛋白质1VII的模拟数据包的头地址，所有的运行数据都在这里面，其中的主要数据部分从文件<proteinBag_1VII_linux.byte>中读取。

PROCEDURE_STEP : get qomdRunPara
运行参数InputPara * qomdRunPara从文件"cppCuda_run_para_linux_2024.txt"中读取。

PROCEDURE_STEP : reset qomdRunPara in proteinBag
运行参数InputPara * qomdRunPara送进蛋白质1VII的模拟数据包proteinBag_1VII。

PROCEDURE_STEP : reset CudaGridBlock
设置GPU运行参数，暂时不用，保留模块。

PROCEDURE_STEP : init osci data before run
在运行模拟器前，还要根据运行参数qomdRunPara初始化一些必要的数据，都在模拟数据包proteinBag_1VII里面。

PROCEDURE_STEP : init dvnn vertexList
DvnnAtomicVertexList* vertexList是一个独立于模拟数据包proteinBag_1VII的瞬时（当前时刻）原子数据包，
可以随时从模拟数据包proteinBag_1VII中copy数据出来，DvnnAtomicVertexList数据结构是可见的，可以在此基础上进一步写代码。

PROCEDURE_STEP : init dvnn timePointList
DvnnTimePointList* timePointList是另一个独立的动态点云（时间变化的）原子数据包，
从DvnnAtomicVertexList* vertexList数据包中copy数据出来。
通过函数dvnn_save_timePointList_listData_to_byte_file将数据输出到字节文件"time_point_linux.byte"里。
该字节文件的读写方法都代码可见。

PROCEDURE_STEP : qomd_femto_iterator set run method
模拟器可以有四种运行方案，由运行参数InputPara * qomdRunPara确定。

PROCEDURE_STEP : iterate
模拟器运行模块，循环运行，并且每一步都输出数据到vertexList和timePointList，并且写数据到文件"time_point_linux.byte"里。

PROCEDURE_STEP : output dvnn vertexList
结束模拟并输出数据到文件"dvnn_vertexList_2.txt"和"dvnn_vertexList_pdb_2.txt"。
```

```bash
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCEDURE_STEP : proteinBag_1VII get from byte file <proteinBag_1VII_linux.byte>                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

1 将数据文件 "proteinBag_1VII_linux.byte" 或者 "proteinBag_1VII_win.byte" 中的数据读入数据包 void * proteinBag_1VII
  proteinBag_1VII 是蛋白质模拟运行数据包的头地址，具体结构不可见，但是有函数接口用于修改其中的参数和读出数据


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCEDURE_STEP : init dvnn sliderList                                                                                           //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

2 初始化一个参数调整数据块，DvnnSliderList 结构可见，是一个1600个滑块按钮 DvnnSlider，结构可见，可编程控制。
  每个DvnnSlider包含三个值，min=-1,act=0,max=+1,其中min,max固定不变，act在[-1,+1]之间变化调整。
  每个滑块按钮 DvnnSlider 相当于是一个规范化的单参数调节器。
  这个就是同学们自己设计的神经网络的调整对象，通过对这些参数的调整，改变蛋白质模拟器的运行效果。

  dvnn_reset_dvnnSliderList_by_random是一个例子，将1600个滑块act初始化为[-1,+1]之间的随机值。


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCEDURE_STEP : get hhmoPanel from byte file                                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

3 将数据文件 "hhmoPanel_linux.byte" 或者 "hhmoPanel_win.byte" 中的数据读入数据包 void * hhmoPanel
  hhmoPanel 是参数调整操控板的头地址，具体结构不可见，修改其中的参数的途径是通过DvnnSliderList结构实现的。

  qomd_shell3_adjust_hhmoPanel_by_dvnnSliderList 函数用于将 DvnnSliderList 的数据转录给 hhmoPanel。

  hhmoPanel 中的参数结构比 DvnnSliderList 中的参数要复杂，但是主体也是1000多个[min,act,max]型的单参数调节器。
  hhmoPanel 中的参数编号与 DvnnSliderList 中的参数编号一致，但是不是都用到了，有些是空置保留。
  具体：  0~1040，电荷参数
       1101~1113，频谱参数
       1201~1213，频谱参数
       1301~1313，频谱参数
       1401~1413，频谱参数
  这些参数是实际使用的。也就是说，DvnnSliderList 结构中的参数，就修改这些，其他的没有作用。

  hhmoPanel 参数调整操控板中的基准参数是在文件 "hhmoPanel_linux.byte" 中预置好的，
  然后再根据参数调整数据块 DvnnSliderList 中的参数修订，获得实际运行的参数，
  也就是说，只要保存新的 DvnnSliderList 数据，就可以保留前次运行训练的成果，下次再接着用。


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCEDURE_STEP : reset hhmo_para                                                                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

4 调整 proteinBag_1VII 蛋白质模拟运行数据包中的相关参数（电荷参数、频谱参数），
这个步骤的相关参数为什么没有固定在数据文件 "proteinBag_1VII_linux.byte" 中，思考一下。


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCEDURE_STEP : get qomdRunPara                                                                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

5 将数据文件 "cppCuda_run_para_linux_2024.txt" 或者 "cppCuda_run_para_win_2024.txt" 中的数据读入数据块 InputPara * qomdRunPara
  这个文件是运行参数选项，可以自己修改，标记 do not modify 的就不要修改了。

PROTEIN_NAME        |         1VII                                              | not used, preserve, do not modify
GPU_DEVICE_NAME     |         970                                               | not used, preserve, do not modify
DATA_DIR            |         /home/m1/qomd2022/1VII/                           | not used, preserve, do not modify
RUN_START           |         1                                                 | 1 = START or 0 = CONTINUE, used, do not modify
PARA_ELEC           |         2                                                 | 1 = use qomd(old fixed para), 2 = use hhmo(new adjustable para), add 2024-10-70
PARA_FREQ           |         2                                                 | 1 = use qomd(old fixed para), 2 = use hhmo(new adjustable para), add 2024-10-70
RUN_MODE            |         1                                                 | 1 = SER or 2 = PAR or 3 = CUDA, could be modified, 1 or 2
RUN_METHOD          |         2                                                 | 1 = COUVDW or 2 = ELCCOL, could be modified, 1 or 2
RUN_GPU             |         1                                                 | 1 = one gpu or 2 = two gpus, not used, preserve, do not modify
RUN_ROUNDS          |         1000                                              | form 100 to 100000, used, could be modified
WRITE_FREQ          |         10                                                | write frequence, for compute temperature, could be modified
WRITE_MEDIUM_FREQ   |         10000                                             | write medium data files and pdb file, to protect run data when power off for servers, not used, preserve, do not modify
BOND_DEVI_RATIO     |         0.0435                                            | double bond_deviation_ratio = 0.0435;           qomd run para, used, do not modify
ANGLE_DEVI_RATIO    |         0.0435                                            | double angle_deviation_ratio = 0.0435;          qomd run para, used, do not modify
PLANE_DEVI_RATIO    |         0.0435                                            | double plane_deviation_ratio = 0.0435;          qomd run para, used, do not modify
DIHE_LIMIT_RATIO    |         0.0435                                            | double dihedral_ratio_LimitDeviation = 0.0435;  qomd run para, used, do not modify
DIHE_FLATTOP_RATIO  |         0.9                                               | double dihedral_ratio_FlattopDeviation = 0.9;   qomd run para, used, do not modify
END                 |                                                           |                                                          |

可以修改的选项为： 
PARA_ELEC   ：电荷参数选项，1 = use qomd(旧的参数，固定，不能用hhmoPanel调整), 2 = use hhmo(新的，可以用hhmoPanel调整), 
PARA_FREQ   ：频谱参数选项，1 = use qomd(旧的参数，固定，不能用hhmoPanel调整), 2 = use hhmo(新的，可以用hhmoPanel调整), 
RUN_MODE    ：串行模式，并行模式，GPU模式（暂无），这里的串行、并行，不是物理并行，而是为适应GPU模式做的预备，本质上都是CPU串行。
RUN_METHOD  ：电荷力的计算方式，
                 1 = COUVDW 旧的计算方式，只适用于旧的电荷参数qomd，当采用这个模式时， PARA_ELEC选项不起作用，但PARA_FREQ还是有作用的。
                 2 = ELCCOL 新的计算方式，适用于旧的电荷参数qomd 以及新的电荷参数hhmo，就是说 PARA_ELEC选项起作用，PARA_FREQ也有作用的。
RUN_ROUNDS  ：运行步数
WRITE_FREQ  ：输出温度的频次

这里可以看到，（1）PARA_ELEC 与 RUN_METHOD 要有个配合，当 RUN_METHOD = 1 = COUVDW 时，PARA_ELEC 不起作用。
             （2）PARA_ELEC 和 PARA_FREQ 选 1 = use qomd 的时候，hhmoPanel 不起作用。
其他可以自由组合。


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCEDURE_STEP : switch_ProteinPackage_Osci_Harmonic_by_HHMO                                                                    //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCEDURE_STEP : switch_ProteinPackage_elccol_charge_by_vertex_hhmoCharge                                                       //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

6 这两个步骤，根据 PARA_ELEC 和 PARA_FREQ 选项，在 proteinBag_1VII 蛋白质模拟运行数据包中做参数切换。
  这两步操作过程不可见，不用任何修改。


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCEDURE_STEP : reset CudaGridBlock                                                                                            //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

7 设置GPU选项，暂无实际作用，但不要删除。


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCEDURE_STEP : init osci data before run                                                                                      //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

8 运行初始化，不要修改。
```

## procedure_24103-读取并检查轨迹文件

void procedure_24103()简介
从文件"time_point_linux.byte"读取点云数据的样例。

```
RUN_PROCEDURE                |        24103       |  读取并检查轨迹文件
END
```

**功能说明**:

-   触发 `all_procedures_2024()` 函数调用 `procedure_24103()`
-   读取 `time_point.byte` 轨迹文件
-   打印开头和结尾的几帧数据，用于快速验证轨迹是否生成正确
-   输出数据类型大小信息，确保跨平台兼容性

**适用场景**:

-   验证之前模拟生成的轨迹文件
-   检查数据完整性
-   调试和故障排除

## 24104-计算RMSD (均方根偏差)

- 计算两个指定时间点之间的均方根偏差（RMSD），目前设置的`timeA=0;timeB=900`，即计算第 0 个时间点和第 900 个时间点之间的 RMSD。**在运行24104之前要先运行24102！！**
    - `rmsd_covMatNormL2_EPSILON`：协方差矩阵 L2 范数的阈值，用于判断协方差矩阵是否接近奇异，从而影响计算稳定性。
    - `rmsd_svd_EPSILON`：奇异值分解（SVD）中的精度阈值。
    - `rmsd_svd_ROUND`：SVD 计算的最大迭代次数，限制计算时间。
- 输入文件：`dvnn_vertexList_1`
- 输入蛋白质结构文件：`dvnn_vertexList_pdb_1.txt`（将 `.txt` 重命名为 `.pdb` ，就可以使用`Jmol`来显示蛋白质）
- 输出文件：`dvnn_vertexList_2`
- 输出蛋白质结构文件： `dvnn_vertexList_pdb_2.txt`（将 `.txt` 重命名为 `.pdb` ，就可以使用`Jmol`来显示蛋白质）

```
RUN_PROCEDURE                |        24104       |  计算 RMSD
END
```

**功能说明**:

-   读取轨迹文件
-   计算两个指定时间点（如初始结构和末尾结构）之间的RMSD值
-   用于分析结构变化和模拟质量

**RMSD意义**:

-   衡量两个分子结构的相似度
-   值越小表示结构越相似
-   是蛋白质动力学研究的重要指标

## 24105-运行蛋白质"1VII"完整模拟

1vii的PDB蛋白质数据，模拟运动

```
RUN_PROCEDURE                |        24105       |  运行 "1VII" 的模拟
END
```

**功能说明**:

-   执行1VII蛋白质的完整分子动力学模拟
-   从PDB数据库的标准构象开始
-   适用于常规蛋白质动力学研究

**蛋白质信息**:

-   **1VII**: 著名的蛋白质折叠研究对象
-   **原子数**: 596个原子
-   **氨基酸数**: 36个氨基酸
-   **特点**: 小蛋白质，适合快速测试和算法验证

## 24115-运行蛋白质"1VII_rod"折叠模拟

1vii从长杆模型开始（长链状态），模拟折叠过程

```
RUN_PROCEDURE                |        24115       |  运行 "1VII_rod" 的模拟
END
```

**功能说明**:

-   从长杆状态（完全展开的链状构象）开始模拟
-   观察蛋白质的折叠过程
-   这是蛋白质折叠研究的经典设置

**应用价值**:

-   研究蛋白质折叠机制
-   验证力场参数的合理性
-   生成折叠轨迹数据

## 其他蛋白质模拟流程

**流程 24106 - 4UZX蛋白质**:

```
RUN_PROCEDURE                |        24106       |  运行 "4UZX" 的模拟
END
```

**流程 24107 - 2RRK蛋白质**:

```bash
RUN_PROCEDURE                |        24107       |  运行 "2RRK" 的模拟
END
```

## 使用指南

### 1. 选择运行流程

要运行特定的流程，请确保对应的 `RUN_PROCEDURE` 块是文件中第一个有效的（未被注释的）块。

### 2. 注释其他流程

在不需要的流程行首添加 `#` 来注释掉：

```
#RUN_PROCEDURE                |        24104       |  计算 RMSD
#END
```

### 3. 修改活跃流程

移除目标流程前的 `#` 注释符：

```
RUN_PROCEDURE                |        24105       |  运行 "1VII" 的模拟
END
```

## 典型使用场景

### 场景1: 进行新的蛋白质模拟

1.  选择流程 24105 (1VII) 或 24115 (1VII_rod)
2.  确保相应的输入文件存在
3.  检查CUDA参数配置
4.  运行程序

### 场景2: 检查模拟结果

1.  先运行模拟流程 (24105/24115)
2.  然后选择流程 24103 检查轨迹
3.  最后选择流程 24104 计算RMSD

### 场景3: 系统测试

1.  选择流程 101 进行基础测试
2.  验证程序正常工作
3.  再进行实际模拟

## 注意事项

### 1. 文件格式要求

-   使用严格的列格式对齐
-   保持 `|` 分隔符的位置
-   `END` 标志必须存在

### 2. 依赖文件检查

在运行模拟流程前，确保以下文件存在：

-   对应的蛋白质数据包文件 (`proteinBag_*.byte`)
-   CUDA参数配置文件 (`cppCuda_run_para_linux_2024.txt`)
-   HHMO参数文件 (`hhmoPanel_linux.byte`)

### 3. 输出目录

确保 `OutputData/` 目录存在且有写入权限

### 4. 运行时间估计

-   流程 101: 几秒
-   流程 24103/24104: 几秒到几分钟
-   流程 24105/24115: 几分钟到几小时（取决于模拟步数）

## 故障排除

### 问题1: 程序无响应

-   检查是否有有效的 `RUN_PROCEDURE` 块
-   确认文件格式正确

### 问题2: 找不到输入文件

-   检查 `InputData/` 目录中的蛋白质数据包文件
-   确认文件名与流程对应

### 问题3: 权限错误

-   确保对 `OutputData/` 目录有写入权限
-   检查文件系统空间是否充足

## 参数扩展

如需添加新的运行流程：

1.  在 `qomdProtein2024.cpp` 的 `all_procedures_2024()` 函数中添加新的条件分支
2.  在此配置文件中添加对应的 `RUN_PROCEDURE` 块
3.  确保新流程有相应的输入数据支持

## 相关文件

-   **主程序**: `qomdProtein2024.cpp`
-   **CUDA参数**: `cppCuda_run_para_linux_2024.txt`
-   **输入数据**: `InputData/proteinBag_*.byte`
-   **输出数据**: `OutputData/` 目录下的各种文件