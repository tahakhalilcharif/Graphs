import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt

graph_data = pd.read_csv('graph.csv')
results_data = pd.read_csv('results.csv')

G = nx.DiGraph()

for _, row in graph_data.iterrows():
    G.add_edge(row['From'], row['To'], weight=row['Weight'])

pos = nx.spring_layout(G)
labels = nx.get_edge_attributes(G, 'weight')
plt.figure(figsize=(8, 6))
nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=500, font_size=10)
nx.draw_networkx_edge_labels(G, pos, edge_labels=labels)
plt.title('Graph Visualization')
plt.savefig('graph.png', format='png')
plt.close()

for _, row in results_data.iterrows():
    source = row['Source']
    destination = row['Destination']
    if row['ShortestPath'] != 'No Path Found':
        path = list(map(int, row['ShortestPath'].split(' -> ')))
        cost = row['Cost']
        print(f"Shortest path from {source} to {destination}: {path} with cost {cost}")
        
        path_edges = list(zip(path, path[1:]))
        plt.figure(figsize=(8, 6))
        nx.draw(G, pos, with_labels=True, node_color='lightgreen', node_size=500, font_size=10)
        nx.draw_networkx_edges(G, pos, edgelist=path_edges, edge_color='red', width=2)
        nx.draw_networkx_edge_labels(G, pos, edge_labels=labels)
        plt.title('Shortest Path Highlighted (DFS)')
        plt.savefig('dfs.png', format='png')
        plt.close()
    else:
        print(f"No path found from {source} to {destination}.")
