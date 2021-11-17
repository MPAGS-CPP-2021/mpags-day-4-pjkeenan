#include "PlayfairCipher.hpp"

#include <string>
#include <iostream>
#include <algorithm>
// #include <map>

void PlayfairCipher::changeJtoI(std::string& testKey){
    auto isJ = [] (char test) {
        if (test == 'J'){
            return 'I';
        }
        else {
            return test;
        }
    };
    std::transform(std::begin(testKey), std::end(testKey), std::begin(testKey), isJ);
    // Given solution 
    // std::transform(std::begin(key_), std::end(key_), std::begin(key_), [] (char c) { return (c == 'J') ? 'I' : c; });
}

PlayfairCipher::PlayfairCipher(const std::string& key) {
    this ->setKey(key);
}

void PlayfairCipher::setKey(const std::string& key) {
    // Store the original key
    key_ = key;

    // Append the alphabet
    key_ += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Make sure the key is upper case
    std::transform(std::begin(key_), std::end(key_), std::begin(key_), ::toupper);

    // Remove non-alpha charcters
    
    auto nonAlpha = [] (char test) {
        return !std::isalpha(test);
    };
    auto iter = std::remove_if(std::begin(key_), std::end(key_), nonAlpha);
    key_.erase(iter, std::end(key_));
    // Given solution
    // key_.erase(std::remove_if(std::begin(key_), std::end(key_), [] (char c) {return !std::isalpha(c); }), std::end(key_));

    this -> changeJtoI(key_);

    // Remove duplicated letters
    std::string foundLetters {""};
    auto checkDuplicates = [&] (char c) {
        if (foundLetters.find(c) == std::string::npos) { //std::string::npos ~ end of string -> letter not found -> don't erase
            foundLetters += c;
            return false;
        }
        else {
            return true;
        }
    };
    auto iterato = std::remove_if(std::begin(key_), std::end(key_), checkDuplicates);
    key_.erase(iterato, std::end(key_));

    // Store the coords of each letter
    // coordinate is object of type std::pair <int, int>
    for (std::size_t i{0}; i < key_.size(); ++i) {
        // Get coordinates in {row, col} format
        coordinate coord {i / width_, i % width_};
        // Store the playfair cipher key map
        let2coord_[key_[i]] = coord;
        coord2let_[coord] = key_[i];
    } 
}

std::string PlayfairCipher::applyCipher(std::string inputText, const CipherMode cipherMode) {
    // Create output string , initially copy of input text
    std::string outputText{""};
    // Change J -> I
    this -> changeJtoI(inputText);
    
    switch (cipherMode) {
        case CipherMode::Encrypt: {
            // Assume only needed to alter to appropriate digraphs when encrypting 
            // If repeated chars in a diagraph add an X or Q if XX
            // i.e. abbc -> ab bc as repeated letters in separate digraphs

            for (std::size_t i {1}; i < inputText.size(); i+=2){
                if (inputText[i] == inputText[i-1]){
                    char addChar {'X'};
                    if (inputText[i] == 'X'){
                        addChar ='Q';
                    }
                    inputText.insert( i, 1, addChar);
                }
            }
            // if the size of input is odd, add a trailing Z
            // If odd length ends with 'Z' get 'ZZ'?!?
            if (inputText.size() % 2 == 1){
                inputText += 'Z';
            }

            // Loop over the input in Digraphs
            coordinate coord1 {0,0};
            coordinate coord2 {0,0};
            coordinate newCoord1 {0,0};
            coordinate newCoord2 {0,0};

            for (std::size_t i {0}; i < inputText.size(); i+=2){
                //   - Find the coords in the grid for each digraph
                auto let2coordIter1 = let2coord_.find(inputText[i]);
                auto let2coordIter2 = let2coord_.find(inputText[i+1]);
                // Check pair found in map
                if (let2coordIter1 == let2coord_.end() || let2coordIter2 == let2coord_.end()){
                    std::cerr << "[Error] Did not find letter in letter to coordinate map\n";
                }
                coord1 = (*let2coordIter1).second;
                coord2 = (*let2coordIter2).second;
                //   - Apply the rules to these coords to get 'new' coords
                // coordinates in {row, col} format
                // 1) see if letters are on same row 
                if (coord1.first == coord2.first){
                    newCoord1.first = coord1.first;
                    newCoord2.first = coord2.first;
                    newCoord1.second = (coord1.second + 1) % width_;
                    newCoord2.second = (coord2.second + 1) % width_;
                }
                // 2) see if letters are on same column
                else if (coord1.second == coord2.second){
                    newCoord1.first = (coord1.first + 1) % width_;
                    newCoord2.first = (coord2.first + 1) % width_;
                    newCoord1.second = coord1.second;
                    newCoord2.second = coord2.second;
                }
                //Must form a rectangle
                else {
                    newCoord1.first = coord1.first;
                    newCoord2.first = coord2.first;
                    newCoord1.second = coord2.second;
                    newCoord2.second = coord1.second;
                }
                //   - Find the letter associated with the new coords
                auto coord2letIter1 = coord2let_.find(newCoord1);
                auto coord2letIter2 = coord2let_.find(newCoord2);
                // Check coordinate is found in map
                if (let2coordIter1 == let2coord_.end() || let2coordIter2 == let2coord_.end()){
                    std::cerr << "[Error] Did not find coordinate in coordinate to letter map\n";
                }
                // Add new letters to output string
                outputText += (*coord2letIter1).second;
                outputText += (*coord2letIter2).second;
            } // end loop over digraphs to encrypt
            break;
        } // end encrypt case
        case CipherMode::Decrypt: {
            // Loop over the input in Digraphs
            coordinate coord1 {0,0};
            coordinate coord2 {0,0};
            coordinate newCoord1 {0,0};
            coordinate newCoord2 {0,0};

            for (std::size_t i {0}; i < inputText.size(); i+=2){
                //   - Find the coords in the grid for each digraph
                auto let2coordIter1 = let2coord_.find(inputText[i]);
                auto let2coordIter2 = let2coord_.find(inputText[i+1]);
                // Check pair found in map
                if (let2coordIter1 == let2coord_.end() || let2coordIter2 == let2coord_.end()){
                    std::cerr << "[Error] Did not find letter in letter to coordinate map\n";
                }
                coord1 = (*let2coordIter1).second;
                coord2 = (*let2coordIter2).second;
                //   - Apply the rules to these coords to get 'new' coords
                // coordinates in {row, col} format
                // 1) see if letters are on same row 
                if (coord1.first == coord2.first){
                    newCoord1.first = coord1.first;
                    newCoord2.first = coord2.first;
                    newCoord1.second = (coord1.second + width_ - 1) % width_;
                    newCoord2.second = (coord2.second + width_ - 1) % width_;
                }
                // 2) see if letters are on same column
                else if (coord1.second == coord2.second){
                    newCoord1.first = (coord1.first + width_ - 1) % width_;
                    newCoord2.first = (coord2.first + width_ - 1) % width_;
                    newCoord1.second = coord1.second;
                    newCoord2.second = coord2.second;
                }
                //Must form a rectangle
                else {
                    newCoord1.first = coord1.first;
                    newCoord2.first = coord2.first;
                    newCoord1.second = coord2.second;
                    newCoord2.second = coord1.second;
                }
                //   - Find the letter associated with the new coords
                auto coord2letIter1 = coord2let_.find(newCoord1);
                auto coord2letIter2 = coord2let_.find(newCoord2);
                // Check coordinate is found in map
                if (let2coordIter1 == let2coord_.end() || let2coordIter2 == let2coord_.end()){
                    std::cerr << "[Error] Did not find coordinate in coordinate to letter map\n";
                }
                // Add new letters to output string
                outputText += (*coord2letIter1).second;
                outputText += (*coord2letIter2).second;
            } // end loop over digraphs to decrypt
            break;
        } //end decrypt case
    }
    return outputText;
}