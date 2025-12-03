import sys

def run():
    lines: list[str] = []
    for line in sys.stdin:
        lines.append(line)

    print('A:', A(lines))
    print('B:', B(lines))

def A(lines: list[str]):
    count = 0
    position = 50
    for line in lines:
        assert len(line) > 2
        direction = line[0]
        value = int(line[1:])
        match direction:
            case 'L':
                position = posdec(position, value)
            case 'R':
                position = posinc(position, value)
            case _:
                raise Exception(f'unknown direction {direction}')
        if position == 0:
            count += 1
    return count

def B(lines: list[str]):
    count = 0
    position = 50
    for line in lines:
        direction = line[0]
        value = int(line[1:])
        match direction:
            case 'L':
                for x in range(position-1, position - value, -1):
                    if x % 100 == 0:
                        count += 1
                position = posdec(position, value)
            case 'R':
                for x in range(position+1, position + value):
                    if x % 100 == 0:
                        count += 1
                position = posinc(position, value)
            case _:
                raise Exception(f'unknown direction {direction}')
    return A(lines) + count

def posinc(pos: int, val: int) -> int:
    return (pos + val) % 100

def posdec(pos: int, val: int) -> int:
    return (pos - val) % 100

if __name__ == '__main__':
    run()

