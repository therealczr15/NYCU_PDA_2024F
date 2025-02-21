# PDA Lab1 - Corner Stitching

## Overview
This project implements the ***Corner Stitching*** data structure for efficient spatial data management. <br>
The goal is to organize space tiles as maximal horizontal stripes, ensuring no adjacent space tiles exist on the left or right while maximizing tile width.

For more detailed problem description: [2024PDA_Lab1.pdf](2024PDA_Lab1.pdf)

## Implementation
This project provides the following core functionalities:
- Point Finding - Given a coordinate (x, y), efficiently determine which tile covers that position.
- Block Insertion – Dynamically insert blocks while maintaining adjacency relationships.
- Enumerate All Tiles – Traverse and output all tiles in the corner stitching structure.

## Results
| layout4.png                      | layout5.png                      |
|:--------------------------------:|:--------------------------------:|
| ![Layout 4](drawing/layout4.png) | ![Layout 5](drawing/layout5.png) |

| layout6.png                      | layout7.png                      |
|:--------------------------------:|:--------------------------------:|
| ![Layout 6](drawing/layout6.png) | ![Layout 7](drawing/layout7.png) |
