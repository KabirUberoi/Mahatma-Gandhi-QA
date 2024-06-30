// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class address{
    public: 
    int book_code;
    int page;
    int paragraph; 
    int sentence_no;

    address(int book_code, int page, int paragraph, int sentence_no){
        this->book_code = book_code; 
        this->page = page; 
        this->paragraph = paragraph; 
        this->sentence_no = sentence_no; 
    }

    bool operator == (const address &ad){ 
        if ((this->book_code==ad.book_code) && (this->page==ad.page) && (this->paragraph==ad.paragraph) && (this->sentence_no==ad.sentence_no))
            return true; 
        return false; 
    }

};

class StringHash{
    public: 
    string str;
    int count = 0; 
    vector<address> locations;

    StringHash(string str){
        this->str = str;
        this->count++;
    }
};

class Dict {
public: 
    long long p[128]={};
    vector<vector<StringHash> > HashMap;
    long long size = 0;
    long long capacity = 4096;

    long long hash(string str);

    vector<address> get_locations(string word);

    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    long long get_word_count(string word);

    void dump_dictionary(string filename);
};