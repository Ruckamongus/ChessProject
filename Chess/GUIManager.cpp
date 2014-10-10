#include <Chess/GUIManager.hpp>
#include <Chess/Board.hpp>
#include <Phox/Internal/Exception.hpp>

namespace
{
    void p(const std::string& T)
    {
        std::cout << T << '\n';
    }
}

void GUIManager::init(sf::RenderWindow& Window)
{
    m_GUI.setWindow(Window);
    m_GUI.setGlobalFont("Resources\\DejaVuSans.ttf");

    m_OutputBox = tgui::TextBox::create("Resources\\Black.conf");
    m_OutputBox->setPosition(tgui::Layout1d(544), tgui::Layout1d(34));
    m_OutputBox->setSize(240, 180);
    m_OutputBox->setTextSize(11);
    m_OutputBox->setReadOnly(1);
    m_GUI.add(m_OutputBox, "OutputBox");

    m_InputBox = tgui::TextBox::create("Resources\\Black.conf");
    m_InputBox->setPosition(544, 250);
    m_InputBox->setSize(240, 180);
    m_InputBox->setTextSize(11);
    m_InputBox->setReadOnly(0);
    m_GUI.add(m_InputBox, "InputBox");

    m_TimeBox = tgui::EditBox::create("Resources\\Black.conf");
    m_TimeBox->setSize(244, 24);
    m_TimeBox->setPosition(541, 454);
    m_TimeBox->setNumbersOnly(1);
    m_TimeBox->setText("10");
    m_GUI.add(m_TimeBox, "TimeBox");

    m_RunInput = tgui::Button::create("Resources\\Black.conf");
    m_RunInput->setSize(128, 22);
    m_RunInput->setPosition(658, 486);
    m_RunInput->setText("Run input");
    m_RunInput->connect("pressed", [&]
                        {
                            if (m_Board != nullptr)
                            {
                                m_Board->runParser(m_InputBox->getText(), m_TimeBox->getText());
                            }
                        }
                       );
    m_GUI.add(m_RunInput, "RunInput");

    m_EnforceTouchMove = tgui::Checkbox::create("Resources\\Black.conf");
    m_EnforceTouchMove->setPosition(16, 544);
    m_EnforceTouchMove->setText("Enforce Touch-Move");
    m_EnforceTouchMove->setSize(18, 18);
    m_GUI.add(m_EnforceTouchMove, "EnforceTouchMove");

    m_VerboseLogging = tgui::Checkbox::create("Resources\\Black.conf");
    m_VerboseLogging->setPosition(16, 566);
    m_VerboseLogging->setText("Verbose Logging");
    m_VerboseLogging->setSize(18, 18);
    m_GUI.add(m_VerboseLogging, "VerboseLogging");

    m_960Board = tgui::Checkbox::create("Resources\\Black.conf");
    m_960Board->setPosition(528 - 128, 570);
    m_960Board->setText("960 Variant");
    m_960Board->setSize(18, 18);
    m_GUI.add(m_960Board, "960Board");

    m_RenewBoard = tgui::Button::create("Resources\\Black.conf");
    m_RenewBoard->setSize(128, 22);
    m_RenewBoard->setText("New Game");
    m_RenewBoard->setPosition(528 - 128, 544);
    m_RenewBoard->connect("pressed", [&]
                        {
                            m_NewGame = 1 + m_960Board->isChecked();
                        }
                       );
    m_GUI.add(m_RenewBoard, "RenewBoard");
}

void GUIManager::addText(const std::string& Text)
{
    m_OutputBox->addText(Text);
}

void GUIManager::draw()
{
    tgui::Callback callback;
   // while (m_GUI.pollCallback(callback))
    {
        //Just catch changes and tell child widgets
    }
    m_GUI.draw();
}

void GUIManager::update(sf::Event& event)
{
    m_GUI.handleEvent(event);
}

void GUIManager::linkBoard(Board& board)
{
    m_Board = &board;
}

void GUIManager::reset()
{
    m_OutputBox->setText("");
    m_InputBox->setText("");
}

std::size_t  GUIManager::getNewGame()
{
    if (m_NewGame)
    {
        std::size_t Return = m_NewGame;
        m_NewGame = 0;
        return Return;
    }
    return 0;
}
