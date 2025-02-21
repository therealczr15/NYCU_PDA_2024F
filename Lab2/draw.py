import sys
import matplotlib.pyplot as plt
import math
from matplotlib.patches import Patch

def read_floorplan(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
    
    outline_w, outline_h = map(int, lines[0].strip().split()[:2])
    blocks = []

    for line in lines[1:]:
        parts = line.strip().split()
        name = parts[0]
        x1, y1, x2, y2 = map(int, parts[1:])
        blocks.append((name, x1, y1, x2, y2))

    return outline_w, outline_h, blocks

def plot_floorplan(file_path, output_image):
    outline_w, outline_h, blocks = read_floorplan(file_path)
    total_area = outline_w * outline_h 
    
    fig, ax = plt.subplots(figsize=(10, 10))
    ax.set_xlim(0, outline_w)
    ax.set_ylim(0, outline_h)
    ax.set_xticks(range(0, outline_w + 1, max(1, outline_w // 10)))
    ax.set_yticks(range(0, outline_h + 1, max(1, outline_h // 10)))
    ax.set_aspect('equal')
    ax.grid(True, linestyle='--', alpha=0.6)

    min_x = min(block[1] for block in blocks)
    min_y = min(block[2] for block in blocks)
    max_x = max(block[3] for block in blocks)
    max_y = max(block[4] for block in blocks)
    
    for name, x1, y1, x2, y2 in blocks:
        width, height = x2 - x1, y2 - y1
        rect = plt.Rectangle((x1, y1), width, height, edgecolor='black', facecolor='lightblue', lw=1)
        ax.add_patch(rect)
        font_size = min(width, height) / math.sqrt(total_area) * 160    
        ax.text(x1 + width / 2, y1 + height / 2, name, fontsize=font_size, ha='center', va='center', color='black')
    
    outline = plt.Rectangle((0, 0), outline_w, outline_h, edgecolor='red', facecolor='none', lw=3, label='Fixed Outline')
    ax.add_patch(outline)
    
    
    floorplan = plt.Rectangle((min_x, min_y), max_x - min_x, max_y - min_y, edgecolor='green',  facecolor='none', lw=3, linestyle='dashed')
    ax.add_patch(floorplan)
    
    legend_elements = [
        Patch(facecolor='none', edgecolor='red', lw=2, label='Fixed Outline'),
        Patch(facecolor='none', edgecolor='green', lw=2, linestyle='dashed', label='Floorplan Area')
    ]
    
    ax.legend(handles=legend_elements, loc='upper right', bbox_to_anchor=(1.1, 1.1))
    
    plt.xlabel("X", fontsize=12)
    plt.ylabel("Y", fontsize=12)
    plt.title("Fixed-Outline Floorplan", fontsize=14)
    plt.xticks(fontsize=10)
    plt.yticks(fontsize=10)
    
    plt.savefig(output_image, dpi=300, bbox_inches='tight')
    plt.close()

if __name__ == "__main__":
    input_file = sys.argv[1]
    output_file = sys.argv[2].strip() 
    plot_floorplan(input_file, output_file)
