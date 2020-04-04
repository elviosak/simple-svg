#include "mainwindow.h"

MainWindow::~MainWindow()
{
}

MainWindow::MainWindow(QString arg, QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/appIcon"));
    setWindowTitle("Simple Svg");
    QWidget * central = new QWidget(this);
    QVBoxLayout * vbox = new QVBoxLayout();
    vbox->setSpacing(0);
    vbox->setAlignment(Qt::AlignHCenter);
    central->setLayout(vbox);
    setCentralWidget(central);
    QString txt = "<svg xmlns=\"http://www.w3.org/2000/svg\" "
                  "width=\"48\" height=\"48\" fill=\"none\" "
                  "stroke-linecap=\"round\" stroke-linejoin=\"round\">\n\n"
                  "<circle stroke-width=\"4\" stroke=\"#2fcee9\" cx=\"24\" cy=\"24\" r=\"22\"/>\n"
                  "<path stroke-width=\"4\" stroke=\"#2fcee9\"\n"
                  "d=\"M12 14 v20M13 24h10M24 14v20 M34 14v1"
                  "M34 24v10\""
                  "/>\n\n"
                  "</svg>";

    btn = new QPushButton();
    btn->setObjectName("viewer");
    btn->setFixedSize(128,128);
    btn->setIconSize(QSize(128,128));
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setStyleSheet("#viewer {background-color:#ddd;margin:0;border:0;padding:0;}");
    QSlider * sliderH = new QSlider(Qt::Vertical);
    QLabel * sizeLabel = new QLabel("128x128");
    sliderH->setRange(4,9);
    sliderH->setValue(7);
    sliderH->setTickInterval(1);
    sliderH->setSingleStep(1);
    sliderH->setPageStep(1);

    sliderH->setTickPosition(QSlider::TicksLeft);
    connect(sliderH, &QSlider::valueChanged, this, [=](int val){
        int newVal = 1;
        for (int i = 1; i<=val; ++i){
            newVal *= 2;
        }
        btn->setFixedSize(newVal, newVal);
        btn->setIconSize(QSize(newVal, newVal));
        sizeLabel->setText(QString::number(newVal) + "x" + QString::number(newVal));
        updateSvg();
    });



    QGridLayout * gridBtn = new QGridLayout();

    gridBtn->addWidget(btn, 0, 0, Qt::AlignCenter);
    gridBtn->addWidget(sliderH, 0,1);
    gridBtn->addWidget(sizeLabel, 1, 0, 1, 1, Qt::AlignCenter);
    vbox->addLayout(gridBtn);
    vbox->addSpacing(10);



    QLineEdit * lineColor = new QLineEdit();
    lineColor->setText("#ddd");
    connect(lineColor, &QLineEdit::textChanged, this, [=](QString colorText){
        if (QColor(colorText).isValid())
            btn->setStyleSheet("#viewer {background-color:"+ colorText +";margin:0;border:0;padding:0;}");
    });
    QPushButton * btnColor = new QPushButton("Pick");
    connect(btnColor, &QPushButton::clicked, this, [=](){
        QColor color = QColorDialog::getColor(QColor(lineColor->text()));
        if (color.isValid()){
            lineColor->setText(color.name());
            btn->setStyleSheet("#viewer {background-color:"+ color.name() +";margin:0;border:0;padding:0;}");
        }

    });

    QHBoxLayout * boxColor = new QHBoxLayout();
    boxColor->addWidget(lineColor);
    boxColor->addWidget(btnColor);
    vbox->addLayout(boxColor);
    vbox->addSpacing(10);

    text = new QTextEdit();
    text->setFontFamily("Mono");
    text->setAcceptRichText(false);
    text->setMinimumHeight(250);
    text->setMinimumWidth(300);
    text->setTabStopWidth(20);
    text->setText(txt);


    QTextCursor cursor = text->textCursor();
    cursor.movePosition(QTextCursor::EndOfLine);
    cursor.movePosition(QTextCursor::Right);
    text->setTextCursor(cursor);

    QToolBar * bar = new QToolBar();
    QString code;
    int offset;
    code = "<path d=\"M0 0\"/>";
    offset = 9;
    bar->addAction(createAction("path", code, offset));
    code = "<line x1=\"\" y1=\"\" x2=\"\" y2=\"\" stroke=\"\"/>";
    offset = 10;
    bar->addAction(createAction("line", code, offset));
    code = "<rect x=\"\" y=\"\" width=\"\" height=\"\" rx=\"\"/>";
    offset = 9;
    bar->addAction(createAction("rect", code, offset));

    code = "<circle cx=\"\" cy=\"\" r=\"\"/>";
    offset = 12;
    bar->addAction(createAction("circle", code, offset));

    code = "<ellipse cx=\"\" cy=\"\" rx=\"\" ry=\"\"/>";
    offset = 13;
    bar->addAction(createAction("ellipse", code, offset));

    code = "<polyline points=\"0,0\" fill=\"\" />";
    offset = 18;
    bar->addAction(createAction("polyline", code, offset));

    vbox->addWidget(bar);

    vbox->addWidget(text);
    text->setFocus();

    QHBoxLayout * boxOpenSave = new QHBoxLayout();
    QPushButton * btnSave = new QPushButton(QIcon(":/save"),"Save");
    QPushButton * btnOpen = new QPushButton(QIcon(":/open"),"Open");
    boxOpenSave->addWidget(btnOpen);
    boxOpenSave->addWidget(btnSave);
    vbox->addSpacing(10);
    vbox->addLayout(boxOpenSave);
    connect(btnOpen, &QPushButton::clicked, this, &MainWindow::open);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::save);

    connect(text, &QTextEdit::textChanged, this, &MainWindow::updateSvg);
    if(arg.length()>0){
        openArg(arg);
    }
    else{
      updateSvg();
    }

}
QAction * MainWindow::createAction(QString name, QString code, int offset){
    QAction * a = new QAction(name);
    connect(a, &QAction::triggered, this, [=]{
        QTextCursor cursor = text->textCursor();
        int initialPosition = cursor.position();
        qDebug() << "position" << initialPosition << "offset" << offset;
        cursor.insertText(code + "\n");
        cursor.setPosition(initialPosition + offset);
        text->setTextCursor(cursor);
    });
    return a;
}

QPushButton * MainWindow::createButton(QString name, QString code, int offset){
    QPushButton * button = new QPushButton(name);
    button->setObjectName("button");
    button->setFocusPolicy(Qt::NoFocus);
    connect(button, &QPushButton::clicked, this, [=]{
        QTextCursor cursor = text->textCursor();
        int initialPosition = cursor.position();
        qDebug() << "position" << initialPosition << "offset" << offset;
        cursor.insertText(code + "\n");
        cursor.setPosition(initialPosition + offset);
        text->setTextCursor(cursor);
    });
    return button;
}
void MainWindow::openArg(QString filePath){
    if(QFile::exists(filePath)){
        QFile file;
        file.setFileName(filePath);
        file.open(QIODevice::ReadOnly);
        text->setText(QString(file.readAll()));
        file.close();
    }
    else {
        updateSvg();
    }
}

void MainWindow::open(){
    QUrl url = QFileDialog::getOpenFileUrl(this, "Select file:", QUrl(), "Svg File (*.svg)");
    QString filePath = url.toLocalFile();
    qDebug() << "filePath" <<filePath;
    if(QFile::exists(filePath)){
        QFile file;
        file.setFileName(filePath);
        file.open(QIODevice::ReadOnly);
        text->setText(QString(file.readAll()));
        file.close();
    }
}

void MainWindow::save(){

    QUrl url = QFileDialog::getSaveFileUrl(this, "Save file:", QUrl(), "Svg File (*.svg)");
    QString filePath = url.toLocalFile();
    QFile file;
    QByteArray data = text->toPlainText().toUtf8();
    file.setFileName(filePath);

    if(file.exists()){
        bool removed = file.remove();
        qDebug() << filePath << "removed" << removed;
    }
    file.open(QIODevice::WriteOnly);
    auto written = file.write(data);
    file.close();
    qDebug() << "write" << written;


}

void MainWindow::updateSvg(){
    QSvgRenderer * svgrender = new QSvgRenderer();
    QByteArray data = QString(text->toPlainText()).toUtf8();
    svgrender->load(data);
    QPixmap pix(btn->size());
    pix.fill(Qt::GlobalColor::transparent);
    QPainter p;
    p.begin(&pix);
    svgrender->render(&p);
    p.end();
    btn->setIcon(pix);
}

