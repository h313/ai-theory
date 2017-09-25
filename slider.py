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

def find_space(puzzle):
    for row, col in enumerate(puzzle):
        if -1 in col:
            return [row, col]

def djikstra(puzzle, prev_cost):
    # Get the current cost of this iteration
    curr_cost = total_cost(puzzle)
    print(curr_cost)

    # If it has a greater cost, turn back
    if curr_cost < prev_cost:
        return False

    # Now try out each new move
    space = find_space(puzzle)
    # Do it up-down
    for i in [-1, 1]:
        puzz = puzzle
        # swap out the space and the thing to be changed
        temp = puzz[space[0]][space[1]]
        puzz[space[0]][space[1]] = puzz[space[0] + i][space[1]]
        puzz[space[0] + i][space[1]] = temp

        if djikstra(puzz, curr_cost):
            print("uau")
            exit(0)

    # Now do it left-right
    for i in [-1, 1]:
        puzz = puzzle
        # swap out the space and the thing to be changed
        temp = puzz[space[0]][space[1]]
        puzz[space[0]][space[1]] = puzz[space[0]][space[1] + i]
        puzz[space[0]][space[1] + i] = temp

        if djikstra(puzz, curr_cost):
            print("uau")
            exit(0)
