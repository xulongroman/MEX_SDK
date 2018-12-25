#ifndef     _CSVPARSER_HPP_
# define    _CSVPARSER_HPP_

#include <stdexcept>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <iostream>
#include <locale>
#include <fstream>

namespace csv
{
    class Row
    {
    	public:
    	    Row(const std::vector<std::wstring> &);
    	    ~Row(void);

    	public:
            unsigned int size(void) const;
            void push(const std::wstring &);
            bool set(const std::wstring &, const std::wstring &); 

    	private:
    		const std::vector<std::wstring> _header;
    		std::vector<std::wstring> _values;

        public:

            template<typename T>
            const T getValue(unsigned int pos) const
            {
                if (pos < _values.size())
                {
                    T res;
                    std::wstringstream ss;
                    ss << _values[pos];
                    ss >> res;
                    return res;
                }
                throw Error("can't return this value (doesn't exist)");
            }
            const std::wstring operator[](unsigned int) const;
            const std::wstring operator[](const std::wstring &valueName) const;
            friend std::wostream& operator<<(std::wostream& os, const Row &row);
            friend std::wofstream& operator<<(std::wofstream& os, const Row &row);
    };

    enum DataType {
        eFILE = 0,
        ePURE = 1
    };

    class Parser
    {

    public:
        Parser(const std::wstring &, const DataType &type = eFILE, wchar_t sep = ',');
        ~Parser(void);

    public:
        Row &getRow(unsigned int row) const;
        unsigned int rowCount(void) const;
        unsigned int columnCount(void) const;
        std::vector<std::wstring> getHeader(void) const;
        const std::wstring getHeaderElement(unsigned int pos) const;
        const std::wstring &getFileName(void) const;


    protected:
    	void parseHeader(void);
    	void parseContent(void);

    private:
        std::wstring _file;
        const DataType _type;
        const wchar_t _sep;
        std::vector<std::wstring> _originalFile;
        std::vector<std::wstring> _header;
        std::vector<Row *> _content;

    public:
        Row &operator[](unsigned int row) const;
    };
}

#endif /*!_CSVPARSER_HPP_*/
