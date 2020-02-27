#include "tab.hpp"
#include "tab_group.hpp"
#include "../../util/util.hpp"

using namespace morda;

tab_group::tab_group(std::shared_ptr<morda::context> c, const puu::forest& desc) :
		widget(std::move(c), desc),
		choice_group(this->context, desc)
{
	for(const auto& p : desc){
		if(!is_property(p)){
			continue;
		}

		if(p.value == "look"){
			for(const auto& pp : p.children){
				if(!is_property(pp)){
					continue;
				}

				if(p.value == "filler"){
					this->set_filler(this->context->loader.load<res_image>(get_property_value(pp).to_string()));
				}
			}
		}
	}

	if(!this->filler){
		this->set_filler(this->context->loader.load<res_image>("morda_img_tabs_filler"));
	}
}

void tab_group::set_filler(std::shared_ptr<res_image> filler){
	this->filler = std::move(filler);
	this->fillerTexture = this->filler->get();
}

morda::Vec2r tab_group::measure(const morda::Vec2r& quotum)const{
	Vec2r ret(quotum);
	for(unsigned i = 0; i != ret.size(); ++i){
		utki::clampBottom(ret[i], real(0));
	}

	real length = 0;

	Sidesr prevBorders = real(0);

	for(auto& c : this->children()){
		ASSERT(c)
		auto& lp = this->get_layout_params_as<container::layout_params>(*c);

		auto t = dynamic_cast<const morda::tab*>(c.get());
		if(!t){
			throw utki::invalid_state("Non-tab widget added to tab_group, only tab widgets are allowed to be added to tab_group");
		}

		morda::Vec2r d;

		for(unsigned j = 0; j != d.size(); ++j){
			if(lp.dims[j] == layout_params::max || lp.dims[j] == layout_params::fill){
				throw utki::invalid_state("'max' or 'fill' encountered in layout parameters for tab_group container");
			}else if(lp.dims[j] == layout_params::min){
				d[j] = -1;
			}else{
				d[j] = lp.dims[j];
			}
		}

		d = c->measure(d);

		length += d.x;

		auto borders = t->getActualBorders();
		length -= std::min(prevBorders.right(), borders.left());
		prevBorders = borders;

		if(quotum.y < 0){
			utki::clampBottom(ret.y, d.y);
		}
	}

	if(quotum.x < 0){
		ret.x = length;
	}

	return ret;
}

void tab_group::lay_out(){
	real pos = 0;

	Sidesr prevBorders = 0;

	for(auto& c : this->children()){
		ASSERT(c)
		auto& lp = this->get_layout_params_as<container::layout_params>(*c);

		auto dim = this->dims_for_widget(*c, lp);
		c->resize(dim);

		auto t = dynamic_cast<morda::tab*>(c.get());
		if(!t){
			throw utki::invalid_state("Non-tab widget added to tab_group, only tab widgets are allowed to be added to tab_group");
		}

		auto borders = t->getActualBorders();

		pos -= std::min(prevBorders.right(), borders.left());
		c->move_to(Vec2r(pos, std::round((this->rect().d.y - c->rect().d.y) / 2)));
		pos += dim.x;

		prevBorders = borders;
	}
}

void tab_group::render(const morda::Matr4r& matrix)const{
	for(auto& w: this->children()){
		if(!this->is_active(*w)){
			this->render_child(matrix, *w);
		}
	}

	auto& ab = this->get_active();
	if(ab){
		this->render_child(matrix, *ab);
	}

	// render filler
	if(this->children().size() != 0){
		real ce = this->children().back()->rect().right();
		real l = this->rect().d.x - ce;
		if(l > 0){
			Matr4r m(matrix);
			m.translate(ce, this->rect().d.y - this->fillerTexture->dims.y);
			m.scale(l, this->fillerTexture->dims.y);
			this->fillerTexture->render(m);
		}
	}
}