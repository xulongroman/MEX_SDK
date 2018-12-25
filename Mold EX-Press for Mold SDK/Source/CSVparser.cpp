#include <fstream>
#include <sstream>
#include <iomanip>
#include <locale>
#include <string>
#include <fstream>
#include <cstdlib>
#include "CSVparser.hpp"
#include "stxutif.h"
using namespace gel;


namespace csv {

	std::wstring g_log = L"";

	void LogError(const std::wstring msg, bool bInit = false)
	{
		std::wofstream logfile;
		if (bInit)
		{
			g_log = msg;
			logfile.open(g_log.c_str(), std::ios::out|std::ios::trunc);
		}
		else
		{
			logfile.open(g_log.c_str(), std::ios::out|std::ios::app);
			if (logfile.is_open()) 
			{
				logfile << msg << std::endl;
				logfile.close();
			}
		}
	}

  Parser::Parser(const std::wstring &data, const DataType &type, wchar_t sep)
    : _type(type), _sep(sep)
  {
      std::wstring line;
      if (type == eFILE)
      {
        _file = data;
		LogError(_file + std::wstring(L".log"), true);
        std::wifstream ifile(_file.c_str());
		ifile.imbue(stdx::utf8_locale);
        if (ifile.is_open())
        {
            while (ifile.good())
            {
                getline(ifile, line);
                if (line != L"")
                    _originalFile.push_back(line);
            }
            ifile.close();

            if (_originalFile.size() == 0)
			{
				LogError(std::wstring(L"No Data in ").append(_file));
				return;
			}

            parseHeader();
            parseContent();
        }
		else
		{
			LogError(std::wstring(L"Failed to open ").append(_file));
		}
      }
      else
      {
        std::wistringstream stream(data);
		stream.imbue(stdx::utf8_locale);
        while (std::getline(stream, line))
          if (line != L"")
            _originalFile.push_back(line);
		if (_originalFile.size() == 0)
	        LogError(std::wstring(L"No Data in pure content"));

        parseHeader();
        parseContent();
      }
  }

  Parser::~Parser(void)
  {
     std::vector<Row *>::iterator it;

     for (it = _content.begin(); it != _content.end(); it++)
          delete *it;
  }

  void Parser::parseHeader(void)
  {
	  //HeaderElement may be ""
//       std::stringstream ss(_originalFile[0]);
//       std::string item;
// 
//       while (std::getline(ss, item, _sep))
//           _header.push_back(item);

	  std::vector<std::wstring>::iterator it;
	  bool quoted = false;
	  int tokenStart = 0;
	  unsigned int i = 0;

	  it = _originalFile.begin();

	  for (; i != it->length(); i++)
	  {
		  if (it->at(i) == L'"')
			  quoted = ((quoted) ? (false) : (true));
		  else if (it->at(i) == L',' && !quoted)
		  {
			  _header.push_back(it->substr(tokenStart, i - tokenStart));
			  tokenStart = i + 1;
		  }
	  }

	  //end
	  _header.push_back(it->substr(tokenStart, it->length() - tokenStart));
  }

  void Parser::parseContent(void)
  {
     std::vector<std::wstring>::iterator it;
     
     it = _originalFile.begin();
     it++; // skip header

	 int count = 0;
     for (; it != _originalFile.end(); it++)
     {	 
         bool quoted = false;
         int tokenStart = 0;
         unsigned int i = 0;

         Row *row = new Row(_header);

         for (; i != it->length(); i++)
         {
              if (it->at(i) == L'"')
                  quoted = ((quoted) ? (false) : (true));
              else if (it->at(i) == L',' && !quoted)
              {
                  row->push(it->substr(tokenStart, i - tokenStart));
                  tokenStart = i + 1;
              }
         }

         //end
         row->push(it->substr(tokenStart, it->length() - tokenStart));

         // if value(s) missing
         if (row->size() != _header.size())
		 {
			 wchar_t number[256];
			 _itow(count+1, number, 10);
			 LogError(std::wstring(L"Corrupted Data in No ").append(std::wstring(number)));
			 return;
		 }
         _content.push_back(row);

		 count++;
     }
  }

  Row &Parser::getRow(unsigned int rowPosition) const
  {
      if (rowPosition < _content.size())
          return *(_content[rowPosition]);
  }

  Row &Parser::operator[](unsigned int rowPosition) const
  {
      return Parser::getRow(rowPosition);
  }

  unsigned int Parser::rowCount(void) const
  {
      return _content.size();
  }

  unsigned int Parser::columnCount(void) const
  {
      return _header.size();
  }

  std::vector<std::wstring> Parser::getHeader(void) const
  {
      return _header;
  }

  const std::wstring Parser::getHeaderElement(unsigned int pos) const
  {
	  if (pos < _header.size())
		  return _header[pos];
  }

  const std::wstring &Parser::getFileName(void) const
  {
      return _file;    
  }
  
  /*
  ** ROW
  */

  Row::Row(const std::vector<std::wstring> &header)
      : _header(header) {}

  Row::~Row(void) {}

  unsigned int Row::size(void) const
  {
    return _values.size();
  }

  void Row::push(const std::wstring &value)
  {
    _values.push_back(value);
  }

  bool Row::set(const std::wstring &key, const std::wstring &value) 
  {
    std::vector<std::wstring>::const_iterator it;
    int pos = 0;

    for (it = _header.begin(); it != _header.end(); it++)
    {
        if (key == *it)
        {
          _values[pos] = value;
          return true;
        }
        pos++;
    }
    return false;
  }

  const std::wstring Row::operator[](unsigned int valuePosition) const
  {
       if (valuePosition < _values.size())
           return _values[valuePosition];
  }

  const std::wstring Row::operator[](const std::wstring &key) const
  {
      std::vector<std::wstring>::const_iterator it;
      int pos = 0;

      for (it = _header.begin(); it != _header.end(); it++)
      {
          if (key == *it)
              return _values[pos];
          pos++;
      }
  }

  std::wostream &operator<<(std::wostream &os, const Row &row)
  {
      for (unsigned int i = 0; i != row._values.size(); i++)
          os << row._values[i] << L" | ";

      return os;
  }

  std::wofstream &operator<<(std::wofstream &os, const Row &row)
  {
    for (unsigned int i = 0; i != row._values.size(); i++)
    {
        os << row._values[i];
        if (i < row._values.size() - 1)
          os << L",";
    }
    return os;
  }
}
