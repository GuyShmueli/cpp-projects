//
// Created on 2/20/2022.
//

#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H
#include "User.h"
#include <cmath>
#include <map>
#include <memory>
#include <algorithm> // Required for std::max_element

typedef std::vector<double> feat_vec;

struct feat_map_compare
{
    bool operator()(const sp_movie &a, const sp_movie &b) const
    {
        return *a < *b; // Utilizes Movie's operator< for comparison
    }
};

typedef std::map<sp_movie, feat_vec, feat_map_compare> feat_map;

typedef std::pair<sp_movie, double> movie_double_pair;
typedef std::vector<movie_double_pair> rec_vec;

class RecommendationSystem
{
private:
    feat_map _movie_data;
    double calculate_average (const User& user);
    rank_map adjust_ratings (const User& user);
    double vector_norm (const feat_vec& features) const;
    feat_vec initialize_feat_vec () const;
    void calc_pref_vec(feat_vec& pref_vec,
             const rank_map& ranked_movies, feat_map& unwatched_movies) const;
    double dot_product(const feat_vec& a,
                         const feat_vec& b)const noexcept(false);
    sp_movie calc_similarity (feat_vec& pref_vec,
                              feat_map& unwatched_movies) const;


public:
    // Default empty constructor
    explicit RecommendationSystem() = default;

    // No copy constructor
    //RecommendationSystem(const RecommendationSystem& other) = delete;

    // No assignment operator
//    RecommendationSystem& operator= (const RecommendationSystem& rhs)
//    = delete;

    //explicit RecommendationSystem()
    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie(const std::string& name,int year,const
    std::vector<double>& features);


    /**
     * a function that calculates the movie with highest
     * score based on movie features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const User& user);

    /**
     * a function that calculates the movie with highest
     * predicted score based on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const User& user, int k);


    /**
     * Predict a user rating for a movie given argument using item
     * cf procedure with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(const User &user, const sp_movie &movie,
												  int k);

	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string &name, int year) const;


	// TODO operator<<
    friend std::ostream& operator<< (std::ostream& os,
            const RecommendationSystem& rs);


};


#endif //RECOMMENDATIONSYSTEM_H
