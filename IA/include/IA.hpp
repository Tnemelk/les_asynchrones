#include <cassert>
#include <iostream>
#include <stdio.h>
#include <set>
#include <algorithm>
#include <cmath>
#include <ainetlib16.hpp>

using namespace std;
using namespace ainet16;

static string address;
static string port_str;
static int port;
static string name;
static Actions * actions;
static GameParameters * p;
static Session * session;
static Welcome * welcome;
static set<int> played_cells;

void play_turn();

float dist(float x1, float x2, float y1, float y2);
float cellDistance(const TurnPlayerCell & C1, const TurnPlayerCell & C2);
float radius(const TurnPlayerCell & C);

float moveDistance(const TurnPlayerCell & cell);
bool dangerEnnemyCell(const TurnPlayerCell & ennemyCell, const TurnPlayerCell & myCell);
bool canEatEnnemyCell(const TurnPlayerCell & ennemyCell, const TurnPlayerCell & myCell);
vector<int> isInDangerFromAnotherCell(const TurnPlayerCell & myCell);

int getNearestDangerousCell(vector<int> dangerousCells, const TurnPlayerCell & myCell);
void goToOppositeDirectionFromDangerousCell(int ennemiIndex, const TurnPlayerCell & myCell);
bool targetIsSafe(const TurnPlayerCell & cell, const Position & pos);

//~ struct Position
//~ {
	//~ float x; //! L'abscisse, comprise dans [0,map_width[
	//~ float y; //! L'ordonnée, comprise dans [0,map_height[
//~ };
//~ 
//~ struct MoveAction
//~ {
	//~ int pcell_id;
	//~ Position position;
//~ };
//~ 
//~ struct DivideAction
//~ {
	//~ int pcell_id;
	//~ Position position;
	//~ float mass;
//~ };
//~ 
//~ struct CreateVirusAction
//~ {
	//~ int pcell_id;
	//~ Position position;
//~ };



