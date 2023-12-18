from pprint import pprint

def done(states: list[str]):
    for s in states:
        if not s.endswith('Z'):
            return False
    return True

def igen(instructions: str):
    i = 0
    is_len = len(instructions)
    while True:
        yield instructions[i]
        i = (i + 1) % is_len

if __name__ == '__main__':
    instructions = input()
    input()

    network: dict[str, tuple[str,str]] = {}
    while True:
        try:
            line = input()
        except EOFError:
            break

        key, left, right = filter(lambda s: s != '=', line.split())
        key, left, right = key, left[1:][:-1], right[:-1]
        network[key] = (left, right)

    state = [k for k in network.keys() if k[-1] == 'A']
    counters = {}
    for s in state:
        igenerator = igen(instructions)
        count = 0
        while not s.endswith('Z'):
            count += 1
            i = next(igenerator)
            if i == 'L':
                s = network[s][0]
            elif i == 'R':
                s = network[s][1]
            else:
                assert False
        print(f'{count=}')
        counters[s] = count
    pprint(counters)

