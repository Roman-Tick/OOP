#include <iostream>
#include <fstream>
#include <string>
#include <new>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

int compare(string s1, string s2, char letter) //checks if two words have the same letter positioning
{
    string s1_position = "0", s2_position = "0"; //stores the position of the letter for each word
    string current_position;
    char int_to_str[20]; //only here because int needs to be converted to char before it can be converted to string

    for(int i=0; i<s1.length(); i++) // loops through each letter in s1
    {
        if(s1[i] == letter) //if word has user input letter at this position
        {
            //concatenates position number to s1_position if it finds the letter within the word
            itoa(i+1, int_to_str, 10); //converts int i to char and stores in int_to_str
            current_position = string(int_to_str); //(or stringstream) converts char to string and stores in current_position
            s1_position = s1_position + current_position;
        }
    }

    for(int i=0; i<s2.length(); i++) //loops through each letter in s2 (same as above but with different word)
    {
        if(s2[i] == letter) //if word has user input letter at this position
        {
            //concatenates position number to s2_position if it finds the letter within the word
            itoa(i+1, int_to_str, 10);  //converts int i to char and stores in int_to_str
            current_position = string(int_to_str); //converts char to string and stores in current_position
            s2_position = s2_position + current_position;
        }
    }

    if(s1_position == s2_position) //checks if both words have the same letter positioning
    {
        return 0; //contains same letter positioning 
    }
    else
    {
        return 1; //doesnt contain same letter positioning 
    }
}

/**
 * @brief 
 * 
 * @param words 
 * @param letter_words 
 * @return std::vector<std::string> 
 */
std::vector<std::string> remove_letter_words(std::vector<string> words, std::vector<string> letter_words)
{
    for(int i = 0; i<letter_words.size(); i++) //loops through each word in letter word list
    {
        for(int j = 0; j<words.size();) //loops through total word list (conatins all words included in this round)
        {
            string current_word = words[j];
            if(current_word == letter_words[i]) // remove word if it contains the given letter
            {
                words.erase(words.begin()+j); //delete from words list 
            }
            else
            {
                j++; //only increase j here because element index changes when a word is removed 
            }
        }
    }
    return words; //returns all non letter words 
}

//returns list of words that contain the given letter 
std::vector<std::string> create_letter_list(std::vector<string> words, char letter)
{
    std::vector<string> letter_list; //create list of strings that contain the letter

    for(int w=0; w<words.size(); w++) //for all words in word list
    {
        for(int l=0; l<words[w].length(); l++) //for each letter in the word/position in string
        {
            if(words[w][l] == letter) //if it finds the given letter in the word
            {
                letter_list.push_back(words[w]); //adds word to letter list
                break;
            }
        }
    }
    return letter_list;
}

std::vector<std::string> get_biggest_category(std::vector<string> words, char letter) // sorts word list into categories
{
    vector<vector<string> > categories; //2D vector (vector of string vectors) to store each category
    int j = 0;

    for(int i=0; i<words.size(); i++) //for each word in list
    {
        for(j=0; j<categories.size(); j++) //for each row in categories - check the position of the first word in that row
        {
            if(compare(words[i], categories[j][0], letter) == 0) //same positioning
            {
                categories[j].push_back(words[i]);
                break;
            }
        }
        //if didnt find a match - (j = cat.size) create new string vector, add word[i] to vector, add vector to cat
        if(j == categories.size())
        {
            vector<string> new_vec; //create new vector to be added to categories
            new_vec.push_back(words[i]); //adds word to this new vector
            categories.push_back(new_vec); //adds new row to categories, inturn creating a new category 
            new_vec.pop_back(); //delete word from new vector - dont know if this really needs to be done
        }
    }

    //find biggest category and return it
    int row_num = 0;
    for(int x = 0; x<categories.size(); x++)
    {
        if(categories[x].size() > categories[row_num].size())
        {
            row_num = x;
        }
    }
    return categories[row_num];
}

string update(string word, string output_word, char letter) //updates computer ouput to reveal any hidden letters that the user got correct
{
    for(int i=0; i<word.length(); i++) //for every position/index in word
    {
        if(word[i] == letter) //where ever the letter is found in the word, it places the letter in the same position in the output word 
        {
            output_word[i] = letter; //basically "reveals" where the letter in found in the word
        }
    }
    return output_word; //returns updated word
}

string downgrade(string output_word, char letter) //removes letter from output word
{
    for(int i=0; i<output_word.length(); i++) //for every position/index in word
    {
        if(output_word[i] == letter) //where ever the letter is found is replaced by a _
        {
            output_word[i] = '_';
        }
    }
    return output_word; //returns updated word
}

int found(string final_word) // checks whether output word contains any "_" - so we know if the user has guessed the correct word
{
    for(int i=0; i<final_word.length(); i++) //for every position in word, check if it contains "_"
    {
        if(final_word[i] == '_')
        {
            return 1; //if it contains "_", we know the user has not guessed the correct word yet. therefor, we can return/exit function
        }
    }
    return 0; // if word contains no "_"
}

std::vector<std::string> get_words(int word_length)
{
    vector<string> words; // create vector/list to store all words from file
    string line; //create string to hold each line of the file

    ifstream file ("Dictionary.txt"); // choose which file to open
    if(file.is_open())  //keeps file open and allows us to read from it
    {
        while(getline(file, line)) //for each line in file
        {
            if(line.length() == word_length) //only add word to word list if it is the same length that the user has input
            {
                words.push_back(line); //add line from file to words
            }
        }
        file.close();
    }
    return words;
}

string create_dashs(int word_length)
{
    string output_word; //"hidden" word that gets displayed to user
    for(int i=0; i<word_length; i++) //set word to hangman format - set every position to "_"
    {
        output_word = output_word + "_";
    }
    return output_word;
}

void get_hint_from_file(char letter)
{
    string line; //create string to hold each line of the file
    ifstream file ("Riddles.txt"); // choose which file to open
    if(file.is_open())  //keeps file open and allows us to read from it
    {
        while(getline(file, line)) //for each line in file
        {
            if(line[0] == letter) //if letter matchs the hint we are looking for then print it for the user
            {
                cout << line.erase(0,1) << endl << endl;
            }
        }
        file.close();
    }
}

void hint(std::vector<string> words, std::vector<char> prev_letters) // gives user a hint of what the next letter might be 
{
    //this function gives a hint based on what the most common/least common letter in the word list is
    //this way if the user guesses the hint they are still against the odds/still have alot of words to choose from
    vector<string> letter_words;
    vector<string> non_letter_words;
    vector<string> biggest_cat;
    char most_common_letter;
    int most_common_letter_count = 0;
    char least_common_letter;
    int least_common_letter_count = 0;
    vector<char> all_letters = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

    cout << "Loading Hint";
    for(int i=0; i<all_letters.size(); i++)
    {
        if(find(prev_letters.begin(), prev_letters.end(), all_letters[i]) != prev_letters.end()){continue;} //if letter in prev_letters -> continue 
        
        letter_words = create_letter_list(words, all_letters[i]); // creates list of words that contain the given letter (all_letters[i])
        if(letter_words.size() != 0) // if there are no words with the current letter (all_letter[i]), then it skips this statement
        {
            non_letter_words = remove_letter_words(words, letter_words); //contains all words without all_letters[i]
            biggest_cat = get_biggest_category(letter_words, all_letters[i]); //letter words now equals the biggest category (of letter words)
            cout << ".";

            if(biggest_cat.size() > most_common_letter_count)
            {
                most_common_letter_count = biggest_cat.size();
                most_common_letter = all_letters[i];
            }

            if(non_letter_words.size() > least_common_letter_count)
            {
                least_common_letter_count = non_letter_words.size();
                least_common_letter = all_letters[i];
            }
        }
    }
    cout << endl; //for end of loading hint......
    if(most_common_letter_count > least_common_letter_count)
    {
        cout << "The word contains the answer to this riddle: " << endl;
        get_hint_from_file(most_common_letter);
    }
    else
    {
        cout << "The word does not contain the answer to this riddle: " << endl;
        get_hint_from_file(least_common_letter);
    }
}

int word_guessing(int word_length, int guesses)
{
    vector<string> words; // create vector/list to store all words from file
    words = get_words(word_length); //grabs a list of words for the file "Dictionary.txt" that are the same length the user has input

    string line; //create string to hold each line of the user input
    vector<string> letter_words; //create vector/list for all words that contain the input letter
    vector<char> prev_letters; //create vector/list for all previous chosen letters
    string output_word = create_dashs(word_length); //"hidden" word that gets displayed to user
    vector<vector<string> > prev_words; 

    while(guesses != 0)
    {
        cout << "Enter a Letter: ";
        getline(cin, line); //grabs input from user (whatever has been written on the input line)
        if(line.length() > 1) //checks if input only contains one character
        {
            if(line == "hint") // checks if user input = "hint"
            {
                hint(words, prev_letters); // gives user a hint of what the next letter might be 
            }
            else if(line == "undo") // checks if user input = "undo"
            {
                guesses++;
                char last_letter = prev_letters.back();//grabs last letter the user input
                prev_letters.erase(prev_letters.end() - 1); // pops last letter the user input
                words = prev_words.back();//words = previous word list
                prev_words.erase(prev_words.end() - 1);//pop last of prev words
                output_word = downgrade(output_word, last_letter); //removes last letter from output word
                cout << output_word << ", "; //displays word that the user needs to guess
                cout << "Guesses left = " << guesses << ", Words left = " << words.size() << endl;
            }
            else
            {
                cout << "Enter only one letter please. " << endl;
            }
        }
        else if(line.find_first_not_of("qwertyuioplkjhgfdsazxcvbnm") != string::npos) // checks if user input anything other than a letter
        {
            cout << "Only letter inputs are allowed. " << endl;
        }
        else if(find(prev_letters.begin(), prev_letters.end(), line[0]) != prev_letters.end())
        {
            //checks whether letter is in prev_letter vector (checks if letter has already been chosen)
            cout << "Already picked that letter. Try another. " << endl;
        }
        else
        {
            prev_words.push_back(words);
            letter_words = create_letter_list(words, line[0]); // creates list of words that contain the given letter
            words = remove_letter_words(words, letter_words); //words now contains all non given letter words

            if(letter_words.size() > words.size() ) //if there are more words containing the letter than words without
            {
                letter_words = get_biggest_category(letter_words, line[0]); //letter words now equals the biggest category (of letter words)
                if(letter_words.size() > words.size()) //if there are more words in this category than non letter words
                {
                    output_word = update(letter_words[0], output_word, line[0]); //updates output to user and reveals any hidden letters
                    words = letter_words; //new word list is now the biggest category of letter related words
                }
            }

            if(found(output_word) == 0) //if user guessed the correct word
            {
                cout << "Well done. You guessed the correct word (" << output_word << ")" << endl;
                exit(0);
            }
            else //user has not guessed the word yet
            {
                cout << output_word << ", "; //displays word that the user needs to guess
                guesses--;
                cout << "Guesses left = " << guesses << ", Words left = " << words.size() << endl;
                prev_letters.push_back(line[0]); //adds letter to vector/list of already picked letters
            }
        }
    }

    //reaches this point if the user has ran out of guesses and not found the word
    cout << "Sorry, the word was " << words[0] << " " << endl;
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc < 3) //if user enters less than 2 arguments in command line. if they enter more, the program just ignores it
    {
        cout << "Please enter word length then number of guesses allowed as arguments in command line. " << endl;
        exit(0);
    }

    word_guessing(atoi(argv[1]), atoi(argv[2]) );
    return 0;
}
