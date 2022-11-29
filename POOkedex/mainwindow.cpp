#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
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

    //visuel pokedex
    setFixedSize(QSize(800, 542));
    QPixmap bkgnd(":/pokeRessources/pokedex-bg-1668673433.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::KeepAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    //boutons invisibles
    qApp->setStyleSheet("QPushButton{background:transparent;}");

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

    //toolTip
    ui->Validation->setToolTip("bouton de validation");
    ui->lineEdit->setToolTip("ID du pocket monster");
    ui->PokeP1->setToolTip("Pokemon suivant");
    ui->PokeM1->setToolTip("Pokemon précedent");
    ui->PokeP10->setToolTip("10ème pokemon suivant");
    ui->PokeM10->setToolTip("10ème pokemon précedent");
    ui->PokePrecedent->setToolTip("Retour en arrière");
    ui->VisuelS->setToolTip("Visuel suivant");
    ui->VisuelP->setToolTip("Visuel précedent");
    ui->NomPokemon->setToolTip("Nom du pokemon");
    ui->typePoke->setToolTip("Type du pokemon");
    ui->IMG->setToolTip("tu as vraiment besoin de savoir c'est quoi ?");

    manager = new QNetworkAccessManager(this);
    Precedentpoke = new QStack<int>;

    //affichage pikachu
    getPokemon(25);
    ui->lineEdit->setText("25");
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
    count = 0;
}

void MainWindow::getImg(QString nom){
    imgReply = manager->get(QNetworkRequest(QUrl("https://img.pokemondb.net/sprites/home/normal/" + nom +".png")));
    connect(imgReply, SIGNAL(finished()), this, SLOT(manageImgReply()));
}

void MainWindow::getVectorImg(QString nom){
    imgReply = manager->get(QNetworkRequest(QUrl("https://img.pokemondb.net/artwork/vector/large/" + nom +".png")));
    connect(imgReply, SIGNAL(finished()), this, SLOT(manageImgReply()));
}

void MainWindow::getSSImg(QString nom){
    imgReply = manager->get(QNetworkRequest(QUrl("https://img.pokemondb.net/sprites/sword-shield/icon/" + nom +".png")));
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
            display = pokeData["name"].toString();
            name = display;
            display[0] = display[0].toUpper();
            ui->NomPokemon->setText(display);
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
        if(ui->lineEdit->text().toInt() > 905 || ui->lineEdit->text().toInt() < 1){
            ui->NomPokemon->setText("Ce pokemon n'existe pas\nessayez entre 1 et 905");
            ui->typePoke->clear();
            ui->IMG->clear();
        }
        else{
            ui->NomPokemon->setText("erreur de connexion internet");
            ui->typePoke->clear();
            ui->IMG->clear();
        }
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
    getPokemon((ui->lineEdit->text()).toInt());
}

void MainWindow::on_lineEdit_returnPressed()
{
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

void MainWindow::on_VisuelS_clicked()
{
    if(count == 2){
        count = 0;
        getImg(name);
    }
    else if(count == 1){
        count++;
        getSSImg(name);
    }
    else{
        count++;
        getVectorImg(name);
    }
}

void MainWindow::on_VisuelP_clicked()
{
    if(count == 2){
        count--;
        getVectorImg(name);
    }
    else if(count == 1){
        count--;
        getImg(name);
    }
    else{
        count = 2;
        getSSImg(name);
    }
}
