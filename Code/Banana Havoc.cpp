// FINAL PROJECT: Tower Defense Game, "Banana Havoc," programmed by Michael Bartlett
//This is a mostly-automated video game which challenges the player to upgrade their tower to overcome each wave of enemies.
//The player is represented by a tower which automatically defends itself, so the player just has to sit back and watch.
//Enemies are reprsented as bananas, and the tower shoots lasers at the closest enemy until it dies, and so forth until all enemies are killed.
//Upon victory the player can select an upgrade for their tower to combat the increasing health and speed of the enemies.

//* Preprocessing directives
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")	//This line prevents the command line from showing (since it isn't used directly)
//*

//* Libraries
#include <SFML/Graphics.hpp>    //For graphical display.
#include <SFML/Audio.hpp>       //For audio output.
#include <iostream>             //For console input/output.
#include <fstream>              //For loading/saving user's progress in the form of a file.
#include <math.h>               //For more advanced math operations other than standard operations.
#include <vector>               //For the vector data-structure.
#include <time.h>               //For reading time, used to seed random number generators.
#include "tower.h"              //Self-made object used for the "tower" object which represents the player.
#include "baddy.h"              //Self-made object, for enemies that assault the player's tower.
#include "laz.h"                //Self-made object, for the laser that the tower uses as a weapon.
#include "save.h"               //Methods for saving/loading player's save file concisely.
#include "ico.c"                //Used for the the icon in the corner of the graphical window.
//*

//* Namespaces, to prevent redundancy in code.
using namespace std; //Simplifies calls to standard library functions and objects.
using namespace sf;  //Simplifies calls to SFML (Simple and Fast Multimedia Library) functions and objects.
//*

//* Global variables/constants
const string FONT_PATH = "src/sprites/swgothe.ttf";	//Directory for font used for text display.
Font font;          //SFML font object (where the font is loaded into memory.)
Music bgm;          //SFML music object for background music.
Texture texture;    //SFML texture object to load image files for displaying.
Sprite sprite;      //SFML sprite object, which is what is actually displayed each frame.
//*


//Beginning of direct game processes.
int main() {
	
	//* Objects/variables used regardless of game-state (playing, win, or lose)
	if (!font.loadFromFile(FONT_PATH)) { cerr << "Can't find font"; return 0; }                     //Returns an error if font-file can't be found.
	if (!bgm.openFromFile("src/sound/bg.wav")) { cerr << "No background music found"; return 0; }   //Returns an error if background music file can't be found.
	bgm.setVolume(10);                                                                              //Sets music to reasonable volume.
	bgm.setPlayingOffset(Time(seconds(1)));                                                         //Offsets music file to create a better illusion of looping music.
	bgm.play();                                                                                     //Begins streaming of music.
	bool paused = false;                                                                            //Used to pause game if the user unfocuses on game window.
	//*

	
	while (true) {  //Loop which enables the playing of subsequent levels without running another instance of the program.
		
		//* Variables used to break loop on loss or victory.
		bool win = false;
		int brake = 0;    //"break" is a keyword and would make a call to break ambiguous if this variable were named such.
		//*

		//* Creates health-bar object for visual feedback of tower-damage.
		RectangleShape max, bar, back;		//Health bar is composed of three rectangles: max & current health, and an outline.

		max.setFillColor(Color(100, 0, 0)); bar.setFillColor(Color::Red); back.setFillColor(Color(175, 175, 175));  //Sets their respective colors.
		max.setSize(Vector2f(200, 20)); bar.setSize(Vector2f(200, 20)); back.setSize(Vector2f(210, 30));            //Sets their respecitve size.
		max.setPosition(125, 675); bar.setPosition(125, 675); back.setPosition(120, 670);                           //Sets their respective position.
		//*

		//* The window where all objects will be displayed in
		RenderWindow window(sf::VideoMode(750, 750), "Banana Havoc");       //Creates dimensions and title for window.
		window.setFramerateLimit(1000);                                     //Caps FPS for above-average processors animating too quickly.
		window.setIcon(banicon.width, banicon.height, banicon.pixel_data);  //Where "ico.c" is used.
		//*

		//* Instantiates all self-made objects for use.
		vector<baddy> bad(0);   //A vector of baddies for our population of enemies
		laz lazr;               //A laser object for the tower to defend itself
		tower tow;              //A tower object for enemies to attack
		int level;              //A variable to hold the current level that the player is on.
		//*

		//The total amount of upgrades are loaded here, and if the user won the last game it prompts another upgrade (from "save.h").
		load(level, tow.healthup, lazr.laserup, lazr.coolup, win); 
		//Three aspects of the tower can be upgraded:
		//      Laser power (how much health an enemy loses per hit)
		//      Total health (how many times the tower can take damage)
		//      Laser cool down (how many frames the program waits before firing the laser again.)
		

		//* Sets background image based on level, and returns an error if the background image can't be found.
		if ((level - 9) / 10 <  1) { if (!texture.loadFromFile("src/sprites/bggrass.png")) { cerr << "No background"; return 0; } }
		if ((level - 9) / 10 == 1) { if (!texture.loadFromFile("src/sprites/bgwater.png")) { cerr << "No background"; return 0; } }
		if ((level - 9) / 10 == 2) { if (!texture.loadFromFile("src/sprites/bgmagma.png")) { cerr << "No background"; return 0; } }
		if ((level - 9) / 10 == 3) { if (!texture.loadFromFile("src/sprites/bgpenta.png")) { cerr << "No background"; return 0; } }
		if ((level - 9) / 10 == 4) { if (!texture.loadFromFile("src/sprites/bgtripy.png")) { cerr << "No background"; return 0; } }
		if ((level - 9) / 10 >= 5) { if (!texture.loadFromFile("src/sprites/bgspace.png")) { cerr << "No background"; return 0; } }
		sprite.setTexture(texture);
		sprite.setOrigin(Vector2f(534, 534));       //Sets origin to center of the image so it rotates about its center.
		sprite.setPosition(Vector2f(374, 405));     //Sets the image position to a visible area of the window.
		//*

		//**// Displays dialog for upgrading the tower if the player beat the last level.
		while (win == true) {
			window.clear();  //Clears window for a fresh palette.
			
			//* Ensures music still plays correctly while locked in this loop.
			if (bgm.getPlayingOffset().asSeconds() >= 8.72) { 
				bgm.setPlayingOffset(Time(seconds(0.05)));      //Loops background music more precisely than the file itself does.
			}
			if (bgm.getStatus() == 0) { bgm.play(); }           //Resumes music stream if it gets stopped for some reason (i.e. gameover).
			//*

			//** Displays readable text, prompting the user for input of the upgrade they desire, or if they wish to exit.
			string str = "UPGRADE";
			//Creates the Text object, sets its string, color, position, font size, and font.
			Text upgr; upgr.setString(str);	upgr.setColor(Color::Yellow); upgr.setPosition(165, 30);	upgr.setCharacterSize(70); upgr.setFont(font);
			window.draw(upgr);

			str = "Press \"H\" to upgrade the tower's health";
			upgr.setString(str); upgr.setColor(Color(0, 100, 255)); upgr.setPosition(85, 150); upgr.setCharacterSize(25); upgr.setFont(font);
			window.draw(upgr);

			str = "Press \"L\" to upgrade the laser's strength";
			upgr.setString(str); upgr.setColor(Color(0, 100, 255)); upgr.setPosition(80, 250);
			window.draw(upgr);
			

			//* Removes instruction to upgrade laser cooldown if it is at the maximum upgrade value.
			if (lazr.coolup < 480) {
				str = "Press \"C\" to reduce the cooldown of the laser";
				upgr.setString(str); upgr.setColor(Color(0, 100, 255)); upgr.setPosition(60, 350);
				window.draw(upgr);
			}
			//This check is necessary as a cooldown of zero frames ends the game almost instantaneously (one laser per frame is very fast).
			//* 

			str = "Press \"N\" to continue without an upgrade";
			upgr.setString(str); upgr.setColor(Color(0, 100, 255)); upgr.setPosition(75, 450);
			window.draw(upgr);

			str = "Press \"Esc\" to quit";
			upgr.setString(str); upgr.setColor(Color(255, 0, 100)); upgr.setPosition(250, 550);
			window.draw(upgr);

			window.display();
			//**

			//* Checks if a relevant key has been pressed, and performs the respective action.
			Event evn;
			while (window.pollEvent(evn)) {
				if (evn.type == Event::Closed) { window.close(); return 0; }
				if (evn.type == sf::Event::KeyPressed) {
					if (evn.key.code == sf::Keyboard::H) { tow.healthup++; win = false; }  //'H' for max health upgrade.
					if (evn.key.code == sf::Keyboard::L) { lazr.laserup++; win = false; }  //'L' for laser power upgrade.
					if (evn.key.code == sf::Keyboard::C) { 
					     if (lazr.coolup < 480) { lazr.coolup += 20; win = false; }        //"'C' for cooldown upgrade.
					}	
					if (evn.key.code == sf::Keyboard::N) { win = false; }                  //'N' continues to next level with no upgrade.
					if (evn.key.code == sf::Keyboard::Escape) { win = false; window.close(); return 0; }  //Properly exits program.
				}
			}
			//*
		}
		//**//

		//* Applies upgrades from load and upgrade prompt (if one was selected).
		tow.maxhealth += tow.healthup; tow.health = tow.maxhealth;   //Applies health upgrade.
		lazr.strength += lazr.laserup; lazr.cool -= lazr.coolup;     //Applies laser strength and cooldown upgrades.
		
		tow.sprit();  //Applies respective sprite based on tower health upgrade (tower looks stronger with larger max health.)
		//*

		//* Creates population of enemies based on level progression (number of level is the quantity of enemies).
		for (int i = 0; i < level; i++)	{
			if (i >= bad.size()) {
				baddy w(level);
				bad.push_back(w);
			}
			bad[i].inpos(i);
			bad[i].speed = 2.0 * double(level) / 500.0;  //Enemies get faster each level.
		}
		//*

		int badc = 0, frame = 0;  //Used as counters of enemies killed and frames processed respectively.


		//* Establishes text objects of upgrade, level, and enemy info to be drawn later during the game itself.
		string str = "Level " + to_string(level - 9) + "\nLaser damage: " + to_string(lazr.strength) + "\nCooldown: " + to_string(lazr.cool) + "\n";
		Text levl; levl.setString(str);	levl.setColor(Color::White); levl.setPosition(135, 10);	levl.setCharacterSize(20);	levl.setFont(font);

		Text bananas;
		str = "Bananas: " + to_string(bad.size() - bad[0].ripc());
		bananas.setString(str);	bananas.setColor(Color::Yellow); bananas.setPosition(450, 10); bananas.setCharacterSize(20); bananas.setFont(font);
		//*

		while (window.isOpen()) { //Loop that displays each frame.
			
			//* Extraneous event checking
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed) { window.close(); }                             //Closes process if graphic window is closed.
				if (!window.isOpen()) { return 0; }               //Stops program if somehow closing the window doesn't trigger a close event.
				if (event.type == sf::Event::Resized) { window.setSize(Vector2u(750, 750)); }  //Sets window back to intended size if resized.
				if (event.type == sf::Event::LostFocus) { paused = true; }                     //Pauses if window is unfocused.
				if (event.type == sf::Event::GainedFocus) { paused = false; }                  //Unpauses if window is refocused.
				if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::Escape) { window.close(); return 0; }  //Closes program if esc is pushed.
				}
			}
			//*

			//* Ensures proper looping of music (copied from line 106).
			if (bgm.getPlayingOffset().asSeconds() >= 8.72) { bgm.setPlayingOffset(Time(seconds(0.05))); }
			if (bgm.getStatus() == 0) { bgm.play(); }
			//*
			
			if (paused) { continue; }  //Prevents processing of game events if a pause event is triggered.

			frame++;  //increments frame counter (for laser cooldown calculations).

			//** Draws various objects to the frame.
			window.clear();                                                //Clears the window so no trails appear.
			sprite.setRotation(sprite.getRotation() + bad[0].speed/10.0);  //Rotates the background image for hypnotic effect.
			window.draw(sprite);                                           //Draws the background to the frame.
			window.draw(tow.sprite);                                       //Draws the respective tower's sprite to the frame.
			window.draw(levl);                                             //Draws text showing what level the player is on currnetly.
			
			//* Updates text containing amount of enemies remaining and draws it to the frame.
			str = "Bananas: " + to_string(bad.size() - bad[0].ripc());
			bananas.setString(str);
			window.draw(bananas);
			//*

			//**

			//*** Checks the status of all enemies.
			for (int i = 0; i < bad.size(); i++)	{  //All following commands are run on each enemy in the population.
				
				if (bad[i].dead) { continue; }         //If the enemy is dead then nothing needs to be done to them.
						
				//They way the classes are constructed, this if statement simultaneously updates the enemy's position and checks if a game-over results from it.
				if (tow.hit(bad[i].iter()) == false) {

					//if this branch is entered, that means the tower is out of health and the game is over.

					bad[0].rips(); badc == 0, frame = 0; bad.clear();  //Resets enemy kill counter and clears the population to free memory.
					
					//* Creates "Game Over" text object to show player that they have lost.
					Text text;
					text.setString("Game Over");
					text.setColor(Color::Red);
					text.setPosition(60, 330);
					text.setCharacterSize(100);
					text.setFont(font);
					//*
					
					//** Loop to provide player opportunity to acknowledge defeat.
					while (true) {
						if (!window.isOpen()) { return 0; }  //Closes program if graphic window has been closed.
						
						window.clear();                      //Clears clutter from the window.
						
						window.draw(text);                   //Draws "Game Over" to frame.
						
						//* Moves objects that are displayed regardless of condition out of view.
						bar.setPosition(Vector2f(-1000, -1000)); 
						max.setPosition(Vector2f(-1000, -1000)); 
						bad[0].tots();
						//*

						window.display();  //Actually displays the "Game Over."

						//* Checks for user to push any key to close the window properly.
						Event event1;
						while (window.pollEvent(event1)) {
							if (event1.type == Event::Closed) { window.close(); return 0; }
							if (event1.type == sf::Event::KeyPressed) { window.close(); return 0; }
							else { continue; }
						}
						//*

					}
					//**

				}
				
				//The last "if" was for a game over, this branch is for if the game continues.
				else {
					window.draw(bad[i].sprite);                                  //Draws this specific enemy to the frame.
					while (bad[badc].dead && badc < bad.size() - 1) { badc++; }  //Error checks to make sure the enemies-remaining counter is accurate.

					//* Displays laser beam from tower.
					if (lazr.shoot(bad[badc].func, frame, bad[badc].getx(), bad[badc].gety(), tow.healthup) == true) {
						//All the paramters in this function are to ensure that laser displays accurately over this specific enemy's coordinates.

						bad[badc].hit(lazr.strength, badc); //Calculates the damage from the laser to the enemy.
						window.draw(lazr.lazt);             //Draws the laser to the frame.
					}
					
 					//* Shows the laser for more than 1 frame so the player can actually see it.
					if (frame - lazr.frame <= double(lazr.cool) / 3.0 && frame > lazr.cool &&win == false) { window.draw(lazr.lazt);} 
					//*

					//* Updates the health bar's proportional size to reflect the tower's remaining health.
					window.draw(back); window.draw(max);
					bar.setSize(Vector2f((double((double(tow.health) / double(tow.maxhealth))) * 200.0), 20));
					window.draw(bar);
					window.draw(tow.heal);
					//*
				}
			}
			//***

			window.display();  //Displays everything drawn to the frame during the enemy check loop.

			//Check for if the player won.
			if (bad[0].ripc() >= level) {  //If the number of dead enemies is the same number as the level, the player won.

				win = true;      //Used to write victory to the save file.
				save(level + 1, tow.healthup, lazr.laserup, lazr.coolup, win);  //Saves all the current upgrades and the victory to file.
				
				//Loop allows time for player to acknowledge victory.
				while (brake == 0) {
					if (!window.isOpen()) { return 0; } //If the window is closed, end the program.
					
					//* Clear the window and move constantly displaying objects out of view.
					window.clear(); bar.setPosition(Vector2f(-1000, -1000)); max.setPosition(Vector2f(-1000, -1000));
					//*

					//* Creates and displays text object for victory message and instructions to continue.
					Text text;
					text.setString("You Win!");
					text.setColor(Color::Blue);
					text.setPosition(120, 250);
					text.setCharacterSize(100);
					text.setFont(font);
					window.draw(text);
					text.setString("Press any key to start\n     the next game!\n\n  Press \"Esc\" to quit!");
					text.setColor(Color(0, 100, 255));
					text.setPosition(180, 400);
					text.setCharacterSize(30);
					window.draw(text);
					window.display();
					//*

					//* Checks for user input on victory screen.
					Event event2;
					while (window.pollEvent(event2)) {
						if (event2.type == Event::Closed) { window.close(); return 0; }        //Ends program if graphics window is closed.
						if (event2.type == sf::Event::KeyPressed) {	
							if (event2.key.code == sf::Keyboard::Escape) { window.close(); return 0; }  //Closes program if esc is pressed.
							
							else { brake = 1; bad[0].rips(); bad[0].tots(); }   //If any other key is pressed, continues to the next level.
						}
						else { continue; }
					}
					//*
				}
				
				if (brake = 1) { brake = 0; break; }  //This check is used to end the loop if the user decided to continue to the next level.
			}
		}
	}
}