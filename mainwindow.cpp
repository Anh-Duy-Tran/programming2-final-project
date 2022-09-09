#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "qrightclickbutton.hh"

// global variable for the button size
int BUTTON_SIZE = 30;

// global variable for the board size in total
int BOARD_PIXEL_SIZE = 540;

// global variable for the number of title in each side of the board
int BOARD_SIDE;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // call the method to set up all the possible widget for the game once
    initialize_widgets();

    // begin the application by first show the menu screen
    show_menu_scene();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset_board()
{
    // reset the gameEnded boolean value
    gameEnded = false;

    // reset the messageText
    ui->messageText->setText("");

    // calculate the button size according to the number of button in a row
    BUTTON_SIZE = BOARD_PIXEL_SIZE / board_size_box->currentData().toInt();
    int button_x = BOARD_OFFSET_X;
    int button_y = BOARD_OFFSET_Y;

    for (int i = 0; i < BOARD_SIDE; i++)
    {
        for (int j = 0; j < BOARD_SIDE; j++)
        {
            gameboard[i][j]->setGeometry(QRect(QPoint(button_x, button_y), QSize(BUTTON_SIZE, BUTTON_SIZE)));
            gameboard[i][j]->setIcon(QIcon("../Icon/block.jpeg"));
            gameboard[i][j]->setIconSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
            gameboard[i][j]->setStyleSheet("");
            gameboard[i][j]->setText("");

            button_y += BUTTON_SIZE;
        }
        button_x += BUTTON_SIZE;
        button_y = BOARD_OFFSET_Y;
    }
}

void MainWindow::open_all_mines()
{
    for (int i = 0; i < BOARD_SIDE; i++)
    {
        for (int j = 0; j < BOARD_SIDE; j++)
        {
            Square square = board->getSquare(i,j);
            if (square.hasMine() and not square.hasFlag())
            {
                gameboard[i][j]->setIcon(QIcon("../Icon/boom2.jpeg"));
                gameboard[i][j]->setIconSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
            }
            if (square.hasFlag() and not square.hasMine())
            {
                gameboard[i][j]->setIcon(QIcon("../Icon/flag2.jpeg"));
                gameboard[i][j]->setIconSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
            }
        }
    }
}

void MainWindow::gameOver(bool won)
{
    // Stop the timer
    timer->stop();
    // Change the game state variable
    gameEnded = true;

    // Change the message text
    if (won)
    {
        ui->messageText->setText("YOU WON!");
    }
    else
    {
        ui->messageText->setText("YOU LOSE!");

        // Open all the remaining mine if lose
        open_all_mines();
    }


    return;
}

void MainWindow::update()
{
    for (int i = 0; i < BOARD_SIDE; i++)
    {
        for (int j = 0; j < BOARD_SIDE; j++)
        {

            Square square = board->getSquare(i, j);
            if(square.isOpen())
            {
                if (square.hasMine())
                {
                    gameOver(false);
                    gameboard[i][j]->setIcon(QIcon("../Icon/boom.jpeg"));
                    gameboard[i][j]->setIconSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                    continue;
                }
                QString path = IconPath[square.countAdjacent()];
                gameboard[i][j]->setIcon(QIcon(path));
                gameboard[i][j]->setIconSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                continue;
            }
        }
    }

    // check if the game is over or not
    if (board->isGameOver())
    {
        gameOver(true);
    }
}

void MainWindow::show_menu_scene()
{

    // Show all the menu widgets
    title_label->show();
    play_button->show();
    seed_input->show();
    board_size_box->show();
    game_difficulty_box->show();
    label->show();
    quit_button->show();

    // Hide all the game widgets
    ui->lcdNumber->hide();
    exit_button->hide();
    check_button->hide();
    ui->messageText->hide();
    for (auto &container : gameboard)
    {
        for (auto element : container)
            element->hide();
    }

}

void MainWindow::show_game_scene()
{
    // initalize a new GameBoard instance
    board = new GameBoard;

    BOARD_SIDE = board_size_box->currentData().toInt();

    board->BOARD_SIDE = BOARD_SIDE;
    board->MINE_PROBABALITY = game_difficulty_box->currentData().toInt();

    if (seed_input->text() == "")
    {
        board->init(std::time(NULL));
    }
    else
        board->init(seed_input->text().toInt());



    // Hide all the menu widgets
    title_label->hide();
    play_button->hide();
    seed_input->hide();
    label->hide();
    quit_button->hide();

    board_size_box->hide();
    game_difficulty_box->hide();

    // Reset all the button appearance
    reset_board();

    // Clear the message
    ui->messageText->setText("");

    // Show all the game board widgets
    ui->lcdNumber->show();
    ui->messageText->show();
    exit_button->show();
    check_button->show();


    for (int i = 0; i < BOARD_SIDE; i++)
    {
        for (int j = 0; j < BOARD_SIDE; j++)
        {
            gameboard[i][j]->show();
        }
    }
}

void MainWindow::initialize_widgets()
{

    // MAIN MENU WIDGETS //

    // Game label
    title_label = new QLabel(this);
    title_label->setText("MINESWEEPER");

    QFont font = title_label->font();
    font.setPointSize(72);
    font.setBold(true);
    title_label->setFont(font);
    title_label->setGeometry(20, 70, 700, 200);

    // Play Button
    play_button = new QPushButton("Play", this);
    play_button->setGeometry(QRect(QPoint(300, 250), QSize(100, 50)));

    connect(play_button, &QPushButton::released, this, &MainWindow::show_game_scene);


    // Seed number entry
    seed_input = new QLineEdit(this);
    seed_input->setPlaceholderText("Seed..");
    seed_input->setFocus();
    seed_input->setGeometry(100, 250, 100, 20);

    // Board size and game difficulty dropbox

    // Label
    label = new QLabel("Game setting: ", this);
    label->setGeometry(100, 200, 100, 50);

    // Combo boxes
    board_size_box = new QComboBox(this);
    game_difficulty_box = new QComboBox(this);

    // Add options to the combo boxes
    board_size_box->addItem("6x6", 6);
    board_size_box->addItem("10x10", 10);
    board_size_box->addItem("15x15", 15);
    board_size_box->addItem("18x18", 18);

    // The number is the percentage of boom
    game_difficulty_box->addItem("Easy", 7);
    game_difficulty_box->addItem("Medium", 13);
    game_difficulty_box->addItem("Hard", 20);

    board_size_box->setGeometry(100, 280, 100, 20);
    game_difficulty_box->setGeometry(100, 310, 100, 20);

    // Quit button
    quit_button = new QPushButton("Quit", this);
    quit_button->setGeometry(QRect(QPoint(300, 300), QSize(100, 50)));

    connect(quit_button, &QPushButton::released, this, [this](){this->close();});

    // GAME BOARD WIDGETS //


    // Create and store all the button in side a gameboard container
    int button_x = 40;
    int button_y = 40;

    for (int i = 0; i < MAX_BOARD_SIZE; i++)
    {
        gameboard.push_back({});
        for (int j = 0; j < MAX_BOARD_SIZE; j++)
        {

            // Create the button, make "this" the parent
            QPushButton * button = new QRightClickButton(this);

            // Connect button signal to appropriate slot
            connect(button, &QRightClickButton::clicked, this, [i, j, this]{MainWindow::onLeftClicked(i, j);});
            connect(button, SIGNAL(rightClicked()), this, SLOT(onRightClicked()));
            button->hide();

            // Store the button to a container
            gameboard[i].push_back(button);

            // Offset the position for the next button
            button_y += BUTTON_SIZE;
        }
        button_x += BUTTON_SIZE;
        button_y = 40;
    }

    // Game timer
    timer = new QTimer(this);
    time = new QTime(0, 0);

    connect(play_button, &QPushButton::released, this, [this](){timer->start(10);});
    connect(timer,&QTimer::timeout,this, [this](){
        *time=time->addMSecs(10);
        ui->lcdNumber->display(time->toString("mm:ss"));
    });


    // Exit button
    exit_button = new QPushButton("Exit", this);
    exit_button->setGeometry(QRect(QPoint(600, 600), QSize(100, 50)));
    connect(exit_button, &QPushButton::released, this, [this](){
        // deallocate the GameBoard pointer
        delete board;

        show_menu_scene();
        timer->stop();
        delete time;
        time = new QTime(0,0);
    });

    // Check button
    check_button = new QPushButton("Check", this);
    check_button->setGeometry(QRect(QPoint(600, 550), QSize(100, 50)));
    connect(check_button, &QPushButton::clicked, this, [this](){
        if (not gameEnded)
        {
            if(check_all_flag())
            {
                ui->messageText->setText("All flags are correct !");
            }
            else
                ui->messageText->setText("One or more flag \n is not correct !");
        }
    });
}

bool MainWindow::check_all_flag()
{
    for (int i = 0; i < BOARD_SIDE; i++)
    {
        for (int j = 0; j < BOARD_SIDE; j++)
        {
            Square square = board->getSquare(i, j);
            if (square.hasFlag() and not square.hasMine())
            {
                return false;
            }
        }

    }
    return true;
}

void MainWindow::onLeftClicked(int i, int j)
{
    // Do nothing if the game is ended
    if (gameEnded)
        return;

    // clear the message after every turn
    ui->messageText->clear();

    board->openSquare(i, j);
    update();
}

void MainWindow::onRightClicked()
{
    // Do nothing if the game is ended
    if (gameEnded)
        return;

    // clear the message after every turn
    ui->messageText->clear();

    auto sender = QObject::sender();

    // Search for the sender
    for (int i = 0; i < (int)gameboard.size(); i++)
    {
        for (int j = 0; j < (int)gameboard[0].size(); j++)
        {

            if(gameboard[i][j] == sender)
            {
                Square square = board->getSquare(i, j);

                // If user right-clicked on a opened title then open all the surrounding tiles.
                if (square.isOpen())
                {
                    int min_x = std::max(0, i - 1);
                    int max_x = std::min(BOARD_SIDE - 1, i + 1);
                    int min_y = std::max(0, j - 1);
                    int max_y = std::min(BOARD_SIDE - 1, j + 1);


                    for(int y = min_y; y <= max_y; ++y)
                    {
                        for(int x = min_x; x <= max_x; ++x)
                        {
                            if(not board->getSquare(x, y).isOpen())
                            {
                                board->openSquare(x, y); // Indirect recursive call
                            }
                        }
                    }
                    update();
                    return;
                }

                //  Else, add/remove flag
                if(square.hasFlag())
                {
                    square.removeFlag();
                    gameboard[i][j]->setIcon(QIcon("../Icon/block.jpeg"));
                    gameboard[i][j]->setIconSize(QSize(BUTTON_SIZE, BUTTON_SIZE));

                    board->setSquare(square, i, j);
                    return;
                }
                square.addFlag();

                gameboard[i][j]->setIcon(QIcon("../Icon/flag.jpeg"));
                gameboard[i][j]->setIconSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                board->setSquare(square, i, j);

                if (board->isGameOver())
                {
                    gameOver(true);
                }

                return;
            }
        }
    }
}
