# 3 运行参数cppCuda_run_para_linux_2024

## 文件概述

**文件名**: `cppCuda_run_para_linux_2024.txt`

**作用**: 控制分子动力学模拟的核心计算参数和物理力场参数

**位置**: 项目根目录

**重要性**: ⭐⭐⭐⭐⭐ (核心配置文件，直接影响模拟结果和系统温度)

## 文件结构说明

该文件包含了分子动力学模拟的所有关键控制参数。程序启动时会读取这些参数来配置模拟环境。

## 参数分类详解

### 1. 占位符与未使用参数

### PROTEIN_NAME

```
PROTEIN_NAME        |        |1VII
```

-   **状态**: 未使用，仅为占位符
-   **用途**: 保留用于将来扩展
-   **建议**: 保持不变

### GPU_DEVICE_NAME

```
GPU_DEVICE_NAME     |        |970
```

-   **状态**: 未使用，仅为占位符
-   **用途**: 记录GPU设备型号
-   **建议**: 可更新为实际使用的GPU型号

### DATA_DIR

```
DATA_DIR            |        |/home/m1/qomd2022/1VII/
```

-   **状态**: 未使用，仅为占位符
-   **用途**: 保留的数据目录路径
-   **建议**: 保持不变

------

### 2. 模拟运行控制参数

### RUN_START - 模拟启动模式

```
RUN_START           |        |1
```

**参数值说明**:

-   `1`: START - 从头开始新的模拟
-   `0`: CONTINUE - 从断点继续先前的模拟

**注意**: 断点续跑功能暂未完成，建议使用 `1`

**修改建议**:

-   ✅ 可修改
-   新手用户保持 `1`

### RUN_MODE - 计算模式

```
RUN_MODE            |        |2
```

**参数值说明**:

-   `1`: SER (串行) - 单线程计算
-   `2`: PAR (并行) - 多线程计算
-   `3`: CUDA (GPU) - GPU加速计算 (暂未完成)

**性能影响**:

-   SER: 速度慢，适合调试
-   PAR: 速度快，推荐使用

**修改建议**:

-   ✅ 可修改
-   推荐使用 `2` (PAR)

### RUN_METHOD - 非键相互作用计算方法

```
RUN_METHOD          |        |2
```

**参数值说明**:

-   `1`: COUVDW (库仑力+范德华力) - 旧方法，暂不推荐
-   `2`: ELCCOL (电荷力+碰撞力) - 新方法，推荐使用
-   `3`: only_4_osci (仅键合力) - 用于测试

**物理意义**:

-   ELCCOL方法物理模型更准确
-   参数可调性更好

**修改建议**:

-   ✅ 可修改
-   推荐使用 `2` (ELCCOL)

### RUN_GPU - GPU数量

```
RUN_GPU             |        |1
```

**参数值说明**:

-   `1`: 使用单个GPU设备
-   `2`: 使用两个GPU设备

**注意**: GPU功能暂未完成

**修改建议**:

-   保持 `1`

### RUN_ROUNDS - 总模拟步数

```
RUN_ROUNDS          |        |1000
```

**参数说明**:

-   每轮对应 1 飞秒 (fs) 的模拟时间
-   取值范围: 1 ~ 2,147,483,647 (int类型最大值)
-   当前设置: 1000步 = 1皮秒 (ps)

**时间尺度参考**:

-   1,000步 = 1ps (适合快速测试)
-   100,000步 = 100ps (短期模拟)
-   1,000,000步 = 1ns (常规模拟)

**修改建议**:

-   ✅ 可修改
-   测试用: 100-1000
-   研究用: 10000-1000000

### WRITE_FREQ - 状态输出频率

```
WRITE_FREQ          |        |1
```

**参数说明**:

-   每隔多少步输出一次温度等评估值
-   值为1表示每步都输出

**修改建议**:

-   ✅ 可修改
-   频繁输出: 1-10 (适合调试)
-   正常输出: 100-1000 (适合长期模拟)

### WRITE_MEDIUM_FREQ - 中继文件写入频率

```
WRITE_MEDIUM_FREQ   |        |10000
```

**状态**: 未使用，用于断点续跑功能 **修改建议**: 保持不变

------

### 3. 力场参数选择

### PARA_ELEC - 电荷参数来源

```
PARA_ELEC           |        |2
```

**参数值说明**:

-   `1`: QOMD - 使用旧的固定电荷参数
-   `2`: HHMO - 使用新的可调电荷参数

**推荐设置**: `2` (HHMO)

### PARA_FREQ - 振动频率参数来源

```
PARA_FREQ           |        |2
```

**参数值说明**:

-   `1`: QOMD - 使用旧的固定频率参数
-   `2`: HHMO - 使用新的可调频率参数

**推荐设置**: `2` (HHMO)

------

### 4. 键合力参数 (核心物理参数)

### 振子最大偏移比例 (影响系统温度)

### BOND_DEVI_RATIO - 键长偏移比例

```
BOND_DEVI_RATIO     |        |0.06
```

**物理意义**:

-   控制化学键可允许的最大拉伸/压缩范围
-   最大偏移量 = 平衡键长 × 偏移比例

**示例**:

-   平衡键长 1.0Å，比例 0.06，则最大偏移 0.06Å
-   键长变化范围: [0.94Å, 1.06Å]

**温度影响**:

-   数值越大 → 原子运动范围越大 → 温度越高
-   数值越小 → 原子运动受限 → 温度越低

**修改建议**:

-   ✅ 可小范围修改 (0.02-0.10)
-   默认 0.06 适合室温模拟

### ANGLE_DEVI_RATIO - 键角偏移比例

```
ANGLE_DEVI_RATIO    |        |0.06
```

**物理意义**:

-   控制键角可允许的最大偏移范围
-   影响分子的弯曲柔性

**修改建议**:

-   ✅ 可小范围修改 (0.02-0.10)
-   通常与BOND_DEVI_RATIO保持相近

### PLANE_DEVI_RATIO - 平面偏移比例

```
PLANE_DEVI_RATIO    |        |0.06
```

**物理意义**:

-   控制平面约束的偏移范围
-   影响芳香环等平面结构的稳定性

**修改建议**:

-   ✅ 可小范围修改 (0.02-0.10)

### 二面角振子参数

### DIHE_LIMIT_RATIO - 二面角谷底偏移比例

```
DIHE_LIMIT_RATIO    |        |0.0435
```

**物理意义**:

-   控制二面角在能量谷底的振动范围
-   影响分子构象变化的容易程度

**修改建议**:

-   ⚠️ 建议暂时不修改
-   需要深入了解二面角势能函数

### DIHE_FLATTOP_RATIO - 二面角峰顶偏移比例

```
DIHE_FLATTOP_RATIO  |        |0.9
```

**物理意义**:

-   控制二面角在能量峰顶的行为
-   影响构象转换的能垒

**修改建议**:

-   ⚠️ 建议暂时不修改

### 振子强制回位参数

### BOND_ENFORCE_RATIO - 键长强制回位比例

```
BOND_ENFORCE_RATIO  |        |0.3
```

**物理意义**:

-   当键长偏移超过极限时，强制回拉的力度
-   防止分子结构过度扭曲

**作用机制**:

-   正常情况下原子自由振动
-   超出极限时执行强制回位操作
-   回拉幅度 = 超出量 × 回位比例

**修改建议**:

-   ✅ 可小范围修改 (0.1-0.5)
-   影响系统温度和稳定性

### ANGLE_ENFORCE_RATIO & PLANE_ENFORCE_RATIO

```
ANGLE_ENFORCE_RATIO |        |0.3
PLANE_ENFORCE_RATIO |        |0.3
```

**作用**: 类似于BOND_ENFORCE_RATIO，分别控制角度和平面的强制回位

### 极限偏移量动态修改率

### BOND_DEVIMODI_RATE - 键长偏移量调整速率

```
BOND_DEVIMODI_RATE  |        |0.1
```

**物理意义**:

-   控制"极限偏移量"的动态调整速度
-   极限偏移量在[最小偏移量, 最大偏移量]区间内变化
-   代表振子当前携带的能量

**作用机制**:

-   控制能量在振子间的流转速率
-   影响下一步的振动位移
-   直接影响系统温度

**修改建议**:

-   ✅ 可小范围修改 (0.05-0.2)
-   值越大温度变化越快

### ANGLE_DEVIMODI_RATE & PLANE_DEVIMODI_RATE

```
ANGLE_DEVIMODI_RATE |        |0.1
PLANE_DEVIMODI_RATE |        |0.1
```

**作用**: 类似功能，分别控制角度和平面约束的动态调整

------

### 5. 非键力参数 (ELCCOL方法专用)

### RATIO_ELCCOL_R_WALL - 碰撞墙位置比例

```
RATIO_ELCCOL_R_WALL |        |0.7
```

**物理意义**:

-   碰撞墙位置 = 接触位置 × R_WALL比例
-   接触位置 ≈ 两原子范德华半径之和
-   决定原子间"硬碰撞"的起始距离

**示例**:

-   接触距离 3.0Å，比例 0.7，则碰撞墙位置 2.1Å
-   原子间距 < 2.1Å 时发生强烈排斥

**修改建议**:

-   ✅ 可修改 (0.5-0.9)
-   影响分子的紧密程度和温度

### RATIO_ELCCOL_COL1 - 强碰撞力参数

```
RATIO_ELCCOL_COL1   |        |0.2
```

**物理意义**:

-   当原子间距 < 碰撞墙位置时的回弹力度
-   数值越大，碰撞反弹越强烈

**修改建议**:

-   ✅ 可修改 (0.1-0.5)
-   影响系统温度

### RATIO_ELCCOL_COL2 - 弱碰撞力参数

```
RATIO_ELCCOL_COL2   |        |0.1
```

**物理意义**:

-   当 碰撞墙位置 < 间距 < 接触位置 时的回弹力度
-   比COL1更温和的排斥力

**修改建议**:

-   ✅ 可修改 (0.05-0.3)

### RATIO_ELCCOL_ELC - 电荷力参数

```
RATIO_ELCCOL_ELC    |        |0.02
```

**物理意义**:

-   当间距 > 接触位置时的电荷相互作用强度
-   控制长程静电力的大小
-   对蛋白质折叠驱动力影响很大

**重要性**: ⭐⭐⭐⭐⭐

-   蛋白质折叠的主要驱动力
-   影响二级结构形成

**修改建议**:

-   ✅ 可修改 (0.01-0.05)
-   小心调整，影响折叠结果

------

## 参数调整策略

### 1. 温度控制策略

### 降低温度 (获得更稳定的结构):

-   减小 BOND/ANGLE/PLANE_DEVI_RATIO (0.04-0.05)
-   减小 DEVIMODI_RATE (0.05-0.08)
-   增大 ENFORCE_RATIO (0.4-0.5)

### 升高温度 (增加构象采样):

-   增大 BOND/ANGLE/PLANE_DEVI_RATIO (0.08-0.10)
-   增大 DEVIMODI_RATE (0.15-0.20)
-   减小 ENFORCE_RATIO (0.2-0.3)

### 2. 折叠研究策略

### 增强折叠驱动力:

-   适当增大 RATIO_ELCCOL_ELC (0.03-0.04)
-   保持合理的温度参数

### 平衡探索与稳定性:

-   中等温度设置
-   逐步调整电荷力参数

### 3. 系统稳定性策略

### 防止结构崩塌:

-   保持 ENFORCE_RATIO ≥ 0.2
-   碰撞参数不要设置过大
-   监控键长键角偏移水平

------

## 参数监控指标

### 1. 温度监控

-   `t_DeltaR`: 总温度 (目标: 250-350K)
-   `t_DeltaRElcCol`: 非键贡献温度
-   `t_DeltaRCouVdw`: 另一种非键贡献温度

### 2. 结构稳定性监控

-   `LimitDeviationLevel_Bond`: 键长偏差水平 (0.2-0.8)
-   `LimitDeviationLevel_Angle`: 键角偏差水平
-   `Mode_1_Level_Bond`: 强制回位操作比例 (<0.1)

### 3. 碰撞统计监控

-   `Num_elccol_move_COL1/COL2/ELC`: 各种作用计数
-   合理的比例分布表明参数设置恰当

------

## 常见问题与解决方案

### 1. 温度过高 (>400K)

**症状**: t_DeltaR > 400 **解决**:

-   减小偏移比例参数
-   增大强制回位比例
-   减小动态修改率

### 2. 温度过低 (<200K)

**症状**: t_DeltaR < 200 **解决**:

-   增大偏移比例参数
-   减小强制回位比例
-   增大动态修改率

### 3. 结构不稳定

**症状**: 键长键角偏差过大 **解决**:

-   增大强制回位比例
-   检查碰撞参数设置
-   降低动态修改率

### 4. 折叠效果差

**症状**: RMSD变化很小 **解决**:

-   适当增大电荷力参数
-   确保使用1VII_rod起始构象
-   增加模拟时间

------

## 推荐参数组合

### 1. 标准设置 (室温模拟)

```
RUN_MODE            = 2
RUN_METHOD          = 2
PARA_ELEC           = 2
PARA_FREQ           = 2
BOND_DEVI_RATIO     = 0.06
BOND_ENFORCE_RATIO  = 0.3
BOND_DEVIMODI_RATE  = 0.1
RATIO_ELCCOL_ELC    = 0.02
```

### 2. 低温设置 (结构优化)

```
BOND_DEVI_RATIO     = 0.04
BOND_ENFORCE_RATIO  = 0.4
BOND_DEVIMODI_RATE  = 0.08
RATIO_ELCCOL_ELC    = 0.015
```

### 3. 高温设置 (构象采样)

```
BOND_DEVI_RATIO     = 0.08
BOND_ENFORCE_RATIO  = 0.25
BOND_DEVIMODI_RATE  = 0.15
RATIO_ELCCOL_ELC    = 0.025
```

------

## 注意事项

### 1. 参数修改原则

-   **小步调整**: 每次只修改一个参数
-   **记录变化**: 详细记录每次修改和结果
-   **监控指标**: 关注温度和结构稳定性指标

### 2. 备份策略

-   修改前备份原始参数文件
-   为不同的研究目标保存不同的参数组合

### 3. 性能考虑

-   更精细的参数设置可能需要更多计算时间
-   平衡精度和效率

### 4. 跨平台注意

-   Windows和Linux版本使用相同的参数文件
-   确保数值格式兼容性

------

这份文档提供了CUDA参数文件的完整说明。建议在修改参数前仔细阅读相关章节，并在小规模测试后再进行大规模模拟。



## RUN_METHOD

控温度是模拟折叠的关键，实际的蛋白质折叠中，温度也是关键要素，不同的温度，蛋白质折叠形态就会不同。

src/aBase.h

```
// 温度相关参数
double t_DeltaR;         // 温度变化量（总）
double t_DeltaRElcCol;   // 与静电/碰撞相关的温度变化
double t_DeltaRCouVdw;   // 与库仑力/范德华力相关的温度变化
```



RUN_METHOD : 1 = COUVDW or 2 = ELCCOL，RUN_MODE   : 1 = SER    or 2 = PAR，各代表什么意思

答：COUVDW 库伦力范德华力模型，ELCCOL电荷力碰撞力模型，SER串行模式，PAR并行模式。是模拟器内部的运行模式选择。这个先不必细究，随便选就可以。

## WRITE_FREQ

iterate中输出round和温度temperature是从宏观角度观测运行是否正常，在300K范围附近波动都属于正常，WRITE_FREQ用来控制输出温度的频次，不必每一步都计算温度。

（WRITE_FREQ数值越大，running time也会变大）

time_start的值在WRITE_FREQ不同时也不同

## RATIO_ELCCOL_R_WALL

碰撞半径比例（默认0.7）

## 都是可以控温度的

```
BOND_ENFORCE_RATIO  |        |0.3                 |                             | double bond_normal_inner_regressive_enforce_ration  = 0.3;   qomd run para, used, could be modified

ANGLE_ENFORCE_RATIO |        |0.3                 |                             | double angle_normal_inner_regressive_enforce_ration = 0.3;   qomd run para, used, could be modified

PLANE_ENFORCE_RATIO |        |0.3                 |                             | double plane_normal_inner_regressive_enforce_ration = 0.3;   qomd run para, used, could be modified

BOND_DEVIMODI_RATE  |        |0.1                 |                             | double bond_limit_deviation_modify_rate  = 0.5;   qomd run para, used, could be modified

ANGLE_DEVIMODI_RATE |        |0.1                 |                             | double angle_limit_deviation_modify_rate = 0.5;   qomd run para, used, could be modified

PLANE_DEVIMODI_RATE |        |0.1                 |                             | double plane_limit_deviation_modify_rate = 0.5;   qomd run para, used, could be modified

RATIO_ELCCOL_R_WALL |        |0.7                 |                             | 0.7,  qomd run para, used, do not modify

RATIO_ELCCOL_COL1   |        |0.2                 |                             | 0.5,  qomd run para, used, could be modified

RATIO_ELCCOL_COL2   |        |0.1                 |                             | 0.3,  qomd run para, used, could be modified

RATIO_ELCCOL_ELC    |        |0.02                |                             | 0.05, qomd run para, used, could be modified
```

PROTEIN_NAME    |     1VII                       

| not used, preserve, do not modify

蛋白质名，保留，未使用

 

GPU_DEVICE_NAME   |     970                        

| not used, preserve, do not modify

显卡名，保留，未使用

 

DATA_DIR      |     /home/m1/qomd2022/1VII/              

| not used, preserve, do not modify

路径名，保留，未使用

 

RUN_START      |     1                        

| 1 = START or 0 = CONTINUE, used, do not modify

选项：重新开始模拟/继续上一次的模拟（此功能需要输出保存现场的中继文件）（此功能未完成，但是此功能实现比较简单，以前版本就有这个功能，可以很快搞好），

 

PARA_ELEC      |     2                        

| 1 = use qomd(old fixed para), 2 = use hhmo(new adjustable para), add 2024-10-70

选项：电荷参数（qomd旧参数，固定不可调）/电荷参数（hhmo新参数，可调），

 

PARA_FREQ      |     2                        

| 1 = use qomd(old fixed para), 2 = use hhmo(new adjustable para), add 2024-10-70

选项：频谱参数（qomd旧参数，固定不可调）/频谱参数（hhmo新参数，可调），

 

RUN_MODE      |     2                        

| 1 = SER or 2 = PAR or 3 = CUDA, could be modified, 1 or 2

选项：串行/并行/GPU（此功能未完成），

 

RUN_METHOD     |     2                        

| 1 = COUVDW or 2 = ELCCOL or 3 = only_4_osci, could be modified, 1 or 2 or 3

选项：非键计算（COUVDW旧方法）/非键计算（ELCCOL新方法）/不计算非键（仅做测试用），

解释：虽然COUVDW仍然可以运行，但是我们后面基本不用这种方式了（暂时不打算进一步研究这个COUVDW模式了）。

 

RUN_GPU       |     1                        

| 1 = one gpu or 2 = two gpus, not used, preserve, do not modify

选项：用一个显卡/用两个显卡，（此功能未完成）

 

RUN_ROUNDS     |     10000                       

| form 100 to 100000, used, could be modified

参数：模拟步数

 

WRITE_FREQ     |     1                        

| write frequence, for compute temperature, could be modified

参数：输出温度等评估值的频度

 

WRITE_MEDIUM_FREQ  |     10000                       

| write medium data files and pdb file, to protect run data when power off for servers, not used, preserve, do not modify

参数：输出中继文件的频度，为了中断运行（或意外停机）后继续运行，配合RUN_START选项0，（此功能未完成）

 

BOND_DEVI_RATIO   |     0.2                        

| double bond_deviation_ratio = 0.0435;      qomd run para, used, do not modify

参数：BOND振子的最大偏移比例。“此参数可以小范围修改”，“此参数影响温度”

解释：BOND振子的“最大偏移量”由“平衡位值”和这个“最大偏移比例”共同确定。

按照0.2取此参数，比如平衡位（两原子距离）是1.0，则“最大偏移量”为0.2，则“最大（拉伸）量”为1.2，“最大（压缩）量”（压缩）为0.8。

同时确定一个“最小偏移量”，取“最大偏移量”的0.5（此值内置固定），

按照0.2取此参数，比如平衡位（两原子距离）是1.0，则“最小偏移量”为0.1，则“最小（拉伸）量”为1.1，“最小（压缩）量”（压缩）为0.9。

 

ANGLE_DEVI_RATIO  |     0.2                        

| double angle_deviation_ratio = 0.0435;     qomd run para, used, do not modify

参数：ANGLE振子的最大偏移比例。“此参数可以小范围修改”，“此参数影响温度”

解释：同上。

 

PLANE_DEVI_RATIO  |     0.2                        

| double plane_deviation_ratio = 0.0435;     qomd run para, used, do not modify

参数：PLANE振子的最大偏移比例。“此参数可以小范围修改”，“此参数影响温度”

解释：同上。

 

DIHE_LIMIT_RATIO  |     0.0435                      

| double dihedral_ratio_LimitDeviation = 0.0435; qomd run para, used, do not modify

参数：dihedral振子的“谷底”最大偏移比例。“此参数暂时不改”。

解释：这个解释比较复杂，暂缓。

 

DIHE_FLATTOP_RATIO |     0.9                        

| double dihedral_ratio_FlattopDeviation = 0.9;  qomd run para, used, do not modify

参数：dihedral振子的“峰顶”偏移比例。“此参数暂时不改”

解释：这个解释比较复杂，暂缓。

 

BOND_ENFORCE_RATIO |     0.3                        

| double bond_normal_inner_regressive_enforce_ration = 0.3;  qomd run para, used, could be modified

参数：BOND振子的回位操作比例参数。“此参数可以小范围修改”，“此参数影响温度”

解释：“强制回位操作”：当振子的状态超出了“极限偏移量”（参考参数BOND_DEVIMODI_RATE的解释）时，执行拉回操作，这个“拉回操作的具体量值”由“超出量”结合这个“回位操作比例参数”共同确定。

 

ANGLE_ENFORCE_RATIO |     0.3                        

| double angle_normal_inner_regressive_enforce_ration = 0.3;  qomd run para, used, could be modified

参数：ANGLE振子的回位操作比例参数。“此参数可以小范围修改”，“此参数影响温度”

解释：同上。

 

PLANE_ENFORCE_RATIO |     0.3                        

| double plane_normal_inner_regressive_enforce_ration = 0.3;  qomd run para, used, could be modified

参数：PLANE振子的回位操作比例参数。“此参数可以小范围修改”，“此参数影响温度”

解释：同上。

 

BOND_DEVIMODI_RATE |     0.1                        

| double bond_limit_deviation_modify_rate = 0.5;  qomd run para, used, could be modified

参数：BOND振子的“极限偏移量”修改算法比例参数。“此参数可以小范围修改”，“此参数影响温度”

解释：前面提到的“最大偏移量”是一个固定值，含义是该振子的偏移量（拉伸态或压缩态）都不能超过这个值，而每一步模拟过程中都可以在“最大偏移量”和“最小偏移量”范围内修改一个参数称为“极限偏移量”，

既满足条件：“最大偏移量”>“极限偏移量”>“最小偏移量”

这个“极限偏移量”可以表示当前该振子所具有的能量，是不断变化的，相当于是能量的流转（参考前面“振子能量流转”的解释），而这个比例参数（BOND_DEVIMODI_RATE）是用来控制这个“极限偏移量”修改算法的。这个“极限偏移量”也决定下一步模拟的振动位移量，也就是对温度有影响。

 

ANGLE_DEVIMODI_RATE |     0.1                        

| double angle_limit_deviation_modify_rate = 0.5;  qomd run para, used, could be modified

参数：ANGLE振子的“极限偏移量”修改算法比例参数。“此参数可以小范围修改”，“此参数影响温度”

解释：同上。

 

PLANE_DEVIMODI_RATE |     0.1                        

| double plane_limit_deviation_modify_rate = 0.5;  qomd run para, used, could be modified

参数：PLANE振子的“极限偏移量”修改算法比例参数。“此参数可以小范围修改”，“此参数影响温度”

解释：同上。

 

RATIO_ELCCOL_R_WALL |     0.7                        

| 0.7, qomd run para, used, do not modify

参数：碰撞墙的位置参数

解释：ELCCOL为电荷力/碰撞力的相互作用计算，所有的非键两原子之间的相互作用可以认为是，当距离足够近时，执行碰撞计算，当距离超出碰撞作用范围时，执行电荷力计算。而这个“碰撞作用范围”由两个值设定，“碰撞墙位置”和“接触位置”，可以类比为两个有弹性的皮球，“碰撞墙位置”相当于皮球挤在一起的极限位置，“接触位置”相当于皮球仅仅是挨在一起。“接触位置”的设定由两个范德华距离（类似于原子半径）相加得到，而“碰撞墙位置”由这个参数“RATIO_ELCCOL_R_WALL”结合“接触位置”计算得到。所以这个参数也是个经验值，可以修改。“此参数影响温度”

 

RATIO_ELCCOL_COL1  |     0.2                        

| 0.5, qomd run para, used, could be modified

参数：碰撞力的计算参数1，

解释：当前非键两原子之间的距离比“碰撞墙位置”还要小，执行回弹计算的比例参数，这个参数也是个经验值，可以修改。“此参数影响温度”

 

RATIO_ELCCOL_COL2  |     0.1                        

| 0.3, qomd run para, used, could be modified

参数：碰撞力的计算参数2，

解释：当前非键两原子之间的距离比“碰撞墙位置”大，但是小于“接触位置”，执行回弹计算的比例参数，这时的回弹力要小于前面的情况，这个参数也是个经验值，可以修改。“此参数影响温度”

 

RATIO_ELCCOL_ELC  |     0.02                       

| 0.05, qomd run para, used, could be modified

参数：电荷力的计算参数，

解释：当前非键两原子之间的距离比“接触位置”大，可以认为没有回弹力作用，仅仅计算电荷力作用，这个比例参数对温度影响很大（因为电荷力计算是主要的折叠驱动力），这个参数也是个经验值，可以修改。“此参数影响温度”
