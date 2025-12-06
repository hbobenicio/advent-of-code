import sys
import logging
import os

# OBS what I call by `col` and `row` are swapped, but I'm too lazy now to change it now...

def run():
    if os.getenv('DEBUG') is not None:
        logging.basicConfig(level=logging.DEBUG)
        
    grid: list[str] = input_parse()
    print('A:', A(grid))
    print('B:', B(grid))

def input_parse() -> list[str]:
    lines: list[str] = []
    for line in sys.stdin:
        lines.append(line.strip())
    return lines

def A(grid: list[str]) -> int:
    def gen_neighbor_indexes(pos: tuple[int, int]):
        for i in range(-1, 2):
            for j in range(-1, 2):
                if i == 0 and j == 0:
                    continue
                
                px = pos[0] + i
                py = pos[1] + j
                
                if px < 0 or py < 0:
                    continue
                
                if px >= len(grid) or py >= len(grid):
                    continue
                
                yield (px, py)
    
    def gen_neighbors(pos: tuple[int, int]):
        for neighbor_index in gen_neighbor_indexes(pos):
            col = neighbor_index[0]
            row = neighbor_index[1]
            yield grid[col][row]
            
    def count_adjacent_rolls(pos: tuple[int, int]) -> int:
        count = 0
        for neighbor in gen_neighbors(pos):
            if neighbor == '@':
                count += 1
        return count
    
    def forklift_can_access_count() -> int:
        count = 0
        for col in range(0, len(grid)):
            for row in range(0, len(grid[col])):
                x = grid[col][row]
                if x == '@':
                    if count_adjacent_rolls((col, row)) < 4:
                        count += 1
        return count
    return forklift_can_access_count()

def B(grid: list[str]) -> int:
    def gen_neighbor_indexes(pos: tuple[int, int]):
        for i in range(-1, 2):
            for j in range(-1, 2):
                if i == 0 and j == 0:
                    continue
                
                px = pos[0] + i
                py = pos[1] + j
                
                if px < 0 or py < 0:
                    continue
                
                if px >= len(grid) or py >= len(grid):
                    continue
                
                yield (px, py)
    
    def gen_neighbors(pos: tuple[int, int]):
        for neighbor_index in gen_neighbor_indexes(pos):
            col = neighbor_index[0]
            row = neighbor_index[1]
            yield grid[col][row]
            
    def count_adjacent_rolls(pos: tuple[int, int]) -> int:
        count = 0
        for neighbor in gen_neighbors(pos):
            if neighbor == '@':
                count += 1
        return count
    
    def forklift_can_access_count() -> int:
        count = 0
        to_be_removed: list[tuple[int, int]] = []
        for col in range(0, len(grid)):
            for row in range(0, len(grid[col])):
                x = grid[col][row]
                if x == '@':
                    if count_adjacent_rolls((col, row)) < 4:
                        count += 1
                        to_be_removed.append((col, row))
        for pos in to_be_removed:
            col, row = pos[0], pos[1]
            grid[col] = grid[col][:row] + '.' + grid[col][row+1:]
        return count
    
    total = 0
    while (count := forklift_can_access_count()) > 0:
        total += count
    return total

if __name__ == '__main__':
    run()
