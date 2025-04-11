#pragma once

class Shader;
class img;
class item;

#include <vector>

class Uhotbar {
public:
	Uhotbar();

	void draw(Shader* shaderProgram);
	int getSelectedBar();
	void setSelectedBar(int num);
	void addItemToHotbar(int itemId);
	item* getSelectedItem();

	std::vector<item*> items;
private:
	img* hotbarImg;
	img* selectedImg;

	int selectedBar;
};