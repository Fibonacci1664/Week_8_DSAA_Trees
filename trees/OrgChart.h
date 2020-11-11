#ifndef ORGCHART_H
#define ORGCHART_H

#include <iostream>
#include <string>
#include <vector>

// Forward declarations.
class Person;
class OrgChart;

/** A person within the organisation. */
class Person
{
	// OrgChart is allowed to use the private members of Person.
	friend class OrgChart;

public:
	/** Constructor, specifying the person's name and manager. */
	Person(const std::string& name, Person* parent)
		: name_(name), parent_(parent)
	{
	}

	/** Destructor. */
	~Person()
	{
		for (Person* child : children_)
		{
			delete child;
		}
	}

	/** Create and return a new Person object, making it a child of the calling object. */
	Person* add_child(const std::string& name)
	{
		Person* child = new Person(name, this);	// Person pointer created on the HEAP.
		children_.push_back(child);
		return child;
	}

	/** Return the level of this person within the organisation.
	    The CEO is level 1; they manage people at level 2, and so on. */
	// "const" here (and below) means that this method doesn't change this object.
	int level(int depth = 0) const
	{
		if (parent_ == nullptr)
		{
			return 1;
		}

		depth = parent_->level() + 1;

		return depth;
	}
	
private:
	// -- Helper methods that will be used by OrgChart below --

	/** Print out this person and all their descendants, indented by depth spaces. */
	void print(int depth = 0) const
	{
		for (int i = 0; i < depth; i++)
		{
			std::cout << " ";
		}

		std::cout << "- " << name_ << "\n"; // The first time this executes we are printing the root.

		// Print the children, indented by one more space.
		/*
		 * After we have printed Dan we DO NOT enter this loop the next time we arrive here
		 * but instead skip over it, only to the "walk back up the code" to the for loop
		 * to then enter it and call print on what is the 2nd Person pointer at this level.
		 * How is this achieved, I understand we are looping over each pointer at each level.
		 * But I'm confused as to how the code appears to move backwards???
		 * 
		 * How does the code know to move back up the tree back to depth 1 (Ben) once it
		 * first gets down to depth 2 and realises that Dan has no children.
		 * 
		 * CALL STACK!!, Pushing and popping as we go!
		 */
		for (Person* child : children_)
		{
			child->print(depth + 1);
		}
	}

	// FIXME: Add more helper methods here!

	bool contains(const std::string& name, bool found = false) const
	{
		/*
		 * We have an extra "found" check here at the start to prevent us having to search over anymore children than necessary.
		 * We will only have to search as much tree as we need until we find a match. Then after we pop back to the root
		 * we will only search the remaining children of the root and no deeper as we have already found a match so we can just return,
		 * For this particular structure the root only has 2 children.
		 * 
		 * e.g.
		 * (BEST CASE) name == root_->name
		 * (WORST CASE) If we didn't find it from a search of the left then we will of course
		 * need to search the right to find it. basically traverse the whole tree.
		 * (TYPICAL CASE) If we find it while initially searching the left then we will NOT need
		 * to traverse the right side of the tree but instead just immediately return
		 * once we get to the righthand child of the root_
		 * 
		 * At this point everything is popped and so we rtn to the calling func in OrgChart.
		 */

		// If the names match OR if we have previously found a match.
		// Check if the name passed is the root or the child we are current on.
		if (name_ == name || found)
		{
			found = true;
			return found;
		}
		else
		{
			// Name is somewhere in the tree structure.
			// Check all the children.
			for (Person* child : children_)
			{
				/*
				 * Set found to whatever is returned by contains, passing in the name and current status of found
				 * This way we track whether or not we have found a matching name.
				 * This bool value will be passed back up the tree when we pop all the way back.
				 */
				found = child->contains(name, found);
			}

			/*
			 * Check if we have already at some point found a match, if so then return that value.
			 * Otherwise return the else statement which will return found(false) having NEVER been set to true.
			 */
			if (found)
			{
				return found;	// Rtn true.
			}
			else
			{
				return found;	// Rtn false.
			}
		}		
	}

	int size(int count = 0)
	{
		// Iterate over every node in the tree counting as we go.
		for (Person* child : children_)
		{
			count = child->size(count + 1);
		}

		// Return count.
		return count;
	}

	int height(int currHeight = 0, int maxStoredHeight = 0)
	{
		// Iterate over every node in the tree counting as we go.
		for (Person* child : children_)
		{
			maxStoredHeight = child->height(currHeight + 1, maxStoredHeight);
		}

		if (currHeight > maxStoredHeight)
		{
			maxStoredHeight = currHeight;
			return maxStoredHeight;
		}
		else
		{
			return maxStoredHeight;
		}	
	}

	// -- Member variables --

	/** The name of this person. */
	std::string name_;

	/** Pointer to this person's manager.
		If this person is at the root of the tree, parent_ == nullptr. */
	Person* parent_;

	/** Pointers to the people this person manages.
	    Each Person owns the people they manage, so it's responsible for
		deleting them in the destructor above. */
	std::vector<Person* > children_;
};

/** A organisational chart, represented as a tree of Person nodes. */
class OrgChart
{
public:
	/** Constructor, specifying the CEO's name. */
	OrgChart(const std::string& name)
		: root_(new Person(name, nullptr))
	{
	}

	/** Destructor. */
	~OrgChart()
	{
		delete root_;
	}

	/** Return a pointer to the CEO's Person object. */
	Person* root()
	{
		return root_;
	}

	/** Print out all the people in the organisation. */
	void print() const
	{
		root_->print();
	}

	/** Returns true if name is a member of the organisation. */
	bool contains(const std::string& name) const
	{
		bool contains = root_->contains(name);

		return contains;
	}

	/** Returns the number of people in the organisation. */
	int size() const
	{
		int size = root_->size();

		// Return num + 1, we add 1 as we need to include the root node.
		return size + 1;
	}

	/** Returns the number of levels in the org chart. */
	int height() const
	{
		int height = root_->height();

		return height + 1;
	}

private:
	/** Pointer to the Person at the root of the tree.
		This is guaranteed not to be nullptr, i.e. there's always at
		least one person in the organisation.
		The OrgChart object owns this Person, and is responsible
		for deleting it in the destructor above. */
	Person* root_;
};

#endif