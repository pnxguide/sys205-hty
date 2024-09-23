#include <iostream>
#include <string>

void convert_from_csv_to_hty(std::string csv_file_path,
                             std::string hty_file_path) {
    // TODO: Implement this
}

int main() {
    std::string csv_file_path, hty_file_path;

    // Get arguments
    std::cout << "Please enter the .csv file path:" << std::endl;
    std::cin >> csv_file_path;
    std::cout << "Please enter the .hty file path:" << std::endl;
    std::cin >> hty_file_path;

    // Convert
    convert_from_csv_to_hty(csv_file_path, hty_file_path);

    return 0;
}