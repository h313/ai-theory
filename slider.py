import re
import sys
import hashlib

puzz_list = []


def manhattan(r, c, puzzle):
    target_row = puzzle[r][c] // 4
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
    for i in range(0, 4):
        for it in range(0, 4):
            if puzzle[i][it] == 15:
                return [i, it]


def djikstra(puzzle, prev_cost):
    # Get the current cost of this iteration
    curr_cost = total_cost(puzzle)

    # If this array has already been checked, turn back
    m = hashlib.md5()
    m.update(str(puzzle).encode('utf-8'))
    md5 = m.hexdigest()
    if md5 in puzz_list:
        return False
    else:
        puzz_list.append(md5)

    for i in puzzle:
        print(i)
    print(curr_cost)

    # If it has zero cost, done
    if curr_cost is 0:
        return True
    elif curr_cost > prev_cost:
        return False

    # Now try out each new move
    # Do it up-down
    for i in [-1, 1]:
        space = find_space(puzzle)
        puzz = puzzle[:]
        if 0 <= space[0] + i <= 3:
            # swap out the space and the thing to be changed
            temp = puzz[space[0] + i][space[1]]
            puzz[space[0] + i][space[1]] = puzz[space[0]][space[1]]
            puzz[space[0]][space[1]] = temp

            if djikstra(puzz, curr_cost):
                return True

    # Now do it left-right
    for i in [-1, 1]:
        space = find_space(puzzle)
        puzz = puzzle[:]
        if 0 <= space[1] + i <= 3:
            # swap out the space and the thing to be changed
            temp = puzz[space[0]][space[1] + i]
            puzz[space[0]][space[1] + i] = puzz[space[0]][space[1]]
            puzz[space[0]][space[1]] = temp

            if djikstra(puzz, curr_cost):
                return True

    return False


# Read the file and put it into an array
f = open("puzzle.txt", "r")
inp = f.readlines()
puz = []
for string in inp:
    puz.append(re.findall("[\w']+", string))

for i in range(0, 4):
    for it in range(0, 4):
        puz[i][it] = ord(puz[i][it]) - 65
        # make sure to turn S into #15
        if puz[i][it] is 18:
            puz[i][it] = 15

print(puz)

print(djikstra(puz, sys.maxsize))
print(puzz_list)