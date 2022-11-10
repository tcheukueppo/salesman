# TSP - The Salesman Problem

Experimenting multiprocessing systems via the salesman problem, we use the naive
approach(brute force technique)to solve this problem but we adapt our solution
to different types of architectures and techniques, which are

[Check here for details on these approach](https://github.com/tcheukueppo/salesman/tree/master/docs_jpg)

## Sequential processing

Branch and bound method using brute force, do a depth-first search.

### build

```bash
make salesman_seq
```
### Usage

```bash
salesman_seq <starting_vertex> <path/to/graph/file>
```

#### Format of the input file

```
<number of vertices>
<starting_vertex><space><incident_vertex><space><weight>
<starting_vertex><space><incident_vertex><space><weight>
...
```

### Example

```bash
salesman_seq 1 ./graphs/g1
```

## Threads

## Distributed computing
