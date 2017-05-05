// Josh Mau
// Mar 30, 2017
// P.h -- Header file for Object Oriented
//		Read and categorize UTF-8 characters
// 		from multiple files.

#ifndef P_H_INCLUDED
#define P_H_INCLUDED

#include <string>
#include <vector>
#include <set>
#include <map>

class P {
	public:
		// Constructors and Destructor -------------------------
		P();                      // Default Constructor
		P(const P &);             // Copy Constructor
		P(const std::string);     // Test Constructor (Takes properties filename)
        ~P();                     // Destructor

		// BEGIN operators -------------------------------------------------
		// Assignment 
		P &operator=(const P &);  // Assignment Operator
		
		// ostream
		friend std::ostream &operator<<(std::ostream &, const P &);
		
		// Boolean Evaluation
		operator bool()const;
		// END operators ----------------------------------------------------
		
		// Methods of Class 'P' --------------------------------
		void readfile(const std::string);     // Pareses Properties file

        std::set<std::string> props() const;  // Returns std::set of unique property names

        void count(int codepoint);            // Takes a codepoint and counts it
		int count(std::string) const;         // Takes a property name and returns the count of that property
               
		int size() const;                     // Returns number of unique property names
        
        bool empty() const;                   // QUESTION: are there no property names?
        void clear();                         // CAUTION: Clears all object data
        void clear_properties();              // Clear list of properties (if readfile is called twice)
        
    private:
        std::string properties_file_name;
		std::set<std::string> unique_properties;
		std::map<int, std::string> hex_to_property;
		std::map<std::string, int> property_counts;
    
};

#endif
