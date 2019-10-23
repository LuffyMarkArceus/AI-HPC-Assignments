import numpy as np
import copy


class Node:
    def __init__(self, grid, parent=None):
        self.grid = grid
        self.parent = parent
        self.f, self.h, self.g = 0, 0, 0

    def calc_f(self):
        self.h = (~(self.grid == final)).sum()
        self.g = self.parent.g + 1 if self.parent else 0
        self.f = self.g + self.h

    def __eq__(self, other):
        return (self.grid == other.grid).all()


size = int(input('Enter size of grid: '))
print('Enter initial state of grid:')
initial = np.array([input().split() for _ in range(size)])
assert initial.shape == (size, size)
print('Enter final state of grid:')
final = np.array([input().split() for _ in range(size)])
assert final.shape == (size, size)

initial = Node(initial)
initial.calc_f()

open_node_list = [initial]
close_node_list = []

while open_node_list:
    open_node_list.sort(key=lambda x: x.f)
    current = open_node_list.pop(0)
    close_node_list.append(current)
    print('Current Grid:')
    print(current.grid)
    print('Cost of current grid: f({}) = h({}) + g({})'.format(current.f, current.h, current.g))

    if (current.grid == final).all():
        print('Solution found! Number of moves taken:', current.g)
        break

    row, col = np.argwhere(current.grid == '-')[0]

    print('\nPossible moves:')

    if row != 0:
        child = copy.deepcopy(current)
        child.grid[row - 1][col], child.grid[row][col] = child.grid[row][col], child.grid[row - 1][col]
        child.parent = current
        child.calc_f()
        if child not in close_node_list:
            open_node_list.append(child)
        print('\n', child.grid, 'Cost: f({}) = h({}) + g({})'.format(child.f, child.h, child.g))

    if row != size - 1:
        child = copy.deepcopy(current)
        child.grid[row + 1][col], child.grid[row][col] = child.grid[row][col], child.grid[row + 1][col]
        child.parent = current
        child.calc_f()
        if child not in close_node_list:
            open_node_list.append(child)
        print('\n', child.grid, 'Cost: f({}) = h({}) + g({})'.format(child.f, child.h, child.g))

    if col != 0:
        child = copy.deepcopy(current)
        child.grid[row][col - 1], child.grid[row][col] = child.grid[row][col], child.grid[row][col - 1]
        child.parent = current
        child.calc_f()
        if child not in close_node_list:
            open_node_list.append(child)
        print('\n', child.grid, 'Cost: f({}) = h({}) + g({})'.format(child.f, child.h, child.g))

    if col != size - 1:
        child = copy.deepcopy(current)
        child.grid[row][col + 1], child.grid[row][col] = child.grid[row][col], child.grid[row][col + 1]
        child.parent = current
        child.calc_f()
        if child not in close_node_list:
            open_node_list.append(child)
        print('\n', child.grid, 'Cost: f({}) = h({}) + g({})'.format(child.f, child.h, child.g))

    print('-' * 50)
