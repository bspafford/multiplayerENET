#include "hotbar.h"
#include "shaderClass.h"
#include "img.h"
#include "main.h"
#include "item.h"

Uhotbar::Uhotbar() {
	hotbarImg = new img("hotbar.png", { 0, 0 }, false);
	selectedImg = new img("selectedHotbar.png", { 0, 0 }, false);

	for (int i = 0; i < 9; i++) {
		items.push_back(new item(0, 0));
	}

	items[0] = new item(1, 1);
	items[1] = new item(2, 2);
	items[2] = new item(3, 30);
	items[3] = new item(4, 4);
	items[4] = new item(5, 5);
}

void Uhotbar::draw(Shader* shaderProgram) {
	hotbarImg->setLoc(vector{ -Main::screenSize.x / 2.f + hotbarImg->getSize().x / 2.f, 0 });
	hotbarImg->draw(shaderProgram);

	selectedImg->setLoc(hotbarImg->loc + Main::pixelSize - vector{ selectedBar * 20 * Main::pixelSize, 0 });
	selectedImg->draw(shaderProgram);

	for (int i = 0; i < items.size(); i++) {
		items[i]->setLoc(hotbarImg->loc - vector{3 + 20.f * i, 3} * Main::pixelSize);
		items[i]->draw(shaderProgram);
	}
}

int Uhotbar::getSelectedBar() {
	return selectedBar;
}

void Uhotbar::setSelectedBar(int num) {
	selectedBar = num;
}

void Uhotbar::addItemToHotbar(int itemId) {
	// see if already in hotbar
	// if not in hot bar, make sure there's space
	// if there is space, then update item

	int openSpace = -1;
	for (int i = 0; i < items.size(); i++) {
		if (items[i]->getId() == itemId) {
			items[i]->add1();
			return;
		} else if (items[i]->getId() == 0 && openSpace == -1)
			openSpace = i;
	}

	if (openSpace != -1) {
		items[openSpace]->setItem(itemId);
	}
}

item* Uhotbar::getSelectedItem() {
	return items[selectedBar];
}