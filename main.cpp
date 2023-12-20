#include "UserDatabase.h"
#include "User.h"
#include <iostream>
#include <string>
using namespace std;

//////////////////////////i/////////////////////////////////////////////////////
//
// You are free to do whatever you want with this file, since you won't
// be turning it in.  Presumably, you will make changes to help you test
// your classes.  For example, you might try to implement only some User
// member functions to start out with, and so replace our main routine with
// one that simply creates a User and verifies that the member functions you
// implemented work correctly.
//
//////////////////////////i/////////////////////////////////////////////////////


  // If your program is having trouble finding these files. replace the
  // string literals with full path names to the files.  Also, for test
  // purposes, you may want to create some small, simple user and movie
  // data files to makde debuggiing easier, so you can replace the string
  // literals with the names of those smaller files.

const string USER_DATAFILE  = "/Users/rolandyang/Documents/GitHub/CS32/Project_4/pnetphlix/pnetphlix/users.txt";
const string MOVIE_DATAFILE = "/Users/rolandyang/Documents/GitHub/CS32/Project_4/pnetphlix/pnetphlix/movies.txt";

#include "MovieDatabase.h"
#include "Movie.h"
#include <vector>
#include "Recommender.h"
#include <chrono>

void findMatches(const Recommender& r, const MovieDatabase& md, const string& user_email, int num_recommendations)
{
    auto start = chrono::steady_clock::now();
    vector<MovieAndRank> recommendations = r.recommend_movies(user_email, num_recommendations);
    auto stop = chrono::steady_clock::now();
    if (recommendations.empty())
        cout << "We found no movies to recommend :(.\n";
    else {
        for (int i = 0; i < recommendations.size(); i++) {
            const MovieAndRank& mr = recommendations[i];
            Movie* m = md.get_movie_from_id(mr.movie_id);
            cout << (i+1) << ". " << m->get_title() << " (" << m->get_release_year() << ")\n Rating: " << m->get_rating() << "\n Compatibility Score: " << mr.compatibility_score << "\n\n";
        }
    }
    cout << "Took " << (chrono::duration_cast<chrono::milliseconds>(stop - start).count()) << "ms to GENERATE " << num_recommendations << " RECCOMENDATIONS FOR " << user_email << endl;
}

int main()
{
    auto userStart = chrono::steady_clock::now();
    UserDatabase udb;
    if (udb.load(USER_DATAFILE) == false)  // In skeleton, load always return false
    {
        cout << "Failed to load user data file " << USER_DATAFILE << "!" << endl;
        return 1;
    }
    auto userStop = chrono::steady_clock::now();

    auto movieStart = chrono::steady_clock::now();
    MovieDatabase mdb;
    if (mdb.load(MOVIE_DATAFILE) == false)  // In skeleton, load always return false
    {
        cout << "Failed to load movie data file " << MOVIE_DATAFILE << "!" << endl;
        return 1;
    }
    auto movieStop = chrono::steady_clock::now();
    Recommender r(udb, mdb);
    
    cout << "Took " << (chrono::duration_cast<chrono::milliseconds>(userStop - userStart).count()) << "ms to LOAD USER DATABASE" << endl;
    cout << "Took " << (chrono::duration_cast<chrono::milliseconds>(movieStop - movieStart).count()) << "ms to LOAD MOVIE DATABASE" << endl;
    
    cout << endl;
    
    cout << "Welcome to Roland's SICKOMODE TESTER" << endl;
    for (;;)
    {
        cout << "\nEnter choice from menu: " << "\n" <<
        "1 - Search for user by email address" << endl <<
        "2 - Search for movie by id" << endl <<
        "3 - Search for movies by director" << endl <<
        "4 - Search for movies by actor" << endl <<
        "5 - Search for movies by genre" << endl <<
        "6 - Get recommendations" << endl <<
        "7 - Exit" << endl;
        
        int choice;
        cin >> choice;
        cin.ignore(10000, '\n');
        
        // The world's most efficient code ever
        if (choice == 1)
        {
            cout << "\nEnter an email to search for: ";
            string email;
            getline(cin, email);
            auto start = chrono::steady_clock::now();
            User* u = udb.get_user_from_email(email);
            auto stop = chrono::steady_clock::now();
            if (u == nullptr)
                cout << "No user in the database has that email address." << endl << endl;
            else
            {
                cout << "Full Name: " << u->get_full_name() << endl;
                vector<string> watch_history = u->get_watch_history();
                cout << "Watch History: ";
                for (int i = 0; i < watch_history.size(); i++)
                    cout << watch_history[i] << ", ";
                cout << endl << endl;
            }
            cout << "Took " << (chrono::duration_cast<chrono::milliseconds>(stop - start).count()) << "ms to SEARCH FOR " << email << endl;
        }
        else if (choice == 2)
        {
            cout << "\nEnter an id to search for: ";
            string id;
            getline(cin, id);
            auto start = chrono::steady_clock::now();
            Movie* m = mdb.get_movie_from_id(id);
            auto stop = chrono::steady_clock::now();
            if (m == nullptr)
                cout << "No movie in the database has that id." << endl << endl;
            else
            {
                cout << "Title: " << m->get_title() << endl;
                cout << "Release Year: " << m->get_release_year() << endl;
                cout << "Rating: " << m->get_rating() << endl;
                
                vector<string> directors = m->get_directors();
                vector<string> actors = m->get_actors();
                vector<string> genres = m->get_genres();
                
                cout << "Directors: ";
                for (int i = 0; i < directors.size(); i++)
                    cout << directors[i] << ", ";
                cout << endl;
                
                cout << "Actors: ";
                for (int i = 0; i < actors.size(); i++)
                    cout << actors[i] << ", ";
                cout << endl;
                
                cout << "Genres: ";
                for (int i = 0; i < genres.size(); i++)
                    cout << genres[i] << ", ";
                
                cout << endl << endl;
            }
            cout << "Took " << (chrono::duration_cast<chrono::milliseconds>(stop - start).count()) << "ms to SEARCH FOR " << id << endl;
        }
        else if (choice == 3)
        {
            cout << "\nEnter an director to search for: ";
            string director;
            getline(cin, director);
            auto start = chrono::steady_clock::now();
            vector<Movie*> m = mdb.get_movies_with_director(director);
            auto stop = chrono::steady_clock::now();
            if (m.size() == 0)
                cout << "No directors in the database with that name." << endl << endl;
            else
            {
                cout << "Movies: ";
                for (int i = 0; i < m.size(); i++)
                    cout << m[i]->get_title() << "(" << m[i]->get_id() << "),";
                cout << endl << endl;
            }
            cout << "Took " << (chrono::duration_cast<chrono::milliseconds>(stop - start).count()) << "ms to SEARCH FOR " << director << endl;
        }
        else if (choice == 4)
        {
            cout << "\nEnter an actor to search for: ";
            string actor;
            getline(cin, actor);
            auto start = chrono::steady_clock::now();
            vector<Movie*> m = mdb.get_movies_with_actor(actor);
            auto stop = chrono::steady_clock::now();
            if (m.size() == 0)
                cout << "No actors in the database with that name." << endl << endl;
            else
            {
                cout << "Movies: ";
                for (int i = 0; i < m.size(); i++)
                    cout << m[i]->get_title() << "(" << m[i]->get_id() << "),";
                cout << endl << endl;
            }
            cout << "Took " << (chrono::duration_cast<chrono::milliseconds>(stop - start).count()) << "ms to SEARCH FOR " << actor << endl;
        }
        else if (choice == 5)
        {
            cout << "\nEnter a genre to search for: ";
            string genre;
            getline(cin, genre);
            auto start = chrono::steady_clock::now();
            vector<Movie*> m = mdb.get_movies_with_genre(genre);
            auto stop = chrono::steady_clock::now();
            if (m.size() == 0)
                cout << "No genres in the database with that title." << endl << endl;
            else
            {
                cout << "Movies: ";
                for (int i = 0; i < m.size(); i++)
                    cout << m[i]->get_title() << "(" << m[i]->get_id() << "),";
                cout << endl << endl;
            }
            cout << "Took " << (chrono::duration_cast<chrono::milliseconds>(stop - start).count()) << "ms to SEARCH FOR " << genre << endl;
        }
        else if (choice == 6)
        {
            cout << "\nEnter an email to generate recommendations for: ";
            string email;
            getline(cin, email);
            
            cout << "Enter # of recommendations you want to generate: ";
            int numRecs;
            cin >> numRecs;
            cin.ignore(10000, '\n');
            
            findMatches(r, mdb, email, numRecs);
            
            cout << endl;
        }
        else if (choice == 7)
        {
            cout << "peace out uwu" << endl;
            break;
        }
        else
            cout << "Invalid choice!!!" << endl;
    }
}
