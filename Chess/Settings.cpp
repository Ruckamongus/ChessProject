#include <Chess/Settings.hpp>
#include <Phox/Utilities/Conversions.hpp>

SettingsParser::~SettingsParser()
{
    saveToFile();
}


bool SettingsParser::loadFile(const std::string& Filename)
{
    m_Data.clear();
    m_Filename = Filename;
    return read();
}


bool SettingsParser::saveToFile()
{
    if (m_isChanged)
    {
        m_isChanged = write();
        return m_isChanged;
    }
    return 1;
}


bool SettingsParser::read()
{
    std::ifstream in(m_Filename);
    if (!in.is_open())
    {
        //std::cerr << "[Error] Unable to open settings file: \"" << m_Filename << "\" for reading. Using default Values." << std::endl;
        return 0;
    }
    std::string line, Parameter, Value;
    while(!in.eof())
    {
        std::getline(in, line);
        if (line.size() > 0 && line[0] != '#')
        {
            std::size_t j = 0;
            std::size_t length = line.size();
            while (line[j] != ' ') j++;
            Parameter = line.substr(0,j);
            while (line[j] == ' ' || line[j] == '=') j++;
            int a = j;
            while(j < length && (line[j] != ' ' || line[j] != '\n')) j++;
            int b = j;
            Value = line.substr(a, b);
        }
        else
        {
            Parameter = line;
            Value = "";
        }
        m_Data.emplace_back(std::make_pair(Parameter, Value));
    }
    in.close();
    m_isChanged = 0;
    return 1;
}


bool SettingsParser::write() const
{
    std::ofstream out(m_Filename);
    if (!out.is_open())
    {
        //std::cerr << "[Error] Unable to open settings file: \"" << m_Filename << "\" for writing." << std::endl;
        return 0;
    }
    for (std::size_t i = 0; i < m_Data.size(); ++i)
    {
        if (m_Data[i].first[0] == '#' || m_Data[i].first[0] == 0)
            out << m_Data[i].first << std::endl;
        else
            out << m_Data[i].first << " = " << m_Data[i].second << std::endl << std::endl;
    }

    out.close();
    return 1;
}


bool SettingsParser::isChanged() const
{
    return m_isChanged;
}


int SettingsParser::findIndex(const std::string& Parameter) const
{
    for (std::size_t i = 0; i < m_Data.size(); ++i)
    {
        if (m_Data[i].first[0] == '#') continue;
        if (m_Data[i].first == Parameter)
        {
            return i;
        }
    }
    return -1;
}


void SettingsParser::get(const std::string& Parameter, std::string& Value) const
{
    int i = findIndex(Parameter);
    if (i > -1)
    {
        Value = m_Data[i].second;
    }
}


void SettingsParser::get(const std::string& Parameter, bool& Value) const
{
    int i = findIndex(Parameter);
    if (i > -1)
    {
        Value = ((m_Data[i].second == "1") ? 1 : 0);
    }
}

const std::string SettingsParser::get(const std::string& Parameter) const
{
    int i = findIndex(Parameter);
    if (i > -1)
    {
        return m_Data[i].second;
    }

    return "";
}

void SettingsParser::get(const std::string& Parameter, char& Value) const
{
    int i = findIndex(Parameter);
    if (i > -1)
    {
        Value = m_Data[i].second[0];
    }
}


void SettingsParser::get(const std::string& Parameter, int& Value) const
{
    int i = findIndex(Parameter);
    if (i > -1)
    {
        std::stringstream ss(m_Data[i].second);
        ss >> Value;
    }
}


void SettingsParser::get(const std::string& Parameter, float& Value) const
{
    int i = findIndex(Parameter);
    if (i > -1)
    {
        std::stringstream ss(m_Data[i].second);
        ss >> Value;
    }
}


void SettingsParser::set(const std::string& Parameter, std::string Value)
{
    int i = findIndex(Parameter);
    if (i > -1)
    {
        m_Data[i].second = Value;
    }

    else m_Data.emplace_back(std::make_pair(Parameter, Value));

    m_isChanged = 1;
}


void SettingsParser::set(const std::string& Parameter, bool Value)
{
    int i = findIndex(Parameter);
    if (i > -1)
    {
        m_Data[i].second = (Value) ? "1" : "0";
    }

    else m_Data.emplace_back(std::make_pair(Parameter, Phox::toString(Value)));

    m_isChanged = 1;
}


void SettingsParser::set(const std::string& Parameter, char Value)
{
    int i = findIndex(Parameter);
    if (i > -1)
    {
        std::string tmp;
        tmp = Value;
        m_Data[i].second = tmp;
    }

    else m_Data.emplace_back(std::make_pair(Parameter, Phox::toString(Value)));

    m_isChanged = 1;
}


void SettingsParser::set(const std::string& Parameter, int Value)
{
    int i = findIndex(Parameter);
    if (i > -1)
    {
        std::stringstream ss;
        ss << Value;
        m_Data[i].second = ss.str();
    }

    else m_Data.emplace_back(std::make_pair(Parameter, Phox::toString(Value)));

    m_isChanged = 1;
}


void SettingsParser::set(const std::string& Parameter, float Value)
{
    int i = findIndex(Parameter);
    if (i > -1)
    {
        std::stringstream ss;
        ss << Value;
        m_Data[i].second = ss.str();
    }

    else m_Data.emplace_back(std::make_pair(Parameter, Phox::toString(Value)));

    m_isChanged = 1;
}
