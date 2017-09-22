import Queue

def manhattan(r, c, puzzle):
    target_row = puzzle[r][c] / 4
    target_col = puzzle[r][c] % 4

    d = abs(target_row - r) + abs(target_col - c)
    return d

def total_cost(puzzle):
    total = 0
    for i in range(0, 4):
        for it in range(0, 4):
            total += manhattan(i, it, puzzle)

    return total

def djikstra(puzzle):
    return "todo"