#ifndef SSTRING_H_
#define SSTRING_H_

#include<iostream>
#include<vector>
#include<assert.h>

class SChar
{
public:
	SChar() = default;
	inline SChar(char ch){m_char[0] = ch;}
	inline char& operator[](int index)
	{
		assert(0 <= index && index < 4);
		return m_char[index];
	}
	inline char operator[](int index) const
	{
		assert(0 <= index && index < 4);
		return m_char[index];
	}
	inline std::string toString()const
	{
		std::string result;
		if ((m_char[0] & 0xF0) == 0xF0) {
			result.append({ m_char[0], m_char[1], m_char[2], m_char[3] });
		}
		else if ((m_char[0] & 0xE0) == 0xE0) {
			result.append({ m_char[0] , m_char[1] , m_char[2] });
		}
		else if ((m_char[0] & 0xC0) == 0xC0) {
			result.append({ m_char[0] , m_char[1] });
		}
		else if (m_char[0] >= 0) {
			result.append({ m_char[0] });
		}
		return result;
	}

	friend std::ostream& operator<<(std::ostream& out, const SChar& c);
private:
	char m_char[4]{ 0 };
};

class SString
{
public:
	using iterator = std::vector<SChar>::iterator;
	using const_iterator = std::vector<SChar>::const_iterator;

	inline SString() = default;
	inline SString(char ch)
	{
		m_str.push_back(SChar(ch));
	}
	inline SString(const SChar& ch)
	{
		m_str.push_back(ch);
	}
	inline SString(const char* str)
	{
		push_back(str);
	}
	inline SString(const std::string& str)
	{
		push_back(str.c_str());
	}

	inline void push_back(const SString& s)
	{
		push_back(s.toString().c_str());
	}
	void push_back(const char* s);
	inline SString::iterator insert(SString::iterator where, const SChar& val) 
	{ return m_str.insert(where, val); };
	inline SString::iterator insert(SString::iterator where, SString::iterator first,SString::iterator last) 
	{ return m_str.insert(where, first,last); };
	inline SString::iterator insert(SString::iterator where, const char* s)
	{
		SString str(s);
		return m_str.insert(where, str.begin(),str.end());
	};
	inline SString::iterator insert(SString::iterator where, const SString& str)
	{
		return m_str.insert(where, str.begin(), str.end());
	};


	std::string toString()const;

	static std::string CharToString(const SChar& c);

	inline const_iterator begin()const { return m_str.begin(); }
	inline const_iterator end()const { return m_str.end(); }
	inline iterator begin() { return m_str.begin(); }
	inline iterator end() { return m_str.end(); }

	inline SChar& back() { return m_str.back(); }
	inline void pop_back() { m_str.pop_back(); }
	inline SChar& operator[](int index) { assert(0 <= index && index < size()); return m_str[index]; }
	inline SChar operator[](int index)const { assert(0 <= index && index < size()); return m_str[index];}

	inline iterator erase(int pos)
	{
		assert(0 <= pos && pos < size());
		return m_str.erase(m_str.begin() + pos);
	}

	inline void clear() { m_str.clear(); }
	inline bool empty()const { return m_str.empty(); }
	inline int size()const { return m_str.size(); }

	friend std::ostream& operator<<(std::ostream& out, const SString& s);

	inline SString operator+(const SString& s) const {
		SString result(*this);
		result.push_back(s);
		return result;
	}
	inline SString operator+(const char* s) const {
		SString result(*this);
		result.push_back(s);
		return result;
	}
	inline SString operator+(const std::string& s) const {
		SString result(*this);
		result.push_back(s);
		return result;
	}
	inline SString& operator+=(const char* s) {
		this->push_back(s);
		return *this;
	}
	inline SString& operator+=(const std::string& s) {
		this->push_back(s);
		return *this;
	}
	inline SString& operator+=(const SString& s) {
		this->push_back(s);
		return *this;
	}
	inline SString& operator=(const char* s) {
		clear();
		push_back(s);
		return *this;
	}
	inline SString& operator=(const std::string& s) {
		clear();
		push_back(s);
		return *this;
	}

private:
	std::vector<SChar> m_str;
};

#endif // !SSTRING_H_