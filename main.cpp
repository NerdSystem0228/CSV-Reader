#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include "include/TextTable.h"

using namespace std;
namespace fs = std::filesystem;

void showCSV(ifstream& file);
vector<vector<string>> getCSVData(ifstream& file);

int main(int argc, char* argv[]) {
    // Define the path to the csv file
    fs::path filePath;


    // Check if the user has provided the correct number of arguments
    switch(argc) {
        case 1:
            std::cout << "csvReader: missing csv file\nUsage: \'csvReader path/to/csvfile\'" << endl;
            break;
        case 2:
        {
            filePath = (string) argv[1];
            
            // Check if the file exists
            if (!fs::exists(filePath)) {std::cout << "csvReader: file does not exist." << endl; return 1;}

            // Check if the file is a directory    
            if (fs::is_directory(filePath)) { std::cout << "csvReader: the path provided is a directory." << endl; return 1; }                      

            // Check if the file is a csv file            
            if (filePath.extension() != ".csv") { std::cout << "csvReader: the file provided is not a csv file." << endl; return 1; }

            ifstream file(filePath);

            if (!file.is_open()) { std::cout << "csvReader: could not open the file." << endl; return 1; }
            
            showCSV(file);
            break;
        }
        default:
            std::cout << "csvReader: too much options\nTry \'csvReader --help\' for more information." << endl;
            break;
    }
    return 0;
}

vector<vector<string>> getCSVData(ifstream& file) {
    string line;
    string fieldData;
    vector<string> fieldRowData;
    vector<vector<string>> fieldsData;
    int rows = 0;
    while (getline(file, line)) {
        //Get data from the row
        for (int i = 0; i < line.size(); i++) {
            //Verifies if there's a comma in the end of the line, if it does, add a empty field to complete the columns
            if (i == line.size()-1) {
                if (line[i] == ',') {
                    fieldRowData.push_back(string{""});
                }
            }

            //Get each char of the string from the field
            if (line[i] != ',') {
                fieldData.push_back(line[i]);
                continue;
            }

            //Push empty field data when no char was collected from the field
            if (fieldData.empty()) { 
                fieldRowData.push_back(string{""});
                continue;
            }

            // Push the field data to the vector that stores all data from the fields in a row
            fieldRowData.push_back(fieldData);
            fieldData.clear();
        }

        // Push unhandled field data to the field row data before pushing the field row data to the fieldsData
        if (!fieldData.empty()) {
            fieldRowData.push_back(fieldData);
            fieldData.clear();
        }

        fieldsData.push_back(fieldRowData);        
        fieldRowData.clear();
        rows += 1;
    }  

    return fieldsData;
}

void showCSV(ifstream& file) {
    
    vector<vector<string>> fieldsData = getCSVData(file);
    TextTable table( '-', '|', '+' );
    
    for (int i = 0; i < fieldsData.size(); i++) {
        for (int j = 0; j < fieldsData[i].size(); j++) {
             table.add(fieldsData[i][j]);
        }
        table.endOfRow();
    }
    table.setAlignment( fieldsData[0].size(), TextTable::Alignment::RIGHT );
    std::cout << table;
}