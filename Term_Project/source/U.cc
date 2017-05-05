// Josh Mau
// Mar 30, 2017
// U.cc -- Implementation of U.h methods

#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "U.h"

// BEGIN ctors/dtors ----------------------------
// Default Constructor -- Accumulated String (utf_string) is empty. --------
U::U(){
    utf_file_name = "";
}	
// -------------------------------------------------------------------------

// Copy Constructor -- Copy an existing U object to a new object of type: U. ----------
U::U(const U &rhs) : utf_file_name(rhs.utf_file_name), utf_string(rhs.utf_string),
    codepoint_map(rhs.codepoint_map), utf_index(rhs.utf_index){
    // Nothing left to do...
}
// ------------------------------------------------------------------------------------

// Test Constructor -- Takes a literal string. --------------
U::U(std::string s){
    read_string(s);
}
// ----------------------------------------------------------

// Destructor -- No dynamic data
U::~U(){}
// END ctors/dtors --------------------------------------

// BEGIN Operators ---------------------------------------
// Assignment Operator (assign another U) --------------
U &U::operator=(const U &rhs) {
    utf_file_name = rhs.utf_file_name;
    utf_string = rhs.utf_string;
    utf_index = rhs.utf_index;
    return *this;
}
// -----------------------------------------------------

// Assignment Operator (Assign a string to a U) --------
U &U::operator=(const std::string &s){
	clear();
	append(s);
	return *this;
}
// -----------------------------------------------------
		
// Concatenation Operators (U + U) ---------------------
U U::operator+(const U &u)const{
	U other(*this);
	int u_index = 0;
	// utf_index is automcatically incremeneted for the next available slot (0 based)
	for ( int i = other.utf_index; i < other.utf_index + u.utf_index ; ++i){
		other.utf_string[i] = u.utf_string.find(u_index)->second;
		other.codepoint_map[i] = u.codepoint_map.find(u_index)->second;
		++u_index;
	}
	other.utf_index += u.utf_index;
	return other;
}
// -----------------------------------------------------

// Concatenation Operators (U + S) ---------------------
U U::operator+(const std::string &s)const{
	U other(*this);
	other.append(s);
	return other;
}
// -----------------------------------------------------

// Concatenation Operators (S + U) ---------------------
U operator+(const std::string &s, const U &u){
	U other(s);
	other += u;
	return other;
}
// -----------------------------------------------------

// Append Operators (U += s) ---------------------------
U &U::operator+=(const std::string &s){
	append(s);
	return *this;
}
// -----------------------------------------------------

// Append Operators (U += u) ---------------------------
U &U::operator+=(const U &u){
	int u_index = 0;
	// utf_index is automcatically incremeneted for the next available slot (0 based)
	for ( int i = utf_index; i < utf_index + u.utf_index ; ++i){
		utf_string[i] = u.utf_string.find(u_index)->second;
		codepoint_map[i] = u.codepoint_map.find(u_index)->second;
		++u_index;
	}
	utf_index += u.utf_index;
	return *this;
}
// -----------------------------------------------------

// Subscripting Operator -------------------------------
std::string U::operator[](const int i)const{
	return U::get(i);
}
// ------------------------------------------------------

// ostream ---------------------------------------------
std::ostream &operator<<(std::ostream &out, const U &u){
	return out << u.get();
}
// -----------------------------------------------------

// Boolean Evaluation
U::operator bool()const{
	return (utf_index > 0 || utf_file_name != "");
}

// Comparison (U == U) ---------------------
bool U::operator==(const U &u)const{
	
	for (int i = 0 ; i < utf_index ; ++i){
		if (utf_string.find(i)->second != u.utf_string.find(i)->second)
			return false;
	}
	return true;
}
// -----------------------------------------

// Comparison (U == S) ---------------------
bool U::operator==(const std::string &s)const{
	std::string temp = "";
	for (int i = 0 ; i < utf_index ; ++i){
		for (auto c : utf_string.find(i)->second){
			temp += c;
		}
	}
	return !(temp != s);
}
// -----------------------------------------

// Comparison (S == U) ---------------------
bool operator==(const std::string &s, const U &u){
	return (u == s);
}
// -----------------------------------------

// Comparison (U != U) ---------------------
bool U::operator!=(const U &u)const{
	return !(*this == u);
}
// -----------------------------------------

// Comparison (U != S) ---------------------
bool U::operator!=(const std::string &s)const{
	return !(*this == s);
}
// -----------------------------------------

// Comparison (S != U) ---------------------
bool operator!=(const std::string &s, const U &u){
	return !(s == u);
}
// -----------------------------------------
// END Operators ---------------------------------------

// BEGIN methods ---------------------------------------
// Methods of Class 'U' --------------------------------
void U::readfile(const std::string fileName){
    std::ifstream readfile(fileName);
    std::string line = "";			
    
    if (!readfile)
        throw std::string("File \"" + fileName + "\" could not be opened!");
    
    while(std::getline(readfile, line)){
        read_string(line);
        if ( !readfile.eof()){
            utf_string[utf_index].push_back('\n');
            codepoint_map[utf_index] = '\n';
            utf_index++;
        }
    }
}
        
void U::read_string(const std::string line){
    int i = 0;
    int flag = 0;
    for ( char c : line ){
        if (flag > 0){
            // If the next bit doesn't begin with 10xxxxxx error out
            if ( ((c&0x000000FF)&0xC0) != 0x80){
                throw std::string("Error in byte" + std::to_string(c));
            }
            flag--;     // decrement flag
            i++;        // incrememnt current index of tempString
            continue;   // skip byte (already been read) 
        }
        
        // For Range U+0000 - U+007F
        if (c >= 0x0000 && c <= 0x007F){
            codepoint_map[utf_index] = c;
            utf_string[utf_index].push_back(line[i]);
            utf_index++;
        }else{
            
            // Convert to unsigned int
            unsigned int a = (c&0x000000FF);
            
            // For Range U+0080 - U+07FF
            if ((a&0xE0) == 0xC0){
                flag = 1;                                 // Skip next byte
                a &= 0x1F;                                // Remove first 3 bits 110xxxxx
                a <<= 6;                                  // Shift left 6 for next byte
                a |= ((line[i+1]&(0x000000FF)) & 0x3F );  // Remove first two bits (10xxxxxx) of next unicode character and OR it with currenct value
                codepoint_map[utf_index] = a;
                utf_string[utf_index].push_back(line[i]);
                utf_string[utf_index].push_back(line[i+1]);
                utf_index++;
            }
            // For Range U+0800 = U+FFFF
            else if ((a&0xF0) == 0xE0){
                flag = 2;                                 // Skip next two bytes
                a &= 0x0F;                                // Remove first 4 bits 1110xxxx
                a <<= 6;                                  // Shift left 6 for next byte
                a |= ((line[i+1]&(0x000000FF)) & 0x3F );  // Remove first two bits (10xxxxxx) of next unicode character and OR it with currenct value
                a <<= 6;                                  // Shift left 6 for next byte
                a |= ((line[i+2]&(0x000000FF)) & 0x3F );  // Remove first two bits (10xxxxxx) of next unicode character and OR it with currenct value
                codepoint_map[utf_index] = a;
                utf_string[utf_index].push_back(line[i]);
                utf_string[utf_index].push_back(line[i+1]);
                utf_string[utf_index].push_back(line[i+2]);
                utf_index++;
            }
            
            // For Range U+10000 - U+1FFFFF
            else if ((a&0xF8) == 0xF0){
                flag = 3;                                 // Skip next three bytes
                a &= 0x07;                                // Remove first five bits 11110xxx
                a <<= 6;                                  // Shift left 6 for next byte
                a |= ((line[i+1]&(0x000000FF)) & 0x3F );  // Remove first two bits (10xxxxxx) of next unicode character and OR it with currenct value
                a <<= 6;                                  // Shift left 6 for next byte
                a |= ((line[i+2]&(0x000000FF)) & 0x3F );  // Remove first two bits (10xxxxxx) of next unicode character and OR it with currenct value
                a <<= 6;                                  // Shift left 6 for next byte
                a |= ((line[i+3]&(0x000000FF)) & 0x3F );  // Remove first two bits (10xxxxxx) of next unicode character and OR it with currenct value
                codepoint_map[utf_index] = a;
                utf_string[utf_index].push_back(line[i]);
                utf_string[utf_index].push_back(line[i+1]);
                utf_string[utf_index].push_back(line[i+2]);
                utf_string[utf_index].push_back(line[i+3]);
                utf_index++;
            }
            else {
				throw std::string("Error in byte" + std::to_string(a));
			}
        }
        i++;    // Increment Index Counter
    }
}

void U::append(const std::string line){
    read_string(line);
}

std::string U::get()const{
    std::string accum_string = "";
    for (int i = 0 ; i <= size()-1 ; i++){		// size - 1 for 0-base indexing
        accum_string += get(i);
    }
    return accum_string;
}

std::string U::get(const int index)const{
    if (index >= utf_index)
        throw std::string("Index \"" + std::to_string(index) + "\" is out of bounds! Method: get(int)");
    std::string str;
    auto itor = utf_string.find(index);
	if (itor != utf_string.end()){
		for (auto c : itor->second){
			str += c;
		}
	}
    return str;
}

std::string U::get(const int start, const int end)const{
    if (start >= end)
        throw std::string("Invalid half-open interval!");

    std::string interval_string = "";
    for (int i = start ; i < end ; i++)
        interval_string += get(i);
    return interval_string;
}
            
int U::codepoint(const int index)const{
	//if ((unsigned int)index > codepoint_map.size())
	//	throw std::string("Index \"" + std::to_string(index) + "\" is out of bounds! Method: codepoint");
    auto itor = codepoint_map.find(index);
	if(itor != codepoint_map.end()){
		return itor->second;
	}
	return -1;
}

int U::size() const{ 
    return utf_index;
}

bool U::empty()const{
    return (utf_index == 0);
}

void U::clear(){
    utf_string.clear();
	codepoint_map.clear();
	utf_index = 0;
	utf_file_name = "";
}
// END methods of class U ------------------------------