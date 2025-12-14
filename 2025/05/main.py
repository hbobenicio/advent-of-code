import sys
import logging
import os

import operator

Range = tuple[int, int]

def run():
    if os.getenv('DEBUG') is not None:
        logging.basicConfig(level=logging.DEBUG)
        
    ranges, ids = input_parse()
    print('A:', A(ranges, ids))
    print('B:', B(ranges))

def input_parse():
    ranges: list[Range] = []
    for line in sys.stdin:
        line = line.strip()
        if len(line) == 0:
            break
        r: list[str] = line.split('-')
        assert len(r) == 2
        value: Range = (int(r[0]), int(r[1]))
        ranges.append(value)
        
    ids: list[int] = []
    for line in sys.stdin:
        line = line.strip()
        ids.append(int(line))
    
    return ranges, ids

def A(ranges: list[Range], ids) -> int:
    fresh_count = 0
    for x in ids:
        for r in ranges:
            rr = range(r[0], r[1] + 1)
            if x in rr:
                fresh_count += 1
                break
    return fresh_count

def B(ranges: list[Range]) -> int:
    
    def disjoints(r1: Range, r2: Range) -> bool:
        if r2[0] <= r1[0] and r1[0] <= r2[1]:
            return False
        if r2[0] <= r1[1] and r1[1] <= r2[1]:
            return False
        return True
    
    def range_size(r: Range) -> int:
        return r[1] - r[0] + 1
    
    # this simplifies A LOT the problem we need to solve
    ranges.sort(key=operator.itemgetter(0))
    
    selected_ranges: list[Range] = []
    i, j = 0, 1
    while True:
        if i >= len(ranges):
            break
        r1 = ranges[i]
        
        if j >= len(ranges): 
            selected_ranges.append(r1)
            break
        r2 = ranges[j]
        
        # r2 is in r1
        if r2[1] <= r1[1]:
            j += 1
            continue
        
        # r2 is past r1
        if disjoints(r1, r2):
            selected_ranges.append(r1)
            i = j
            j += 1
            continue
        
        # r2 intersects r1
        intersection = (r1[0], r2[0] - 1)
        if intersection[0] > intersection[1]:
            i = j
            j += 1
            continue
        
        selected_ranges.append(intersection)
        i = j
        j += 1
        
    logging.debug(selected_ranges)
    return sum(map(range_size, selected_ranges))

if __name__ == '__main__':
    run()
