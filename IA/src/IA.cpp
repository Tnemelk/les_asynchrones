#include "../include/IA.hpp"

void play_turn(){
	// Let us now try to do some actions!

	// Action 1: let us try to move cell (id=73) to position (x=42, y=4242)
	actions.add_move_action(73, 42, 4242);
	// Action 2: let us try to divide cell (id=51) to create a child cell of mass=3 towards the (0,0) corner
	actions.add_divide_action(51, 0, 0, 3);
	// Action 3: let us try to create a virus from cell (id=42) towards the (map_width,map_height) corner
	actions.add_create_virus_action(42, p.map_width, p.map_height);
	// Action 4: one time over 420, let us abandon our cells to get new ones the next turn
	if (rand() % 420 == 42)
		actions.add_surrender_action();

	printf("Sending actions...\n");
	session.send_actions(actions);
}