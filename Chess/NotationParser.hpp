#ifndef __NotationParserHPP__
#define __NotationParserHPP__

#include <string>
#include <queue>
#include <Chess/John/JohnDefinitions.hpp>

class NotationParser
{
    public:
        void setString(const std::string& String);
        void setBoard(game g);//Yes, copy it in.
        int parse();
        inline std::string getErrorString() const {return m_ErrorString;}

        std::queue<move> getMoves() const {return m_MoveQueue;}

    private:
        std::string m_RawString;
        std::string m_ErrorString;
        std::queue<std::string> m_RawQueue;
        std::queue<move> m_MoveQueue;
        game m_Board;
};


#endif
