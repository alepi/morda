/* The MIT License:

Copyright (c) 2015 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */

// Home page: http://morda.googlecode.com

/**
 * @author Ivan Gagis <igagis@gmail.com>
 */

#pragma once

#include "Widget.hpp"
#include "List.hpp"

#include "containers/ScrollContainer.hpp"

#include <ting/Buffer.hpp>


namespace morda{


class Tree{
	size_t size_var = 0;
	
	std::vector<Tree> children;
	
public:
	class Iterator{
		friend class Tree;
		
		std::vector<size_t> pathIdx;
		std::vector<Tree*> pathPtr;
		
		Iterator(Tree* node, size_t idx){
			ASSERT(node)
			this->pathPtr.push_back(node);
			this->pathIdx.push_back(idx);
		}
	public:
		Iterator() = default;
		
		Iterator(const Iterator&) = default;
		Iterator& operator=(const Iterator&) = default;
		
		size_t depth()const noexcept{
			return this->pathIdx.size();
		}
		
		const decltype(pathIdx)& path()const noexcept{
			return this->pathIdx;
		}
		
		bool operator==(const Iterator& i){
			return this->pathIdx == i.pathIdx;
		}
		
		bool operator!=(const Iterator& i){
			return !this->operator==(i);
		}
		
		Tree& operator*(){
			return this->pathPtr.back()->children[this->pathIdx.back()];
		}
		
		Iterator& operator++(){
			if(this->pathIdx.size() == 0){
				return *this;
			}
			
			{
				auto& list = this->pathPtr.back()->children;
				auto& idx = this->pathIdx.back();

				if(list[idx].children.size() != 0){
					this->pathPtr.push_back(&list[idx]);
					this->pathIdx.push_back(0);
					return *this;
				}
			}

			for(; this->pathIdx.size() != 0;){
				auto& list = this->pathPtr.back()->children;
				auto& idx = this->pathIdx.back();
				
				++idx;
				
				if(idx == list.size()){
					this->pathPtr.pop_back();
					this->pathIdx.pop_back();
				}else{
					break;
				}
			}
			
			return *this;
		}
		
		Iterator& operator--(){
			if(this->pathIdx.size() == 0){
				return *this;
			}
			
			{
				auto& idx = this->pathIdx.back();

				if(idx == 0){
					this->pathPtr.pop_back();
					this->pathIdx.pop_back();
					return *this;
				}
			}
			
			for(;;){
				auto& list = this->pathPtr.back()->children;
				auto& idx = this->pathIdx.back();
				
				--idx;
				
				if(list[idx].children.size() != 0){
					this->pathPtr.push_back(&list[idx]);
					this->pathIdx.push_back(list[idx].children.size());
				}else{
					break;
				}
			}
			
			return *this;
		}
		
		
		
		Iterator& operator+=(size_t d){
			for(decltype(d) i = 0; i != d && this->pathIdx.size() != 0; ++i){
				this->operator++();
			}
			return *this;
		}
		
		Iterator& operator-=(size_t d){
			for(decltype(d) i = 0; i != d && this->pathIdx.size() != 0; ++i){
				this->operator--();
			}
			return *this;
		}
		
	private:
		Iterator& descentTo(size_t index){
			if(this->pathIdx.size() == 0){
				ASSERT(this->pathPtr.size() == 1)
				this->pathIdx.push_back(index);
				return *this;
			}
			
			if(this->pathIdx.back() >= this->pathPtr.back()->children.size()){
				return *this;
			}
			this->pathPtr.push_back(&this->operator*());
			this->pathIdx.push_back(index);
			return *this;
		}
	};
	
	void add(Iterator addInto, size_t numChildrenToAdd){
		for(auto t : addInto.pathPtr){
			TRACE(<< "t = " << t << std::endl)
			t->size_var += numChildrenToAdd;
		}
		
		(*addInto).add(numChildrenToAdd);
	}
	
	void add(size_t numChildrenToAdd){
		this->children.resize(this->children.size() + numChildrenToAdd);
		this->size_var += numChildrenToAdd;
	}
	
	void removeAll(Iterator from){
		size_t numChildrenToRemove = (*from).children.size();
		(*from).children.clear();
		(*from).size_var -= numChildrenToRemove;
		for(auto t : from.pathPtr){
			ASSERT(t->size_var >= numChildrenToRemove)
			t->size_var -= numChildrenToRemove;
		}
	}
	
	void removeAll(){
		this->children.clear();
		this->size_var = 0;
	}
	
	decltype(size_var) size()const noexcept{
		return this->size_var;
	}
	
	size_t numChildren()const noexcept{
		return this->children.size();
	}
	
	Iterator begin(){
		if(this->children.size() == 0){
			return Iterator();
		}
		return Iterator(this, 0);
	}
	
	Iterator end(){
		return Iterator();
	}
	
	Iterator pos(const std::vector<size_t> path){
		Iterator ret;
		ret.pathPtr.push_back(this);
		
		for(auto i = path.begin(); i != path.end(); ++i){
			auto oldDepth = ret.depth();
			ret.descentTo(*i);
			if(oldDepth == ret.depth()){
				break;
			}
		}
		
		return std::move(ret);
	}
	
private:

};



class TreeView :
		virtual public Widget,
		private ScrollContainer
{
	std::shared_ptr<List> list;
public:
	TreeView(const stob::Node* chain = nullptr);
	
	TreeView(const TreeView&) = delete;
	TreeView& operator=(const TreeView&) = delete;
	
	
	class ItemsProvider :
			public virtual ting::Shared,
			private List::ItemsProvider
	{
		friend class TreeView;
		friend std::shared_ptr<List::ItemsProvider> std::static_pointer_cast<List::ItemsProvider>(const std::shared_ptr<ItemsProvider>&);
		
		void recycle(size_t index, std::shared_ptr<Widget> w)override;
		
		std::shared_ptr<Widget> getWidget(size_t index)override;
		
		size_t count() const noexcept override;
		
		mutable Tree visibleTree;
		
		//cached values for faster lookup by index
		mutable size_t iterIndex;
		mutable decltype(visibleTree)::Iterator iter;
		
		const decltype(iter) iterForIndex(size_t index)const;
		
	protected:
		ItemsProvider(){
		}
	public:
		
		virtual std::shared_ptr<Widget> getWidget(const std::vector<size_t>& path, bool isCollapsed) = 0;
		
		virtual void recycle(const std::vector<size_t>& path, std::shared_ptr<Widget> w){}
		
		virtual size_t count(const std::vector<size_t>& path)const noexcept = 0;
		
		void uncollapse(const std::vector<size_t>& path);
		void collapse(const std::vector<size_t>& path);
		
		void notifyDataSetChanged();
		
		//TODO: add notification functions for insert/change/delete
		
	private:
		
	};
private:

public:
	void setItemsProvider(std::shared_ptr<ItemsProvider> provider = nullptr);
	
	void setVerticalScrollPosAsFactor(real factor){
		this->list->setScrollPosAsFactor(factor);
	}
	
	void setHorizontalScrollPosAsFactor(real factor){
		this->setScrollPosAsFactor(Vec2r(factor, 0));
	}
};



}
