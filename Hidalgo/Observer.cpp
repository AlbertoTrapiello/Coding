// ConsoleApplication5.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <vector>
#include <iostream>

using namespace std;

class Observer // clase interfaz para los observadores
{
protected:
	int Index = -1; // index wich allows to find each observer in the vector

public:
	int getIndex()
	{
		return Index; // returns the value of the index
	}
	void setIndex(int i)
	{
		Index = i; // sets the value for the index
	}
	~Observer()
	{

	}
	virtual void update(int & v) = 0; // every observer can react different to the changes
};

class SujetoConcreto: public Observer
{
	int estadoSujeto; // the actual value of the SujetoConcreto

public:
	void update(int & v) // updates the value
	{
		cout << "observers[" << Index << "] has recived the new value: " << v << endl;
	}
};

class Observable
{
	bool changed; // variable wich states if the observable has Changed
	int value; // value to be observed, so that when it changes the Observer can be notified
	vector <SujetoConcreto> observers; // list of observers
	int index = 0; // index of the recently created object of the vector

/*
protected:
	void setChanged() // NO IDEA
	{

	}
	void clearChanged() // NO IDEA
	{

	}
*/

public:
	~Observable()
	{

	}
	void setValue(int v)
	{
		value = v; // changes the value to the new value
		Observable::hasChanged(); // notifies the observers
		cout << "New value: " << value << endl;
	}
	int getValue()
	{
		return value; // returns the value
	}
	void decreaseIndex()
	{
		for (int i = 0; i < observers.size(); i++)
		{
			if (observers[i].getIndex() > 0)
			{
				cout << "The observer[" << i << "] had previously as index: " << observers[i].getIndex() << endl;
				observers[i].setIndex(observers[i].getIndex() - 1); // it reduces in 1 all the indexes from all the elements from the vector
				cout << "The observers[" << i << "]'s index is: " << observers[i].getIndex() << endl;
			}
		}
	}
	void addObserver(SujetoConcreto & o)
	{
		o.setIndex(index); // each SujetoConcreto of the vector has it's own position
		observers.push_back(o); // when a new SujetoConcreto wish to observe the variable it needs to attach to the vector so it can be notified as the rest
		cout << "The index given to this observer is: " << index << " and the observer has " << o.getIndex() << endl;
		index++; // each time an element is deleted the index increases
	}
	void deleteObserver(SujetoConcreto & o)
	{
		if (o.getIndex() < index)
		{
			cout << "Deleted observer has " << o.getIndex() << " as index, and the actual index is " << index << endl;
			observers.erase(observers.begin() + o.getIndex()); // erases the vector's object wich position is the one dictated by the object
			Observable::decreaseIndex(); // it changes all the indexes from the vector
			index--; // each time an element is deleted the index decreases
			cout << "The new index is: " << index << endl;
		}
		else
			cout << "Couldn't delete the observer" << endl; //
	}
	void deleteObservers(SujetoConcreto & o)
	{
		observers.clear(); // deletes all the objects from the vector
		index = 0; // if all the elements of the vector are deleted tha inde goes to 0
	}
	int countObservers()
	{
		return observers.size(); // returns the size of the vector
	}
	void hasChanged()
	{
		Observable::notifyObservers(value); // when a changes happends it notifies the changes to the observers
	}
	void notifyObservers(int & arg)
	{
		for (int i = 0; i < observers.size(); i++)
			observers[i].update(value); // for each element from the vector, it should be responsible of updating himself
	}
};

int main(void)
{
	Observable O;
	SujetoConcreto SC[5];

	O.setValue(2);
	for(int i = 0; i < 3; i++)
		O.addObserver(SC[i]);
	O.setValue(3);
	O.deleteObserver(SC[0]);
	O.countObservers();
	O.setValue(4);
	O.deleteObserver(SC[2]);
	O.countObservers();

	for(int i = 3; i < 5; i++)
		O.addObserver(SC[i]);
	O.countObservers();

	system("PAUSE");
	return 0;
}
