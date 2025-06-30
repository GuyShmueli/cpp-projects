
#include "RecommendationSystemLoader.h"
#include <fstream>
#include <sstream>
#define YEAR_SEPARATOR '-'
#define ERROR_MSG "input file is incorrect"
#include <memory>

//// Start
//using features = std::vector<double>;
//constexpr char NO_VAL[] = "NA";
//constexpr double MIN_RANK = 0.0;
//constexpr double MAX_RANK = 10.0;
//constexpr char FILE_ERR[] = "File error";
//constexpr char RANGE_ERR[] = "Range error";
//
//int read_features(std::istringstream *stream, features* features_vec) {
//    std::string temp;
//    while ((*stream) >> temp) {
//        if (temp == NO_VAL) {
//            features_vec->push_back(0.0);
//        } else {
//            double feature = std::stod(temp);
//            if (feature < MIN_RANK || feature > MAX_RANK) {
//                return EXIT_FAILURE;
//            }
//            features_vec->push_back(feature);
//        }
//    }
//    return EXIT_SUCCESS;
//}
//
//// ptr_type is assumed to be std::unique_ptr<RecommenderSystem>
//std::unique_ptr<RecommendationSystem> RecommendationSystemLoader::
//create_rs_from_movies(const std::string &movies_file_path) noexcept(false)
//{
//    auto sys = std::make_unique<RecommendationSystem>();
//    std::ifstream in_file(movies_file_path);
//    if (in_file.fail()) {
//        in_file.close();
//        throw std::invalid_argument(FILE_ERR);
//    }
//
//    std::string line;
//    while (getline(in_file, line)) {
//        std::istringstream stream(line);
//        std::string name_and_year;
//        stream >> name_and_year;
//        std::istringstream ny_stream(name_and_year);
//
//        std::string movie_name;
//        int movie_year;
//        getline(ny_stream, movie_name, '-');
//        ny_stream >> movie_year;
//
//        features features_vec;
//        int flag = read_features(&stream, &features_vec);
//        if (flag == EXIT_FAILURE) {
//            in_file.close();
//            throw std::out_of_range(RANGE_ERR);
//        }
//        sys->add_movie(movie_name, movie_year, features_vec);
//    }
//    in_file.close();
//    return sys;
//}
//// STOP



uniq_ptr_rec_sys RecommendationSystemLoader::create_rs_from_movies(const
std::string &movies_file_path) noexcept(false)
{
    uniq_ptr_rec_sys rs = std::make_unique<RecommendationSystem>();
    std::ifstream in_file;
    in_file.open(movies_file_path);
    std::string buffer;
    while (getline(in_file, buffer))
    {
        std::string movie_det;
        double ranking;
        std::istringstream splitted_line(buffer);
        splitted_line >> movie_det;
        std::vector<double> vec;
        while (splitted_line >> ranking)
        {
            if (ranking <= 0){
                throw std::invalid_argument(ERROR_MSG);
            }
            vec.push_back(ranking);
        }
        size_t end = buffer.find(YEAR_SEPARATOR);
        rs->add_movie(buffer.substr(0, end),
                  std::stoi(buffer.substr(end + 1, buffer.length())), vec);
    }
    in_file.close();
    return rs;
}

