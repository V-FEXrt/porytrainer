#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include "trainer_util.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    trainer_scene_(std::make_unique<QGraphicsScene>())
{
    ui_->setupUi(this);


    pokemon_ui_items_.push_back({
        .species = ui_->comboBox_species,
        .item = ui_->comboBox_item,
        .iv = ui_->spinBox_iv,
        .level = ui_->spinBox_level,
        .move_1 = ui_->comboBox_move1,
        .move_2 = ui_->comboBox_move2,
        .move_3 = ui_->comboBox_move3,
        .move_4 = ui_->comboBox_move4,
        .sprite = ui_->graphicsView_species,
        .scene = std::make_unique<QGraphicsScene>(),
    });

    pokemon_ui_items_.push_back({
        .species = ui_->comboBox_species_2,
        .item = ui_->comboBox_item_2,
        .iv = ui_->spinBox_iv_2,
        .level = ui_->spinBox_level_2,
        .move_1 = ui_->comboBox_move1_2,
        .move_2 = ui_->comboBox_move2_2,
        .move_3 = ui_->comboBox_move3_2,
        .move_4 = ui_->comboBox_move4_2,
        .sprite = ui_->graphicsView_species_2,
        .scene = std::make_unique<QGraphicsScene>(),
    });

    pokemon_ui_items_.push_back({
        .species = ui_->comboBox_species_3,
        .item = ui_->comboBox_item_3,
        .iv = ui_->spinBox_iv_3,
        .level = ui_->spinBox_level_3,
        .move_1 = ui_->comboBox_move1_3,
        .move_2 = ui_->comboBox_move2_3,
        .move_3 = ui_->comboBox_move3_3,
        .move_4 = ui_->comboBox_move4_3,
        .sprite = ui_->graphicsView_species_3,
        .scene = std::make_unique<QGraphicsScene>(),
    });

    pokemon_ui_items_.push_back({
        .species = ui_->comboBox_species_4,
        .item = ui_->comboBox_item_4,
        .iv = ui_->spinBox_iv_4,
        .level = ui_->spinBox_level_4,
        .move_1 = ui_->comboBox_move1_4,
        .move_2 = ui_->comboBox_move2_4,
        .move_3 = ui_->comboBox_move3_4,
        .move_4 = ui_->comboBox_move4_4,
        .sprite = ui_->graphicsView_species_4,
        .scene = std::make_unique<QGraphicsScene>(),
    });

    pokemon_ui_items_.push_back({
        .species = ui_->comboBox_species_5,
        .item = ui_->comboBox_item_5,
        .iv = ui_->spinBox_iv_5,
        .level = ui_->spinBox_level_5,
        .move_1 = ui_->comboBox_move1_5,
        .move_2 = ui_->comboBox_move2_5,
        .move_3 = ui_->comboBox_move3_5,
        .move_4 = ui_->comboBox_move4_5,
        .sprite = ui_->graphicsView_species_5,
        .scene = std::make_unique<QGraphicsScene>(),
    });

    pokemon_ui_items_.push_back({
        .species = ui_->comboBox_species_6,
        .item = ui_->comboBox_item_6,
        .iv = ui_->spinBox_iv_6,
        .level = ui_->spinBox_level_6,
        .move_1 = ui_->comboBox_move1_6,
        .move_2 = ui_->comboBox_move2_6,
        .move_3 = ui_->comboBox_move3_6,
        .move_4 = ui_->comboBox_move4_6,
        .sprite = ui_->graphicsView_species_6,
        .scene = std::make_unique<QGraphicsScene>(),
    });
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::initUI(QString root) {

    root_ = root;
    parser_util_ = std::make_unique<ParserUtil>(root_);

    ui_->comboBox_trainerClasses->clear();
    ui_->comboBox_trainerPics->clear();
    ui_->comboBox_encounterMusic->clear();
    ui_->comboBox_gender->clear();

    for (const auto& pokemon_ui_item : pokemon_ui_items_) {
        pokemon_ui_item.species->clear();
        pokemon_ui_item.item->clear();
        pokemon_ui_item.move_1->clear();
        pokemon_ui_item.move_1->clear();
        pokemon_ui_item.move_1->clear();
        pokemon_ui_item.move_1->clear();
    }

    ui_->listWidget_trainers->clear();

    for (const auto& pair : ai_script_check_boxes_)
    {
        ui_->gridLayout_aiScriptOptions->removeWidget(pair.second.get());
    }
    ai_script_check_boxes_.clear();

    ui_->comboBox_trainerClasses->addItems(parser_util_->ReadDefines("include/constants/trainer_classes.h", "CLASS_"));
    ui_->comboBox_trainerPics->addItems(parser_util_->ReadDefines("include/constants/trainers.h", "TRAINER_PIC_"));
    ui_->comboBox_encounterMusic->addItems(parser_util_->ReadDefines("include/constants/trainers.h", "TRAINER_ENCOUNTER_MUSIC_"));
    ui_->comboBox_gender->addItems({"MALE", "FEMALE"});

    for (const auto& pokemon_ui_item : pokemon_ui_items_) {
        pokemon_ui_item.species->addItems(parser_util_->ReadDefines("include/constants/species.h", "SPECIES_"));
        pokemon_ui_item.item->addItems(parser_util_->ReadDefines("include/constants/items.h", "ITEM_"));
        QStringList move_list = parser_util_->ReadDefines("include/constants/moves.h", "MOVE_");
        pokemon_ui_item.move_1->addItems(move_list);
        pokemon_ui_item.move_2->addItems(move_list);
        pokemon_ui_item.move_3->addItems(move_list);
        pokemon_ui_item.move_4->addItems(move_list);
    }

    // Fill in trainers
    fex::TrainerUtil util(root_.toStdString() + "/src/data/trainers.h", root_.toStdString() + "/src/data/trainer_parties.h");
    auto trainers = util.ReadTrainersV();

    for (unsigned int i = 0; i < trainers.size(); i++)
    {
        QString identifier = QString::fromStdString(trainers[i]->identifier());
        ui_->listWidget_trainers->addItem(identifier);
        trainers_.insert(std::pair<QString, std::unique_ptr<fex::TrainerValue>>(identifier, std::move(trainers[i])));
    }

    // Add Script AI checkboxes
    QStringList scripts = parser_util_->ReadDefines("include/constants/battle_ai.h", "AI_SCRIPT_");

    int width = 4;
    int height = 4;

    if (scripts.size() > width * height)
    {
        // todo WARN # of scripts size too large
    }

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int index = i * height + j;
            if (index >= scripts.size())
            {
                break;
            }

            QString script = scripts[index];

            std::unique_ptr<QCheckBox> box = std::make_unique<QCheckBox>();

            box->setToolTip(script);
            QString trunc = script.mid(10); // 10 = len(AI_SCRIPT_)
            if (trunc.length() > 12) {
                trunc.chop(trunc.length() - 9);
                trunc += "...";
            }
            box->setText(trunc);

            ui_->gridLayout_aiScriptOptions->addWidget(box.get(), i, j);

            ai_script_check_boxes_[script] = std::move(box);
        }
    }

    clearUI();
}

void MainWindow::clearUI()
{
    trainer_scene_->clear();
    ui_->comboBox_trainerClasses->setCurrentText("");
    ui_->lineEdit_trainerName->setText("");
    ui_->comboBox_trainerPics->setCurrentText("");
    ui_->checkBox_doubleBattle->setChecked(false);
    ui_->comboBox_gender->setCurrentText("");
    ui_->comboBox_encounterMusic->setCurrentText("");

    ui_->listWidget_items->clear();

    for (int i = 0; i < 6; i++) {
        pokemon_ui_items_[i].species->setCurrentText("");
        pokemon_ui_items_[i].item->setCurrentText("");
        pokemon_ui_items_[i].iv->clear();
        pokemon_ui_items_[i].level->clear();
        pokemon_ui_items_[i].move_1->setCurrentText("");
        pokemon_ui_items_[i].move_2->setCurrentText("");
        pokemon_ui_items_[i].move_3->setCurrentText("");
        pokemon_ui_items_[i].move_4->setCurrentText("");
        pokemon_ui_items_[i].scene->clear();
    }

    for (const auto& pair : ai_script_check_boxes_)
    {
        pair.second->setChecked(false);
    }
}

void MainWindow::on_comboBox_trainerClasses_activated(const QString &trainerClass)
{
//    /*logError*/(trainerClass);
}

void MainWindow::on_comboBox_trainerPics_activated(const QString &trainerPics)
{
//    /*logError*/(trainerPics);
}

void MainWindow::on_listWidget_trainers_itemSelectionChanged()
{
    auto it = trainers_.find(ui_->listWidget_trainers->currentItem()->text());
    if (it == trainers_.end()) {
        return;
    }

    clearUI();

    const std::unique_ptr<fex::TrainerValue>& trainer = it->second;

    QString pic_filename = QString::fromStdString(trainer->trainer_pic()).toLower() + "_front_pic.png";
    pic_filename = pic_filename.mid(12); // 12 = len(TRAINER_PIC_)
    QString pic_path = root_ + "/graphics/trainers/front_pics/" + pic_filename;
    QPixmap pixmap = QPixmap(pic_path);
    pixmap = pixmap.scaled(pixmap.width()*2, pixmap.height()*2, Qt::KeepAspectRatio);
    trainer_scene_->addPixmap(pixmap);
    ui_->graphicsView_trainerSprite->setScene(trainer_scene_.get());

    ui_->comboBox_trainerClasses->setCurrentText(QString::fromStdString(trainer->trainer_class()));
    ui_->lineEdit_trainerName->setText(QString::fromStdString(trainer->trainer_name()));
    ui_->comboBox_trainerPics->setCurrentText(QString::fromStdString(trainer->trainer_pic()));
    ui_->checkBox_doubleBattle->setChecked(trainer->double_battle());

    QString gender = "MALE";
    QString encounter_music = "";

    for (const std::string& value : trainer->encounter_music_gender()) {
        if (value == "F_TRAINER_FEMALE") {
            gender = "FEMALE";
        } else {
            encounter_music = QString::fromStdString(value);
        }
    }

    ui_->comboBox_gender->setCurrentText(gender);
    ui_->comboBox_encounterMusic->setCurrentText(encounter_music);

    for(const std::string& value : trainer->items()) {
        ui_->listWidget_items->addItem(QString::fromStdString(value));
    }

    int ui_index = 0;
    for (const std::unique_ptr<fex::PokemonValue>& pokemon : trainer->party()) {
        pokemon_ui_items_[ui_index].species->setCurrentText(QString::fromStdString(pokemon->species()));
        pokemon_ui_items_[ui_index].item->setCurrentText(QString::fromStdString(pokemon->held_item()));
        pokemon_ui_items_[ui_index].iv->setValue(pokemon->iv());
        pokemon_ui_items_[ui_index].level->setValue(pokemon->level());

        QString species_name = QString::fromStdString(pokemon->species()).toLower().mid(8); // 8 = len(SPECIES_)
        QString sprite_path = root_ + "/graphics/pokemon/" + species_name + "/front.png";
        QPixmap sprite = QPixmap(sprite_path);
        sprite = sprite.scaled(sprite.width()*0.9, sprite.height()*0.9, Qt::KeepAspectRatio);
        pokemon_ui_items_[ui_index].scene->addPixmap(sprite);
        pokemon_ui_items_[ui_index].sprite->setScene(pokemon_ui_items_[ui_index].scene.get());

        if (pokemon->moves().size() > 0) {
            pokemon_ui_items_[ui_index].move_1->setCurrentText(QString::fromStdString(pokemon->moves()[0]));
            pokemon_ui_items_[ui_index].move_2->setCurrentText(QString::fromStdString(pokemon->moves()[1]));
            pokemon_ui_items_[ui_index].move_3->setCurrentText(QString::fromStdString(pokemon->moves()[2]));
            pokemon_ui_items_[ui_index].move_4->setCurrentText(QString::fromStdString(pokemon->moves()[3]));
        }
        ui_index++;
    }

    for (const std::string& ai_flag : trainer->ai_flags())
    {
        ai_script_check_boxes_[QString::fromStdString(ai_flag)]->setChecked(true);
    }
}

void MainWindow::on_actionOpen_Project_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    initUI(dir);
}
