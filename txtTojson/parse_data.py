import json
import re


def parse_txt_to_json(input_file_path, output_file_path):
    """
    读取并解析特定格式的 .txt 文件，将其内容完整转换为 JSON 格式。
    此版本修正了关键的解析逻辑，确保能处理所有数据行。

    Args:
        input_file_path (str): 输入的 .txt 文件路径。
        output_file_path (str): 输出的 .json 文件路径。
    """
    try:
        with open(input_file_path, 'r', encoding='utf-8') as f:
            text_data = f.read()
    except FileNotFoundError:
        print(f"错误: 输入文件未找到 -> '{input_file_path}'")
        return
    except Exception as e:
        print(f"读取文件时发生错误: {e}")
        return

    final_data = {}
    section_counts = {}

    # 按 "=======" 分隔符分割整个文本
    sections = re.split(r'(=======.*?=======)', text_data)
    cleaned_sections = [s.strip() for s in sections if s and s.strip()]

    # 遍历所有部分 (header, content, header, content, ...)
    for i in range(0, len(cleaned_sections), 2):
        header = cleaned_sections[i]
        content = cleaned_sections[i + 1]

        match = re.search(r'=======\s*(.*?)\s*=======', header)
        if not match:
            continue

        original_section_name = match.group(1).replace(" ", "")

        # 处理重名部分
        if original_section_name in section_counts:
            section_counts[original_section_name] += 1
            unique_section_name = f"{original_section_name}_{section_counts[original_section_name]}"
        else:
            section_counts[original_section_name] = 1
            unique_section_name = original_section_name

        section_data = {}
        item_list = []

        lines = content.strip().split('\n')

        # 解析 actItem 和 maxItem
        if lines and '->' in lines[0]:
            properties = re.findall(r'\w+->(\w+)\s*=\s*(\d+)', lines[0])
            for key, value in properties:
                section_data[key] = int(value)
            item_lines = lines[1:]
        else:
            item_lines = lines

        # 逐行解析项目数据
        for line in item_lines:
            line = line.strip()
            if not line:
                continue

            item = {}

            # --- 关键修正 ---
            # 这个正则表达式现在可以正确处理键、等号和值之间的任意空格
            pairs = re.findall(r'(\w+)\s*=\s*([-\.\w]+)', line)

            for key, value in pairs:
                # 尝试转换数据类型
                try:
                    if '.' in value:
                        item[key] = float(value)
                    else:
                        item[key] = int(value)
                except ValueError:
                    item[key] = value

            if item:
                item_list.append(item)

        section_data['items'] = item_list
        final_data[unique_section_name] = section_data

    # 将最终结果写入JSON文件
    try:
        with open(output_file_path, 'w', encoding='utf-8') as f:
            json.dump(final_data, f, indent=4, ensure_ascii=False)
        print(f"成功！文件已转换为JSON格式并保存至: '{output_file_path}'")
    except Exception as e:
        print(f"写入JSON文件时发生错误: {e}")


import os
import glob
import logging
from datetime import datetime

# --- 主程序 ---
if __name__ == "__main__":
    # 指定输入目录和输出目录
    input_directory = '/Users/fay/Code/0719/procedure_24105_OutputData'
    output_directory = './output_json'  # 输出目录

    # 创建输出目录（如果不存在）
    os.makedirs(output_directory, exist_ok=True)

    # 设置日志配置
    log_directory = './logs'
    os.makedirs(log_directory, exist_ok=True)
    log_filename = os.path.join(log_directory, f'batch_process_{datetime.now().strftime("%Y%m%d_%H%M%S")}.log')

    # 配置日志格式
    logging.basicConfig(
        level=logging.INFO,
        format='%(asctime)s - %(levelname)s - %(message)s',
        handlers=[
            logging.FileHandler(log_filename, encoding='utf-8'),
            logging.StreamHandler()  # 同时输出到控制台
        ]
    )

    logger = logging.getLogger(__name__)

    # 开始处理日志
    logger.info("=" * 50)
    logger.info("批量文件处理开始")
    logger.info(f"输入目录: {input_directory}")
    logger.info(f"输出目录: {output_directory}")
    logger.info(f"日志文件: {log_filename}")
    logger.info("=" * 50)

    # 查找目录下所有txt文件
    txt_files = glob.glob(os.path.join(input_directory, '*.txt'))

    # 统计成功和失败的文件数量
    success_count = 0
    failure_count = 0
    failed_files = []

    logger.info(f"找到 {len(txt_files)} 个txt文件待处理")

    # 批量处理
    for i, input_file in enumerate(txt_files, 1):
        # 获取文件名（不含路径和扩展名）
        filename = os.path.basename(input_file)
        name_without_ext = os.path.splitext(filename)[0]

        # 生成对应的json文件名
        output_filename = os.path.join(output_directory, f'{name_without_ext}.json')

        logger.info(f'[{i}/{len(txt_files)}] 正在处理: {filename} -> {os.path.basename(output_filename)}')

        # 异常处理，确保单个文件失败不影响其他文件
        try:
            # 调用转换函数
            parse_txt_to_json(input_file, output_filename)
            success_count += 1
            logger.info(f'✓ 处理成功: {filename}')
        except Exception as e:
            failure_count += 1
            failed_files.append(filename)
            error_msg = f'✗ 处理失败: {filename} - 错误: {str(e)}'
            logger.error(error_msg)
            # 继续处理下一个文件，不中断程序

    # 输出处理结果统计
    logger.info("=" * 50)
    logger.info("批量文件处理完成！")
    logger.info(f"成功处理: {success_count} 个文件")
    logger.info(f"失败处理: {failure_count} 个文件")

    if failed_files:
        logger.info("失败的文件列表:")
        for failed_file in failed_files:
            logger.info(f"  - {failed_file}")

    logger.info(f"日志文件已保存到: {log_filename}")
    logger.info("=" * 50)