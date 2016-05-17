#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QDialog>
#include <QMessageBox>

/*
*Quoted IN DEV, are in developpement components, they aren't influencing results, can be uncommented but no actions or not as we wish
*/


/*
*
*MAIN WINDOW
*
* Initialize Window, set to false buttons that aren't fully developped or not convincing, all can be changed to see react of program
* DO NOT change the comportement of the program marked, if IMPORTANT quote are mentionned, then it is useful !! Feel free to read comments to understand why
*
*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    imageNumber=0;

    /*
    *STYLE
    */
    this->setWindowTitle("Image Modifying Custom Software - Blanchon Marc"); // Changing title of window
    this->setStyleSheet("color:white;");//Color of font
    this->setStyleSheet("background-color:#282828;");//color of bakckground
    ui->Browserlabel->setStyleSheet("color:white;");//color of label
    ui->Explalabel->setStyleSheet("color:white;");//color of label
    ui->groupBox_8->setStyleSheet("background-color:#505050;");// background of groupbox right
    ui->DirectoryBrowser->setStyleSheet("color:white;");//color of browser
    ui->FileBrowser->setStyleSheet("color:white;");//color of browser


    ui->SaveFile->setStyleSheet("color:white;");
    ui->OpenFile->setStyleSheet("color:white;");
    ui->pushButton_3->setStyleSheet("color:white;");

    ui->groupBox_3->setStyleSheet("color:white;");
    ui->groupBox_4->setStyleSheet("color:white;");
    ui->groupBox_5->setStyleSheet("color:white;");
    ui->groupBox_6->setStyleSheet("color:white;");
    ui->groupBox_7->setStyleSheet("color:white;");

    ui->LAOButton->setStyleSheet("color:white;");




    //These 6 ui->Dir... can be commented to have access to some informations on folder when navigating, feel free to comment, but reduce readability of software
    //This Trick is not working .. So this is mandatory to keep Type, Size and Date tabs actives ..
    //Instead of using these commands, directly modifying CSS inside and modify the size of header tabs to be wider

    //ui->DirectoryBrowser->setColumnHidden(1,true);//Hiding column for better readability, remove size
    //ui->DirectoryBrowser->setColumnHidden(2,true);//Same, remove Type
    //ui->DirectoryBrowser->setColumnHidden(3,true);//Same, remove Date

    //ui->DirectoryBrowser->hideColumn(1);//Hiding column for better readability, remove size
    //ui->DirectoryBrowser->hideColumn(2);//Same, remove Type
    //ui->DirectoryBrowser->hideColumn(3);//Same, remove Date



    ui->HPButton->setEnabled(false);//In Dev, filtering completed , result not satisfying //Just change false by true to see result
    ui->LPButton->setEnabled(false);//In Dev, filtering completed , result not satisfying //Just change false by true to see result
    ui->KCenterButton->setEnabled(false);//In Dev, filtering completed , result not satisfying //Just change false by true to see result // ALL BLACK ..

    //TAB IN DEV
    ui->tabWidget->setTabEnabled(3,false);


    //IMPORTANT !! AVOID CRASHES DUE TO BAD SOFTWARE USES
    ui->tabWidget->setEnabled(false);//Set to false to makes the user select an image before manipulating //Change when click on a file
    //If Previous command is commented, the program will crash if user click on filter before selecting image

    Browse(); //Browser Mechanism packed into a function
}

MainWindow::~MainWindow()
{
    delete ui;

}

/*
*
*SLOTS CONNECTS
*
* Used for connect manually slots, used in really special cases, mostly removed (or totally removed if program done in another way) because of simplicity of QT
*
*/

void MainWindow :: ConnectToSlots()
{

}

/*
*
*BROWSER
*
* Declaration and react of the browser, mainly File management and Filtering entries
*
*/

void MainWindow :: Browse()//Browser System Filtering
{

    /*
    *Directory Browser Part
    */

    QString StartingPath = "C:/"; //Setting the start of the tree path
    directoryModel = new QFileSystemModel(this); //Creating the new object FileSystem used to build and return path at the end (Simple Browser System)

    directoryModel -> setFilter(QDir::NoDotAndDotDot | QDir::AllDirs); //Creating the filter for the tree view of directories  //Permit to separate Dir and Files
    //Filter Hide protected and special extansions dotanddot and it displays only directories
    directoryModel -> setRootPath(StartingPath); //Setting the path to the tree view of directory

    ui -> DirectoryBrowser -> setModel(directoryModel); //Giving the input of the tree view in gui


    /*
    *File Browser Part
    *Same Principle as Directory, pretty much a copy paste and changing the filters
    */

    fileModel = new QFileSystemModel(this);
    fileModel -> setFilter(QDir::NoDotAndDotDot | QDir::Files);
    //Filter Hide protected and special extansions dotanddot and it displays only Files
    fileModel -> setRootPath(StartingPath); //Setting the path to the tree view of directory

    ui -> FileBrowser -> setModel(fileModel); //Setting the first display before clicking as the same as Directory and change it when clicked in Directory


}

/*
*
*Saving Function
*
* Permits to save the file each time clicked on button
* Using maximum input parameters to minimize the code and the objects creation
*
*/


void MainWindow::SaveWithName(bool First, QString Name, FilteredImage* FI)
{
    if(First)//If this is the first time, then we will create folder !!
    {
        QString Check = absoluteFilePathSelected.split('/').last();//Deleting the Name of file in the path
        Check='/'+Check;

        savingFolderPathSelected=absoluteFilePathSelected.replace(Check,"");//Setting the Folder path
        FI->TemporaryFolder(savingFolderPathSelected);//Creating Temporary Folder

        absoluteFilePathSelected.append(Check); // Rebuild the filePath
    }

    FI->TemporarySave(FI->get_savingPath(),ui->DisplayLabel,Name);//Save Image


    /*
     *OverView in Developpement, Unable to Update ui and unable to display images on a second label
     *
    QString overviewPath=absoluteFilePathSelected;
    overviewPath=overviewPath.replace(overviewPath.split('/').last(),"")+"Temp";

    QDir dir1(overviewPath);//Setting the directory we will display all files

    foreach(QString dirFile, dir1.entryList())//for each file in folder //FOREACH1
    {
        Overview.insert(overviewPath+dirFile);//insert unique values of images in the set
    }//ENDOREACH1

    LoadOverview(imageNumber);

    update();

    */

}


/*
*
*LOADING OVERVIEW
*
*Just Displaying the image needed // NOT WORKING, IN DEV
*
*/

void MainWindow::LoadOverview(int imageNumber){
    std::set<QString>::iterator it;
    int compare=0;

    QMessageBox msgBox;//Initialize message Box
    msgBox.setText("LOL");//Set the title in bold
    msgBox.setStandardButtons(QMessageBox::Cancel);//Displaying the Cancel button
    msgBox.setDefaultButton(QMessageBox::Cancel);//Set the Default button as cancel, this is the only one in fact

    for (it=Overview.begin(); it!=Overview.end(); ++it){
        if(compare == imageNumber){
            ui->OverViewLabel->setPixmap(QPixmap(*it));
        }
        compare++;

        QMessageBox msgBox;//Initialize message Box
        msgBox.setText(*it);//Set the title in bold
        msgBox.setStandardButtons(QMessageBox::Cancel);//Displaying the Cancel button
        msgBox.setDefaultButton(QMessageBox::Cancel);//Set the Default button as cancel, this is the only one in fact
    }

    update();

}

/*
*
*BROWSER COMPLEMENT
*
* Only slots functions, permit also to initialize some variables useful in the whole program
*
*/

void MainWindow::on_DirectoryBrowser_clicked(const QModelIndex &index)//Clicking in the Directory Tree // To Ouput index to display files corresponding
{
    QString DirPath = directoryModel -> fileInfo(index).absoluteFilePath();// Getting the absolute path in the directory tree
    ui -> FileBrowser -> setRootIndex(fileModel -> setRootPath(DirPath)); // Setting the path in FileTree After this ikt displays only files in the clicked directory
}

void MainWindow::on_FileBrowser_clicked(const QModelIndex &index)//Clicking in File tree // Output the Path of the file
{
    absoluteFilePathSelected = fileModel -> fileInfo(index).absoluteFilePath();
}

/*
*
*OPEN FILE
*
* Checking format of the file clicked, if everything's good, then permits the user to click on buttons
*
*/

void MainWindow::on_OpenFile_clicked()//When Open is Clicked
{
    //ui -> TEST -> setText(absoluteFilePathSelected); // Test Path //Uncomment to see mechanism (label is hidden in top left)

    QString extansionCheck = absoluteFilePathSelected.split('.').last();

    //ui -> TEST2 -> setText(extansionCheck); //Test Extansion //Uncomment to see mechanism (label is hidden in top left)

    if(extansionCheck == "jpg" || extansionCheck == "jpeg" ||extansionCheck == "bmp" || extansionCheck == "png" || extansionCheck == "JPG" || extansionCheck == "JPEG" ||extansionCheck == "BMP" || extansionCheck == "PNG")//Checking extansions taking account caps // IF1
    {
        Image * I= new Image(absoluteFilePathSelected);//Creating Image Object
        I->Display(ui->DisplayLabel);
        ui->tabWidget->setEnabled(true);

        //STYLE BUTTONS when TRUE

        ui->BWButton->setStyleSheet("background-color:#505050;");
        ui->BWButton->setStyleSheet("color:white;");
        ui->SobelButton->setStyleSheet("background-color:#505050;");
        ui->SobelButton->setStyleSheet("color:white;");
        ui->GrayButton->setStyleSheet("background-color:#505050;");
        ui->GrayButton->setStyleSheet("color:white;");
        ui->LaplacianButton->setStyleSheet("background-color:#505050;");
        ui->LaplacianButton->setStyleSheet("color:white;");
        ui->PrewittButton->setStyleSheet("background-color:#505050;");
        ui->PrewittButton->setStyleSheet("color:white;");
        ui->ScharrButton->setStyleSheet("background-color:#505050;");
        ui->ScharrButton->setStyleSheet("color:white;");
        ui->RobertsButton->setStyleSheet("background-color:#505050;");
        ui->RobertsButton->setStyleSheet("color:white;");
        ui->EroButton->setStyleSheet("background-color:#505050;");
        ui->EroButton->setStyleSheet("color:white;");
        ui->DilaButton->setStyleSheet("background-color:#505050;");
        ui->DilaButton->setStyleSheet("color:white;");
        ui->ClosingButton->setStyleSheet("background-color:#505050;");
        ui->ClosingButton->setStyleSheet("color:white;");
        ui->OpeningButton->setStyleSheet("background-color:#505050;");
        ui->OpeningButton->setStyleSheet("color:white;");
        ui->WhiteButton->setStyleSheet("background-color:#505050;");
        ui->WhiteButton->setStyleSheet("color:white;");
        ui->BlackButton->setStyleSheet("background-color:#505050;");
        ui->BlackButton->setStyleSheet("color:white;");


    }else{//When the file is not an image
        QMessageBox msgBox;//Initialize message Box
        msgBox.setText("<b>The selected file is not an image.</b>\n");//Set the title in bold
        msgBox.setInformativeText("You are using <b>"+ extansionCheck + "</b> instead of standards extansions (jpg, jpeg, bmp, png).");//Explaining the error and bolding the source of error
        msgBox.setStandardButtons(QMessageBox::Cancel);//Displaying the Cancel button
        msgBox.setDefaultButton(QMessageBox::Cancel);//Set the Default button as cancel, this is the only one in fact
        msgBox.exec();//Execute the messageBox
    }//ENDIF1

}

/*
*
*CLOSE WITHOUT SAVING (Name of pushbutton isn't changed, this is a mystake)
*
* TRICK1: Every time the program run, it create temp folder, with this button, it will let no folder, so this is the invert function as usuall but really usefull
*
*/

void MainWindow::on_pushButton_3_clicked()//Close without saving, It delete temporary Folder If Exist
{

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit" , "Are you sure you want to Quit without Saving?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
          FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

          QString DeletingPath=absoluteFilePathSelected;
          DeletingPath=DeletingPath.replace(DeletingPath.split('/').last(),"Temp");

          FI->DeleteTemporaryFolder(DeletingPath);

          exit(0);
     }
}

/*
*
*CLOSE WITH SAVE
*
* TRICK2: instead of deleting the files, it just do nothing and close window !! :O
*
*/

void MainWindow::on_SaveFile_clicked()//Close with saving, It keeps temporary Folder If Exist
{

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit" , "Are you sure you want to Quit with Saving?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
         exit(0);
      }
}


/*
*
*START BIG SECTION / FIRST TAB IMAGE PROCESSING
*
*/


/*
*
*FILTERS
*
* Kind of copy paste for each buttons, just changing name of save and name of called function then append each objetcs in vector
* Function is commented only the first time, to have more readability of code after
*
*/

void MainWindow::on_GrayButton_clicked()
{

    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);//Create Object FilteredImage

    if(FirstLaunch){//Is it the first time program is running? //IF1

        SaveWithName(FirstLaunch,"BaseImage", FI);//Then save the base image, without changes in the temp folder
        FirstLaunch=false;//Set first time runing to false, so we will never recall the base image saving till we don't close program
    }//ENDIF1


    FI->GrayScale(ui->DisplayLabel,absoluteFilePathSelected);//Call of the selected function in function of the button clicked
    update();//Update the user interface, like this is it safe, and no useless overcharge of code (not adding ui commands) in functions
    //This could be used in functions, but we had to pass ui in pointer to the function to being able to refresh the ui, it will be really heavy

    Images.push_back(FI);//pushback the Image Vector with filtered image

    SaveWithName(FirstLaunch,"GrayScale", FI);//Saving the new image in the folder, passing FirstLaunch as argument because it is a trick

}



void MainWindow::on_SobelButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->Sobel_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"Sobel", FI);

}

void MainWindow::on_PrewittButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->Prewitt_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"Prewitt", FI);
}

void MainWindow::on_ScharrButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->Scharr_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"Scharr", FI);
}

void MainWindow::on_RobertsButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->Roberts_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"Roberts", FI);
}

void MainWindow::on_LaplacianButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->Laplacian_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"Laplacian", FI);

}

void MainWindow::on_BWButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->BW_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"BlackWhite", FI);
}

void MainWindow::on_HPButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->HP_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"HighPass", FI);
}

void MainWindow::on_LPButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->LP_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"LowPass", FI);

}

void MainWindow::on_EroButton_clicked()
{

    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->Ero_Morph(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"Erosion", FI);

}

void MainWindow::on_DilaButton_clicked()
{

    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->Dila_Morph(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"Dilation", FI);
}

void MainWindow::on_ClosingButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->Closing_Morph(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"Closing", FI);
}

void MainWindow::on_OpeningButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->Opening_Morph(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"Opening", FI);
}

void MainWindow::on_WhiteButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->WhiteTopHat_Morph(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"WhiteTopHat", FI);
}

void MainWindow::on_BlackButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->BlackTopHat_Morph(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"BlackTopHat", FI);
}


/*
*
*END BIG SECTION / FIRST TAB IMAGE PROCESSING
*
*/



/*
*
*START BIG SECTION / SECOND TAB KIRSCH/ORIENTED
*
*/

void MainWindow::on_KTopButton_clicked()
{

    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->KirschTop_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"KirschTop", FI);

}

void MainWindow::on_KTopLeftButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->KirschTopLeft_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"KirschTopLeft", FI);
}

void MainWindow::on_KLeftButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->KirschLeft_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"KirschLeft", FI);
}


void MainWindow::on_KBotLeftButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->KirschBottomLeft_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"KirschBottomLeft", FI);
}


void MainWindow::on_KBotButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->KirschBottom_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"KirschBottom", FI);
}


void MainWindow::on_KBotRightButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->KirschBottomRight_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"KirschBottomRight", FI);
}


void MainWindow::on_KRightButton_clicked()
{

    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->KirschRight_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"KirschRight", FI);
}


void MainWindow::on_KTopRightButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->KirschTopRight_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"KirschTopRight", FI);
}


void MainWindow::on_KCenterButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->KirschCenter_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"Kirsch8Direction", FI);
}

void MainWindow::on_KCrossButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->KirschCross_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"KirschCross", FI);
}

void MainWindow::on_KNSButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->KirschNS_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"KirschNorthSouth", FI);
}

void MainWindow::on_KEWButton_clicked()
{
    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->KirschEW_Filter(ui->DisplayLabel,absoluteFilePathSelected);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"KirschEastWest", FI);
}


/*
*
*END BIG SECTION / SECOND TAB KIRSCH/ORIENTED
*
*/



/*
*
*START BIG SECTION / THIRD TAB AESTHETIC
*
*/


void MainWindow::on_pushButton_clicked()//Brightness change //Mystake not changing the name of pushbutton....
{
    int bright=ui->BrightSlider->value();

    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->BrightnessChange(ui->DisplayLabel,absoluteFilePathSelected,bright);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"BrightnessChange", FI);
}

void MainWindow::on_ContrastButton_clicked()
{
    double contrast=ui->ContrastSlider->value();
    contrast/=100;

    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->ContrastChange(ui->DisplayLabel,absoluteFilePathSelected,contrast);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"ContrastChange", FI);
}


void MainWindow::on_GammaButton_clicked()
{

    double gamma=ui->GammaSlider->value();

    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->GammaChange(ui->DisplayLabel,absoluteFilePathSelected,gamma);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"GammaChange", FI);

}

void MainWindow::on_LAOButton_clicked()
{
    int bright=ui->BrightSlider->value();

    double contrast=ui->ContrastSlider->value();
    contrast/=100;

    double gamma=ui->GammaSlider->value();

    FilteredImage* FI= new FilteredImage(absoluteFilePathSelected);

    if(FirstLaunch){

        SaveWithName(FirstLaunch,"BaseImage", FI);
        FirstLaunch=false;
    }


    FI->LOAChange(ui->DisplayLabel,absoluteFilePathSelected, bright, contrast ,gamma);
    update();

    Images.push_back(FI);

    SaveWithName(FirstLaunch,"ValuesChanged", FI);


}

/*
*
*END BIG SECTION / THIRD TAB AESTHETICS
*
*/


/*
*
*START BIG SECTION / FOURTH TAB OVERVIEW
*IN DEV ! NOT WORKING
*
*/


void MainWindow::on_OverviewDisplayButton_clicked()
{
    QMessageBox msgBox3;//Initialize message Box
    msgBox3.setText("blabla");//Set the title in bold
    msgBox3.setStandardButtons(QMessageBox::Cancel);//Displaying the Cancel button
    msgBox3.setDefaultButton(QMessageBox::Cancel);//Set the Default button as cancel, this is the only one in fact

    LoadOverview(imageNumber);
}


/*
*
*END BIG SECTION / FOURTH TAB OVERVIEWS
*IN DEV !
*
*/
