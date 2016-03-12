#include "../include/IA.hpp"
#include <vector>

string address;
string port_str;
int port;
string name;
Actions * actions;
GameParameters * p;
Session * session;
Welcome * welcome;
set<int> played_cells;
set<int> targeted_neutral_cells;


void play_turn(){
	played_cells.clear();
	targeted_neutral_cells.clear();
	
	/** strategie de fuite **/
	
	// pour chacune de mes cellules
	for (const TurnPlayerCell & cell : session->my_player_cells())
	{
		if (*(targeted_neutral_cells.lower_bound(cell.pcell_id))==cell.pcell_id) continue;
		// recupere l'iD de la cellule ennemie la + dangereuse
		vector<int> dangerousCells = isInDangerFromAnotherCell(cell);
		if (dangerousCells.size())
		{
			int ennemiIndex = getNearestDangerousCell(dangerousCells, cell);
			goToOppositeDirectionFromDangerousCell(ennemiIndex, cell);
			played_cells.insert(cell.pcell_id);
			continue;
		}
		vector<NeutralCell> edible = get_edible_neutrals_in_range(cell);
		if (edible.size() > 0){
			actions->add_move_action(cell.pcell_id, edible[0].position.x, edible[0].position.y);
			played_cells.insert(cell.pcell_id);
			targeted_neutral_cells.insert(edible[0].id);
		}

		// DEFAULT : avance Random
		actions->add_move_action(cell.pcell_id, float(rand())/RAND_MAX*p->map_width, float(rand())/RAND_MAX*p->map_height);
		played_cells.insert(cell.pcell_id);
	}
		

	
	
/**	// Let us now try to do some actions!

	// Action 1: let us try to move cell (id=73) to position (x=42, y=4242)
	actions->add_move_action(73, 42, 4242);
	// Action 2: let us try to divide cell (id=51) to create a child cell of mass=3 towards the (0,0) corner
	actions->add_divide_action(51, 0, 0, 3);
	// Action 3: let us try to create a virus from cell (id=42) towards the (map_width,map_height) corner
	actions->add_create_virus_action(42, p->map_width, p->map_height);
	// Action 4: one time over 420, let us abandon our cells to get new ones the next turn
	if (rand() % 420 == 42)
		actions->add_surrender_action();

	printf("Sending actions...\n");
**/
	session->send_actions(*actions);
}


// distance entre 2 coordonnées
float dist(float x1, float x2, float y1, float y2)
{
	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

// distance entre 2 cellules
float dist(const TurnPlayerCell & C1, const TurnPlayerCell & C2)
{
	return dist( C1.position.x, C2.position.x, C1.position.y, C2.position.y);
}

// distance entre 2 positions
float dist(const Position & p1,const Position & p2)
{
	return dist( p1.x, p2.x, p1.y, p2.y);
}

// get radius
float radius(const TurnPlayerCell & C)
{
	return C.mass*p->radius_factor;
}
// get radius
float radius(const NeutralCell & C)
{
	return C.mass*p->radius_factor;
}
// get radius
float radius(const TurnVirus & C)
{
	return p->virus_mass*p->radius_factor;
}

// distance qu'une cellule peut franchir en 1 tour
float moveDistance(const TurnPlayerCell & cell)
{
	return max(0.0f, p->base_cell_speed - p->speed_loss_factor * cell.mass);
}

// Est que myCell peut être mangé par ennemyCell au prochain tour ?
bool dangerEnnemyCell(const TurnPlayerCell & ennemyCell, const TurnPlayerCell & myCell)
{
	if(
		(radius(myCell) < radius(ennemyCell)) &&
		(dist(ennemyCell, myCell) < (moveDistance(ennemyCell) + radius(ennemyCell)))
	) 
        return true;
    else
        return false;
}

// Est que myCell peut manger ennemyCell au prochain tour ?
bool canEatEnnemyCell(const TurnPlayerCell & ennemyCell, const TurnPlayerCell & myCell)
{
	if(
		(radius(myCell) > radius(ennemyCell)) &&
		(dist(ennemyCell, myCell) < (moveDistance(myCell) + radius(myCell)))
	) 
        return true;
    else
        return false;
}

// teste si myCell est en danger imminent
vector<int> isInDangerFromAnotherCell(const TurnPlayerCell & myCell)
{
	vector<int> dangerousEnnemyCells;
	// teste tous les ennemis
	int cpt=0;
	for (const TurnPlayerCell & cell : session->ennemy_player_cells())
	{
		// si en danger
		if (dangerEnnemyCell(cell, myCell))
		{
			dangerousEnnemyCells.push_back(cpt);
		}
		cpt++;
	}	
	return dangerousEnnemyCells;
}

// recupere l'index du celle ennemi le + dangereux
int getNearestDangerousCell(vector<int> dangerousCells, const TurnPlayerCell & myCell)
{
	double minDist = dist(session->ennemy_player_cells()[0], myCell);
	int index = 1;
	for (int i=1; i<dangerousCells.size(); i++)
	{
		if (dist(session->ennemy_player_cells()[i], myCell) < minDist)
		{
			minDist = dist(session->ennemy_player_cells()[i], myCell);
			index = i;
		}
	}
	return index;
}

vector<NeutralCell> get_edible_neutrals_in_range(const TurnPlayerCell & myCell){
	vector<NeutralCell> r; 
	for (const NeutralCell & cell : session->neutral_cells())
	{
		// ignore already targeted neutral cell 
		if (*(targeted_neutral_cells.lower_bound(cell.id))==cell.id) continue;
		// ignore if out of range
		if (dist(myCell.position, cell.position) > max(radius(myCell),radius(cell))) continue;
		//else
		r.push_back(cell);
	}
	return r;
}

// va dans la direction opposée
void goToOppositeDirectionFromDangerousCell(int ennemiIndex, const TurnPlayerCell & myCell)
{
	// Action 1: let us try to move cell (id=73) to position (x=42, y=4242)
	actions->add_move_action(myCell.pcell_id, session->ennemy_player_cells()[ennemiIndex].position.x*(-1), 
									   session->ennemy_player_cells()[ennemiIndex].position.y*(-1));
}

Position select_safe_move(const TurnPlayerCell arg, const Position & target){
	return target;
}


