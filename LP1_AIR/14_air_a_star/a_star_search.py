class Node:
    def __init__(self, name, h=0):
        self.name = name
        self.parent = None
        self.f, self.g, self.h = 0, 0, h
        self.adjacent_nodes = []

    def __repr__(self):
        return self.name

    def __eq__(self, other):
        return self.name == other.name


n = int(input('Enter number of nodes: '))
nodes = {}
node_names = []

# create nodes
for i in range(n):
    name = input(f'Enter name of node {i+1}: ')
    h = int(input(f'Enter heuristic value of node {i+1}: '))
    nodes[name] = Node(name, h)
    node_names.append(name)

# create edges
for name in node_names:
    while input(f'\nDo you want to add adjacent node for node {name}: ') == 'y':
        adj_name = input(f'Enter name of adjacent node of {name}: ')
        dist = int(input(f'Enter distance between nodes {name} and {adj_name}: '))
        nodes[name].adjacent_nodes.append((nodes[adj_name], dist))

# display graph
print("\nGraph:\n")
for name in node_names:
    print(f'{name}(h={nodes[name].h}) --> {nodes[name].adjacent_nodes}')

start = nodes[input('\nEnter name of start node: ')]
goal = nodes[input('Enter name of goal node: ')]

OPEN = [start]
CLOSED = []
goal_found = False

while OPEN:
    OPEN.sort(key=lambda x: x.f)
    print('\nOPEN list:', OPEN)
    print('CLOSED list:', CLOSED)
    current = OPEN.pop(0)
    CLOSED.append(current)

    # goal found
    if current == goal:
        print('Goal node found!')
        path = [current]
        while current.parent:
            path.append(current.parent)
            current = current.parent
        print('Path:', '->'.join([n.name for n in reversed(path)]))
        goal_found = True
        break

    # explore adjacent nodes
    print(f'Exploring {current}(f={current.f}) --> {current.adjacent_nodes}')
    for node, dist in current.adjacent_nodes:
        if node not in CLOSED:
            new_g = current.g + dist
            new_f = node.h + new_g
            if node in OPEN and new_f < node.f:
                node.parent = current
                node.g = new_g
                node.f = new_f
                print(f'f-value of node {node} = {node.f}')
            elif node not in OPEN:
                node.g = new_g
                node.f = new_g + node.h
                print(f'f-value of node {node} = {node.f}')
                node.parent = current
                OPEN.append(node)

if not goal_found:
    print('\nFAILURE! Goal node not found.')
