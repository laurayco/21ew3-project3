#include <yesnomenu.h>
#include <menu.h>

const char* yesno_menu_entries[] = {
    "No",
    "Yes",
    nullptr
};

YesNoMenu::YesNoMenu () : Menu(yesno_menu_entries) {

}