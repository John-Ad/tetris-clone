#pragma once
#include <cstdlib>
#include <vector>
#include <string>
#include <string.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <ctime>
#include <time.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;

class game_engine
{
private:
	//window variables
	RenderWindow* window;
	VideoMode vm;
	Event ev;

	//map variables
	float grid_x[25];
	float grid_y[25];
	RectangleShape walls[3];

	//shape variables
	vector<RectangleShape>objects;
	RectangleShape shapes[4];

	int orientation = 0;		//this states the orientation the shape is currently in

	bool shps[4] =
	{
					false,
					false,
					false,
					false
	};

	int objects_x[1000];		//divisions of 4 to store x and y values of all objects
	int objects_y[1000];
	int objects_counter = 0;

	int shape_1[8] =		// 4 x values followed by 4 y values
	{						//box shape
				8,
				9,
				8,
				9,

				0,
				0,
				1,
				1,
	};
	int shape_2[8] = 
	{						//T shape
				8,
				9,
				9,
				10,

				1,
				0,
				1,
				1
	};
	int shape_3[8] =
	{						//pole shape
				8,
				9,
				10,
				11,

				1,
				1,
				1,
				1
	};
	int shape_4[8] =
	{						// L shape
					9,
					9,
					9,
					10,

					0,
					1,
					2,
					2,
	};

	int timer = 0;				//used to help slow downwards movement of shape, while maintaining responsive x-axis movement
	int floor_timer = 0;		//used to help with movement when a shape reaches the floor
	int shape_choice;
	int holder[8] = { 0,0,0,0,0,0,0,0 };

	bool moving = false;

	//score variables
	Font font;
	Text score;
	int score_counter = 0;

	//kewboard variables
	bool A = false;
	bool D = false;

	//init functions
	void initWindow();
	void initMap();
	void initScore();

	//shape choosing and movement functions
	void choose_shape(int e);
	void randomShape();
	void moveShape();
	void quick_move();

	//shape collisions:
	void lateral_shape_collision(int change);
	void vertical_shape_collision();

	//shape orientation
	void changeOrientation(int shp);

	//line removal
	void removeLine();
	void moveDown(int line);

	//shortening of array
	void shortenArray(int hold[], int line);

	//score function
	void updateScore();

	//render functions
	void renderMap();
	void renderShape();
	void renderObjects();
	void renderScore();
public:
	game_engine();
	virtual ~game_engine();

	bool running();
	void pollevents();
	void update();
	void render();
};

