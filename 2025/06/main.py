import sys
import logging
import os

def run():
    if os.getenv('DEBUG') is not None:
        logging.basicConfig(level=logging.DEBUG)
        
    # operands, operators = input_parse()
    # print('A:', A(operands, operators))
    expressions = input_parse_b()
    print('B:', B(expressions))

def input_parse() -> tuple[list[list[int]], list[str]]:
    all_fields = []
    for line in sys.stdin:
        line = line.strip()
        fields: list[str] = line.split(' ')
        fields = list(filter(lambda s: len(s) > 0, fields))
        all_fields.append(fields)
    
    # skip the last line
    operands = all_fields[:len(all_fields) -1]
    # and map it to int
    for i in range(len(operands)):
        operands[i] = list(map(int, operands[i]))
        
    operators: list[str] = all_fields[-1]
    
    logging.debug('operands: %s', operands)
    logging.debug('operators: %s', operators)
    
    return operands, operators

def input_parse_b() -> list[list[str]]:
    expressions = []
    
    lines = []
    for line in sys.stdin:
        line = line.strip('\n')
        lines.append(line)
    
    line_len = len(lines[0])
    offset = 0
    for c in range(line_len):
        col = []
        for line in lines:
            col.append(line[c])
        
        # we didnt find a space yet
        if any(map(lambda s: s != ' ', col)):
            continue
        
        # all fields are spaces!
        
        expression = []
        for row in range(len(lines)):
            parsed_line = lines[row][offset:c]
            expression.append(parsed_line)
        expressions.append(expression)
        offset = c + 1
        
    # the loop ends, but we still need to add the last one
    expression = []
    for row in range(len(lines)):
        parsed_line = lines[row][offset:]
        expression.append(parsed_line)
    expressions.append(expression)
        
    return expressions
    
def A(operands: list[list[int]], operators: list[str]) -> int:
    def expr_eval(col: int) -> int:
        operator: str = operators[col]
        match operator:
            case '+':
                total = 0
                for row in range(len(operands)):
                    operand: int = operands[row][col]
                    total += operand
                return total
            
            case '*':
                total = 1
                for row in range(len(operands)):
                    operand: int = operands[row][col]
                    total *= operand
                return total
            
            case _:
                assert(False)
                
    grand_total = 0
    cols = len(operands[0])
    for col in range(cols):
        grand_total += expr_eval(col)
        
    return grand_total

def B(expressions: list[list[str]]) -> int:
    grand_total = 0
    for expr in expressions:
        operator: str = expr[-1].strip()
        operands: list[str] = expr[:len(expr) - 1]
        logging.debug('operator: %s', operator)
        logging.debug('operands: %s', operands)
        match operator:
            case '+':
                expr_value = 0
                oplen = len(operands[0])
                for i in range(oplen):
                    s = ''
                    for operand in operands:
                        s += operand[i]
                    s.replace(' ', '0')
                    expr_value += int(s)
                grand_total += expr_value
                
            case '*':
                expr_value = 1
                oplen = len(operands[0])
                for i in range(oplen):
                    s = ''
                    for operand in operands:
                        s += operand[i]
                    s.replace(' ', '1')
                    expr_value *= int(s)
                grand_total += expr_value
            case _:
                assert(False)
                
    return grand_total

if __name__ == '__main__':
    run()
