#ifndef MENU_VARIABLES_H_
#define MENU_VARIABLES_H_


enum MenuScreen {
    menuscreen_main,
    menuscreen_options
};

enum MenuMainOption {
    menumain_startgame,
    menumain_options
};

class MenuVariables {
public:
    int currentScreen = menuscreen_main;
    int menuMainOption = menumain_startgame;
    int difficultyLevel = 1;
};

extern MenuVariables menu;


#endif