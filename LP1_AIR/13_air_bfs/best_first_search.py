class Node:
    def __init__(self, name):
        self.name = name
        self.parent = None
        self.adjacent_nodes = []
        self.dist = float('inf')

    def __repr__(self):
        return self.name

    def __eq__(self, other):
        return self.name == other.name


n = int(input("Enter number of nodes: "))
nodes = {}
names = []

# create nodes
for i in range(n):
    name = input(f"Enter name of node {i+1}: ")
    nodes[name] = Node(name)
    names.append(name)

# create edges
for name in names:
    while input(f"\nDo you want to add adjacent node to node {name}: ") == 'y':
        adj_name = input(f"Enter name of node adjacent to {name}: ")
        dist = int(input(f"Enter distance between nodes {name} and {adj_name}: "))
        nodes[name].adjacent_nodes.append((nodes[adj_name], dist))

# display graph
print("\nGraph:\n")
for name in names:
    print(f"{name}: {nodes[name].adjacent_nodes}")

start = nodes[input("\nEnter name of start node: ")]
goal = nodes[input("Enter name of goal node: ")]
start.dist = 0
OPEN = [start]
CLOSED = []
goal_found = False

while OPEN:
    OPEN.sort(key=lambda x: x.dist)
    print("\nPriority queue contents:", OPEN)
    current = OPEN.pop(0)
    CLOSED.append(current)

    # if goal node found
    if current == goal:
        goal_found = True
        cost = current.dist
        print(f"\nGoal node {current} found!")

        # print path
        path_list = [current]
        while current.parent:
            path_list.append(current.parent)
            current = current.parent

        print("Path:", '->'.join([n.name for n in reversed(path_list)]))
        print("Path cost:", cost)
        break

    # explore adjacent nodes
    print(f"Exploring {current}: {current.adjacent_nodes}")
    for node, dist in current.adjacent_nodes:
        if node not in CLOSED:
            # if node in OPEN, select best path
            if node in OPEN and current.dist + dist < node.dist:
                node.parent = current
                node.dist = current.dist + dist
            elif node not in OPEN:
                node.parent = current
                node.dist = current.dist + dist
                OPEN.append(node)

if not goal_found:
    print("FAILURE! No path found.")
