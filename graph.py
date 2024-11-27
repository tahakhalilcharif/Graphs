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


plt.figure(figsize=(10, 8))
nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=700, font_size=12, font_weight='bold')
nx.draw_networkx_edge_labels(G, pos, edge_labels=labels, font_color='black', font_size=10)
plt.title('Graph Visualization', fontsize=16)
plt.axis('off')
plt.savefig('graph.png', format='png', bbox_inches='tight')
plt.close()


for _, row in results_data.iterrows():
    source = row['Source']
    destination = row['Destination']
    if row['ShortestPath'] != 'No Path Found':
        path = list(map(int, row['ShortestPath'].split(' -> ')))
        cost = row['Cost']
        print(f"Shortest path from {source} to {destination}: {path} with cost {cost}")
        
        
        path_edges = list(zip(path, path[1:]))
        plt.figure(figsize=(10, 8))
        nx.draw(G, pos, with_labels=True, node_color='lightgreen', node_size=700, font_size=12, font_weight='bold')
        nx.draw_networkx_edges(G, pos, edgelist=path_edges, edge_color='red', width=2, alpha=0.7)
        nx.draw_networkx_edge_labels(G, pos, edge_labels=labels, font_color='black', font_size=10)
        plt.title(f'Shortest Path from {source} to {destination} Highlighted (DFS)', fontsize=16)
        plt.axis('off')
        plt.savefig(f'shortest_path_{source}_to_{destination}.png', format='png', bbox_inches='tight')
        plt.close()
    else:
        print(f"No path found from {source} to {destination}.")