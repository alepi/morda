#pragma once

#include "push_button.hpp"
#include "nine_patch_button.hpp"

namespace morda{

class nine_patch_push_button :
		public push_button,
		public nine_patch_button
{
public:
	nine_patch_push_button(std::shared_ptr<morda::context> c, const puu::forest& desc);
	
protected:
	void on_pressed_changed()override;
	
	bool on_mouse_button(bool isDown, const morda::vector2& pos, mouse_button button, unsigned pointerID) override{
		return this->push_button::on_mouse_button(isDown, pos, button, pointerID);
	}

	void on_hover_changed(unsigned pointerID) override{
		this->push_button::on_hover_changed(pointerID);
	}

};

}
