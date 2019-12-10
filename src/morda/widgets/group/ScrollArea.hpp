#pragma once

#include "../container.hpp"

#include <functional>


namespace morda{


/**
 * @brief Scroll area container widget.
 * Scroll area is a container which can add an offset to its children widget positions.
 * From GUI scripts it can be instantiated as "ScrollArea".
 * Note, that Scrollarea has same layout parameters as simple Container and those work similarly,
 * except 'max' value. If layout dimension is specified as 'max' then child widget will be stretched to the
 * parent (ScrollArea) size in case child's minimal size is less than ScrollArea size, otherwise child will be assigned
 * its minimal size.
 */
class ScrollArea : public Container{
	//offset from top left corner
	Vec2r curScrollPos = Vec2r(0);

	//cached effectiveDim
	Vec2r effectiveDim;

	//cached scroll factor
	Vec2r curScrollFactor;

protected:
	Vec2r dimForWidget(const Widget& w, const LayoutParams& lp)const;

public:
	ScrollArea(const stob::Node* chain);

	ScrollArea(const ScrollArea&) = delete;
	ScrollArea& operator=(const ScrollArea&) = delete;


	bool onMouseButton(bool isDown, const morda::Vec2r& pos, MouseButton_e button, unsigned pointerID)override;

	bool onMouseMove(const morda::Vec2r& pos, unsigned pointerID)override;

	void render(const morda::Matr4r& matrix) const override;

	morda::Vec2r measure(const morda::Vec2r& quotum) const override{
		return this->Widget::measure(quotum);
	}

	void lay_out()override;

	void onChildrenListChanged()override;

	/**
	 * @brief Get current scroll position.
	 * @return Current scrolling position in pixels.
	 */
	const Vec2r& scrollPos()const{
		return this->curScrollPos;
	}

	/**
	 * @brief Set scroll position.
	 * @param newScrollPos - new scroll position.
	 */
	void setScrollPos(const Vec2r& newScrollPos);

	/**
	 * @brief Set scroll position as factor.
	 * @param factor - factor with components from range [0:1].
	 */
	void setScrollPosAsFactor(const Vec2r& factor);

	/**
	 * @brief Get current scroll position as factor.
	 * @return Current scroll position as factor with components from range [0:1].
	 */
	const Vec2r& scrollFactor()const{
		return this->curScrollFactor;
	}

	Vec2r visibleAreaFraction()const noexcept;

private:
	void updateEffectiveDim();

	void updateScrollFactor();

	void clampScrollPos();

	void arrangeWidgets();
};



}
