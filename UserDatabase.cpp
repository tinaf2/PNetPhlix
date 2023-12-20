#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <vector>

#include "User.h"

#include <iostream>
#include <fstream>
using namespace std;

UserDatabase::UserDatabase()
{
    m_hasLoaded = false;
}

UserDatabase::~UserDatabase()
{
    for (int i = 0; i < m_users.size(); i++)
    {
        delete m_users[i];
        m_users[i] = nullptr;
    }
}

bool UserDatabase::load(const string& filename)
{
    if (m_hasLoaded)
        return false;
    
    ifstream infile(filename);
    if (!infile)
    {
        cerr << "FAILED TO LOAD: " << filename << endl;
        return false;
    }
    
    int count = 0;
    while (infile)
    {
        string name;
        getline(infile, name);
        
        // A failed attempt to read name means that there is no entry left
        if (!infile)
            break;
        
        string email;
        getline(infile, email);
        int numMovies;
        infile >> numMovies;
        infile.ignore(10000, '\n');
        vector<string> watchHistory;
        for (int i = 0; i < numMovies; i++)
        {
            string movie;
            getline(infile, movie);
            watchHistory.push_back(movie);
        }
        
        User* user = new User(name, email, watchHistory);
        m_users.push_back(user);
        m_tree.insert(email, user);
        // cerr << "Inserted: " << email << " with User: " << name << endl;
        
        count++;
        infile.ignore(10000, '\n'); // Skip line
    }
    
    cerr << count << " user entries successfully loaded!" << endl;
    
    m_hasLoaded = true;
    return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<string, User*>::Iterator user = m_tree.find(email);
    if (!user.is_valid())
        return nullptr;
    return user.get_value();
}
