#include "menu.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

Menu::Menu() : entries { nullptr } {

}

Menu::Menu(const char** options) : entries(options) {

}

Menu::Menu(const Menu& other) : entries(other.entries) {

}

Menu::Menu(const Menu&& other) : entries(other.entries) {

}

const bool Menu::is_valid_choice(const unsigned int choice) const {
    return choice < this->get_length();
}

size_t Menu::get_length() const {
    const char** ptr = this->entries;
    while(*ptr++!=nullptr);
    return (ptr - this->entries) - 1;
}

void Menu::display_menu(std::ostream& out) const {
    const char** ptr = this->entries;
    while(*ptr!=nullptr) {
        out << (ptr - this->entries) + 1 << ' ' << *(ptr++) << std::endl;
    }
}

unsigned int Menu::get_choice(std::istream& in, const bool validate) const {
    if(validate) {
        while(true) {
            const unsigned int choice = this->get_choice(in, false);
            if(this->is_valid_choice(choice)) {
                return choice;
            }
        }
    } else {
        do {
            int choice;
            
            if(!(in >> choice)) {
                in.clear();
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return static_cast<int>(choice - 1); // adjust so that it is array-aligned.
            }
        } while(true);
    }
}

unsigned int Menu::get_choice(std::istream& in) const {
    return this->get_choice(in, true);
}

unsigned int Menu::get_choice(std::istream& in, std::ostream& out) const {
    while(true) {
        this->display_menu(out);
        const unsigned int choice = this->get_choice(in, false);
        if(this->is_valid_choice(choice)) {
            return choice;
        }
    }
}

const char* Menu::get_choice_text(const unsigned int choice) const {
    return entries[choice];
}