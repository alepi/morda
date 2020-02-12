#pragma once

#include "../widget.hpp"

#include "../base/BlendingWidget.hpp"

#include "../../res/ResImage.hpp"


namespace morda{

/**
 * @brief Image widget.
 * This widget can display an image.
 * From GUI script it can be instantiated as "Image".
 *
 * @param image - image resource.
 * @param keepAspectRatio - try to keep aspect ratio of the image when scaling.
 * @param repeatX - replicate image horizontally if size of the widget is bigger than size of the image resource.
 * @param repeatY - replicate image vertically if size of the widget is bigger than size of the image resource.
 */
class Image :
		public virtual Widget,
		public BlendingWidget
{
	Image(const Image&);
	Image& operator=(const Image&);

	std::shared_ptr<const morda::ResImage> img;

	mutable std::shared_ptr<const morda::ResImage::QuadTexture> scaledImage;

	bool keepAspectRatio = false;

	r4::vec2b repeat_v = r4::vec2b(false);
	
	mutable std::shared_ptr<VertexArray> vao;

public:
	Image(std::shared_ptr<morda::context> c, const puu::forest& desc);
public:
	virtual ~Image()noexcept{}

	void render(const morda::Matr4r& matrix)const override;

	morda::Vec2r measure(const morda::Vec2r& quotum)const override;

	void setImage(const std::shared_ptr<const ResImage>& image);

	void on_resize() override;

	const decltype(repeat_v)& repeat()const noexcept{
		return this->repeat_v;
	}

	void setRepeat(decltype(repeat_v) r){
		this->repeat_v = r;
		this->scaledImage.reset();
	}

	void setKeepAspectRatio(bool keepAspectRatio){
		this->keepAspectRatio = keepAspectRatio;
		this->invalidate_layout();
	}
private:

};

}
