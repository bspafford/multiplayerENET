#include "item.h"
#include "main.h"
#include "shaderClass.h"
#include "img.h"

#include "font.h"

item::item(int id, int quantity) {
	this->id = id;
	this->quantity = quantity;

	if (id == 0)
		return;

	itemImg = new img(std::to_string(id) + ".png", { 0, 0 }, false);
	itemImg->setSize({ 16, 16 });

    font = new Font("1", "images/minecraft.ttf", 30.0f);
}

void item::draw(Shader* shaderProgram) {
	if (itemImg)
		itemImg->draw(shaderProgram);

	if (font && (quantity != 0 && quantity != 1))
		font->draw();
}

void item::setItem(int itemId) {
	this->id = itemId;
	quantity = 1;

	itemImg = new img(std::to_string(id) + ".png", { 0, 0 }, false);
	itemImg->setSize({ 16, 16 });

	font = new Font(std::to_string(quantity), "images/minecraft.ttf", 30.0f);
}

void item::setLoc(vector loc) {
	if (itemImg)
		itemImg->setLoc(loc);

	if (font) {
		font->setLoc({ -loc.x + Main::pixelSize * 17 - font->getSize().x, loc.y + Main::pixelSize * 6});
		font->setText(std::to_string(quantity));
	}
}

int item::getId() {
	return id;
}

int item::getQuantity() {
	return quantity;
}

void item::remove1() {
	quantity--;

	if (quantity <= 0) {
		id = 0;
		delete itemImg;
		itemImg = nullptr;
	}
}

void item::add1() {
	quantity++;
}