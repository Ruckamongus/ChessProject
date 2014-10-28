#include <Chess/GUIManager.hpp>
#include <Chess/Board.hpp>
#include <Phox/Internal/Exception.hpp>

void GUIManager::init(sf::RenderWindow& Window)
{
    m_GUI.setWindow(Window);
    m_GUI.setGlobalFont("Resources\\DejaVuSans.ttf");

    m_OutputBox = tgui::TextBox::create("Resources\\Black.conf");
    m_OutputBox->setPosition(544, 34 + 32);
    m_OutputBox->setSize(240, 180);
    m_OutputBox->setTextSize(11);
    m_OutputBox->setReadOnly(1);
    m_GUI.add(m_OutputBox, "OutputBox");

    m_InputBox = tgui::TextBox::create("Resources\\Black.conf");
    m_InputBox->setPosition(544, 250 + 32);
    m_InputBox->setSize(240, 180);
    m_InputBox->setTextSize(11);
    m_InputBox->setReadOnly(0);
    m_GUI.add(m_InputBox, "InputBox");

    m_TimeBox = tgui::EditBox::create("Resources\\Black.conf");
    m_TimeBox->setSize(244, 24);
    m_TimeBox->setPosition(541, 454 + 32);
    m_TimeBox->setNumbersOnly(1);
    m_TimeBox->setText("10");
    m_GUI.add(m_TimeBox, "TimeBox");

    m_RunInput = tgui::Button::create("Resources\\Black.conf");
    m_RunInput->setSize(128, 22);
    m_RunInput->setPosition(658, 486 + 32);
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

    m_NetworkChatbox = tgui::TextBox::create("Resources\\Black.conf");
    m_NetworkChatbox->setSize(240, 180);
    m_NetworkChatbox->setPosition(816, 34 + 32);
    m_NetworkChatbox->setTextSize(11);
    m_NetworkChatbox->setReadOnly(1);
    m_GUI.add(m_NetworkChatbox, "NetworkChatbox");

    m_NetworkUser = tgui::EditBox::create("Resources\\Black.conf");
    m_NetworkUser->setSize(244, 24);
    m_NetworkUser->setPosition(816, 282);
    m_NetworkUser->setText("User");
    m_GUI.add(m_NetworkUser, "NetworkUser");

    m_NetworkIP = tgui::EditBox::create("Resources\\Black.conf");
    m_NetworkIP->setSize(244, 24);
    m_NetworkIP->setPosition(816, 328);
    m_NetworkIP->setText("127.0.0.1");
    m_GUI.add(m_NetworkIP, "NetworkIP");

    m_NetworkPort = tgui::EditBox::create("Resources\\Black.conf");
    m_NetworkPort->setSize(244, 24);
    m_NetworkPort->setPosition(816, 374);
    m_NetworkPort->setText("22222");
    m_GUI.add(m_NetworkPort, "NetworkPort");

    m_NetworkConnect = tgui::Button::create("Resources\\Black.conf");
    m_NetworkConnect->setSize(114, 22);
    m_NetworkConnect->setText("Connect");
    m_NetworkConnect->setPosition(816, 414 - 10);
    m_NetworkConnect->connect("pressed", [&]
                        {
                            m_NetworkSignal.clear();
                            m_NetworkSignal << "Connect";
                            m_NetworkSignal << m_NetworkUser->getText();
                            m_NetworkSignal << m_NetworkIP->getText();
                            m_NetworkSignal << m_NetworkPort->getText();
                        }
                       );
    m_GUI.add(m_NetworkConnect, "NetworkConnect");

    m_NetworkHost = tgui::Button::create("Resources\\Black.conf");
    m_NetworkHost->setSize(114, 22);
    m_NetworkHost->setText("Host");
    m_NetworkHost->setPosition(946, 414 - 10);
    m_NetworkHost->connect("pressed", [&]
                        {
                            m_NetworkSignal.clear();
                            m_NetworkSignal << "Host";
                            m_NetworkSignal << m_NetworkPort->getText();
                        }
                       );
    m_GUI.add(m_NetworkHost, "NetworkHost");

    m_NetworkForfeit = tgui::Button::create("Resources\\Black.conf");
    m_NetworkForfeit->setSize(114, 22);
    m_NetworkForfeit->setText("Forfeit");
    m_NetworkForfeit->setPosition(816, 448 - 10);
    m_NetworkForfeit->connect("pressed", [&]
                        {
                            m_NetworkSignal.clear();
                            m_NetworkSignal << "Forfeit";
                        }
                       );
    m_GUI.add(m_NetworkForfeit, "NetworkForfeit");

    m_NetworkRematch = tgui::Button::create("Resources\\Black.conf");
    m_NetworkRematch->setSize(114, 22);
    m_NetworkRematch->setText("Rematch");
    m_NetworkRematch->setPosition(946, 448 - 10);
    m_NetworkRematch->connect("pressed", [&]
                        {
                            m_NetworkSignal.clear();
                            m_NetworkSignal << "Rematch";
                        }
                       );
    m_GUI.add(m_NetworkRematch, "NetworkRematch");

    m_NetworkChat = tgui::EditBox::create("Resources\\Black.conf");
    m_NetworkChat->setSize(244, 24);
    m_NetworkChat->setPosition(816, 454 + 32);
    m_NetworkChat->setText("");
    m_NetworkChat->connect("ReturnKeyPressed", [&]
                              {
                                m_NetworkChat->setText("");
                                m_NetworkSignal.clear();
                                m_NetworkSignal << "Chat";
                                m_NetworkSignal << (static_cast <std::string> (m_NetworkChat->getText()));
                              }
                            );
    m_GUI.add(m_NetworkChat, "NetworkChat");
}

void GUIManager::addText(const std::string& Text)
{
    m_OutputBox->addText(Text);
}

void GUIManager::draw()
{
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

std::size_t GUIManager::getNewGame()
{
    if (m_NewGame)
    {
        std::size_t Return = m_NewGame;
        m_NewGame = 0;
        return Return;
    }
    return 0;
}

Phox::cStreamBuffer GUIManager::getNetworkSignal()
{
    auto Return = m_NetworkSignal;
    m_NetworkSignal.clear();
    return Return;
}

void GUIManager::handleSignal(Phox::cStreamBuffer Signal)
{
    if (!Signal.getWorkingBytes()) return;//Nothing to do here
    std::string Sig;

    Signal >> Sig;

    if (Sig == "Error")
    {
        m_NetworkChatbox->addText(Signal.readString());
    }

    else
    {
        m_NetworkChatbox->addText(Sig);
    }
}
