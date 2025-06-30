#include "User.h"
#include "RecommendationSystem.h"


// implement your cpp code here
User::User(const std::string &user_name, const rank_map &ranking,
           std::shared_ptr<RecommendationSystem> &rec_sys_ptr) :
           _user_name(user_name), _ranking(ranking), _rec_sys_ptr(rec_sys_ptr)
           {}

std::string User::get_name() const
{
    return this->_user_name;
}

void User::add_movie_to_rs(const std::string &name, int year,
                     const std::vector<double> &features,
                     double rate)
{
    // Creating a new movie to be added to the recommendation system
    sp_movie movie_to_add = std::make_shared<Movie>(name, year);

    // Updating the _ranking map by setting rate as the value associated
    // with the key movie_to_add
    _ranking[movie_to_add] = rate;

    // Adding the movie with the given name, year, and features
    // to the recommendation system
    _rec_sys_ptr->add_movie(name, year, features);
}

sp_movie User::get_recommendation_by_content() const
{
    return this->_rec_sys_ptr->recommend_by_content(*this);
}

sp_movie User::get_recommendation_by_cf(int k) const
{
    return this->_rec_sys_ptr->recommend_by_cf(*this, k);
}

double User::get_prediction_score_for_movie(const std::string& name,
                                      int year, int k) const
{
    sp_movie new_movie = std::make_shared<Movie>(name, year);

    return this->_rec_sys_ptr->predict_movie_score(*this, new_movie, k);
}

const rank_map& User::get_ranks() const
{
    return this->_ranking;
}

std::ostream& operator<< (std::ostream& os, const User& user)
{
    os << "name: " << user._user_name << "\n"
    << *(user._rec_sys_ptr) << std::endl;
    return os;
}





