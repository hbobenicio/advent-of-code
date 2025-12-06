import sys
import itertools
import logging
import os

def run():
    if os.getenv('DEBUG') is not None:
        logging.basicConfig(level=logging.DEBUG) 
    ranges: list[tuple[str,str]] = input_parse()
    print('A:', A(ranges))
    print('B:', B(ranges))

def input_parse() -> list[tuple[str,str]]:
    lines: list[str] = []
    for line in sys.stdin:
        lines.append(line)
    assert len(lines) == 1

    input: str = lines[0]
    ranges: list[tuple[str, str]] = []
    for r in input.split(','):
        fields = r.split('-')
        assert len(fields) == 2
        f1, f2 = fields[0], fields[1]
        ranges.append((f1, f2))
        
    return ranges

def A(rs: list[tuple[str, str]]) -> int:
    sum1, sum2 = 0, 0
    bads: list[int] = []
    for r in rs:
        for x in range(int(r[0]), int(r[1])+1):
            if bad_id(str(x)):
                bads.append(x)
    s = set(bads)

    for x in bads:
        sum1 += x

    for x in s:
        sum2 += x
    return sum2

def B(rs: list[tuple[str, str]]) -> int:
    def bad(val: int | str) -> bool:
        val = str(val)
        for batch_size in range(1, int(len(val) / 2) + 1):
            batches = list(itertools.batched(val, batch_size))
            if len(set(batches)) == 1:
                return True
        return False
    sum = 0
    for r in rs:
        bads = []
        first, last = int(r[0]), int(r[1])
        for x in range(first, last + 1):
            if bad(x):
                sum += x
                bads.append(x)
        logging.debug('range:%s bads:%s', r, bads)
    return sum

def bad_id(ident: str) -> bool:
    l = len(ident)
    if l % 2 != 0:
        return False
    half = int(l / 2)
    return ident[:half] == ident[half:]

if __name__ == '__main__':
    run()
