// combobox.cpp
#include "combobox.h"
#include <iostream>
#include <filesystem>

ComboBox::ComboBox() : selectedIndex(0),
                       isOpen(false), itemHeight(40.0f)
{
    if (!font.loadFromFile("../fonts/arial.ttf"))
    {
        std::cerr << "Error loading font" << std::endl;
    }

    box.setFillColor(sf::Color::White);
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(2.0f);

    selectedText.setFont(font);
    selectedText.setCharacterSize(16);
    selectedText.setFillColor(sf::Color::Black);
}

ComboBox::~ComboBox() {}

void ComboBox::setPosition(float x, float y)
{
    box.setPosition(x, y);
    selectedText.setPosition(x + 10, y + 10);
}

void ComboBox::setSize(float width, float height)
{
    box.setSize(sf::Vector2f(width, height));
    itemHeight = height;
}

void ComboBox::addItem(const std::string &filename, int gameCount)
{
    std::string displayName = std::filesystem::path(filename).stem().string();
    std::string itemText = displayName;
    items.push_back({itemText, gameCount});
}

void ComboBox::loadItems(const std::vector<std::string> &pgnFiles, PGNReader &reader)
{
    clear();
    for (const auto &file : pgnFiles)
    {
        if (reader.readFile(file))
        {
            addItem(file, reader.getGamesCount());
        }
    }
    selectedIndex = 0;
}

void ComboBox::clear()
{
    items.clear();
    selectedIndex = 0;
}

void ComboBox::handleKeyPress(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Up)
        selectPrevious();
    else if (key == sf::Keyboard::Down)
        selectNext();
    else if (key == sf::Keyboard::Return)
        confirmSelection();
}

void ComboBox::handleMouseClick(float x, float y)
{
    if (box.getGlobalBounds().contains(x, y))
    {
        toggleOpen();
    }
}

void ComboBox::draw(sf::RenderWindow &window)
{
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(items.size()))
    {
        selectedText.setString(items[selectedIndex].first);
    }

    window.draw(box);
    window.draw(selectedText);
}

int ComboBox::getSelectedIndex() const
{
    return selectedIndex;
}

std::string ComboBox::getSelectedFileName() const
{
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(items.size()))
    {
        return items[selectedIndex].first;
    }
    return "";
}

int ComboBox::getSelectedGameCount() const
{
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(items.size()))
    {
        return items[selectedIndex].second;
    }
    return 0;
}

void ComboBox::selectNext()
{
    if (selectedIndex < static_cast<int>(items.size()) - 1)
    {
        selectedIndex++;
    }
}

void ComboBox::selectPrevious()
{
    if (selectedIndex > 0)
    {
        selectedIndex--;
    }
}

void ComboBox::confirmSelection()
{
    isOpen = false;
}

void ComboBox::reset()
{
    selectedIndex = 0;
    isOpen = false;
}

void ComboBox::toggleOpen()
{
    isOpen = !isOpen;
}
