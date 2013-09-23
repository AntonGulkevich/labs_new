#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::Dialog), scene_(new QGraphicsScene(parent)), code_(new Code(QString::null))
{
    LOG << "Constructed!";
    ui_->setupUi(this);

    pen_.red = QPen(Qt::red);
    pen_.red.setWidth(2);
    pen_.black = QPen(Qt::black);
    pen_.black.setWidth(2);
    pen_.gray = QPen(Qt::gray);
    pen_.grid_b = QPen(Qt::black);
    pen_.grid_b.setStyle(Qt::DashLine);
    pen_.grid_g = QPen(Qt::gray);
    pen_.grid_g.setStyle(Qt::DotLine);

    scene_ = new QGraphicsScene(ui_->graphicsView);

    ui_->graphicsView->setScene(scene_);
    ui_->lineEdit->setMaxLength(16);

    connect(ui_->lineEdit, SIGNAL(textChanged(QString)), code_, SLOT(textChanged(QString)));
    connect(ui_->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(drawDigits(QString)));
    connect(code_, SIGNAL(sequenceUpdated()), this, SLOT(updateScene()));

    QObjectList buttons = ui_->groupBox->children();
    for (QObjectList::Iterator button = buttons.begin(); button != buttons.end(); ++button) {
        QRadioButton *b = qobject_cast<QRadioButton *>(*button);
        connect(b, SIGNAL(clicked()), this, SLOT(updateScene()));
    }
}

Dialog::~Dialog()
{
    LOG << "Destructed!";
    delete ui_;
}

int Dialog::getY(Code::level level) const {
    switch(level) {
        case Code::H:
           return -periodOffset_ * 2;
        case Code::HM:
            return -periodOffset_;
        case Code::M:
            return 0;
        case Code::LM:
            return periodOffset_;
        case Code::L:
            return periodOffset_ * 2;
    }
}

void Dialog::grid() const {
    LOG << "grid()!";
    scene_->clear();
    for (int r = ymin_; r < ymax_; r+=periodOffset_ / 2)
        scene_->addLine(xmin_, r, xmax_, r, (r%periodOffset_)?pen_.grid_g:pen_.grid_b);

    for (int c = xmin_; c < xmax_; c+=periodOffset_ / 2 )
        scene_->addLine(c, ymin_, c, ymax_, (c%periodOffset_)?pen_.grid_g:pen_.grid_b);
    scene_->addLine(xmin_, 0, xmax_, 0, pen_.black);
}
// private slots
void Dialog::updateScene() {
    LOG << "updateScene()!";
    grid();
    drawGraph();
    drawDigits(ui_->lineEdit->text());

}
void Dialog::drawGraph() const {
    LOG << "drawGraph()!";
    QList<Code::level> lvs;

   code_->setSeq(ui_->lineEdit->text());

        Code::type type;
        if (ui_->rb_nrz->isChecked())
            type = Code::NRZ;
        else if (ui_->rb_nrzi->isChecked())
            type = Code::NRZI;
        else if (ui_->rb_manchecter->isChecked())
            type = Code::Manchester;
        else if (ui_->rb_ami->isChecked())
            type = Code::AMI;
        else if (ui_->rb_hdb3->isChecked())
            type = Code::HDB3;
        else if (ui_->rb_2b1q->isChecked())
            type = Code::_2B1Q;

        lvs = code_->getLevels(type);



    int x = 0;
    if (!lvs.isEmpty()) {
        LOG << "not empty";
    Code::level old = lvs.front();
    for (QList<Code::level>::Iterator it = lvs.begin(); it != lvs.end(); ++it) {
        scene_->addLine(x, getY(*it), x + periodOffset_ / 2, getY(*it), pen_.red);
        scene_->addLine(x, getY(old), x, getY(*it), pen_.red);
        x += periodOffset_ / 2;
        old = *it;
    }
    }
}

void Dialog::drawDigits(QString seq) const {

    QPainterPath path;

    QFont font;
    font.setPixelSize(15);

    int x = 15;
    for (int i = 0; i < seq.size(); ++i) {
        path.addText(x, -80, font,  seq.at(i));
        scene_->addPath(path);
        x += periodOffset_;
    }
}


void Dialog::on_pushButton_clicked()
{
    LOG << "Clear()";
    ui_->lineEdit->clear();
}

void Dialog::on_groupBox_clicked()
{
        updateScene();
}
