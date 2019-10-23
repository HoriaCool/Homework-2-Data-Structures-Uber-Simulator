// Copyright 2019 Nedelcu Horia (nedelcu.horia.alexandru@gmail.com)

#include <utility>
#include <string>
#include <vector>
#include <list>
#include "./solver.h"

template <class T>
void swap(T& a, T& b) {
    T c(std::move(a));
    a = std::move(b);
    b = std::move(c);
}

bool operator==(const Driver &lhs, const Driver &rhs) {
    return lhs.id == rhs.id;
}

bool operator!=(const Driver &lhs, const Driver &rhs) {
    return lhs.id != rhs.id;
}

bool comp_rating(const Driver &lhs, const Driver &rhs) {
    if (!lhs.nr_races && rhs.nr_races) {
        return true;
    }

    if (lhs.nr_races && !rhs.nr_races) {
        return false;
    }

    if (!lhs.nr_races && !rhs.nr_races) {
        return lhs.name > rhs.name;
    }

    if (lhs.rating / lhs.nr_races < rhs.rating / rhs.nr_races) {
        return true;
    }

    if (lhs.rating / lhs.nr_races == rhs.rating / rhs.nr_races) {
        return lhs.name > rhs.name;
    }

    return false;
}

bool comp_races(const Driver &lhs, const Driver &rhs) {
    if (lhs.nr_races < rhs.nr_races) {
        return true;
    } else if (lhs.nr_races == rhs.nr_races) {
        return lhs.name > rhs.name;
    }

    return false;
}

bool comp_dist(const Driver &lhs, const Driver &rhs) {
    if (lhs.dist < rhs.dist) {
        return true;
    } else if (lhs.dist == rhs.dist) {
        return lhs.name > rhs.name;
    }

    return false;
}

bool comp_uber(const Driver &lhs, const Driver &rhs,
    int src, const std::vector<std::vector<int>> &dist) {
    if (lhs.status < rhs.status) {
        return true;
    } else if (lhs.status == rhs.status &&
               dist[rhs.node][src] != -1 &&
               (dist[lhs.node][src] == -1 ||
                dist[lhs.node][src] > dist[rhs.node][src])) {
        return true;
    } else if (lhs.status == rhs.status &&
               dist[lhs.node][src] == dist[rhs.node][src]) {
        return comp_rating(lhs, rhs);
    }

    return false;
}

solver::solver(): dist_graph(),
    hash_graph(PRIME_CAPACITY_FOR_HASH, string_hash), graph(0),
	hash_driver(PRIME_CAPACITY_FOR_HASH, string_hash), drivers(),
	rating_top(comp_rating), races_top(comp_races), dist_top(comp_dist) {}

solver::~solver() {}

void solver::task1_solver(std::ifstream& fin, std::ofstream& fout) {
    int i, n, m, src, dst, q1;
	std::string str;

	fin >> n >> m;
	graph.setSize(n);
    dist_graph = std::vector<std::vector<int>>(n);

	for (i = 0; i < n; ++i) {
		fin >> str;
        graph.addInfo(i, str);
		hash_graph.set(str, i);
	}

	for (i = 0; i < m; ++i) {
		fin >> str;
		src = hash_graph[str];
		fin >> str;
		dst = hash_graph[str];

		graph.addEdge(src, dst);
	}

    fin >> q1;

    for (i = 0; i < q1; ++i) {
        fin >> str;
        src = hash_graph[str];
        fin >> str;
        dst = hash_graph[str];

        fout << (graph.pathFrom(src, dst)? "y\n": "n\n");
    }
}

void solver::task2_solver(std::ifstream& fin, std::ofstream& fout) {
	int i, src, dst, q2;
    std::string str;

    fin >> q2;

    for (i = 0; i < q2; ++i) {
        fin >> str;
        src = hash_graph[str];
        fin >> str;
        dst = hash_graph[str];

        fout << graph.distFrom(src, dst) << '\n';
    }
}

void solver::task3_solver(std::ifstream& fin, std::ofstream& fout) {
	int i, q3, type, a, b, c, dist_ac, dist_cb;
    bool edge_ab, edge_ba;
    std::string str;
    char q_type;

    fin >> q3;

    for (i = 0; i < q3; ++i) {
        fin >> q_type;

        fin >> str;
        a = hash_graph[str];
        fin >> str;
        b = hash_graph[str];

        fin >> type;

        if (q_type == 'c') {
            switch (type) {
                case 0:
                    graph.addEdge(a, b);
                    break;
                case 1:
                    graph.removeEdge(a, b);
                    graph.removeEdge(b, a);
                    break;
                case 2:
                    graph.addEdge(a, b);
                    graph.addEdge(b, a);
                    break;
                default:
                    edge_ab = graph.hasEdge(a, b);
                    edge_ba = graph.hasEdge(b, a);

                    if (edge_ab && !edge_ba) {
                        graph.addEdge(b, a);
                        graph.removeEdge(a, b);
                    }

                    if (!edge_ab && edge_ba) {
                        graph.addEdge(a, b);
                        graph.removeEdge(b, a);
                    }
            }
        } else {
            switch (type) {
                case 0:
                    fout << (graph.pathFrom(a, b)? "y\n": "n\n");
                    break;
                case 1:
                    fout << graph.distFrom(a, b) << '\n';
                    break;
                default:
                    fin >> str;
                    c = hash_graph[str];

                    dist_ac = graph.distFrom(a, c);
                    dist_cb = graph.distFrom(c, b);

                    if (dist_ac != -1 && dist_cb != -1) {
                        fout << dist_ac + dist_cb << '\n';
                    } else {
                        fout << -1 << '\n';
                    }
            }
        }
    }

    for (i = 0; i < graph.getSize(); ++i) {
        dist_graph[i] = graph.getDistNodes(i);
    }
}

void solver::task4_solver(std::ifstream& fin, std::ofstream& fout) {
	int i, k, q3, src, dst, index_driver, node, index_uber, nr_drivers;
    std::vector<int> neighbors_dst;
    std::list<Driver> list_top;
    std::string str1, str2;
    Driver new_driver;
    double rating;

    fin >> q3;

    for (i = 0; i < q3; ++i) {
        fin >> str1;

        if (str1 == "d") {
            // read driver and location names
            fin >> str1 >> str2;

            // location index
            node = hash_graph[str2];

            if (hash_driver.lookup(str1)) {
                index_driver = hash_driver[str1];

                drivers[index_driver].status = Driver::Status::ON;
                drivers[index_driver].node = node;
            } else {
                hash_driver.set(str1, drivers.size());

                new_driver.id = drivers.size();
                new_driver.name = str1;
                new_driver.status = Driver::Status::ON;
                new_driver.node = node;
                new_driver.rating = 0;
                new_driver.nr_races = 0;
                new_driver.dist = 0;

                drivers.push_back(new_driver);

                rating_top.insertInOrder(new_driver);
        		races_top.insertInOrder(new_driver);
        		dist_top.insertInOrder(new_driver);
            }
        } else if (str1 == "b") {
            // read driver name
            fin >> str1;

            index_driver = hash_driver[str1];
            drivers[index_driver].status = Driver::Status::OFF;
        } else if (str1 == "r") {
            // read start and end locations names; rating given by client
            fin >> str1 >> str2 >> rating;

            if (!drivers.size()) {
                fout << "Soferi indisponibili\n";
                continue;
            }

            src = hash_graph[str1];
            dst = hash_graph[str2];

            index_uber = 0;
            for (unsigned int j = 1; j < drivers.size(); ++j) {
                if (comp_uber(drivers[index_uber], drivers[j],
                    src, dist_graph)) {
                    index_uber = j;
                }
            }

            if (drivers[index_uber].status == Driver::Status::OFF ||
                dist_graph[drivers[index_uber].node][src] == -1) {
                fout << "Soferi indisponibili\n";
                continue;
            }

            if (dist_graph[src][dst] == -1) {  // Can't access destination
                neighbors_dst = graph.getNeighbors(dst);

                for (unsigned int j = 0; j < neighbors_dst.size(); ++j) {
                    if (dist_graph[src][neighbors_dst[j]] != -1) {
                        dst = neighbors_dst[j];
                        break;
                    }
                }
            }

            if (dist_graph[src][dst] == -1) {
                fout << "Destinatie inaccesibila\n";
                continue;
            }

            drivers[index_uber].rating += rating;
            drivers[index_uber].nr_races++;

            drivers[index_uber].dist +=
            dist_graph[drivers[index_uber].node][src] + dist_graph[src][dst];

            drivers[index_uber].node = dst;

            rating_top.remove(drivers[index_uber]);
        	races_top.remove(drivers[index_uber]);
        	dist_top.remove(drivers[index_uber]);

            rating_top.insertInOrder(drivers[index_uber]);
        	races_top.insertInOrder(drivers[index_uber]);
        	dist_top.insertInOrder(drivers[index_uber]);
        } else if (str1 == "top_rating") {
            fin >> nr_drivers;

            nr_drivers = (nr_drivers < rating_top.getSize())?
                          nr_drivers: rating_top.getSize();

            k = 0;
            list_top = rating_top.getList();
            for (auto it = list_top.begin();
                it != list_top.end() && k < nr_drivers; ++it, ++k) {
            	rating = ((*it).nr_races? (*it).rating / (*it).nr_races: 0.0);

                fout << (*it).name << ':'
                     << std::fixed << std::setprecision(3) << rating << ' ';
            }
            fout << '\n';
        } else if (str1 == "top_dist") {
            fin >> nr_drivers;

            nr_drivers = (nr_drivers < dist_top.getSize())?
                          nr_drivers: dist_top.getSize();

            k = 0;
            list_top = dist_top.getList();
            for (auto it = list_top.begin();
                it != list_top.end() && k < nr_drivers; ++it, ++k) {
                fout << (*it).name << ':'
                     << (*it).dist << ' ';
            }
            fout << '\n';
        } else if (str1 == "top_rides") {
            fin >> nr_drivers;

            nr_drivers = (nr_drivers < races_top.getSize())?
                          nr_drivers: races_top.getSize();

            k = 0;
            list_top = races_top.getList();
            for (auto it = list_top.begin();
                it != list_top.end() && k < nr_drivers; ++it, ++k) {
                fout << (*it).name << ':'
                     << (*it).nr_races << ' ';
            }
            fout << '\n';
        } else {
            // read driver name
            fin >> str1;

            index_driver = hash_driver[str1];
            rating = (drivers[index_driver].nr_races?
                      drivers[index_driver].rating /
                      drivers[index_driver].nr_races: 0.0);

            fout << str1 << ": "
                 << graph.getInfo(drivers[index_driver].node) << ' '
                 << std::fixed << std::setprecision(3) << rating << ' '
                 << drivers[index_driver].nr_races << ' '
                 << drivers[index_driver].dist << ' '
                 << (drivers[index_driver].status? "online\n": "offline\n");
        }
    }
}

void solver::task5_solver(std::ifstream& fin, std::ofstream& fout) {
	int i, j, nr_intersections, combustible, index_driver, src, dst, distance;
	std::vector<int> dist_comb(graph.getSize(), INF),
                     nodes_perm(graph.getSize());
	std::string str;

	fin >> combustible >> str;
	index_driver = hash_driver[str];
	src = drivers[index_driver].node;

	fin >> nr_intersections;

	for (i = 0; i < nr_intersections; ++i) {
		fin >> str;
		dst = hash_graph[str];
		distance = dist_graph[src][dst];

		if (distance != -1 && distance <= combustible) {
			dist_comb[dst] = distance;
		}
	}

	for (i = 0; i < graph.getSize(); ++i) {
		nodes_perm[i] = i;
	}

	for (i = 0; i < graph.getSize(); ++i) {
		for (j = i + 1; j < graph.getSize(); ++j) {
			if (dist_comb[i] > dist_comb[j]) {
				swap(dist_comb[i], dist_comb[j]);
				swap(nodes_perm[i], nodes_perm[j]);
			}

			if (dist_comb[i] == dist_comb[j] &&
                graph.getInfo(nodes_perm[i]) > graph.getInfo(nodes_perm[j])) {
				swap(dist_comb[i], dist_comb[j]);
				swap(nodes_perm[i], nodes_perm[j]);
			}
		}
	}

	for (i = 0; i < graph.getSize(); ++i) {
		if (dist_comb[i] != INF) {
			fout << graph.getInfo(nodes_perm[i]) << ' ';
		} else {
			break;
		}
	}
}
