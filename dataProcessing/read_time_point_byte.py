#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Python脚本：读取time_point.byte文件中的蛋白质轨迹数据

基于对C++代码的分析，time_point.byte文件的结构如下：
1. 前4字节：totalStamps (int) - 总时间步数
2. 接下来4字节：totalVertex (int) - 总原子数
3. 后续数据：DvnnTimePoint结构体数组

DvnnTimePoint结构体包含：
- vertexIdx (int): 原子索引
- vertexNameSN (int): 原子序列号
- valid (int): 有效性标志
- t (int): 时间步
- x, y, z (double): 三维坐标
"""

import struct
import sys
import os
import json
from datetime import datetime


def read_time_point_file(filename):
    """
    读取time_point.byte文件

    Args:
        filename (str): 文件路径

    Returns:
        tuple: (header_info, time_points)
    """
    print(f"尝试读取文件: {filename}")

    if not os.path.exists(filename):
        print(f"错误：文件 {filename} 不存在")
        return None, None

    file_size = os.path.getsize(filename)
    print(f"文件大小: {file_size} 字节")

    try:
        with open(filename, 'rb') as f:
            # 读取文件头：总时间步数和总原子数
            data = f.read(4)  # 读取totalStamps (int, 4字节)
            if len(data) < 4:
                print("错误：文件太小，无法读取totalStamps")
                return None, None
            total_stamps = struct.unpack('<i', data)[0]  # 小端序整数

            data = f.read(4)  # 读取totalVertex (int, 4字节)
            if len(data) < 4:
                print("错误：文件太小，无法读取totalVertex")
                return None, None
            total_vertex = struct.unpack('<i', data)[0]  # 小端序整数

            print(f"文件头信息：")
            print(f"  总时间步数 (totalStamps): {total_stamps}")
            print(f"  总原子数 (totalVertex): {total_vertex}")
            print(f"  预期数据点总数: {total_stamps * total_vertex}")

            # 计算DvnnTimePoint结构体大小
            # struct DvnnTimePoint {
            #     int vertexIdx;       // 4字节
            #     int vertexNameSN;    // 4字节
            #     int valid;           // 4字节
            #     int t;               // 4字节
            #     double x, y, z;      // 每个8字节，共24字节
            # }
            # 总大小：4+4+4+4+24 = 40字节

            point_size = 40  # DvnnTimePoint结构体大小
            expected_points = total_stamps * total_vertex
            remaining_size = expected_points * point_size

            print(f"预期数据部分大小: {remaining_size} 字节")
            print(f"实际剩余文件大小: {file_size - 8} 字节")

            # 读取所有轨迹点数据
            trajectory_data = f.read(remaining_size)
            actual_data_size = len(trajectory_data)
            print(f"实际读取数据大小: {actual_data_size} 字节")

            if actual_data_size < remaining_size:
                print(f"警告：实际读取的数据大小 {actual_data_size} 小于预期大小 {remaining_size}")
                actual_points = actual_data_size // point_size
                print(f"实际数据点数: {actual_points}")
            else:
                actual_points = expected_points

            # 解析轨迹点数据
            time_points = []
            for i in range(actual_points):
                offset = i * point_size
                point_data = trajectory_data[offset:offset + point_size]

                if len(point_data) < point_size:
                    print(f"警告：第{i}个数据点数据不完整，仅有{len(point_data)}字节")
                    break

                # 解析DvnnTimePoint结构体
                # 格式: '<iiiiDDD' 表示小端序，4个int + 3个double
                try:
                    values = struct.unpack('<iiiiddd', point_data)

                    point = {
                        'vertexIdx': values[0],  # 原子索引
                        'vertexNameSN': values[1],  # 原子序列号
                        'valid': values[2],  # 有效性标志
                        't': values[3],  # 时间步
                        'x': values[4],  # X坐标
                        'y': values[5],  # Y坐标
                        'z': values[6]  # Z坐标
                    }
                    time_points.append(point)
                except struct.error as e:
                    print(f"错误：解析第{i}个数据点时出错: {e}")
                    break

            header_info = {
                'total_stamps': total_stamps,
                'total_vertex': total_vertex,
                'actual_points': len(time_points)
            }

            print(f"成功解析 {len(time_points)} 个数据点")
            return header_info, time_points

    except Exception as e:
        print(f"读取文件时发生错误: {e}")
        return None, None


def save_results_to_txt(header_info, time_points, output_file):
    """
    将所有结果保存到txt文件中

    Args:
        header_info (dict): 头部信息
        time_points (list): 轨迹点列表
        output_file (str): 输出文件名
    """
    print(f"正在保存结果到txt文件: {output_file}")

    try:
        with open(output_file, 'w', encoding='utf-8') as f:
            # 写入文件头部信息
            f.write("=" * 80 + "\n")
            f.write("QOMD time_point.byte 文件解析结果\n")
            f.write("=" * 80 + "\n")
            f.write(f"解析时间: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
            f.write(f"总时间步数: {header_info['total_stamps']}\n")
            f.write(f"总原子数: {header_info['total_vertex']}\n")
            f.write(f"实际数据点数: {header_info['actual_points']}\n")
            f.write("=" * 80 + "\n\n")

            # 写入数据点详情
            f.write("数据点详情:\n")
            f.write("-" * 80 + "\n")
            f.write(
                f"{'索引':<8} {'原子idx':<8} {'序列号':<8} {'有效性':<8} {'时间步':<8} {'X坐标':<12} {'Y坐标':<12} {'Z坐标':<12}\n")
            f.write("-" * 80 + "\n")

            for i, point in enumerate(time_points):
                f.write(f"{i:<8} {point['vertexIdx']:<8} {point['vertexNameSN']:<8} "
                        f"{point['valid']:<8} {point['t']:<8} {point['x']:<12.4f} "
                        f"{point['y']:<12.4f} {point['z']:<12.4f}\n")

            # 写入统计信息
            if time_points:
                time_steps = set(point['t'] for point in time_points)
                vertex_indices = set(point['vertexIdx'] for point in time_points)
                valid_points = sum(1 for point in time_points if point['valid'] == 1)

                f.write("\n" + "=" * 80 + "\n")
                f.write("统计信息:\n")
                f.write("=" * 80 + "\n")
                f.write(f"时间步范围: {min(time_steps)} ~ {max(time_steps)}\n")
                f.write(f"时间步总数: {len(time_steps)}\n")
                f.write(f"原子索引范围: {min(vertex_indices)} ~ {max(vertex_indices)}\n")
                f.write(f"有效数据点数: {valid_points}\n")
                f.write(f"无效数据点数: {len(time_points) - valid_points}\n")

                # 坐标范围统计
                x_coords = [point['x'] for point in time_points if point['valid'] == 1]
                y_coords = [point['y'] for point in time_points if point['valid'] == 1]
                z_coords = [point['z'] for point in time_points if point['valid'] == 1]

                if x_coords:
                    f.write(f"X坐标范围: {min(x_coords):.4f} ~ {max(x_coords):.4f}\n")
                    f.write(f"Y坐标范围: {min(y_coords):.4f} ~ {max(y_coords):.4f}\n")
                    f.write(f"Z坐标范围: {min(z_coords):.4f} ~ {max(z_coords):.4f}\n")

        print(f"成功保存txt文件: {output_file}")

    except Exception as e:
        print(f"保存txt文件时发生错误: {e}")


def save_results_to_json(header_info, time_points, output_file):
    """
    将所有结果保存到json文件中

    Args:
        header_info (dict): 头部信息
        time_points (list): 轨迹点列表
        output_file (str): 输出文件名
    """
    print(f"正在保存结果到json文件: {output_file}")

    try:
        # 构建完整的数据结构
        result_data = {
            "metadata": {
                "parse_time": datetime.now().isoformat(),
                "file_info": header_info,
                "description": "QOMD time_point.byte文件解析结果"
            },
            "header": header_info,
            "time_points": time_points
        }

        # 添加统计信息
        if time_points:
            time_steps = list(set(point['t'] for point in time_points))
            vertex_indices = list(set(point['vertexIdx'] for point in time_points))
            valid_points = [point for point in time_points if point['valid'] == 1]

            statistics = {
                "time_steps": {
                    "range": [min(time_steps), max(time_steps)],
                    "count": len(time_steps),
                    "all_steps": sorted(time_steps)
                },
                "vertices": {
                    "range": [min(vertex_indices), max(vertex_indices)],
                    "count": len(vertex_indices),
                    "all_indices": sorted(vertex_indices)
                },
                "data_quality": {
                    "total_points": len(time_points),
                    "valid_points": len(valid_points),
                    "invalid_points": len(time_points) - len(valid_points)
                }
            }

            # 坐标统计
            if valid_points:
                x_coords = [point['x'] for point in valid_points]
                y_coords = [point['y'] for point in valid_points]
                z_coords = [point['z'] for point in valid_points]

                statistics["coordinates"] = {
                    "x_range": [min(x_coords), max(x_coords)],
                    "y_range": [min(y_coords), max(y_coords)],
                    "z_range": [min(z_coords), max(z_coords)]
                }

            result_data["statistics"] = statistics

        # 保存为JSON文件
        with open(output_file, 'w', encoding='utf-8') as f:
            json.dump(result_data, f, ensure_ascii=False, indent=2)

        print(f"成功保存json文件: {output_file}")

    except Exception as e:
        print(f"保存json文件时发生错误: {e}")


def print_sample_data(header_info, time_points, sample_size=10):
    """
    打印样本数据

    Args:
        header_info (dict): 头部信息
        time_points (list): 轨迹点列表
        sample_size (int): 要打印的样本数量
    """
    print(f"\n轨迹数据统计：")
    print(f"  成功读取的数据点数: {len(time_points)}")

    if len(time_points) > 0:
        print(f"\n前 {min(sample_size, len(time_points))} 个数据点：")
        print("索引  原子idx 序列号   有效性 时间步      X坐标        Y坐标        Z坐标")
        print("-" * 80)

        for i, point in enumerate(time_points[:sample_size]):
            print(f"{i:4d}  {point['vertexIdx']:6d} {point['vertexNameSN']:6d} "
                  f"{point['valid']:6d} {point['t']:6d}  "
                  f"{point['x']:10.4f}  {point['y']:10.4f}  {point['z']:10.4f}")

        if len(time_points) > sample_size:
            print(f"\n... (省略中间数据) ...")
            print(f"\n最后 {min(sample_size, len(time_points))} 个数据点：")
            print("索引  原子idx 序列号   有效性 时间步      X坐标        Y坐标        Z坐标")
            print("-" * 80)

            start_idx = max(0, len(time_points) - sample_size)
            for i in range(start_idx, len(time_points)):
                point = time_points[i]
                print(f"{i:4d}  {point['vertexIdx']:6d} {point['vertexNameSN']:6d} "
                      f"{point['valid']:6d} {point['t']:6d}  "
                      f"{point['x']:10.4f}  {point['y']:10.4f}  {point['z']:10.4f}")


def extract_coordinates_by_time(time_points, time_step):
    """
    提取特定时间步的所有原子坐标

    Args:
        time_points (list): 轨迹点列表
        time_step (int): 指定的时间步

    Returns:
        list: 该时间步的所有原子坐标
    """
    coordinates = []
    for point in time_points:
        if point['t'] == time_step and point['valid'] == 1:
            coordinates.append({
                'vertexIdx': point['vertexIdx'],
                'vertexNameSN': point['vertexNameSN'],
                'x': point['x'],
                'y': point['y'],
                'z': point['z']
            })

    # 按原子索引排序
    coordinates.sort(key=lambda x: x['vertexIdx'])
    return coordinates


def save_coordinates_to_pdb(coordinates, output_file, time_step=0):
    """
    将坐标保存为PDB格式文件

    Args:
        coordinates (list): 原子坐标列表
        output_file (str): 输出文件名
        time_step (int): 时间步
    """
    with open(output_file, 'w') as f:
        f.write(f"HEADER    PROTEIN STRUCTURE FROM QOMD SIMULATION\n")
        f.write(f"TITLE     TIME STEP {time_step}\n")
        f.write(f"MODEL        1\n")

        for i, coord in enumerate(coordinates):
            # PDB格式的ATOM行
            f.write(f"ATOM  {i + 1:5d}  CA  ALA A{coord['vertexIdx']:4d}    "
                    f"{coord['x']:8.3f}{coord['y']:8.3f}{coord['z']:8.3f}"
                    f"  1.00 20.00           C\n")

        f.write("ENDMDL\n")
        f.write("END\n")


def main():
    """主函数"""
    print("=== QOMD time_point.byte 文件读取工具 ===")

    # 默认文件路径
    default_file = "../DataSet/procedure_24105_OutputData/time_point_linux.byte"

    # 检查命令行参数
    if len(sys.argv) > 1:
        filename = sys.argv[1]
    else:
        filename = default_file

    print(f"正在读取文件: {filename}")

    # 读取文件
    header_info, time_points = read_time_point_file(filename)

    if header_info is None or time_points is None:
        print("读取文件失败")
        return

    # 打印样本数据
    print_sample_data(header_info, time_points)

    # 生成输出文件名
    base_name = os.path.splitext(os.path.basename(filename))[0]
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")

    txt_output = f"{base_name}_results_{timestamp}.txt"
    json_output = f"{base_name}_results_{timestamp}.json"

    # 保存所有结果到txt和json文件
    save_results_to_txt(header_info, time_points, txt_output)
    save_results_to_json(header_info, time_points, json_output)

    # 分析时间步分布
    if time_points:
        time_steps = set(point['t'] for point in time_points)
        print(f"\n时间步分析：")
        print(f"  时间步范围: {min(time_steps)} ~ {max(time_steps)}")
        print(f"  时间步总数: {len(time_steps)}")

        # 提取第0步和最后一步的坐标
        min_time = min(time_steps)
        max_time = max(time_steps)

        print(f"\n提取时间步 {min_time} 的原子坐标...")
        coords_initial = extract_coordinates_by_time(time_points, min_time)
        print(f"时间步 {min_time} 包含 {len(coords_initial)} 个有效原子")

        if max_time != min_time:
            print(f"提取时间步 {max_time} 的原子坐标...")
            coords_final = extract_coordinates_by_time(time_points, max_time)
            print(f"时间步 {max_time} 包含 {len(coords_final)} 个有效原子")

            # 保存为PDB文件
            save_coordinates_to_pdb(coords_initial, f"initial_structure_t{min_time}.pdb", min_time)
            save_coordinates_to_pdb(coords_final, f"final_structure_t{max_time}.pdb", max_time)
            print(f"\n已保存PDB文件:")
            print(f"  初始结构: initial_structure_t{min_time}.pdb")
            print(f"  最终结构: final_structure_t{max_time}.pdb")
        else:
            save_coordinates_to_pdb(coords_initial, f"structure_t{min_time}.pdb", min_time)
            print(f"\n已保存PDB文件: structure_t{min_time}.pdb")

    print(f"\n=== 处理完成 ===")
    print(f"生成的文件:")
    print(f"  - {txt_output}")
    print(f"  - {json_output}")
    if time_points:
        print(f"  - PDB结构文件")


if __name__ == "__main__":
    main()