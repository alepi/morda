#pragma once

#include "../config.hpp"

#include "../resource_loader.hpp"

#include "ResImage.hpp"

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
class ResNinePatch : public resource{
	friend class resource_loader;
	
	const std::shared_ptr<const ResImage> image;
	
	Sidesr borders_v;
	
public:
	ResNinePatch(const ResNinePatch&) = delete;
	ResNinePatch& operator=(const ResNinePatch&) = delete;
	
	ResNinePatch(std::shared_ptr<morda::context> c, std::shared_ptr<const ResImage> image, Sidesr borders) :
			resource(std::move(c)),
			image(std::move(image)),
			borders_v(borders)
	{}
	
	
	class ImageMatrix :	virtual public utki::shared{
		const std::array<std::array<std::shared_ptr<const ResImage>, 3>, 3> images_v;
		
		std::weak_ptr<const ResNinePatch> parent;
	
		real mul;//for erasing from the cache
	public:
		decltype(images_v)& images()const noexcept{
			return this->images_v;
		}
		
		ImageMatrix(std::array<std::array<std::shared_ptr<const ResImage>, 3>, 3>&& l, std::shared_ptr<const ResNinePatch> parent, real mul);
		
		~ImageMatrix()noexcept;
	};
	
	std::shared_ptr<ImageMatrix> get(Sidesr borders)const;
	
	const decltype(borders_v)& borders()const noexcept{
		return this->borders_v;
	}
private:
	mutable std::map<real, std::weak_ptr<ImageMatrix>> cache;
	
	static std::shared_ptr<ResNinePatch> load(morda::context& ctx, const puu::forest& desc, const papki::file& fi);
};

}
