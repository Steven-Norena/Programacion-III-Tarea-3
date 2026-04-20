//
// Created by STEVEN on 18/04/2026.
//

#include "CommandCenter.h"

int main() {
    Entity hero("Warrior", 100);
    CommandCenter center(hero);

    center.registerCommand("heal", std::bind(healFunction, std::ref(hero), std::placeholders::_1));

    center.registerCommand("move", [&hero](const std::list<std::string>& args) {
        if (args.size() != 2) throw std::invalid_argument("Move requiere x y."); //
        auto it = args.begin();
        int nx = std::stoi(*it++); // [cite: 89]
        int ny = std::stoi(*it);
        hero.setPosition(nx, ny);
    });

    center.registerCommand("attack", AttackCounter(hero));


    center.registerCommand("reset", [&hero](const std::list<std::string>& args) {
        hero.reset();
    });

    std::list<std::pair<std::string, std::list<std::string>>> steps1 = {
        {"move", {"0", "0"}},
        {"heal", {"20"}}
    };
    center.registerMacro("tactical_retreat", steps1);

    std::list<std::pair<std::string, std::list<std::string>>> steps2 = {
        {"attack", {"10"}},
        {"attack", {"15"}},
        {"attack", {"20"}}
    };
    center.registerMacro("full_assault", steps2);

    std::list<std::pair<std::string, std::list<std::string>>> steps3 = {
        {"reset", {}},
        {"move", {"1", "1"}}
    };
    center.registerMacro("emergency_reset", steps3);

    center.execute("move", {"10", "20"});
    center.execute("heal", {"abc"});
    center.executeMacro("full_assault");  // Macro
    center.executeMacro("tactical_retreat");

    center.removeCommand("attack");
    center.execute("attack", {"10"});

    center.showHistory();
    std::cout << "\nEstado Final: " << hero.getStatus() << std::endl;

    return 0;
}