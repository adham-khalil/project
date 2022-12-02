#include <iostream>
#include <vector> 
#include <unordered_map>
#include <fstream>
#include <sstream>
using namespace std;

struct edge{
    string src,dest;
};


class graph{
    
    public:
    unordered_map<string , vector<string>> adjlist;

    void creategraph(vector<edge> &edges){

        for(auto &edge : edges){
            adjlist[edge.src].push_back(edge.dest);
        }
    }

    void display(int size, vector<string> v){
    for(int i = 0 ; i < size ; i++){
        cout << v[i] << "->";
        for(int j = 0 ; j < adjlist[v[i]].size() ; j++){
            cout << adjlist[v[i]][j];
            cout << "\t";
        }
        cout << endl;
        
    }
    
    }
};


class SearchEngine{
    public:
    vector<string> webpages;
    vector<edge> links;
    graph web;
    unordered_map<string,int> clicks;


    void getwebpages(string str){
        ifstream Myfile(str);
        string src,dest;
        if(Myfile.is_open()){
            vector<string> sites;
            vector<bool> isnode;
            int node_index = 0;
            while(!Myfile.eof()){
                string line;
                string site;
                getline(Myfile,line);
                stringstream ss(line);
                bool isnode = true;
                string node;


                while(getline(ss,site,',')){
                    if(isnode){
                        node = site;
                        webpages.push_back(site);
                        isnode = false;
                    }

                    else{
                        edge e;
                        e.src = node;
                        e.dest = site;
                        links.push_back(e);
                    }
                }
            }

        }

    }


    void createweb(){
        web.creategraph(links);
    }


    unordered_map<string,int> getclicks(string str){


        ifstream Myfile(str);

        int numofclicks;

        unordered_map<string,int> siteclicks;

        if(Myfile.is_open()){

            while(!Myfile.eof()){
                string line;
                getline(Myfile,line);
                stringstream ss(line);
                string element,site;
                int clicks ;
                int index =0;
                while(getline(ss,element,',')){
                    if(index == 0){
                        site = element;
                        index++;
                    }
                    else{
                        clicks = stoi(element);
                    }
                }
                siteclicks.insert({site,clicks});

            }
        }

        return siteclicks;
    }

    void updateclicks(string str){
        clicks = getclicks("file2.csv");
        clicks[str]++;
        int index = 0;
        ofstream Myfile("file2.csv");
        while(index < webpages.size()){
            Myfile << webpages[index] << ',' << clicks[webpages[index]];
            index++;
            if(index == webpages.size()){
                break;
            }
            else
                Myfile << endl;
        }
    }

    int nodepr(string str){
        return web.adjlist[str].size();
    }


    vector<string> pointed_at(string str){
        int size = webpages.size();
        vector<string> points_at;
        for(int i = 0 ;  i < size ; i++){
            for(int j = 0 ; j < size ; j++){
                if(web.adjlist[webpages[i]][j] == str){
                    points_at.push_back(webpages[j]);
                    cout << points_at[j] << endl;
                }
            }
        }

        return points_at;
    }


    vector<string> pagerank(){
        unordered_map<string, double> pr;
        int size = webpages.size();
        int index = 0;
        double startval = 1/size;
        while(index < size){
            pr[webpages[index]] = startval;
            index++;
        }

        return {};
    }



};

int main(){
    SearchEngine s;
    s.getwebpages("file1.csv");
    s.createweb();
    s.getclicks("file2.csv");
    s.updateclicks("www.test1.com");

}