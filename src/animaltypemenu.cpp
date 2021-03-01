#include <animaltypemenu.h>
#include <menu.h>

const char* animaltype_menu_entries[] = {
    "Oviparous",
    "Mammal",
    nullptr
};

AnimalTypeMenu::AnimalTypeMenu () : Menu(animaltype_menu_entries) {

}