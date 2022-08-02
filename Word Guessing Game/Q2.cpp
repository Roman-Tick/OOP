#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <bits/stdc++.h>
#include <vector>
#include <new>
#include <ctime>
//#include <algorithm>

using namespace std;

struct sub_sequence{
    string word;
    int amount;
};

std::vector<std::string> read_file()
{
    vector<string> words;
    string line;

    ifstream file ("Dictionary.txt"); // chose which file to open
    if(file.is_open())  //keeps file open and allows us to read from it - maybe move this to seperate function
    {
        while(getline(file, line)) //for each line in file
        {
            words.push_back(line);
        }
        file.close();
    }
    return words;
}

class find_sub_strings
{
    private:
        vector<sub_sequence> word_rankings;

    public:
        find_sub_strings() //constructor  -  old-(vector<int> sizes, vector<string> words)
        {
            /*
            //could also move this section outside the object and create a new object for each substring size
            //for each size, it searchs for substrings then prints them
            for(int i=0; i<sizes.size(); i++) //for each size in size list
            {
                search(sizes[i], words);
                print_rank(sizes[i]);
                word_rankings.clear(); //delete all items in vector so it can start again with new size
            }
            */
        }

        int find_in_list(string sub_string) //checks if substring is in word_rankings list
        {
            for(int i=0; i<word_rankings.size(); i++) //for each struct in word_rankings
            {
                if(word_rankings[i].word == sub_string) //if substring was found - increase amount
                {
                    word_rankings[i].amount++;
                    return 0;
                }
            }

            //if substring not found - add it to list
            sub_sequence new_string;
            new_string.word = sub_string;
            new_string.amount = 1;
            word_rankings.push_back(new_string);
            return 1;
        }

        string get_sub(int index, string word, int sub_size) //returns substring at a given position of given size
        {
            string sub_string;
            for(int i=0; i<sub_size; i++) //number of cycles/loops = sub string size
            {
                sub_string = sub_string + word[index]; //concatenate each letter at position to substring
                index++;
            }

            return sub_string;
        }

        void search(int sub_size, vector<string> words) //searches for substrings 
        {
            for(int i=0; i<words.size(); i++) //for each word 
            {
                for(int l=0; l<=(words[i].size()-sub_size); l++) //loops through each letter for string
                {
                    string sub_string = get_sub(l, words[i], sub_size); //returns substring at index l of given size
                    find_in_list(sub_string); //adds substring to the word rankings list
                }
            }
        }

        //insertion sort
        void sort_list() //ranks each struct (sub_suqence) in word_rankings by there amount (highest to lowest)
        {
            int j;
            for(int i=1; i<word_rankings.size(); i++) //for each struct in word_rankings
            {
                string word_holder = word_rankings[i].word; //stores the word/string from struct we are currently checking
                int amount_holder = word_rankings[i].amount;
                j=i;
                while(j>0 && word_rankings[j-1].amount < amount_holder) //checks if all elements to the left are smaller than
                {
                    word_rankings[j].amount = word_rankings[j-1].amount;
                    word_rankings[j].word = word_rankings[j-1].word;
                    j--;
                }
                word_rankings[j].amount = amount_holder; //insert in correct position
                word_rankings[j].word = word_holder;
            }
        }

        void print_rank(int sub_size) //prints top 10 most common sub strings in the word list
        {
            cout << "Found " << word_rankings.size() << " sub-sequences of length " << sub_size << endl;
            sort_list(); //sorts word list (word_rankings) in decending order

            for(int i=0; i<10; i++) //for top 10 - print members
            {
                cout << word_rankings[i].word << " " << word_rankings[i].word << endl;
            }
        }
};

int check_lettering(string user_input, string word)
{
    int difference = 0;
    for(int i=0;i,user_input.size();i++)
    {
        if(i>=word.size())
        {
            break;
        }
        if(user_input[i] != word[i])
        {
            difference++;
        }
    }
    return difference;
}

std::vector<std::vector<std::string>> add_to_close_words(std::vector<vector<string> > close_words, string file_word, string user_input) //std::vector<std::string>
{ 
    int inserted = 0;
    vector<string> new_vec;
    new_vec.push_back(file_word);
    vector<vector<string>> new_close_words;
    for(int i=0;i<close_words.size();i++)
    {
        if(check_lettering(user_input, file_word) == check_lettering(user_input, close_words[i][0]))
        {
            close_words[i].push_back(file_word);
            inserted = 1;
        }
        else if(check_lettering(user_input, close_words[i][0]) > check_lettering(user_input, file_word))
        {
            if(inserted == 0)
            {
                inserted = 1;
                new_close_words.push_back(new_vec);
            }
        }
        new_close_words.push_back(close_words[i]);
    }
    if(inserted == 0)
    {
        new_close_words.push_back(new_vec);
    }
    return new_close_words;
}

void spell_check(string file_name)
{
    string user_input;
    cout << "Enter a word you would like to spell check: ";
    getline(cin, user_input);
    string line; //create string to hold each line of the file
    ifstream file (file_name); // choose which file to open
    vector<vector<string> > close_words; 
    int found = 0;
    char current_letter = 'a';

    cout << "Loading"; 
    if(file.is_open())  //keeps file open and allows us to read from it
    {
        while(getline(file, line)) //for each line in file
        {
            if(line == user_input) //if letter matchs the hint we are looking for then print it for the user
            {
                cout << endl << "The word *" << user_input << "* is spelt correctly. " << endl << endl;
                found = 1;
                break;
            }
            close_words = add_to_close_words(close_words, line, user_input);
            if(line[0] != current_letter)
            {
                cout << ".";
                current_letter = line[0];
            }
        }
        file.close();
    }

    if(found == 1){return;}
    cout << endl << "Word is not recognised. Similar spellings: ";
    //can also print more close words by printing close_words[1] and close_words[2]
    for(int j=0;j<close_words[0].size();j++)
    {
        cout << close_words[0][j] << ", ";
    }
}

void find_sub_sequences()
{
    vector<string> words; //vector to store all words from file in
    words = read_file(); //returns all words from file

    vector<int> sizes; //vector to store all string sizes that want to be tested 
    sizes.push_back(2);
    sizes.push_back(3);
    sizes.push_back(4);

    //find_sub_strings test(sizes, words); //create object
    
    find_sub_strings length2;
    length2.search(2, words);
    length2.print_rank(2);

    find_sub_strings length3;
    length3.search(3, words);
    length3.print_rank(3);
}

int main(int argc, char *argv[])
{
    clock_t start_time = clock();
    if(argc == 3)
    {
        vector<string> words;
        words.push_back(argv[1]);
        words.push_back(argv[2]);
    
        if(words[0] == "read") //if user enters less than 2 arguments in command line. if they enter more, the program just ignores it   argv[2].erase(0,1)
        {
            find_sub_sequences();
        }
        else if(words[0] == "check")
        {
            spell_check(words[1]);
        }
        else
        {
            cout << "Enter a valid File and Action. Eg *check Dictionary.txt*" << endl;
        }
    }
    else
    {
        cout << "Please Enter action followed by a file name. Eg *check Dictionary.txt*" << endl;
    }
    
    clock_t end_time = clock();
    cout << "Elasped Time: " << (double(end_time - start_time)/CLOCKS_PER_SEC) << "secs";
    return 0;
}


