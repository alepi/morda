#include "ChoiceButton.hpp"

#include "ChoiceGroup.hpp"

using namespace morda;



ChoiceButton::ChoiceButton(const stob::Node* chain) :
		Widget(chain),
		Button(chain),
		ToggleButton(chain)
{
}


void ChoiceButton::onPressedChanged() {
	auto cg = this->find_ancestor<ChoiceGroup>();
	if(!cg){
		this->ToggleButton::onPressedChanged();
		return;
	}
	
	this->ToggleButton::onPressedChanged();
	
	if(this->isPressed()){
		cg->setActiveChoiceButton(this->sharedFromThis(this));
	}
}

bool ChoiceButton::on_mouse_button(bool isDown, const morda::Vec2r& pos, MouseButton_e button, unsigned pointerID) {
	if(this->isPressed()){
		return true;
	}
	return this->ToggleButton::on_mouse_button(isDown, pos, button, pointerID);
}
