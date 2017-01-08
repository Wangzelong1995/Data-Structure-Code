/*
 * CS 106B Trailblazer
 * This file contains the main method to run the overall program.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp, Keith Schwarz, et al
 * @version 2014/11/19 (initial version for 14au)
 */

#include "gevents.h"
#include "gwindow.h"
#include "TrailblazerGUI.h"

static const std::string WINDOW_TITLE = "CS 106B Trailblazer";
static const bool RANDOM_USE_SEED = false;   // true to get predictable random mazes

/*
 * Prints an introductory text message on the screen.
 */
static void intro();

/*
 * Main program.
 */
int main() {
    if (RANDOM_USE_SEED) {
        setRandomSeed(106);
    }
    
    intro();

    // create GUI window and position the console to its right
    setConsoleLocation(800 + 6 * WorldAbstract::WINDOW_MARGIN, 20);   // temporary
    // setConsoleSize(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    setConsoleEventOnClose(true);
    
    TrailblazerGUI gui(WINDOW_TITLE);
    gui.snapConsoleLocation();

    // main event loop to process events as they happen
    while (true) {
        GEvent e = waitForEvent(ACTION_EVENT | MOUSE_EVENT | WINDOW_EVENT);
        if (e.getEventType() == MOUSE_CLICKED || e.getEventType() == MOUSE_MOVED) {
            gui.processMouseEvent(GMouseEvent(e));
        } else if (e.getEventClass() == ACTION_EVENT) {
            gui.processActionEvent(GActionEvent(e));
        } else if (e.getEventClass() == WINDOW_EVENT) {
            gui.processWindowEvent(GWindowEvent(e));
        }
    }
    return 0;
}

static void intro() {
    std::cout << "Welcome to CS 106B Trailblazer!" << std::endl;
    std::cout << "This program searches for paths through graphs" << std::endl;
    std::cout << "representing roadmaps. It demonstrates several graph " << std::endl;
    std::cout << "algorithms for finding paths, such as breadth-first" << std::endl;
    std::cout << "search (BFS), Dijkstra's Algorithm, and A* search," << std::endl;
    std::cout << "You can use alternate route to find a different path." << std::endl;
}
