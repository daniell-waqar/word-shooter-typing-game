#include <SDL.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;
#include <cctype>

class Game {
public:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    vector<string> dictionary;
    string currentWord = "";
    SDL_Color validColor = {0, 255, 0, 255};   // Green for valid words
    SDL_Color invalidColor = {255, 0, 0, 255}; // Red for invalid words
    SDL_Color currentColor;                    // Color of the rectangle

    Game() {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Word Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_StartTextInput();  // Start SDL text input
        loadDictionary("/home/waqar/WordShooter/src/words_alpha.txt");  // Set the absolute path to the dictionary
        currentColor = invalidColor; // Start with invalid color
    }

    ~Game() {
        SDL_StopTextInput();
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void loadDictionary(const string& filepath) {
        ifstream file(filepath);
        if (!file) {
            cout << "Failed to open dictionary file at: " << filepath << endl;
            return;
        }
        string word;
        while (getline(file, word)) {
            word = normalize(word);
            if (!word.empty()) {
                dictionary.push_back(word);
            }
        }
        cout << "Total words loaded: " << dictionary.size() << endl;
    }

    string normalize(const string& word) {
        string normalized;
        for (char c : word) {
            if (isalnum(static_cast<unsigned char>(c))) {
                normalized += tolower(static_cast<unsigned char>(c));
            }
        }
        return normalized;
    }

    bool isValidWord(const string& word) {
        string normWord = normalize(word);
        bool valid = find(dictionary.begin(), dictionary.end(), normWord) != dictionary.end();
        cout << "Checking word: " << normWord << " - Valid: " << (valid ? "Yes" : "No") << endl;
        return valid;
    }

    void run() {
        bool quit = false;
        SDL_Event event;
        while (!quit) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                } else if (event.type == SDL_TEXTINPUT) {
                    currentWord += event.text.text;
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_BACKSPACE && !currentWord.empty()) {
                        currentWord.pop_back();
                    } else if (event.key.keysym.sym == SDLK_RETURN) {
                        if (isValidWord(currentWord)) {
                            currentColor = validColor;
                            cout << "Valid word: " << currentWord << endl;
                        } else {
                            currentColor = invalidColor;
                            cout << "Invalid word: " << currentWord << endl;
                        }
                        currentWord.clear();
                    }
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Clear the screen
            SDL_RenderClear(renderer);

            // Draw the rectangle with the current color
            SDL_Rect rect = {100, 100, 600, 400};
            SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
            SDL_RenderFillRect(renderer, &rect);

            SDL_RenderPresent(renderer);
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
