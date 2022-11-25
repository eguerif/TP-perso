#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void getPokemon(int);
    void getPokePrecedent(int);
    void getImg(QString);

private slots:
    void managePokemonReply();
    void manageImgReply();
    void on_Validation_clicked();

    void on_PokeP1_clicked();

    void on_PokeM1_clicked();

    void on_PokeP10_clicked();

    void on_PokeM10_clicked();

    void on_PokePrecedent_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QNetworkReply *pokemonReply;
    QNetworkReply *imgReply;
    QStack<int> *pokePrecedent;
    int *save;
};

#endif // MAINWINDOW_H
