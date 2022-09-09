#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "gameboard.hh"

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include <QLineEdit>
#include <QTimer>
#include <QTime>
#include <QComboBox>
#include <QIcon>

using namespace std;

const int MAX_BOARD_SIZE = 18;

const int BOARD_OFFSET_X = 40;
const int BOARD_OFFSET_Y = 50;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slot for the function to handle button's left click signal
    void onLeftClicked(int i, int j);

    // Slot for the function to handle button's right click signal
    void onRightClicked();

private:

    Ui::MainWindow *ui;

    // Container for the icons path
    vector<QString> IconPath {
        "../Icon/num0.jpeg",
        "../Icon/num1.jpeg",
        "../Icon/num2.jpeg",
        "../Icon/num3.jpeg",
        "../Icon/num4.jpeg",
        "../Icon/num5.jpeg",
        "../Icon/num6.jpeg",
        "../Icon/num7.jpeg",
        "../Icon/num8.jpeg",
    };

    // Mainwindow widgets

    QLabel *title_label;
    QLabel *label;

    QPushButton *play_button;
    QPushButton *quit_button;

    QLineEdit *seed_input;

    // ComboBox to chose the game's difficulty and size
    QComboBox *game_difficulty_box;
    QComboBox *board_size_box;

    // Game widgets

    QPushButton *exit_button;
    QPushButton *check_button;
    // Container for all the button objects
    vector<vector<QPushButton*>> gameboard;

    // timer for the game's timer
    QTimer *timer;
    QTime *time;

    // Pointer to a GameBoard instance
    GameBoard *board;

    bool gameEnded = false;

    /*
     * Description: Reveal all the remaining boom when lose
     */
    void open_all_mines();

    /*
     * Description: Check if all the flag the user have put is
     * correct or not
     *
     * Return:
     *      true if all the flag are correctly put else false
     */
    bool check_all_flag();

    /*
     * Description: Update all the button according to the state
     *              of the game and the user input.
     */
    void update();

    /*
     * Description: Update all the button according to the state
     *              of the game and the user input.
     */
    void gameOver(bool won);

    /*
     * Description: Update all the button according to the state
     *              of the game and the user input.
     */
    void initialize_widgets();

    /*
     * Description: reset the icon and resize all the buttons
     */
    void reset_board();

    /*
     * Description: Hide all the game's widgets and show the menu's
     */
    void show_menu_scene();

    /*
     * Description: Hide all the menu's widget and show the game's,
     *              Also initalize a new GameBoard instance.
     */
    void show_game_scene();
};
#endif // MAINWINDOW_HH
