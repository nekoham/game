#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Network.hpp>
#include "Menu.h"
#include <math.h>
#include <vector>
#include <string>

int main()
{
	//Declear variables
	srand(time(NULL));
	int shootTimer = 0;
	int enemyTimer = 0;
	int enemyCounter = 0;
	int itemTimer = 0;
	int eventTimer = 0;
	int bulletTimer = 20;
	int score = 0;
	int lightingState = 0;
	int bulletDMG = 2;
	int bulletStatus = 0;
	int gameStatus = 0;
	float pie = 3.14159265359;
	Vector2f position;

	//Window
	RenderWindow window(VideoMode(1600, 900), "test", Style::Default);
	window.setFramerateLimit(60);

	//Menu
	Menu menu(window.getSize().x, window.getSize().y);

	//Font
	Font fontIG;

	if (!fontIG.loadFromFile("Font/Verdana.ttf"))
	{
		printf("FAILED TO LOAD FONT");
	}

	//Background
	Texture backgroundTex;
	Sprite background;

	backgroundTex.loadFromFile("Texture/background1.png");
	background.setTexture(backgroundTex);
	background.setScale(1.f, 1.f);

	//Knight
	Texture knightTex;
	Sprite knight;
	int knightHp = 10;
	int knightArmor = 5;

	knightTex.loadFromFile("Texture/knight.png");
	knight.setTexture(knightTex);
	knight.setScale(Vector2f(0.4f, 0.4f));
	knight.setOrigin(Vector2f(knight.getTexture()->getSize().x / 2, knight.getTexture()->getSize().y / 2));
	knight.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y / 2));

	//Enemy
	struct Enemy
	{
		Texture enemyTex;
		Sprite enemy;
		Vector2f enemyVelo;
		Vector2f enemyCenter;
		Vector2f enemyDir;
		Vector2f enemyDirNorm;
		int hp = 5;
		float rotation_enemy;
	};

	Enemy badguy;
	badguy.enemyTex.loadFromFile("Texture/enemy.png");
	badguy.enemy.setTexture(badguy.enemyTex);
	badguy.enemy.setScale(Vector2f(0.4f, 0.4f));
	badguy.enemy.setOrigin(Vector2f(badguy.enemy.getTexture()->getSize().x / 2, badguy.enemy.getTexture()->getSize().y / 2));

	vector<Enemy>enemies;

	//Bullet
	struct Bullet
	{
		CircleShape shape;
		Vector2f bulletVelo;
	};

	Bullet projectile;
	projectile.shape.setRadius(10.f);
	projectile.shape.setFillColor(Color(255, 215, 0));
	projectile.shape.setOutlineThickness(3.f);
	projectile.shape.setOutlineColor(Color::Black);
	projectile.shape.setOrigin(Vector2f(projectile.shape.getRadius() / 2, projectile.shape.getRadius() / 2));

	vector<Bullet>bullets;

	Bullet projectileDirection_1;
	projectileDirection_1.shape.setRadius(10.f);
	projectileDirection_1.shape.setFillColor(Color(255, 215, 0));
	projectileDirection_1.shape.setOutlineThickness(3.f);
	projectileDirection_1.shape.setOutlineColor(Color::Black);
	projectileDirection_1.shape.setOrigin(Vector2f(projectile.shape.getRadius() / 2, projectile.shape.getRadius() / 2));

	Bullet projectileDirection_2;
	projectileDirection_2.shape.setRadius(10.f);
	projectileDirection_2.shape.setFillColor(Color(255, 215, 0));
	projectileDirection_2.shape.setOutlineThickness(3.f);
	projectileDirection_2.shape.setOutlineColor(Color::Black);
	projectileDirection_2.shape.setOrigin(Vector2f(projectile.shape.getRadius() / 2, projectile.shape.getRadius() / 2));

	//HP Bar
	RectangleShape HpBar;
	HpBar.setFillColor(Color::Red);
	HpBar.setOutlineThickness(10);
	HpBar.setOutlineColor(Color::Black);
	HpBar.setPosition(Vector2f(1050, 800));

	//Armor Bar
	RectangleShape ArmorBar;
	ArmorBar.setFillColor(Color(192, 192, 192));
	ArmorBar.setOutlineThickness(10);
	ArmorBar.setOutlineColor(Color::Black);
	ArmorBar.setPosition(Vector2f(1050, 725));

	//HP Potion
	CircleShape hpPotion;
	hpPotion.setRadius(20);
	hpPotion.setOutlineThickness(3);
	hpPotion.setOutlineColor(Color::Black);
	hpPotion.setFillColor(Color::Red);
	int hpPlus = 2;

	vector<CircleShape>lifeExtenders;

	//Armor Potion
	CircleShape armorPotion;
	armorPotion.setRadius(20);
	armorPotion.setOutlineThickness(3);
	armorPotion.setOutlineColor(Color::Black);
	armorPotion.setFillColor(Color(192, 192, 192));
	int armorPlus = 1;

	vector<CircleShape>armorExtenders;

	RectangleShape speedBullet; //Speed
	speedBullet.setSize(Vector2f(10.f, 10.f));
	speedBullet.setFillColor(Color::Cyan);
	speedBullet.setPosition(Vector2f(300.f, 100.f));

	RectangleShape sizeBullet; //Speed
	sizeBullet.setSize(Vector2f(10.f, 10.f));
	sizeBullet.setFillColor(Color::Red);
	sizeBullet.setPosition(Vector2f(300.f, 100.f));

	vector<RectangleShape>speedBuff;
	vector<RectangleShape>sizeBuff;

	//Score
	string output = "SCORE : " + to_string(score);

	Text scoreIG;
	scoreIG.setFont(fontIG);
	scoreIG.setCharacterSize(72);
	scoreIG.setString(output);

	//Lighting
	CircleShape preLighting;
	preLighting.setRadius(200);
	preLighting.setFillColor(Color(255,0,0, 80));
	preLighting.setOrigin(Vector2f(preLighting.getRadius() , preLighting.getRadius() ));

	CircleShape countdownLighting;
	countdownLighting.setRadius(0);
	countdownLighting.setFillColor(Color(255,0,0, 150));
	countdownLighting.setOrigin(Vector2f(countdownLighting.getRadius() , countdownLighting.getRadius() ));

	//Program start running
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;

			case Event::MouseButtonReleased:
				switch (event.mouseButton.button)
				{
				case Mouse::Left:
					if (menu.menu[1].getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))))
					{
						gameStatus = 1;
					}
					if (menu.menu[3].getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))))
					{
						window.close();
					}
					break;
				default:
					break;
				}

			default:
				break;
			}

		}

		//Menu
		if (gameStatus == 0)
		{
			for (int i = 1; i < max_menu_choice; i++)
			{
				if (menu.menu[i].getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))))
				{
					menu.menu[i].setFillColor(Color::Red);
					menu.menu[i].setCharacterSize(75);
					menu.menu[i].setPosition(Vector2f((window.getSize().x / 2) - menu.menu[i].getGlobalBounds().width / 2, window.getSize().y / (max_menu_choice + 1) * (i + 1)));
				}
				else 
				{ 
					menu.menu[i].setFillColor(Color::Blue); 
					menu.menu[i].setCharacterSize(60);
					menu.menu[i].setPosition(Vector2f((window.getSize().x / 2) - menu.menu[i].getGlobalBounds().width / 2, window.getSize().y / (max_menu_choice + 1) * (i + 1)));
				}
			}
		}

		//Game
		else if (gameStatus == 1)
		{
			//Update knight -> Direction and rotating
			Vector2f knightCenter = Vector2f(knight.getPosition().x - 10, knight.getPosition().y - 10);
			Vector2f mousePos = Vector2f(Mouse::getPosition(window));
			Vector2f aimDir = mousePos - knightCenter;
			Vector2f aimDirNorm = aimDir / float(sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)));

			float dx_knight = mousePos.x - knight.getPosition().x;
			float dy_knight = mousePos.y - knight.getPosition().y;
			float rotation_knight = (atan2(dy_knight, dx_knight)) * 180 / pie;
			knight.setRotation(rotation_knight + 90);

			//Update control
			if (Keyboard::isKeyPressed(Keyboard::A) && knight.getPosition().x - +knight.getGlobalBounds().width / 2 > 0)
			{
				knight.move(-8.f, 0.f);
			}
			if (Keyboard::isKeyPressed(Keyboard::D) && knight.getPosition().x + knight.getGlobalBounds().width / 2 < window.getSize().x)
			{
				knight.move(8.f, 0.f);
			}
			if (Keyboard::isKeyPressed(Keyboard::W) && knight.getPosition().y - knight.getGlobalBounds().height / 2 > 0)
			{
				knight.move(0.0f, -8.f);
			}
			if (Keyboard::isKeyPressed(Keyboard::S) && knight.getPosition().y + knight.getGlobalBounds().height / 2 < window.getSize().y)
			{
				knight.move(0.0f, 8.f);
			}

			//Update enemy
			if (enemyTimer < 20 && enemyCounter < 3)
			{
				enemyTimer += 1;
			}

			if (enemyTimer >= 20) // spawn enemy
			{
				badguy.enemy.setPosition(Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
				enemies.push_back(Enemy(badguy));
				enemyTimer = 0;
				enemyCounter += 1;
			}

			for (size_t j = 0; j < enemies.size(); j++)
			{
				if (enemies[j].hp <= 0) // enemy delete if dies
				{
					enemies.erase(enemies.begin() + j);
					enemyCounter -= 1;
					score += 5;
					break;
				}

				if (enemies[j].enemy.getGlobalBounds().intersects(knight.getGlobalBounds())) // enemy delete if attack(reach) knight
				{
					enemies.erase(enemies.begin() + j);
					if (knightArmor > 0) { knightArmor -= 1; }
					else { knightHp -= 1; }
					enemyCounter -= 1;
					break;
				}

				// rotating enemy
				float dx_enemy = knight.getPosition().x - enemies[j].enemy.getPosition().x;
				float dy_enemy = knight.getPosition().y - enemies[j].enemy.getPosition().y;
				enemies[j].rotation_enemy = (atan2(dy_enemy, dx_enemy)) * 180 / pie;
				enemies[j].enemy.setRotation(enemies[j].rotation_enemy + 90);

				//moving enemy
				enemies[j].enemyCenter = Vector2f(enemies[j].enemy.getPosition().x - 10, enemies[j].enemy.getPosition().y - 10);
				enemies[j].enemyDir = knightCenter - enemies[j].enemyCenter;
				enemies[j].enemyDirNorm = enemies[j].enemyDir / float(sqrt(pow(enemies[j].enemyDir.x, 2) + pow(enemies[j].enemyDir.y, 2)));
				enemies[j].enemyVelo = enemies[j].enemyDirNorm * 2.f;
				enemies[j].enemy.move(enemies[j].enemyVelo);

				//prevent enemies from overlapping


			}

			//Update shooting
			if (shootTimer < bulletTimer)
			{
				shootTimer += 1;
			}

			if (Mouse::isButtonPressed(Mouse::Left) && shootTimer >= bulletTimer) // spawn bullet
			{
				projectile.shape.setPosition(knightCenter);
				projectile.bulletVelo = aimDirNorm * 20.f;
				bullets.push_back(Bullet(projectile));

				shootTimer = 0;
			}

			for (size_t i = 0; i < bullets.size(); i++)  //Collision and movement bullet
			{
				bullets[i].shape.move(bullets[i].bulletVelo);

				if (bullets[i].shape.getPosition().y + 500 < 0 || bullets[i].shape.getPosition().y - 500 > window.getSize().y - bullets[i].shape.getRadius() || bullets[i].shape.getPosition().x + 500 < 0 || bullets[i].shape.getPosition().x - 500 > window.getSize().x - bullets[i].shape.getRadius())
				{
					bullets.erase(bullets.begin() + i);
					break;
				}

				for (size_t j = 0; j < enemies.size(); j++)
				{
					if (bullets[i].shape.getGlobalBounds().intersects(enemies[j].enemy.getGlobalBounds()))
					{
						enemies[j].hp -= bulletDMG;
						bullets.erase(bullets.begin() + i);
						break;
					}
				}
			}

			//Update Hp Armor Bar
			HpBar.setSize(Vector2f(50 * knightHp, 50));
			ArmorBar.setSize(Vector2f(50 * knightArmor, 50));

			//Bullet Status
			switch (bulletStatus)
			{
			case 0: //normal
				bulletTimer = 20;
				break;
			case 1: //speed buff
				bulletTimer = 8;
				break;
			case 2: //dmg size buff
				projectile.shape.setRadius(15.f);
				projectile.shape.setFillColor(Color(170, 108, 57));
				bulletDMG = 3;
				break;
			default:
				bulletTimer = 20;
				break;
			}

			//Update items
			if (itemTimer < 200)
			{
				itemTimer += 1;
			}

			if (itemTimer >= 200)
			{
				int itemChoice = rand() % 4; // 0 = hp pot spawn. 1 = armor pot spawn.
				switch (itemChoice)
				{
				case 0:
					hpPotion.setPosition(Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
					lifeExtenders.push_back(CircleShape(hpPotion));
					itemTimer = 0;
					break;

				case 1:
					armorPotion.setPosition(Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
					armorExtenders.push_back(CircleShape(armorPotion));
					itemTimer = 0;
					break;
				case 2:
					speedBullet.setPosition(Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
					speedBuff.push_back(RectangleShape(speedBullet));
					itemTimer = 0;
					break;
				case 3:
					sizeBullet.setPosition(Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
					sizeBuff.push_back(RectangleShape(sizeBullet));
					itemTimer = 0;
					break;
				default:
					break;
				}
			}

			for (size_t i = 0; i < lifeExtenders.size(); i++)
			{
				if (lifeExtenders[i].getGlobalBounds().intersects(knight.getGlobalBounds()))
				{
					if (knightHp + hpPlus > 10) { knightHp = 10; }
					else { knightHp += hpPlus; }
					lifeExtenders.erase(lifeExtenders.begin() + i);
					break;
				}
			}

			for (size_t i = 0; i < armorExtenders.size(); i++)
			{
				if (armorExtenders[i].getGlobalBounds().intersects(knight.getGlobalBounds()))
				{
					if (knightArmor + armorPlus > 10) { knightArmor = 10; }
					else { knightArmor += armorPlus; }
					armorExtenders.erase(armorExtenders.begin() + i);
					break;
				}
			}

			for (size_t i = 0; i < speedBuff.size(); i++)
			{
				if (speedBuff[i].getGlobalBounds().intersects(knight.getGlobalBounds()))
				{
					bulletStatus = 1;
					speedBuff.erase(speedBuff.begin() + i);
				}
			}

			for (size_t i = 0; i < sizeBuff.size(); i++)
			{
				if (sizeBuff[i].getGlobalBounds().intersects(knight.getGlobalBounds()))
				{
					bulletStatus = 2;
					sizeBuff.erase(sizeBuff.begin() + i);
				}
			}

			//Update event in game (Lighting Strike)
			if (eventTimer < 300)
			{
				eventTimer += 1;
				position = Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100);
			}

			if (eventTimer >= 300)
			{
				lightingState = 1;
				preLighting.setPosition(position);
				countdownLighting.setPosition(position);
				countdownLighting.setRadius(countdownLighting.getRadius() + 1);
				countdownLighting.setOrigin(Vector2f(countdownLighting.getRadius(), countdownLighting.getRadius()));

				if (countdownLighting.getRadius() == preLighting.getRadius())
				{
					if (preLighting.getGlobalBounds().intersects(knight.getGlobalBounds()))
					{
						knightArmor = 0;
						knightHp -= 5;
					}
					lightingState = 0;
					countdownLighting.setRadius(0);
					eventTimer = 0;
				}
			}

			//Update score
			output = "SCORE : " + to_string(score);
			scoreIG.setString(output);

			//Update Game Status
			if (knightHp <= 0)
			{
				gameStatus = 0;
			}
		}

		//Clear window to draw a new one
		window.clear();

		//Draw window
		if (gameStatus == 0)
		{
			menu.draw(window);
		}
		else if (gameStatus == 1)
		{
			window.draw(background);

			if (lightingState == 1)
			{
				window.draw(preLighting);
				window.draw(countdownLighting);
			}

			for (size_t i = 0; i < lifeExtenders.size(); i++)
			{
				window.draw(lifeExtenders[i]);
			}

			for (size_t i = 0; i < armorExtenders.size(); i++)
			{
				window.draw(armorExtenders[i]);
			}

			for (size_t i = 0; i < speedBuff.size(); i++)
			{
				window.draw(speedBuff[i]);
			}

			for (size_t i = 0; i < sizeBuff.size(); i++)
			{
				window.draw(sizeBuff[i]);
			}

			for (size_t i = 0; i < bullets.size(); i++)
			{
				window.draw(bullets[i].shape);
			}

			window.draw(knight);

			for (size_t i = 0; i < enemies.size(); i++)
			{
				window.draw(enemies[i].enemy);
			}

			window.draw(HpBar);

			window.draw(ArmorBar);

			window.draw(scoreIG);

		}

		window.display();

	}
	return 0;
}
