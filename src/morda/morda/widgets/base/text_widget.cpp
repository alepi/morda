#include "text_widget.hpp"

#include "../../context.hpp"

#include "../../util/util.hpp"

using namespace morda;

void text_widget::set_font(std::shared_ptr<res_font> font){
	if(!font){
		throw std::invalid_argument("text_widget::SetFont(): passed argument is null");
	}

	this->font_v = std::move(font);

	this->invalidate_layout();

	this->on_font_changed();
}

text_widget::text_widget(std::shared_ptr<morda::context> c, const puu::forest& desc) :
		widget(std::move(c), desc),
		color_widget(this->context, desc)
{
	for(const auto& p : desc){
		if(!is_property(p)){
			continue;
		}

		if(p.value == "font"){
			this->font_v = this->context->loader.load<morda::res_font>(get_property_value(p).to_string().c_str());
		}
	}

	// load default font if needed
	if(!this->font_v){
		this->font_v = this->context->loader.load<res_font>("morda_fnt_normal");
	}
}

single_line_text_widget::single_line_text_widget(std::shared_ptr<morda::context> c, const puu::forest& desc) :
		widget(std::move(c), desc),
		text_widget(this->context, desc)
{
	for(const auto& p : desc){
		if(!is_property(p)){
			continue;
		}

		if(p.value == "text"){
			this->set_text(unikod::toUtf32(get_property_value(p).to_string()));
		}
	}
}

vector2 single_line_text_widget::measure(const morda::vector2& quotum)const noexcept{
	vector2 ret(this->bb.d.x, this->font().get_height());

	for(unsigned i = 0; i != ret.size(); ++i){
		if(quotum[i] >= 0){
			ret[i] = quotum[i];
		}
	}

	return ret;
}

void single_line_text_widget::on_text_changed(){
	this->recompute_bounding_box();
	this->text_widget::on_text_changed();
}

void text_widget::set_text(std::u32string&& text){
	this->lines.clear();
	this->lines.emplace_back(std::move(text));
	this->invalidate_layout();
	this->on_text_changed();
}

std::u32string text_widget::get_text()const{
	std::u32string ret;

	for(auto& l : this->lines){
		ret.append(l);
	}

	return ret;
}
