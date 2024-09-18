#include <iostream>
#include <map>
#include <fstream>
#include <queue>
#include <stack>
#include <climits>
#include <vector>
#include <string>

#define ll long long
#define pb push_back
#define fi first
#define se second
#define mp make_pair
using namespace std;

map<string, ll> M;
char color[200][200] = {'\0'};
vector< pair<ll, ll> > v[100010]; // Adjacency list
ll N; // Number of vertices
string station[200];
map<string, string> tourm;

// Class for priority queue comparison
class comparedis {
public:
    bool operator()(pair<ll, ll>& p, pair<ll, ll>& q) {
        return (p.se > q.se); // For min heap use > sign
    }
};

// Function to recharge smart card
void recharge() {
    fstream f;
    ll amt, ini, cid, fin, x;
    ll c_id, amount;
    f.open("paisa.txt", ios::in | ios::out);
    if (!f) {
        cout << "File Not Found\n" << endl;
        return;
    }
    f.seekg(0);
    cout << "Enter Card Id\n";
    cin >> c_id;
    cout << "Enter Amount\n";
    cin >> amount;
    f.clear();
    while (!f.eof()) {
        ini = f.tellg();
        f.ignore();
        f >> cid >> amt;
        fin = f.tellg();
        if (cid == c_id) {
            x = amt + amount;
            f.seekg(ini);
            f << cid << endl << x;
            cout << "Recharge Details\n";
            cout << "Card Id: " << cid << endl;
            cout << "Initial Balance: " << amt << endl;
            cout << "Recharge Amount: " << amount << endl;
            cout << "Total Balance: " << x << endl;
            break;
        }
    }
    f.close();
}

// Function to get tourist place data
void gettour() {
    ifstream fin;
    string s1, s2;
    fin.open("tourplace.txt", ios::in);
    if (!fin) {
        cout << "File Not Found\n";
        return;
    }
    fin.seekg(0);
    fin.clear();
    while (getline(fin, s1) && getline(fin, s2)) {
        tourm[s1] = s2;
    }
    fin.close();
}

// Function to display all stations and their lines
void displayAllStations() {
    cout << "List of all Delhi Metro stations and their lines:" << endl;
    for (const auto& entry : M) {
        string stationName = entry.first;
        ll stationIndex = entry.second;
        char line = '\0';

        // Determine line color for each station
        for (int i = 0; i < 200; ++i) {
            if (color[stationIndex][i] != '\0') {
                line = color[stationIndex][i];
                break;
            }
        }

        // Print station details
        cout << "Station: " << stationName << ", Line: ";
        switch (line) {
            case 'b': cout << "Blue"; break;
            case 'y': cout << "Yellow"; break;
            case 'o': cout << "Orange"; break;
            case 'g': cout << "Green"; break;
            case 'r': cout << "Red"; break;
            case 'v': cout << "Violet"; break;
            default: cout << "Unknown"; break;
        }
        cout << endl;
    }
}

// Function to display path and interchange information
void disp(ll src, ll dest, ll par[]) {
    ll i, x, y, cn = 0, ci = 0;
    stack<ll> st;
    st.push(dest);
    i = dest;
    while (par[i] != -1) {
        i = par[i];
        st.push(i);
    }
    char col = '\0';
    while (!st.empty()) {
        x = st.top();
        st.pop();
        if (!st.empty()) {
            y = st.top();
        }
        cout << station[x] << " ";
        cn++;
        if (col == '\0') {
            col = color[x][y];
        } else if (col != '\0' && col != color[x][y]) {
            char c = color[x][y];
            ci++;
            switch (c) {
                case 'b': cout << "\t\tChange to blue line"; break;
                case 'y': cout << "\t\tChange to yellow line"; break;
                case 'o': cout << "\t\tChange to orange line"; break;
                case 'g': cout << "\t\tChange to green line"; break;
                case 'r': cout << "\t\tChange to red line"; break;
                case 'v': cout << "\t\tChange to violet line"; break;
                default: cout << "\t\tChange to unknown line"; break;
            }
            col = c;
        }
        cout << endl;
    }
    cout << endl << "No of stations = " << cn << endl;
    cout << "No of interchange stations = " << ci - 1 << endl;
    cout << endl;
}

// Function to find shortest path using BFS
void bfs(ll src, ll dest) {
    bool vis[100010] = {false};
    ll par[100010];
    for (ll i = 0; i < N; ++i) {
        par[i] = -1;
    }
    queue<ll> q;
    q.push(src);
    vis[src] = true;
    while (!q.empty()) {
        ll x = q.front();
        q.pop();
        ll vsz = v[x].size();
        for (ll i = 0; i < vsz; ++i) {
            ll y = v[x][i].fi;
            if (!vis[y]) {
                par[y] = x;
                vis[y] = true;
                q.push(y);
            }
        }
        v[x].clear();
    }
    disp(src, dest, par);
}

// Function to find the most economical path using Dijkstra's Algorithm
void dijkstra(ll src, ll dest) {
    bool vis[100010] = {false};
    ll dist[100010], par[100010];
    for (ll i = 0; i < N; ++i) {
        dist[i] = LLONG_MAX;
        par[i] = -1;
    }
    priority_queue< pair<ll, ll>, vector< pair<ll, ll> >, comparedis> pq;
    pq.push(mp(src, 0));
    dist[src] = 0;
    par[src] = -1;
    vis[src] = true;
    while (!pq.empty()) {
        pair<ll, ll> k = pq.top();
        pq.pop();
        ll x = k.fi;
        ll vsz = v[x].size();
        for (ll i = 0; i < vsz; ++i) {
            ll y = v[x][i].fi;
            ll w = v[x][i].se;
            if (dist[x] + w < dist[y]) {
                par[y] = x;
                dist[y] = dist[x] + w;
            }
            if (!vis[y]) {
                vis[y] = true;
                pq.push(mp(y, dist[y]));
            }
        }
        v[x].clear();
    }
    disp(src, dest, par);
}

// Function to initialize station mapping from file
void consmap() {
    ifstream fin;
    string s;
    fin.open("list.txt", ios::in);
    ll l = 0;
    fin.seekg(0);
    fin.clear();
    while (getline(fin, s)) {
        M[s] = l;
        station[l] = s;
        l++;
    }
    N = l - 1;
    fin.close();
}

// Function to add edges to the graph from file
void addedge(char fname[], ll w) {
    ifstream fin;
    string s;
    ll x, y;
    fin.open(fname, ios::in);
    fin.seekg(0);
    getline(fin, s);
    x = M[s];
    char c = fname[0];
    fin.clear();
    while (getline(fin, s)) {
        y = M[s];
        v[x].pb(mp(y, w));
        v[y].pb(mp(x, w));
        color[x][y] = c;
        color[y][x] = c;
        x = y;
    }
    fin.close();
}

// Function to initialize graph edges from files
void consgraph() {
    string s;
    addedge("blueline.txt", 0);
    addedge("yellowline.txt", 0);
    addedge("redline.txt", 0);
    addedge("greenline.txt", 0);
    addedge("violetline.txt", 0);
    addedge("bluext.txt", 0);
    addedge("orangeline.txt", 1);
}

// Main function to drive the program
int main() {
    string source, destination;
    ll src, dest, choice, dec;
    char ch;

    gettour();
    consmap();

    do {
        cout << endl;
        cout << "1. To Route between two stations\n";
        cout << "2. To check nearest metro station to a tourist place\n";
        cout << "3. To Recharge your Smart Card\n";
        cout << "4. To Display All Stations and Their Lines\n";
        cin >> dec;
        cin.ignore(); // To ignore newline character left in the buffer

        switch (dec) {
            case 1:
                do {
                    consgraph(); // To build the adjacency matrix
                    cout << "Enter station 1\n";
                    getline(cin, source);
                    cout << "Enter station 2\n";
                    getline(cin, destination);
                    src = M[source];
                    dest = M[destination];
                    cout << "1. For most economic path\n";
                    cout << "2. For shortest path\n";
                    cin >> choice;
                    cin.ignore(); // To ignore newline character left in the buffer
                    switch (choice) {
                        case 1: dijkstra(src, dest); break;
                        case 2: bfs(src, dest); break;
                    }
                    cout << "Do you wish to check for any other station\n";
                    cin >> ch;
                    cin.ignore(); // To ignore newline character left in the buffer
                } while (ch == 'Y' || ch == 'y');
                break;

            case 2:
                do {
                    string place;
                    cout << "Enter a place\n";
                    getline(cin, place);
                    string st = tourm[place];
                    cout << st << endl;
                    cout << "Do you wish to check for any other place\n";
                    cin >> ch;
                    cin.ignore(); // To ignore newline character left in the buffer
                } while (ch == 'Y' || ch == 'y');
                break;

            case 3:
                do {
                    recharge();
                    cout << "Do you wish to recharge some other smart card\n";
                    cin >> ch;
                    cin.ignore(); // To ignore newline character left in the buffer
                } while (ch == 'Y' || ch == 'y');
                break;

            case 4:
                displayAllStations();
                break;
        }

        cout << "Do you wish to go back to main menu\n";
        cin >> ch;
        cin.ignore(); // To ignore newline character left in the buffer
    } while (ch == 'Y' || ch == 'y');

    return 0;
}
