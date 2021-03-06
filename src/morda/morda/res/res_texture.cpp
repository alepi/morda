#include "res_texture.hpp"

#include "../util/raster_image.hpp"
#include "../util/util.hpp"
#include "../context.hpp"


using namespace morda;



std::shared_ptr<res_texture> res_texture::load(morda::context& ctx, const puu::forest& desc, const papki::file& fi){
	for(auto& p: desc){
		if(p.value == "file"){
			fi.set_path(get_property_value(p).to_string());
		}
	}

	return std::make_shared<res_texture>(utki::make_shared_from_this(ctx), load_texture(*ctx.renderer, fi));
}
