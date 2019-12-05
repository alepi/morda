#include "ResTexture.hpp"

#include "../exception.hpp"
#include "../util/RasterImage.hpp"
#include "../util/util.hpp"



using namespace morda;



//static
std::shared_ptr<ResTexture> ResTexture::load(const stob::Node& chain, const papki::File& fi){
//	TRACE(<< "ResTexture::Load(): enter" << std::endl)

//	TRACE(<< "ResTexture::Load(): Loading image, file path = " << fileVal->value() << std::endl)
	fi.setPath(chain.side("file").up().value());

	return std::make_shared<ResTexture>(loadTexture(fi));
}
