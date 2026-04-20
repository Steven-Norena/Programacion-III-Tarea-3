//
// Created by STEVEN on 18/04/2026.
//

#ifndef PROGRAMACION_III_TAREA_3_COMMANDCENTER_H
#define PROGRAMACION_III_TAREA_3_COMMANDCENTER_H

#include <functional>
#include <list>
#include <string>
#include <map>
#include <iostream>

class Entity {
    std::string name;
    int health;
    int x, y;

public:
    Entity(const std::string&& n, const int& h) : name(n), health(h), x(0), y(0) {}

    void modifyHealth(const int& amount) { health += amount; }
    void setPosition(const int& nx, const int& ny) { x = nx; y = ny; }
    void reset() { health = 100; x = 0; y = 0; }

    std::string getStatus() const {
        return name + " [HP: " + std::to_string(health) + " | Pos: (" +
               std::to_string(x) + "," + std::to_string(y) + ")]";
    }
};

using Command = std::function<void(const std::list<std::string>&)>;

class AttackCounter {
    Entity& entity;
    int count = 0;
public:
    explicit AttackCounter(Entity& e) : entity(e) {}
    void operator()(const std::list<std::string>& args) {
        if (args.empty()) throw std::invalid_argument("Faltan argumentos para attack.");
        int damage = std::stoi(args.front());
        entity.modifyHealth(-damage);
        count++;
        std::cout << "(Functor) Ataques realizados: " << count << std::endl;
    }
};


inline void healFunction(Entity& e, const std::list<std::string>& args) {
    if (args.empty()) throw std::invalid_argument("Heal requiere 1 argumento."); //
    int amount = std::stoi(args.front());
    if (amount < 0) throw std::domain_error("Valor negativo no permitido.");
    e.modifyHealth(amount);
}


class CommandCenter {
    std::map<std::string, Command> commands;
    std::list<std::string> history;

    std::map<std::string, std::list<std::pair<std::string, std::list<std::string>>>> macros;

    Entity& target;

public:
    explicit CommandCenter(Entity& e) : target(e) {}

    void registerCommand(const std::string& name, const Command& cmd) {
        commands[name] = cmd;
    }

    void execute(const std::string& name, const std::list<std::string>& args) {

        const std::map<std::string, Command>::iterator it = commands.find(name);

        if (it != commands.end()) {
            std::string before = target.getStatus();

            try {
                it->second(args);
                std::string after = target.getStatus();
                history.push_back("Cmd: " + name + " | " + before + " -> " + after);
            } catch (const std::exception& e) {
                std::cerr << "Error en " << name << ": " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Comando '" << name << "' no encontrado." << std::endl;
        }
    }

    void removeCommand(const std::string& name) {
        auto it = commands.find(name);
        if (it != commands.end()) {
            commands.erase(it);
            std::cout << "Comando '" << name << "' eliminado." << std::endl;
        }
    }

    void registerMacro(const std::string& name, const std::list<std::pair<std::string, std::list<std::string>>>& steps) {
        macros[name] = steps;
    }

    void executeMacro(const std::string& name) {
        auto itMacro = macros.find(name);
        if (itMacro != macros.end()) {
            std::cout << "--- Ejecutando Macro: " << name << " ---" << std::endl;

            auto& steps = itMacro->second;
            for (auto itStep = steps.begin(); itStep != steps.end(); ++itStep) {
                this->execute(itStep->first, itStep->second);
            }
        } else {
            std::cerr << "Macro '" << name << "' no existe." << std::endl;
        }
    }

    void showHistory() {
        std::cout << "\n--- HISTORIAL DE EJECUCIÓN ---" << std::endl;
        for (std::list<std::string>::iterator it = history.begin(); it != history.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }
};

#endif //PROGRAMACION_III_TAREA_3_COMMANDCENTER_H