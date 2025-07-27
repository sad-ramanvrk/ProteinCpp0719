# time_point.byte 输出控制说明

## 问题分析

根据对 `time_point.byte` 文件的解析结果，我们发现：

1.  文件头信息

    ：

    -   总时间步数 (totalStamps): 1000
    -   总原子数 (totalVertex): 596
    -   实际数据点数: 596

2.  实际数据内容

    ：

    -   所有596个数据点的时间步 (t) 都是 0
    -   只包含了初始时刻的原子坐标

## 原因分析

通过分析 `qomdProtein2024.cpp` 的源代码，发现问题出现在主循环部分：

### 当前代码状态 (第1158-1170行)

```cpp
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
```

**关键发现**：

-   在主循环中，负责写入时间序列数据的代码被完全注释掉了
-   只有在初始化时写入了第0帧的数据
-   程序只在第100步输出一个快照文件，但不会写入到 `time_point.byte`

## 解决方案

要获得多时刻的轨迹数据，需要进行以下修改：

### 方案1：启用每步输出（数据量大）

取消注释第1158-1170行的代码：

```cpp
// 从proteinBag拷贝当前坐标到dvnnVertexList
qomd_shell3_copy_current_from_proteinBag_dvnnVertexList(dvnnVertexList, proteinBag_1VII);
// 将dvnnVertexList的坐标拷贝到timePointList的当前时间戳位置
int timeStamp = round + 1;
dvnn_copy_vertexList_to_timePointList_by_timeStamp(timePointList, dvnnVertexList, timeStamp);
// 将当前时间戳的数据追加写入轨迹文件
dvnn_save_timePointList_listData_to_byte_file(fname_output_TIME_POINT_byte, timePointList);
```

**结果**：每一步都会保存轨迹，文件大小将非常大（1000步 × 596原子 × 40字节 ≈ 23.8MB）

### 方案2：按频率输出（推荐）

修改代码，使用 `WRITE_FREQ` 参数控制输出频率：

```cpp
// 按指定频率保存轨迹数据
if (round % qomdRunPara->WRITE_FREQ == 0)
{
    // 从proteinBag拷贝当前坐标到dvnnVertexList
    qomd_shell3_copy_current_from_proteinBag_dvnnVertexList(dvnnVertexList, proteinBag_1VII);
    // 将dvnnVertexList的坐标拷贝到timePointList的当前时间戳位置
    int timeStamp = round;
    dvnn_copy_vertexList_to_timePointList_by_timeStamp(timePointList, dvnnVertexList, timeStamp);
    // 将当前时间戳的数据追加写入轨迹文件
    dvnn_save_timePointList_listData_to_byte_file(fname_output_TIME_POINT_byte, timePointList);
}
```

### 方案3：自定义输出频率

在 `cppCuda_run_para_linux_2024.txt` 中添加新的参数，如：

```
TRAJECTORY_WRITE_FREQ |      |10                  |                             | # [参数] 轨迹输出频率。每隔多少步输出一次原子坐标。
```

然后在代码中使用这个参数：

```cpp
// 按轨迹输出频率保存数据
if (round % qomdRunPara->TRAJECTORY_WRITE_FREQ == 0)
{
    // ... 保存代码
}
```

## 修改步骤

### 步骤1：修改源代码

编辑 `ProteinCpp2025_0109/src/qomdProtein2024.cpp` 文件：

1.  找到第1158-1170行
2.  取消注释相关代码
3.  添加频率控制逻辑（可选）

### 步骤2：重新编译

```bash
cd ProteinCpp2025_0109/obj
make clean
make
```

### 步骤3：调整输出频率（可选）

在 `cppCuda_run_para_linux_2024.txt` 中设置合适的 `WRITE_FREQ` 值：

-   `WRITE_FREQ = 1`：每步输出（文件很大）
-   `WRITE_FREQ = 10`：每10步输出一次
-   `WRITE_FREQ = 100`：每100步输出一次

### 步骤4：重新运行模拟

```bash
./qomdProtein2024
```

## 预期结果

修改后的程序将输出包含多个时间步的 `time_point.byte` 文件：

-   如果 `WRITE_FREQ = 10`，1000步模拟将产生100个时间切片
-   文件大小：约 100 × 596 × 40字节 ≈ 2.4MB
-   每个时间切片包含596个原子的坐标

## 注意事项

1.  **文件大小**：频率越高，文件越大，请根据存储空间调整
2.  **性能影响**：频繁的文件I/O可能略微影响模拟性能
3.  **内存使用**：需要确保有足够内存存储时间序列数据
4.  **数据一致性**：修改后需要重新验证数据格式的正确性

## 验证方法

使用我们的Python脚本重新读取修改后的文件：

```bash
python read_time_point_byte.py
```

应该能看到多个不同的时间步值，而不是全部为0。