#include "SceneObjectList.h"
#include "SceneObject.h"

namespace cg {

	SceneObjectList::SceneObjectList(SceneNode* parent) {
		_parent = parent;
		_head = _tail = nullptr;
		_count = 0;
	}

	SceneObjectList::~SceneObjectList() {
		while (_head) {
			remove(_head);
		}
	}

	void SceneObjectList::add(SceneObject* sceneObject){
		if (_head) {
			_tail->_next = sceneObject;
		}
		else {//this is the first object in the list
			_head = sceneObject;
		}
		sceneObject->_next = nullptr;
		sceneObject->_previous = _tail;
		_tail = sceneObject;
		SceneObject::makeUse(sceneObject);
		_count++;
	}

	void SceneObjectList::remove(SceneObject* sceneObject) {
		if (sceneObject->_previous)
			sceneObject->_previous->_next = sceneObject->_next;
		if (sceneObject->_next)
			sceneObject->_next->_previous = sceneObject->_previous;
		if (sceneObject == _head)
			_head = sceneObject->_next;
		if (sceneObject == _tail)
			_tail = sceneObject->_previous;
		SceneObject::release(sceneObject);
		_count--;
	}

	SceneObject* SceneObjectListIterator::next() {
		if (_currentObject) {
			_currentObject = _currentObject->_next;
		}
		else {
			start();
		}
		return _currentObject;
	}

	SceneObject* SceneObjectListIterator::previous() {
		if (_currentObject) {
			_currentObject = _currentObject->_previous;
		}
		else {
			end();
		}
		return _currentObject;
	}
}