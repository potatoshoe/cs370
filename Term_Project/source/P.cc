// Josh Mau
// Mar 30, 2017
// P.cc -- Implementation of P.h methods

#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "P.h"

// Default Constructor -- Accumulated String (utf_string) is empty.
P::P(){
    properties_file_name = "";
}	

// Copy Constructor -- Copy an existing U object to a new object of type: U.
P::P(const P &rhs) : properties_file_name(rhs.properties_file_name), unique_properties(rhs.unique_properties),
    hex_to_property(rhs.hex_to_property), property_counts(rhs.property_counts){
    // Nothing left to do...
}
// Test Constructor -- Take a property file and literal string.
P::P(std::string p) : properties_file_name(p){
    readfile(properties_file_name);
}

// Assignment Operator
P &P::operator=(const P &rhs) {
    properties_file_name = rhs.properties_file_name;
    unique_properties = rhs.unique_properties;
    hex_to_property = rhs.hex_to_property;
    property_counts = rhs.property_counts;
    return *this;
}

// Destructor -- No dynamic data
P::~P(){}

std::ostream &operator<<(std::ostream &out, const P &prop_reference){
	std::string temp = "Property: Count\n"
						"---------------\n";
	for (auto &p : prop_reference.property_counts){
		temp += p.first + ": " + std::to_string(p.second) + '\n';
	}
	return out << temp;
}

P::operator bool()const{
	return ( size() > 0 || properties_file_name != "");
}


// Methods of Class 'P' --------------------------------
void P::readfile(const std::string fileName){
    clear_properties();
    std::ifstream propfile(fileName);	// open property file
    std::string line = "";
    if (!propfile)	// throw error if could not open property file
        throw std::string("Properties file \"" + fileName + "\" could not be opened!");
		
    while(std::getline(propfile, line)){	// Read property file line by line
        std::string temp_string = "";	
        int hex_value = 0;
        int semi_colon_count = 0;
        
        for (char c : line){
            if (c == ';')
                break;
            else
                temp_string += c;		// store hex code as string until first semicolon
        }
        
        std::istringstream iss(temp_string);	// ISS -- istringstream, parse string to hex code for storage
        iss >> std::hex >> hex_value;
        temp_string = "";						// reset temp_string for new data (property name)
        
        for (char c : line){					// Loop through line
            if (c == ';')						// Read data starting at semicolon 2 and ending at 3 (property names ie. 'Lu')
                semi_colon_count++;
            else if (semi_colon_count == 2)		// If not hit 3rd semicolon yet, keep adding char to string
                temp_string += c;
            else if(semi_colon_count > 2)		// On third semicolon, break and add to the following data members
                break;
        }
        
        hex_to_property[hex_value] = temp_string;		// Maps hex code to a property value (ie. 0x0065 mapped to Lu -- 'A')
        unique_properties.insert(temp_string);				// Adds property names to set (not multiset. aka add unique names only)
    }	

    for (auto property_name : unique_properties)
        property_counts[property_name] = 0;					// Init all properties to count of 0
        
    propfile.close();
}

// Returns set of all property names
std::set<std::string> P::props() const {return unique_properties;}
            
// Loops through all property counts until it finds the method
// 	parameter or hits the end of the map.
int P::count(std::string property) const {
	return property_counts.find(property)->second;			// return that property->second (first = name; second = count)
}

void P::count(int codepoint) {
    if (hex_to_property.find(codepoint) != hex_to_property.end())
        property_counts[hex_to_property[codepoint]]++;
}
    
// return size (stored per data object in private vars)
int P::size() const{ 
    return unique_properties.size();
}

bool P::empty() const {
	return (unique_properties.size() == 0);
}

void P::clear() {
	clear_properties();
}

void P::clear_properties(){
    unique_properties.clear();
    hex_to_property.clear();
    property_counts.clear();
	properties_file_name = "";
}		