#include <mainmenu.h>
#include <menu.h>

const char* main_menu_entries[] = {
    "Load Animal Data",
    "Generate Data",
    "Display Animal Data",
    "Add Record",
    "Delete Record",
    "Save Animal Data",
    "Quit",
    nullptr
};

MainMenu::MainMenu () : Menu(main_menu_entries) {

}