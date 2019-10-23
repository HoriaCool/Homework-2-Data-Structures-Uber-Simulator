// Copyright 2019 SD_Homework_Team

#ifndef SOLVER_H_
#define SOLVER_H_

#include <fstream>
#include <utility>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include "./sorted_list.h"
#include "./list_graph.h"
#include "./hashtable.h"
#include "./hash_functions.h"
#define INF 1e6

template <class T>
void swap(T&, T&);

class Driver {
 public:
    int id;
    std::string name;
    enum Status {OFF = false, ON = true} status;
    double rating;
    int node, nr_races, dist;

    friend bool operator==(const Driver &, const Driver &);
    friend bool operator!=(const Driver &, const Driver &);
};

// @return True if lhs < rhs, False otherwise
bool comp_rating(const Driver &, const Driver &);
// @return True if lhs < rhs, False otherwise
bool comp_races(const Driver &, const Driver &);
// @return True if lhs < rhs, False otherwise
bool comp_dist(const Driver &, const Driver &);
// @return True if lhs < rhs, False otherwise
bool comp_uber(const Driver &, const Driver &,
    int, const std::vector<std::vector<int>> &);

class solver {
 private:
    std::vector<std::vector<int>> dist_graph;
    Hashtable<std::string, int> hash_graph;
    ListGraph<std::string> graph;

    Hashtable<std::string, int> hash_driver;
    std::vector<Driver> drivers;

    SortedList<Driver> rating_top;
    SortedList<Driver> races_top;
    SortedList<Driver> dist_top;

 public:
    solver();

    ~solver();

    void task1_solver(std::ifstream&, std::ofstream&);

    void task2_solver(std::ifstream&, std::ofstream&);

    void task3_solver(std::ifstream&, std::ofstream&);

    void task4_solver(std::ifstream&, std::ofstream&);

    void task5_solver(std::ifstream&, std::ofstream&);
};

#endif  // SOLVER_H_
