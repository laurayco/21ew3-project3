#ifndef __menu_h__
#define __menu_h__
#include <iostream>

class Menu {
    private:
        const char** entries;
    protected:
        Menu();
        Menu(const char**);
        Menu(const Menu&);
        Menu(const Menu&&);
        virtual void display_menu(std::ostream&) const;
        virtual const bool is_valid_choice(const unsigned int) const;
    public:
        unsigned int get_choice(std::istream&, const bool) const;
        unsigned int get_choice(std::istream&) const;
        unsigned int get_choice(std::istream&, std::ostream&) const;
        const char* get_choice_text(const unsigned int) const;
        size_t get_length() const;
};

#endif