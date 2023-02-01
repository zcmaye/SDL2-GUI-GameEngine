#include "SString.h"
/*@SChar*/
std::ostream& operator<<(std::ostream& out, const SChar& c)
{
	if ((c[0] & 0xF0) == 0xF0) {
		out << c[0] << c[1] << c[2] << c[3];
	}
	else if ((c[0] & 0xE0) == 0xE0) {
		out << c[0] << c[1] << c[2];
	}
	else if ((c[0] & 0xC0) == 0xC0) {
		out << c[0] << c[1];
	}
	else if (c[0] >= 0) {
		out << c[0];
	}
	return out;
}
/*@SString*/
std::ostream& operator<<(std::ostream& out, const SString& s)
{
	for (const SChar& c : s)
	{
		out << c;
	}
	return out;
}

/*@SString*/
void SString::push_back(const char* s)
{
	if (!s)
		return;

	int len = strlen(s);
	if (len == 0)
		return;

	for (int idx = 0; idx < len; )
	{
		SChar c;
		if ((s[idx] & 0xF0) == 0xF0)	//F0 -> 1111 0000
		{
			c[0] = s[idx++];
			c[1] = s[idx++];
			c[2] = s[idx++];
			c[3] = s[idx++];
			m_str.push_back(c);
		}
		else if ((s[idx] & 0xE0) == 0xE0) //E0 -> 11100000
		{
			c[0] = s[idx++];
			c[1] = s[idx++];
			c[2] = s[idx++];
			m_str.push_back(c);
		}
		else if ((s[idx] & 0xC0) == 0xC0) //C0 -> 11100000
		{
			c[0] = s[idx++];
			c[1] = s[idx++];
			m_str.push_back(c);
		}
		else if (s[idx] >= 0)				//ascii
		{
			c[0] = s[idx++];
			m_str.push_back(c);
		}
		else
		{
			idx++;
		}
	}

}

std::string SString::toString() const
{
	std::string result;
	for (auto& c : m_str)
	{
		if ((c[0] & 0xF0) == 0xF0) {
			result.push_back(c[0]);
			result.push_back(c[1]);
			result.push_back(c[2]);
			result.push_back(c[3]);
		}
		else if ((c[0] & 0xE0) == 0xE0) {
			result.push_back(c[0]);
			result.push_back(c[1]);
			result.push_back(c[2]);
		}
		else if ((c[0] & 0xC0) == 0xC0) {
			result.push_back(c[0]);
			result.push_back(c[1]);
		}
		else if (c[0] >= 0) {
			result.push_back(c[0]);
		}
	}
	return result;
}

std::string SString::CharToString(const SChar& c)
{
	std::string result;
	if ((c[0] & 0xF0) == 0xF0) {
		result.push_back(c[0]);
		result.push_back(c[1]);
		result.push_back(c[2]);
		result.push_back(c[3]);
	}
	else if ((c[0] & 0xE0) == 0xE0) {
		result.push_back(c[0]);
		result.push_back(c[1]);
		result.push_back(c[2]);
	}
	else if ((c[0] & 0xC0) == 0xC0) {
		result.push_back(c[0]);
		result.push_back(c[1]);
	}
	else if (c[0] >= 0) {
		result.push_back(c[0]);
	}
	return result;
}
