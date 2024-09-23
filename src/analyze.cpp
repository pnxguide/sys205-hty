#include <iostream>
#include <string>
#include <vector>

#include "../third_party/nlohmann/json.hpp"

nlohmann::json extract_metadata(std::string hty_file_path);
std::vector<int> project_single_column(nlohmann::json metadata,
                                       std::string hty_file_path,
                                       std::string projected_column);
void display_column(nlohmann::json metadata, std::string column_name,
                    std::vector<int> data);
std::vector<int> filter(nlohmann::json metadata, std::string hty_file_path,
                        std::string projected_column, int operation,
                        int filtered_value);
std::vector<std::vector<int>> project(
    nlohmann::json metadata, std::string hty_file_path,
    std::vector<std::string> projected_columns);
void display_result_set(nlohmann::json metadata,
                        std::vector<std::string> column_name,
                        std::vector<std::vector<int>> result_set);
std::vector<std::vector<int>> project_and_filter(
    nlohmann::json metadata, std::string hty_file_path,
    std::vector<std::string> projected_columns, std::string filtered_column,
    int op, int value);
void add_row(nlohmann::json metadata, std::string hty_file_path,
             std::string modified_hty_file_path,
             std::vector<std::vector<int>> rows);

int main() { return 0; }
