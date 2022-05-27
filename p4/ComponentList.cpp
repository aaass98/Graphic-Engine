#include "Component.h"
#include "ComponentList.h"
#include "SceneObject.h"

namespace cg {
	ComponentList::~ComponentList() {
		while (_head!=_tail) {
			remove(_tail);
		}
		_parent->release(_head);
	}

	ComponentList::ComponentList(SceneObject* object) {
		_parent = object;
		Transform* transform = new Transform();
		_parent->_transform = transform;
		transform->_sceneObject = object;
		_parent->makeUse(transform);
		_head = _tail = transform;
		_count = 1;
	}

	bool ComponentList::add(Component* component){
		component->_sceneObject = _parent;
		_tail->_next = component;
		component->_next = nullptr;
		component->_previous = _tail;
		_tail = component;
		_parent->makeUse(component);
		_count++;
		return true;
	}
	
	bool ComponentList::remove(Component* component) {
		if (component == _head) {
			return false;
		}
		else {
			component->_previous->_next = component->_next;
			if (component->_next) {
				component->_next->_previous = component->_previous;
			}
			if (component == _tail) {
				_tail = component->_previous;
			}
			_parent->release(component);
			_count--;
			return true;
		}
	}

	Component* ComponentListIterator::next() {
		if (_currentComponent) {
			_currentComponent = _currentComponent->_next;
		}
		else {
			start();
		}
		return _currentComponent;
	}

	Component* ComponentListIterator::previous() {
		if (_currentComponent) {
			_currentComponent = _currentComponent->_previous;
		}
		else {
			end();
		}
		return _currentComponent;
	}

	Component::Component(const char* const typeName, SceneObject* sceneObject) :
		_next{ nullptr },
		_previous{ nullptr },
		_typeName{ typeName },
		_sceneObject{ sceneObject }
	{
		_sceneObject->addComponent(this);
	}

}