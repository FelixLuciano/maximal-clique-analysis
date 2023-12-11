#!/usr/bin/python3
import argparse
from pathlib import Path

import networkx as nx


def parse_arguments():
    parser = argparse.ArgumentParser(description='Generate undirected graph with N vertices and density D.')
    
    parser.add_argument("vertices", type=int, help='Number of vertices')
    parser.add_argument("density", type=float, help='Graph density')
    parser.add_argument("-o", type=Path, help='Output file')
    
    args = parser.parse_args()
    
    return args

def main(args):
    graph = nx.fast_gnp_random_graph(args.vertices, args.density)

    args.o.parent.mkdir(parents=True, exist_ok=True)

    with open(args.o, "w") as output:
        output.write(f"{args.vertices} {graph.number_of_edges()}\n")
        output.writelines(f"{x+1} {y+1}\n" for x, y in graph.edges())

    print(f"Generated '{args.o}'")


if __name__ == "__main__" :
    main(parse_arguments())
