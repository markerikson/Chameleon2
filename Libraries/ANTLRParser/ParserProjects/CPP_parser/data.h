#pragma once

class data
{
public:
	data()
		:m_pData(NULL), m_nLength(0) {	}

	virtual ~data()
	{
		if(m_pData != NULL)
			delete [] m_pData;
		m_pData = NULL;
	}

	data(const char* pVal)
		:m_pData(NULL), m_nLength(0)
	{
		if(pVal == NULL)
			return;

		m_nLength = strlen(pVal) + 1;
		m_pData = new char[m_nLength];
		memset(m_pData, 0, m_nLength);
		strcpy(m_pData, pVal);
	}

	data(const data& another)
		:m_pData(NULL),m_nLength(0)
	{
		if(another.m_pData == NULL || another.m_nLength == 0)
			return;

		m_nLength = another.m_nLength;
		m_pData = new char[m_nLength];
		memcpy(m_pData, another.m_pData, m_nLength);
	}

public:
	data& operator=(const data& another)
	{
		if(another.m_pData == NULL || another.m_nLength == 0)
		{
			m_pData = NULL;
			m_nLength = 0;
		}
		else
		{
			m_nLength = another.m_nLength;
			m_pData = new char[m_nLength];
			memcpy(m_pData, another.m_pData, m_nLength);
		}

		return *this;
	}

	bool empty()
	{
		return (m_pData == NULL) || m_pData[0] == 0;
	}

	char* c_str()
	{
		return m_pData;
	}

private:
	char* m_pData;
	int m_nLength;
};
