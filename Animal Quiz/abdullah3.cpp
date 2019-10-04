//Bashir Abdullah
//Animal quiz 
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

//providing start instructions.
void welcome()
{
    cout << "Enter at least five animal names, e.g., cat, dog, etc..." << endl;
}

//reading names until empty line
string read_names()
{
    string names, all_names;
    do
    {
        getline(cin, names);
        all_names += names + " ";
    }
    while (!names.empty());

    //validating input
    bool ok = true;
    for (auto c: all_names)
        if (c == '_')
            ok = false;
    if (!ok)
        cout << "Warning: bad underscore character '_' in names" << endl;

    return all_names;
}

//Split string on tokens by spaces.
vector<string> tokenize(string names)
{
    istringstream iss(names);
    vector<string> animals;
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(animals));
    return animals;
}

//display list of animals
void showAnimals(vector<string> animals)
{
    int id = 1;
    for (auto name: animals)
    {
        cout << id++ << ": " << name << endl;
    }
}

//return list of n randomly selected animals
vector<string> selectRandomAnimals(int n, const vector<string> &animals)
{
    int animalsCount = animals.size();
    //Checking that n not exceed total number of animals
    if (n > animalsCount)
    {
        cout << "Warning: you can not select more animals than available.\n" << endl;
        return animals;
    }

    vector<string> ret(n); // for storing selected animals
    vector<bool> used(animalsCount, false);
    int selected = 0;
    while (selected < n)
    {
        int id = rand()%animalsCount; // generate random number
        if (!used[id])
        {
            used[id] = true; // mark as selected
            ret[selected++] = animals[id]; // add to return vector
        }
    }
    return ret;
}

string makeQuery(int level, const vector<string> names)
{

    //Combine vector of name to one string.
    string query = accumulate(names.begin(), names.end(), string(""));

    //Random shuffle chars in string
    random_shuffle(query.begin(), query.end());

    int length = query.length();
    int hidden = min(level, length);
    for (int i = 0; i < hidden; )
    {
        int pos = rand()%length;
        if (query[pos] != '_')
        {
            query[pos] = '_';
            ++i;
        }
    }
    return query;
}

//compare two vectors as sorted set of strings. Return true if a == b
bool equals(vector<string> &a, vector<string> &b)
{
    int s1 = a.size(), s2 = b.size();
    if (s1 != s2)
    {
        return false;
    }
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    for (int i = 0; i < s1; ++i)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }
    return true;
}

//function for game playing
void play(const vector<string> animals)
{
    int level = 0;
    int score = 0; //+1 for right answer, -1 for wrong. if |score| == 2, change level

    do
    {
        int pickNames = rand()%3+1; // number of picking names
        vector<string> selectedNames = selectRandomAnimals(pickNames, animals); // select random animals
        string query = makeQuery(level, selectedNames); // create query string
        do
        {
            cout << "What are " << pickNames << " animals in \"" << query << "\"? ";
            string command;
            getline(cin, command); //read user answer
            //process help
            if (command == "?")
            {
                showAnimals(animals);
            }
            //process quit
            else if (command == "quit")
            {
                cout << "\nbye..." << endl;
                return;
            }
            else
            {
                vector<string> answer = tokenize(command);
                //check user answer
                if (equals(answer, selectedNames))
                {
                    cout << "Yes!" << endl;
                    score = max(1, score+1);
                    //increase level
                    if (score == 2)
                    {
                        cout << "Succeeded two consecutive times, challenge goes up!" << endl;
                        score = 0;
                        ++level;
                    }
                }
                else
                {
                    cout << "Nope!" << endl;
                    score = min(-1, score-1);
                    //decrease level
                    if (score == -2)
                    {
                        cout << "Missed two consecutive times, challenge goes down!" << endl;
                        score = 0;
                        level = max(0, level-1);
                    }
                }
                break;
            }
        }
        while (true);
    }
    while (true);
}

int main()
{
    //init random
    srand(time(NULL));

    //provide instructions
    welcome();

    //read user input
    string names = read_names();

    //tokenize each name from input
    vector<string> animals = tokenize(names);

    //print tokenized animals
    showAnimals(animals);

    //run player
    play(animals);

    return 0;
}
