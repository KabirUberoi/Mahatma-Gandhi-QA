#include "dict.h"

Dict::Dict(){
    p[0]=1;
    for(int i=1; i<128; i++){
        p[i]=((p[i-1]*31)%capacity);
    }
    HashMap.resize(capacity);    
}

long long Dict::hash(string str){
    long long ans = 0;
    for(int i=0; i<str.length(); i++){
        ans+=((str[i])*p[i]);
        ans = ans%capacity;
    }
    return ans;
}

Dict::~Dict(){ 
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    string Sentence; 
    for(char x:sentence){Sentence+=tolower(x);}
    
    vector<string> words; string tempstr="";
    string spaces = " \".,-:!\"\'()?—[]˙;@";

    for(int i=0; i<Sentence.length(); i++){
        bool spaceCheck = 0;
        for(int j=0; j<spaces.length(); j++){
            if(Sentence[i]==spaces[j]){
                spaceCheck=1;
                if(tempstr!=""){
                    words.push_back(tempstr);
                    tempstr="";
                    break;
                }
            };
        }
        if(!spaceCheck){
            tempstr+=Sentence[i];
        }
    }

    if(tempstr!=""){words.push_back(tempstr);}

    for(string x:words){
        long long hval = hash(x);
        if(HashMap[hval].size()!=0){
            bool checking = 0;
            for(int i=0; i<HashMap[hval].size(); i++){
                if(HashMap[hval][i].str==x){
                    HashMap[hval][i].count++;
                    HashMap[hval][i].locations.push_back(address(book_code,page,paragraph,sentence_no));
                    checking=1;
                    break;
                }
            }
            if(checking == 0){
                StringHash temp(x);
                temp.locations.push_back(address(book_code,page,paragraph,sentence_no));
                HashMap[hval].push_back(temp);
            }
        }
        else{
            StringHash temp(x);
            temp.locations.push_back(address(book_code,page,paragraph,sentence_no));
            HashMap[hval].push_back(temp);
        }
    }

    size+=words.size();

    while(size>(capacity*2)){
        capacity=capacity*2;
    }
    
    while(size>=(0.6 * capacity)){
        this->capacity=this->capacity*2;
        for(int i=1; i<128; i++){
            p[i]=((p[i-1]*31)%capacity);
        }
        vector<vector<StringHash> > tempHash;
        tempHash.resize(capacity);
        for(int i=0; i<HashMap.size(); i++){
            if(HashMap[i].size()>0){
                for(StringHash x:HashMap[i]){
                    tempHash[hash(x.str)].push_back(x);
                }
            }
        }
        HashMap = tempHash;
    } 
    return;
}

long long Dict::get_word_count(string word){
    long long hval = hash(word);
    if(HashMap[hval].size()!=0){
        for(int i=0; i<HashMap[hval].size(); i++){
            if(HashMap[hval][i].str==word){return HashMap[hval][i].count;}
        }
    }
    return -1;
}

vector<address> Dict::get_locations(string word){
    long long hval = hash(word);
    if(HashMap[hval].size()!=0){
        for(int i=0; i<HashMap[hval].size(); i++){
            if(HashMap[hval][i].str==word){return HashMap[hval][i].locations;}
        }
    }
    vector<address> ans;
    return ans;
}

void Dict::dump_dictionary(string filename){
    ofstream outputFile; 
    outputFile.open(filename);
    if (outputFile.is_open()) {
        for(int i=0; i<HashMap.size(); i++){
            if(HashMap[i].size()!=0){
                for(int j=0; j<HashMap[i].size(); j++){
                    outputFile << HashMap[i][j].str<<", "<<HashMap[i][j].count<<"->"<<"\n";
                    for(auto x: HashMap[i][j].locations){
                        outputFile << x.book_code <<" "<<x.page<<" "<<x.paragraph<<"\n";
                    }
                }
            }
        }
        outputFile.close();
    }
    return;
}