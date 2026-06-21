#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "pgnreader.h"

class ComboBox
{
private:
    std::vector<std::pair<std::string, int>> items; // nome file, numero partite
    int selectedIndex;
    sf::RectangleShape box;
    sf::Text selectedText;
    sf::Font font;
    bool isOpen;
    float itemHeight;

public:
    ComboBox();
    ~ComboBox();

    void reset();
    void setPosition(float x, float y);
    void setSize(float width, float height);
    void addItem(const std::string &filename, int gameCount);
    void loadItems(const std::vector<std::string> &pgnFiles, PGNReader &reader);
    void clear();

    void handleKeyPress(sf::Keyboard::Key key);
    void handleMouseClick(float x, float y);

    void draw(sf::RenderWindow &window);

    int getSelectedIndex() const;
    std::string getSelectedFileName() const;
    int getSelectedGameCount() const;

    void selectNext();
    void selectPrevious();
    void confirmSelection();
    void toggleOpen();
};

#endif
