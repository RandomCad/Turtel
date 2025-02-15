#pragma once
#include <stack>
#include <stdexcept>

/**
 * @brief Repräsentiert den Zustand der Turtle.
 */
struct TurtleState {
    double x;      ///< X-Position
    double y;      ///< Y-Position
    double angle;  ///< Richtung (in Grad oder Radiant)
};

/**
 * @brief Ein Stack zum Speichern und Abrufen von Turtle-Zuständen.
 */
class MarkerStack {
public:
    /**
     * @brief Speichert den aktuellen Zustand der Turtle.
     *
     * @param state Der zu speichernde Zustand.
     */
    void mark(const TurtleState &state) {
        markers.push(state);
    }
    
    /**
     * @brief Gibt den zuletzt gespeicherten Zustand zurück und entfernt ihn.
     *
     * @return Der zuletzt gespeicherte Turtle-Zustand.
     * @throws std::runtime_error, wenn kein Marker vorhanden ist.
     */
    TurtleState pop() {
        if (markers.empty()) {
            throw std::runtime_error("Kein Marker vorhanden.");
        }
        TurtleState state = markers.top();
        markers.pop();
        return state;
    }
    
private:
    std::stack<TurtleState> markers; ///< Intern verwendeter Stack.
};
