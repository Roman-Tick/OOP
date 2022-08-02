#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

std::vector<std::string> read_file(string file_name) //reads lines from a file and returns a string vector containing all those lines
{
    vector<string> words; //vector to store words from file
    string line; //string to store each line from file

    ifstream file (file_name); // chose which file to open
    if(file.is_open())  //keeps file open and allows us to read from it - maybe move this to seperate function
    {
        while(getline(file, line)) //for each line in file
        {
            words.push_back(line); //add line form file to words vector
        }
        file.close();
    }
    return words;
}

std::vector<std::string> fit_to_word_size(vector<string> word_list, int word_size) //takes string vector and removes all words that dont = word_size
{
    if(word_size == -1)
    {
        return word_list;
    }
    vector<string> new_list;
    for(int i=0; i<word_list.size();i++) //for all words in word_list
    {
        if(word_list[i].size() == word_size)
        {
            new_list.push_back(word_list[i]);
        }
    }
    return new_list;
}

class Game
{
    private:
        int table_size = 0; //size of table/grid
        int word_size = -1; //size of the words we are looking for
        vector<string> found_words; //stores all words that have been foud 
        vector<vector<char>> puzzle_table; //2D char vector that holds values of table/grid

    public:
        Game(int argc, char * argv[])
        {
            string file_name = argv[1]; // had to make this string because find_first_not_of would not accept char*
            if (file_name.find_first_not_of("0123456789") != string::npos) //checks if first user input is a number 
            {
                create_table_from_file(argv[1]); // creates table from file that user has input
            }
            else //create table from table size
            {
                table_size = atoi(argv[1]); //sets table size from user input
                create_random_table(); //create table of random chars
            }

            if (argc>2) //checks if two strings were input by the user
            {
                word_size = atoi(argv[2]);
            }
            
            //print_table(); // for debugging 
            word_search();
        }

        Game(Game &other) //copy constructor - sets all values the same 
        {
            table_size = other.table_size;
            word_size = other.word_size;
            puzzle_table = other.puzzle_table;
            word_search();
        }

        Game operator=(Game &other) //assignment operator
        {
            return *this;
        }

        std::vector<std::string> found() // getter - returns list of found words
        {
            return found_words;
        }

        vector<vector<char>> get_puzzle_table()  //getter - returns puzzle_table
        {
            return puzzle_table;
        }

        int get_word_size() // getter - returns word size
        {
            return word_size;
        }

        void word_search() // performs word search on grid
        {
            vector<string> search_words; //list to store the words the function is looking for
            search_words = read_file("Dictionary.txt"); //grabs words from Dictionary.txt
            for(int i=0;i<search_words.size();i++){search_words[i].pop_back();} //eraes the last char of each word - this is because the file Dictionary.txt contains \n after each word
            search_words = fit_to_word_size(search_words, word_size); // clears all words that dont = word_size

            int flag = 0; //this is to stop searching the same line when it searches the diagonals
            for(int i=0;i<search_words.size();i++) //for all words in search words
            {
                for(int row=0;row<puzzle_table.size();row++) //for each row in puzzle_table
                {
                    search_row(search_words[i], row); //search rows of puzzle_table
                    search_column(search_words[i], row); //search columns of puzzle_table
                    search_diagonals(search_words[i], row, flag); //search diagonals of puzzle_table
                    flag = 1;
                }
                flag = 0;
            }
        }

        void search_diagonals(string search_word, int row, int flag) //searchs both diagonals in puzzle_table for a single word
        {
            for(int col=0;col<puzzle_table.size();col++) //for each column in puzzle_table
            {
                search_right_diagonal(search_word, row, col, flag); //search diagonals going from left to right
                search_left_diagonal(search_word, row, col, flag); //search diagonals going from right to left
                if(flag == 1){break;} //breaks for loop so it doesnt keep searching in the same diagonal
            }
        }

        void search_left_diagonal(string search_word, int row, int col, int flag) //search diagonals going from right to left
        {
            string diagonal = ""; // string to hold all chars in diagonal
            int temp_col = puzzle_table[0].size()-1-col; //temporary value for colunm because we need to keep the original
            int temp_row = row; //temporary value for row because we need to keep the original
            while(temp_row<puzzle_table.size() && temp_col>=0) // add all chars in diagonal to string one by one, starts at bottom and goes up
            {
                diagonal += puzzle_table[temp_row][temp_col];
                temp_row++;
                temp_col--;
            }
            
            temp_row = diagonal.find(search_word); //returns index where the found word starts, returns -1 if the word is not found
            if(flag == 1)
            {
                temp_col = puzzle_table[0].size()-temp_row; //for below middle diagonal 
            }
            else
            {
                temp_col = puzzle_table[0].size()-temp_row-col; //for above middle diagonal
            }
            if(temp_row != -1) //if word is found
            {
                add_to_found_list(row+temp_row+1, temp_col, search_word, "Direction is Left Diagonal"); //adds word to found list
            }
            reverse(diagonal.begin(), diagonal.end()); // reverse string so it can search if the word is going the other way
            temp_row = diagonal.find(search_word); //returns index where the found word starts, returns -1 if the word is not found
            if(flag == 1)
            {
                temp_col = puzzle_table[0].size()-diagonal.size()+temp_row+1; //for below middle diagonal 
            }
            else
            {
                temp_col = temp_row+1; //for above middle diagonal
            }
            if(temp_row != -1) //if word is found
            {
                add_to_found_list(diagonal.size()-temp_row+row, temp_col, search_word, "Direction is Reverse Left Diagonal"); //adds word to found list
            }
        }

        void search_right_diagonal(string search_word, int row, int col, int flag)
        {
            string diagonal = ""; // string to hold all chars in diagonal
            int temp_col = col; //temporary value for colunm because we need to keep the original
            int temp_row = row; //temporary value for row because we need to keep the original
            while(temp_row<puzzle_table.size() && temp_col<puzzle_table[0].size()) // add all chars in diagonal to string one by one, starts at top and goes down
            {
                diagonal += puzzle_table[temp_row][temp_col];
                temp_row++;
                temp_col++;
            }
            temp_row = diagonal.find(search_word); //returns index where the found word starts, returns -1 if the word is not found
            if(temp_row != -1) //
            {
                add_to_found_list(row+temp_row+1, col+temp_row+1, search_word, "Direction is Right Diagonal"); //adds word to found list
            }
            reverse(diagonal.begin(), diagonal.end());
            temp_row = diagonal.find(search_word); //returns index where the found word starts, returns -1 if the word is not found
            if(flag == 1) //if word is found
            {
                col = diagonal.size()-temp_row; //for below middle diagonal 
            }
            else
            {
                col = diagonal.size()-temp_row+col; //for above middle diagonal
            }
            if(temp_row != -1) //if word is found
            {
                add_to_found_list(diagonal.size()-temp_row+row, col, search_word, "Direction is Reverse Right Diagonal"); //adds word to found list
            }

        }

        void search_column(string search_word, int col)
        {
            string col_string = ""; // string to hold all chars in column
            for(int i=0;i<puzzle_table.size();i++) //make row of puzzle_table into string
            {
                col_string += puzzle_table[i][col];
            }
            
            int row = col_string.find(search_word); //returns index where the found word starts, returns -1 if the word is not found
            if(row != -1) //if word is found
            {
                add_to_found_list(row+1, col+1, search_word, "Direction is Vertical"); //adds word to found list
            }
            reverse(col_string.begin(), col_string.end()); //reverse row_string to see if the word is backwards
            row = col_string.find(search_word); //returns index where the found word starts, returns -1 if the word is not found
            if(row != -1) //if word is found
            {
                add_to_found_list(col_string.size()-row, col+1, search_word, "Direction is Reverse Vertical"); //adds word to found list
            }
        }

        void search_row(string search_word, int row)
        {
            string row_string = ""; // string to hold all chars in row
            for(int i=0;i<puzzle_table[row].size();i++) //make row of puzzle_table into string
            {
                row_string += puzzle_table[row][i];
            }
            
            int col = row_string.find(search_word); //returns index where the found word starts, returns -1 if the word is not found
            if(col != -1) //if word is found
            {
                add_to_found_list(row+1, col+1, search_word, "Direction is Horizontal"); //adds word to found list
            }
            reverse(row_string.begin(), row_string.end()); //reverse row_string to see if the word is backwards
            col = row_string.find(search_word); //returns index where the found word starts, returns -1 if the word is not found
            if(col != -1) //if word is found
            {
                add_to_found_list(row+1, row_string.size()-col, search_word, "Direction is Reverse Horizontal"); //adds word to found list
            }
        }

        void add_to_found_list(int row, int col, string word, string description) //adds word to found list along with its location and description
        {
            string new_word = "";
            new_word += word;
            new_word += " @(";
            new_word += to_string(row);
            new_word += ",";
            new_word += to_string(col);
            new_word += "), ";
            new_word += description;
            found_words.push_back(new_word);
        }

        void create_random_table() //creates table of random chars of table_size
        {
            vector<char> row;
            for(int i=0; i<table_size; i++) //for each row
            {
                for(int j=0; j<table_size; j++) //for each column
                {
                    row.push_back(97 + rand() % 26); //random char
                }
                puzzle_table.push_back(row); //add row to table
                row.clear(); //clear row so we can add new char's 
            }
        }

        void create_table_from_file(string file_name) //grabs words from a file and creates a table from them
        {
            vector<string> words; //vector of strings to store words from file
            words = read_file(file_name); // grabs words from file
            vector<char> row; //vector to store row of char's
            for(int i=0; i<words.size(); i++) //for each row 
            {
                for(int j=0; j<words[i].size(); j++) //for each column
                {
                    row.push_back(words[i][j]); //add letter from word to puzzle_table
                }
                puzzle_table.push_back(row); // add row to puzzle_table
                row.clear(); //clear row so we can add new char's 
            }
            
        }

        void print_table() //prints table with numbered rows and columns - helpful when debugging
        {
            for (int i = 0; i < puzzle_table.size(); i++) // for each row
            {
                if(i<9){cout << i + 1 << "     ";} //i dont like this - think of another way
                else{cout << i + 1 << "    ";}
                
                for (int j = 0; j < puzzle_table[i].size(); j++) // for each column
                {
                    cout << puzzle_table[i][j] << " "; //prints char from puzzle_table
                }    
                cout << endl;
            }
            cout << endl << "      "; //debug - puts numbers at bottom
            for(int i = 0; i < puzzle_table[0].size(); i++) //debug - puts numbers at bottom
            {
                cout << i+1 << " ";
            }
            cout << endl; //debug - puts numbers at bottom
        }
        
};

std::ostream& operator<<(std::ostream &s, Game &game) //operator overloader - prints puzzle_table 
{
    vector<vector<char>> puzzle_table = game.get_puzzle_table(); //grabs puzzle_table from game
    for (int i = 0; i < puzzle_table.size(); i++) //for each row
    {
        if(i<9){cout << i + 1 << "     ";} //i dont like this -- think of another way
        else{cout << i + 1 << "    ";}
                
        for (int j = 0; j < puzzle_table[i].size(); j++) //for each column
        {
            cout << puzzle_table[i][j] << " "; //prints char from puzzle_table
        }    
        cout << endl;
    }
    return s << " "; //dont know how to return what is obove but function needs to return something
}

bool operator!=(Game &game1, Game &game2) //checks if two game objects have the same contents 
{
    if(game1.get_puzzle_table() == game2.get_puzzle_table() && game1.found() == game2.found()
    && game1.get_word_size() == game2.get_word_size())
    {
        return false;
    }
    return true;
}

string operator==(Game &game1, Game &game2) //checks if two game objects have the same contents 
{
    if(game1 != game2)
    {
        return "false";
    }
    return "true";
}

int main(int argc, char * argv[]) //from assignment sheet
{
    auto start = clock();
    Game game(argc, argv);
    cout << game << endl;
    vector<string> words = game.found();
    for(auto l : words)
        cout << l << endl;
    cout << "Found " << words.size() << " words" << endl;
    auto cpu_time = clock() - start;
    cout << "Cpu time = " << double(cpu_time) / CLOCKS_PER_SEC << endl;
    Game game1(game);
    Game game2 = game1;
    if(game1 != game2)
        cout << "Error in game copy 1" << endl;
    if (game2 != game)
        cout << "Error in game copy 2" << endl;
    cout << "Games same " << (game2 == game1) << endl;
    return 0; 
}