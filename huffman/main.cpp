#include<iostream>
#include<string>
#include<unordered_map>
#include<queue>
#include<fstream>
#include<cmath>
using namespace std; 
unordered_map<char, string> codes; 
unordered_map<char, int> frequencies;
unordered_map<char, double> prob;
double L_average, entropy;

struct huffmanTree 
{ 
    char data;          
    int frequency;
    bool leaf;           
    huffmanTree *left, *right;
    huffmanTree()
    {
        left = right = NULL;
        this->data = ' ';
        this->frequency = 0;
        leaf=0;
    }
    huffmanTree(char data, int frequency, bool leaf) 
    { 
        left = right = NULL; 
        this->data = data; 
        this->frequency = frequency;
        this->leaf=leaf; 
    } 
};

double average_length()
{
    int count=0;
    int temp=0;
    for(auto i : codes)
    {
        temp+=i.second.length();
        count++;
    }
    return (double)temp/count;
}

void probabilities(string message)
{
    for(auto i : frequencies)
    {
        prob[i.first]=(double) i.second/message.length();
    }
}

void calc_entropy()
{
    for(auto i : prob)
    {
        entropy += (double) i.second * log2(1/i.second);
    }
}

struct compare 
{ 
    bool operator()(huffmanTree* left, huffmanTree* right) 
    { 
        return (left->frequency > right->frequency); 
    } 
};

priority_queue<huffmanTree*, vector<huffmanTree*>, compare> Heap;

void printCodes(huffmanTree* root, string str) 
{ 
    if (!root)
    {
        return;
    } 
    if (root->leaf == 1) 
    {
        cout << root->data << ": " << str << "\n";
    } 
    printCodes(root->left, str + "0"); 
    printCodes(root->right, str + "1"); 
}

void storeCodes(huffmanTree* root, string str) 
{ 
    if (root==NULL)
    {
        return;
    } 
    if (root->leaf == 1)
    {
        codes[root->data]=str;
    } 
    storeCodes(root->left, str + "0"); 
    storeCodes(root->right, str + "1"); 
} 

void allCodes(int size) 
{ 
    huffmanTree *left, *right, *top; 
    for (auto& i : frequencies) 
    {
        Heap.push(new huffmanTree(i.first, i.second,1));
    } 
    while (Heap.size() != 1) 
    { 
        left = Heap.top(); 
        if(left->data == '\n')
            cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n";
        Heap.pop(); 
        right = Heap.top(); 
        Heap.pop(); 
        top = new huffmanTree('$', left->frequency + right->frequency,0); 
        top->left = left; 
        top->right = right; 
        Heap.push(top); 
    } 
    storeCodes(Heap.top(), ""); 
}
 
void getFrequency(string str, int n) 
{ 
    for (int i=0; i<str.size(); i++) 
    {
        frequencies[str[i]]++;
    } 
} 

string decoder(huffmanTree* root, string encodedMessage) 
{
    string result = "";
    struct huffmanTree* current = root; 
    for (int i = 0; i < encodedMessage.size(); i++) 
    { 
        if (encodedMessage[i] == '0') 
        {
            current = current->left;
        }
        else if (encodedMessage[i] == '1')
        {
            current = current->right;
        }
        if (current->leaf) 
        { 
           
            result += current->data; 
            current = root;
            
        }
    } 
    return result;
}

void compress(string &message)
{
    getFrequency(message, message.length()); 
    allCodes(message.length());
}

void ratio(string original, string compressed)
{
    average_length();
    cout << "The compression ratio= " << (double)L_average/8 << endl;
}

int main()
{
    string line,message;
    ifstream inputFile("input.txt");

    while(getline(inputFile, line))
    {
        getFrequency(line, line.size());
        message += line+ "\n";
    }
    cout << message << endl;
    inputFile.close();

    allCodes(message.size());

    cout<<"\nHuffman Codes are :\n"<<endl;
    printCodes(Heap.top(), "");

    string encodedMessage = "";
    for (auto& i : message) 
    {
        encodedMessage += codes[i];
    }
    cout<<"\nEncoded message is : "<<encodedMessage<<endl;
    cout<<"\nSize of encoded message is: "<<encodedMessage.size()<<endl;

    ofstream outputFile("output.bin", ios::binary);
    outputFile << encodedMessage;
    outputFile.close();

    cout<<"\nEncoded message saved in 'output.bin' file"<<endl;

    cout<<"\nDecoded message is: "<<decoder(Heap.top(), encodedMessage)<<endl;

    probabilities(message);

    L_average = average_length();
    cout<<"\nAverage length of the codes: "<<L_average<<endl;

    calc_entropy();
    cout<<"\nEntropy is: "<<entropy<<endl;

    return 0;
}
