## Part 3: Tabu Search Algorithm

In this part of the project, we will implement the Tabu Search algorithm. The Tabu Search algorithm is a metaheuristic search algorithm that guides a local search procedure to explore the solution space beyond local optimality. It uses memory structures to avoid cycles and to keep track of previously visited solutions.

The algorithm is particularly useful for solving combinatorial optimization problems, such as the Traveling Salesman Problem (TSP), job scheduling, and vehicle routing problems.

Here we build the Tabu Search algorithm for the backpack problems. The backpack problem is a classic optimization problem where the goal is to maximize the total value of items that can be carried in a backpack with a limited weight capacity. Each item has a weight and a value, and the objective is to select a subset of items that maximizes the total value without exceeding the weight limit.

The Tabu Search algorithm is implemented in the `src` folder. `TabuSearch` takes as input the location of the test data file, tabu duration, how many iterations to perform, and output interval.

To test the algorithm, we will use the provided tabu search program in `./tabu_lsg`. We can run both programs on provided data sets in the folder `./data`.

In the files with the test data, the first line contains the number of items. The second line contains the maximum permissible total mass of the backpack. All subsequent lines consist of the weight and mass of the items. There are three files: data.1, data.2 and data.3

## To run the code
First make sure you are in the correct directory:
```bash
cd Part_3
```
Use the following command to compile and run the test program:
```bash
make
./tabu
```
Use the following command to delete the generated files:
```bash
make clean
```