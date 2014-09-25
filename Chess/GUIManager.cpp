#include <Chess/GUIManager.hpp>
#include <Chess/Board.hpp>

bool GUIManager::init(sf::RenderWindow& Window)
{
    m_GUI.setWindow(Window);
    bool Loaded = m_GUI.setGlobalFont("Resources\\DejaVuSans.ttf");
    if (!Loaded) return -1;

    m_OutputBox->load("Resources\\Black.conf");
    m_OutputBox->setPosition(544, 34);
    m_OutputBox->setSize(240, 180);
    m_OutputBox->setTextSize(11);
    m_OutputBox->setReadOnly(1);

    m_InputBox->load("Resources\\Black.conf");
    m_InputBox->setPosition(544, 250);
    m_InputBox->setSize(240, 180);
    m_InputBox->setTextSize(11);
    m_InputBox->setReadOnly(0);

    m_TimeBox->load("Resources\\Black.conf");
    m_TimeBox->setSize(244, 24);
    m_TimeBox->setPosition(541, 454);
    m_TimeBox->setNumbersOnly(1);
    m_TimeBox->setText("10");

    m_RunInput->load("Resources\\Black.conf");
    m_RunInput->setSize(128, 22);
    m_RunInput->setPosition(658, 486);
    m_RunInput->setText("Run input");
    m_RunInput->bindCallback(tgui::Button::LeftMouseClicked);
    m_RunInput->setCallbackId(4);

    m_EnforceTouchMove->load("Resources\\Black.conf");
    m_EnforceTouchMove->setPosition(16, 544);
    m_EnforceTouchMove->setText("Enforce Touch-Move");
    m_EnforceTouchMove->setTextColor(sf::Color::Black);
    m_EnforceTouchMove->setSize(18, 18);
    m_EnforceTouchMove->setCallbackId(1);
    m_EnforceTouchMove->bindCallback(tgui::ClickableWidget::LeftMouseClicked);

    m_VerboseLogging->load("Resources\\Black.conf");
    m_VerboseLogging->setPosition(16, 566);
    m_VerboseLogging->setText("Verbose Logging");
    m_VerboseLogging->setTextColor(sf::Color::Black);
    m_VerboseLogging->setSize(18, 18);
    m_VerboseLogging->setCallbackId(2);
    m_VerboseLogging->bindCallback(tgui::ClickableWidget::LeftMouseClicked);

    m_960Board->load("Resources\\Black.conf");
    m_960Board->setPosition(528 - 128, 570);
    m_960Board->setText("960 Variant");
    m_960Board->setTextColor(sf::Color::Black);
    m_960Board->setSize(18, 18);
    m_960Board->setCallbackId(5);
    m_960Board->bindCallback(tgui::ClickableWidget::LeftMouseClicked);

    m_RenewBoard->load("Resources\\Black.conf");
    m_RenewBoard->setSize(128, 22);
    m_RenewBoard->setText("New Game");
    m_RenewBoard->setPosition(528 - 128, 544);
    m_RenewBoard->bindCallback(tgui::Button::LeftMouseClicked);
    m_RenewBoard->setCallbackId(3);

    return 1;
}

void GUIManager::addText(const std::string& Text)
{
    m_OutputBox->addText(Text);
}

void GUIManager::draw()
{
    tgui::Callback callback;
    while (m_GUI.pollCallback(callback)){}
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

void GUIManager::doCallbacks()
{
    tgui::Callback callback;
    while (m_GUI.pollCallback(callback))
    {
        if (callback.id == 1)//Touch-Move
        {
            if (m_Board != nullptr)
            {
                m_Board->m_EnforceTouchMove = m_EnforceTouchMove->isChecked();
            }
        }

        if (callback.id == 2)//Verbose logging
        {
            if (m_Board != nullptr)
            {
                m_Board->m_VerboseLogging = m_VerboseLogging->isChecked();
            }
        }

        if (callback.id == 3)//New game
        {
            m_NewGame = 1 + m_960Board->isChecked();
        }

        if (callback.id == 4)//Run input
        {
            if (m_Board != nullptr)
            {
                m_Board->runParser(m_InputBox->getText(), m_TimeBox->getText());
            }
        }

        if (callback.id == 5)//960 Board
        {
            /*
            if (m_Board != nullptr)
            {
                m_Board->runParser(m_InputBox->getText(), m_TimeBox->getText());
            }
            */
        }
    }
}

void GUIManager::reset()
{
    m_OutputBox->setText("");
    m_InputBox->setText("");
}
