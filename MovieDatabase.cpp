#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>

#include <fstream>
#include <sstream>

using namespace std;

MovieDatabase::MovieDatabase()
{
    // Replace this line with correct code.
    m_hasLoaded = false;
}

bool MovieDatabase::load(const string& filename)
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
        string id;
        getline(infile, id);
        
        // A failed attempt to read name means that there is no entry left
        if (!infile)
            break;
        
        string title;
        getline(infile, title);
        
        string year;
        getline(infile, year);
        
        string directors;
        getline(infile, directors);
        stringstream sstr(directors);
        vector<string> v_directors;
        while (sstr.good())
        {
            string substr;
            getline(sstr, substr, ',');
            v_directors.push_back(substr);
        }
        
        string actors;
        getline(infile, actors);
        stringstream sstr2(actors);
        vector<string> v_actors;
        while (sstr2.good())
        {
            string substr;
            getline(sstr2, substr, ',');
            v_actors.push_back(substr);
        }
        
        string genres;
        getline(infile, genres);
        stringstream sstr3(genres);
        vector<string> v_genres;
        while (sstr3.good())
        {
            string substr;
            getline(sstr3, substr, ',');
            v_genres.push_back(substr);
        }
        
        float rating;
        infile >> rating;
        infile.ignore(10000, '\n');
        
        Movie* movie = new Movie(id, title, year, v_directors, v_actors, v_genres, rating);
        m_movies.push_back(movie);
        m_id_movie_tree.insert(getLowercase(id), movie);
        for (int i = 0; i < v_directors.size(); i++)
            m_director_movie_tree.insert(getLowercase(v_directors[i]), movie);
        for (int i = 0; i < v_actors.size(); i++)
            m_actor_movie_tree.insert(getLowercase(v_actors[i]), movie);
        for (int i = 0; i < v_genres.size(); i++)
            m_genre_movie_tree.insert(getLowercase(v_genres[i]), movie);
        
        // cerr << "Inserted: " << id << " | " << title << endl;
        
        count++;
        infile.ignore(10000, '\n'); // Skip line
    }
    
    cerr << count << " movie entries successfully loaded!" << endl;
    
    m_hasLoaded = true;
    return true;
}
    
Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    TreeMultimap<string, Movie*>::Iterator movie = m_id_movie_tree.find(getLowercase(id));
    if (!movie.is_valid())
        return nullptr;
    return movie.get_value();
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    return getMovies(1, director);
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    return getMovies(2, actor);
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    return getMovies(3, genre);
}

MovieDatabase::~MovieDatabase()
{
    for (int i = 0; i < m_movies.size(); i++)
    {
        delete m_movies[i];
        m_movies[i] = nullptr; // Just for safe measure
    }
}

string MovieDatabase::getLowercase(const std::string word) const
{
    string res = "";
    for (int i = 0; i < word.size(); i++)
        res += tolower(word.at(i));
    return res;
}

vector<Movie*> MovieDatabase::getMovies(int search, string query) const
{
    TreeMultimap<string, Movie*>::Iterator movieIterator;
    switch(search)
    {
        case 1: // director
            movieIterator = m_director_movie_tree.find(getLowercase(query));
            break;
        case 2: // actor
            movieIterator = m_actor_movie_tree.find(getLowercase(query));
            break;
        case 3: // genre
            movieIterator = m_genre_movie_tree.find(getLowercase(query));
            break;
        default:
            cerr << "ERROR: getMovies failed! Query: " << search << endl;
            throw 1;
    }
    vector<Movie*> res;
    while (movieIterator.is_valid())
    {
        res.push_back(movieIterator.get_value());
        movieIterator.advance();
    }
    return res;
}
