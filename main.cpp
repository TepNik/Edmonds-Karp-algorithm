#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <cassert>

void processing_line(std::vector<std::string> &names,
                     std::vector<std::vector<int>> &adj_matrix,
                     const std::string &line);
void add_num(std::vector<std::string> &names,
             std::vector<std::vector<int>> &adj_matrix,
             const std::string &first_name,
             const std::string &second_name,
             int num);
void add_name(std::vector<std::string> &names,
              std::vector<std::vector<int>> &adj_matrix,
              const std::string &name);
std::vector<std::vector<int>> get_stream(const std::vector<std::vector<int>> &adj_matrix_max_stream);
void output_matrix(const std::vector<std::string> &names, const std::vector<std::vector<int>> &matrix);
std::vector<int> BFS(const std::vector<std::vector<int>> &adj_matrix,
                     const std::vector<std::vector<int>> &adj_matrix_reidual,
                     int from, int to);

int INF = 10000000;

int main()
{
    std::ifstream fin("input.txt");
    std::vector<std::string> names;
    std::vector<std::vector<int>> adj_matrix_max_stream;
    {
        std::string line;
        std::getline(fin, line);
        while(fin)
        {
            processing_line(names, adj_matrix_max_stream, line);
            std::getline(fin, line);
        }
    }
    std::cout << "Max stream matrix:\n";
    output_matrix(names, adj_matrix_max_stream);
    std::vector<std::vector<int>> adj_matrix_stream = get_stream(adj_matrix_max_stream);
    std::cout << "Stream matrix:\n";
    output_matrix(names, adj_matrix_stream);
    return 0;
}

void processing_line(std::vector<std::string> &names,
                     std::vector<std::vector<int>> &adj_matrix,
                     const std::string &line)
{
    std::stringstream ss {line};

    std::string first_name;
    ss >> first_name;
    add_name(names, adj_matrix, first_name);

    std::string second_name;
    ss >> second_name;
    add_name(names, adj_matrix, second_name);

    int val;
    ss >> val;
    add_num(names, adj_matrix, first_name, second_name, val);
}

void add_num(std::vector<std::string> &names,
             std::vector<std::vector<int>> &adj_matrix,
             const std::string &first_name,
             const std::string &second_name,
             int num)
{
    int ind1 = std::find(names.begin(), names.end(), first_name) - names.begin();
    int ind2 = std::find(names.begin(), names.end(), second_name) - names.begin();

    adj_matrix[ind1][ind2] = num;
}

void add_name(std::vector<std::string> &names,
              std::vector<std::vector<int>> &adj_matrix,
              const std::string &name)
{
    int find_ind = std::find(names.begin(), names.end(), name) - names.begin();
    if (find_ind == names.size())
    {
        names.push_back(name);
        adj_matrix.push_back(std::vector<int>(names.size(), INF));
        for(int i = 0; i < names.size() - 1; ++i)
            adj_matrix[i].push_back(INF);
    }
}

std::vector<std::vector<int>> get_stream(const std::vector<std::vector<int>> &adj_matrix_max_stream)
{
    std::vector<std::vector<int>> result(adj_matrix_max_stream);
    for(int i = 0; i < result.size(); ++i)
        for(int j = 0; j < result[i].size(); ++j)
            if (result[i][j] != INF)
                result[i][j] = 0;

    int source_ind = -1;
    for(int i = 0; i < adj_matrix_max_stream.size() && source_ind == -1; ++i)
    {
        bool is_good = true;
        for (int j = 0; j < adj_matrix_max_stream.size() && is_good; ++j)
            if (adj_matrix_max_stream[j][i] != INF)
                is_good = false;
        if (is_good)
            source_ind = i;
    }
    int stock_ind = -1;
    for(int i = 0; i < adj_matrix_max_stream.size() && stock_ind == -1; ++i)
    {
        bool is_good = true;
        for (int j = 0; j < adj_matrix_max_stream.size() && is_good; ++j)
            if (adj_matrix_max_stream[i][j] != INF)
                is_good = false;
        if (is_good)
            stock_ind = i;
    }
    if (source_ind == stock_ind)
        return result;

    while(true)
    {
        auto path = BFS(adj_matrix_max_stream, result, source_ind, stock_ind);
        if (path.size() == 0)
            return result;
        int min_bandwith = adj_matrix_max_stream[path[0]][path[1]] - result[path[0]][path[1]];
        for(int i = 1; i < path.size() - 1; ++i)
        {
            int now_bandwith = adj_matrix_max_stream[path[i]][path[i + 1]] - result[path[i]][path[i + 1]];
            if (now_bandwith < min_bandwith)
                min_bandwith = now_bandwith;
        }
        for(int i = 0; i < path.size() - 1; ++i)
            result[path[i]][path[i + 1]] += min_bandwith;
    }
}

void output_matrix(const std::vector<std::string> &names, const std::vector<std::vector<int>> &matrix)
{
    int width = 2;
    std::cout << std::setw(width) << "" << ' ';
    for(int i = 0; i < names.size(); ++i)
        std::cout << std::setw(width) << names[i] << ' ';
    std::cout << '\n';
    for(int i = 0; i < matrix.size(); ++i)
    {
        std::cout << std::setw(width) << names[i] << ' ';
        for(int j = 0; j < matrix[i].size(); ++j)
            std::cout << std::setw(width) << (matrix[i][j] < INF ? std::to_string(matrix[i][j]) : "-") << ' ';
        std::cout << '\n';
    }
}

std::vector<int> BFS(const std::vector<std::vector<int>> &adj_matrix,
                     const std::vector<std::vector<int>> &adj_matrix_reidual,
                     int from, int to)
{
    std::queue<int> need_to_visit;
    struct how_visited
    {
        bool visited = false;
        int parent = -1;
    };
    std::vector<how_visited> visited(adj_matrix.size());
    visited[from] = {true, -1};
    need_to_visit.push(from);
    bool is_find = false;
    while(need_to_visit.size() && !is_find)
    {
        int ind = need_to_visit.front();
        need_to_visit.pop();
        for(int i = 0; i < adj_matrix[ind].size() && !is_find; ++i)
            if (adj_matrix[ind][i] != INF && adj_matrix[ind][i] - adj_matrix_reidual[ind][i] > 0 && visited[i].visited == false)
            {
                visited[i] = {true, ind};
                need_to_visit.push(i);
                if (i == to)
                    is_find = true;
            }
    }
    if (need_to_visit.size() == 0 || visited[to].visited == false)
        return {};
    std::vector<int> result;
    result.push_back(to);
    int ind = visited[to].parent;
    while(ind != -1)
    {
        result.push_back(ind);
        ind = visited[ind].parent;
    }
    std::reverse(result.begin(), result.end());
    return result;
}