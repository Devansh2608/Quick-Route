// #include <iostream>
// #include <unordered_map>
// #include <vector>
// #include <string>
// #include <queue>
// #include <cmath>
#include<bits/stdc++.h>

using namespace std;

class Graph_M {
public:
    class Vertex {
    public:
        unordered_map<string, int> nbrs;
    };

    unordered_map<string, Vertex*> vtces;

    Graph_M() {}

    int numVertex() {
        return this->vtces.size();
    }

    bool containsVertex(const string& vname) {
        return this->vtces.find(vname) != this->vtces.end();
    }

    void addVertex(const string& vname) {
        Vertex* vtx = new Vertex();
        vtces[vname] = vtx;
    }
//remove vertex function
    void removeVertex(const string& vname) {
        Vertex* vtx = vtces[vname];
        vector<string> keys;
        for (auto& p : vtx->nbrs) {
            keys.push_back(p.first);
        }

        for (const string& key : keys) {
            Vertex* nbrVtx = vtces[key];
            nbrVtx->nbrs.erase(vname);
        }

        delete vtx;
        vtces.erase(vname);
    }

    int numEdges() {
        int count = 0;
        for (auto& p : vtces) {
            Vertex* vtx = p.second;
            count += vtx->nbrs.size();
        }

        return count / 2;
    }

    bool containsEdge(const string& vname1, const string& vname2) {
        if (vtces.find(vname1) == vtces.end() || vtces.find(vname2) == vtces.end()) {
            return false;
        }
        Vertex* vtx1 = vtces[vname1];
        return vtx1->nbrs.find(vname2) != vtx1->nbrs.end();
    }

    void addEdge(const string& vname1, const string& vname2, int value) {
        if (vtces.find(vname1) == vtces.end() || vtces.find(vname2) == vtces.end()) {
            return;
        }
        Vertex* vtx1 = vtces[vname1];
        Vertex* vtx2 = vtces[vname2];
        if (vtx1->nbrs.find(vname2) != vtx1->nbrs.end()) {
            return;
        }
        vtx1->nbrs[vname2] = value;
        vtx2->nbrs[vname1] = value;
    }

    void removeEdge(const string& vname1, const string& vname2) {
        if (vtces.find(vname1) == vtces.end() || vtces.find(vname2) == vtces.end()) {
            return;
        }
        Vertex* vtx1 = vtces[vname1];
        Vertex* vtx2 = vtces[vname2];
        if (vtx1->nbrs.find(vname2) == vtx1->nbrs.end()) {
            return;
        }
        vtx1->nbrs.erase(vname2);
        vtx2->nbrs.erase(vname1);
    }

    void display_Map() {
        cout << "\t Delhi Metro Map" << endl;
        cout << "\t------------------" << endl;
        cout << "----------------------------------------------------" << endl;

        for (auto& p : vtces) {
            string key = p.first;
            Vertex* vtx = p.second;
            cout << key << " =>" << endl;
            for (auto& nbr : vtx->nbrs) {
                cout << "\t" << nbr.first << "\t" << nbr.second << endl;
            }
        }

        cout << "\t------------------" << endl;
        cout << "---------------------------------------------------" << endl;
    }
   

    void display_Stations() {
        cout << "\n***********************************************************************\n" << endl;
        int i = 1;
        for (auto& p : vtces) {
            cout << i << ". " << p.first << endl;
            i++;
        }
        cout << "\n***********************************************************************\n" << endl;
    }

    bool hasPath(const string& vname1, const string& vname2, unordered_map<string, bool>& processed) {
        if (containsEdge(vname1, vname2)) {
            return true;
        }

        processed[vname1] = true;

        Vertex* vtx = vtces[vname1];
        for (auto& nbr : vtx->nbrs) {
            if (processed.find(nbr.first) == processed.end()) {
                if (hasPath(nbr.first, vname2, processed)) {
                    return true;
                }
            }
        }

        return false;
    }

    class DijkstraPair {
    public:
        string vname;
        string psf;
        int cost;

        bool operator<(const DijkstraPair& o) const {
            return cost > o.cost;
        }
    };

    int dijkstra(const string& src, const string& des, bool nan) {
        int val = 0;
        vector<string> ans;
        unordered_map<string, DijkstraPair*> map;

        priority_queue<DijkstraPair> heap;

        for (auto& p : vtces) {
            string key = p.first;
            DijkstraPair* np = new DijkstraPair();
            np->vname = key;
            np->psf = "";
            np->cost = INT_MAX;

            if (key == src) {
                np->cost = 0;
                np->psf = key;
            }

            heap.push(*np);
            map[key] = np;
        }

        while (!heap.empty()) {
            DijkstraPair rp = heap.top();
            heap.pop();

            if (rp.vname == des) {
                val = rp.cost;
                break;
            }

            map.erase(rp.vname);

            ans.push_back(rp.vname);

            Vertex* v = vtces[rp.vname];
            for (auto& nbr : v->nbrs) {
                if (map.find(nbr.first) != map.end()) {
                    int oc = map[nbr.first]->cost;
                    Vertex* k = vtces[rp.vname];
                    int nc;
                    if (nan) {
                        nc = rp.cost + 120 + 40 * k->nbrs[nbr.first];
                    } else {
                        nc = rp.cost + k->nbrs[nbr.first];
                    }

                    if (nc < oc) {
                        DijkstraPair* gp = map[nbr.first];
                        gp->psf = rp.psf + nbr.first;
                        gp->cost = nc;

                        heap.push(*gp);
                    }
                }
            }
        }

        return val;
    }

    class Pair {
    public:
        string vname;
        string psf;
        int min_dis;
        int min_time;
    };

    string Get_Minimum_Distance(const string& src, const string& dst) {
        int min = INT_MAX;
        string ans;
        unordered_map<string, bool> processed;
        deque<Pair*> stack;

        Pair* sp = new Pair();
        sp->vname = src;
        sp->psf = src + "  ";
        sp->min_dis = 0;
        sp->min_time = 0;

        stack.push_front(sp);

        while (!stack.empty()) {
            Pair* rp = stack.front();
            stack.pop_front();

            if (processed.find(rp->vname) != processed.end()) {
                continue;
            }

            processed[rp->vname] = true;

            if (rp->vname == dst) {
                int temp = rp->min_dis;
                if (temp < min) {
                    ans = rp->psf;
                    min = temp;
                }
                continue;
            }

            Vertex* rpvtx = vtces[rp->vname];
            for (auto& nbr : rpvtx->nbrs) {
                if (processed.find(nbr.first) == processed.end()) {
                    Pair* np = new Pair();
                    np->vname = nbr.first;
                    np->psf = rp->psf + nbr.first + "  ";
                    np->min_dis = rp->min_dis + rpvtx->nbrs[nbr.first];
                    stack.push_front(np);
                }
            }
        }

        ans += to_string(min);
        return ans;
    }

    string Get_Minimum_Time(const string& src, const string& dst) {
        int min = INT_MAX;
        string ans;
        unordered_map<string, bool> processed;
        deque<Pair*> stack;

        Pair* sp = new Pair();
        sp->vname = src;
        sp->psf = src + "  ";
        sp->min_dis = 0;
        sp->min_time = 0;

        stack.push_front(sp);

        while (!stack.empty()) {
            Pair* rp = stack.front();
            stack.pop_front();

            if (processed.find(rp->vname) != processed.end()) {
                continue;
            }

            processed[rp->vname] = true;

            if (rp->vname == dst) {
                int temp = rp->min_time;
                if (temp < min) {
                    ans = rp->psf;
                    min = temp;
                }
                continue;
            }

            Vertex* rpvtx = vtces[rp->vname];
            for (auto& nbr : rpvtx->nbrs) {
                if (processed.find(nbr.first) == processed.end()) {
                    Pair* np = new Pair();
                    np->vname = nbr.first;
                    np->psf = rp->psf + nbr.first + "  ";
                    np->min_time = rp->min_time + 120 + 40 * rpvtx->nbrs[nbr.first];
                    stack.push_front(np);
                }
            }
        }

        int hours = min / 3600;
        int minutes = (min % 3600) / 60;
        ans += to_string(hours) + " hour(s) and " + to_string(minutes) + " minute(s)";
        return ans;
    }
};

int main() {
    Graph_M g;
    int choice;
    g.addVertex("Noida Sector 62");
    g.addVertex("Botanical Garden");
    g.addVertex("Anand Vihar");
    g.addVertex("Rajiv Chowk");

    g.addEdge("Noida Sector 62", "Botanical Garden", 10);
    g.addEdge("Botanical Garden", "Anand Vihar", 20);
    g.addEdge("Anand Vihar", "Rajiv Chowk", 30);
    g.addEdge("Noida Sector 62", "Anand Vihar", 25);

    do {
        cout << "\nMenu:\n";
        cout << "1. Add Vertex\n";
        cout << "2. Add Edge\n";
        cout << "3. Display Map\n";
        cout << "4. Display Stations\n";
        cout << "5. Get Minimum Distance Path\n";
        cout << "6. Get Minimum Time Path\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string vname;
                cout << "Enter vertex name: ";
                cin.ignore();
                getline(cin, vname);
                g.addVertex(vname);
                break;
            }
            case 2: {
                string vname1, vname2;
                int value;
                cout << "Enter first vertex name: ";
                cin.ignore();
                getline(cin, vname1);
                cout << "Enter second vertex name: ";
                getline(cin, vname2);
                cout << "Enter edge value: ";
                cin >> value;
                g.addEdge(vname1, vname2, value);
                break;
            }
            case 3: {
                g.display_Map();
                break;
            }
            case 4: {
                g.display_Stations();
                break;
            }
            case 5: {
                string src, dst;
                cout << "Enter source station: ";
                cin.ignore();
                getline(cin, src);
                cout << "Enter destination station: ";
                getline(cin, dst);
                cout<<endl;
                //int ans=  g.Get_Minimum_Distance(src, dst);
                // if(ans==INT_MAX)
                // cout<<"No direct or indirect path"<<endl;
                // else
                cout << "Minimum Distance Path: " << g.Get_Minimum_Distance(src, dst) << endl;
                break;
            }
            case 6: {
                string src, dst;
                cout << "Enter source station: ";
                cin.ignore();
                getline(cin, src);
                cout << "Enter destination station: ";
                getline(cin, dst);
                cout << "Minimum Time Path: " << g.Get_Minimum_Time(src, dst) << endl;
                break;
            }
            case 7: {
                cout << "Exiting...\n";
                break;
            }
            default: {
                cout << "Invalid choice, please try again.\n";
            }
        }
    } while (choice != 7);

    return 0;
}
