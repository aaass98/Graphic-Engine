#ifndef __SceneObjectList_h
#define __SceneObjectList_h

namespace cg {
class SceneObject;
class SceneNode;

//favor performance over memory -> double linked list
class SceneObjectList {
public:
	

	SceneObjectList(SceneNode* parent);

	~SceneObjectList();

	//add a new sceneObject to the list
	void add(SceneObject* sceneObject);

	//remove a sceneObject from the list
	void remove(SceneObject* sceneObject);

	inline int getCount() {
		return _count;
	}
private:
	SceneObjectList() {
		_head = nullptr;
		_tail = nullptr;
		_parent = nullptr;
		_count = 0;
	}

	SceneObject* _head;
	SceneObject* _tail;
	SceneNode* _parent;
	int _count;

	friend class SceneObjectListIterator;
};//SceneObjectList

class SceneObjectListIterator {
public:

	//returns a pointer to the object the iterator is currently referencing
	SceneObject* getReference() {
		return _currentObject;
	}

	//moves the iterator to the next position, returning it
	SceneObject* next();

	//moves the iterator to the previous position, returning it
	SceneObject* previous();

	//moves iterator to the list head
	SceneObject* start() {
		_currentObject = _list->_head;
		return _currentObject;
	}

	//moves iterator to the list tail
	SceneObject* end() {
		_currentObject = _list->_tail;
		return _currentObject;
	}

	//release the iterator from memory
	void dispose() {
		delete this;
	}

private:
	SceneObjectListIterator() {
		_currentObject = nullptr;
		_list = nullptr;
	}

	SceneObjectListIterator(SceneObjectList* list) {
		_currentObject = nullptr;
		_list = list;
	}

	SceneObject* _currentObject;
	SceneObjectList* _list;

	friend SceneNode;
};//SceneObjectListIterator

}//end namespace cg
#endif //__SceneObjectList_h