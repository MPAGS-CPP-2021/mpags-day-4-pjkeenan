#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

#include "CipherMode.hpp"

#include <cstddef>
#include <string>
#include <vector>
#include <map>

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */
class PlayfairCipher {
  public:
    /**
     * \brief Create a new PlayfairCipher, with the given string as the key
     *
     * \param key the string to use as the key in the cipher
     */
    explicit PlayfairCipher(const std::string& key);

    /**
     * \brief Set the cipher key
     *
     * \param key the string to use as the key in the cipher
     */
    void setKey(const std::string& key);

    /**
     * \brief Apply the cipher to the provided text
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    std::string applyCipher(std::string inputText,
                            const CipherMode cipherMode);

    /**
     * \brief Set all cases of letter J in string to letter I
     *
     * \param key The string to perform the switch on
     */
    void changeJtoI(std::string& testKey);

   private:
    /// The cipher key, default to empty string
    std::string key_{""};
    /// The width of the grid
    const std::size_t width_ {5};
    /// Object type to hold letter coorinates on the grid
    typedef std::pair <int, int> coordinate;
    /// Map from a letter to a grid coordinate
    using Letter2Coordinate = std::map <char, coordinate>;
    Letter2Coordinate let2coord_;
    /// Map from a coordinate to a letter
    using Coordinate2Letter = std::map <coordinate, char>;
    Coordinate2Letter coord2let_;
};

#endif