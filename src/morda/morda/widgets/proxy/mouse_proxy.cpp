#include "mouse_proxy.hpp"

using namespace morda;

mouse_proxy::mouse_proxy(std::shared_ptr<morda::context> c, const puu::forest& desc) :
		widget(std::move(c), desc)
{}

bool mouse_proxy::on_mouse_button(bool isDown, const morda::vector2& pos, mouse_button button, unsigned pointerId){
	if(this->mouse_button_handler){
		return this->mouse_button_handler(*this, isDown, pos, button, pointerId);
	}
	return false;
}

bool mouse_proxy::on_mouse_move(const morda::vector2& pos, unsigned pointerId){
	if(this->mouse_move_handler){
		return this->mouse_move_handler(*this, pos, pointerId);
	}
	return false;
}

void mouse_proxy::on_hover_changed(unsigned pointerID){
	if(this->hover_changed_handler){
		this->hover_changed_handler(*this, pointerID);
	}
}
