#include "RecommendationSystem.h"

double RecommendationSystem::calculate_average (const User &user)
{
    const rank_map& user_ranks = user.get_ranks();

    double sum = 0;
    int count = 0;
    for (const auto& pair : user_ranks)
    {
        if (pair.second != 0)
        {
            sum += pair.second;
            count++;
        }
    }
    double avg = sum / count;
    return avg;
}

rank_map RecommendationSystem::adjust_ratings (const User &user)
{
    double avg = calculate_average(user);

    auto copy_adjusted_ranks = user.get_ranks();

    for (auto& pair : copy_adjusted_ranks)
    {
        if (pair.second != 0)
        {
            pair.second -= avg;
        }
    }

    return copy_adjusted_ranks;
}


feat_vec RecommendationSystem::initialize_feat_vec () const
{
    feat_vec init_feat_vec;
    size_t size = _movie_data.begin()->second.size();

    for (size_t i = 0; i < size; i++)
    {
        init_feat_vec.push_back(0.0);
    }

    return init_feat_vec;
}


void RecommendationSystem::calc_pref_vec(feat_vec& pref_vec,
             const rank_map& ranked_movies, feat_map& unwatched_movies) const
{
    for (const auto& movie : this->_movie_data)
    {
        // Check if the movie has been rated (found in ranked_movies)
        auto it = ranked_movies.find(movie.first);

        // Movie was found
        if (it != ranked_movies.end())
        {
            // Movie has been rated, update the preference vector
            for (size_t j = 0; j < pref_vec.size(); j++)
            {
                pref_vec[j] += movie.second[j] * it->second;
            }

            // If the movie's rating is 0, it means it's not watched
            if (it->second == 0)
            {
                unwatched_movies[movie.first] = movie.second;
            }
        }

        // Movie not found in tmp, so it's considered not watched
        else
        {
            unwatched_movies[movie.first] = movie.second;
        }
    }
}

double RecommendationSystem::dot_product(const feat_vec& a,
                                     const feat_vec& b) const noexcept(false)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("Vectors must be of the same length.");
    }

    double sum = 0.0;
    for (size_t i = 0; i < a.size(); ++i)
    {
        sum += a[i] * b[i];
    }

    return sum;
}

double RecommendationSystem::vector_norm (const feat_vec& features) const
{
    return std::sqrt(dot_product(features, features));
}


sp_movie RecommendationSystem::calc_similarity (feat_vec& pref_vec,
                                    feat_map& unwatched_movies) const
{
    double pref_vec_norm = vector_norm(pref_vec);
    feat_vec similarity_vec;

    for (const auto& movie : unwatched_movies)
    {

        double curr_dot_prod_calc = dot_product(pref_vec, movie.second);
        double curr_feat_vec_norm = vector_norm(movie.second);
        similarity_vec.push_back(curr_dot_prod_calc /
                                 (pref_vec_norm * curr_feat_vec_norm));
    }

    // Find the iterator to the maximum element in the similarity_vec
    auto max_it = std::max_element(similarity_vec.begin(),
                                   similarity_vec.end());

    // Calculate the index of the maximum similarity score
    std::size_t max_index = std::distance(similarity_vec.begin(), max_it);

    // Iterate through unwatched_movies to find the movie
    // corresponding to max_index
    auto movie_it = unwatched_movies.begin();
    std::advance(movie_it, max_index);

    // Return the sp_movie with the highest similarity score
    return movie_it->first;
}



sp_movie RecommendationSystem::recommend_by_content(const User &user)
{
    // Step 1
    auto adjusted_ranks = adjust_ratings(user);

    // Step 2
    feat_vec pref_vec = initialize_feat_vec();
    calc_pref_vec(pref_vec, adjusted_ranks, this->_movie_data);

    // Step 3
    feat_map unwatched_movies;

    const rank_map& user_ranks = user.get_ranks();

    for (const auto& movie_pair : this->_movie_data)
    {
        // If the movie is not found in the user's rankings or has a 0 rating,
        // it is considered unwatched
        if (user_ranks.find(movie_pair.first) == user_ranks.end()
        || user_ranks.at(movie_pair.first) == 0)
        {
            unwatched_movies.insert(movie_pair);
        }
    }

    sp_movie recommended_movie = calc_similarity(pref_vec, unwatched_movies);

    return recommended_movie;
}

bool movie_double_comparator(const movie_double_pair& a,
                             const movie_double_pair& b)
{
    return a.second > b.second;
}

double RecommendationSystem::predict_movie_score(const User &user,
                                                 const sp_movie &movie, int k)
{
    rec_vec similarity_vector;

    auto unwatched_movie_it = this->_movie_data.find(movie);

    feat_vec unwatched_movie_feat_vec = unwatched_movie_it->second;

    double unwatched_movie_norm = vector_norm(unwatched_movie_feat_vec);

    for (const auto& watched_movie : user.get_ranks())
    {
        if (watched_movie.second != 0)
        {
            feat_vec watched_movie_feat_vec =
                    this->_movie_data.find(watched_movie.first)->second;

            double watched_movie_norm = vector_norm(watched_movie_feat_vec);

            double dot_prod = dot_product(unwatched_movie_feat_vec,
                                          watched_movie_feat_vec);

            double similarity = dot_prod /
                                (unwatched_movie_norm * watched_movie_norm);

            similarity_vector.push_back
                    (std::make_pair(watched_movie.first, similarity));
        }
    }

    std::sort(similarity_vector.begin(),
              similarity_vector.end(), movie_double_comparator);
    double numerator = 0.0;
    double denominator = 0.0;
    for (int i = 0; i < k; i++)
    {
        numerator += similarity_vector[i].second *
            user.get_ranks().find(similarity_vector[i].first)->second;
        denominator += similarity_vector[i].second;
    }
    return numerator / denominator;
}

sp_movie RecommendationSystem::recommend_by_cf(const User& user, int k)
{
    // Creating an empty rec_vec
    rec_vec recommendation_vector;

    for (const auto& movie : this->_movie_data)
    {
        // Detecting the unranked movies and creating a score for them
        if (user.get_ranks().find(movie.first) == user.get_ranks().end() ||
            user.get_ranks().find(movie.first)->second == 0)
        {
            double predict_score = predict_movie_score(user, movie.first, k);
            recommendation_vector.push_back(
                    std::make_pair(movie.first, predict_score));
        }
    }

    std::sort(recommendation_vector.begin(),
              recommendation_vector.end(), movie_double_comparator);

    return recommendation_vector.begin()->first;
}

sp_movie RecommendationSystem::add_movie(const std::string& name,int year,
         const std::vector<double>& features)
{

    // Create the new movie to be added
    sp_movie movie_to_add = std::make_shared<Movie>(name, year);

    // Add it to the movies dataset
    (this->_movie_data)[movie_to_add] = features;

    return movie_to_add;
}

sp_movie RecommendationSystem::get_movie(const std::string &name,
                                         int year) const
{
    for (const auto& movie : this->_movie_data)
    {
        if (movie.first->get_name() == name &&
            movie.first->get_year() == year)
        {
            return movie.first;
        }
    }
    return nullptr;
}

std::ostream& operator<< (std::ostream& os,
                          const RecommendationSystem& rs)
{
    for (const auto& movie : rs._movie_data)
    {
        os << *(movie.first); // Utilizes the operator<< in Movie class
    }
    return os;
}