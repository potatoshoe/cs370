// Josh Mau
// Mar 30, 2017
// U.h -- Header file for Object Oriented
//		Read and categorize UTF-8 characters
// 		from multiple files.

#ifndef U_H_INCLUDED
#define U_H_INCLUDED

#include <string>
#include <vector>
#include <set>
#include <map>

class U {
	public:
		// Constructors and Destructor -------------------------
		U();                      // Default Constructor
		U(const U &);             // Copy Constructor
		U(std::string);           // Test Constructor (Takes a string literal)
		~U();                     // Destructor
		
		// BEGIN Operators: ----------------------------------------------------
		// Assignment Operators
		U &operator=(const U &);  		// Assigns this to a U::U)
		U &operator=(const std::string &); 	// Assigns this to a std::string
		
		// Concatenation Operators
		U operator+(const U &)const;
		U operator+(const std::string &)const;
		friend U operator+(const std::string &, const U &);
		
		// Append Operators
		U &operator+=(const std::string &);
		U &operator+=(const U &);
		
		// Subscripting Operator
		std::string operator[](const int)const;
		
		// ostream
		friend std::ostream &operator<<(std::ostream &, const U &);
		
		// Boolean Evaluation
		operator bool()const;
		
		// Comparison 
		bool operator==(const U &)const;
		bool operator==(const std::string &)const;
		friend bool operator==(const std::string &, const U &);
		bool operator!=(const U &)const;
		bool operator!=(const std::string &)const;
		friend bool operator!=(const std::string &, const U &);
		
		
		// END OPERATORS --------------------------------------------------------
		
		// Methods of Class 'U' --------------------------------
		void readfile(const std::string);         // Calls read_string on each line of a file
        void read_string(const std::string);      // Parses UTF-8 characters from string
		void append(const std::string);           // Calls read_string for a passed string (append to utf_string)
    
		std::string get()const;                        // Returns full utf_string
		std::string get(const int)const;               // Returns UTF-8 character at index of utf_string
		std::string get(const int, const int)const;    // Returns half open interval of UTF-8 characters
		
        int codepoint(const int)const;                // Returns the codepoint of the UTF-8 character at index
        
		int size() const;                         // Total size of utf_string (utf_index)
    
        bool empty()const;                             // QUESTION: is the utf_string empty?
        void clear();                             // CAUTION: Clears entire utf_string
        
    private:
        std::string utf_file_name;
		std::map<int, std::vector<int>> utf_string;
        std::map<int, int> codepoint_map;
        int utf_index = 0;
    
};

#endif
