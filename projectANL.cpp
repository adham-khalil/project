#include <iostream>
#include <vector> 
#include <unordered_map>
#include <fstream>
using namespace std;

struct edge{
    string src,dest;
};


class graph{
    
    public:
    unordered_map<string , vector<string>> adjlist;

    void creategraph(vector< edge> &edges){

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
        cout << i << endl;
        
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
            while(!Myfile.eof()){    
                Myfile >> src >> dest;
                int size  = webpages.size();
                bool exists = false;
                for(int i = 0 ; i < size ; i++){
                    if(src == webpages[i]){
                        exists = true;
                    }
                }
                if(!exists){
                    webpages.push_back(src);
                }
                edge e;
                e.src = src;
                if(dest != ""){

                    e.dest = dest;
                    links.push_back(e);
                }
                else{
                    e.dest = " ";
                    links.push_back(e);
                }
                
            }
        }
    }

    void createweb(){
        web.creategraph(links);
    }


    void getclicks(string str){
        ifstream Myfile(str);
        string site;
        int numofclicks;
        if(Myfile.is_open()){
            while(!Myfile.eof()){
                Myfile>> site >> numofclicks;
                if(site != ""){
                    clicks[site] = numofclicks;
                    cout << clicks[site] << endl;
                }
            }
        }
    }

    void updateclicks(string str){
        clicks[str]++;
        int index = 0;
        ofstream Myfile("file2.txt");
        while(index < webpages.size()){
            Myfile << webpages[index] << "\t" << clicks[webpages[index]];
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
    s.getwebpages("file1.txt");
    s.createweb();
    s.web.display(4,s.webpages);


}