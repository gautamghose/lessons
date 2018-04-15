#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include <sstream>
#include <map>
#include <memory> //for unique_ptr

struct graph_edge {
    int end_vertex_; //equates to the graph_vertex.id_
    int weight_;
    graph_edge(int end, int w) : end_vertex_(end), weight_(w) {}
};

struct graph_vertex {
    int id_;
    std::vector<graph_edge> edges_;
    graph_vertex(int id) : id_(id) {}
    void add_edge(int end, int w)
    {
        edges_.push_back(graph_edge(end,w));
    }
};

typedef std::vector<int> connected_t;

struct graph {
    enum {
        e_unexplored = 0,
        e_visiting = 1,
        e_visited = 2
    };

    //std::vector<graph_vertex> vertices_;
    graph() {}
    void add_edge(int src, int target, int weight)
    {
        std::cout << "src, target = " << src << "," << target << std::endl;
        auto it = vertices_map_.find(src);
        if (it == vertices_map_.end()) {
            vertices_map_.insert( std::pair<int, std::unique_ptr<graph_vertex>>
                                    (src, std::unique_ptr<graph_vertex>(new graph_vertex(src))) );
        }
        vertices_map_[src]->add_edge(target, weight);

        it = vertices_map_.find(target);
        if (it == vertices_map_.end()) {
            vertices_map_.insert( std::pair<int, std::unique_ptr<graph_vertex>>
                                    (target, std::unique_ptr<graph_vertex>(new graph_vertex(target))) );
        }
        vertices_map_[target]->add_edge(src, weight);
    }

    void get_connected_components(std::vector<connected_t>& components)
    {
        components.clear();

        int *visits_arr = (int*)calloc(vertices_map_.size(),sizeof(int));
        size_t nr_vertices_explored = 0;
        while (nr_vertices_explored < vertices_map_.size()) {
            int start_at = get_next_unexplored_vertex(visits_arr);
            assert(start_at >= 0 && start_at < (int)vertices_map_.size());
            connected_t next_cc; //obtain next connected component
            do_dfs(start_at, visits_arr, next_cc);
            nr_vertices_explored += next_cc.size();
            components.push_back(next_cc);
        }
    }
protected:
    void do_dfs(int start, int* visits_arr, connected_t& cc)
    {
        if (visits_arr[start] != e_unexplored) return;
        visits_arr[start] = e_visiting;
        cc.push_back(start);
        assert(vertices_map_.find(start) != vertices_map_.end());
        graph_vertex* v = vertices_map_[start].get();
        auto it = v->edges_.begin();
        for (; it != v->edges_.end(); it++) {
            graph_edge& e = (*it);
            assert(vertices_map_.find(e.end_vertex_) != vertices_map_.end());
            assert(v->id_ == start);
            if (visits_arr[e.end_vertex_] == e_unexplored) {
                do_dfs(e.end_vertex_, visits_arr, cc);
            }
        }
        visits_arr[start] = e_visited;
        return;
    }
    int get_next_unexplored_vertex(int* visits_arr)
    {
        for (size_t i=0; i<vertices_map_.size(); i++) {
            if (visits_arr[i] == 0) return (int)i;
        }
        return (-1);
    }
    std::map<int, std::unique_ptr<graph_vertex>> vertices_map_;
};

struct name_aliases {
    graph names_;
    std::map<std::string, int> all_names_; //names mapped to vertex-id's
    std::vector<std::string> all_names_vector_; //
    std::map<std::string, std::string> all_names_groups_mapped_;

    void add_synonym(const std::string& name1, const std::string& name2)
    {
        assert(name1 != name2);
        int id1 = -1, id2 = -1;
        auto it = all_names_.find(name1);
        if (it == all_names_.end()) {
            all_names_vector_.push_back(name1); 
            all_names_[name1] = id_counter_ ++;
        }
        it = all_names_.find(name2);
        if (it == all_names_.end()) {
            all_names_vector_.push_back(name2); 
            all_names_[name2] = id_counter_ ++;
        }
        id1 = all_names_[name1];
        id2 = all_names_[name2];
        assert(id1 != id2);
        names_.add_edge(id1, id2, 1);
    }

    void finalize()
    {
        std::vector<connected_t> connected_names;
        names_.get_connected_components(connected_names); 

        //create the group/set names
        auto it = connected_names.begin();
        for (; it!=connected_names.end(); it++) {
            const connected_t& vertices = (*it);
            auto it2 = vertices.begin();
            std::stringstream group_ss;
            for (; it2 != vertices.end(); it2++) {
                int vertex_id = (*it2);
                group_ss << all_names_vector_[vertex_id] << "-";
            }
            group_ss << "end";
            
            it2 = vertices.begin();
            for (; it2 != vertices.end(); it2++) {
                int vertex_id = (*it2);
                assert(all_names_groups_mapped_.find(all_names_vector_[vertex_id]) == all_names_groups_mapped_.end());
                all_names_groups_mapped_[all_names_vector_[vertex_id]] = group_ss.str();
            }
        }
    }

    std::string lookup_name(const std::string& name)
    {
        if (all_names_groups_mapped_.find(name) == all_names_groups_mapped_.end()) {
            return std::string("nothing");
        } else {
            return all_names_groups_mapped_[name];
        }
    }

    name_aliases() : id_counter_(0) {}
protected:
    int id_counter_;
};

static name_aliases babynames;

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <name-aliases-filename>" << std::endl;
        return 1;
    }
    
    std::ifstream ifs(argv[1]);
    if (!ifs.is_open()) {
        std::cerr << "Could not open " << argv[1] << " for reading.\n";
        return 1;
    }

    std::string oneline;
    while (!ifs.eof()) {
        std::getline(ifs, oneline);
        if (oneline.empty()) continue;
        std::cout << "processing line: " << oneline << std::endl;
        std::size_t pos = oneline.find_first_of(",",0);
        if (pos == std::string::npos) {
            std::cerr << "Invalid format detected at the following line in input file " << argv[1] << std::endl;
            std::cerr << oneline << std::endl;
            return 2;
        }
        babynames.add_synonym(oneline.substr(0,pos), oneline.substr(pos+1));
    }
    
    ifs.close();

    babynames.finalize();

    std::cout << "Post-process has completed. Ready to look up names. Ctrl-C to end program.\n";
    std::string name;
    while (true) {
        std::cout << "What name to look up: ";
        std::cin >> name;
        std::cout << name << ": " << babynames.lookup_name(name) << std::endl;
    }
    return 0;
}


