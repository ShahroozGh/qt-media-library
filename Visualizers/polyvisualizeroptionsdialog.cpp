#include "polyvisualizeroptionsdialog.h"
#include "ui_polyvisualizeroptionsdialog.h"

PolyVisualizerOptionsDialog::PolyVisualizerOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PolyVisualizerOptionsDialog)
{
    ui->setupUi(this);

    //Connect signals
    QObject::connect(ui->totalPointsSpinBox, SIGNAL(editingFinished()), this, SLOT(optionModified()));
    QObject::connect(ui->minRadSpinBox, SIGNAL(editingFinished()), this, SLOT(optionModified()));
    QObject::connect(ui->gainSpinBox, SIGNAL(editingFinished()), this, SLOT(optionModified()));
    QObject::connect(ui->logCheckBox, SIGNAL(clicked()), this, SLOT(optionModified()));
    QObject::connect(ui->mirrorSpinBox, SIGNAL(editingFinished()), this, SLOT(optionModified()));
    QObject::connect(ui->altMirrorCheckBox, SIGNAL(clicked()), this, SLOT(optionModified()));
    QObject::connect(ui->fillVisCheckBox, SIGNAL(clicked()), this, SLOT(optionModified()));
    QObject::connect(ui->rotateCheckBox, SIGNAL(clicked()), this, SLOT(optionModified()));
    QObject::connect(ui->showInvCheckBox, SIGNAL(clicked()), this, SLOT(optionModified()));
    QObject::connect(ui->glowEnableCheckBox, SIGNAL(clicked()), this, SLOT(optionModified()));

}

PolyVisualizerOptionsDialog::~PolyVisualizerOptionsDialog()
{
    delete ui;
}

void PolyVisualizerOptionsDialog::initOptions()
{
    ui->totalPointsSpinBox->setValue(totalPoints);
    ui->minRadSpinBox->setValue(minRadius);

    ui->gainSpinBox->setValue(gain);

    ui->logCheckBox->setChecked(useLogScale);
    ui->mirrorSpinBox->setValue(mirror);
    ui->altMirrorCheckBox->setChecked(alternateMirror);
    ui->rotateCheckBox->setChecked(rotate);
    ui->showInvCheckBox->setChecked(showInvSpectrum);
    ui->glowEnableCheckBox->setChecked(glow);


    ui->fillVisCheckBox->setChecked(fillVisualizer);
}

void PolyVisualizerOptionsDialog::on_buttonBox_accepted()
{
    totalPoints = ui->totalPointsSpinBox->value();
    minRadius = ui->minRadSpinBox->value();

    gain = ui->gainSpinBox->value();


    useLogScale = ui->logCheckBox->isChecked();
    mirror = ui->mirrorSpinBox->value(); //Works best when factor of totalPoints
    alternateMirror = ui->altMirrorCheckBox->isChecked();
    rotate = ui->rotateCheckBox->isChecked();
    showInvSpectrum = ui->showInvCheckBox->isChecked();
    glow = ui->glowEnableCheckBox->isChecked();

    fillVisualizer = ui->fillVisCheckBox->isChecked();
}

void PolyVisualizerOptionsDialog::optionModified()
{
    on_buttonBox_accepted();
    emit changesMade();
}

