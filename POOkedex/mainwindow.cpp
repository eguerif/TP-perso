#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QString>
#include <QDebug>
#include <QUrl>
#include <QBrush>
#include <QRegion>
#include <QStack>
#include <QChar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(QSize(800, 542));
    QPixmap bkgnd(":/pokeRessources/pokedex-bg-1668673433.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::KeepAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    //boutons invisibles
    qApp->setStyleSheet("QPushButton { background-color: rgba(10, 0, 0, 0);}");

    //bouton round
    ui->Validation->setFixedHeight(200);
    ui->Validation->setFixedWidth(200);
    QRect rect(4,4,60,60);
    QRegion round(rect, QRegion::Ellipse);
    ui->Validation->setMask(round);
    ui->PokePrecedent->setFixedHeight(200);
    ui->PokePrecedent->setFixedWidth(200);
    QRect rect2(0,0,40,40);
    QRegion round2(rect2, QRegion::Ellipse);
    ui->PokePrecedent->setMask(round2);

    manager = new QNetworkAccessManager(this);
    Precedentpoke = new QStack<int>;
    getPokemon(25);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete manager;
    delete Precedentpoke;
}

void MainWindow::getPokemon(int id){
    pokemonReply = manager->get(QNetworkRequest(QUrl("https://pokeapi.co/api/v2/pokemon/" + QString::number(id))));
    connect(pokemonReply, SIGNAL(finished()),this, SLOT(managePokemonReply()));
    Precedentpoke->push(id);
}

void MainWindow::getImg(QString nom){
    imgReply = manager->get(QNetworkRequest(QUrl("https://img.pokemondb.net/sprites/home/normal/" + nom +".png")));
    connect(imgReply, SIGNAL(finished()), this, SLOT(manageImgReply()));
}

void MainWindow::managePokemonReply(){
    QString source = pokemonReply->readAll();
    QString display;
    QJsonObject pokeData;
    QJsonArray nbType;
    QJsonObject pokeType;

    QJsonDocument doc = QJsonDocument::fromJson(source.toUtf8());

    if(!doc.isNull()) {

        if(doc.isObject()) {
            pokeData = doc.object();
            ui->NomPokemon->setText(pokeData["name"].toString());
            getImg(pokeData["name"].toString());
            nbType = pokeData["types"].toArray();
            if(nbType.count() == 1){
                pokeType = nbType[0].toObject()["type"].toObject();
                display = "(" + pokeType["name"].toString() + ")";
                ui->typePoke->setText(display);
            }
            else{
                pokeType = nbType[0].toObject()["type"].toObject();
                display = pokeType["name"].toString();
                pokeType = nbType[1].toObject()["type"].toObject();
                display = "(" + display + ", " + pokeType["name"].toString() + ")";
                ui->typePoke->setText(display);
            }
        }
        else {
            //qDebug() << "La réponse n’est pas correctement formée." << endl;
        }
    }
    else {
        ui->NomPokemon->setText("erreur de connexion internet");
        //qDebug() << "La requête a renvoyé une réponse vide." << endl;
    }
}

void MainWindow::manageImgReply(){
    QByteArray imgData = imgReply->readAll();

    QPixmap pixmap;

    pixmap.loadFromData(imgData);

    if (!pixmap.isNull()) {

        ui->IMG->clear();
        ui->IMG->setPixmap(pixmap.scaled(ui->IMG->size(), Qt::KeepAspectRatio));
    }

}

void MainWindow::on_Validation_clicked()
{
    ui->NomPokemon->setText("Ce pokemon n'existe pas");
    getPokemon((ui->lineEdit->text()).toInt());
}

void MainWindow::on_lineEdit_returnPressed()
{
    ui->NomPokemon->setText("Ce pokemon n'existe pas");
    getPokemon((ui->lineEdit->text()).toInt());
}

void MainWindow::on_PokeP1_clicked()
{
    if(ui->lineEdit->text().toInt() < 905){
        ui->lineEdit->setText(QString::number(ui->lineEdit->text().toInt()+1));
        getPokemon(ui->lineEdit->text().toInt());
    }
    else{
        ui->lineEdit->setText("1");
        getPokemon(ui->lineEdit->text().toInt());
    }
}

void MainWindow::on_PokeM1_clicked()
{
    if(ui->lineEdit->text().toInt() > 1){
        ui->lineEdit->setText(QString::number(ui->lineEdit->text().toInt()-1));
        getPokemon(ui->lineEdit->text().toInt());
    }
    else{
        ui->lineEdit->setText("905");
        getPokemon(ui->lineEdit->text().toInt());
    }
}

void MainWindow::on_PokeP10_clicked()
{
    if(ui->lineEdit->text().toInt() < 896){
        ui->lineEdit->setText(QString::number(ui->lineEdit->text().toInt()+10));
        getPokemon(ui->lineEdit->text().toInt());
    }
    else{
        ui->lineEdit->setText("1");
        getPokemon(ui->lineEdit->text().toInt());
    }
}

void MainWindow::on_PokeM10_clicked()
{
    if(ui->lineEdit->text().toInt() > 10){
        ui->lineEdit->setText(QString::number(ui->lineEdit->text().toInt()-10));
        getPokemon(ui->lineEdit->text().toInt());
    }
    else{
        ui->lineEdit->setText("905");
        getPokemon(ui->lineEdit->text().toInt());
    }
}

void MainWindow::on_PokePrecedent_clicked()
{
    if(Precedentpoke->size() >=2){
        Precedentpoke->pop();
        getPokemon(Precedentpoke->pop());
    }
}
