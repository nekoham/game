#include "Menu.h"

Menu::Menu(float width, float height)
{
	if (!fontMenu.loadFromFile("Font/Verdana.ttf"))
	{
		printf("FAILED TO LOAD FONT");
	}

	menuBG_Tex.loadFromFile("Texture/menuBG.png");
	menuBG.setTexture(menuBG_Tex);
	menuBG.setScale(1.f, 1.f);

	menu[0].setFont(fontMenu);
	menu[0].setFillColor(Color::Blue);
	menu[0].setOutlineThickness(5.f);
	menu[0].setOutlineColor(Color::Black);
	menu[0].setCharacterSize(90);
	menu[0].setString("Gunner Knight");
	menu[0].setPosition(Vector2f((width / 2) - menu[0].getGlobalBounds().width / 2, height / (max_menu_choice + 1) * 0.7));

	menu[1].setString("Play");

	menu[2].setString("Scoreboard");

	menu[3].setString("Exit");

	for (int i = 1; i < max_menu_choice; i++)
	{
		menu[i].setFont(fontMenu);
		menu[i].setFillColor(Color::Blue);
		menu[i].setOutlineThickness(5.f);
		menu[i].setOutlineColor(Color::Black);
		menu[i].setCharacterSize(60);
		menu[i].setPosition(Vector2f((width / 2) - menu[i].getGlobalBounds().width / 2, height / (max_menu_choice + 1) * (i+1)));
	}

}


Menu::~Menu()
{
}

void Menu::draw(RenderWindow& window)
{
	window.draw(menuBG);

	for (int i = 0; i < max_menu_choice; i++)
	{
		window.draw(menu[i]);
	}
}
