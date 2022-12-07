# TSP - The Salesman Problem

Experimenting multiprocessing systems via the salesman problem, we use the naive
approach(brute force technique)to solve this problem but we adapt our solution
to different types of architectures and techniques, which are

[Find more documentation here!](https://github.com/tcheukueppo/salesman/tree/master/docs_jpg)

# Build instructions

Build with

```bash
make
```

# Documentation

Try `salesman -h` for a short description

```bash
-t NTHREADS     specify the number of threads.
-d DEBUG_FLAG   enable debugging.
-g NUM_VERTICES specify the number of vertices(for random generation only).
-f GRAPH_FILE   load the graph from a file.
-h              display this help message.
-v              print current version.
```

# Acceleration graphs

To generate the graphs you see below, use `plotter`, try `plotter -h` for more information.

```bash
usage:
   plotter [ -h ] [ -n NUM_NODES ] [ -g GRAPH_FILE ] [ -n NTHREAD_LIMIT ] [ -p GRAPH ]

   -h:               display this help message.
   -n NUM_NODES:     the maximum numbers of nodes to generate the graph.
   -d DATA_FILE:     the maximum numbers of nodes to generate the graph.
   -g GRAPH_FILE:    path to store the generated graph.
   -t NTHREAD_LIMIT: benchmark for from 2 to NTHREAD_LIMIT threads.
   -p GRAPH:         specify the path to the generate graph(not to be 
                     confused with the graph DATA STRUCTURE).
```

# Statistics

A of GPU TIME against the NUMBER OF THREADS

![](./plots/test_8/graph.png)

**For 8 vertices**

![](./plots/test_10/graph.png)

**For 10 vertices**

![](./plots/test_6/graph.png)

**For 6 vertices**
