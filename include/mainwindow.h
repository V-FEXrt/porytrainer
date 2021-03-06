#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QGraphicsView>

#include <memory>
#include <map>

#include "trainer_value.h"
#include "parser_util.h"

struct PokemonUIItem {
    QComboBox* species;
    QComboBox* item;
    QSpinBox* iv;
    QSpinBox* level;
    QComboBox* move_1;
    QComboBox* move_2;
    QComboBox* move_3;
    QComboBox* move_4;
    QGraphicsView* sprite;
    std::unique_ptr<QGraphicsScene> scene;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_comboBox_trainerClasses_activated(const QString &arg1);
    void on_comboBox_trainerPics_activated(const QString &arg1);
    void on_listWidget_trainers_itemSelectionChanged();

    void on_actionOpen_Project_triggered();

    void on_lineEdit_searchTrainers_textChanged(const QString &arg1);

    void on_actionSave_triggered();

    void on_lineEdit_trainerName_textEdited(const QString &arg1);

    void on_checkBox_doubleBattle_clicked(bool checked);

    void on_comboBox_encounterMusic_activated(const QString &arg1);

    void on_comboBox_gender_activated(const QString &arg1);

    void on_pushButton_addItem_clicked();

    void on_pushButton_removeItem_clicked();

    void on_aiCheckBoxes_clicked();

    void on_pokemonSpecies_activated(const QString &arg1);
    void on_pokemonHeldItem_activated(const QString &arg1);
    void on_pokemonLevel_textChanged(const QString &value);
    void on_pokemonIv_textChanged(const QString &value);
    void on_pokemonMoves_activated(const QString &arg1);

private:
    void initUI(QString root);
    void clearUI();

    Ui::MainWindow* ui_;

    QString root_;

    std::unique_ptr<ParserUtil> parser_util_;

    std::map<QString, std::unique_ptr<fex::TrainerValue>> trainers_;
    std::vector<PokemonUIItem> pokemon_ui_items_;
    std::unique_ptr<QGraphicsScene> trainer_scene_;
    std::map<QString, std::unique_ptr<QCheckBox>> ai_script_check_boxes_;

signals:
};

#endif // MAINWINDOW_H
