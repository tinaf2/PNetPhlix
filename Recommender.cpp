#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"

#include <string>
#include <vector>
using namespace std;

#include <unordered_set>
#include <unordered_map>
#include <set>

#include "User.h"
#include "UserDatabase.h"
#include "Movie.h"
#include "MovieDatabase.h"

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
{
    udb = &user_database;
    mdb = &movie_database;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    User* user = udb->get_user_from_email(user_email); // O(logU): Find user from email
    
    // Return empty vector if movie_count is negative or zero or user is invalid
    if (movie_count <= 0 || user == nullptr)
        return vector<MovieAndRank>();
    
    unordered_set<Movie*> visited;
    unordered_map<Movie*, int> map;
    
    std::vector<string> watch_history = user->get_watch_history(); // O(1)
    
    // Get movies that user has watched
    vector<Movie*> watched_movies;
    for (int i = 0; i < watch_history.size(); i++) // O(historySize * logM)
    {
        Movie* movie = mdb->get_movie_from_id(watch_history[i]); // O(logM)
        watched_movies.push_back(movie);
        visited.insert(movie);
    }
    
    for (int i = 0; i < watched_movies.size(); i++)
    {
        vector<string> directors = watched_movies[i]->get_directors(); // O(1)
        vector<string> actors = watched_movies[i]->get_actors(); // O(1)
        vector<string> genres = watched_movies[i]->get_genres(); // O(1)
        
        for (int j = 0; j < directors.size(); j++)
        {
            vector<Movie*> directorMovies = mdb->get_movies_with_director(directors[j]);
            addPoints(20, directorMovies, visited, map);
        }
        
        for (int j = 0; j < actors.size(); j++)
        {
            vector<Movie*> actorMovies = mdb->get_movies_with_actor(actors[j]);
            addPoints(30, actorMovies, visited, map);
        }
        
        for (int j = 0; j < genres.size(); j++)
        {
            vector<Movie*> genreMovies = mdb->get_movies_with_genre(genres[j]);
            addPoints(1, genreMovies, visited, map);
        }
    }
    
    // Sort the results
    set<pair<Movie*, int>, comp> sortedMovies(map.begin(), map.end());
    
    vector<MovieAndRank> res;
    
    int count = 0;
    for (auto& pair : sortedMovies)
    {
        if (count >= movie_count) // Take care of extraneous results
            break;
        res.push_back(MovieAndRank(pair.first->get_id(), pair.second));
        count++;
    }
    
    return res;
}

void Recommender::addPoints(int numPoints, std::vector<Movie*>& movies, std::unordered_set<Movie*>& visited, std::unordered_map<Movie*, int>& map) const
{
    for (int z = 0; z < movies.size(); z++)
    {
        if (visited.find(movies[z]) == visited.end())
        {
            visited.insert(movies[z]);
            map[movies[z]] = numPoints;
        }
        else if (map.find(movies[z]) != map.end()) // TODO: Check Big(O)
            map[movies[z]] = map[movies[z]] + numPoints;
    }
}

template<typename T>
bool Recommender::comp::operator()(const T& a, const T& b) const
{
    // Sort by compatibility score
    if (b.second > a.second)
        return false;
    else if (b.second < a.second)
        return true;
    
    // If compatibility score the same, sort by rating
    if (b.first->get_rating() > a.first->get_rating())
        return false;
    else if (b.first->get_rating() < a.first->get_rating())
        return true;
    
    // If rating the same, sort alphabetically
    return b.first->get_title().compare(a.first->get_title()) >= 0; // TODO: CHECK
}
