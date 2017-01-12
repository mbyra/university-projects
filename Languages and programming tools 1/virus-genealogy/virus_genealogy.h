#ifndef VIRUS_GENEALOGY_H
#define VIRUS_GENEALOGY_H

#include <iostream>
#include <algorithm>
#include <cassert>
#include <memory>
#include <map>
#include <vector>
#include <set>
#include <exception>


// 1. All needed exception classes:

class VirusNotFound : public std::exception {
public:
	char const * what() const throw() {
		return "Exception: VirusNotFound";
	}
};

class VirusAlreadyCreated : public std::exception {
public:
	char const * what() const throw() {
		return "Exception: VirusAlreadyCreated";
	}
};

class TriedToRemoveStemVirus : public std::exception {
public:
	char const * what() const throw() {
		return "Exception: TriedToRemoveStemVirus";
	}
};


// 2. Wrapper class for VirusGenealogy with additional containers with parents and children of the virus
//    Thanks to use of wrapper the logic of virus node and connections between nodes are separated

template <class Virus>
class VirusGenealogyWrapper {
	Virus virus;

public:
	// Children and parents sets contain smart pointers to appropriate virus nodes.
	// Due to parents set graph is no longer directed (as in description), but the logic of a program is the same.
	std::set<std::shared_ptr<VirusGenealogyWrapper<Virus>>> children;
	std::set<std::shared_ptr<VirusGenealogyWrapper<Virus>>> parents;

	VirusGenealogyWrapper(typename Virus::id_type id) : virus(id) {}

	Virus& getVirus() {
		return virus;
	}

};


// 3. The main class of VirusGenealogy:


/*
  How the the try { my_function } ... swap ... try ... works:
  Our class have 2 copies of graph.
  After each function it is guaranteed that the copy1 is equal to copy2.
  Each operation is performed on copy2:
    * in case of exception copy2 is returned to copy1 state - strong exception guarantee of each operation
  Then copy2 is swapped with copy1 i.e. basic representation of graph
    * O(1) complexity, no-throw guarantee.
  Copy is updated:
    * in case of exception copy is restored by coping from original (i.e. copy1)
    * it's expensive but in return in case of exception whole operation is still performed and exceptions are rare
  Result: valid copy on both copies, with old state restored if there was exception
*/

template <class Virus>
class VirusGenealogy {
private:

	const typename Virus::id_type _stem_id;
	std::map<typename Virus::id_type, std::shared_ptr<VirusGenealogyWrapper<Virus>>> virus_map;
	std::map<typename Virus::id_type, std::shared_ptr<VirusGenealogyWrapper<Virus>>> virus_map_copy; //copy of the graph

    void undo()
	{
		virus_map_copy = virus_map; // return copy to the original state
	}

	void my_create(typename Virus::id_type const &id, std::vector<typename Virus::id_type> const &parent_ids)
	{
		std::pair<typename Virus::id_type, std::shared_ptr<VirusGenealogyWrapper<Virus>>>
			n(id, std::make_shared<VirusGenealogyWrapper<Virus> >(id));
		// Inserts new pair into virus_map;
		virus_map_copy.insert(n);
		// For every given parent...
		for (auto parent_id : parent_ids) {
			// Adds to new virus wrapper's parents list pointer to node of given "parent_id":
			n.second->parents.insert(virus_map_copy.find(parent_id)->second);
			// After insertion the parent wrapper's children list must be appended fresh virus node:
			virus_map_copy.find(parent_id)->second->children.insert(virus_map_copy.find(id)->second);
		}
	}

	void my_connect(typename Virus::id_type const &child_id, typename Virus::id_type const &parent_id)
	{
		// Explanation: for found child/parent node in set, by its pointer to wrapper (second in pair),
		// it appends wrapper's list of parents/children with pointer to parent/children of given id:
		virus_map_copy.find(parent_id)->second->children.insert(virus_map_copy.find(child_id)->second);
		virus_map_copy.find(child_id)->second->parents.insert(virus_map_copy.find(parent_id)->second);
	}

	void my_remove(typename Virus::id_type const &id)
	{
			std::vector<typename Virus::id_type> my_parents = get_parents(id);
			std::vector<typename Virus::id_type> my_children = get_children(id);
			for (auto parent : my_parents)
			{
				virus_map_copy.find(parent)->second->children.erase(virus_map_copy.find(id)->second);
			}
			for (auto child : my_children)
			{
				virus_map_copy.find(child)->second->parents.erase(virus_map_copy.find(id)->second);
				if (virus_map_copy.find(child)->second->parents.size() == 0)
				{
					my_remove(child); // if child has no parents it should be removed
				}
			}
			virus_map_copy.erase(id);
	}

public:

	// Creates new genealogy.
	// Creates new stem virus with id "stem_id".
	// Implementation note: inserts new pair of stem_id and shared pointer to it into just initialized virus_map.
	VirusGenealogy(typename Virus::id_type const &stem_id)
		: _stem_id(stem_id)
	{
		try {
			virus_map_copy.insert(std::make_pair(stem_id, std::make_shared<VirusGenealogyWrapper<Virus> >(stem_id)));
		}
		catch (std::exception& e) {
			undo();
			std::cerr << "Got exception. Function won't update anything" << std::endl;
			return;
		}

		swap(virus_map, virus_map_copy);

		try {
			virus_map_copy.insert(std::make_pair(stem_id, std::make_shared<VirusGenealogyWrapper<Virus> >(stem_id)));
		}
		catch (std::exception& e) {
			undo();
			std::cerr << "Got exception. Function still calculated correctly" << std::endl;
			return;
		}
	}

	// Requirements say that any attempt to use copy constructor...
	VirusGenealogy(const VirusGenealogy &) = delete;
	// or assignment operator for objects of class VirusGenealogy should result in compilation error
	VirusGenealogy & operator=(const VirusGenealogy &) = delete;
	VirusGenealogy & operator=(VirusGenealogy &&) = delete;

	// Returns id of stem virus.
	typename Virus::id_type get_stem_id() const
	{
		return _stem_id;
	}

	// Returns logical value dependent on if virus of given id exists.
	bool exists(typename Virus::id_type const &id) const
	{
		return virus_map_copy.find(id) != virus_map_copy.end(); //we use copy becouse this function is used in remove ( in all other cases copy1 = copy2)
	}

	// Returns the vector containing id's of immediate successors (i.e. children) of the virus of given id.
	// Throws VirusNotFound exception if virus of given id doesn't exist.
	std::vector<typename Virus::id_type> get_children(typename Virus::id_type const &id) const
	{
		if (!exists(id)) {
			throw VirusNotFound();
		}

		std::vector<typename Virus::id_type> children;
		for (auto c : virus_map_copy.find(id)->second->children) {
			children.push_back(c->getVirus().get_id());
		}

		return children;
	}

	// Returns the vector containing id's of immediate prodecessors (i.e. parents) of the virus of given id.
	// Throws VirusNotFound exception if virus of given id doesn't exist.
	std::vector<typename Virus::id_type> get_parents(typename Virus::id_type const &id) const
	{
		if (!exists(id)) {
			throw VirusNotFound();
		}

		std::vector<typename Virus::id_type> parents;
		for (auto p : virus_map_copy.find(id)->second->parents) { //we use copy becouse this function is used in remove ( in all other cases copy1 = copy2)
			parents.push_back(p->getVirus().get_id());
		}

		return parents;
	}

	// Returns reference to object representing virus of given id.
	// Throws VirusNotFound exception if virus of given id doesn't exist.
	Virus& operator[](typename Virus::id_type const &id) const
	{
		if (!exists(id)) {
			throw VirusNotFound();
		}

		return virus_map.find(id)->second->getVirus();
	}

	// Creates node representing virus of given id by constructing it from one parent.
	// Throws VirusAlreadyCreated if virus of given id already exists.
	// Throws VirusNotFound if given parent doesn't exist.
	void create(typename Virus::id_type const &id, typename Virus::id_type const &parent_id)
	{
		std::vector<typename Virus::id_type> const parent_ids = { parent_id };
		create(id, parent_ids); //just calls create on vector
	}

	// Creates node representing virus of given id by constructing it from vector of parents.
	// Throws VirusAlreadyCreated if virus of given id already exists.
	// Throws VirusNotFound if any of given parents doesn't exist.
	void create(typename Virus::id_type const &id, std::vector<typename Virus::id_type> const &parent_ids)
	{
		if (exists(id)) {
			throw VirusAlreadyCreated();
		}
		if (parent_ids.empty()) {
			throw VirusNotFound();
		}
		for (typename Virus::id_type id : parent_ids) {
			if (!exists(id)) {
				throw VirusNotFound();
			}
		}
		try {
			my_create(id, parent_ids);
		}
		catch (std::exception& e) {
			undo();
			std::cerr << "Got exception. Function won't update anything" << std::endl;
			return;
		}

		swap(virus_map, virus_map_copy);

		try {
			my_create(id, parent_ids);
		}
		catch (std::exception& e) {
			undo();
			std::cerr << "Got exception. Function still calculated correctly" << std::endl;
			return;
		}
	}


	// Adds a new edge in genealogy graph between virus with "child_id" and virus with "parent_id".
	// Throws VirusNotFound exception if any of viruses doesn't exist.
	void connect(typename Virus::id_type const &child_id, typename Virus::id_type const &parent_id)
	{
		if (!exists(child_id) || (!exists(parent_id))) {
			throw VirusNotFound();
		}
		try {
			my_connect(child_id, parent_id);
		}
		catch (std::exception& e) {
			undo();
			std::cerr << "Got exception. Function won't update anything" << std::endl;
			return;
		}

		swap(virus_map, virus_map_copy);

		try {
			my_connect(child_id, parent_id);
		}
		catch (std::exception& e) {
			undo();
			std::cerr << "Got exception. Function still calculated correctly" << std::endl;
			return;
		}
	}

    // Removes virus of given id.
    // Throws VirusNotFound exception if virus of given id doesn't exist.
    // Throws TriedToRemoveStemVirus when virus of given id is stem virus.
	void remove(typename Virus::id_type const &id)
	{
		if (!exists(id)) {
			throw VirusNotFound();
		}
		if (id == _stem_id) {
			throw TriedToRemoveStemVirus();
		}
		try {
			my_remove(id);
		}
		catch (std::exception& e) {
			undo();
			std::cerr << "Got exception. Function won't update anything" << std::endl;
			return;
		}

		swap(virus_map, virus_map_copy);

		try {
			my_remove(id);
		}
		catch (std::exception& e) {
			undo();
			std::cerr << "Got exception. Function still calculated correctly" << std::endl;
			return;
		}
	}
};

#endif // VIRUS_GENEALOGY_H
