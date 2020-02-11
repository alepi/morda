#pragma once

#include "../widget.hpp"

#include "../../res/ResGradient.hpp"

namespace morda{

/**
 * @brief Gradient widget.
 * This is a widget which can display a rectangle with gradient.
 * From GUI script it can be instantiated as "Gradient".
 *
 * @param gradient - gradient resource name.
 */
class Gradient : public Widget{
	std::shared_ptr<ResGradient> gradient;

public:
	Gradient(const std::shared_ptr<morda::context>& c, const puu::forest& desc);

	Gradient(const Gradient&) = delete;
	Gradient& operator=(const Gradient&) = delete;

	void render(const morda::Matr4r& matrix)const override;
};

}
