#include <searchtypemenu.h>
#include <menu.h>

const char* searchtype_menu_entries[] = {
    "By Tracking Number",
    "By Name",
    nullptr
};

SearchTypeMenu::SearchTypeMenu () : Menu(searchtype_menu_entries) {

}