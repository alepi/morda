#pragma once

#include "toggle_button.hpp"
#include "../label/nine_patch.hpp"

namespace morda{

/**
 * @brief Checkbox widget.
 * Checkbox is a toggle button which shows tick sign when checked and empty box when unchecked.
 * In GUI script can be instantiated as 'Checkbox'. Only available after initializing
 * standard morda widgets.
 */
class check_box :
		public toggle_button,
		private nine_patch
{
	std::shared_ptr<widget> checkWidget;
public:
	check_box(std::shared_ptr<morda::context> c, const puu::forest& desc);
	
	check_box(const check_box&) = delete;
	check_box& operator=(const check_box&) = delete;

	bool on_mouse_button(bool isDown, const morda::vector2& pos, mouse_button button, unsigned pointerId)override{
		return this->toggle_button::on_mouse_button(isDown, pos, button, pointerId);
	}

	void on_hover_changed(unsigned pointerID)override{
		this->toggle_button::on_hover_changed(pointerID);
	}

	void on_pressed_changed()override;

private:

};

}
