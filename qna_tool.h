#pragma once
#include <iostream>
#include <fstream>
#include "Node.cpp"
#include "dict.cpp"
using namespace std;

class ShortAddress{
    public: 
    int book_code;
    int page;
    int paragraph; 

    ShortAddress(int book_code, int page, int paragraph){
        this->book_code = book_code; 
        this->page = page; 
        this->paragraph = paragraph; 
    }

    bool operator == (const ShortAddress &ad) const{ 
        if ((this->book_code==ad.book_code) && (this->page==ad.page) && (this->paragraph==ad.paragraph))
            return true; 
        return false; 
    }

};

class top_k {
public:
    int k; 
    vector<pair<long long,ShortAddress> > heap; 

    void heapify_down(int i) {
        long long leftNode = 1e14; long long rightNode = 1e14; 
        if(2*i+1<heap.size()){leftNode = heap[2*i+1].first;}
        if(2*i+2<heap.size()){rightNode = heap[2*i+2].first;}

        if(leftNode>heap[i].first && rightNode>heap[i].first){
            return; 
        }
        if(leftNode<heap[i].first){
            if(rightNode<leftNode){
                auto temp = heap[i]; heap[i] = heap[2*i+2]; heap[2*i+2] = temp;
                heapify_down(2*i+2);
            }
            else{
                auto temp = heap[i]; heap[i] = heap[2*i+1]; heap[2*i+1] = temp;
                heapify_down(2*i+1);
            }
        }
        else if(rightNode<heap[i].first){
            auto temp = heap[i]; heap[i] = heap[2*i+2]; heap[2*i+2] = temp;
            heapify_down(2*i+2);
        }
        return;
    }

    void heapify_up(int i){
        if(i==0){return;}
        if(heap[(i-1)/2].first>heap[i].first){
            auto temp = heap[i]; heap[i]=heap[(i-1)/2]; heap[(i-1)/2]=temp;
            heapify_up((i-1)/2);
        }
    }

    void insert(pair<long long,ShortAddress> n){
        heap.push_back(n);
        heapify_up(heap.size()-1);
    }
    
    void add(pair<long long,ShortAddress> val) {
        if(heap.size()==0){heap.push_back(val); return;}
        if(heap.size()<k){
            insert(val);
        }
        else if(heap[0].first<=val.first){
            heap[0]=val;
            heapify_down(0);
        }
        return;
    }

    ShortAddress remove(){
        ShortAddress ans = heap[0].second;
        heap[0] = heap[heap.size()-1];
        heap.pop_back(); 
        heapify_down(0);
        return ans;
    }
};

struct Score {
    ShortAddress location;
    long long score;
};

class Chaining {
public:
    Chaining();
    int hash(ShortAddress location);
    void createScore(ShortAddress location, long long score);
    std::vector<ShortAddress> getTopK(int k);
    long long getscore(ShortAddress location);
    void addScore(ShortAddress location, long long score);
    bool doesExist(ShortAddress location);
    int databaseSize();

    std::vector<std::vector<Score> > bankStorage2d;
    vector<ShortAddress> nonZero;

    int p[3] = {};
    int capacity = 500;
    int BaseSize=0;
    
};

class Stop_Word_Hash{
    public:
        vector<vector<string>> H_Table;
        int size;

    Stop_Word_Hash(){
        vector<string> stopwords=
        {
            "a", "about", "above", "across", "after", "afterwards", "again", "against", "all", "almost", 
            "alone", "along", "already", "also", "although", "always", "am", "among", "amongst", "amount", 
            "an", "and", "another", "any", "anyhow", "anyone", "anything", "anyway", "anywhere", "are", 
            "around", "as", "at", "back", "be", "became", "because", "become", "becomes", "becoming", 
            "been", "before", "beforehand", "behind", "being", "below", "beside", "besides", "between", 
            "beyond", "both", "bottom", "but", "by", "call", "can", "cannot", "ca", "could", "did", "do", 
            "does", "doing", "done", "down", "due", "during", "each", "eight", "either", "eleven", "else", 
            "elsewhere", "empty", "enough", "even", "ever", "every", "everyone", "everything", "everywhere", 
            "except", "few", "fifteen", "fifty", "first", "five", "for", "former", "formerly", "forty", 
            "four", "from", "front", "full", "further", "get", "give", "go", "had", "has", "have", "he", 
            "hence", "her", "here", "hereafter", "hereby", "herein", "hereupon", "hers", "herself", "him", 
            "himself", "his", "how", "however", "hundred", "i", "if", "in", "indeed", "into", "is", "it", 
            "its", "itself", "keep", "last", "latter", "latterly", "least", "less", "just", "made", "make", 
            "many", "may", "me", "meanwhile", "might", "mine", "more", "moreover", "most", "mostly", "move", 
            "much", "must", "my", "myself", "name", "namely", "neither", "never", "nevertheless", "next", 
            "nine", "no", "nobody", "none", "noone", "nor", "not", "nothing", "now", "nowhere", "of", "off", 
            "often", "on", "once", "one", "only", "onto", "or", "other", "others", "otherwise", "our", 
            "ours", "ourselves", "out", "over", "own", "part", "per", "perhaps", "please", "put", "quite", 
            "rather", "re", "really", "regarding", "same", "say", "see", "seem", "seemed", "seeming", 
            "seems", "serious", "several", "she", "should", "show", "side", "since", "six", "sixty", "so", 
            "some", "somehow", "someone", "something", "sometime", "sometimes", "somewhere", "still", 
            "such", "take", "ten", "than", "that", "the", "their", "them", "themselves", "then", "thence", 
            "there", "thereafter", "thereby", "therefore", "therein", "thereupon", "these", "they", "third", 
            "this", "those", "though", "three", "through", "throughout", "thru", "thus", "to", "together", 
            "too", "top", "toward", "towards", "twelve", "twenty", "two", "under", "until", "up", "unless", 
            "upon", "us", "used", "using", "various", "very", "via", "was", "we", "well", "were", "what", 
            "whatever", "when", "whence", "whenever", "where", "whereafter", "whereas", "whereby", "wherein", 
            "whereupon", "wherever", "whether", "which", "while", "whither", "who", "whoever", "whole", 
            "whom", "whose", "why", "will", "with", "within", "without", "would", "yet", "you", "your", 
            "yours", "yourself", "yourselves",  "mahatma","gandhi","mohandas", "karamchand","gandhiji", "mk",
            
        };

        vector<string> element(0);
        H_Table.resize(100,element);
        for(string word:stopwords){
            insert_in_hash_map(word);
        }
    }

    int hash_function(string word){
        int hv=0;
        int itr=1;
        for(char c:word){
            hv+=(c-'31')*itr;
            hv=hv%100;
            if (itr<5){
            itr++;}
        }
        return abs(hv%100);
    }

    bool is_stop_word(string word){
        int hv=hash_function(word);
        bool found=false;
        int sizen=H_Table[hv].size();
        if(sizen==0){
            found=false;
        }

        for(string stopword:H_Table[hv]){
            if(word==stopword){found=true;
            break;}
        }
        return found;
    };

    void insert_in_hash_map(string word){
        int hv=hash_function(word);
        H_Table[hv].push_back(word);
    };

};

struct Account {
    std::string id;
    long long balance;
};

class WordHash {
public:
    WordHash();
    int hash(std::string id);
    void createAccount(std::string id, long long count);
    long long getBalance(std::string id);
    void addTransaction(std::string id, long long count);
    bool doesExist(std::string id);

    std::vector<std::vector<Account> > bankStorage2d;

    int p[100] = {};
    int capacity = 500;
    int BaseSize=0;
    
};


class QNA_tool {

private:

    void query_llm(string filename, Node* root, int k, string API_KEY, string question);
    vector<vector<vector<vector<string> > > > corpus;

    int last_inserted_book_code = 0;
    int last_inserted_page_number = -1;
    int last_inserted_para_number = -1;

public:

    Dict dictionary; 

    Dict CompressedDict;

    Stop_Word_Hash stopwords=Stop_Word_Hash();

    WordHash MassFrequency; 

    WordHash CompressedMassFrequency;
    
    QNA_tool();
    ~QNA_tool(); 

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    Node* get_top_k_para(string question, int k);

    std::string get_paragraph(int book_code, int page, int paragraph);

    void query(string question, string filename, string API_KEY);

    Node* compressed_get_top_k_para(string question, int k);

};

