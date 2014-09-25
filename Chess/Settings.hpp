#ifndef __SettingsH__
#define __SettingsH__
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
//Modified from SFML wiki
//License is Public Domain

class SettingsParser
{
    public:
        SettingsParser() = default;
        ~SettingsParser();//I should follow the rule of 5... but GCC can do it for me.
        bool loadFile(const std::string& Filename);
        bool saveToFile();
        bool isChanged() const;

        void get(const std::string& Parameter, std::string& Value) const;
        void get(const std::string& Parameter, bool& Value) const;
        void get(const std::string& Parameter, char& Value) const;
        void get(const std::string& Parameter, int& Value) const;
        void get(const std::string& Parameter, float& Value) const;
        const std::string get(const std::string& Parameter) const;

        void set(const std::string& Parameter, std::string Value);
        void set(const std::string& Parameter, bool Value);
        void set(const std::string& Parameter, char Value);
        void set(const std::string& Parameter, int Value);
        void set(const std::string& Parameter, float Value);


    private:
        int findIndex(const std::string& Parameter) const;
        bool read();
        bool write() const;

        bool m_isChanged;
        std::string m_Filename;
        std::vector<std::pair<std::string, std::string>> m_Data;
};

#endif
