
#ifndef USER_H
#define USER_H
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include "Movie.h"

class RecommendationSystem;
typedef std::unordered_map<sp_movie, double, hash_func,equal_func> rank_map;

class User
{
private:
    std::string _user_name;
    rank_map _ranking;
    std::shared_ptr<RecommendationSystem> _rec_sys_ptr;
public:
	/**
	 * Constructor for the class
	 */
	// TODO User() this constructor can be implemented however you want

    User(const std::string& user_name, const rank_map& ranking,
     std::shared_ptr<RecommendationSystem>& rec_sys_ptr);


	/**
	 * a getter for the user's name
	 * @return the username
	 */
	// TODO get_name()
    std::string get_name() const;

	/**
	 * function for adding a movie to the DB
	 * @param name name of movie
     * @param year year it was made
	 * @param features a vector of the movie's features
	 * @param rate the user rate for this movie
	 */
	void add_movie_to_rs(const std::string &name, int year,
                         const std::vector<double> &features,
                         double rate);


    /**
     * a getter for the ranks map
     * @return
     */
    // TODO get_ranks()
    const rank_map& get_ranks() const;

	/**
	 * returns a recommendation according to the movie's content
	 * @return recommendation
	 */
	sp_movie get_recommendation_by_content() const;

	/**
	 * returns a recommendation according to the similarity recommendation method
	 * @param k the number of the most similar movies to calculate by
	 * @return recommendation
	 */
	sp_movie get_recommendation_by_cf(int k) const;

	/**
	 * predicts the score for a given movie
	 * @param name the name of the movie
	 * @param year the year the movie was created
	 * @param k the parameter which represents the number of the most similar
	 * movies to predict the score by.
	 * @return predicted score for the given movie
	 */
	double get_prediction_score_for_movie(const std::string& name,
                                          int year, int k) const;

	/**
	 * output stream operator
	 * @param os the output stream
	 * @param user the user
	 * @return output stream
	 */
	// TODO &operator<<
    friend std::ostream& operator<< (std::ostream& os, const User& user);
};



#endif //USER_H
