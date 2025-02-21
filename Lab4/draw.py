import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as mpatches
import matplotlib.lines as mlines
import sys

def read_data(file_path):
    routing_area = None
    chips = []
    bumps_chip1 = []
    bumps_chip2 = []
    connections = []

    with open(file_path, 'r') as f:
        for line in f:
            data = line.split()
            if data[0] == "RoutingArea":
                routing_area = list(map(int, data[1:]))
            elif data[0] == "Chip1" or data[0] == "Chip2":
                chips.append((data[0], list(map(int, data[1:]))))
            elif data[0] == "BumpChip1":
                bumps_chip1.append(tuple(map(int, data[1:])))
            elif data[0] == "BumpChip2":
                bumps_chip2.append(tuple(map(int, data[1:])))
            elif data[0] == "Connection":
                connections.append([tuple(map(int, data[i:i+2])) for i in range(1, len(data), 2)])

    return routing_area, chips, bumps_chip1, bumps_chip2, connections

def plot_data(file_path, output_image):
    routing_area, chips, bumps_chip1, bumps_chip2, connections = read_data(file_path)
    fig, ax = plt.subplots(figsize=(8, 6))

    x1, y1, x2, y2 = routing_area
    ax.plot([x1, x2, x2, x1, x1], [y1, y1, y2, y2, y1], color='blue', linewidth=2, label="Routing Area")

    for chip, rect in chips:
        x1, y1, x2, y2 = rect
        linestyle = "dashed"
        ax.plot([x1, x2, x2, x1, x1], [y1, y1, y2, y2, y1], color='green', linewidth=2, linestyle=linestyle, label=chip)

    ax.scatter(*zip(*bumps_chip1), color='red', label="Bumps Chip 1", marker='o', s=5)
    ax.scatter(*zip(*bumps_chip2), color='purple', label="Bumps Chip 2", marker='o', s=5)

    colors = plt.cm.get_cmap("tab10", len(connections))
    for i, path in enumerate(connections):
        x_vals, y_vals = zip(*path)
        ax.plot(x_vals, y_vals, color=colors(i), linewidth=1.0)

    routing_patch = mpatches.Patch(facecolor='none', edgecolor='blue', linewidth=2, label="Routing Area")
    chip1_patch = mpatches.Patch(facecolor='none', edgecolor='green', linestyle='dashed', linewidth=2, label="Chip 1")
    chip2_patch = mpatches.Patch(facecolor='none', edgecolor='green', linestyle='dashed', linewidth=2, label="Chip 2")
    bump_chip1_marker = mlines.Line2D([], [], color='red', marker='o', linestyle='None', markersize=3, label="Bumps Chip 1")
    bump_chip2_marker = mlines.Line2D([], [], color='purple', marker='o', linestyle='None', markersize=3, label="Bumps Chip 2")

    ax.legend(handles=[routing_patch, chip1_patch, chip2_patch, bump_chip1_marker, bump_chip2_marker], loc="upper left", fontsize=8)
    
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.grid(True)
    plt.title("Routing Area, Chips, and Routed Connections")

    plt.savefig(output_image, dpi=300)
    plt.close()

if __name__ == "__main__":    
    input_file = sys.argv[1]
    output_image = sys.argv[2].strip()
    plot_data(input_file, output_image)

