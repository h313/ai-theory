import re
import sys
import hashlib
import copy

# This thing stores md5 sums of states visited in the past
puzz_list = []
# "I will take The Ring," he said, "though i do not know the way."
stack = []


# Calculates the manhattan distance of one square
def manhattan(r, c, puzzle):
    target_row = puzzle[r][c] // 4
    target_col = puzzle[r][c] % 4

    d = abs(target_row - r) + abs(target_col - c)
    return d


# Checks the total manhattan distance of one state
def total_cost(puzzle):
    total = 0
    for i in range(0, 4):
        for it in range(0, 4):
            total += manhattan(i, it, puzzle)

    return total


# Finds the indices of the square that's the space
def find_space(puzzle):
    for i in range(0, 4):
        for it in range(0, 4):
            if puzzle[i][it] == 15:
                return [i, it]


# Pure magic
def djikstra(puzzle, prev_cost):
    # Get the current cost of this iteration
    curr_cost = total_cost(puzzle)

    # If this array has already been checked, turn back
    # Using md5 sums to save space
    m = hashlib.md5()
    m.update(str(puzzle).encode('utf-8'))
    md5 = m.hexdigest()
    if md5 in puzz_list:
        return False
    else:
        puzz_list.append(md5)

    # If it has zero cost, done
    if curr_cost is 0:
        stack.append(puzzle)
        return True
    elif curr_cost > prev_cost:
        return False

    # Now try out each new move
    # Do it up-down
    for i in [-1, 1]:
        space = find_space(puzzle)
        puzz = copy.deepcopy(puzzle)
        if 0 <= space[0] + i <= 3:
            # swap out the space and the thing to be changed
            temp = puzz[space[0] + i][space[1]]
            puzz[space[0] + i][space[1]] = puzz[space[0]][space[1]]
            puzz[space[0]][space[1]] = temp

            # This way got to the right solution so add this state onto the thing
            if djikstra(puzz, curr_cost) is True:
                stack.append(puzzle)
                return True

    # Now do it left-right
    for i in [-1, 1]:
        space = find_space(puzzle)
        puzz = copy.deepcopy(puzzle)
        if 0 <= space[1] + i <= 3:
            # swap out the space and the thing to be changed
            temp = puzz[space[0]][space[1] + i]
            puzz[space[0]][space[1] + i] = puzz[space[0]][space[1]]
            puzz[space[0]][space[1]] = temp

            # This way got to the right solution so add this state onto the thing
            if djikstra(puzz, curr_cost) is True:
                stack.append(puzzle)
                return True

    return False


# Read the file and put it into an array
f = open('puzzle.txt', 'r')
inp = f.readlines()
puz = []
for string in inp:
    puz.append(re.findall("[\w']+", string))

# Turn letters into numbers for easier manhattan distance things
for i in range(0, 4):
    for it in range(0, 4):
        # make sure to turn S into #15
        puz[i][it] = 15 if ord(puz[i][it]) - 65 is 18 else ord(puz[i][it]) - 65

# Actually do the thing
djikstra(puz, sys.maxsize)

# Print the finished thing
for puzzle in reversed(stack):
    for row in puzzle:
        temp = ''
        for square in row:
            # m a g i c
            temp += chr(83 if square == 15 else square + 65) + ' '
        print(temp)
    print()
