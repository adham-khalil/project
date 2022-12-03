#include <iostream>
#include <vector> 
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm>
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


bool bs(int high , int low , string target, vector<string> v){
    if(low<=high){
        int mid = (high+low)/2;

        if(v[mid] == target)
            return true;

        if(v[mid] > target)
            return bs(mid-1,low,target,v);

        return bs(high,mid+1,target,v);
    }

    return false;


}

class SearchEngine{


    public:
    vector<string> webpages;
    vector<edge> links;
    graph web;
    unordered_map<string,int> clicks;
    unordered_map<string,vector<string>> keywords;
    unordered_map<string,vector<string>> points_at;
    unordered_map<string, double> pr;
    unordered_map<string,int> appearence;
    unordered_map<string,double> ctr;


    void getwebpages(){
        ifstream Myfile("file1.csv");
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


    void getclicks(){


        ifstream Myfile("file2.csv");

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
                appearence[site] = 0;

            }
        }

        clicks = siteclicks;
    }

    void updateclicks(string str){
    
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

    void update_appearences(string str){
        appearence[str]++;
    }

    void getkeywords(){
        ifstream Myfile("file3.csv");
        while(!Myfile.eof()){
            string line,word;
            getline(Myfile,line);
            stringstream ss(line);
            bool is_site = true;
            vector<string> site_keywords;
            string site;
            int index = 0;
            while(getline(ss,word,',')){

                if(is_site){
                    is_site = false;
                    site = word;
                }

                else{
                    site_keywords.push_back(word);
                }
            }

            sort(site_keywords.begin(),site_keywords.end());
            keywords.insert({site,site_keywords});
        }


    }


    vector<string> website_keywords(string keyword){
        vector<string> comp;
        for(int i = 0 ; i < webpages.size() ; i++){
            int high = keywords[webpages[i]].size()-1;
            int low = 0;
            if(bs(high,low,keyword,keywords[webpages[i]])){
                comp.push_back(webpages[i]);
            }
        }

        return comp;
    }


    double num_of_pages(string str){
        return web.adjlist[str].size();
    }


    vector<string> pointed_at(string str){
        int size = webpages.size();
        vector<string> temp;
        int count = 0;
        for(int i = 0 ; i < size ; i++){
            int n = web.adjlist[webpages[i]].size();
            for(int j = 0 ; j < n ; j++){
                if(web.adjlist[webpages[i]][j] == str){
                    temp.push_back(webpages[i]);
                }
            }
        }

        return temp;
    }


    void initial_pagerank(){
        int size = webpages.size();
        double initial_pr = 1 / (double)size;
        for(int i = 0 ; i < size ; i++){
            pr[webpages[i]] = initial_pr;
            points_at.insert({webpages[i],pointed_at(webpages[i])});
        }
    }




    void pagerank(){

        
        int size = webpages.size();

        for(int i = 0 ; i < 3 ; i++){
            unordered_map<string,double> temp = pr;
            for(int j = 0 ; j < size ; j++){
                int n = points_at[webpages[j]].size();
                double sum = 0.0;
                for(int k = 0 ; k < n ; k++){
                    string str = points_at[webpages[j]][k];
                    double last_pr = temp[str];
                    double rank = num_of_pages(str);
                    sum+= last_pr/rank;

                }


                pr[webpages[j]] = sum;

            }
        }
    }

    void calc_score(vector<string> sites){

    }

    void CTR(vector<string> results){
        int size = results.size();
        for(int i = 0 ; i< size ; i++){
            double clicked = clicks[results[i]];
            double appearences = appearence[results[i]];
            double rate = (double) clicked/(double) appearences;
            ctr[results[i]] = rate;
       }
    }

    vector<string> search(){
        string keyword;
        cout << "enter your search:";
        cin >> keyword;
        vector<string> results = website_keywords(keyword);

        int size = results.size();

        for(int i = 0 ; i < size ; i++){
            update_appearences(results[i]);
        }

        CTR(results);

        return results;
    }

    void displaysearch(){
        vector<string> results = search();


    }



};

int main(){
    SearchEngine s;
    s.getwebpages();
    s.createweb();
    s.getclicks();
    s.getkeywords();
    s.initial_pagerank();
    s.pagerank();
}