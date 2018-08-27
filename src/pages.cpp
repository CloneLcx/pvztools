
#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include <QGridLayout>
#include <QString>
#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QSpacerItem>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QDesktopServices>
#include <QMessageBox>
#include <QFileDialog>
#include <QCoreApplication>
#include <QApplication>
#include <QDateTime>
#include <QClipboard>
#include <QTableWidget>
#include <QHeaderView>

#include <array>

#include "src/list.h"
#include "src/pages.h"

namespace Pt
{

// void SET_COMBO_TEXT(QComboBox *, QStringList, unsigned int);
// void SET_COMBO_TEXT(QComboBox *combo, QStringList list, unsigned int size)
// {
//     if (combo->count() == 0)
//         for (size_t i = 0; i < size; i++)
//             combo->addItem(list[i]);
//     else
//         for (size_t i = 0; i < size; i++)
//             combo->setItemText(i, list[i]);
// }

#define SET_COMBO_TEXT(combo, list, size) \
    if (combo->count() == 0)              \
        for (size_t i = 0; i < size; i++) \
            combo->addItem(list[i]);      \
    else                                  \
        for (size_t i = 0; i < size; i++) \
            combo->setItemText(i, list[i]);

// Level

LevelPage::LevelPage(QWidget *parent)
    : QWidget(parent)
{
    getGoldSunflowerTrophyButton = new QPushButton(this);
    getAllShopItemsButton = new QPushButton(this);
    unlockAllModeCheckBox = new QCheckBox(this);
    directWinButton = new QPushButton(this);

    adventureRadioButton = new QRadioButton(this);
    miniGamesRadioButton = new QRadioButton(this);
    puzzleRadioButton = new QRadioButton(this);
    survivalRadioButton = new QRadioButton(this);

    adventureCombo = new QComboBox(this);
    miniGamesCombo = new QComboBox(this);
    puzzleCombo = new QComboBox(this);
    survivalCombo = new QComboBox(this);

    mixmodeButton = new QPushButton(this);

    showHideGamesCheckBox = new QCheckBox(this);

    izeArrayCombo = new QComboBox(this);
    lockIzeArrayCheckBox = new QCheckBox(this);

    levelLineEdit = new QLineEdit(this);
    levelLineEdit->setValidator(new QIntValidator(-1, 999999999, this));
    levelLineEdit->setAlignment(Qt::AlignRight);
    jumpLevelButton = new QPushButton(this);

    // set default status
    adventureRadioButton->setChecked(true);

    levelLineEdit->setText("1008");

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(getGoldSunflowerTrophyButton, 0, 0, 1, 1);
    mainLayout->addWidget(getAllShopItemsButton, 0, 1, 1, 1);
    mainLayout->addWidget(unlockAllModeCheckBox, 0, 2, 1, 2);
    mainLayout->addWidget(directWinButton, 0, 4, 1, 1);
    mainLayout->addWidget(adventureRadioButton, 1, 0, 1, 1);
    mainLayout->addWidget(miniGamesRadioButton, 2, 0, 1, 1);
    mainLayout->addWidget(puzzleRadioButton, 3, 0, 1, 1);
    mainLayout->addWidget(survivalRadioButton, 4, 0, 1, 1);
    mainLayout->addWidget(adventureCombo, 1, 1, 1, 1);
    mainLayout->addWidget(miniGamesCombo, 2, 1, 1, 2);
    mainLayout->addWidget(puzzleCombo, 3, 1, 1, 2);
    mainLayout->addWidget(survivalCombo, 4, 1, 1, 2);
    mainLayout->addWidget(mixmodeButton, 1, 4, 1, 1);
    mainLayout->addWidget(showHideGamesCheckBox, 2, 3, 1, 2);
    mainLayout->addWidget(izeArrayCombo, 3, 3, 1, 1);
    mainLayout->addWidget(lockIzeArrayCheckBox, 3, 4, 1, 1);
    mainLayout->addWidget(levelLineEdit, 4, 3, 1, 1);
    mainLayout->addWidget(jumpLevelButton, 4, 4, 1, 1);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    connect(getGoldSunflowerTrophyButton, &QPushButton::clicked,
            this, &LevelPage::GetGoldSunflowerTrophy);

    connect(getAllShopItemsButton, &QPushButton::clicked,
            this, &LevelPage::GetAllShopItems);

    connect(unlockAllModeCheckBox, &QCheckBox::clicked,
            this, &LevelPage::UnlockAllMode);

    connect(directWinButton, &QPushButton::clicked,
            this, &LevelPage::DirectWin);

    connect(mixmodeButton, &QPushButton::clicked,
            this, [=]() {
                // Adventure  // 0
                // Mini-Games // 16-50
                // Puzzle     // 51-72
                // Survival   // 1-15
                int mode;

                if (miniGamesRadioButton->isChecked())
                    mode = miniGamesCombo->currentIndex() + 16;
                else if (puzzleRadioButton->isChecked())
                    mode = puzzleCombo->currentIndex() + 51;
                else if (survivalRadioButton->isChecked())
                    mode = survivalCombo->currentIndex() + 1;
                else
                    mode = 0;

                int level = adventureCombo->currentIndex() + 1;

                emit MixMode(mode, level);
            });

    connect(showHideGamesCheckBox, &QCheckBox::clicked,
            this, &LevelPage::ShowHideGames);

    connect(izeArrayCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [=](int index) {
                bool on = lockIzeArrayCheckBox->isChecked();
                int mode = index;
                emit LockIZE(on, mode);
            });

    connect(lockIzeArrayCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                bool on = checked;
                int mode = izeArrayCombo->currentIndex();
                emit LockIZE(on, mode);
            });

    connect(jumpLevelButton, &QPushButton::clicked,
            this, [=]() {
                int level = levelLineEdit->text().toInt();
                emit JumpLevel(level);
            });
}

void LevelPage::TranslateUI()
{
    getGoldSunflowerTrophyButton->setText(tr("Get Gold Sunflower Trophy"));
    getAllShopItemsButton->setText(tr("Get All Shop Items"));
    unlockAllModeCheckBox->setText(tr("Unlock All Mode Temporarily"));
    directWinButton->setText(tr("Direct Win"));

    adventureRadioButton->setText(tr("Adventure"));
    miniGamesRadioButton->setText(tr("Mini-Games"));
    puzzleRadioButton->setText(tr("Puzzle"));
    survivalRadioButton->setText(tr("Survival"));

    SET_COMBO_TEXT(adventureCombo, List::Get().adventureList, 50);
    SET_COMBO_TEXT(miniGamesCombo, List::Get().miniGamesList, 35);
    SET_COMBO_TEXT(puzzleCombo, List::Get().puzzleList, 22);
    SET_COMBO_TEXT(survivalCombo, List::Get().survivalList, 15);

    mixmodeButton->setText(tr("Mix Mode"));

    showHideGamesCheckBox->setText(tr("Show Hide Games"));

    SET_COMBO_TEXT(izeArrayCombo, List::Get().izeArrayList, 8);
    lockIzeArrayCheckBox->setText(tr("Lock IZE Array"));

    jumpLevelButton->setText(tr("Jump Level"));
}

// Resources

ResourcePage::ResourcePage(QWidget *parent)
    : QWidget(parent)
{
    sunLabel = new QLabel(this);
    sunLineEdit = new QLineEdit(this);
    sunLineEdit->setValidator(new QIntValidator(0, 999999999, this));
    sunLineEdit->setAlignment(Qt::AlignRight);
    sunButton = new QPushButton(this);

    sunLimitLabel = new QLabel(this);
    sunLimitLineEdit = new QLineEdit(this);
    sunLimitLineEdit->setValidator(new QIntValidator(0, 999999999, this));
    sunLimitLineEdit->setAlignment(Qt::AlignRight);
    sunLimitButton = new QPushButton(this);

    moneyLabel = new QLabel(this);
    moneyLineEdit = new QLineEdit(this);
    moneyLineEdit->setValidator(new QIntValidator(0, 999999999, this));
    moneyLineEdit->setAlignment(Qt::AlignRight);
    moneyButton = new QPushButton(this);

    moneyLimitLabel = new QLabel(this);
    moneyLimitLineEdit = new QLineEdit(this);
    moneyLimitLineEdit->setValidator(new QIntValidator(0, 999999999, this));
    moneyLimitLineEdit->setAlignment(Qt::AlignRight);
    moneyLimitButton = new QPushButton(this);

    manyFallingSunCheckBox = new QCheckBox(this);
    noFallingSunCheckBox = new QCheckBox(this);
    autoCollectCheckBox = new QCheckBox(this);
    zombieNoFallingCheckBox = new QCheckBox(this);

    manyFallingSunCheckBox->setEnabled(false);

    valueLabel = new QLabel(this);
    valueCombo = new QComboBox(this);
    valueLineEdit = new QLineEdit(this);
    valueLineEdit->setValidator(new QIntValidator(0, 999999999, this));
    valueLineEdit->setAlignment(Qt::AlignRight);
    valueButton = new QPushButton(this);

    damageLabel = new QLabel(this);
    damageCombo = new QComboBox(this);
    damageLineEdit = new QLineEdit(this);
    damageLineEdit->setValidator(new QIntValidator(0, 999999999, this));
    damageLineEdit->setAlignment(Qt::AlignRight);
    damageButton = new QPushButton(this);

    hpLabel = new QLabel(this);
    hpCombo = new QComboBox(this);
    hpLineEdit = new QLineEdit(this);
    hpLineEdit->setValidator(new QIntValidator(0, 999999999, this));
    hpLineEdit->setAlignment(Qt::AlignRight);
    hpButton = new QPushButton(this);

    timeLabel = new QLabel(this);
    timeCombo = new QComboBox(this);
    timeLineEdit = new QLineEdit(this);
    timeLineEdit->setValidator(new QIntValidator(0, 999999999, this));
    timeLineEdit->setAlignment(Qt::AlignRight);
    timeButton = new QPushButton(this);

    sunLineEdit->setText("8000");
    sunLimitLineEdit->setText("9990");

    moneyLineEdit->setText("80000");
    moneyLimitLineEdit->setText("99999");

    valueLineEdit->setText("");
    damageLineEdit->setText("");
    hpLineEdit->setText("");
    timeLineEdit->setText("");

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(sunLabel, 0, 0, 1, 1);
    mainLayout->addWidget(sunLineEdit, 0, 1, 1, 1);
    mainLayout->addWidget(sunButton, 0, 2, 1, 1);
    mainLayout->addWidget(sunLimitLabel, 0, 3, 1, 1);
    mainLayout->addWidget(sunLimitLineEdit, 0, 4, 1, 1);
    mainLayout->addWidget(sunLimitButton, 0, 5, 1, 1);
    mainLayout->addWidget(moneyLabel, 1, 0, 1, 1);
    mainLayout->addWidget(moneyLineEdit, 1, 1, 1, 1);
    mainLayout->addWidget(moneyButton, 1, 2, 1, 1);
    mainLayout->addWidget(moneyLimitLabel, 1, 3, 1, 1);
    mainLayout->addWidget(moneyLimitLineEdit, 1, 4, 1, 1);
    mainLayout->addWidget(moneyLimitButton, 1, 5, 1, 1);
    mainLayout->addWidget(manyFallingSunCheckBox, 2, 5, 1, 1);
    mainLayout->addWidget(noFallingSunCheckBox, 3, 5, 1, 1);
    mainLayout->addWidget(autoCollectCheckBox, 4, 5, 1, 1);
    mainLayout->addWidget(zombieNoFallingCheckBox, 5, 5, 1, 1);
    mainLayout->addWidget(valueLabel, 2, 0, 1, 1);
    mainLayout->addWidget(valueCombo, 2, 1, 1, 1);
    mainLayout->addWidget(valueLineEdit, 2, 2, 1, 1);
    mainLayout->addWidget(valueButton, 2, 3, 1, 1);
    mainLayout->addWidget(damageLabel, 3, 0, 1, 1);
    mainLayout->addWidget(damageCombo, 3, 1, 1, 1);
    mainLayout->addWidget(damageLineEdit, 3, 2, 1, 1);
    mainLayout->addWidget(damageButton, 3, 3, 1, 1);
    mainLayout->addWidget(hpLabel, 4, 0, 1, 1);
    mainLayout->addWidget(hpCombo, 4, 1, 1, 1);
    mainLayout->addWidget(hpLineEdit, 4, 2, 1, 1);
    mainLayout->addWidget(hpButton, 4, 3, 1, 1);
    mainLayout->addWidget(timeLabel, 5, 0, 1, 1);
    mainLayout->addWidget(timeCombo, 5, 1, 1, 1);
    mainLayout->addWidget(timeLineEdit, 5, 2, 1, 1);
    mainLayout->addWidget(timeButton, 5, 3, 1, 1);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    connect(manyFallingSunCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    noFallingSunCheckBox->setChecked(false);
            });

    connect(noFallingSunCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    manyFallingSunCheckBox->setChecked(false);
            });

    connect(autoCollectCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    manyFallingSunCheckBox->setEnabled(true);
                else
                {
                    manyFallingSunCheckBox->setEnabled(false);
                    manyFallingSunCheckBox->setChecked(false);
                }
            });

    connect(sunButton, &QPushButton::clicked,
            this, [=]() {
                int sun = sunLineEdit->text().toInt();
                emit SetSun(sun);
            });

    connect(sunLimitButton, &QPushButton::clicked,
            this, [=]() {
                int sun_limit = sunLimitLineEdit->text().toInt();
                emit SetSunLimit(sun_limit);
            });

    connect(moneyButton, &QPushButton::clicked,
            this, [=]() {
                int money = moneyLineEdit->text().toInt();
                emit SetMoney(money);
            });

    connect(moneyLimitButton, &QPushButton::clicked,
            this, [=]() {
                int money_limit = moneyLimitLineEdit->text().toInt();
                emit SetMoneyLimit(money_limit);
            });

    connect(manyFallingSunCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    emit SetFallingSun(Sun::Many);
                else
                    emit SetFallingSun(Sun::Normal);
            });

    connect(noFallingSunCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                if (checked)
                    emit SetFallingSun(Sun::None);
                else
                    emit SetFallingSun(Sun::Normal);
            });

    connect(autoCollectCheckBox, &QCheckBox::clicked,
            this, &ResourcePage::AutoCollect);

    connect(zombieNoFallingCheckBox, &QCheckBox::clicked,
            this, &ResourcePage::ZombieNoFalling);

    // 0 x10
    // 1 0<=x<=127 +2x10
    // 2 0<=x<=127 x10
    // 3
    // 4 0<=x<=127 +5
    // 5 0<=x<=127

    connect(valueCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [=](int index) {
                if (index == 0 || index == 2)
                    valueLabel->setText(tr("Value") + " x10");
                else if (index == 1)
                    valueLabel->setText(tr("Value") + " +2 x10");
                else if (index == 4)
                    valueLabel->setText(tr("Value") + " +5");
                else
                    valueLabel->setText(tr("Value"));

                emit GetValue(index);
            });

    connect(valueButton, &QPushButton::clicked,
            this, [=]() {
                int index = valueCombo->currentIndex();
                int value = valueLineEdit->text().toInt();

                // char limit
                if ((index != 0 && index != 3) && value > 127)
                {
                    valueLineEdit->setText("127");
                    value = 127;
                }

                emit SetValue(index, value);
            });

    connect(damageCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [=](int index) {
                if (index == 17)
                    damageLabel->setText(tr("Damage") + " /4cs");
                else
                    damageLabel->setText(tr("Damage"));

                emit GetDamage(index);
            });

    connect(damageButton, &QPushButton::clicked,
            this, [=]() {
                int index = damageCombo->currentIndex();
                int damage = damageLineEdit->text().toInt();

                // char limit
                if (index == 15 && damage > 127)
                {
                    damageLineEdit->setText("127");
                    damage = 127;
                }
                if (index == 17 && damage > 128)
                {
                    damageLineEdit->setText("128");
                    damage = 128;
                }

                emit SetDamage(index, damage);
            });

    connect(hpCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [=](int index) {
                if (index == 7 || index == 9 || index == 10 || index == 11 || index == 12 || index == 17 || index == 25 || index == 27)
                    hpLabel->setText(tr("HP") + " " + "(" + tr("Accessories") + ")");
                else
                    hpLabel->setText(tr("HP"));

                emit GetHP(index);
            });

    connect(hpButton, &QPushButton::clicked,
            this, [=]() {
                int index = hpCombo->currentIndex();
                int hp = hpLineEdit->text().toInt();
                emit SetHP(index, hp);
            });

    connect(timeCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ResourcePage::GetTime);

    connect(timeButton, &QPushButton::clicked,
            this, [=]() {
                int index = timeCombo->currentIndex();
                int t = timeLineEdit->text().toInt();
                emit SetTime(index, t);
            });
}

void ResourcePage::TranslateUI()
{
    sunLabel->setText(tr("Sun"));
    sunButton->setText(tr("Set"));

    sunLimitLabel->setText(tr("Sun Limit"));
    sunLimitButton->setText(tr("Set"));

    moneyLabel->setText(tr("Money") + " x10");
    moneyButton->setText(tr("Set"));

    moneyLimitLabel->setText(tr("Money Limit") + " x10");
    moneyLimitButton->setText(tr("Set"));

    manyFallingSunCheckBox->setText(tr("Many Falling Sun"));
    noFallingSunCheckBox->setText(tr("No Falling Sun"));
    autoCollectCheckBox->setText(tr("Auto Collect"));
    zombieNoFallingCheckBox->setText(tr("Zombie No Falling"));

    valueLabel->setText(tr("Value") + " x10");
    SET_COMBO_TEXT(valueCombo, List::Get().coinList, 6);
    valueButton->setText(tr("Set"));

    damageLabel->setText(tr("Damage"));
    SET_COMBO_TEXT(damageCombo, List::Get().damageList, 18);
    damageButton->setText(tr("Set"));

    hpLabel->setText(tr("HP"));
    SET_COMBO_TEXT(hpCombo, List::Get().hpList, 31);
    hpButton->setText(tr("Set"));

    timeLabel->setText(tr("Time") + " " + "(" + "cs" + ")");
    SET_COMBO_TEXT(timeCombo, List::Get().timeList, 13);
    timeButton->setText(tr("Set"));
}

void ResourcePage::ShowValue(int value)
{
    valueLineEdit->setText(QString::number(value));
}

void ResourcePage::ShowDamage(int damage)
{
    damageLineEdit->setText(QString::number(damage));
}

void ResourcePage::ShowHP(int hp)
{
    hpLineEdit->setText(QString::number(hp));
}

void ResourcePage::ShowTime(int t)
{
    timeLineEdit->setText(QString::number(t));
}

// Plants

PlantsPage::PlantsPage(QWidget *parent)
    : QWidget(parent)
{
    cobsNoCdCheckBox = new QCheckBox(this);
    magnetsNoCdCheckBox = new QCheckBox(this);
    potatoMineNoCdCheckBox = new QCheckBox(this);
    chomperNoCdCheckBox = new QCheckBox(this);

    plantInvincibleCheckBox = new QCheckBox(this);
    plantWeakCheckBox = new QCheckBox(this);
    lockKernelCheckBox = new QCheckBox(this);
    lockButterCheckBox = new QCheckBox(this);

    noCraterCheckBox = new QCheckBox(this);
    mushroomsAwakeCheckBox = new QCheckBox(this);
    strongBloverCheckBox = new QCheckBox(this);
    strongPeasCheckBox = new QCheckBox(this);

    explodeImmediatelyCheckBox = new QCheckBox(this);
    explodeNeverCheckBox = new QCheckBox(this);
    beghouledFreeMoveCheckBox = new QCheckBox(this);
    attackSuperpositionCheckBox = new QCheckBox(this);

    plantTorchwoodCheckBox = new QCheckBox(this);
    plantGarlicCheckBox = new QCheckBox(this);
    plantUmbrellaCheckBox = new QCheckBox(this);
    plantPaperCheckBox = new QCheckBox(this);

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(cobsNoCdCheckBox, 0, 0, 1, 1);
    mainLayout->addWidget(magnetsNoCdCheckBox, 0, 1, 1, 1);
    mainLayout->addWidget(potatoMineNoCdCheckBox, 0, 2, 1, 1);
    mainLayout->addWidget(chomperNoCdCheckBox, 0, 3, 1, 1);
    mainLayout->addWidget(plantInvincibleCheckBox, 1, 0, 1, 1);
    mainLayout->addWidget(plantWeakCheckBox, 1, 1, 1, 1);
    mainLayout->addWidget(lockKernelCheckBox, 1, 2, 1, 1);
    mainLayout->addWidget(lockButterCheckBox, 1, 3, 1, 1);
    mainLayout->addWidget(noCraterCheckBox, 2, 0, 1, 1);
    mainLayout->addWidget(mushroomsAwakeCheckBox, 2, 1, 1, 1);
    mainLayout->addWidget(strongBloverCheckBox, 2, 2, 1, 1);
    mainLayout->addWidget(strongPeasCheckBox, 2, 3, 1, 1);
    mainLayout->addWidget(explodeImmediatelyCheckBox, 3, 0, 1, 1);
    mainLayout->addWidget(explodeNeverCheckBox, 3, 1, 1, 1);
    mainLayout->addWidget(beghouledFreeMoveCheckBox, 3, 2, 1, 1);
    mainLayout->addWidget(attackSuperpositionCheckBox, 3, 3, 1, 1);
    mainLayout->addWidget(plantTorchwoodCheckBox, 4, 0, 1, 1);
    mainLayout->addWidget(plantGarlicCheckBox, 4, 1, 1, 1);
    mainLayout->addWidget(plantUmbrellaCheckBox, 4, 2, 1, 1);
    mainLayout->addWidget(plantPaperCheckBox, 4, 3, 1, 1);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    connect(plantInvincibleCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    plantWeakCheckBox->setChecked(false);
            });

    connect(plantWeakCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    plantInvincibleCheckBox->setChecked(false);
            });

    connect(lockKernelCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    lockButterCheckBox->setChecked(false);
            });

    connect(lockButterCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    lockKernelCheckBox->setChecked(false);
            });

    connect(explodeImmediatelyCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    explodeNeverCheckBox->setChecked(false);
            });

    connect(explodeNeverCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    explodeImmediatelyCheckBox->setChecked(false);
            });

    connect(cobsNoCdCheckBox, &QCheckBox::clicked,
            this, &PlantsPage::CobCannonNoCD);

    connect(magnetsNoCdCheckBox, &QCheckBox::clicked,
            this, &PlantsPage::MagnetsNoCD);

    connect(potatoMineNoCdCheckBox, &QCheckBox::clicked,
            this, &PlantsPage::PotatoMineNoCD);

    connect(chomperNoCdCheckBox, &QCheckBox::clicked,
            this, &PlantsPage::ChomperNoCD);

    connect(plantInvincibleCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                if (checked)
                    emit SetPlantStrength(Strength::Invincible);
                else
                    emit SetPlantStrength(Strength::Normal);
            });

    connect(plantWeakCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                if (checked)
                    emit SetPlantStrength(Strength::Weak);
                else
                    emit SetPlantStrength(Strength::Normal);
            });

    connect(lockKernelCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                if (checked)
                    emit LockPult(Bullet::Kernel);
                else
                    emit LockPult(Bullet::Normal);
            });

    connect(lockButterCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                if (checked)
                    emit LockPult(Bullet::Butter);
                else
                    emit LockPult(Bullet::Normal);
            });

    connect(noCraterCheckBox, &QCheckBox::clicked,
            this, &PlantsPage::NoCrater);

    connect(mushroomsAwakeCheckBox, &QCheckBox::clicked,
            this, &PlantsPage::MushroomsAwake);

    connect(strongBloverCheckBox, &QCheckBox::clicked,
            this, &PlantsPage::StrongBlover);

    connect(strongPeasCheckBox, &QCheckBox::clicked,
            this, &PlantsPage::StrongPeas);

    connect(explodeImmediatelyCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                if (checked)
                    emit SetPlantExplode(Explode::Immediately);
                else
                    emit SetPlantExplode(Explode::Normal);
            });

    connect(explodeNeverCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                if (checked)
                    emit SetPlantExplode(Explode::Never);
                else
                    emit SetPlantExplode(Explode::Normal);
            });

    connect(beghouledFreeMoveCheckBox, &QCheckBox::clicked,
            this, &PlantsPage::BeghouledFreeMove);

    connect(attackSuperpositionCheckBox, &QCheckBox::clicked,
            this, &PlantsPage::AttackSuperposition);

    connect(plantTorchwoodCheckBox, &QCheckBox::clicked,
            this, &PlantsPage::PlantTorchwood);

    connect(plantGarlicCheckBox, &QCheckBox::clicked,
            this, &PlantsPage::PlantGarlic);

    connect(plantUmbrellaCheckBox, &QCheckBox::clicked,
            this, &PlantsPage::PlantUmbrella);

    connect(plantPaperCheckBox, &QCheckBox::clicked,
            this, &PlantsPage::PlantPaper);
}

void PlantsPage::TranslateUI()
{
    cobsNoCdCheckBox->setText(tr("Cob Cannon NoCD"));
    magnetsNoCdCheckBox->setText(tr("Magnets NoCD"));
    potatoMineNoCdCheckBox->setText(tr("Potato Mine NoCD"));
    chomperNoCdCheckBox->setText(tr("Chomper NoCD"));

    plantInvincibleCheckBox->setText(tr("Plant Invincible"));
    plantWeakCheckBox->setText(tr("Plant Weak"));
    lockKernelCheckBox->setText(tr("Lock Kernel"));
    lockButterCheckBox->setText(tr("Lock Butter"));

    noCraterCheckBox->setText(tr("No Crater"));
    mushroomsAwakeCheckBox->setText(tr("Mushrooms Awake"));
    strongBloverCheckBox->setText(tr("Strong Blover"));
    strongPeasCheckBox->setText(tr("Strong Peas"));

    explodeImmediatelyCheckBox->setText(tr("Explode Immediately"));
    explodeNeverCheckBox->setText(tr("Explode Never"));
    beghouledFreeMoveCheckBox->setText(tr("Beghouled Free Move"));
    attackSuperpositionCheckBox->setText(tr("Attack Superposition"));

    plantTorchwoodCheckBox->setText(tr("Plant Torchwood"));
    plantGarlicCheckBox->setText(tr("Plant Garlic"));
    plantUmbrellaCheckBox->setText(tr("Plant Umbrella"));
    plantPaperCheckBox->setText(tr("Plant Paper"));
}

// Zombies

ZombiesPage::ZombiesPage(QWidget *parent)
    : QWidget(parent)
{
    zombieExplodeImmediatelyCheckBox = new QCheckBox(this);
    zombieExplodeNeverCheckBox = new QCheckBox(this);

    zombieInvisibleCheckBox = new QCheckBox(this);
    zombieShowCheckBox = new QCheckBox(this);

    zombieInvincibleCheckBox = new QCheckBox(this);
    zombieWeakCheckBox = new QCheckBox(this);

    stopSpawningCheckBox = new QCheckBox(this);
    zombiesNoMoveCheckBox = new QCheckBox(this);

    noIceSlowDownCheckBox = new QCheckBox(this);
    noButterImmobilizeCheckBox = new QCheckBox(this);
    no3ZombiesGroupCheckBox = new QCheckBox(this);
    noIceTrailCheckBox = new QCheckBox(this);

    noThrowImpCheckBox = new QCheckBox(this);
    gargantuarEatableCheckBox = new QCheckBox(this);
    zombossNoMoveCheckBox = new QCheckBox(this);
    balloonBurstCheckBox = new QCheckBox(this);

    gatherZombiesCheckBox = new QCheckBox(this);
    gatherZombiesSlider = new QSlider(this);
    gatherZombiesSlider->setOrientation(Qt::Horizontal);
    gatherZombiesSlider->setMinimum(0);
    gatherZombiesSlider->setMaximum(800);
    gatherZombiesSlider->setSingleStep(1);
    gatherZombiesSlider->setPageStep(100);
    gatherZombiesSlider->setValue(600);

    spawningNextWaveButton = new QPushButton(this);
    allZombiesLabel = new QLabel(this);
    allZombiesLabel->setAlignment(Qt::AlignCenter);
    allZombiesCombo = new QComboBox(this);
    allZombiesButton = new QPushButton(this);

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(zombieExplodeImmediatelyCheckBox, 0, 0, 1, 1);
    mainLayout->addWidget(zombieExplodeNeverCheckBox, 0, 1, 1, 1);
    mainLayout->addWidget(zombieInvisibleCheckBox, 1, 0, 1, 1);
    mainLayout->addWidget(zombieShowCheckBox, 1, 1, 1, 1);
    mainLayout->addWidget(zombieInvincibleCheckBox, 2, 0, 1, 1);
    mainLayout->addWidget(zombieWeakCheckBox, 2, 1, 1, 1);
    mainLayout->addWidget(stopSpawningCheckBox, 3, 0, 1, 1);
    mainLayout->addWidget(zombiesNoMoveCheckBox, 3, 1, 1, 1);
    mainLayout->addWidget(noIceSlowDownCheckBox, 0, 2, 1, 1);
    mainLayout->addWidget(noButterImmobilizeCheckBox, 0, 3, 1, 1);
    mainLayout->addWidget(no3ZombiesGroupCheckBox, 1, 2, 1, 1);
    mainLayout->addWidget(noIceTrailCheckBox, 1, 3, 1, 1);
    mainLayout->addWidget(noThrowImpCheckBox, 2, 2, 1, 1);
    mainLayout->addWidget(gargantuarEatableCheckBox, 2, 3, 1, 1);
    mainLayout->addWidget(zombossNoMoveCheckBox, 3, 2, 1, 1);
    mainLayout->addWidget(balloonBurstCheckBox, 3, 3, 1, 1);
    mainLayout->addWidget(gatherZombiesCheckBox, 4, 0, 1, 1);
    mainLayout->addWidget(gatherZombiesSlider, 4, 1, 1, 3);
    mainLayout->addWidget(spawningNextWaveButton, 5, 0, 1, 1);
    mainLayout->addWidget(allZombiesLabel, 5, 1, 1, 1);
    mainLayout->addWidget(allZombiesCombo, 5, 2, 1, 1);
    mainLayout->addWidget(allZombiesButton, 5, 3, 1, 1);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    connect(zombieExplodeImmediatelyCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    zombieExplodeNeverCheckBox->setChecked(false);
            });

    connect(zombieExplodeNeverCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    zombieExplodeImmediatelyCheckBox->setChecked(false);
            });

    connect(zombieInvisibleCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    zombieShowCheckBox->setChecked(false);
            });

    connect(zombieShowCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    zombieInvisibleCheckBox->setChecked(false);
            });

    connect(zombieInvincibleCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    zombieWeakCheckBox->setChecked(false);
            });

    connect(zombieWeakCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                if (state == Qt::Checked)
                    zombieInvincibleCheckBox->setChecked(false);
            });

    connect(zombieExplodeImmediatelyCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                if (checked)
                    emit SetZombieExplode(Explode::Immediately);
                else
                    emit SetZombieExplode(Explode::Normal);
            });

    connect(zombieExplodeNeverCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                if (checked)
                    emit SetZombieExplode(Explode::Never);
                else
                    emit SetZombieExplode(Explode::Normal);
            });

    connect(zombieInvisibleCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                if (checked)
                    emit SetZombieVisibility(Visibility::Invisible);
                else
                    emit SetZombieVisibility(Visibility::Normal);
            });

    connect(zombieShowCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                if (checked)
                    emit SetZombieVisibility(Visibility::Visible);
                else
                    emit SetZombieVisibility(Visibility::Normal);
            });

    connect(zombieInvincibleCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                if (checked)
                    emit SetZombieStrength(Strength::Invincible);
                else
                    emit SetZombieStrength(Strength::Normal);
            });

    connect(zombieWeakCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                if (checked)
                    emit SetZombieStrength(Strength::Weak);
                else
                    emit SetZombieStrength(Strength::Normal);
            });

    connect(stopSpawningCheckBox, &QCheckBox::clicked,
            this, &ZombiesPage::StopSpawning);

    connect(zombiesNoMoveCheckBox, &QCheckBox::clicked,
            this, &ZombiesPage::ZombiesNoMove);

    connect(noIceSlowDownCheckBox, &QCheckBox::clicked,
            this, &ZombiesPage::NoIceSlowDown);

    connect(noButterImmobilizeCheckBox, &QCheckBox::clicked,
            this, &ZombiesPage::NoButterImmobilize);

    connect(no3ZombiesGroupCheckBox, &QCheckBox::clicked,
            this, &ZombiesPage::No3zGroup);

    connect(noIceTrailCheckBox, &QCheckBox::clicked,
            this, &ZombiesPage::NoIceTrail);

    connect(noThrowImpCheckBox, &QCheckBox::clicked,
            this, &ZombiesPage::NoThrowImp);

    connect(gargantuarEatableCheckBox, &QCheckBox::clicked,
            this, &ZombiesPage::GargantuarEatable);

    connect(zombossNoMoveCheckBox, &QCheckBox::clicked,
            this, &ZombiesPage::ZombossNoMove);

    connect(balloonBurstCheckBox, &QCheckBox::clicked,
            this, &ZombiesPage::BalloonBurst);

    connect(gatherZombiesCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                float pos = static_cast<float>(gatherZombiesSlider->value());
                emit GatherZombies(checked, pos);
            });

    connect(gatherZombiesSlider, &QSlider::valueChanged,
            this, [=](int value) {
                if (gatherZombiesCheckBox->isChecked())
                    emit GatherZombies(true, static_cast<float>(value));
            });

    connect(spawningNextWaveButton, &QPushButton::clicked,
            this, &ZombiesPage::SpawningNextWave);

    connect(allZombiesButton, &QPushButton::clicked,
            this, [=]() {
                int zombie_status = allZombiesCombo->currentIndex();
                emit AllZombiesXXX(zombie_status);
            });
}

void ZombiesPage::TranslateUI()
{
    zombieExplodeImmediatelyCheckBox->setText(tr("Immediately Explode"));
    zombieExplodeNeverCheckBox->setText(tr("Never Explode"));

    zombieInvisibleCheckBox->setText(tr("Zombie Invisible"));
    zombieShowCheckBox->setText(tr("Zombie Show"));

    zombieInvincibleCheckBox->setText(tr("Zombie Invincible"));
    zombieWeakCheckBox->setText(tr("Zombie Weak"));

    stopSpawningCheckBox->setText(tr("Stop Spawning"));
    zombiesNoMoveCheckBox->setText(tr("Zombies No Move"));

    noIceSlowDownCheckBox->setText(tr("No Ice Slow Down"));
    noButterImmobilizeCheckBox->setText(tr("No Butter Immobilize"));
    no3ZombiesGroupCheckBox->setText(tr("No 3 Zombies Group"));
    noIceTrailCheckBox->setText(tr("No Ice Trail"));

    noThrowImpCheckBox->setText(tr("No Throw Imp"));
    gargantuarEatableCheckBox->setText(tr("Gargantuar Eatable"));
    zombossNoMoveCheckBox->setText(tr("Zomboss No Move"));
    balloonBurstCheckBox->setText(tr("Balloon Burst"));

    gatherZombiesCheckBox->setText(tr("Gather Zombies"));

    spawningNextWaveButton->setText(tr("Next Wave"));

    allZombiesLabel->setText(tr("All Zombies"));
    SET_COMBO_TEXT(allZombiesCombo, List::Get().zombieStatusList, 10);
    allZombiesButton->setText(tr("Set"));
}

// Spawn

SpawnBriefPage::SpawnBriefPage(QWidget *parent)
    : QWidget(parent)
{
    qRegisterMetaType<std::array<bool, 33>>("std::array<bool, 33>");
    qRegisterMetaType<std::array<bool, 20>>("std::array<bool, 20>");

    limit_count = true;

    unsigned int tmp[20] = {2, 3, 4, 5, 6,
                            7, 8, 11, 12, 14,
                            15, 16, 17, 18, 19,
                            20, 21, 22, 23, 32};
    memcpy(zombieTypes, tmp, sizeof(tmp));

    spawnBriefWidget = new QWidget(this);

    for (size_t i = 0; i < 20; i++)
        spawnCheckBox[i] = new QCheckBox(spawnBriefWidget);

    spawnBriefLayout = new QGridLayout(spawnBriefWidget);
    for (size_t r = 0; r < 4; r++)
        for (size_t c = 0; c < 5; c++)
            spawnBriefLayout->addWidget(spawnCheckBox[r * 5 + c], r, c, 1, 1);

    for (int i = 0; i < spawnBriefLayout->rowCount(); i++)
        spawnBriefLayout->setRowStretch(i, 1);
    for (int i = 0; i < spawnBriefLayout->columnCount(); i++)
        spawnBriefLayout->setColumnStretch(i, 1);

    naturalSpawnButton = new QPushButton(this);
    extremeSpawnButton = new QPushButton(this);

    detailedModeButton = new QPushButton(this);

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(spawnBriefWidget, 0, 0, 10, 4);
    mainLayout->addWidget(naturalSpawnButton, 10, 2, 1, 1);
    mainLayout->addWidget(extremeSpawnButton, 10, 3, 1, 1);
    mainLayout->addWidget(detailedModeButton, 10, 0, 1, 1);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    // zombieTypes[i] == 2 / 5, i == 0 / 3
    connect(spawnCheckBox[0], &QCheckBox::stateChanged,
            this, [=](int state) {
                if (state == Qt::Checked)
                    spawnCheckBox[3]->setChecked(false);
            });
    connect(spawnCheckBox[3], &QCheckBox::stateChanged,
            this, [=](int state) {
                if (state == Qt::Checked)
                    spawnCheckBox[0]->setChecked(false);
            });

    for (size_t i = 0; i < 20; i++)
        if ((zombieTypes[i] != 19) && (zombieTypes[i] != 20))
            connect(spawnCheckBox[i], &QCheckBox::stateChanged,
                    this, &SpawnBriefPage::CountCheck);

    connect(detailedModeButton, &QPushButton::clicked,
            this, &SpawnBriefPage::SwitchToDetailedPage);

    connect(detailedModeButton, &QPushButton::clicked,
            this, &SpawnBriefPage::GetRandomSeed);

    connect(naturalSpawnButton, &QPushButton::clicked,
            this, [=]() {
                std::array<bool, 33> zombies = GetZombies();
                zombies[0] = true;
                emit InternalSpawn(zombies, false);
            });

    connect(extremeSpawnButton, &QPushButton::clicked,
            this, [=]() {
                std::array<bool, 33> zombies = GetZombies();
                zombies[0] = true;
                zombies[1] = true;
                std::array<bool, 20> giga_waves; // useless
                giga_waves.fill(true);
                emit CustomizeSpawn(zombies, false, true, true, true, false, giga_waves);
            });
}

void SpawnBriefPage::TranslateUI()
{
    for (size_t i = 0; i < 20; i++)
        spawnCheckBox[i]->setText(List::Get().zombieList[zombieTypes[i]]);

    naturalSpawnButton->setText(tr("Natural Spawn"));
    extremeSpawnButton->setText(tr("Extreme Spawn"));

    detailedModeButton->setText(tr("Switch Detailed Mode"));
}

void SpawnBriefPage::SwitchLayout(bool on)
{
    if (on)
    {
        unsigned int types[20] = {2, 5, 4, 6, 16,
                                  21, 3, 8, 11, 18,
                                  12, 15, 7, 17, 14,
                                  22, 23, 32, 20, 19};
        for (size_t r = 0; r < 4; r++)
            for (size_t c = 0; c < 5; c++)
                for (size_t i = 0; i < 4; i++)
                    for (size_t j = 0; j < 5; j++)
                        if (types[r * 5 + c] == zombieTypes[i * 5 + j])
                            spawnBriefLayout->addWidget(spawnCheckBox[i * 5 + j], r, c, 1, 1);
    }
    else
    {
        for (size_t r = 0; r < 4; r++)
            for (size_t c = 0; c < 5; c++)
                spawnBriefLayout->addWidget(spawnCheckBox[r * 5 + c], r, c, 1, 1);
    }
}

void SpawnBriefPage::LimitSpawnCount(bool on)
{
    limit_count = on;
    CountCheck();
}

void SpawnBriefPage::CountCheck()
{
    int zombieTypeCount = 1;
    for (size_t i = 0; i < 20; i++)
        if ((zombieTypes[i] != 19) && (zombieTypes[i] != 20))
            zombieTypeCount += spawnCheckBox[i]->isChecked() ? 1 : 0;

    for (size_t i = 0; i < 20; i++)
    {
        if ((zombieTypes[i] != 19) && (zombieTypes[i] != 20))
        {
            if (limit_count && zombieTypeCount >= 11)
            {
                if (!spawnCheckBox[i]->isChecked())
                    spawnCheckBox[i]->setEnabled(false);
            }
            else
            {
                if (!spawnCheckBox[i]->isEnabled())
                    spawnCheckBox[i]->setEnabled(true);
            }
        }
    }
}

std::array<bool, 33> SpawnBriefPage::GetZombies()
{
    std::array<bool, 33> zombies = {false};
    for (size_t i = 0; i < 20; i++)
        zombies[zombieTypes[i]] = spawnCheckBox[i]->isChecked();
    return zombies;
}

void SpawnBriefPage::SetZombies(std::array<bool, 33> zombies)
{
    for (size_t i = 0; i < 20; i++)
        spawnCheckBox[i]->setChecked(zombies[zombieTypes[i]]);
}

SpawnDetailedPage::SpawnDetailedPage(QWidget *parent)
    : QWidget(parent)
{
    qRegisterMetaType<std::array<bool, 33>>("std::array<bool, 33>");
    qRegisterMetaType<std::array<bool, 20>>("std::array<bool, 20>");

    QRegExp regExpHex("[a-fA-F0-9]{8}");
    randomSeedLineEdit = new QLineEdit(this);
    randomSeedLineEdit->setValidator(new QRegExpValidator(regExpHex, this));
    randomSeedLineEdit->setAlignment(Qt::AlignCenter);
    randomSeedButton = new QPushButton(this);

    spawnZombieCombo = new QComboBox(this);
    spawnAddButton = new QPushButton(this);
    spawnDeleteButton = new QPushButton(this);
    spawnClearButton = new QPushButton(this);

    spawnListWidget = new QListWidget(this);

    naturalSpawnRadioButton = new QRadioButton(this);
    extremeSpawnRadioButton = new QRadioButton(this);
    simulateSpawnRadioButton = new QRadioButton(this);

    spawnLimitWidget = new QWidget(this);

    limitFlagCheckBox = new QCheckBox(spawnLimitWidget);
    limitYetiCheckBox = new QCheckBox(spawnLimitWidget);
    limitBungeeCheckBox = new QCheckBox(spawnLimitWidget);
    limitGigaCheckBox = new QCheckBox(spawnLimitWidget);

    spawnLimitLayout = new QHBoxLayout(spawnLimitWidget);
    spawnLimitLayout->addWidget(limitFlagCheckBox);
    spawnLimitLayout->addWidget(limitYetiCheckBox);
    spawnLimitLayout->addWidget(limitBungeeCheckBox);
    spawnLimitLayout->addWidget(limitGigaCheckBox);
    spawnLimitLayout->setMargin(0);
    spawnLimitLayout->setSpacing(4);

    gigaWavesWidget = new QWidget(this);

    for (size_t i = 0; i < 20; i++)
        waveCheckBox[i] = new QCheckBox(gigaWavesWidget);

    waveLine9 = new QFrame(gigaWavesWidget);
    waveLine9->setFrameShape(QFrame::VLine);
    waveLine9->setFrameShadow(QFrame::Sunken);
    waveLine19 = new QFrame(gigaWavesWidget);
    waveLine19->setFrameShape(QFrame::VLine);
    waveLine19->setFrameShadow(QFrame::Sunken);

    gigaWavesLayout = new QHBoxLayout(gigaWavesWidget);
    for (size_t i = 0; i < 9; i++)
        gigaWavesLayout->addWidget(waveCheckBox[i]);
    gigaWavesLayout->addWidget(waveLine9);
    for (size_t i = 9; i < 19; i++)
        gigaWavesLayout->addWidget(waveCheckBox[i]);
    gigaWavesLayout->addWidget(waveLine19);
    gigaWavesLayout->addWidget(waveCheckBox[19]);
    gigaWavesLayout->setMargin(0);
    gigaWavesLayout->setSpacing(4);

    spawnResetButton = new QPushButton(this);
    spawnSetButton = new QPushButton(this);
    briefModeButton = new QPushButton(this);

    spawnSpacer1 = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    spawnSpacer3 = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    spawnSpacer5 = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    spawnSpacer7 = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    spawnSpacer9 = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

    // Set default states, Checked/Enabled/Visible.

    naturalSpawnRadioButton->setChecked(true);

    limitFlagCheckBox->setChecked(true);
    limitYetiCheckBox->setChecked(true);
    limitBungeeCheckBox->setChecked(true);

    spawnLimitWidget->setEnabled(false);
    gigaWavesWidget->setEnabled(false);

    randomSeedLineEdit->setText("00000000");

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(randomSeedLineEdit, 0, 0, 1, 1);
    mainLayout->addWidget(randomSeedButton, 2, 0, 1, 1);
    mainLayout->addWidget(spawnZombieCombo, 0, 1, 1, 1);
    mainLayout->addWidget(spawnAddButton, 0, 2, 1, 1);
    mainLayout->addWidget(spawnDeleteButton, 2, 2, 1, 1);
    mainLayout->addWidget(spawnClearButton, 2, 1, 1, 1);
    mainLayout->addWidget(spawnListWidget, 0, 3, 11, 1);
    mainLayout->addWidget(naturalSpawnRadioButton, 4, 0, 1, 1);
    mainLayout->addWidget(extremeSpawnRadioButton, 4, 1, 1, 1);
    mainLayout->addWidget(simulateSpawnRadioButton, 4, 2, 1, 1);
    mainLayout->addWidget(spawnLimitWidget, 6, 0, 1, 3);
    mainLayout->addWidget(gigaWavesWidget, 8, 0, 1, 3);
    mainLayout->addWidget(spawnResetButton, 10, 1, 1, 1);
    mainLayout->addWidget(spawnSetButton, 10, 2, 1, 1);
    mainLayout->addWidget(briefModeButton, 10, 0, 1, 1);
    mainLayout->addItem(spawnSpacer1, 1, 0, 1, 1);
    mainLayout->addItem(spawnSpacer3, 3, 0, 1, 1);
    mainLayout->addItem(spawnSpacer5, 5, 0, 1, 1);
    mainLayout->addItem(spawnSpacer7, 7, 0, 1, 1);
    mainLayout->addItem(spawnSpacer9, 9, 0, 1, 1);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    connect(naturalSpawnRadioButton, &QRadioButton::toggled,
            this, [=](bool checked) {
                if (checked)
                {
                    spawnLimitWidget->setEnabled(false);
                    limitFlagCheckBox->setEnabled(false);
                    limitYetiCheckBox->setEnabled(false);
                    limitBungeeCheckBox->setEnabled(false);
                    gigaWavesWidget->setEnabled(false);
                }
            });

    connect(extremeSpawnRadioButton, &QRadioButton::toggled,
            this, [=](bool checked) {
                if (checked)
                {
                    spawnLimitWidget->setEnabled(true);
                    limitFlagCheckBox->setEnabled(true);
                    limitYetiCheckBox->setEnabled(true);
                    limitBungeeCheckBox->setEnabled(true);
                    if (limitGigaCheckBox->isChecked())
                        gigaWavesWidget->setEnabled(true);
                }
            });

    connect(simulateSpawnRadioButton, &QRadioButton::toggled,
            this, [=](bool checked) {
                if (checked)
                {
                    spawnLimitWidget->setEnabled(true);
                    limitFlagCheckBox->setEnabled(false);
                    limitYetiCheckBox->setEnabled(false);
                    limitBungeeCheckBox->setEnabled(false);
                    if (limitGigaCheckBox->isChecked())
                        gigaWavesWidget->setEnabled(true);
                }
            });

    connect(limitGigaCheckBox, &QCheckBox::stateChanged,
            this, [=](int state) {
                if ((state == Qt::Checked)            //
                    && !waveCheckBox[0]->isChecked()  //
                    && !waveCheckBox[1]->isChecked()  //
                    && !waveCheckBox[2]->isChecked()  //
                    && !waveCheckBox[3]->isChecked()  //
                    && !waveCheckBox[4]->isChecked()  //
                    && !waveCheckBox[5]->isChecked()  //
                    && !waveCheckBox[6]->isChecked()  //
                    && !waveCheckBox[7]->isChecked()  //
                    && !waveCheckBox[8]->isChecked()  //
                    && !waveCheckBox[9]->isChecked()  //
                    && !waveCheckBox[10]->isChecked() //
                    && !waveCheckBox[11]->isChecked() //
                    && !waveCheckBox[12]->isChecked() //
                    && !waveCheckBox[13]->isChecked() //
                    && !waveCheckBox[14]->isChecked() //
                    && !waveCheckBox[15]->isChecked() //
                    && !waveCheckBox[16]->isChecked() //
                    && !waveCheckBox[17]->isChecked() //
                    && !waveCheckBox[18]->isChecked() //
                    && !waveCheckBox[19]->isChecked())
                {
                    bool speedChangeWaves[20] = {true, true, true, true, true, true, true, true, true,                //
                                                 true, false, false, false, false, false, false, false, false, false, //
                                                 true};
                    for (size_t i = 0; i < 20; i++)
                        waveCheckBox[i]->setChecked(speedChangeWaves[i]);
                }

                if (state == Qt::Checked)
                    gigaWavesWidget->setEnabled(true);
                else if (state == Qt::Unchecked)
                    gigaWavesWidget->setEnabled(false);
            });

    connect(spawnAddButton, &QPushButton::clicked,
            this, [=]() {
                QString spawn_item = spawnZombieCombo->currentText();
                QList<QListWidgetItem *> item_list = spawnListWidget->findItems(spawn_item, Qt::MatchContains);
                if (item_list.count() == 0)
                    spawnListWidget->addItem(spawn_item);
            });

    connect(spawnDeleteButton, &QPushButton::clicked,
            this, [=]() {
                int row = spawnListWidget->currentRow();
                if (row != -1)
                {
                    QListWidgetItem *item_take = spawnListWidget->takeItem(row);
                    delete item_take;
                }
                spawnListWidget->setCurrentRow(-1);
            });

    connect(spawnClearButton, &QPushButton::clicked,
            this, [=]() {
                spawnListWidget->clear();
            });

    connect(briefModeButton, &QPushButton::clicked,
            this, &SpawnDetailedPage::SwitchToBriefPage);

    connect(randomSeedButton, &QPushButton::clicked,
            this, [=]() {
                bool ok;
                int random_seed = randomSeedLineEdit->text().toInt(&ok, 16);
                if (ok)
                {
                    emit SetRandomSeed(random_seed);
                    std::array<bool, 33> zombies = {false}; // useless
                    emit InternalSpawn(zombies, true);
                }
            });

    connect(spawnResetButton, &QPushButton::clicked,
            this, [=]() {
                std::array<bool, 33> zombies = {false}; // useless
                emit InternalSpawn(zombies, true);
            });

    connect(spawnSetButton, &QPushButton::clicked,
            this, [=]() {
                std::array<bool, 33> zombies = GetZombies();

                if (naturalSpawnRadioButton->isChecked())
                    emit InternalSpawn(zombies, false);
                else
                {
                    bool simulate = simulateSpawnRadioButton->isChecked();
                    bool limit_flag = limitFlagCheckBox->isChecked();
                    bool limit_yeti = limitYetiCheckBox->isChecked();
                    bool limit_bungee = limitBungeeCheckBox->isChecked();
                    bool limit_giga = limitGigaCheckBox->isChecked();
                    if (simulate)
                    {
                        zombies[0] = true;
                        zombies[1] = true;
                        limit_flag = true;
                        limit_yeti = true;
                        limit_bungee = true;
                    }
                    std::array<bool, 20> giga_waves = GetGigaWaves();
                    emit CustomizeSpawn(zombies, simulate, limit_flag, limit_yeti, limit_bungee, limit_giga, giga_waves);
                }
            });
}

void SpawnDetailedPage::TranslateUI()
{
    randomSeedButton->setText(tr("Set Seed"));

    SET_COMBO_TEXT(spawnZombieCombo, List::Get().zombieList, 33);
    spawnAddButton->setText(tr("Add >>"));
    spawnDeleteButton->setText(tr("<< Delete"));
    spawnClearButton->setText(tr("Clear List"));

    naturalSpawnRadioButton->setText(tr("Natural Spawn"));
    extremeSpawnRadioButton->setText(tr("Extreme Spawn"));
    simulateSpawnRadioButton->setText(tr("Simulate Natural Spawn"));

    limitFlagCheckBox->setText(tr("Limit Flag"));
    limitYetiCheckBox->setText(tr("Limit Yeti"));
    limitBungeeCheckBox->setText(tr("Limit Bungee"));
    limitGigaCheckBox->setText(tr("Limit Giga"));

    spawnResetButton->setText(tr("Reset"));
    spawnSetButton->setText(tr("Set"));

    briefModeButton->setText(tr("Switch Brief Mode"));
}

std::array<bool, 33> SpawnDetailedPage::GetZombies()
{
    std::array<bool, 33> zombies = {false};
    for (int i = 0; i < 33; i++)
        zombies[i] = spawnListWidget->findItems(List::Get().zombieList[i], Qt::MatchContains).count() > 0 ? true : false;
    return zombies;
}

void SpawnDetailedPage::SetZombies(std::array<bool, 33> zombies)
{
    spawnListWidget->clear();
    for (size_t i = 0; i < 33; i++)
    {
        if (zombies[i])
        {
            QString spawn_item = List::Get().zombieList[i];
            spawnListWidget->addItem(spawn_item);
        }
    }
}

std::array<bool, 20> SpawnDetailedPage::GetGigaWaves()
{
    std::array<bool, 20> giga_waves = {false};
    for (size_t i = 0; i < 20; i++)
        giga_waves[i] = waveCheckBox[i]->isChecked();
    return giga_waves;
}

void SpawnDetailedPage::ShowGigaWaves(std::array<bool, 20> giga_waves)
{
    for (size_t i = 0; i < 20; i++)
        waveCheckBox[i]->setChecked(giga_waves[i]);
}

void SpawnDetailedPage::ShowRandomSeed(int random_seed)
{
    QString seed_text = QString("%1").arg(random_seed, 8, 16, QLatin1Char('0')).toUpper();
    randomSeedLineEdit->setText(seed_text);
}

SpawnPage::SpawnPage(QWidget *parent)
    : QStackedWidget(parent)
{
    briefPage = new SpawnBriefPage(this);
    detailedPage = new SpawnDetailedPage(this);

    addWidget(briefPage);
    addWidget(detailedPage);

    connect(briefPage, &SpawnBriefPage::SwitchToDetailedPage,
            this, [=]() {
                setCurrentWidget(detailedPage);
                if (true) // TODO
                {
                    auto src_zombies = briefPage->GetZombies();
                    auto dst_zombies = detailedPage->GetZombies();

                    int count = 0;
                    for (size_t i = 0; i < 33; i++)
                        if (dst_zombies[i])
                            count++;

                    if (count == 0)
                        detailedPage->SetZombies(src_zombies);
                }
            });

    connect(detailedPage, &SpawnDetailedPage::SwitchToBriefPage,
            this, [=]() {
                setCurrentWidget(briefPage);
                if (true) // TODO
                {
                    auto src_zombies = detailedPage->GetZombies();
                    auto dst_zombies = briefPage->GetZombies();

                    int count = 0;
                    for (size_t i = 0; i < 33; i++)
                        if (dst_zombies[i])
                            count++;

                    if (count == 0)
                        briefPage->SetZombies(src_zombies);
                }
            });

    connect(briefPage, &SpawnBriefPage::InternalSpawn,
            this, &SpawnPage::InternalSpawn);

    connect(briefPage, &SpawnBriefPage::CustomizeSpawn,
            this, &SpawnPage::CustomizeSpawn);

    connect(detailedPage, &SpawnDetailedPage::InternalSpawn,
            this, &SpawnPage::InternalSpawn);

    connect(detailedPage, &SpawnDetailedPage::CustomizeSpawn,
            this, &SpawnPage::CustomizeSpawn);

    connect(briefPage, &SpawnBriefPage::GetRandomSeed,
            this, &SpawnPage::GetRandomSeed);

    connect(detailedPage, &SpawnDetailedPage::SetRandomSeed,
            this, &SpawnPage::SetRandomSeed);
}

void SpawnPage::TranslateUI()
{
    briefPage->TranslateUI();
    detailedPage->TranslateUI();
}

std::array<bool, 33> SpawnPage::GetBriefZombies()
{
    return briefPage->GetZombies();
}

std::array<bool, 33> SpawnPage::GetDetailedZombies()
{
    return detailedPage->GetZombies();
}

void SpawnPage::SetBriefZombies(std::array<bool, 33> zombies)
{
    briefPage->SetZombies(zombies);
}

void SpawnPage::SetDetailedZombies(std::array<bool, 33> zombies)
{
    detailedPage->SetZombies(zombies);
}

void SpawnPage::ShowRandomSeed(int random_seed)
{
    detailedPage->ShowRandomSeed(random_seed);
}

void SpawnPage::ShowGigaWaves(std::array<bool, 20> giga_waves)
{
    detailedPage->ShowGigaWaves(giga_waves);
}

void SpawnPage::SwitchLayout(bool on)
{
    briefPage->SwitchLayout(on);
}

void SpawnPage::LimitSpawnCount(bool on)
{
    briefPage->LimitSpawnCount(on);
}

// Slots

SlotsPage::SlotsPage(QWidget *parent)
    : QWidget(parent)
{
    slotsCountLabel = new QLabel(this);
    slotsCountSpinBox = new QSpinBox(this);
    slotsCountSpinBox->setRange(1, 10);
    slotsCountSpinBox->setSingleStep(1);
    slotsCountSpinBox->setValue(10);
    slotsCountButton = new QPushButton(this);
    hideSlotsCheckBox = new QCheckBox(this);
    showShovelCheckBox = new QCheckBox(this);
    lockShovelCheckBox = new QCheckBox(this);

    slotsSlotCombo = new QComboBox(this);
    slotsSeedCombo = new QComboBox(this);
    slotsSetButton = new QPushButton(this);
    imitaterCheckBox = new QCheckBox(this);
    hideSlotsSeedCheckBox = new QCheckBox(this);

    ignoreSunCheckBox = new QCheckBox(this);
    slotsNoCdCheckBox = new QCheckBox(this);
    purpleSeedUnlimitedCheckBox = new QCheckBox(this);
    plantingFreelyCheckBox = new QCheckBox(this);
    beltNoDelayCheckBox = new QCheckBox(this);

    seedLabel = new QLabel(this);
    seedLabel->setAlignment(Qt::AlignCenter);
    seedCombo = new QComboBox(this);

    seedSpeedLabel = new QLabel(this);
    seedSpeedLabel->setAlignment(Qt::AlignCenter);
    seedSpeedLineEdit = new QLineEdit(this);
    seedSpeedLineEdit->setText("");
    seedSpeedLineEdit->setValidator(new QIntValidator(0, 999999999, this));
    seedSpeedButton = new QPushButton(this);

    seedCostLabel = new QLabel(this);
    seedCostLabel->setAlignment(Qt::AlignCenter);
    seedCostLineEdit = new QLineEdit(this);
    seedCostLineEdit->setText("");
    seedCostLineEdit->setValidator(new QIntValidator(0, 999999999, this));
    seedCostButton = new QPushButton(this);

    seedRechargeLabel = new QLabel(this);
    seedRechargeLabel->setAlignment(Qt::AlignCenter);
    seedRechargeLineEdit = new QLineEdit(this);
    seedRechargeLineEdit->setText("");
    seedRechargeLineEdit->setValidator(new QIntValidator(0, 999999999, this));
    seedRechargeButton = new QPushButton(this);

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(slotsCountLabel, 0, 0, 1, 1);
    mainLayout->addWidget(slotsCountSpinBox, 0, 1, 1, 1);
    mainLayout->addWidget(slotsCountButton, 0, 2, 1, 1);
    mainLayout->addWidget(hideSlotsCheckBox, 0, 3, 1, 1);
    mainLayout->addWidget(showShovelCheckBox, 0, 4, 1, 1);
    mainLayout->addWidget(lockShovelCheckBox, 0, 5, 1, 1);
    mainLayout->addWidget(slotsSlotCombo, 1, 0, 1, 1);
    mainLayout->addWidget(slotsSeedCombo, 1, 1, 1, 2);
    mainLayout->addWidget(slotsSetButton, 1, 3, 1, 1);
    mainLayout->addWidget(imitaterCheckBox, 1, 4, 1, 1);
    mainLayout->addWidget(hideSlotsSeedCheckBox, 1, 5, 1, 1);
    mainLayout->addWidget(ignoreSunCheckBox, 2, 0, 1, 1);
    mainLayout->addWidget(slotsNoCdCheckBox, 2, 1, 1, 1);
    mainLayout->addWidget(purpleSeedUnlimitedCheckBox, 2, 2, 1, 1);
    mainLayout->addWidget(plantingFreelyCheckBox, 2, 3, 1, 1);
    mainLayout->addWidget(beltNoDelayCheckBox, 2, 4, 1, 1);
    mainLayout->addWidget(seedLabel, 3, 0, 1, 1);
    mainLayout->addWidget(seedCombo, 3, 1, 1, 2);
    mainLayout->addWidget(seedSpeedLabel, 3, 3, 1, 1);
    mainLayout->addWidget(seedSpeedLineEdit, 3, 4, 1, 1);
    mainLayout->addWidget(seedSpeedButton, 3, 5, 1, 1);
    mainLayout->addWidget(seedCostLabel, 4, 0, 1, 1);
    mainLayout->addWidget(seedCostLineEdit, 4, 1, 1, 1);
    mainLayout->addWidget(seedCostButton, 4, 2, 1, 1);
    mainLayout->addWidget(seedRechargeLabel, 4, 3, 1, 1);
    mainLayout->addWidget(seedRechargeLineEdit, 4, 4, 1, 1);
    mainLayout->addWidget(seedRechargeButton, 4, 5, 1, 1);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    connect(slotsSlotCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [=](int index) {
                emit GetSlotsSeed(index);
                emit GetSlotsVisible(index);
            });

    connect(slotsSeedCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [=](int index) {
                if (index >= 53)
                    imitaterCheckBox->setEnabled(false); // Zombies have no imitater.
                else
                    imitaterCheckBox->setEnabled(true);
                if (index == 53 || index == 54 || index == 55 || index == 56 || index == 57 || index == 75)
                    slotsSetButton->setEnabled(false); // Will crash.
                else
                    slotsSetButton->setEnabled(true);
            });

    connect(slotsCountButton, &QPushButton::clicked,
            this, [=]() {
                int num = slotsCountSpinBox->text().toInt();
                emit SetSlotsCount(num);
            });

    connect(hideSlotsCheckBox, &QCheckBox::clicked,
            this, &SlotsPage::HideSlots);

    connect(showShovelCheckBox, &QPushButton::clicked,
            this, &SlotsPage::ShowShovel);

    connect(lockShovelCheckBox, &QCheckBox::clicked,
            this, &SlotsPage::LockShovel);

    connect(slotsSetButton, &QPushButton::clicked,
            this, [=]() {
                int slot = slotsSlotCombo->currentIndex();
                int type = slotsSeedCombo->currentIndex();
                bool imitater = imitaterCheckBox->isChecked();
                emit SetSlotsSeed(slot, type, imitater);
            });

    connect(hideSlotsSeedCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                int slot = slotsSlotCombo->currentIndex();
                bool visible = !checked;
                emit SetSlotsVisible(slot, visible);
            });

    connect(ignoreSunCheckBox, &QCheckBox::clicked,
            this, &SlotsPage::IgnoreSun);

    connect(slotsNoCdCheckBox, &QCheckBox::clicked,
            this, &SlotsPage::SlotsNoCoolDown);

    connect(purpleSeedUnlimitedCheckBox, &QCheckBox::clicked,
            this, &SlotsPage::PurpleSeedUnlimited);

    connect(plantingFreelyCheckBox, &QCheckBox::clicked,
            this, &SlotsPage::PlantingFreely);

    connect(beltNoDelayCheckBox, &QCheckBox::clicked,
            this, &SlotsPage::BeltNoDelay);

    connect(seedCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=](int index) {
                emit GetSpeed(index);
                emit GetCost(index);
                emit GetRecharge(index);
            });

    connect(seedSpeedButton, &QPushButton::clicked,
            this, [=]() {
                int index = seedCombo->currentIndex();
                int speed = seedSpeedLineEdit->text().toInt();
                emit SetSpeed(index, speed);
            });

    connect(seedCostButton, &QPushButton::clicked,
            this, [=]() {
                int index = seedCombo->currentIndex();
                int cost = seedCostLineEdit->text().toInt();
                emit SetCost(index, cost);
            });

    connect(seedRechargeButton, &QPushButton::clicked,
            this, [=]() {
                int index = seedCombo->currentIndex();
                int recharge = seedRechargeLineEdit->text().toInt();
                emit SetRecharge(index, recharge);
            });
}

void SlotsPage::TranslateUI()
{
    slotsCountLabel->setText(tr("Count of Slots"));
    slotsCountButton->setText(tr("Set"));

    hideSlotsCheckBox->setText(tr("Hide Slots"));
    showShovelCheckBox->setText(tr("Show Shovel"));
    lockShovelCheckBox->setText(tr("Lock Shovel"));

    SET_COMBO_TEXT(slotsSlotCombo, List::Get().slotsIndexList, 10);
    SET_COMBO_TEXT(slotsSeedCombo, List::Get().seedList, 76);
    slotsSetButton->setText(tr("Set"));
    imitaterCheckBox->setText(tr("Imitater"));
    hideSlotsSeedCheckBox->setText(tr("Hide"));

    ignoreSunCheckBox->setText(tr("Ignore Sun"));
    slotsNoCdCheckBox->setText(tr("Seed No Cool Down"));
    purpleSeedUnlimitedCheckBox->setText(tr("Purple Seed Unlimited"));
    plantingFreelyCheckBox->setText(tr("Planting Freely"));
    beltNoDelayCheckBox->setText(tr("Belt No Delay"));

    seedLabel->setText(tr("Seed"));
    SET_COMBO_TEXT(seedCombo, List::Get().seedList, 48);

    seedSpeedLabel->setText(tr("Speed"));
    seedSpeedButton->setText(tr("Set"));

    seedCostLabel->setText(tr("Cost"));
    seedCostButton->setText(tr("Set"));

    seedRechargeLabel->setText(tr("Recharge"));
    seedRechargeButton->setText(tr("Set"));
}

void SlotsPage::ShowSlotsSeed(int type)
{
    if (type != -1)
        slotsSeedCombo->setCurrentIndex(type);
}

void SlotsPage::ShowSlotsVisible(bool visible)
{
    hideSlotsSeedCheckBox->setChecked(!visible);
}

void SlotsPage::ShowSpeed(int speed)
{
    seedSpeedLineEdit->setText(QString::number(speed));
}

void SlotsPage::ShowCost(int cost)
{
    seedCostLineEdit->setText(QString::number(cost));
}

void SlotsPage::ShowRecharge(int recharge)
{
    seedRechargeLineEdit->setText(QString::number(recharge));
}

// Scene

ScenePage::ScenePage(QWidget *parent)
    : QWidget(parent)
{
    sceneRowLabel = new QLabel(this);
    sceneColLabel = new QLabel(this);

    sceneRowSpinBox = new QSpinBox(this);
    sceneRowSpinBox->setRange(0, 9);
    sceneRowSpinBox->setSingleStep(1);
    sceneRowSpinBox->setValue(0);
    sceneColSpinBox = new QSpinBox(this);
    sceneColSpinBox->setRange(0, 99);
    sceneColSpinBox->setSingleStep(1);
    sceneColSpinBox->setValue(0);

    plantMenuCombo = new QComboBox(this);
    putPlantButton = new QPushButton(this);
    imitaterCheckBox = new QCheckBox(this);

    zombieMenuCombo = new QComboBox(this);
    putZombieButton = new QPushButton(this);

    putGraveButton = new QPushButton(this);
    putRakeButton = new QPushButton(this);

    coinTypeCombo = new QComboBox(this);
    putCoinButton = new QPushButton(this);

    putLadderButton = new QPushButton(this);
    autoLadderButton = new QPushButton(this);
    imitaterPumpkinCheckBox = new QCheckBox(this);

    blockTypeLabel = new QLabel(this);
    blockTypeCombo = new QComboBox(this);
    blockTypeButton = new QPushButton(this);

    rowTypeLabel = new QLabel(this);
    rowTypeCombo = new QComboBox(this);
    rowTypeButton = new QPushButton(this);

    sceneLabel = new QLabel(this);
    sceneCombo = new QComboBox(this);
    sceneButton = new QPushButton(this);

    startLawnMowersButton = new QPushButton(this);
    clearLawnMowersButton = new QPushButton(this);
    resetLawnMowersButton = new QPushButton(this);

    musicLabel = new QLabel(this);
    musicCombo = new QComboBox(this);
    musicButton = new QPushButton(this);

    itemCombo = new QComboBox(this);
    clearItemButton = new QPushButton(this);

    // Set default status.
    imitaterPumpkinCheckBox->setChecked(true);

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(sceneRowLabel, 0, 0, 1, 2);
    mainLayout->addWidget(sceneColLabel, 1, 0, 1, 2);
    mainLayout->addWidget(sceneRowSpinBox, 0, 2, 1, 2);
    mainLayout->addWidget(sceneColSpinBox, 1, 2, 1, 2);
    mainLayout->addWidget(plantMenuCombo, 0, 4, 1, 4);
    mainLayout->addWidget(putPlantButton, 0, 8, 1, 2);
    mainLayout->addWidget(imitaterCheckBox, 0, 10, 1, 2);
    mainLayout->addWidget(zombieMenuCombo, 1, 4, 1, 4);
    mainLayout->addWidget(putZombieButton, 1, 8, 1, 2);
    mainLayout->addWidget(putGraveButton, 2, 0, 1, 2);
    mainLayout->addWidget(putRakeButton, 2, 2, 1, 2);
    mainLayout->addWidget(coinTypeCombo, 2, 4, 1, 2);
    mainLayout->addWidget(putCoinButton, 2, 6, 1, 2);
    mainLayout->addWidget(putLadderButton, 2, 8, 1, 2);
    mainLayout->addWidget(autoLadderButton, 2, 10, 1, 2);
    mainLayout->addWidget(imitaterPumpkinCheckBox, 1, 10, 1, 2);
    mainLayout->addWidget(blockTypeLabel, 3, 0, 1, 2);
    mainLayout->addWidget(blockTypeCombo, 3, 2, 1, 2);
    mainLayout->addWidget(blockTypeButton, 3, 4, 1, 2);
    mainLayout->addWidget(rowTypeLabel, 3, 6, 1, 2);
    mainLayout->addWidget(rowTypeCombo, 3, 8, 1, 2);
    mainLayout->addWidget(rowTypeButton, 3, 10, 1, 2);
    mainLayout->addWidget(sceneLabel, 4, 0, 1, 2);
    mainLayout->addWidget(sceneCombo, 4, 2, 1, 2);
    mainLayout->addWidget(sceneButton, 4, 4, 1, 2);
    mainLayout->addWidget(startLawnMowersButton, 4, 6, 1, 2);
    mainLayout->addWidget(clearLawnMowersButton, 4, 8, 1, 2);
    mainLayout->addWidget(resetLawnMowersButton, 4, 10, 1, 2);
    mainLayout->addWidget(musicLabel, 5, 0, 1, 2);
    mainLayout->addWidget(musicCombo, 5, 2, 1, 3);
    mainLayout->addWidget(musicButton, 5, 5, 1, 2);
    mainLayout->addWidget(itemCombo, 5, 8, 1, 2);
    mainLayout->addWidget(clearItemButton, 5, 10, 1, 2);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    connect(putPlantButton, &QPushButton::clicked,
            this, [=]() {
                int row = sceneRowSpinBox->text().toInt() - 1;
                int col = sceneColSpinBox->text().toInt() - 1;
                int type = plantMenuCombo->currentIndex();
                bool imitater = imitaterCheckBox->isChecked() || type == 48;
                emit Plant(row, col, type, imitater);
            });

    connect(putZombieButton, &QPushButton::clicked,
            this, [=]() {
                int row = sceneRowSpinBox->text().toInt() - 1;
                int col = sceneColSpinBox->text().toInt() - 1;
                int type = zombieMenuCombo->currentIndex();
                emit PutZombie(row, col, type);
            });

    connect(putGraveButton, &QPushButton::clicked,
            this, [=]() {
                int row = sceneRowSpinBox->text().toInt() - 1;
                int col = sceneColSpinBox->text().toInt() - 1;
                emit PutGrave(row, col);
            });

    connect(putRakeButton, &QPushButton::clicked,
            this, [=]() {
                int row = sceneRowSpinBox->text().toInt() - 1;
                int col = sceneColSpinBox->text().toInt() - 1;
                emit PutRake(row, col);
            });

    connect(putCoinButton, &QPushButton::clicked,
            this, [=]() {
                int row = sceneRowSpinBox->text().toInt() - 1;
                int col = sceneColSpinBox->text().toInt() - 1;
                int type = coinTypeCombo->currentIndex() + 1;
                emit PutCoin(row, col, type);
            });

    connect(putLadderButton, &QPushButton::clicked,
            this, [=]() {
                int row = sceneRowSpinBox->text().toInt() - 1;
                int col = sceneColSpinBox->text().toInt() - 1;
                emit PutLadder(row, col);
            });

    connect(autoLadderButton, &QPushButton::clicked,
            this, [=]() {
                bool imitater_pumpkin_only = imitaterPumpkinCheckBox->isChecked();
                emit AutoLadder(imitater_pumpkin_only);
            });

    connect(sceneRowSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &ScenePage::Check);

    connect(sceneColSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &ScenePage::Check);

    connect(blockTypeButton, &QPushButton::clicked,
            this, [=]() {
                int row = sceneRowSpinBox->text().toInt() - 1;
                int col = sceneColSpinBox->text().toInt() - 1;
                // 0 1 2 -> 1 2 3
                int type = blockTypeCombo->currentIndex() + 1;
                emit SetBlockType(row, col, type);
            });

    connect(rowTypeButton, &QPushButton::clicked,
            this, [=]() {
                int row = sceneRowSpinBox->text().toInt() - 1;
                int type = rowTypeCombo->currentIndex();
                emit SetRowType(row, type);
            });

    connect(sceneButton, &QPushButton::clicked,
            this, [=]() {
                int scene = sceneCombo->currentIndex();
                emit SetScene(scene);
            });

    connect(musicButton, &QPushButton::clicked,
            this, [=]() {
                int id = musicCombo->currentIndex() + 1;
                emit SetMusic(id);
            });

    connect(clearItemButton, &QPushButton::clicked,
            this, [=]() {
                int type = itemCombo->currentIndex();
                switch (type)
                {
                case 0:
                    emit ClearItems(1);
                    break;
                case 1:
                    emit ClearItems(2);
                    break;
                case 2:
                    emit ClearItems(3);
                    break;
                case 3:
                    emit ClearItems(4);
                    break;
                case 4:
                    emit ClearItems(5);
                    break;
                case 5:
                    emit ClearItems(7);
                    break;
                case 6:
                    emit ClearItems(10);
                    break;
                case 7:
                    emit ClearItems(11);
                    break;
                case 8:
                    emit ClearItems(12);
                    break;
                case 9:
                    emit ClearAllPlants();
                    break;
                case 10:
                    emit AllZombiesXXX(0);
                    break;
                default:
                    break;
                }
            });

    connect(startLawnMowersButton, &QPushButton::clicked,
            this, &ScenePage::StartLawnMowers);

    connect(clearLawnMowersButton, &QPushButton::clicked,
            this, &ScenePage::ClearLawnMowers);

    connect(resetLawnMowersButton, &QPushButton::clicked,
            this, &ScenePage::ResetLawnMowers);
}

void ScenePage::TranslateUI()
{
    sceneRowLabel->setText(tr("Row") + " (1~6)");
    sceneColLabel->setText(tr("Column") + " (1~9)");

    SET_COMBO_TEXT(plantMenuCombo, List::Get().seedList, 51);
    putPlantButton->setText(tr("Planting"));
    imitaterCheckBox->setText(tr("Imitater"));

    SET_COMBO_TEXT(zombieMenuCombo, List::Get().zombieList, 33);
    putZombieButton->setText(tr("Put"));

    putGraveButton->setText(tr("Grave"));
    putRakeButton->setText(tr("Rake"));
    SET_COMBO_TEXT(coinTypeCombo, List::Get().coinList, 6);
    putCoinButton->setText(tr("Coin"));
    putLadderButton->setText(tr("Ladder"));
    autoLadderButton->setText(tr("Ladder Pumpkin"));
    imitaterPumpkinCheckBox->setText(tr("Imitater Pumpkin Only"));

    blockTypeLabel->setText(tr("Block Type"));
    SET_COMBO_TEXT(blockTypeCombo, List::Get().blockTypeList, 3);
    blockTypeButton->setText(tr("Set"));

    rowTypeLabel->setText(tr("Row Type (Spawn)"));
    SET_COMBO_TEXT(rowTypeCombo, List::Get().rowTypeList, 3);
    rowTypeButton->setText(tr("Set"));

    sceneLabel->setText(tr("Game Scene"));
    SET_COMBO_TEXT(sceneCombo, List::Get().sceneList, 10);
    sceneButton->setText(tr("Set"));

    startLawnMowersButton->setText(tr("Start Lawn Mowers"));
    clearLawnMowersButton->setText(tr("Clear Lawn Mowers"));
    resetLawnMowersButton->setText(tr("Reset Lawn Mowers"));

    musicLabel->setText(tr("Background Music"));
    SET_COMBO_TEXT(musicCombo, List::Get().musicList, 12);
    musicButton->setText(tr("Set"));

    SET_COMBO_TEXT(itemCombo, List::Get().itemList, 11);
    clearItemButton->setText(tr("Clear"));
}

void ScenePage::Check()
{
    // row : 0 ~ 5
    // col : 0 ~ 8
    // -1 means all
    int row = sceneRowSpinBox->value() - 1;
    int col = sceneColSpinBox->value() - 1;

    if (-1 <= row && row <= 5 && -1 <= col && col <= 8)
    {
        blockTypeLabel->setEnabled(true);
        blockTypeCombo->setEnabled(true);
        blockTypeButton->setEnabled(true);
        if (row != -1 && col != -1)
            emit GetBlockType(row, col);
    }
    else
    {
        blockTypeLabel->setEnabled(false);
        blockTypeCombo->setEnabled(false);
        blockTypeButton->setEnabled(false);
    }

    if (-1 <= row && row <= 5)
    {
        rowTypeLabel->setEnabled(true);
        rowTypeCombo->setEnabled(true);
        rowTypeButton->setEnabled(true);
        if (row != -1)
            emit GetRowType(row);
    }
    else
    {
        rowTypeLabel->setEnabled(false);
        rowTypeCombo->setEnabled(false);
        rowTypeButton->setEnabled(false);
    }
}

void ScenePage::ShowBlock(int type)
{
    blockTypeCombo->setCurrentIndex(type - 1);
}

void ScenePage::ShowRow(int type)
{
    rowTypeCombo->setCurrentIndex(type);
}

void ScenePage::ShowScene(int scene)
{
    if (scene != -1)
        sceneCombo->setCurrentIndex(scene);
}

// Lineup

LineupPage::LineupPage(QWidget *parent)
    : QWidget(parent)
{
    qRegisterMetaType<std::string>("std::string");

    quickLineupModeCheckBox = new QCheckBox(this);
    quickPassButton = new QPushButton(this);
    mixModeToSurvivalEndlessButton = new QPushButton(this);

    eatAllGravesButton = new QPushButton(this);
    lilyPadOnPoolButton = new QPushButton(this);
    flowerPotOnRoofButton = new QPushButton(this);
    clearAllPlantsButton = new QPushButton(this);

    openLinkButton = new QPushButton(this);
    endlessBuildLabel = new QLabel(this);
    endlessBuildLabel->setOpenExternalLinks(true);
    endlessBuildCombo = new QComboBox(this);
    oneKeySetupButton = new QPushButton(this);

    stringTextEdit = new QPlainTextEdit(this);

    string2buildButton = new QPushButton(this);
    build2stringButton = new QPushButton(this);

    copyStringButton = new QPushButton(this);
    pasteStringButton = new QPushButton(this);

    allowSwitchSceneCheckBox = new QCheckBox(this);
    keepHpStatusCheckBox = new QCheckBox(this);

    stringTextEdit->setPlainText("0,E 1 1 0 0 0,E 2 1 0 0 0,E 3 1 0 0 0,E 4 1 0 0 0,E 5 1 0 0 0,E 5 2 0 0 0,E 1 5 0 0 0,E 1 4 0 0 0,E 2 4 0 0 0,E 3 4 0 0 0,E 4 4 0 0 0,E 5 4 0 0 0,E 5 5 0 0 0,E 1 7 0 0 0,E 2 7 0 0 0,E 3 8 0 0 0,E 4 9 0 0 0,E 5 9 0 0 0,E 1 9 0 0 0,E 2 9 0 0 0,E 4 7 0 0 0,E 5 7 0 0 0"); // LCX

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(quickLineupModeCheckBox, 0, 0, 1, 3);
    mainLayout->addWidget(quickPassButton, 0, 3, 1, 3);
    mainLayout->addWidget(mixModeToSurvivalEndlessButton, 0, 8, 1, 4);
    mainLayout->addWidget(eatAllGravesButton, 1, 0, 1, 3);
    mainLayout->addWidget(lilyPadOnPoolButton, 1, 3, 1, 3);
    mainLayout->addWidget(flowerPotOnRoofButton, 1, 6, 1, 3);
    mainLayout->addWidget(clearAllPlantsButton, 1, 9, 1, 3);
    mainLayout->addWidget(openLinkButton, 2, 0, 1, 3);
    mainLayout->addWidget(endlessBuildLabel, 2, 3, 1, 3);
    mainLayout->addWidget(endlessBuildCombo, 2, 5, 1, 4);
    mainLayout->addWidget(oneKeySetupButton, 2, 9, 1, 3);
    mainLayout->addWidget(stringTextEdit, 3, 0, 2, 8);
    mainLayout->addWidget(string2buildButton, 3, 8, 1, 4);
    mainLayout->addWidget(build2stringButton, 4, 8, 1, 4);
    mainLayout->addWidget(copyStringButton, 5, 0, 1, 2);
    mainLayout->addWidget(pasteStringButton, 5, 2, 1, 2);
    mainLayout->addWidget(allowSwitchSceneCheckBox, 5, 6, 1, 3);
    mainLayout->addWidget(keepHpStatusCheckBox, 5, 9, 1, 3);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    connect(quickLineupModeCheckBox, &QCheckBox::clicked,
            this, &LineupPage::SetQuickLineupMode);

    connect(quickPassButton, &QPushButton::clicked,
            this, &LineupPage::QuickPass);

    connect(mixModeToSurvivalEndlessButton, &QPushButton::clicked,
            this, [=]() {
                emit MixMode(13, 0);
            });

    connect(eatAllGravesButton, &QPushButton::clicked,
            this, &LineupPage::EatAllGraves);

    connect(lilyPadOnPoolButton, &QPushButton::clicked,
            this, &LineupPage::LilyPadOnPool);

    connect(flowerPotOnRoofButton, &QPushButton::clicked,
            this, &LineupPage::FlowerPotOnRoof);

    connect(clearAllPlantsButton, &QPushButton::clicked,
            this, &LineupPage::ClearAllPlants);

    connect(openLinkButton, &QPushButton::clicked,
            this, [=]() {
                QDesktopServices::openUrl(QUrl("http://lonelystar.org/ArrayDesign"));
            });

    connect(oneKeySetupButton, &QPushButton::clicked,
            this, [=]() {
                int index = endlessBuildCombo->currentIndex();
                QString text = List::Get().endlessBuildStringList[index];
                // if (StringCheck(text)) // no need
                {
                    std::string str = text.toStdString();
                    bool switchable = allowSwitchSceneCheckBox->isChecked();
                    bool keep_status = false;
                    emit SetLineup(str, switchable, keep_status);
                }
            });

    connect(string2buildButton, &QPushButton::clicked,
            this, [=]() {
                QString text = stringTextEdit->toPlainText();
                if (StringCheck(text))
                {
                    std::string str = text.toStdString();
                    bool switchable = allowSwitchSceneCheckBox->isChecked();
                    bool keep_status = keepHpStatusCheckBox->isChecked();
                    emit SetLineup(str, switchable, keep_status);
                }
                else
                {
                    QMessageBox::warning(this, tr("Error"), tr("Wrong string format."), QMessageBox::Ok);
                }
            });

    connect(build2stringButton, &QPushButton::clicked,
            this, [=]() {
                bool ignore_hp = keepHpStatusCheckBox->isChecked();
                emit GetLineup(ignore_hp);
            });

    connect(copyStringButton, &QPushButton::clicked,
            this, [=]() {
                QClipboard *clipboard = QApplication::clipboard();
                QString text = stringTextEdit->toPlainText();
                clipboard->setText(text);
            });

    connect(pasteStringButton, &QPushButton::clicked,
            this, [=]() {
                QClipboard *clipboard = QApplication::clipboard();
                QString text = clipboard->text();
                stringTextEdit->setPlainText(text);
            });

    connect(allowSwitchSceneCheckBox, &QCheckBox::clicked,
            this, [=](bool checked) {
                if (checked)
                {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle(tr("Warning"));
                    msgBox.setText(tr("Modifying game scene can cause many problems, like a texture error or a game crash. It is recommended to reopen an archive from a \"Survival(Hard)\" level with desired scene. "));
                    msgBox.setInformativeText(tr("Are you sure you want to allow the modifier to modify the scene?"));
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::No);
                    int ret = msgBox.exec();
                    if (ret == QMessageBox::No)
                        allowSwitchSceneCheckBox->setChecked(false);
                }
            });
}

void LineupPage::TranslateUI()
{
    quickLineupModeCheckBox->setText(tr("Quick Lineup Mode"));
    quickPassButton->setText(tr("Quick Pass"));
    mixModeToSurvivalEndlessButton->setText(tr("Mix Mode To Survival Endless"));

    eatAllGravesButton->setText(tr("Eat All Graves"));
    lilyPadOnPoolButton->setText(tr("Lily Pad On Pool"));
    flowerPotOnRoofButton->setText(tr("Flower Pot On Roof"));
    clearAllPlantsButton->setText(tr("Clear All Plants"));

    openLinkButton->setText(tr("Open Array Design Page"));
    endlessBuildLabel->setText(tr("<a href='https://tieba.baidu.com/p/5272254427'>Endless Build</a>"));
    SET_COMBO_TEXT(endlessBuildCombo, List::Get().endlessBuildNameList, 171);
    oneKeySetupButton->setText(tr("One Key Setup"));

    string2buildButton->setText(tr("String -> Lineup"));
    build2stringButton->setText(tr("String <- Lineup"));

    copyStringButton->setText(tr("Copy"));
    pasteStringButton->setText(tr("Paste"));

    allowSwitchSceneCheckBox->setText(tr("Allow Switch Scene"));
    keepHpStatusCheckBox->setText(tr("Keep HP Status"));
}

bool LineupPage::StringCheck(const QString &text)
{
    QRegExp reg("[0-5](,[a-fA-F0-9]{1,2} [1-6] [1-9] [0-2] [0-2] [0-1]){0,}");
    return reg.exactMatch(text);
}

void LineupPage::ShowLineup(std::string str)
{
    QString text = QString::fromStdString(str);
    stringTextEdit->clear();
    stringTextEdit->setPlainText(text);
}

// Garden

GardenPage::GardenPage(QWidget *parent)
    : QWidget(parent)
{
    qRegisterMetaType<std::vector<GardenPlant>>("std::vector<GardenPlant>");

    fertilizerUnlimitedCheckBox = new QCheckBox(this);
    bugSprayUnlimitedCheckBox = new QCheckBox(this);
    chocolateUnlimitedCheckBox = new QCheckBox(this);
    treeFoodUnlimitedCheckBox = new QCheckBox(this);

    treeHeightLabel = new QLabel(this);
    treeHeightLineEdit = new QLineEdit(this);
    treeHeightLineEdit->setValidator(new QIntValidator(0, 999999999, this));
    treeHeightButton = new QPushButton(this);

    table = new QTableWidget(this);
    table->setColumnCount(8);
    table->setRowCount(0); // 4 x 8 + 8 + 8 + 1
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->hideColumn(0); // hide index
    table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    table->resizeColumnsToContents();
    table->resizeRowsToContents();

    clearButton = new QPushButton(this);
    loadButton = new QPushButton(this);
    setButton = new QPushButton(this);

    sceneLabel = new QLabel(this);
    typeLabel = new QLabel(this);
    directionLabel = new QLabel(this);
    colorLabel = new QLabel(this);
    statusLabel = new QLabel(this);

    sceneCombo = new QComboBox(this);
    typeCombo = new QComboBox(this);
    directionCombo = new QComboBox(this);
    colorCombo = new QComboBox(this);
    statusCombo = new QComboBox(this);

    // default status
    sceneCombo->setEnabled(false);
    typeCombo->setEnabled(false);
    directionCombo->setEnabled(false);
    colorCombo->setEnabled(false);
    statusCombo->setEnabled(false);

    treeHeightLineEdit->setText("1000");

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(fertilizerUnlimitedCheckBox, 0, 0, 1, 3);
    mainLayout->addWidget(bugSprayUnlimitedCheckBox, 0, 3, 1, 3);
    mainLayout->addWidget(chocolateUnlimitedCheckBox, 0, 6, 1, 3);
    mainLayout->addWidget(treeFoodUnlimitedCheckBox, 0, 9, 1, 3);
    mainLayout->addWidget(treeHeightLabel, 1, 0, 1, 2);
    mainLayout->addWidget(treeHeightLineEdit, 1, 2, 1, 2);
    mainLayout->addWidget(treeHeightButton, 1, 4, 1, 2);
    mainLayout->addWidget(clearButton, 1, 6, 1, 2);
    mainLayout->addWidget(loadButton, 1, 8, 1, 2);
    mainLayout->addWidget(setButton, 1, 10, 1, 2);
    mainLayout->addWidget(table, 2, 0, 5, 8);
    mainLayout->addWidget(sceneLabel, 2, 8, 1, 1);
    mainLayout->addWidget(typeLabel, 3, 8, 1, 1);
    mainLayout->addWidget(directionLabel, 4, 8, 1, 1);
    mainLayout->addWidget(colorLabel, 5, 8, 1, 1);
    mainLayout->addWidget(statusLabel, 6, 8, 1, 1);
    mainLayout->addWidget(sceneCombo, 2, 9, 1, 3);
    mainLayout->addWidget(typeCombo, 3, 9, 1, 3);
    mainLayout->addWidget(directionCombo, 4, 9, 1, 3);
    mainLayout->addWidget(colorCombo, 5, 9, 1, 3);
    mainLayout->addWidget(statusCombo, 6, 9, 1, 3);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    connect(fertilizerUnlimitedCheckBox, &QCheckBox::clicked,
            this, &GardenPage::FertilizerUnlimited);

    connect(bugSprayUnlimitedCheckBox, &QCheckBox::clicked,
            this, &GardenPage::BugSprayUnlimited);

    connect(chocolateUnlimitedCheckBox, &QCheckBox::clicked,
            this, &GardenPage::ChocolateUnlimited);

    connect(treeFoodUnlimitedCheckBox, &QCheckBox::clicked,
            this, &GardenPage::TreeFoodUnlimited);

    connect(treeHeightButton, &QPushButton::clicked,
            this, [=]() {
                int height = treeHeightLineEdit->text().toInt();
                emit SetTreeHeight(height);
            });

    connect(table, &QTableWidget::itemSelectionChanged,
            this, [=]() {
                sceneCombo->blockSignals(true);
                typeCombo->blockSignals(true);
                directionCombo->blockSignals(true);
                colorCombo->blockSignals(true);
                statusCombo->blockSignals(true);

                QList<QTableWidgetItem *> items = table->selectedItems();
                int count = items.count();
                for (int i = 0; i < count; i++)
                {
                    QTableWidgetItem *item = items.at(i);
                    QString text = item->text();
                    // int row = table->row(item);
                    int col = table->column(item);
                    if (col == 3)
                    {
                        int index = List::Get().gardenSceneList.indexOf(text);
                        sceneCombo->setCurrentIndex(index);
                    }
                    else if (col == 4)
                    {
                        int index = List::Get().seedList.indexOf(text);
                        typeCombo->setCurrentIndex(index);
                    }
                    else if (col == 5)
                    {
                        int index = List::Get().directionList.indexOf(text);
                        directionCombo->setCurrentIndex(index);
                    }
                    else if (col == 6)
                    {
                        int index = List::Get().colorList.indexOf(text);
                        colorCombo->setCurrentIndex(index);
                    }
                    else if (col == 7)
                    {
                        int index = List::Get().plantStatusList.indexOf(text);
                        statusCombo->setCurrentIndex(index);
                    }
                }

                sceneCombo->blockSignals(false);
                typeCombo->blockSignals(false);
                directionCombo->blockSignals(false);
                colorCombo->blockSignals(false);
                statusCombo->blockSignals(false);

                sceneCombo->setEnabled(true);
                typeCombo->setEnabled(true);
                directionCombo->setEnabled(true);
                colorCombo->setEnabled(true);
                statusCombo->setEnabled(true);
            });

    connect(sceneCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &GardenPage::UpdateContent);

    connect(typeCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &GardenPage::UpdateContent);

    connect(directionCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &GardenPage::UpdateContent);

    connect(colorCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &GardenPage::UpdateContent);

    connect(statusCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &GardenPage::UpdateContent);

    connect(clearButton, &QPushButton::clicked,
            this, [=]() {
                table->clearContents();
                table->setRowCount(0);
                table->horizontalHeader()->setVisible(false);
            });

    connect(loadButton, &QPushButton::clicked,
            this, [=]() {
                table->clearContents();
                table->setRowCount(0);
                table->horizontalHeader()->setVisible(false);

                sceneCombo->setEnabled(false);
                typeCombo->setEnabled(false);
                directionCombo->setEnabled(false);
                colorCombo->setEnabled(false);
                statusCombo->setEnabled(false);

                emit GetGardenPlants();
            });

    connect(setButton, &QPushButton::clicked,
            this, [=]() {
                std::vector<GardenPlant> plants;

                int count = table->rowCount();
                for (int r = 0; r < count; r++)
                {
                    GardenPlant plant;
                    plant.index = table->item(r, 0)->text().toInt() - 1;
                    plant.row = table->item(r, 1)->text().toInt() - 1;
                    plant.col = table->item(r, 2)->text().toInt() - 1;
                    plant.scene = List::Get().gardenSceneList.indexOf(table->item(r, 3)->text());
                    plant.type = List::Get().seedList.indexOf(table->item(r, 4)->text());
                    plant.direction = List::Get().directionList.indexOf(table->item(r, 5)->text());
                    plant.color = List::Get().colorList.indexOf(table->item(r, 6)->text());
                    plant.status = List::Get().plantStatusList.indexOf(table->item(r, 7)->text());
                    plants.push_back(plant);
                }

                if (count > 0)
                    emit SetGardenPlants(plants);
            });
}

void GardenPage::TranslateUI()
{
    fertilizerUnlimitedCheckBox->setText(tr("Fertilizer Unlimited"));
    bugSprayUnlimitedCheckBox->setText(tr("Bug Spray Unlimited"));
    chocolateUnlimitedCheckBox->setText(tr("Chocolate Unlimited"));
    treeFoodUnlimitedCheckBox->setText(tr("Tree Food Unlimited"));

    treeHeightLabel->setText(tr("Tree Height"));
    treeHeightButton->setText(tr("Set"));

    clearButton->setText(tr("Clear"));
    loadButton->setText(tr("Load"));
    setButton->setText(tr("Set"));

    sceneLabel->setText(tr("Scene"));
    typeLabel->setText(tr("Type"));
    directionLabel->setText(tr("Direction"));
    colorLabel->setText(tr("Color"));
    statusLabel->setText(tr("Status"));

    SET_COMBO_TEXT(sceneCombo, List::Get().gardenSceneList, 4);
    SET_COMBO_TEXT(typeCombo, List::Get().seedList, 48);
    SET_COMBO_TEXT(directionCombo, List::Get().directionList, 2);
    SET_COMBO_TEXT(colorCombo, List::Get().colorList, 13);
    SET_COMBO_TEXT(statusCombo, List::Get().plantStatusList, 4);

    table->setHorizontalHeaderLabels(List::Get().gardenHeadersList);

    if (table->rowCount() > 0)
        emit loadButton->clicked();
}

void GardenPage::ShowGardenPlants(std::vector<GardenPlant> plants)
{
    size_t count = plants.size();
    table->setRowCount(count);

    if (count > 0)
        table->horizontalHeader()->setVisible(true);

    for (size_t r = 0; r < count; r++)
    {
        GardenPlant plant = plants[r];
        QString index = QString::number(plant.index + 1);
        QString row = QString::number(plant.row + 1);
        QString col = QString::number(plant.col + 1);
        QString type = List::Get().seedList[plant.type];
        QString scene = List::Get().gardenSceneList[plant.scene];
        QString direction = List::Get().directionList[plant.direction];
        QString color = List::Get().colorList[plant.color];
        QString status = List::Get().plantStatusList[plant.status];

        table->setItem(r, 0, new QTableWidgetItem(index));
        table->setItem(r, 1, new QTableWidgetItem(row));
        table->setItem(r, 2, new QTableWidgetItem(col));
        table->setItem(r, 3, new QTableWidgetItem(scene));
        table->setItem(r, 4, new QTableWidgetItem(type));
        table->setItem(r, 5, new QTableWidgetItem(direction));
        table->setItem(r, 6, new QTableWidgetItem(color));
        table->setItem(r, 7, new QTableWidgetItem(status));
    }

    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

void GardenPage::UpdateContent()
{
    QList<QTableWidgetItem *> items = table->selectedItems();
    int count = items.count();
    for (int i = 0; i < count; i++)
    {
        QTableWidgetItem *item = items.at(i);
        // int row = table->row(item);
        int col = table->column(item);
        if (col == 3)
            item->setText(List::Get().gardenSceneList[sceneCombo->currentIndex()]);
        else if (col == 4)
            item->setText(List::Get().seedList[typeCombo->currentIndex()]);
        else if (col == 5)
            item->setText(List::Get().directionList[directionCombo->currentIndex()]);
        else if (col == 6)
            item->setText(List::Get().colorList[colorCombo->currentIndex()]);
        else if (col == 7)
            item->setText(List::Get().plantStatusList[statusCombo->currentIndex()]);
    }
}

// Vase

VasePage::VasePage(QWidget *parent)
    : QWidget(parent)
{
    qRegisterMetaType<std::vector<Vase>>("std::vector<Vase>");

    vaseCountLabel = new QLabel(this);
    clearButton = new QPushButton(this);
    loadButton = new QPushButton(this);
    setButton = new QPushButton(this);

    table = new QTableWidget(this);
    table->setColumnCount(6);
    table->setRowCount(0); // 6 x 9
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->hideColumn(0); // hide index
    table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    table->resizeColumnsToContents();
    table->resizeRowsToContents();

    rowLabel = new QLabel(this);
    colLabel = new QLabel(this);
    vaseSkinLabel = new QLabel(this);
    vaseTypeLabel = new QLabel(this);
    vaseContentPlantLabel = new QLabel(this);
    vaseContentZombieLabel = new QLabel(this);
    vaseContentSunLabel = new QLabel(this);

    rowCombo = new QComboBox(this);
    colCombo = new QComboBox(this);
    vaseSkinCombo = new QComboBox(this);
    vaseTypeCombo = new QComboBox(this);
    vaseContentPlantCombo = new QComboBox(this);
    vaseContentZombieCombo = new QComboBox(this);
    vaseContentSunLineEdit = new QLineEdit(this);
    vaseContentSunLineEdit->setValidator(new QIntValidator(0, 99, this));

    // set default status
    rowCombo->setEnabled(false);
    colCombo->setEnabled(false);
    vaseSkinCombo->setEnabled(false);
    vaseTypeCombo->setEnabled(false);
    vaseContentPlantCombo->setEnabled(false);
    vaseContentZombieCombo->setEnabled(false);
    vaseContentSunLineEdit->setEnabled(false);

    vaseContentSunLineEdit->setText("3");

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(vaseCountLabel, 0, 0, 1, 4);
    mainLayout->addWidget(clearButton, 0, 6, 1, 2);
    mainLayout->addWidget(loadButton, 0, 8, 1, 2);
    mainLayout->addWidget(setButton, 0, 10, 1, 2);
    mainLayout->addWidget(table, 1, 0, 6, 8);
    mainLayout->addWidget(rowLabel, 1, 8, 1, 1);
    mainLayout->addWidget(colLabel, 1, 10, 1, 1);
    mainLayout->addWidget(vaseSkinLabel, 2, 8, 1, 1);
    mainLayout->addWidget(vaseTypeLabel, 3, 8, 1, 1);
    mainLayout->addWidget(vaseContentPlantLabel, 4, 8, 1, 1);
    mainLayout->addWidget(vaseContentZombieLabel, 5, 8, 1, 1);
    mainLayout->addWidget(vaseContentSunLabel, 6, 8, 1, 1);
    mainLayout->addWidget(rowCombo, 1, 9, 1, 1);
    mainLayout->addWidget(colCombo, 1, 11, 1, 1);
    mainLayout->addWidget(vaseSkinCombo, 2, 9, 1, 3);
    mainLayout->addWidget(vaseTypeCombo, 3, 9, 1, 3);
    mainLayout->addWidget(vaseContentPlantCombo, 4, 9, 1, 3);
    mainLayout->addWidget(vaseContentZombieCombo, 5, 9, 1, 3);
    mainLayout->addWidget(vaseContentSunLineEdit, 6, 9, 1, 3);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    connect(rowCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &VasePage::UpdateContent);

    connect(colCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &VasePage::UpdateContent);

    connect(vaseSkinCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &VasePage::UpdateContent);

    connect(vaseTypeCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &VasePage::UpdateContent);

    connect(vaseContentPlantCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &VasePage::UpdateContent);

    connect(vaseContentZombieCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &VasePage::UpdateContent);

    connect(vaseContentSunLineEdit, &QLineEdit::textChanged,
            this, &VasePage::UpdateContent);

    connect(clearButton, &QPushButton::clicked,
            this, [=]() {
                table->clearContents();
                table->setRowCount(0);
                table->horizontalHeader()->setVisible(false);

                vaseCountLabel->setText(tr("Vase Count") + ": " + "0");
            });

    connect(loadButton, &QPushButton::clicked,
            this, [=]() {
                table->clearContents();
                table->setRowCount(0);
                table->horizontalHeader()->setVisible(false);

                rowCombo->setEnabled(false);
                colCombo->setEnabled(false);
                vaseSkinCombo->setEnabled(false);
                vaseTypeCombo->setEnabled(false);
                vaseContentPlantCombo->setEnabled(false);
                vaseContentZombieCombo->setEnabled(false);
                vaseContentSunLineEdit->setEnabled(false);

                emit GetVases();
            });

    connect(setButton, &QPushButton::clicked,
            this, [=]() {
                std::vector<Vase> vases;

                int count = table->rowCount();
                for (int r = 0; r < count; r++)
                {
                    Vase vase;
                    vase.index = table->item(r, 0)->text().toInt() - 1;
                    vase.row = table->item(r, 1)->text().toInt() - 1;
                    vase.col = table->item(r, 2)->text().toInt() - 1;
                    vase.skin = List::Get().vaseSkinList.indexOf(table->item(r, 3)->text()) + 3 - 1;
                    vase.type = List::Get().vaseTypeList.indexOf(table->item(r, 4)->text());
                    if (vase.type == 1) // Plant
                        vase.content_plant = List::Get().seedList.indexOf(table->item(r, 5)->text());
                    else if (vase.type == 2) // Zombie
                        vase.content_zombie = List::Get().zombieList.indexOf(table->item(r, 5)->text());
                    else if (vase.type == 3) // Sun
                        vase.content_sun = table->item(r, 5)->text().toInt();
                    // else if (vase.type == 0) // Empty
                    vases.push_back(vase);
                }

                if (count > 0)
                    emit SetVases(vases);
            });

    connect(table, &QTableWidget::itemSelectionChanged,
            this, [=]() {
                rowCombo->blockSignals(true);
                colCombo->blockSignals(true);
                vaseSkinCombo->blockSignals(true);
                vaseTypeCombo->blockSignals(true);
                vaseContentPlantCombo->blockSignals(true);
                vaseContentZombieCombo->blockSignals(true);
                vaseContentSunLineEdit->blockSignals(true);

                QList<QTableWidgetItem *> items = table->selectedItems();
                int count = items.count();
                for (int i = 0; i < count; i++)
                {
                    QTableWidgetItem *item = items.at(i);
                    QString text = item->text();
                    int row = table->row(item);
                    int col = table->column(item);
                    if (col == 1)
                    {
                        int index = text.toInt() - 1;
                        rowCombo->setCurrentIndex(index);
                    }
                    else if (col == 2)
                    {
                        int index = text.toInt() - 1;
                        colCombo->setCurrentIndex(index);
                    }
                    else if (col == 3)
                    {
                        int index = List::Get().vaseSkinList.indexOf(text);
                        vaseSkinCombo->setCurrentIndex(index);
                    }
                    else if (col == 4)
                    {
                        int index = List::Get().vaseTypeList.indexOf(text);
                        vaseTypeCombo->setCurrentIndex(index);
                        SetComboEnabled();
                    }
                    else if (col == 5)
                    {
                        QString type = table->item(row, col - 1)->text();
                        if (type == List::Get().vaseTypeList[1]) // Plant
                        {
                            int index = List::Get().seedList.indexOf(text);
                            vaseContentPlantCombo->setCurrentIndex(index);
                        }
                        else if (type == List::Get().vaseTypeList[2]) // Zombie
                        {
                            int index = List::Get().zombieList.indexOf(text);
                            vaseContentZombieCombo->setCurrentIndex(index);
                        }
                        else if (type == List::Get().vaseTypeList[3]) // Sun
                        {
                            vaseContentSunLineEdit->setText(text);
                        }
                        //  else if (type == List::Get().vaseTypeList[0]) // Empty
                    }
                }

                rowCombo->blockSignals(false);
                colCombo->blockSignals(false);
                vaseSkinCombo->blockSignals(false);
                vaseTypeCombo->blockSignals(false);
                vaseContentPlantCombo->blockSignals(false);
                vaseContentZombieCombo->blockSignals(false);
                vaseContentSunLineEdit->blockSignals(false);

                rowCombo->setEnabled(true);
                colCombo->setEnabled(true);
                vaseSkinCombo->setEnabled(true);
                vaseTypeCombo->setEnabled(true);
                // vaseContentPlantCombo->setEnabled(true);
                // vaseContentZombieCombo->setEnabled(true);
                // vaseContentSunLineEdit->setEnabled(true);
            });

    connect(vaseTypeCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &VasePage::SetComboEnabled);
}

void VasePage::TranslateUI()
{
    vaseCountLabel->setText(tr("Vase Count") + ": " + "0");
    clearButton->setText(tr("Clear"));
    loadButton->setText(tr("Load"));
    setButton->setText(tr("Set"));

    rowLabel->setText(tr("Row"));
    colLabel->setText(tr("Column"));
    vaseSkinLabel->setText(tr("Skin"));
    vaseTypeLabel->setText(tr("Type"));
    vaseContentPlantLabel->setText(tr("Plant"));
    vaseContentZombieLabel->setText(tr("Zombie"));
    vaseContentSunLabel->setText(tr("Sun"));

    if (rowCombo->count() == 0)
        for (size_t i = 0; i < 6; i++)
            rowCombo->addItem(QString::number(i + 1));
    else
        for (size_t i = 0; i < 6; i++)
            rowCombo->setItemText(i, QString::number(i + 1));

    if (colCombo->count() == 0)
        for (size_t i = 0; i < 9; i++)
            colCombo->addItem(QString::number(i + 1));
    else
        for (size_t i = 0; i < 9; i++)
            colCombo->setItemText(i, QString::number(i + 1));

    SET_COMBO_TEXT(vaseSkinCombo, List::Get().vaseSkinList, 4);
    SET_COMBO_TEXT(vaseTypeCombo, List::Get().vaseTypeList, 4);
    SET_COMBO_TEXT(vaseContentPlantCombo, List::Get().seedList, 53);
    SET_COMBO_TEXT(vaseContentZombieCombo, List::Get().zombieList, 33);

    table->setHorizontalHeaderLabels(List::Get().vaseHeadersList);

    if (table->rowCount() > 0)
        emit loadButton->clicked();
}

void VasePage::ShowVases(std::vector<Vase> vases)
{
    size_t count = vases.size();
    table->setRowCount(count);

    vaseCountLabel->setText(tr("Vase Count") + ": " + QString::number(count));

    if (count > 0)
        table->horizontalHeader()->setVisible(true);

    for (size_t r = 0; r < count; r++)
    {
        Vase vase = vases[r];
        QString index = QString::number(vase.index + 1);
        QString row = QString::number(vase.row + 1);
        QString col = QString::number(vase.col + 1);
        QString skin = List::Get().vaseSkinList[vase.skin - 3 + 1];
        QString type = List::Get().vaseTypeList[vase.type];
        QString content;
        if (vase.type == 1) // Plant
            content = List::Get().seedList[vase.content_plant];
        else if (vase.type == 2) // Zombie
            content = List::Get().zombieList[vase.content_zombie];
        else if (vase.type == 3) // Sun
            content = QString::number(vase.content_sun);
        else // if (vase.type == 0) // Empty
            content = tr("Empty");

        table->setItem(r, 0, new QTableWidgetItem(index));
        table->setItem(r, 1, new QTableWidgetItem(row));
        table->setItem(r, 2, new QTableWidgetItem(col));
        table->setItem(r, 3, new QTableWidgetItem(skin));
        table->setItem(r, 4, new QTableWidgetItem(type));
        table->setItem(r, 5, new QTableWidgetItem(content));
    }

    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

void VasePage::UpdateContent()
{
    QList<QTableWidgetItem *> items = table->selectedItems();
    int count = items.count();
    for (int i = 0; i < count; i++)
    {
        QTableWidgetItem *item = items.at(i);
        // int row = table->row(item);
        int col = table->column(item);
        if (col == 1)
            item->setText(rowCombo->currentText());
        else if (col == 2)
            item->setText(colCombo->currentText());
        else if (col == 3)
            item->setText(List::Get().vaseSkinList[vaseSkinCombo->currentIndex()]);
        else if (col == 4)
            item->setText(List::Get().vaseTypeList[vaseTypeCombo->currentIndex()]);
        else if (col == 5)
        {
            if (vaseTypeCombo->currentIndex() == 0)
                item->setText(tr("Empty"));
            else if (vaseTypeCombo->currentIndex() == 1)
                item->setText(List::Get().seedList[vaseContentPlantCombo->currentIndex()]);
            else if (vaseTypeCombo->currentIndex() == 2)
                item->setText(List::Get().zombieList[vaseContentZombieCombo->currentIndex()]);
            else if (vaseTypeCombo->currentIndex() == 3)
                item->setText(vaseContentSunLineEdit->text());
        }
    }
}

void VasePage::SetComboEnabled()
{
    int index = vaseTypeCombo->currentIndex();
    if (index == 0)
    {
        vaseContentPlantCombo->setEnabled(false);
        vaseContentZombieCombo->setEnabled(false);
        vaseContentSunLineEdit->setEnabled(false);
    }
    else if (index == 1)
    {
        vaseContentPlantCombo->setEnabled(true);
        vaseContentZombieCombo->setEnabled(false);
        vaseContentSunLineEdit->setEnabled(false);
    }
    else if (index == 2)
    {
        vaseContentPlantCombo->setEnabled(false);
        vaseContentZombieCombo->setEnabled(true);
        vaseContentSunLineEdit->setEnabled(false);
    }
    else if (index == 3)
    {
        vaseContentPlantCombo->setEnabled(false);
        vaseContentZombieCombo->setEnabled(false);
        vaseContentSunLineEdit->setEnabled(true);
    }
}

// Effects

EffectPage::EffectPage(QWidget *parent)
    : QWidget(parent)
{
    sukhbirCodeButton = new QPushButton(this);
    futureCodeButton = new QPushButton(this);
    mustacheCodeButton = new QPushButton(this);
    trickedoutCodeButton = new QPushButton(this);
    daisiesCodeButton = new QPushButton(this);
    danceCodeButton = new QPushButton(this);
    pinataCodeButton = new QPushButton(this);
    konamiCodeButton = new QPushButton(this);

    itsRainingCheckBox = new QCheckBox(this);
    littleZombieCheckBox = new QCheckBox(this);
    columnLikeCheckBox = new QCheckBox(this);
    zombieQuickCheckBox = new QCheckBox(this);
    whackZombieCheckBox = new QCheckBox(this);
    highGravityCheckBox = new QCheckBox(this);
    graveDangerCheckBox = new QCheckBox(this);
    stormyNightCheckBox = new QCheckBox(this);
    bungeeBlitzCheckBox = new QCheckBox(this);

    iceTrailLabel = new QLabel(this);
    iceTrailCombo = new QComboBox(this);
    iceTrailLineEdit = new QLineEdit(this);
    iceTrailLineEdit->setValidator(new QIntValidator(-9999, 9999, this));
    iceTrailLineEdit->setAlignment(Qt::AlignRight);
    iceTrailButton = new QPushButton(this);

    fullFogCheckBox = new QCheckBox(this);
    noFogCheckBox = new QCheckBox(this);
    seeVaseCheckBox = new QCheckBox(this);

    sukhbirCodeButton->setText("SUKHBIR");
    futureCodeButton->setText("FUTURE");
    mustacheCodeButton->setText("MUSTACHE");
    trickedoutCodeButton->setText("TRICKEDOUT");
    daisiesCodeButton->setText("DAISIES");
    danceCodeButton->setText("DANCE");
    pinataCodeButton->setText("PINATA");
    konamiCodeButton->setText("KONAMI");

    iceTrailLineEdit->setText("800");

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(sukhbirCodeButton, 0, 0, 1, 3);
    mainLayout->addWidget(futureCodeButton, 0, 3, 1, 3);
    mainLayout->addWidget(mustacheCodeButton, 0, 6, 1, 3);
    mainLayout->addWidget(trickedoutCodeButton, 0, 9, 1, 3);
    mainLayout->addWidget(daisiesCodeButton, 1, 0, 1, 3);
    mainLayout->addWidget(danceCodeButton, 1, 3, 1, 3);
    mainLayout->addWidget(pinataCodeButton, 1, 6, 1, 3);
    mainLayout->addWidget(konamiCodeButton, 1, 9, 1, 3);
    mainLayout->addWidget(itsRainingCheckBox, 2, 0, 1, 4);
    mainLayout->addWidget(littleZombieCheckBox, 2, 4, 1, 4);
    mainLayout->addWidget(columnLikeCheckBox, 2, 8, 1, 4);
    mainLayout->addWidget(zombieQuickCheckBox, 3, 0, 1, 4);
    mainLayout->addWidget(whackZombieCheckBox, 3, 4, 1, 4);
    mainLayout->addWidget(highGravityCheckBox, 3, 8, 1, 4);
    mainLayout->addWidget(graveDangerCheckBox, 4, 0, 1, 4);
    mainLayout->addWidget(stormyNightCheckBox, 4, 4, 1, 4);
    mainLayout->addWidget(bungeeBlitzCheckBox, 4, 8, 1, 4);
    mainLayout->addWidget(iceTrailLabel, 5, 0, 1, 3);
    mainLayout->addWidget(iceTrailCombo, 5, 3, 1, 3);
    mainLayout->addWidget(iceTrailLineEdit, 5, 6, 1, 3);
    mainLayout->addWidget(iceTrailButton, 5, 9, 1, 3);
    mainLayout->addWidget(fullFogCheckBox, 6, 0, 1, 4);
    mainLayout->addWidget(noFogCheckBox, 6, 4, 1, 4);
    mainLayout->addWidget(seeVaseCheckBox, 6, 8, 1, 4);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    connect(sukhbirCodeButton, &QPushButton::clicked,
            this, [=]() {
                emit WisdomTreeCode(0);
            });

    connect(futureCodeButton, &QPushButton::clicked,
            this, [=]() {
                emit WisdomTreeCode(5);
            });

    connect(mustacheCodeButton, &QPushButton::clicked,
            this, [=]() {
                emit WisdomTreeCode(20);
            });

    connect(trickedoutCodeButton, &QPushButton::clicked,
            this, [=]() {
                emit WisdomTreeCode(48);
            });

    connect(daisiesCodeButton, &QPushButton::clicked,
            this, [=]() {
                emit WisdomTreeCode(99);
            });

    connect(danceCodeButton, &QPushButton::clicked,
            this, [=]() {
                emit WisdomTreeCode(500);
            });

    connect(pinataCodeButton, &QPushButton::clicked,
            this, [=]() {
                emit WisdomTreeCode(999);
            });

    connect(konamiCodeButton, &QPushButton::clicked,
            this, [=]() {
                emit KonamiCode();
            });

    connect(itsRainingCheckBox, &QCheckBox::clicked,
            this, &EffectPage::ItsRaining);

    connect(littleZombieCheckBox, &QCheckBox::clicked,
            this, &EffectPage::LittleZombie);

    connect(columnLikeCheckBox, &QCheckBox::clicked,
            this, &EffectPage::ColumnLike);

    connect(zombieQuickCheckBox, &QCheckBox::clicked,
            this, &EffectPage::ZombieQuick);

    connect(whackZombieCheckBox, &QCheckBox::clicked,
            this, &EffectPage::WhackZombie);

    connect(highGravityCheckBox, &QCheckBox::clicked,
            this, &EffectPage::HighGravity);

    connect(graveDangerCheckBox, &QCheckBox::clicked,
            this, &EffectPage::GraveDanger);

    connect(stormyNightCheckBox, &QCheckBox::clicked,
            this, &EffectPage::StormyNight);

    connect(bungeeBlitzCheckBox, &QCheckBox::clicked,
            this, &EffectPage::BungeeBlitz);

    connect(iceTrailCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [=](int index) {
                if (index != 0)
                {
                    int row = index - 1;
                    emit GetIceTrailX(row);
                }
                else
                {
                    iceTrailLineEdit->setText(QString::number(800));
                }
            });

    connect(iceTrailButton, &QPushButton::clicked,
            this, [=]() {
                int row = iceTrailCombo->currentIndex() - 1;
                int x = iceTrailLineEdit->text().toInt();
                emit SetIceTrailX(row, x);
            });

    connect(fullFogCheckBox, &QCheckBox::clicked,
            this, &EffectPage::FullFog);

    connect(noFogCheckBox, &QCheckBox::clicked,
            this, &EffectPage::NoFog);

    connect(seeVaseCheckBox, &QCheckBox::clicked,
            this, &EffectPage::SeeVase);
}

void EffectPage::TranslateUI()
{
    itsRainingCheckBox->setText(List::Get().miniGamesList[19 - 16]);
    littleZombieCheckBox->setText(List::Get().miniGamesList[25 - 16]);
    columnLikeCheckBox->setText(List::Get().miniGamesList[27 - 16]);
    zombieQuickCheckBox->setText(List::Get().miniGamesList[29 - 16]);
    whackZombieCheckBox->setText(List::Get().miniGamesList[30 - 16]);
    highGravityCheckBox->setText(List::Get().miniGamesList[44 - 16]);
    graveDangerCheckBox->setText(List::Get().miniGamesList[45 - 16]);
    stormyNightCheckBox->setText(List::Get().miniGamesList[47 - 16]);
    bungeeBlitzCheckBox->setText(List::Get().miniGamesList[48 - 16]);

    iceTrailLabel->setText(tr("Ice Trail Coordinate"));
    SET_COMBO_TEXT(iceTrailCombo, List::Get().rowList, 7);
    iceTrailButton->setText(tr("Set"));

    fullFogCheckBox->setText(tr("Full Fog"));
    noFogCheckBox->setText(tr("No Fog"));
    seeVaseCheckBox->setText(tr("See Vase"));
}

void EffectPage::ShowIceTrailX(int x)
{
    iceTrailLineEdit->setText(QString::number(x));
}

// Others

OthersPage::OthersPage(QWidget *parent)
    : QWidget(parent)
{
    disableSaveDataCheckBox = new QCheckBox(this);
    disableDeleteDataCheckBox = new QCheckBox(this);

    runningInBackgroundCheckBox = new QCheckBox(this);
    disablePauseCheckBox = new QCheckBox(this);

    openDataDirButton = new QPushButton(this);

    debugModeLabel = new QLabel(this);
    debugModeCombo = new QComboBox(this);
    debugModeButton = new QPushButton(this);

    openPakFileButton = new QPushButton(this);
    pakFileLineEdit = new QLineEdit(this);
    unpackPakButton = new QPushButton(this);

    openPakFolderButton = new QPushButton(this);
    pakPathLineEdit = new QLineEdit(this);
    packPakButton = new QPushButton(this);

    targetMapButton = new QPushButton(this);
    cannonLauncherButton = new QPushButton(this);

    pakFileLineEdit->setText("");
    pakPathLineEdit->setText("");

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(disableSaveDataCheckBox, 0, 0, 1, 3);
    mainLayout->addWidget(disableDeleteDataCheckBox, 0, 3, 1, 3);
    mainLayout->addWidget(runningInBackgroundCheckBox, 0, 6, 1, 3);
    mainLayout->addWidget(disablePauseCheckBox, 0, 9, 1, 3);
    mainLayout->addWidget(openDataDirButton, 1, 0, 1, 3);
    mainLayout->addWidget(debugModeLabel, 1, 4, 1, 2);
    mainLayout->addWidget(debugModeCombo, 1, 6, 1, 3);
    mainLayout->addWidget(debugModeButton, 1, 9, 1, 3);
    mainLayout->addWidget(openPakFileButton, 2, 0, 1, 2);
    mainLayout->addWidget(pakFileLineEdit, 2, 2, 1, 8);
    mainLayout->addWidget(unpackPakButton, 2, 10, 1, 2);
    mainLayout->addWidget(openPakFolderButton, 3, 0, 1, 2);
    mainLayout->addWidget(pakPathLineEdit, 3, 2, 1, 8);
    mainLayout->addWidget(packPakButton, 3, 10, 1, 2);
    mainLayout->addWidget(targetMapButton, 4, 0, 1, 4);
    mainLayout->addWidget(cannonLauncherButton, 4, 4, 1, 4);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    connect(openPakFileButton, &QPushButton::clicked,
            this, &OthersPage::GetFileName);

    connect(openPakFolderButton, &QPushButton::clicked,
            this, &OthersPage::GetFolderName);

    connect(disableSaveDataCheckBox, &QCheckBox::clicked,
            this, &OthersPage::DisableSaveData);

    connect(disableDeleteDataCheckBox, &QCheckBox::clicked,
            this, &OthersPage::DisableDeleteData);

    connect(runningInBackgroundCheckBox, &QCheckBox::clicked,
            this, &OthersPage::RunningInBackground);

    connect(disablePauseCheckBox, &QCheckBox::clicked,
            this, &OthersPage::DisablePause);

    connect(openDataDirButton, &QPushButton::clicked,
            this, &OthersPage::OpenDataDir);

    connect(debugModeButton, &QPushButton::clicked,
            this, [=]() {
                int mode = debugModeCombo->currentIndex();
                emit DebugMode(mode);
            });

    connect(unpackPakButton, &QPushButton::clicked,
            this, [=]() {
                QString file_name = pakFileLineEdit->text();
                QString dir_name = pakPathLineEdit->text();
                if (file_name.size() > 0 && dir_name.size() > 0)
                {
                    unpackPakButton->setEnabled(false);
                    packPakButton->setEnabled(false);
                    emit UnpackPAK(file_name, dir_name);
                }
            });

    connect(packPakButton, &QPushButton::clicked,
            this, [=]() {
                QString dir_name = pakPathLineEdit->text();
                if (dir_name.size() > 0)
                {
                    unpackPakButton->setEnabled(false);
                    packPakButton->setEnabled(false);
                    emit PackPAK(dir_name);
                }
            });

    connect(targetMapButton, &QPushButton::clicked,
            this, &OthersPage::ShowTargetMapPage);

    connect(cannonLauncherButton, &QPushButton::clicked,
            this, &OthersPage::ShowCannonLauncherPage);
}

void OthersPage::TranslateUI()
{
    disableSaveDataCheckBox->setText(tr("Disable Save Data"));
    disableDeleteDataCheckBox->setText(tr("Disable Delete Data"));

    runningInBackgroundCheckBox->setText(tr("Running In Background"));
    disablePauseCheckBox->setText(tr("Disable Pause"));

    openDataDirButton->setText(tr("Open Data Dir"));

    debugModeLabel->setText(tr("Debug Mode"));
    SET_COMBO_TEXT(debugModeCombo, List::Get().debugModeList, 5);
    debugModeButton->setText(tr("Set"));

    openPakFileButton->setText(tr("Open File"));
    unpackPakButton->setText(tr("Unpack"));

    openPakFolderButton->setText(tr("Open Folder"));
    packPakButton->setText(tr("Pack"));

    targetMapButton->setText(tr("Target Map Modify"));
    cannonLauncherButton->setText(tr("Cannon Launcher"));
}

void OthersPage::GetFileName()
{
    QString file_name = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        ".",
        tr("PAK files (*.pak) ;; All files (*.*)"));
    if (!file_name.isNull())
    {
        pakFileLineEdit->setText(file_name);
        pakPathLineEdit->setText(QCoreApplication::applicationDirPath() + "/" + QString::number(QDateTime::currentDateTime().toTime_t(), 16));
    }
}

void OthersPage::GetFolderName()
{
    QString dir_name = QFileDialog::getExistingDirectory(
        this,
        tr("Open Folder"),
        ".");
    if (!dir_name.isNull())
        pakPathLineEdit->setText(dir_name);
}

void OthersPage::UnpackFinished()
{
    unpackPakButton->setEnabled(true);
    packPakButton->setEnabled(true);
}

void OthersPage::PackFinished()
{
    unpackPakButton->setEnabled(true);
    packPakButton->setEnabled(true);
}

// Status

StatusPage::StatusPage(QWidget *parent)
    : QWidget(parent)
{
    gameModeTextLabel = new QLabel(this);

    refreshStatusTimer = new QTimer(this);
    refreshStatusTimer->setInterval(100);
    autoRefreshCheckBox = new QCheckBox(this);
    refreshIntervalSpinBox = new QSpinBox(this);
    refreshIntervalSpinBox->setRange(0, 999999);
    refreshIntervalSpinBox->setSingleStep(1);
    refreshIntervalSpinBox->setValue(10);

    gameModeLabel = new QLabel(this);
    gameModeValueLabel = new QLabel(this);
    gameUiLabel = new QLabel(this);
    gameUiValueLabel = new QLabel(this);

    runningTimeLabel = new QLabel(this);
    runningTimeValueLabel = new QLabel(this);
    levelCompletedLabel = new QLabel(this);
    levelCompletedValueLabel = new QLabel(this);

    totalWavesLabel = new QLabel(this);
    totalWavesValueLabel = new QLabel(this);
    pastWavesLabel = new QLabel(this);
    pastWavesValueLabel = new QLabel(this);

    totalHpLabel = new QLabel(this);
    totalHpValueLabel = new QLabel(this);
    triggerHpLabel = new QLabel(this);
    triggerHpValueLabel = new QLabel(this);

    countdownLabel = new QLabel(this);
    countdownValueLabel = new QLabel(this);
    hugewaveCountdownLabel = new QLabel(this);
    hugewaveCountdownValueLabel = new QLabel(this);

    plantCountLabel = new QLabel(this);
    plantCountValueLabel = new QLabel(this);
    zombieCountLabel = new QLabel(this);
    zombieCountValueLabel = new QLabel(this);

    gameModeTextLabel->setText("");

    gameModeValueLabel->setText("0");
    gameUiValueLabel->setText("0");
    runningTimeValueLabel->setText("0");
    levelCompletedValueLabel->setText("0");
    totalWavesValueLabel->setText("0");
    pastWavesValueLabel->setText("0");
    totalHpValueLabel->setText("0");
    triggerHpValueLabel->setText("0");
    countdownValueLabel->setText("0");
    hugewaveCountdownValueLabel->setText("0");
    plantCountValueLabel->setText("0");
    zombieCountValueLabel->setText("0");

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(gameModeTextLabel, 0, 0, 1, 3);
    mainLayout->addWidget(autoRefreshCheckBox, 0, 3, 1, 2);
    mainLayout->addWidget(refreshIntervalSpinBox, 0, 5, 1, 1);
    mainLayout->addWidget(gameModeLabel, 1, 0, 1, 2);
    mainLayout->addWidget(gameModeValueLabel, 1, 2, 1, 1);
    mainLayout->addWidget(gameUiLabel, 1, 3, 1, 2);
    mainLayout->addWidget(gameUiValueLabel, 1, 5, 1, 1);
    mainLayout->addWidget(runningTimeLabel, 2, 0, 1, 2);
    mainLayout->addWidget(runningTimeValueLabel, 2, 2, 1, 1);
    mainLayout->addWidget(levelCompletedLabel, 2, 3, 1, 2);
    mainLayout->addWidget(levelCompletedValueLabel, 2, 5, 1, 1);
    mainLayout->addWidget(totalWavesLabel, 3, 0, 1, 2);
    mainLayout->addWidget(totalWavesValueLabel, 3, 2, 1, 1);
    mainLayout->addWidget(pastWavesLabel, 3, 3, 1, 2);
    mainLayout->addWidget(pastWavesValueLabel, 3, 5, 1, 1);
    mainLayout->addWidget(totalHpLabel, 4, 0, 1, 2);
    mainLayout->addWidget(totalHpValueLabel, 4, 2, 1, 1);
    mainLayout->addWidget(triggerHpLabel, 4, 3, 1, 2);
    mainLayout->addWidget(triggerHpValueLabel, 4, 5, 1, 1);
    mainLayout->addWidget(countdownLabel, 5, 0, 1, 2);
    mainLayout->addWidget(countdownValueLabel, 5, 2, 1, 1);
    mainLayout->addWidget(hugewaveCountdownLabel, 5, 3, 1, 2);
    mainLayout->addWidget(hugewaveCountdownValueLabel, 5, 5, 1, 1);
    mainLayout->addWidget(plantCountLabel, 6, 0, 1, 2);
    mainLayout->addWidget(plantCountValueLabel, 6, 2, 1, 1);
    mainLayout->addWidget(zombieCountLabel, 6, 3, 1, 2);
    mainLayout->addWidget(zombieCountValueLabel, 6, 5, 1, 1);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    connect(autoRefreshCheckBox, &QCheckBox::stateChanged,
            this, [=](int state) {
                if (state == Qt::Checked)
                    refreshStatusTimer->start();
                else
                    refreshStatusTimer->stop();
            });

    connect(refreshIntervalSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, [=](int value) {
                refreshStatusTimer->setInterval(value * 10);
            });

    connect(refreshStatusTimer, &QTimer::timeout,
            this, &StatusPage::GetStatus);
}

void StatusPage::TranslateUI()
{
    autoRefreshCheckBox->setText(tr("Auto Refresh"));

    gameModeLabel->setText(tr("Game Mode"));
    gameUiLabel->setText(tr("Game UI"));
    runningTimeLabel->setText(tr("Running Time"));
    levelCompletedLabel->setText(tr("Level Completed"));
    totalWavesLabel->setText(tr("Total Waves"));
    pastWavesLabel->setText(tr("Past Waves"));
    totalHpLabel->setText(tr("Total HP"));
    triggerHpLabel->setText(tr("Trigger HP"));
    countdownLabel->setText(tr("Count Down"));
    hugewaveCountdownLabel->setText(tr("Huge Wave Count Down"));
    plantCountLabel->setText(tr("Plant Count"));
    zombieCountLabel->setText(tr("Zombie Count"));
}

void StatusPage::ShowStatus(std::array<int, 12> status)
{
    // Adventure  // 0
    // Mini-Games // 16-50
    // Puzzle     // 51-72
    // Survival   // 1-15
    int mode = status[0];
    if (mode == 0)
        gameModeTextLabel->setText(tr("Adventure"));
    else if (mode >= 1 && mode < 16)
        gameModeTextLabel->setText(List::Get().survivalList[mode - 1]);
    else if (mode >= 16 && mode < 51)
        gameModeTextLabel->setText(List::Get().miniGamesList[mode - 16]);
    else // if (mode >= 51 && mode < 73)
        gameModeTextLabel->setText(List::Get().puzzleList[mode - 51]);

    QString countdown = QString::number(status[8]);
    QString hugewave_countdown = QString::number(status[9]);
    if (status[8] > 0 && status[8] < 200)
        countdown = QString("<p style=\"color:red\">") + countdown + QString("</p>");
    if (status[9] > 0 && status[9] < 200)
        hugewave_countdown = QString("<p style=\"color:red\">") + hugewave_countdown + QString("</p>");

    gameModeValueLabel->setText(QString::number(status[0]));
    gameUiValueLabel->setText(QString::number(status[1]));
    runningTimeValueLabel->setText(QString::number(status[2]));
    levelCompletedValueLabel->setText(QString::number(status[3]));
    totalWavesValueLabel->setText(QString::number(status[4]));
    pastWavesValueLabel->setText(QString::number(status[5]));
    totalHpValueLabel->setText(QString::number(status[6]));
    triggerHpValueLabel->setText(QString::number(status[7]));
    countdownValueLabel->setText(countdown);
    hugewaveCountdownValueLabel->setText(hugewave_countdown);
    plantCountValueLabel->setText(QString::number(status[10]));
    zombieCountValueLabel->setText(QString::number(status[11]));
}

bool StatusPage::IsAutoRefresh()
{
    return autoRefreshCheckBox->isChecked();
}

void StatusPage::StartTimer()
{
    refreshStatusTimer->start();
}

void StatusPage::StopTimer()
{
    refreshStatusTimer->stop();
}

// Target Map

TargetMapPage::TargetMapPage(QWidget *parent)
    : QWidget(parent)
{
    qRegisterMetaType<std::array<int, 54>>("std::array<int, 54>");

    for (size_t r = 0; r < 6; r++)
        for (size_t c = 0; c < 9; c++)
            comboBoxes[9 * r + c] = new QComboBox(this);

    seeingStarsRadioButton = new QRadioButton(this);
    artChallengeWallnutRadioButton = new QRadioButton(this);
    artChallengeSunflowerRadioButton = new QRadioButton(this);

    loadButton = new QPushButton(this);
    clearButton = new QPushButton(this);
    setButton = new QPushButton(this);

    seeingStarsRadioButton->setChecked(true);

    mainLayout = new QGridLayout(this);
    for (size_t r = 0; r < 6; r++)
        for (size_t c = 0; c < 9; c++)
            mainLayout->addWidget(comboBoxes[9 * r + c], r, c, 1, 1);
    mainLayout->addWidget(seeingStarsRadioButton, 7, 0, 1, 2);
    mainLayout->addWidget(artChallengeWallnutRadioButton, 7, 2, 1, 2);
    mainLayout->addWidget(artChallengeSunflowerRadioButton, 7, 4, 1, 2);
    mainLayout->addWidget(loadButton, 7, 6, 1, 1);
    mainLayout->addWidget(clearButton, 7, 7, 1, 1);
    mainLayout->addWidget(setButton, 7, 8, 1, 1);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    connect(loadButton, &QPushButton::clicked,
            this, [=]() {
                if (seeingStarsRadioButton->isChecked())
                    emit GetTargetMap(22);
                if (artChallengeWallnutRadioButton->isChecked())
                    emit GetTargetMap(36);
                if (artChallengeSunflowerRadioButton->isChecked())
                    emit GetTargetMap(40);
            });

    connect(clearButton, &QPushButton::clicked,
            this, [=]() {
                for (size_t r = 0; r < 6; r++)
                    for (size_t c = 0; c < 9; c++)
                        comboBoxes[9 * r + c]->setCurrentIndex(0);
            });

    connect(setButton, &QPushButton::clicked,
            this, [=]() {
                std::array<int, 54> map;
                for (size_t r = 0; r < 6; r++)
                    for (size_t c = 0; c < 9; c++)
                        map[9 * r + c] = comboBoxes[9 * r + c]->currentIndex() - 1;
                if (seeingStarsRadioButton->isChecked())
                    emit SetTargetMap(22, map);
                if (artChallengeWallnutRadioButton->isChecked())
                    emit SetTargetMap(36, map);
                if (artChallengeSunflowerRadioButton->isChecked())
                    emit SetTargetMap(40, map);
            });
}

void TargetMapPage::TranslateUI()
{
    for (size_t r = 0; r < 6; r++)
    {
        for (size_t c = 0; c < 9; c++)
        {
            if (comboBoxes[9 * r + c]->count() == 0)
            {
                comboBoxes[9 * r + c]->addItem(tr("Empty"));
                for (size_t i = 0; i < 48; i++)
                    comboBoxes[9 * r + c]->addItem(List::Get().seedList[i]);
            }
            else
            {
                comboBoxes[9 * r + c]->setItemText(0, tr("Empty"));
                for (size_t i = 0; i < 48; i++)
                    comboBoxes[9 * r + c]->setItemText(i + 1, List::Get().seedList[i]);
            }
        }
    }

    seeingStarsRadioButton->setText(List::Get().miniGamesList[22 - 16]);           // Seeing Stars
    artChallengeWallnutRadioButton->setText(List::Get().miniGamesList[36 - 16]);   // Art Challenge Wall-nut
    artChallengeSunflowerRadioButton->setText(List::Get().miniGamesList[40 - 16]); // Art Challenge Sunflower

    loadButton->setText(tr("Load"));
    clearButton->setText(tr("Clear"));
    setButton->setText(tr("Set"));

    setWindowTitle(List::Get().miniGamesList[22 - 16]           //
                   + " / " + List::Get().miniGamesList[36 - 16] //
                   + " / " + List::Get().miniGamesList[40 - 16] //
                   + " " + tr("Target Map Modify"));
}

void TargetMapPage::ShowTargetMap(std::array<int, 54> map)
{
    for (size_t r = 0; r < 6; r++)
        for (size_t c = 0; c < 9; c++)
            comboBoxes[9 * r + c]->setCurrentIndex(map[9 * r + c] + 1);
}

// Cannon Launcher

CannonLauncherPage::CannonLauncherPage(QWidget *parent)
    : QWidget(parent)
{
    indexLabel = new QLabel(this);
    indexSpinBox = new QSpinBox(this);
    indexSpinBox->setRange(0, 1023);
    indexSpinBox->setSingleStep(1);
    indexSpinBox->setValue(0);
    infoLabel = new QLabel(this);

    gridLabel = new QLabel(this);
    rowLabel = new QLabel(this);
    colLabel = new QLabel(this);

    rowSpinBox = new QDoubleSpinBox(this);
    rowSpinBox->setRange(0.0, 7.0);
    rowSpinBox->setSingleStep(1);
    rowSpinBox->setValue(2.00);
    colSpinBox = new QDoubleSpinBox(this);
    colSpinBox->setRange(0.0, 10.0);
    colSpinBox->setSingleStep(1);
    colSpinBox->setValue(9.00);

    coordLabel = new QLabel(this);
    xLabel = new QLabel(this);
    yLabel = new QLabel(this);
    xValueLabel = new QLabel(this);
    yValueLabel = new QLabel(this);

    randomFallCheckBox = new QCheckBox(this);
    launchAllButton = new QPushButton(this);
    launchButton = new QPushButton(this);

    launchButton->setEnabled(false);

    xLabel->setText("X");
    yLabel->setText("Y");
    xValueLabel->setText("");
    yValueLabel->setText("");

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(indexLabel, 0, 0, 1, 1);
    mainLayout->addWidget(indexSpinBox, 0, 1, 1, 1);
    mainLayout->addWidget(infoLabel, 0, 2, 1, 3);
    mainLayout->addWidget(gridLabel, 2, 0, 1, 1);
    mainLayout->addWidget(rowLabel, 2, 1, 1, 1);
    mainLayout->addWidget(rowSpinBox, 2, 2, 1, 1);
    mainLayout->addWidget(colLabel, 2, 3, 1, 1);
    mainLayout->addWidget(colSpinBox, 2, 4, 1, 1);
    mainLayout->addWidget(coordLabel, 3, 0, 1, 1);
    mainLayout->addWidget(xLabel, 3, 1, 1, 1);
    mainLayout->addWidget(xValueLabel, 3, 2, 1, 1);
    mainLayout->addWidget(yLabel, 3, 3, 1, 1);
    mainLayout->addWidget(yValueLabel, 3, 4, 1, 1);
    mainLayout->addWidget(randomFallCheckBox, 4, 0, 1, 1);
    mainLayout->addWidget(launchAllButton, 4, 1, 1, 2);
    mainLayout->addWidget(launchButton, 4, 3, 1, 2);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);

    connect(indexSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CannonLauncherPage::GetCobInfo);

    connect(indexSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &CannonLauncherPage::GetScene);

    connect(rowSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &CannonLauncherPage::GetScene);

    connect(colSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &CannonLauncherPage::GetScene);

    connect(launchButton, &QPushButton::clicked,
            this, [=]() {
                int index = indexSpinBox->value();
                int x = xValueLabel->text().toInt();
                int y = yValueLabel->text().toInt();
                emit LaunchCannon(index, x, y);
            });

    connect(launchAllButton, &QPushButton::clicked,
            this, [=]() {
                bool random_fall = randomFallCheckBox->isChecked();
                int x = xValueLabel->text().toInt();
                int y = yValueLabel->text().toInt();
                emit LaunchAllCannon(random_fall, x, y);
            });
}

void CannonLauncherPage::TranslateUI()
{
    indexLabel->setText(tr("Index"));
    infoLabel->setText(tr("Not Found"));

    gridLabel->setText(tr("Grid"));
    rowLabel->setText(tr("Row"));
    colLabel->setText(tr("Column"));

    coordLabel->setText(tr("Coordinate"));
    randomFallCheckBox->setText(tr("Random Fall"));
    launchAllButton->setText(tr("Launch All"));
    launchButton->setText(tr("Launch"));

    setWindowTitle(tr("Cannon Launcher"));
}

void CannonLauncherPage::ShowCobInfo(bool is_cob_cannon, int row, int col)
{
    if (is_cob_cannon)
    {
        QString text = QString(tr("Cob Cannon")) + " " + tr("in")             //
                       + " " + QString::number(row + 1) + " " + tr("Row")     //
                       + " " + QString::number(col + 1) + " " + tr("Column"); //
        infoLabel->setText(text);
        launchButton->setEnabled(true);
    }
    else
    {
        infoLabel->setText(tr("Not Found"));
        launchButton->setEnabled(false);
    }
}

void CannonLauncherPage::CalculateCoord(int scene)
{
    if (scene == -1)
    {
        xValueLabel->setText("0");
        yValueLabel->setText("0");
        return;
    }

    double row = rowSpinBox->value();
    double col = colSpinBox->value();
    int x, y;

    if (scene == 2 || scene == 3)
        x = 80 * col, y = 60 + 85 * row;
    else if (scene == 4 || scene == 5)
        if (col > 5.0)
            x = 80 * col, y = 50 + 85 * row;
        else
            x = 80 * col, y = 60 + 85 * row + (110 - 20 * col);
    else
        x = 80 * col, y = 45 + 100 * row;

    xValueLabel->setText(QString::number(x));
    yValueLabel->setText(QString::number(y));
}

DocumentPage::DocumentPage(QWidget *parent)
    : QWidget(parent)
{
    document = new QTextBrowser(this);
    document->setOpenExternalLinks(true);

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(document, 0, 0, 1, 1);

    for (int i = 0; i < mainLayout->rowCount(); i++)
        mainLayout->setRowStretch(i, 1);
    for (int i = 0; i < mainLayout->columnCount(); i++)
        mainLayout->setColumnStretch(i, 1);
}

void DocumentPage::TranslateUI()
{
    document->setText(tr("<style>a {text-decoration: none; color: blue}</style>"
                         "<h2>About</h2>"
                         "<p>This software is applicable to Plants vs. Zombies 1.0.0.1051 original English version (<a href=\"https://pvz.lmintlcx.com/getpvz\">download here</a>), the 2nd Chinese edition is theoretically supported but the probability of game crash is very high.</p>"
                         "<p>Excessive modification or forced use of non-corresponding game versions can easily cause the game to crash, please back up user data before use. All effects caused by using this software are responsibility of user himself.</p>"
                         "<p>Some behavior of this software (finding game progress, modifying memory data, remote injection code, etc.) may be considered dangerous by anti-virus software. Please decide whether to trust this software yourself.</p>"
                         "<p>Reasonable uses of this software include, but are not limited to, demonstrations, testing, research, and entertainment. Abuse of modifiers may reduce the fun of game seriously, please use it with caution, especially newbies.</p>"
                         "<p>There may be some bugs with this software, users can summit feedback or feature requirements to the author. The beta version of this software has a time limit, please download latest version if it prompt expires.</p>"
                         "<p>Configuration information of this software is saved in \"HKEY_CURRENT_USER\\SOFTWARE\\Cube Studio\\PvZ Tools\\v2\". You can clean this registry item after delete this software.</p>"
                         "<p>Almost all core data of this software comes from existing information and other modifiers (Including <a href=\"https://tieba.baidu.com/p/870532241\">here</a> <a href=\"https://tieba.baidu.com/p/1014349785\">here</a> <a href=\"https://tieba.baidu.com/p/2843347257\">here</a> and <a href=\"https://tieba.baidu.com/p/5575921846\">here</a>).</p>"
                         "<p>All source code of this software is located at <a href=\"https://github.com/lmintlcx/pvztools\">Github</a>.</p>"
                         "<h2>Spawn</h2>"
                         "<p>This feature is suitable for later period of survival endless. Size of zombie's spawning list is 1000, which is 20 waves per level and 50 zombies per wave.</p>"
                         "<p>The spawn page has two modes: \"brief\" and \"detailed\".</p>"
                         "<p>When switching between different pages, if the number of zombie types selected on target page is 0, selected zombie types on current page will be synchronized to target page (not one-to-one correspondence).</p>"
                         "<p>In brief mode, you can switch layout of the option boxes, in order of zombies in almanac or in same position as the PVZombiesSeed.exe.</p>"
                         "<p>In brief mode, Conehead Zombie and Newspaper Zombie are mutually exclusive. If \"Limit Spawn Count\" is checked, it will limit the number of selected zombie types (up to 10 types except Bungee Zombie and Zombie Yeti).</p>"
                         "<p>The difference between different spawn mode:<br>Natural spawn changes the zombie types only and calls built-in function of game to generates zombies list.<br>Extreme spawn is to evenly populate the zombies list with seleted zombie types.<br>Simulate natural spawn is randomly fill the zombie list with seleted zombie types according to the ratio which is get by statistical law, meanwhile increase the probability of GigaGargantuar in flag wave.</p>"
                         "<p>Special deal with some zombies:<br>When limit Flag Zombie, Flag Zombie will only appear in each flag wave (huge wave).<br>When limit Zombie Yeti, there will be only one Zombie Yeti.<br>When limit Bungee Zombie, Bungee Zombie will only appear in flag wave (huge wave).<br>When limit GigaGargantuar, GigaGargantuar will only appear in selected wave(s) (20 waves total).</p>"
                         "<p>When using natural spawn in brief mode, there must be Zombie.</p>"
                         "<p>When using extreme spawn in brief mode, there must be Zombie and Flag Zombie. Flag Zombie, Zombie Yeti and Bungee Zombie is limited, and GigaGargantuar has no limit.</p>"
                         "<p>When using simulate natural spawn in detailed mode, there must be Zombie and Flag Zombie. Flag Zombie, Zombie Yeti and Bungee Zombie is limited.</p>"
                         "<h2>Lineup</h2>"
                         "<p>Checking \"Quick Lineup Mode\" will enable these features: Auto Collect, Cob Cannon No CD, Plant Invincible, Stop Spawning, Purple Seed Unlimited, Ignore Sun, Slots No CD, No Fog.</p>"
                         "<p>Clicking \"Quick Pass\" will end current level directly, kill all zombies on the field, set the number of sunshine to 8000, and set the number of levels to 1008 (2018 flags completed). You can modify sunshine and level in corresponding page by pause the game immediately after clicking it.</p>"
                         "<p>Format of lineup string is compatible with <a href=\"http://lonelystar.org/ArrayDesign\">Array Design</a>.</p>"
                         "<p>The built-in lineup string includes <a href=\"https://tieba.baidu.com/p/5272254427\">Kaleidoscope of Endless Build Culture (2017 Poker)</a> and some well-known build in <a href=\"https://tieba.baidu.com/f?kw=植物大战僵尸&ie=utf-8&tab=good\">good tab</a> at Baidu Tieba.</p>"
                         "<p>Modifying game scene may cause problems such as texture errors or game crashes. Therefore the \"Allow Switch Scene\" option is not checked by default. It is recommended to reopen a new archive from \"Survival(Hard)\" level with desired scene, and then mix to \"Survival Endless\" mode after finishing design.</p>"
                         "<p>When \"Keep HP Status\" is checked, appearance status of some plants (Wall-nut, Tall-nut, Pumpkin, Garlic, Spikerock) will be preserved when importing or exporting lineup strings.</p>"
                         "<h2>Others</h2>"
                         "<p>The \"Open Data Dir\" feature is temporarily unavailable on XP systems (the data file is located in the \"userdata\" folder in game directory).</p>"));

    setWindowTitle(tr("Document"));
}

} // namespace Pt