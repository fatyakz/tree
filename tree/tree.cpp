#include <iostream>
#include <iomanip>
#include <vector>
#include <random>

struct s_cell {
    int num;
    int sum;
    bool start;
};

int main()
{
    int height = 5;
    int width = 5;
    int num_starts = 4;
    int target = 4;
    bool ones = false;

start:
    std::cout << "Target:";
    std::cin >> target;

    std::vector<s_cell> col;
    std::vector<std::vector<s_cell>> cells;
    std::vector<std::vector<std::vector<s_cell>>> grid;
    std::vector<std::vector<std::vector<std::vector<s_cell>>>> n;
    
    s_cell e_cell;
    e_cell.num = 0;
    e_cell.sum = 0;
    e_cell.start = false;
    

    for (int x = 0; x < width; x++) {
        col.push_back(e_cell);
    }
    for (int y = 0; y < height; y++) {
        cells.push_back(col);
    }

    grid.push_back(cells);
    n.push_back(grid);

    // create starts
    if (ones) {
        int placed = 0;
        srand(time(NULL));
        for (int i = 0; i < height * width; i++) {
            int sx = (rand() % (width - 2)) + 1;
            int sy = (rand() % (height - 2)) + 1;
            if (n[0][0][sx][sy].num < 1) {
                n[0][0][sx][sy].num = 1;
                n[0][0][sx][sy].start = true;
                placed++;
            }
            if (placed >= num_starts) {
                break;
            }
        }
    }
    else {
        int placed = 0;
        int next_place = 1;
        srand(time(NULL));
        for (int i = 0; i < height * width; i++) {
            int sx = (rand() % (width - 2)) + 1;
            int sy = (rand() % (height - 2)) + 1;
            if (n[0][0][sx][sy].num < 1) {
                n[0][0][sx][sy].num = next_place;
                n[0][0][sx][sy].start = true;
                placed++;
                next_place++;
            }
            if (placed >= num_starts) {
                break;
            }
        }
    }

    std::cout << "Start:\n";
    for (int x = 0; x < n[0][0].size(); x++) {
        for (int y = 0; y < n[0][0].size(); y++) {
            std::cout << n[0][0][x][y].num << ",";
        }
        std::cout << "\n";
    } 
    std::cout << "\n";

    std::cout << "Sums:\n";
    for (int x = 0; x < n[0][0].size(); x++) {
        for (int y = 0; y < n[0][0].size(); y++) {
            std::cout << n[0][0][x][y].sum << ",";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    // core loop
    for (int nid = 0; nid < n.size(); nid++) {
        // expand edges
        for (int gid = 0; gid < n[nid].size(); gid++) {
            // top row
            for (int x = 0; x < n[nid][gid].size(); x++) {
                if (n[nid][gid][x][0].num > 0 || n[nid][gid][x][0].sum > 0) {
                    for (int rx = 0; rx < n[nid][gid].size(); rx++) {
                        n[nid][gid][rx].insert(n[nid][gid][rx].begin(), e_cell);
                    }
                    break;
                }
            }
            // bottom row
            for (int x = 0; x < n[nid][gid].size(); x++) {
                if (n[nid][gid][x][n[nid][gid][x].size()-1].num > 0 ||
                    n[nid][gid][x][n[nid][gid][x].size()-1].sum > 0) {
                    for (int rx = 0; rx < n[nid][gid].size(); rx++) {
                        n[nid][gid][rx].push_back(e_cell);
                    }
                    break;
                }
            }
            // left side
            for (int y = 0; y < n[nid][gid][0].size(); y++) {
                if (n[nid][gid][0][y].num > 0 || n[nid][gid][0][y].sum > 0) {
                    col.clear();
                    for (int ry = 0; ry < n[nid][gid][0].size(); ry++) {
                        col.push_back(e_cell);
                    }
                    n[nid][gid].insert(n[nid][gid].begin(), col);
                    break;
                }
            }
            // right side
            for (int y = 0; y < n[nid][gid][0].size(); y++) {
                if (n[nid][gid][n[nid][gid].size()-1][y].num > 0 ||
                    n[nid][gid][n[nid][gid].size()-1][y].sum > 0) {
                    col.clear();
                    for (int ry = 0; ry < n[nid][gid][0].size(); ry++) {
                        col.push_back(e_cell);
                    }
                    n[nid][gid].push_back(col);
                    break;
                }
            }
        }
        
        // build sums
        for (int gid = 0; gid < n[nid].size(); gid++) {
            for (int x = 0; x < n[nid][gid].size(); x++) {
                for (int y = 0; y < n[nid][gid][x].size(); y++) {
                    if (n[nid][gid][x][y].num == 0) {
                        for (int sx = x-1; sx < x+2; sx++) {
                            for (int sy = y-1; sy < y+2; sy++) {
                                if (n[nid][gid].size() > sx && n[nid][gid][x].size() > sy) {
                                    n[nid][gid][x][y].sum += n[nid][gid][sx][sy].num;
                                }
                            }
                        }
                    }
                }
            }
        }

        // build grids for each sum == n + 1
        for (int gid = 0; gid < n[nid].size(); gid++) {
            for (int x = 0; x < n[nid][gid].size(); x++) {
                for (int y = 0; y < n[nid][gid][x].size(); y++) {
                    if (n[nid][gid][x][y].sum == nid + 1 && n[nid][gid][x][y].num == 0) {
                        n.resize(nid + 2);
                        //build col
                        cells.clear();
                        for (int x = 0; x < n[nid][gid].size(); x++) {
                            col.clear();
                            for (int y = 0; y < n[nid][gid][x].size(); y++) {
                                col.push_back(n[nid][gid][x][y]);
                                col[y].sum = 0;
                            }
                            cells.push_back(col);
                            
                        }
                        cells[x][y].num = n[nid][gid][x][y].sum;
                        n[nid + 1].push_back(cells);
                    }
                }
            }
        }

        if (nid == target-1) { break; }
    }

    int maxn = n.size() - 1;
    std::cout << "End: " << maxn << "\n";
    for (int x = 0; x < n[maxn][0].size(); x++) {
        for (int y = 0; y < n[maxn][0][0].size(); y++) {
            std::cout << std::setw(2) << n[maxn][0][x][y].num << ",";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    goto start;
}
