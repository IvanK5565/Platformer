#pragma once
#include "Enums.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <string>
#include <vector>
#include <cmath>
#include <queue>
using std::string, std::vector, std::queue;

namespace myGame {

    /**
     * @brief Trims leading and trailing whitespaces from a string.
     *
     * Removes spaces, newlines, and tabs from both the start and end of the given string.
     *
     * @param source The input string to trim.
     * @return A new string with leading and trailing whitespaces removed.
     */
    static string trim(const std::string& source) {
        std::string s(source);
        s.erase(0, s.find_first_not_of(" \n\r\t"));
        s.erase(s.find_last_not_of(" \n\r\t") + 1);
        return s;
    }

    /**
     * @brief Trims leading and trailing occurrences of a specific character from a string.
     *
     * Removes all instances of the given character `c` from the start and end of the string.
     *
     * @param source The input string to trim.
     * @param c The character to remove from both ends of the string.
     * @return A new string with the specified character removed from the start and end.
     */
    static string trim(const std::string& source, const char c) {
        std::string s(source);
        s.erase(0, s.find_first_not_of(c));
        s.erase(s.find_last_not_of(c) + 1);
        return s;
    }

    /**
     * @brief Splits a string into substrings based on a delimiter.
     *
     * This function breaks the input string into multiple substrings using the provided delimiter `ch`.
     * It stores the substrings in the `strs` vector and returns the number of substrings.
     *
     * @param txt The input string to split.
     * @param strs The vector where the substrings will be stored.
     * @param ch The delimiter used to split the string.
     * @return The number of substrings created.
     */
    static size_t split(const string& txt, vector<string>& strs, char ch) {
        size_t pos = txt.find(ch);
        size_t initialPos = 0;
        strs.clear();

        // Decompose statement
        while (pos != string::npos) {
            strs.push_back(txt.substr(initialPos, pos - initialPos));
            initialPos = pos + 1;

            pos = txt.find(ch, initialPos);
        }

        // Add the last one
        strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

        return strs.size();
    }

    /**
     * @brief Splits a string into substrings based on a delimiter and returns them as a vector.
     *
     * This is an overloaded function that splits the input string `txt` based on the delimiter `ch`
     * and returns a vector containing the substrings.
     *
     * @param txt The input string to split.
     * @param ch The delimiter used to split the string.
     * @return A vector of substrings split from the original string.
     */
    static vector<string> split(const string& txt, char ch) {
        vector<string> strs;
        split(txt, strs, ch);
        return strs;
    }

    /**
     * @brief Global status code used for tracking the state of operations.
     */
    static StatusCode code;

    /**
     * @brief Queue used for storing error messages.
     */
    static queue<string> errors;

    /**
     * @brief Retrieves and removes the first error message from the error queue.
     *
     * If the error queue is empty, returns an empty string. If there are no more errors,
     * the status code is set to `CORRECT`.
     *
     * @return The first error message in the queue, or an empty string if no errors exist.
     */
    static string getError() {
        if (errors.empty()) return "";
        string buf = errors.front();
        errors.pop();
        if (errors.empty()) code = StatusCode::CORRECT;
        return buf;
    }

    /**
     * @brief Adds an error message to the error queue and sets the status code to `UNCORRECT`.
     *
     * This function adds a given error message to the `errors` queue and sets the global `code`
     * status code to `UNCORRECT` to indicate that an error has occurred.
     *
     * @param mess The error message to add to the queue.
     */
    static void setError(string mess) {
        errors.push(mess);
        code = StatusCode::UNCORRECT;
    }

    /**
     * @brief Loads a texture from a file and creates an SDL_Texture.
     *
     * This function attempts to load an image from a given file path, create an SDL_Texture from it,
     * and return the texture. If there is an error loading the image, it logs the error and returns `NULL`.
     *
     * @param renderer The SDL_Renderer used to create the texture.
     * @param path The file path to the image.
     * @return The created SDL_Texture, or `NULL` if the texture could not be created.
     */
    static SDL_Texture* loadTexture(SDL_Renderer* renderer, string path) {
        SDL_Texture* newTexture = NULL;
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == NULL) {
            printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
            setError("Unable to load image " + path + "!SDL_image Error : " + IMG_GetError() + "\n");
            return NULL;
        }

        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
            setError("Unable to load image " + path + "!SDL_image Error : " + SDL_GetError() + "\n");
            return NULL;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
        return newTexture;
    }
}
