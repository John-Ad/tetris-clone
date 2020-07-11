#include "game_engine.h"

//init functions
void game_engine::initWindow()
{
	vm.height = 500;
	vm.width = 500;

	window = new RenderWindow(vm, "tetris", Style::Titlebar | Style::Close);
	window->setFramerateLimit(60);

	float count = 0;
	for (int i = 0; i < 25; i++)
	{
		grid_x[i] = count;
		grid_y[i] = count;
		count = count + 20;
	}
}

void game_engine::initMap()
{
	for (int i=0;i<3;i++)
	{
		walls[i].setFillColor(Color(0,0,50,255));
	}

	walls[0].setSize(Vector2f(20, vm.height));
	walls[1].setSize(Vector2f(20, vm.height));
	walls[2].setSize(Vector2f(240, 20));

	walls[0].setPosition(grid_x[6], grid_y[0]);
	walls[1].setPosition(grid_x[18], grid_y[0]);
	walls[2].setPosition(grid_x[7], grid_y[24]);
}

void game_engine::initScore()
{
	font.loadFromFile("C://Users//adria//source//repos//tetris//tetris//VINERITC.ttf");
	score.setFont(font);
	score.setCharacterSize(20);
	score.setPosition(420, 20);
	score.setString("0");
}

//shape choosing and movement
void game_engine::choose_shape(int e)
{
	orientation = 0;
	for (int i = 0; i < 4; i++)
	{
		shps[i] = false;
	}
	switch (e)
	{
	case 0:
		shps[0] = true;
		for (int i = 0; i < 4; i++)
		{
			shapes[i].setFillColor(Color::Red);
			shapes[i].setSize(Vector2f(20, 20));
			shapes[i].setOutlineColor(Color::Black);
			shapes[i].setOutlineThickness(1);
			shapes[i].setPosition(grid_x[shape_1[i]], grid_y[shape_1[i + 4]]);
		}
		for (int i = 0; i < 8; i++)
		{
			holder[i] = shape_1[i];
		}
		break;
	case 1:
		shps[1] = true;
		for (int i = 0; i < 4; i++)
		{
			shapes[i].setFillColor(Color::Green);
			shapes[i].setSize(Vector2f(20, 20));
			shapes[i].setOutlineColor(Color::Black);
			shapes[i].setOutlineThickness(1);
			shapes[i].setPosition(grid_x[shape_2[i]], grid_y[shape_2[i + 4]]);
		}
		for (int i = 0; i < 8; i++)
		{
			holder[i] = shape_2[i];
		}
		break;
	case 2:
		shps[2] = true;
		for (int i = 0; i < 4; i++)
		{
			shapes[i].setFillColor(Color::Cyan);
			shapes[i].setSize(Vector2f(20, 20));
			shapes[i].setOutlineColor(Color::Black);
			shapes[i].setOutlineThickness(1);
			shapes[i].setPosition(grid_x[shape_3[i]], grid_y[shape_3[i + 4]]);
		}
		for (int i = 0; i < 8; i++)
		{
			holder[i] = shape_3[i];
		}
		break;
	case 3:
		shps[3] = true;
		for (int i = 0; i < 4; i++)
		{
			shapes[i].setFillColor(Color::White);
			shapes[i].setSize(Vector2f(20, 20));
			shapes[i].setOutlineColor(Color::Black);
			shapes[i].setOutlineThickness(1);
			shapes[i].setPosition(grid_x[shape_4[i]], grid_y[shape_4[i + 4]]);
		}
		for (int i = 0; i < 8; i++)
		{
			holder[i] = shape_4[i];
		}
		break;
	default:
		break;
	}
}

void game_engine::randomShape()
{
	if (moving == false)
	{
		int r_shape = rand() % 4 - 0;
		choose_shape(r_shape);
		moving = true;
	}
}

void game_engine::moveShape()
{
	if (moving == true)
	{
		/*
			TODO: write the collision code for the walls
				  Add an integer array for the vector objects
		*/
		//lateral movement:
		if (A == true)
		{
			for (int i = 0; i < 4; i++)
			{
				holder[i] = holder[i] - 1;
				lateral_shape_collision(0);
				A = false;
			}
		}
		if (D == true)
		{
			for (int i = 0; i < 4; i++)
			{
				holder[i] = holder[i] + 1;
				lateral_shape_collision(1);
				D = false;
			}
		}
		//

		//downwards movement:
		if (floor_timer == 0)
		{
			timer++;
			if (timer == 15)
			{
				for (int i = 0; i < 4; i++)
				{
					holder[i + 4] = holder[i + 4] + 1;
				}
				timer = 0;
			}
		}
		//----------------------------------------------------------------------

		//wall collisions:
		for (int i = 0; i < 4; i++)
		{
			if (holder[i] < 7)
			{
				for (int e = 0; e < 4; e++)
				{
					holder[e] = holder[e] + 1;
				}
			}
			if (holder[i] > 17)
			{
				for (int e = 0; e < 4; e++)
				{
					holder[e] = holder[e] - 1;
				}
			}
		}
		//----------------------------------------

		for (int i = 0; i < 4; i++)
		{
			shapes[i].setPosition(grid_x[holder[i]], grid_y[holder[i + 4]]);
		}

		vertical_shape_collision();
		for(int f = 4;f<8;f++)
		{
			if (holder[f] == 23)
			{
				floor_timer += 1;
				if (floor_timer == 10 )
				{
					moving = false;
					for (int i = 0; i < 4; i++)
					{
						objects.push_back(shapes[i]);
					}
					int ctr = 0;
					for (int i = (objects.size() - 4); i < objects.size(); i++)
					{
						objects_x[i] = holder[ctr];
						objects_y[i] = holder[ctr + 4];
						ctr++;

						cout << objects_x[i] << " " << objects_y[i] << endl;
					}
					cout << endl;

					floor_timer = 0;
				}
				f = 8;
			}
		}
	}	
}

void game_engine::quick_move()
{
	int num = 0;
	bool match = false;
	int hold = 0;
	if (moving == true)
	{
		if (objects.size() > 0)
		{
			for (int e = 0; e < 4; e++)
			{
				for (int i = 0; i < objects.size(); i++)
				{
					if (holder[e] == objects_x[i])
					{
						if (hold == 0)
						{
							hold = objects_y[i];			//stores the position that will be used to retrieve the corresponding value from obj_y
						}
						else
						{
							if (objects_y[i] < hold)
							{
								hold = objects_y[i];
							}
						}
						match = true;
					}
				}
			}
		}
		if (match == true)
		{
			for (int i = 0; i < 3; i++)				//finds lowest part of shape
			{
				if (holder[i + 4] < holder[i + 5])
				{
					num = holder[i + 5];
				}
				else
				{
					num = holder[i + 4];
				}
			}

			for (int i = 0; i < 4; i++)
			{
				holder[i + 4] = holder[i + 4] + (hold - num);				//finds the distance from the shapes lowest point to the highest object about to be collided with
																						//this moves all parts of the shape by that amount
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)				//finds lowest part of shape
			{
				if (holder[i + 4] < holder[i + 5])
				{
					num = holder[i + 5];
				}
				else
				{
					num = holder[i + 4];
				}
			}
			for (int i = 0; i < 4; i++)
			{
				holder[i + 4] = holder[i + 4] + (23 - num);
			}
		}
	}
}

//shape collisions:
void game_engine::lateral_shape_collision(int change)
{
	if (objects.size() > 0)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int e = 0; e < 100; e++)
			{
				if (holder[i] == objects_x[e])
				{
					/*
							TODO: write a statement that checks if the shapes needs to move forwards or backwards
								  when colliding with another shape
					*/
					if (holder[i + 4] == objects_y[e])
					{
						switch (change)
						{
						case 0:
							for (int f = 0; f < 4; f++)
							{
								holder[f] = holder[f] + 1;
								i = 4;
								e = 100;
							}
							break;
						case 1:
							for (int f = 0; f < 4; f++)
							{
								holder[f] = holder[f] - 1;
								i = 4;
								e = 100;
							}
							break;
						}
					}
				}
			}
		}
	}
}
void game_engine::vertical_shape_collision()
{
	if (objects.size() > 0)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int e = 0; e < objects.size(); e++)
			{
				if (holder[i] == objects_x[e])					//it first checks for the x value. if that matches, the corrosponding y values are checked aswell.
																//it should be noted that eg object_x[1] and object_y[1] represent the same object
				{
					if (holder[i + 4] == objects_y[e])
					{
						cout << endl << "x = " << objects_x[0] << " y = " << objects_y[0] << endl;
						moving = false;
						for (int i = 0; i < 4; i++)
						{
							holder[i + 4] = holder[i + 4] - 1;									//moves object up
							shapes[i].setPosition(grid_x[holder[i]], grid_y[holder[i + 4]]);	//sets position of shapes
							objects.push_back(shapes[i]);										//adds the current objects to the vector
						}

						int ctr = 0;
						for (int i = (objects.size() - 4); i < objects.size(); i++)
						{
							objects_x[i] = holder[ctr];					//stores corrosponding x and y values
							objects_y[i] = holder[ctr + 4];
							ctr++;

							cout << objects_x[i] << " " << objects_y[i] << endl;		//used to see position values in console window
						}
						cout << endl;

						e = objects.size();
						i = 4;
					}
				}
			}
		}
	}
}

//shape orientation:
void game_engine::changeOrientation(int shp)
{
	if (moving == true)
	{
		switch (shp)
		{
		case 2:		//T shape
			switch (orientation)
			{
			case 0:
				holder[0] = holder[0] + 1;		//left block change
				holder[4] = holder[4] - 1;

				holder[1] = holder[1] + 1;		//upper block change
				holder[5] = holder[5] + 1;

				holder[3] = holder[3] - 1;		//right block change
				holder[7] = holder[7] + 1;

				orientation ++;
				break;
			case 1:
				holder[0] = holder[0] + 1;		//left block change
				holder[4] = holder[4] + 1;

				holder[1] = holder[1] - 1;		//upper block change
				holder[5] = holder[5] + 1;

				holder[3] = holder[3] - 1;		//right block change
				holder[7] = holder[7] - 1;

				orientation++;
				break;
			case 2:
				holder[0] = holder[0] - 1;		//left block change
				holder[4] = holder[4] + 1;

				holder[1] = holder[1] - 1;		//upper block change
				holder[5] = holder[5] - 1;

				holder[3] = holder[3] + 1;		//right block change
				holder[7] = holder[7] - 1;

				orientation ++;
				break;
			case 3:
				holder[0] = holder[0] - 1;		//left block change
				holder[4] = holder[4] - 1;

				holder[1] = holder[1] + 1;		//upper block change
				holder[5] = holder[5] - 1;

				holder[3] = holder[3] + 1;		//right block change
				holder[7] = holder[7] + 1;

				orientation = 0;
				break;
			}
			break;


		case 3:		//pole shape
			switch (orientation)
			{
			case 0:
				holder[0] = holder[0] + 2;
				holder[4] = holder[4] - 2;

				holder[1] = holder[1] + 1;
				holder[5] = holder[5] - 1;

				holder[3] = holder[3] - 1;
				holder[7] = holder[7] + 1;

				orientation++;
				break;
			case 1:
				holder[0] = holder[0] + 2;
				holder[4] = holder[4] + 2;

				holder[1] = holder[1] + 1;
				holder[5] = holder[5] + 1;

				holder[3] = holder[3] - 1;
				holder[7] = holder[7] - 1;

				orientation++;
				break;
			case 2:
				holder[0] = holder[0] - 2;
				holder[4] = holder[4] + 2;

				holder[1] = holder[1] - 1;
				holder[5] = holder[5] + 1;

				holder[3] = holder[3] + 1;
				holder[7] = holder[7] - 1;

				orientation++;
				break;
			case 3:
				holder[0] = holder[0] - 2;
				holder[4] = holder[4] - 2;

				holder[1] = holder[1] - 1;
				holder[5] = holder[5] - 1;

				holder[3] = holder[3] + 1;
				holder[7] = holder[7] + 1;

				orientation = 0;
				break;
			}
			break;
			//------------------------------------------


		case 4:
			switch (orientation)
			{
			case 0:
				holder[0] = holder[0] + 2;
				holder[4] = holder[4] + 2;

				holder[1] = holder[1] + 1;
				holder[5] = holder[5] + 1;

				holder[3] = holder[3] - 1;
				holder[7] = holder[7] + 1;

				orientation++;
				break;
			case 1:
				holder[0] = holder[0] - 2;
				holder[4] = holder[4] + 2;

				holder[1] = holder[1] - 1;
				holder[5] = holder[5] + 1;

				holder[3] = holder[3] - 1;
				holder[7] = holder[7] - 1;

				orientation++;
				break;
			case 2:
				holder[0] = holder[0] - 2;
				holder[4] = holder[4] - 2;

				holder[1] = holder[1] - 1;
				holder[5] = holder[5] - 1;

				holder[3] = holder[3] + 1;
				holder[7] = holder[7] - 1;

				orientation++;
				break;
			case 3:
				holder[0] = holder[0] + 2;
				holder[4] = holder[4] - 2;

				holder[1] = holder[1] + 1;
				holder[5] = holder[5] - 1;

				holder[3] = holder[3] + 1;
				holder[7] = holder[7] + 1;

				orientation = 0;
				break;
			}
			break;
		}
	}
}

//remove line
void game_engine::removeLine()
{
	if (objects.size() > 10)
	{
		int hold[20];
		int count = 0;
		int line = 23;
		for (int i = 0; i < 25; i++)
		{
			count = 0;

			for (int e = 0; e < objects.size(); e++)
			{
				if (objects_y[e] == line)
				{
					hold[count] = e;
					count++;
				}
			}
			if (count > 10)
			{
				shortenArray(hold, line);
				score_counter += 1;
				for (int f = 0; f < count; f++)
				{
					objects.erase(objects.begin() + hold[f]);		//once an object is removed, the remaining objects infront of the removed object
																		//move down 1 place
					for (int g = 0; g < count; g++)
					{
						hold[g] = hold[g] - 1;
					}
				}
				moveDown(line);
			}
			else
			{
				count = 0;
			}
			line -= 1;
		}
	}
}

void game_engine::moveDown(int line)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects_y[i] < line)
		{
			objects_y[i] = objects_y[i] + 1;
			objects[i].setPosition(grid_x[objects_x[i]], grid_y[objects_y[i]]);
		}
	}
}

//shortening of array
void game_engine::shortenArray(int hold[], int line)
{
	int count = 0;
	int holder = 0;

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects_y[i] == line)
		{
			objects_y[i] = 0;
			objects_x[i] = 0;
			count += 1;									//keeps track of the number of open spaces available
		}
		else                                            //4;4;4;0;0;4;0  use this to test the algorithm (0 is an open space)
		{
			if (count > 0)
			{
				holder = i - count;							// (i - count) gives the first available postion
				objects_x[holder] = objects_x[i];
				objects_y[holder] = objects_y[i];
				objects_y[i] = 0;
				objects_x[i] = 0;
			}
		}
	}

	for (int i = 0; i < objects.size(); i++)
	{
		cout << endl << objects_x[i] << " " << objects_y[i] << endl;
	}
	cout << endl << endl;
}

//score update
void game_engine::updateScore()
{
	stringstream ss;
	ss << score_counter;
	score.setFillColor(Color::White);
	score.setString(ss.str());
}

//render functions
void game_engine::renderMap()
{
	for (auto i : walls)
	{
		window->draw(i);
	}
}

void game_engine::renderShape()
{
	if (moving == true)
	{
		for (auto i : shapes)
		{
			window->draw(i);
		}
	}
}

void game_engine::renderObjects()
{
	if (objects.size() > 0)
	{
		for (auto e : objects)
		{
			window->draw(e);
		}
	}
}

void game_engine::renderScore()
{
	window->draw(score);
}

//constructor and destructor
game_engine::game_engine()
{
	initWindow();
	initMap();
	initScore();
}


game_engine::~game_engine()
{
	delete window;
}


//public functions
bool game_engine::running()
{
	if (window->isOpen())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void game_engine::pollevents()
{
	while (window->pollEvent(ev))
	{
		switch (ev.type)
		{
		case Event::Closed:
			window->close();
			break;
		case Event::KeyPressed:
			switch (ev.key.code)
			{
			case Keyboard::E:
				moving = true;
				choose_shape(0);
				break;
			case Keyboard::R:
				moving = true;
				choose_shape(1);
				break;
			case Keyboard::T:
				moving = true;
				choose_shape(2);
				break;
			case Keyboard::Y:
				moving = true;
				choose_shape(3);
				break;
			case Keyboard::Space:
				quick_move();
				break;
			case Keyboard::A:
				A = true;
				break;
			case Keyboard::D:
				D = true;
				break;
			case Keyboard::W:
				if (shps[1] == true)
				{
					changeOrientation(2);
				}
				if (shps[2] == true)
				{
					changeOrientation(3);
				}
				if (shps[3] == true)
				{
					changeOrientation(4);
				}
				break;
			}
			break;
		/*case Event::KeyReleased:
			switch (ev.key.code)
			{
			case Keyboard::A:
				A = false;
				break;
			case Keyboard::D:
				D = false;
				break;
			}
			break;*/
		}
	}
}

void game_engine::update()
{
	randomShape();
	pollevents();
	moveShape();
	removeLine();
	updateScore();
}

void game_engine::render()
{
	window->clear();

	renderMap();
	renderShape();
	renderObjects();
	renderScore();

	window->display();
}
