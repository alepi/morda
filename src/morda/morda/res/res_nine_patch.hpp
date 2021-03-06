#pragma once

#include "../config.hpp"

#include "../resource_loader.hpp"

#include "res_image.hpp"

namespace morda{

/**
 * @brief Nine-patch resource.
 * Nine-patch is a stretchable image with defined unstretchable borders.
 * 
 * %resource description:
 * 
 * @param file - name of the image file, can be raster image or SVG.
 * 
 * @param borders - widths of borders in pixels in the left-top-right-bottom order.
 * 
 * Example:
 * @code
 * morda_npt_rightbutton_pressed{
 *     file{rightbutton_pressed.svg}
 *     borders{5 5 5 6}
 * }
 * @endcode
 */
class res_nine_patch : public resource{
	friend class resource_loader;
	
	const std::shared_ptr<const res_image> image;
	
	sides<real> borders_v;
	
public:
	res_nine_patch(const res_nine_patch&) = delete;
	res_nine_patch& operator=(const res_nine_patch&) = delete;
	
	res_nine_patch(std::shared_ptr<morda::context> c, std::shared_ptr<const res_image> image, sides<real> borders) :
			resource(std::move(c)),
			image(std::move(image)),
			borders_v(borders)
	{}
	
	
	class image_matrix{
		const std::array<std::array<std::shared_ptr<const res_image>, 3>, 3> images_v;
		
		std::weak_ptr<const res_nine_patch> parent;
	
		real mul;//for erasing from the cache
	public:
		decltype(images_v)& images()const noexcept{
			return this->images_v;
		}
		
		image_matrix(std::array<std::array<std::shared_ptr<const res_image>, 3>, 3>&& l, std::shared_ptr<const res_nine_patch> parent, real mul);
		
		~image_matrix()noexcept;
	};
	
	std::shared_ptr<image_matrix> get(sides<real> borders)const;
	
	const decltype(borders_v)& borders()const noexcept{
		return this->borders_v;
	}
private:
	mutable std::map<real, std::weak_ptr<image_matrix>> cache;
	
	static std::shared_ptr<res_nine_patch> load(morda::context& ctx, const puu::forest& desc, const papki::file& fi);
};

}
