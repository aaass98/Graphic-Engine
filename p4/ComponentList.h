#ifndef __ComponentList_h
#define __ComponentList_h

#include "Transform.h"
namespace cg {

//favor performance over memory -> double linked list
//head is always a transform component, and shall not be deleted
class ComponentList {
public:
	ComponentList(SceneObject* object);

	~ComponentList();

	bool add(Component* component);

	bool remove(Component* component);

	Component* getComponent(const char* typeName) {
		Component* component = _head;
		while (component) {
			if(component->typeName() == typeName){
				return component;
			}
			else {
				component = component->_next;
			}
		}
		return nullptr;
	}
	
	inline int getCount() {
		return _count;
	}

private:
	SceneObject* _parent;
	Component* _head;
	Component* _tail;
	int _count;

	friend class ComponentListIterator;
};//SceneObjectList

class ComponentListIterator {
public:
	//returns a pointer to the component the iterator is currently referencing.
	Component* getReference() {
		return _currentComponent;
	}

	//moves the iterator to the next position and retu
	Component* next();

	//moves the iterator to the previous position, returning it
	Component* previous();

	//moves the iterator to the list head (also a transform)
	Component* start()
	{
		_currentComponent = _list->_head;
		return _currentComponent;
	}

	Component* end() {
		_currentComponent = _list->_tail;
		return _currentComponent;
	}

	void dispose() {
		delete this;
	}

private:
	ComponentListIterator() {
		_list = nullptr;
		_currentComponent = nullptr;
	}

	ComponentListIterator(ComponentList* list) {
		_currentComponent = nullptr;
		_list = list;
	}

	Component* _currentComponent;
	ComponentList* _list;

	friend SceneObject;
};


}//end namespace cg
#endif //__SceneObjectList_h