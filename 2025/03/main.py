import sys
import logging
import os

def run():
    if os.getenv('DEBUG') is not None:
        logging.basicConfig(level=logging.DEBUG)
        
    banks: list[str] = input_parse()
    print('A:', A(banks))
    print('B:', B(banks))

def input_parse() -> list[str]:
    lines: list[str] = []
    for line in sys.stdin:
        lines.append(line.strip())
    return lines

def A(banks: list[str]) -> int:
    def joltage_max(bank: str) -> int:
        d1_index = 0
        d1 = int(bank[d1_index])
        for i in range(1, len(bank) - 1):
            x = int(bank[i])
            if x > d1:
                d1 = x
                d1_index = i
        d2: int = max(map(int, bank[d1_index + 1:]))
        return d1 * 10 + d2
    
    sum = 0
    for bank in banks:
        sum += joltage_max(bank)
    return sum

def B(banks: list[str]) -> int:
    def joltage_max(bank: list[int]) -> int:
        offset = 0
        digits = []
        count = 11
        while count >= 0:
            window = bank[offset : len(bank) - count]
            if len(window) == 0: break
            d = -1
            di = -1
            for i in range(0, len(window)):
                x = window[i]
                if x > d:
                    d = x
                    di= i
            digits.append(d)
            offset += di + 1
            count -= 1
        logging.debug('digits: %s', digits)
        return int(''.join(map(str, digits)))
    sum = 0
    for bank in banks:
        ibank: list[int] = list(map(int, bank))
        sum += joltage_max(ibank)
    return sum

if __name__ == '__main__':
    run()
