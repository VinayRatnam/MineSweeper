#include <iostream>
#include "Tiles.h"
#include "Board_Creator.h"
#include "MineCalculator.h"
#include <fstream>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <cmath>
#include "Timer.h"
#include "LeaderboardWindow.h"
using namespace std;

//function for centering text
void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

int main() {
    //read numbers from file
    ifstream input("files/config.cfg");
    int columns;
    int rows;
    int mines;
    bool exit = false;


    //read numbers from file
    if (input.is_open()) {
        string str_columns;
        string str_rows;
        string str_mines;
        getline(input, str_columns);
        getline(input, str_rows);
        getline(input, str_mines);

        columns = stoi(str_columns);
        rows = stoi(str_rows);
        mines = stoi(str_mines);

    }
    else {
        cout << "File not opened" << endl;
    }


    sf::RenderWindow welcome_window(sf::VideoMode(columns * 32, rows * 32 + 100), "Minesweeper"/*, sf::Style::Close*/);

    //set default font
    sf::Font font;
    font.loadFromFile("files/font.ttf");


    //customizes the welcome message
    sf::Text welcome_message;
    welcome_message.setFont(font);
    welcome_message.setString("WELCOME TO MINESWEEPER!");
    welcome_message.setCharacterSize(24);
    welcome_message.setFillColor(sf::Color::White);
    welcome_message.setStyle(sf::Text::Bold | sf::Text::Underlined);

    float hor_center = welcome_window.getSize().x / 2.f;
    float welcome_message_vert = welcome_window.getSize().y / 2.f - 150.f;
    setText(welcome_message, hor_center, welcome_message_vert);


    //customizes enter your name text
    sf::Text enter_name;
    enter_name.setFont(font);
    enter_name.setString("Enter your name:");
    enter_name.setCharacterSize(20);
    enter_name.setFillColor(sf::Color::White);

    float enter_name_vert = welcome_window.getSize().y / 2.f - 75.f;
    setText(enter_name, hor_center, enter_name_vert);


    //customize player name
    string name = "|";
    sf::Text player;
    player.setString(name);
    player.setFont(font);
    player.setCharacterSize(18);
    player.setFillColor(sf::Color::Yellow);

    float player_vert = welcome_window.getSize().y / 2.f - 45.f;
    setText(player, hor_center, player_vert);


    //run welcome window
    while (welcome_window.isOpen() && !exit) {
        sf::Event event;

        while (welcome_window.pollEvent(event)) {
            //do different things for different polled events
            switch (event.type) {
                case sf::Event::Closed:
                    welcome_window.close();
                    exit = true;

                case sf::Event::TextEntered:
                    if (name.size() < 11) {
                        if ((64 < event.text.unicode && event.text.unicode < 91) || (96 < event.text.unicode && event.text.unicode < 123)) {
                            char curr_char = static_cast<char>(event.text.unicode);
                            if (name.size() == 1) {
                                curr_char = toupper(curr_char);
                            }
                            else {
                                curr_char = tolower(curr_char);
                            };
                            name.insert(name.end() - 1, curr_char);
                            player.setString(name);

                            //set new position
                            setText(player, hor_center, player_vert);
                        }
                    }

                    if (event.text.unicode == 8 && name.size() > 1) {
                        name.erase(name.end() - 2);
                        player.setString(name);

                        //set new position
                        setText(player, hor_center, player_vert);
                    }

                    else if (event.text.unicode == 10 && name.size() > 1) {
                        welcome_window.close();
                    }

                default:
                    break;
            }
        }

        //create image for every frame
        welcome_window.clear(sf::Color::Blue);
        welcome_window.draw(welcome_message);
        welcome_window.draw(enter_name);
        welcome_window.draw(player);
        welcome_window.display();
    }

    //render game and leaderboard windows and delete "|" from name
    sf::RenderWindow game_window(sf::VideoMode(columns * 32, rows * 32 + 100), "Minesweeper"/*, sf::Style::Close*/);
    name.erase(name.end() - 1);



    //CREATE ALL TEXTURES AND SPRITES

    //create texture and sprite for hidden tile
    sf::Texture hidden_tile_texture;
    if (!hidden_tile_texture.loadFromFile("files/images/tile_hidden.png")) {
        cout << "texture not loaded" << endl;
    }
    sf::Sprite hidden_tile;
    hidden_tile.setTexture(hidden_tile_texture);

    //create texture and sprite for bomb tile
    sf::Texture bomb_tile_texture;
    if (!bomb_tile_texture.loadFromFile("files/images/mine.png")) {
        cout << "mine texture not loaded" << endl;
    }
    sf::Sprite bomb_tile;
    bomb_tile.setTexture(bomb_tile_texture);

    //create texture and sprite for revealed tile
    sf::Texture revealed_tile_texture;
    if (!revealed_tile_texture.loadFromFile("files/images/tile_revealed.png")) {
        cout << "texture not loaded" << endl;
    }
    sf::Sprite revealed_tile;
    revealed_tile.setTexture(revealed_tile_texture);

    //create texture and sprite for number_1
    sf::Texture number_1_texture;
    if (!number_1_texture.loadFromFile("files/images/number_1.png")) {
        cout << "texture not loaded" << endl;
    }
    sf::Sprite number_1;
    number_1.setTexture(number_1_texture);

    //create texture and sprite for number 2
    sf::Texture number_2_texture;
    if (!number_2_texture.loadFromFile("files/images/number_2.png")) {
        cout << "texture not loaded" << endl;
    }
    sf::Sprite number_2;
    number_2.setTexture(number_2_texture);

    //create texture and sprite for number 3
    sf::Texture number_3_texture;
    if (!number_3_texture.loadFromFile("files/images/number_3.png")) {
        cout << "texture not loaded" << endl;
    }
    sf::Sprite number_3;
    number_3.setTexture(number_3_texture);

    //create texture and sprite for number 4
    sf::Texture number_4_texture;
    if (!number_4_texture.loadFromFile("files/images/number_4.png")) {
        cout << "texture not loaded" << endl;
    }
    sf::Sprite number_4;
    number_4.setTexture(number_4_texture);

    //create texture and sprite for number 5
    sf::Texture number_5_texture;
    if (!number_5_texture.loadFromFile("files/images/number_5.png")) {
        cout << "texture not loaded" << endl;
    }
    sf::Sprite number_5;
    number_5.setTexture(number_5_texture);

    //create texture and sprite for number 6
    sf::Texture number_6_texture;
    if (!number_6_texture.loadFromFile("files/images/number_6.png")) {
        cout << "texture not loaded" << endl;
    }
    sf::Sprite number_6;
    number_6.setTexture(number_6_texture);

    //create texture and sprite for number 7
    sf::Texture number_7_texture;
    if (!number_7_texture.loadFromFile("files/images/number_7.png")) {
        cout << "texture not loaded" << endl;
    }
    sf::Sprite number_7;
    number_7.setTexture(number_7_texture);

    //create texture and sprite for number 8
    sf::Texture number_8_texture;
    if (!number_8_texture.loadFromFile("files/images/number_8.png")) {
        cout << "texture not loaded" << endl;
    }
    sf::Sprite number_8;
    number_8.setTexture(number_8_texture);

    //create texture and sprite for play button
    sf::Texture play_button_texture;
    if (!play_button_texture.loadFromFile("files/images/play.png")) {
        cout << "play button not loaded" << endl;
    }
    sf::Sprite play_button;
    play_button.setTexture(play_button_texture);
    play_button.setPosition(columns * 32 - 240, (rows + 0.5f) * 32.f);

    //create texture and sprite for pause button;
    sf::Texture pause_button_texture;
    if (!pause_button_texture.loadFromFile("files/images/pause.png")) {
        cout << "pause button not loaded" << endl;
    }
    sf::Sprite pause_button;
    pause_button.setTexture(pause_button_texture);
    pause_button.setPosition(columns * 32 - 240, (rows + 0.5f) * 32.f);

    //create texture and sprite for leaderboard
    sf::Texture leaderboard_texture;
    if (!leaderboard_texture.loadFromFile("files/images/leaderboard.png")) {
        cout << "leaderboard not loading" << endl;
    }
    sf::Sprite leaderboard;
    leaderboard.setTexture(leaderboard_texture);
    leaderboard.setPosition(columns * 32 - 176, (rows + 0.5f) * 32.f);

    //create texture and sprite for flag
    sf::Texture flag_texture;
    if (!flag_texture.loadFromFile("files/images/flag.png")) {
        cout << "flag texture not loading" << endl;
    }
    sf::Sprite flag;
    flag.setTexture(flag_texture);

    //create texture and sprite for winning face
    sf::Texture winning_face_texture;
    if (!winning_face_texture.loadFromFile("files/images/face_win.png")) {
        cout << "winning face not loading" << endl;
    }
    sf::Sprite winning_face;
    winning_face.setTexture(winning_face_texture);
    winning_face.setPosition(((columns / 2.0f) * 32) - 32, (rows + 0.5) * 32);

    //create texture and sprite for losing face
    sf::Texture losing_face_texture;
    if (!losing_face_texture.loadFromFile("files/images/face_lose.png")) {
        cout << "losing face texture did not load" << endl;
    }
    sf::Sprite losing_face;
    losing_face.setTexture(losing_face_texture);
    losing_face.setPosition(((columns / 2.0f) * 32) - 32, (rows + 0.5) * 32);

    //create texture and sprite for happy face
    sf::Texture happy_face_texture;
    if (!happy_face_texture.loadFromFile("files/images/face_happy.png")) {
        cout << "happy face texture did not load" << endl;
    }
    sf::Sprite happy_face;
    happy_face.setTexture(happy_face_texture);
    happy_face.setPosition(((columns / 2.0f) * 32) - 32, (rows + 0.5) * 32);

    //create texture and sprite for debug button
    sf::Texture debug_texture;
    if (!debug_texture.loadFromFile("files/images/debug.png")) {
        cout << "debug texture did not load" << endl;
    }
    sf::Sprite debug_sprite;
    debug_sprite.setTexture(debug_texture);
    debug_sprite.setPosition(columns * 32 - 304, (rows + 0.5f) * 32.f);

    //set texture and sprite for digits

    sf::Texture all_digits;
    if (!all_digits.loadFromFile("files/images/digits.png")) {
        cout << "digits texture did not load" << endl;
    }

    sf::Sprite digit_sprites[11];
    for (int i = 0; i < 11; i++) {
        digit_sprites[i].setTexture(all_digits);
        digit_sprites[i].setTextureRect(sf::IntRect(i * 21.f, 0, 21, 32));
    }


    //create all objects before running window
    vector<vector<Tile*>> tile_ptrs = initializeBoard(rows, columns);
    bool first_click = false;
    bool pause = false; // false for play, true for pause
    bool debug_mode = false; //initialize as false
    int face = 0; //initializes as happy face
    int remaining = mines;
    Timer timer; //initialize timer class

    while (game_window.isOpen() && !exit) {
        sf::Event event;

        while (game_window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    game_window.close();
                    exit = true;
                    break;

                case sf::Event::MouseButtonPressed:

                    //for left click
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i localPosition = sf::Mouse::getPosition(game_window);

                        //checks if point clicked is in the game window
                        if ((localPosition.x > 0 && localPosition.x < columns*32) && (localPosition.y > 0 && localPosition.y < rows*32 + 100)) {
                            //checks if point clicked is a tile
                            if (localPosition.y < rows * 32) {
                                int i = floor(localPosition.y / 32);
                                int j = floor(localPosition.x / 32);
                                if (face == 0 && !tile_ptrs[i][j]->getRedFlag() && !pause) {
                                    if (!first_click) {
                                        first_click = true;
                                        remaining = mines;
                                        BoardCreator(mines, tile_ptrs, localPosition, remaining);
                                        timer.start();
                                    }


                                    tile_ptrs[i][j]->leftClick();
                                    //if tile clicked was a mine, end game
                                    auto if_bomb = dynamic_cast<BombTile*>(tile_ptrs[i][j]);
                                    if (if_bomb) {
                                        face = 2;
                                        timer.stop();

                                        for (int x = 0; x < tile_ptrs.size(); x++) {
                                            for (int y = 0; y < tile_ptrs[0].size(); y++) {
                                                auto curr_bomb = dynamic_cast<BombTile*>(tile_ptrs[x][y]);
                                                if (curr_bomb) {
                                                    tile_ptrs[x][y]->leftClick();
                                                }
                                            }
                                        }
                                    }

                                    //if tile has no mines nearby, reveal all nearby tiles
                                    auto if_number = dynamic_cast<NumberTile*>(tile_ptrs[i][j]);
                                    if (if_number) {
                                        if (if_number->getTileNumber() == 0) {
                                            vector<vector<bool>> visited(tile_ptrs.size(),vector<bool>(tile_ptrs[0].size(), false));
                                            NoneNearby(tile_ptrs, i, j, visited);
                                        }
                                    }
                                }
                            }

                            //checks if point clicked is the face
                            float face_x_bound = ((columns / 2.f) * 32.f) - 32.f;
                            float face_y_bound = ((rows + 0.5f) * 32.f);
                            if ((localPosition.x > face_x_bound && localPosition.x < face_x_bound + 64) && (localPosition.y > face_y_bound && localPosition.y < face_y_bound + 64)) {
                                EmptyBoard(columns, rows, tile_ptrs);
                                first_click = false;
                                debug_mode = false;
                                face = 0;
                                timer.reset();
                                remaining = mines;
                                pause = false;
                            }

                            //checks if point clicked is debug button
                            float debug_x_bound = columns * 32.f - 304.f;
                            float debug_y_bound = (rows + 0.5f) * 32.f;
                            if ((localPosition.x > debug_x_bound && localPosition.x < debug_x_bound + 64) && (localPosition.y > debug_y_bound && localPosition.y < debug_y_bound + 64) && !pause) {
                                if (face == 0 && first_click) {
                                    debug_mode = !debug_mode;
                                }
                            }

                            //checks if point clicked is pause/play button
                            float pause_x_bound = columns * 32.f - 240.f;
                            float pause_y_bound = (rows + 0.5f) * 32.f;
                            if ((localPosition.x > pause_x_bound && localPosition.x < pause_x_bound + 64.f) && (localPosition.y > pause_y_bound && localPosition.y < pause_y_bound + 64.f)) {
                                if (face == 0) {
                                    pause = !pause;
                                    if (pause) {
                                        timer.stop();
                                    }
                                    else {
                                        timer.start_again();
                                    }
                                }


                            }

                            //checks if point clicked is leaderboard button
                            float leaderboard_x_bound = columns * 32.f - 176.f;
                            float leaderboard_y_bound = (rows + .05f) * 32.f;
                            if ((localPosition.x > leaderboard_x_bound && localPosition.x < leaderboard_x_bound + 64.f) && (localPosition.y > leaderboard_y_bound && localPosition.y < leaderboard_y_bound + 64.f)) {
                                bool pause_2 = pause;
                                pause = true;
                                if (first_click && !pause_2) {
                                    timer.stop();
                                }

                                if (face == 0) {
                                    game_window.draw(play_button);
                                }

                                for (int i = 0; i < rows; i++) {
                                    for (int j = 0; j < columns; j++) {
                                        revealed_tile.setPosition(j * 32.f, i * 32.f);
                                        game_window.draw(revealed_tile);
                                    }
                                }

                                game_window.display();

                                //load leaderboard data and open leaderboard window
                                vector<vector<string>> leaderboard_data = loadLeaderboardData("files/leaderboard.txt");
                                string total_text = allText(leaderboard_data);
                                sf::RenderWindow leaderboard_window(sf::VideoMode(columns * 16, (rows * 16)+ 50.f), "Minesweeper"/*, sf::Style::Close*/);

                                //make sf::texts for leaderboard text
                                sf::Text leaderboard_text;
                                leaderboard_text.setFont(font);
                                leaderboard_text.setString("LEADERBOARD");
                                leaderboard_text.setCharacterSize(20);
                                leaderboard_text.setFillColor(sf::Color::White);
                                leaderboard_text.setStyle(sf::Text::Bold | sf::Text::Underlined);
                                setText(leaderboard_text, columns * 8.f, (rows * 8.f) - 95.f);

                                sf::Text leaderboard_content;
                                leaderboard_content.setFont(font);
                                leaderboard_content.setString(total_text);
                                leaderboard_content.setCharacterSize(18);
                                leaderboard_content.setFillColor(sf::Color::White);
                                leaderboard_content.setStyle(sf::Text::Bold);
                                setText(leaderboard_content, columns * 8.f, (rows * 8.f) + 45.f);

                                leaderboard_window.clear(sf::Color::Blue);
                                leaderboard_window.draw(leaderboard_content);
                                leaderboard_window.draw(leaderboard_text);
                                leaderboard_window.display();

                                while (leaderboard_window.isOpen()) {
                                    sf::Event leaderboard_event;
                                    while (leaderboard_window.pollEvent(leaderboard_event)) {
                                        if (leaderboard_event.type == sf::Event::Closed) {
                                            leaderboard_window.close();
                                        }
                                    }
                                    leaderboard_window.requestFocus();
                                }

                                if (first_click && !pause_2 && face == 0) {
                                    timer.start_again();
                                }
                                if (!pause_2) {
                                    pause = false;
                                }


                            }
                        }
                    }

                    //for right click
                    if (event.mouseButton.button == sf::Mouse::Right) {
                        sf::Vector2i localPosition = sf::Mouse::getPosition(game_window);

                        //checks if point clicked is in the game window
                        if ((localPosition.x > 0 && localPosition.x < columns*32) && (localPosition.y > 0 && localPosition.y < rows*32)) {
                            //checks if point clicked is a tile
                            if (localPosition.y < rows * 32 && !pause) {
                                if (face == 0) {
                                    int i = floor(localPosition.y / 32);
                                    int j = floor(localPosition.x / 32);

                                    tile_ptrs[i][j]->rightClick(remaining);
                                }
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        //checks if user won game
        if (winCondition(tile_ptrs) && timer.getRunning()) {
            face = 1;
            timer.stop();
            //sets red flag on all bomb tiles when game is won
            for (int i = 0; i < tile_ptrs.size(); i++) {
                for (int j = 0; j < tile_ptrs[0].size(); j++) {
                    auto curr_tile = dynamic_cast<BombTile*>(tile_ptrs[i][j]);
                    if (curr_tile) {
                        if (!curr_tile->getRedFlag()) {
                            curr_tile->rightClick(remaining);
                        }
                    }
                }
            }

            vector<vector<string>> file_data = loadLeaderboardData("files/leaderboard.txt");
            int winner_time = timer.getTime();

            if (updateLeaderboard(winner_time, name, file_data)) {
                //write to the leaderboard file
                ofstream new_leaderboard("files/leaderboard.txt");


                for (int j = 0; j < file_data.size(); j++) {
                    new_leaderboard << file_data[j][0] << "," << file_data[j][1] << endl;
                }
            }


        }


        game_window.clear(sf::Color::White);

        //draw the pause/play button
        if (pause) { //paused
            game_window.draw(play_button);
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    revealed_tile.setPosition(j * 32.f, i * 32.f);
                    game_window.draw(revealed_tile);
                }
            }
        }
        else { //in play mode
            game_window.draw(pause_button);
            for (int i = 0; i < tile_ptrs.size(); i++) {
                for (int j = 0; j < tile_ptrs[0].size(); j++) {


                    //displays hidden tile if tile has not been revealed yet
                    if (!tile_ptrs[i][j]->getCurrDisplay()) {
                        hidden_tile.setPosition(tile_ptrs[i][j]->getXPosition(), tile_ptrs[i][j]->getYPosition());
                        game_window.draw(hidden_tile);

                        //checks if tile should have red flag on it
                        if (tile_ptrs[i][j]->getRedFlag()) {
                            flag.setPosition(tile_ptrs[i][j]->getXPosition(), tile_ptrs[i][j]->getYPosition());
                            game_window.draw(flag);
                        }
                    }

                        //displays revealed tile
                    else {
                        revealed_tile.setPosition(tile_ptrs[i][j]->getXPosition(), tile_ptrs[i][j]->getYPosition());
                        game_window.draw(revealed_tile);

                        //checks if tile should have red flag on it
                        if (tile_ptrs[i][j]->getRedFlag()) {
                            flag.setPosition(tile_ptrs[i][j]->getXPosition(), tile_ptrs[i][j]->getYPosition());
                            game_window.draw(flag);
                        }

                        //displays mine or number
                        NumberTile* curr_tile = dynamic_cast<NumberTile*>(tile_ptrs[i][j]);
                        if (curr_tile) {
                            int tile_num = curr_tile->getTileNumber();

                            //display right number
                            switch (tile_num) {
                                case 0: {
                                    break;
                                }
                                case 1:
                                    number_1.setPosition(tile_ptrs[i][j]->getXPosition(), tile_ptrs[i][j]->getYPosition());
                                    game_window.draw(number_1);
                                    break;
                                case 2:
                                    number_2.setPosition(tile_ptrs[i][j]->getXPosition(), tile_ptrs[i][j]->getYPosition());
                                    game_window.draw(number_2);
                                    break;
                                case 3:
                                    number_3.setPosition(tile_ptrs[i][j]->getXPosition(), tile_ptrs[i][j]->getYPosition());
                                    game_window.draw(number_3);
                                    break;
                                case 4:
                                    number_4.setPosition(tile_ptrs[i][j]->getXPosition(), tile_ptrs[i][j]->getYPosition());
                                    game_window.draw(number_4);
                                    break;
                                case 5:
                                    number_5.setPosition(tile_ptrs[i][j]->getXPosition(), tile_ptrs[i][j]->getYPosition());
                                    game_window.draw(number_5);
                                    break;
                                case 6:
                                    number_6.setPosition(tile_ptrs[i][j]->getXPosition(), tile_ptrs[i][j]->getYPosition());
                                    game_window.draw(number_6);
                                    break;
                                case 7:
                                    number_7.setPosition(tile_ptrs[i][j]->getXPosition(), tile_ptrs[i][j]->getYPosition());
                                    game_window.draw(number_7);
                                    break;
                                case 8:
                                    number_8.setPosition(tile_ptrs[i][j]->getXPosition(), tile_ptrs[i][j]->getYPosition());
                                    game_window.draw(number_8);
                                    break;
                            }
                        }
                        else {
                            //game ends
                            bomb_tile.setPosition(tile_ptrs[i][j]->getXPosition(), tile_ptrs[i][j]->getYPosition());
                            game_window.draw(bomb_tile);
                        }
                    }
                }
            }
        }




        //draw buttons
        game_window.draw(debug_sprite);
        game_window.draw(leaderboard);

        switch (face) {
            case 0:
                game_window.draw(happy_face);
                break;
            case 1:
                game_window.draw(winning_face);
                break;
            case 2:
                game_window.draw(losing_face);
                break;
        }

        //draw counter
        if (remaining >= 0) {
            //hundred digit
            int hundreds = remaining / 100;
            digit_sprites[hundreds].setPosition(33.f, 32.f * (rows+ 0.5f) + 16);
            game_window.draw(digit_sprites[hundreds]);

            //tens digit
            int tens = (remaining % 100) / 10;
            digit_sprites[tens].setPosition(54.f, 32.f * (rows+ 0.5f) + 16);
            game_window.draw(digit_sprites[tens]);

            //ones digit
            int ones = remaining - (hundreds * 100) - (tens * 10);
            digit_sprites[ones].setPosition(75.f, 32.f * (rows+ 0.5f) + 16);
            game_window.draw(digit_sprites[ones]);
        }
        else { //for negative numbers
            //draw negative sign
            digit_sprites[10].setPosition(33.f, 32.f * (rows+ 0.5f) + 16);
            game_window.draw(digit_sprites[10]);

            int abs_val = -1 * remaining;

            //tens digit
            int tens = abs_val / 10;
            digit_sprites[tens].setPosition(54.f, 32.f * (rows+ 0.5f) + 16);
            game_window.draw(digit_sprites[tens]);

            //ones digit
            int ones = abs_val % 10;
            digit_sprites[ones].setPosition(75.f, 32.f * (rows+ 0.5f) + 16);
            game_window.draw(digit_sprites[ones]);
        }

        //draw timer
        int time_passed = timer.getTime();
        int minutes_passed = time_passed / 60;
        int seconds_passed = time_passed - (minutes_passed * 60);

        //draw minutes digits
        int first_digit = minutes_passed / 10;
        digit_sprites[first_digit].setPosition(columns * 32.f - 97.f, 32 * (rows + 0.5f) + 16.f);
        game_window.draw(digit_sprites[first_digit]);
        int second_digit = minutes_passed % 10;
        digit_sprites[second_digit].setPosition(columns * 32.f - 76.f, 32 * (rows + 0.5f) + 16.f);
        game_window.draw(digit_sprites[second_digit]);

        //draw seconds digits
        int third_digit = seconds_passed / 10;
        digit_sprites[third_digit].setPosition(columns * 32.f - 54.f, 32 * (rows + 0.5f) + 16.f);
        game_window.draw(digit_sprites[third_digit]);
        int fourth_digit = seconds_passed % 10;
        digit_sprites[fourth_digit].setPosition(columns * 32.f - 33.f, 32 * (rows + 0.5f) + 16.f);
        game_window.draw(digit_sprites[fourth_digit]);


        //reveals all mines in debug mode
        if (debug_mode && !pause) {
            for (int i = 0; i < tile_ptrs.size(); i++) {
                for (int j = 0; j < tile_ptrs[0].size(); j++) {
                    auto curr_bomb = dynamic_cast<BombTile*>(tile_ptrs[i][j]);
                    if (curr_bomb) {

                        bomb_tile.setPosition(tile_ptrs[i][j]->getXPosition(), tile_ptrs[i][j]->getYPosition());
                        game_window.draw(bomb_tile);
                    }
                }
            }
        }


        game_window.display();
    }

    return 0;
}