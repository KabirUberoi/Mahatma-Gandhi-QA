#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

bool Is_Vowel(char ch){
    if(ch=='a' || ch=='e' || ch=='i' || ch=='o' || ch=='u'){return true;}
    else{return false;}
}

bool Find_Double(string s){
    if(s.size()<2){return false;}
    else{
        if(s[s.size()-1]!=s[s.size()-2]){
            return false;
        }
        else if(s[s.size()-1]==s[s.size()-2] && s[s.size()-1]=='l' || s[s.size()-1]=='s' || s[s.size()-1]=='z'){
            return false;
        }
        else{
            return true;
        }
    }
}

bool Check_Vowel(string s){
    for(int i=0;i<s.size();i++){
        if(Is_Vowel(s[i])==true){
            return true;
        }
        else{
            continue;
        }
    }
    return false;
}


int Find_M(string s){
    int m=0;
    int curr; 
    string str="";

    if(Is_Vowel(s[0])){
        curr=0;
        str=str+"v";
    }
    else{
        curr=1;
        str=str+"c";
    }

    for(int i=0;i<s.length();i++){
        if(Is_Vowel(s[i]) and curr==1){
            str=str+"v";
            curr=0;
        }
        else if(Is_Vowel(s[i])==false and curr==0){
            str=str+"c";
            curr=1;
        }
    }
    
    if(str=="v"){
        return 0;
    }
    else{
        if(str[str.length()-1]=='v'){
            str.pop_back();
        }
        for(int i=0;i<str.length();i++){
            if(str[i]=='v'){
                m++;
            }
        }
        return m;
    }
}

string porter_stemmer(string word){
    string compressed="";

    if(word.size()>=4 and word.substr(word.length()-4,4)=="sses"){
        return word.substr(0,word.length()-4)+"ss"; 
    }
    else if(word.size()>=3 and word.substr(word.length()-3,3)=="ies"){
        return word.substr(0,word.length()-3)+"i";
    }
    else if(word.size()>=1 and word.substr(word.length()-1,1)=="s"){
        return word.substr(0,word.length()-1);
    }
    else if(word.size()>=3 and Find_M(word.substr(0,word.length()-3))>0 and word.substr(word.length()-3,3)=="eed" ){
        return word.substr(0,word.length()-3)+"ee";
    }
    else if(word.size()>2 and Check_Vowel(word.substr(0,word.length()-2))==true and word.substr(word.length()-2,2)=="ed" and word[word.size()-3]!='e'){
        compressed=word.substr(0,word.length()-2);
        return word.substr(0,word.length()-2);
    }
    else if(word.size()>3 and Check_Vowel(word.substr(0,word.length()-3))==true and word.substr(word.length()-3,3)=="ing"){
        compressed=word.substr(0,word.length()-3);
        if(Find_Double(compressed)==true){
            compressed.pop_back();
            return compressed;
        }
        return compressed;
    }
    else if(word.size()>1 and Check_Vowel(word.substr(0,word.size()-1)) and word[word.size()-1]=='y'){
        word.pop_back();
        word=word+"i";
        return word;
    }
    else if(word.size()>7 and Find_M(word.substr(0,word.size()-7))>0 and word.substr(word.length()-7,7)=="ational"){
        return word.substr(0,word.length()-7)+"ate";    
    }
    else if((word.size()>7 and Find_M(word.substr(0,word.size()-7))>0 and word.substr(word.length()-7,7)=="ization")){
        return word.substr(0,word.length()-7)+"ize";
    }
    else if((word.size()>6 and Find_M(word.substr(0,word.size()-6))>0 and word.substr(word.length()-6,6)=="biliti")){
        return word.substr(0,word.length()-6)+"ble";
    }
    else if((word.size()>5 and Find_M(word.substr(0,word.size()-5))>0 and word.substr(word.length()-5,5)=="icate")){
        return word.substr(0,word.length()-5)+"ic";
    }
    else if((word.size()>3 and Find_M(word.substr(0,word.size()-3))>0 and word.substr(word.length()-3,3)=="ful")){
        return word.substr(0,word.length()-3);
    }
    else if((word.size()>4 and Find_M(word.substr(0,word.size()-4))>0 and word.substr(word.length()-4,4)=="ness")){
        return word.substr(0,word.length()-4);
    }
    else if((word.size()>4 and Find_M(word.substr(0,word.size()-4))>0 and word.substr(word.length()-4,4)=="ance")){
        return word.substr(0,word.length()-4);
    }
    else if((word.size()>3 and Find_M(word.substr(0,word.size()-3))>0 and word.substr(word.length()-3,3)=="ent")){
        return word.substr(0,word.length()-3);
    }
    else if((word.size()>3 and Find_M(word.substr(0,word.size()-3))>0 and word.substr(word.length()-3,3)=="ive")){
        return word.substr(0,word.length()-3);
    }

    return word;
}

Chaining::Chaining(){
    p[0]=1;
    for(int i=1; i<3; i++){
        p[i]=((p[i-1]*31));
    }
    bankStorage2d.resize(capacity);
}

int Chaining::hash(ShortAddress location) {
    int ans =(((location.book_code*p[2])%capacity) + ((location.page*p[1])%capacity) + ((location.paragraph)%capacity));
    ans = ans%capacity; 
    return ans; 
}

void Chaining::createScore(ShortAddress location, long long score) {
    if(BaseSize>=(0.6 * capacity)){
        capacity=capacity*2;
        vector<vector<Score>> tempHash;
        tempHash.resize(capacity);
        for(int i=0; i<bankStorage2d.size(); i++){
            if(bankStorage2d[i].size()>0){
                for(Score x: bankStorage2d[i]){
                    tempHash[hash(x.location)].push_back(x);
                }
            }
        }
        bankStorage2d = tempHash;
    }
    Score temp = {location,score};
    int hval = hash(location);
    bankStorage2d[hval].push_back(temp);
    nonZero.push_back(location);
    BaseSize++;
}

std::vector<ShortAddress> Chaining::getTopK(int k) {
    
    if(BaseSize==0){vector<ShortAddress> v={}; return v;}
    top_k heap; 
    heap.k = k; 
    for(ShortAddress x: nonZero){
        heap.add({getscore(x),x});
    }
    vector<ShortAddress> TempAns; 
    vector<ShortAddress> ans; 
    while(heap.heap.size()!=0){
        TempAns.push_back(heap.remove()); 
    }
    for(int i=(TempAns.size()-1); i>=0; i--){
        ans.push_back(TempAns[i]);
    }
    return ans;
}

long long Chaining::getscore(ShortAddress location) {
    int hval = hash(location);
    if(bankStorage2d[hval].size()!=0){
        for(int i=0; i<bankStorage2d[hval].size(); i++){
            if(bankStorage2d[hval][i].location==location){return bankStorage2d[hval][i].score;}
        }
    }
    return -1;
}

void Chaining::addScore(ShortAddress location, long long score) {
    int hval = hash(location);
    if(bankStorage2d[hval].size()==0){
        createScore(location,score);
    }
    else{
        int c = 0;
        for(int i=0; i<bankStorage2d[hval].size(); i++){
            if(bankStorage2d[hval][i].location==location){
                bankStorage2d[hval][i].score+=score;
                break;
            }
            else{c++;}
        }
        if(c==bankStorage2d[hval].size()){
            createScore(location,score);
        } 
    }
}

bool Chaining::doesExist(ShortAddress location) {
    int hval = hash(location);
    if(bankStorage2d[hval].size()!=0){
        for(int i=0; i<bankStorage2d[hval].size(); i++){
            if(bankStorage2d[hval][i].location==location){return true;}
        }
    }
    return false; 
}

int Chaining::databaseSize() {
    return BaseSize; 
}

WordHash::WordHash(){
    p[0]=1;
    for(int i=1; i<100; i++){
        p[i]=((p[i-1]*31)%capacity);
    }
    bankStorage2d.resize(capacity);
}

int WordHash::hash(std::string id) {
    int ans = 0;
    for(int i=0; i<id.size(); i++){
        ans+=((id[i]-'a')*p[i]);
        ans = ans%capacity;
    }
    return ans; // Placeholder return value
}

void WordHash::createAccount(std::string id, long long count) {
    if(BaseSize>=(0.6 * capacity)){
        capacity=capacity*2;
        for(int i=1; i<100; i++){
            p[i]=((p[i-1]*31)%capacity);
        }
        vector<vector<Account>> tempHash;
        tempHash.resize(capacity);
        for(int i=0; i<bankStorage2d.size(); i++){
            if(bankStorage2d[i].size()>0){
                for(Account x: bankStorage2d[i]){
                    tempHash[hash(x.id)].push_back(x);
                }
            }
        }
        bankStorage2d = tempHash;
    }
    Account temp = {id,count};

    int hval = hash(id);
    bankStorage2d[hval].push_back(temp);
    BaseSize++;
}


long long WordHash::getBalance(std::string id) {
    int hval = hash(id);
    if(bankStorage2d[hval].size()!=0){
        for(int i=0; i<bankStorage2d[hval].size(); i++){
            if(bankStorage2d[hval][i].id==id){return bankStorage2d[hval][i].balance;}
        }
    }
    return -1;
}

void WordHash::addTransaction(std::string id, long long count) {
    int hval = hash(id);
    if(bankStorage2d[hval].size()==0){
        createAccount(id,count);
    }
    else{
        int c = 0;
        for(int i=0; i<bankStorage2d[hval].size(); i++){
            if(bankStorage2d[hval][i].id==id){
                bankStorage2d[hval][i].balance+=count;
                break;
            }
            else{c++;}
        }
        if(c==bankStorage2d[hval].size()){
            createAccount(id,count);
        } 
    }
}

bool WordHash::doesExist(std::string id) {
    int hval = hash(id);
    if(bankStorage2d[hval].size()!=0){
        for(int i=0; i<bankStorage2d[hval].size(); i++){
            if(bankStorage2d[hval][i].id==id){return true;}
        }
    }
    return false;
}

QNA_tool::QNA_tool(){
    ifstream inputFile; 
    inputFile.open("unigram_freq.csv");
    string line = "";

    getline(inputFile,line);
    string word; 
    long long frequency;
    string temp;
    long long itr=1;
    string s;
    char c;
    stringstream inputString(line);

    getline(inputString,word,',');
    getline(inputString,temp);

    line = "";
    int i=0; 
    while(getline(inputFile,line)){
        stringstream inputString(line);

        getline(inputString,word,',');
        getline(inputString,temp);

        //frequency = stol(temp.c_str());
        frequency=0;
        s=temp.c_str();
        itr=1;
        for(long long i=s.size()-1;i>=0;i--){
            c=s[i];
            frequency+=(c-'0')*itr;
            itr*=10;
        }

        MassFrequency.addTransaction(word,frequency);
        word = porter_stemmer(word);
        CompressedMassFrequency.addTransaction(word,frequency);
        line = "";
    }
}

QNA_tool::~QNA_tool(){
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    dictionary.insert_sentence(book_code,page,paragraph,sentence_no,sentence);
    
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
        string temp = porter_stemmer(x);
        CompressedDict.insert_sentence(book_code,page,paragraph,sentence_no,temp);
    }
    return;
}

Node* QNA_tool::get_top_k_para(string question, int k) {
    string Sentence; 
    for(char x:question){Sentence+=tolower(x);}
    
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
    Chaining scoreKeeping; 

    for(string x:words){
        vector<address> wordLocations = dictionary.get_locations(x);
        for(auto y:wordLocations){
            ShortAddress addr(y.book_code,y.page,y.paragraph);
            long long increment = (1e12/MassFrequency.getBalance(x) * dictionary.get_word_count(x));
            scoreKeeping.addScore(addr,increment);
        }
    }
    vector<ShortAddress> TopAddress = scoreKeeping.getTopK(k);
    vector<Node*> ans; ans.resize(k,nullptr); 
    ShortAddress x = TopAddress[0];
    ans[0] = new Node(); 
    ans[0]->book_code = x.book_code;
    ans[0]->page =x.page;
    ans[0]->paragraph = x.paragraph;

    for(int i=1; i<ans.size(); i++){
        ShortAddress x = TopAddress[i];
        ans[i] = new Node(); 
        ans[i]->book_code = x.book_code;
        ans[i]->page =x.page;
        ans[i]->paragraph = x.paragraph;
        ans[i-1]->right = ans[i];
    }
    if (!ans.empty()) {
    ans.back()->right = nullptr;
}
    return ans[0];
}

Node* QNA_tool::compressed_get_top_k_para(string question, int k) {
    string Sentence = ""; 
    for(char x:question){Sentence+=tolower(x);}
    
    vector<string> words; string tempstr="";
    string spaces = " \".,-:!\"\'()?—[]˙;@";

    for(int i=0; i<Sentence.length(); i++){
        bool spaceCheck = 0;
        for(int j=0; j<spaces.length(); j++){
            if(Sentence[i]==spaces[j]){
                spaceCheck=1;
                if(tempstr!=""){
                    if(!stopwords.is_stop_word(tempstr)){
                    words.push_back(tempstr);}
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
    if(words.size()==0){words.push_back("mahatma"); words.push_back("gandhi");}

    for(int i=0; i<words.size(); i++){
        words[i] = porter_stemmer(words[i]);
    }

    Chaining scoreKeeping; 

    for(string x:words){
        vector<address> wordLocations = CompressedDict.get_locations(x);
        for(auto y:wordLocations){
            ShortAddress addr(y.book_code,y.page,y.paragraph);
            long long increment = (1e12/CompressedMassFrequency.getBalance(x) * CompressedDict.get_word_count(x));
            scoreKeeping.addScore(addr,increment);
        }
    }
    vector<ShortAddress> TopAddress = scoreKeeping.getTopK(k);
    vector<Node*> ans; ans.resize(k,nullptr); 
    ShortAddress x = TopAddress[0];
    ans[0] = new Node(); 
    ans[0]->book_code = x.book_code;
    ans[0]->page =x.page;
    ans[0]->paragraph = x.paragraph;

    for(int i=1; i<ans.size(); i++){
        ShortAddress x = TopAddress[i];
        ans[i] = new Node(); 
        ans[i]->book_code = x.book_code;
        ans[i]->page =x.page;
        ans[i]->paragraph = x.paragraph;
        ans[i-1]->right = ans[i];
    }
    if(!ans.empty()){
        ans.back()->right = nullptr;
    }
    return ans[0];
}

void QNA_tool::query(string question, string filename, string API_KEY){
    string to_feed_chatgpt;
    Node* n = compressed_get_top_k_para(question, 5);
    Node* current = n;
    
    to_feed_chatgpt.append("\nQ: ");
    to_feed_chatgpt.append(question);
    query_llm(filename,n,5,API_KEY,to_feed_chatgpt);

    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){
    // cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    std::string filename = "corpus/mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        std::string token;
        iss.ignore(1);

        int idx = 0;
        while(std::getline(iss, token, ',')){
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            if(token[0] == '\''){
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } 
            else{
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question){

    Node* traverse = root;
    int num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    ofstream outfile("query.txt");;
    outfile << question << endl;

    outfile.close();
 
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}