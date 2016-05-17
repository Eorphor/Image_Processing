#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>
#include <image.h>
#include <vector>
#include <set>

/*
*Quoted IN DEV, are in developpement components, they aren't influencing results, can be uncommented but no actions or not as we wish
*/


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //Special Variable

    bool FirstLaunch; //Is it the first time the program run?


    //IN DEV
    int imageNumber; //The number of the image overview
    //END IN DEV


    //Storing our objects

    std::vector<Image*> Images;

    void ConnectToSlots(); //Function connection to slots
    void Browse(); //Function for File Browser
    void SaveWithName(bool ,QString, FilteredImage*); // Function saving, compact with maximized inputs to avoid useless object creating
    void LoadOverview(int);

private slots:
    void on_DirectoryBrowser_clicked(const QModelIndex &index);

    void on_OpenFile_clicked();

    void on_FileBrowser_clicked(const QModelIndex &index);

    void on_GrayButton_clicked();

    void on_pushButton_3_clicked();

    void on_SaveFile_clicked();

    void on_SobelButton_clicked();

    void on_PrewittButton_clicked();

    void on_ScharrButton_clicked();

    void on_RobertsButton_clicked();

    void on_LaplacianButton_clicked();

    void on_BWButton_clicked();

    void on_HPButton_clicked();

    void on_LPButton_clicked();

    void on_EroButton_clicked();

    void on_DilaButton_clicked();

    void on_ClosingButton_clicked();

    void on_OpeningButton_clicked();

    void on_WhiteButton_clicked();

    void on_BlackButton_clicked();

    void on_KTopButton_clicked();

    void on_KTopLeftButton_clicked();

    void on_KLeftButton_clicked();

    void on_KBotLeftButton_clicked();

    void on_KBotButton_clicked();

    void on_KBotRightButton_clicked();

    void on_KRightButton_clicked();

    void on_KTopRightButton_clicked();

    void on_pushButton_clicked();

    void on_ContrastButton_clicked();

    void on_GammaButton_clicked();

    void on_KCenterButton_clicked();

    void on_KCrossButton_clicked();

    void on_KNSButton_clicked();

    void on_KEWButton_clicked();

    void on_LAOButton_clicked();

    void on_OverviewDisplayButton_clicked();

private:
    Ui::MainWindow *ui;

    /*
    * Custom File BROWSER
    */

    QFileSystemModel *directoryModel;                             //Object corresponding to the Directory Browser
    QFileSystemModel *fileModel;                            //Object corresponding to the File Browser

    QString absoluteFilePathSelected;                                   //Path of the Selected File
    QString savingFolderPathSelected;                                   //Path of the Saving Folder


    /*
    *OverView
    *IN DEV, NOT WORKING
    */

    std::set<QString> Overview; //Because unique values

    //END IN DEV
};

#endif // MAINWINDOW_H
