#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

 int main(int argc, char const *argv[])
 {
    std::string segmlist;
    std::ifstream in("sys_3.txt");
    std::vector<std::string> raw_data;

    while(std::getline(in, segmlist, ' ')){
      raw_data.push_back(segmlist);
    }

    for(auto i: raw_data){
        std::cout << i << " ";
    }
    std::cout << std::endl;
          
    for(auto i: raw_data){
        
    }
    int _size = std::stoi(raw_data[0]);

    std::cout << _size << std::endl;
    int _iter = std::stoi(raw_data[1]);

    std::vector<std::vector<double>> _A(_size, std::vector<double>(_size, 0.00));
    std::vector<double> _B(_size, 0.00);

    int count = 2;
    
    for(int i = 0; i < _size; i++){
      for(int j = 0; j < _size; j++){
        _A[i][j] = std::stod(raw_data[count]);
        count++;
      } 
    }

    for(int i = 0; i < _size; i++){
        _B[i] = std::stod(raw_data[count]);
        std::cout << _B[i] << " ";
        count++;
    }
    std::cout << std::endl;
    auto input_data_ = std::make_tuple(_size, _A, _B, _iter);
    double epsi = 0.001;
    std::vector<double> X(_size, 0.0);

    while(_iter != 0){
        bool flag = true;

        std::vector<double> X_new(_size, 0.0);
        for(int i = 0; i < _size; i++){
            
            X_new[i] = _B[i]/_A[i][i];

            for(int j = 0; j < _size; j++){
                if(j == i) continue;
                X_new[i] -= (_A[i][j]/_A[i][i]) * X[j];
            }

            
            flag = flag && (std::fabs(X_new[i] - X[i]) < epsi);
            
            std::cout << std::fabs(X_new[i] - X[i]) << std::endl;
            X[i] = round(X_new[i]*1000)/1000;

        }

        std::cout << "Values ";
        for(auto i: X){
            std::cout <<  i << " ";
        }
        for(auto i: X_new){
            std::cout <<  i << " ";
        }
        std::cout << std::endl;
        _iter--;

        if(flag){
            return 0;
        }
    }
    for(auto i: X){
            std::cout << i << " ";
        }
    std::cout << std::endl;
    
        return 0;
 }
 