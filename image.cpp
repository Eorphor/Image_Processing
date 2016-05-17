#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QDir>
#include <QtGui>
#include "image.h"
#include <QMessageBox>
#include <cmath>

/*
*Quoted IN DEV, are in developpement components, they aren't influencing results, can be uncommented but no actions or not as we wish
*/


/*
*
*IMAGE FUNCTION WHICH IS NOT DERIVED
*
* This function is called only at start, this is the reason it's not a pure constant function, because we declare it and never redeclare it, so it's not Polymorphism
*
*/


void Image :: Display(QLabel* label)//Displaying the Image
{
    label->setPixmap(QPixmap(get_path())); //Displaying via path

    if((QPixmap(get_path()).height() > label->height()) && (QPixmap(get_path()).width() > label->width()))//Hack to have the full image size dependent of the label size //IF1
    {//This hack is usefull only if the full image sizes (height and width) are so wide. If we consider each parameters, we can have so many image defomation so taking both parameters in one condition
        label->setScaledContents(true);
    }//ENDIF1
}


/*
*
*FUNCTIONALITIES // Polymorphism (possibility to add many other classes and define other way of work for each functions, here we only derive one class but we could add "GrayScaledImage" class)
*
* Mainly functions about manipulating files, folders, saves, images
* In some functions some lines are commented, if it is mentionned to be possible, feel free to uncomment and test the result, it will all apear in messageBoxes
*
*/

void FilteredImage :: TemporaryFolder(QString path)
{
   //Comments are debugging, to see the process just uncomment the 3 commented blocks about QMessageBox

   // QMessageBox msgBox;//Initialize message Box
   // msgBox.setText(path);//Set the title in bold
   // msgBox.setStandardButtons(QMessageBox::Cancel);//Displaying the Cancel button
   // msgBox.setDefaultButton(QMessageBox::Cancel);//Set the Default button as cancel, this is the only one in fact
   // msgBox.exec();//Execute the messageBox

    int numberCheck=0;
    bool Loop=true;

    if(QDir(path+"/Temp").exists())//IF1
    {
       // QMessageBox msgBox;//Initialize message Box
       // msgBox.setText(path + "NOT OOOK");//Set the title in bold
       // msgBox.setStandardButtons(QMessageBox::Cancel);//Displaying the Cancel button
       // msgBox.setDefaultButton(QMessageBox::Cancel);//Set the Default button as cancel, this is the only one in fact
       // msgBox.exec();//Execute the messageBox

        while(Loop){//WHILE1    //Trick to have the maximum number in folders, then another loop to decrement
            if(QDir(path+"/TempOld"+QString::number(numberCheck)).exists()){//IF2
                numberCheck++;
            }else{
                Loop = false;
                }//ENDIF2
        }//ENDWHILE1

        while(numberCheck>=0)
        {
            QDir(path+"/TempOld"+QString::number(numberCheck)).rename(path+"/TempOld"+QString::number(numberCheck), path+"/TempOld"+QString::number(numberCheck+1));
            numberCheck--;

        }

        QDir(path+"/Temp").rename(path+"/Temp", path+"/TempOld"+QString::number(0));
    }else{


    }//ENDIF1



        // QMessageBox msgBox;//Initialize message Box
        // msgBox.setText(path + "OOOK");//Set the title in bold
        // msgBox.setStandardButtons(QMessageBox::Cancel);//Displaying the Cancel button
        // msgBox.setDefaultButton(QMessageBox::Cancel);//Set the Default button as cancel, this is the only one in fact
        // msgBox.exec();//Execute the messageBox

        savingPath=path+"/Temp";
        QDir().mkdir(savingPath);


}


void FilteredImage :: DeleteTemporaryFolder(QString path) const//Recursive function for the temporary folder
{
    QDir dir(path);//Setting the directory we will delete all files then delete himself

    foreach(QString dirFile, dir.entryList())//for each file in folder, delete the file , trick using foreach loop //FOREACH1
    {
        dir.remove(dirFile);//Delete files
    }//ENDOREACH1

    QString comeBack="/Temp";//Setting reglular expression to filer

    path.replace(comeBack, "");//Replacing in the path to come back in folders

    QDir NextDir(path);


    if (!NextDir.rmdir(dir.absolutePath())) //Deleting the Temporary folder and alert if unable //IF1
    {
        QMessageBox msgBox;//Initialize message Box
        msgBox.setText("Unable to delete Temporary Folder");//Alert Message
        msgBox.setInformativeText("You didn't work on project so no Temporary Folder created");//Explaining the error
        msgBox.setStandardButtons(QMessageBox::Cancel);//Displaying the Cancel button
        msgBox.setDefaultButton(QMessageBox::Cancel);//Set the Default button as cancel, this is the only one in fact
        msgBox.exec();//Execute the messageBox
    }//ENDIF1


}

void FilteredImage :: TemporarySave(QString path, QLabel* label, QString functionName)
{
    int numberCheck=0;//Number of the image
    bool Loop=true;//Loop COndition
    path.replace(path.split('/').last(),"");
    path+="Temp/"; //Setting to the temp Folder

    //savingPath=path;

    while(Loop)//Calling a loop to add the number in function of existing files //WHILE1
    {
        if(!QFile(path+"Filtered"+functionName+QString::number(numberCheck)+".jpg").exists())//Looking if the file exist //IF1     //Type of check Path PathTemporaryFolder//FunctionName"Number"
        {

            label->pixmap()->save(path+"Filtered"+functionName+QString::number(numberCheck)+".jpg");//Saving Image with correct Name without erasing others
            Loop=false;//Loop Exist


        }else{
            numberCheck++;

        }//ENDIF1
    }//ENDWHILE1
}





/*
*
*START USEFULL FUNCTIONS
*
*/

template <typename T>//Templated to be able to deal with int or double arrays
void FilteredImage::ApplyOneFilter(QLabel* label, QString path, T ValueTab1[9])const //Generic Function to appy one filter to image
{//Enter label where display image, path of the main image, and the filter as an array of 9 elements
    QPixmap DisplayedImage(path);//Pixmap to display

    QImage image = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height;
    QRgb color;
    width=image.width();
    height=image.height();

    QPixmap imageFirstPix(path);
    QImage imageFirst = imageFirstPix.toImage();//Converting to Image

    int sumPixel,count=0;

    //Double Loop for Pixels ignoring contour using filter
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            sumPixel=0;
            count=0;

            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= image.pixel(i+l, j+m);//Getting the parameters of pixel

                    sumPixel+= ValueTab1[count]*color;//Setting the values in function of counter and matrix vector
                    count++;

                }//ENDFOR3
            }//ENDFOR4

            imageFirst.setPixel(i, j, qRgb(sumPixel, sumPixel, sumPixel));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1



    //Double Loop for calculating the approximation
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {

            if(i == 0 || j == 0 || i == width-1 || j == height-1){//IF1
                image.setPixel(i,j,qRgb(255, 255, 255));//White contour
            }else{

                color=imageFirst.pixel(i,j);
                image.setPixel(i,j,qRgb(color,color,color));
            }//ENDIF1


        }//ENDFOR2
    }//ENDFOR1




    DisplayedImage=DisplayedImage.fromImage(image);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label
}

template <typename Ta, typename Tb>
void FilteredImage::ApplyTwoFilters(QLabel* label, QString path, Ta ValueTab1[9], Tb ValueTab2[9])const //Generic Function to appy two filters to image
{//Enter label where display image, path of the main image, and the filters as arrays of 9 elements

    QPixmap DisplayedImage(path);//Pixmap to display

    QImage image = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height;
    QRgb color;
    width=image.width();
    height=image.height();

    QPixmap imageFirstPix(path);
    QImage imageFirst = imageFirstPix.toImage();//Converting to Image

    QPixmap imageSecondPix(path);
    QImage imageSecond = imageSecondPix.toImage();//Converting to Image

    int sumPixel,count=0;

    //Double Loop for Pixels ignoring contour
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            sumPixel=0;
            count=0;

            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= image.pixel(i+l, j+m);//Getting the parameters of pixel

                    sumPixel+= ValueTab1[count]*color;//Setting the values in function of counter and matrix vector
                    count++;

                }//ENDFOR3
            }//ENDFOR4

            imageFirst.setPixel(i, j, qRgb(sumPixel, sumPixel, sumPixel));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1


    count=0;
    //Double Loop for Pixels ignoring contour::2nd matrix
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {
            sumPixel=0;
            count=0;

            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= image.pixel(i+l, j+m);//Getting the parameters of pixel

                    sumPixel+= ValueTab2[count]*color;//Setting the values in function of counter and matrix vector
                    count++;

                }//ENDFOR3
            }//ENDFOR4

            imageSecond.setPixel(i, j, qRgb(sumPixel, sumPixel, sumPixel));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1


    //Double Loop for calculating the approximation
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {

            if(i == 0 || j == 0 || i == width-1 || j == height-1){//IF1
                image.setPixel(i,j,qRgb(255, 255, 255));//White contour
            }else{

                color=std::abs(imageFirst.pixel(i,j))+std::abs(imageSecond.pixel(i,j));//Getting color, approximate from both calculated images, using cast double to convert instantly during calculus
                image.setPixel(i,j,qRgb(color,color,color));
            }//ENDIF1


        }//ENDFOR2
    }//ENDFOR1




    DisplayedImage=DisplayedImage.fromImage(image);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label

}


/*
*
*END USEFULL FUNCTIONS
*
*/



/*
*
*START BIG SECTION / FIRST TAB IMAGE PROCESSING
*
*/



/*
*
*FILTERS
*
* Here the functions are just calling generic functions
* For Some which are not calling generic functions, this is special computation and this can't be improved by generic functions
*
*/


void FilteredImage::GrayScale(QLabel * label, QString path) const
{

    QPixmap DisplayedImage(path);//Pixmap to display

    QImage image = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height,gray;
    QRgb color;
    width=image.width();
    height=image.height();

    //Double Loop for Pixels
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {
        color= image.pixel(i, j);//Getting the parameters of pixel
        gray = (qGreen(color)+qBlue(color)+qRed(color))/3;//Converting in an average gray level
        image.setPixel(i, j, qRgb(gray, gray, gray));//Setting the Image Pixel
        }//ENDFOR2
    }//ENDFOR1

    DisplayedImage=DisplayedImage.fromImage(image);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label

}

void FilteredImage::Sobel_Filter(QLabel * label, QString path) const
{

    int ValueTab1[9] = {-1,0,1,-2,0,2,-1,0,1};//Declare the filter1 [-1 0 1 ; -2 0 2 ; -1 0 1]
    int ValueTab2[9] = {-1,-1,-1,0,0,0,1,1,1};//Declare the filter2 [-1 -2 -1 ; 0 0 0 ; 1 2 1]
    this->ApplyTwoFilters(label,path,ValueTab1,ValueTab2);// Call generic function to apply filters on image

}

void FilteredImage::Prewitt_Filter(QLabel * label, QString path) const
{

    int ValueTab1[9] = {-1,0,1,-1,0,1,-1,0,1};//Declare the filter1 [-1 0 1 ; -1 0 1 ; -1 0 1]
    int ValueTab2[9] = {-1,-2,-1,0,0,0,1,2,1};//Declare the filter2 [-1 -1 -1 ; 0 0 0 ; 1 1 1]
    this->ApplyTwoFilters(label,path,ValueTab1,ValueTab2);// Call generic function to apply filters on image

}

void FilteredImage::Scharr_Filter(QLabel * label, QString path) const
{

    int ValueTab1[9] = {3,0,-3,10,0,-10,3,0,-3};//Declare the filter1 [3 0 -3 ; 10 0 -10 ; 3 0 -3]
    int ValueTab2[9] = {3,10,3,0,0,0,-3,-10,-3};//Declare the filter2 [3 10 3 ; 0 0 0 ; -3 -10 -3]
    this->ApplyTwoFilters(label,path,ValueTab1,ValueTab2);// Call generic function to apply filters on image

}

void FilteredImage::Roberts_Filter(QLabel * label, QString path) const
{

    int ValueTab1[9] = {1,0,0,0,0,0,0,0,-1};//Declare the filter1 [1 0 0 ; 0 0 0 ; 0 0 -1]
    int ValueTab2[9] = {0,0,1,0,0,0,-1,0,0};//Declare the filter2 [0 0 1 ; 0 0 0 ; -1 0 0]
    this->ApplyTwoFilters(label,path,ValueTab1,ValueTab2);// Call generic function to apply filters on image

}

void FilteredImage::Laplacian_Filter(QLabel * label, QString path) const
{
    int ValueTab1[9] = {0,1,0,1,-4,1,0,1,0};//Declare the filter [0 1 0 ; 1 -4 1 ; 0 1 0]

    this->ApplyOneFilter(label,path,ValueTab1);// Call generic function to apply filter on image

}

void FilteredImage::BW_Filter(QLabel * label, QString path) const
{

    QPixmap DisplayedImage(path);//Pixmap to display

    QImage image = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height,sum;
    QRgb color;
    width=image.width();
    height=image.height();

    //Double Loop for Pixels
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {
        color= image.pixel(i, j);//Getting the parameters of pixel
        sum = (qGreen(color)+qBlue(color)+qRed(color))/3;//Converting in an average gray level
            if(sum>=127)//IF1 // Check if the final color will be black or white
            {
                image.setPixel(i, j, qRgb(255, 255, 255));//Setting the Image Pixel
            }else{
                image.setPixel(i, j, qRgb(0, 0, 0));//Setting the Image Pixel
            }//ENDIF1
        }//ENDFOR2
    }//ENDFOR1

    DisplayedImage=DisplayedImage.fromImage(image);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label

}


void FilteredImage::HP_Filter(QLabel * label, QString path) const
{

    double ValueTab1[9] = {0,-0.25,0,-0.25,2,-0.25,0,-0.25,0};//Declare the filter [0 -1/4 0 ; -1/4 2 -1/4 ; 0 -1/4 0]

    this->ApplyOneFilter(label,path,ValueTab1);// Call generic function to apply filter on image

}

void FilteredImage::LP_Filter(QLabel * label, QString path) const //Here template is fully used, we use double this is why we templated function
{

    double ValueTab1[9] = {1/9,1/9,1/9,1/9,1/9,1/9,1/9,1/9,1/9};//Declare the filter [1/9 1/9 1/9 ; 1/9 1/9 1/9 ; 1/9 1/9 1/9]

    this->ApplyOneFilter(label,path,ValueTab1);// Call generic function to apply filter on image

}


/*
*
*MORPHOLOGY
*
* Here the subtility between programs are more sharp so, to, differenciate dilation and erosion only one parameter saveColor is changed (closing = 255 , opening = 0)
* And Then we just change the symbol of comparision (closing = "<" , opening = ">") this is the only difference between both
*
*Morphoogy functions are not generized because of complexity of calculation, and difficulty of manipulate comparision signs in template
*
*/

void FilteredImage::Ero_Morph(QLabel * label, QString path) const
{
    QPixmap DisplayedImage(path);//Pixmap to display

    QImage imageEro = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height;
    QRgb color;
    width=imageEro.width();
    height=imageEro.height();

    QPixmap imageFirstPix(path);
    QImage imageFirst = imageFirstPix.toImage();//Converting to Image

    int saveColor,gray;


    //Double Loop for Pixels ignoring contour Minimal Value for Erosion
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            saveColor=255;


            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= imageEro.pixel(i+l, j+m);//Getting the parameters of pixel
                    gray = (qGreen(color)+qBlue(color)+qRed(color))/3;//Converting in an average gray level
                    if(gray<saveColor){saveColor=gray;}//IF/ENDIF1

                }//ENDFOR3
            }//ENDFOR4

            imageFirst.setPixel(i, j, qRgb(saveColor, saveColor, saveColor));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1



    //Double Loop for calculating the approximation
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {

            if(i == 0 || j == 0 || i == width-1 || j == height-1){//IF1
                imageEro.setPixel(i,j,qRgb(255, 255, 255));//White contour
            }else{

                color=imageFirst.pixel(i,j);
                imageEro.setPixel(i,j,qRgb(color,color,color));
            }//ENDIF1


        }//ENDFOR2
    }//ENDFOR1




    DisplayedImage=DisplayedImage.fromImage(imageEro);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label

}

void FilteredImage::Dila_Morph(QLabel * label, QString path) const
{
    QPixmap DisplayedImage(path);//Pixmap to display

    QImage imageDila = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height;
    QRgb color;
    width=imageDila.width();
    height=imageDila.height();

    QPixmap imageFirstPix(path);
    QImage imageFirst = imageFirstPix.toImage();//Converting to Image

    int saveColor,gray;


    //Double Loop for Pixels ignoring contour Maximal Value for Dilation
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            saveColor=0;


            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= imageDila.pixel(i+l, j+m);//Getting the parameters of pixel
                    gray = (qGreen(color)+qBlue(color)+qRed(color))/3;//Converting in an average gray level
                    if(gray>saveColor){saveColor=gray;}//IF/ENDIF1

                }//ENDFOR3
            }//ENDFOR4

            imageFirst.setPixel(i, j, qRgb(saveColor, saveColor, saveColor));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1



    //Double Loop for calculating the approximation
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {

            if(i == 0 || j == 0 || i == width-1 || j == height-1){//IF1
                imageDila.setPixel(i,j,qRgb(255, 255, 255));//White contour
            }else{

                color=imageFirst.pixel(i,j);
                imageDila.setPixel(i,j,qRgb(color,color,color));
            }//ENDIF1


        }//ENDFOR2
    }//ENDFOR1




    DisplayedImage=DisplayedImage.fromImage(imageDila);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label

}

void FilteredImage::Closing_Morph(QLabel * label, QString path) const //In this Function we will use Image Which will create image, then use crated image to create the final one //So it's a cascade image function
{
    QPixmap DisplayedImage(path);//Pixmap to display

    QImage imageClosing = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height;
    QRgb color;
    width=imageClosing.width();
    height=imageClosing.height();

    QPixmap imageFirstPix(path);
    QImage imageFirst = imageFirstPix.toImage();//Converting to Image

    QPixmap imageSecondPix(path);
    QImage imageSecond = imageSecondPix.toImage();//Converting to Image

    int saveColor,gray;


    //Double Loop for Pixels ignoring contour First Dilation Then Erosion //DILATION HERE
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            saveColor=0;


            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= imageClosing.pixel(i+l, j+m);//Getting the parameters of pixel
                    gray = (qGreen(color)+qBlue(color)+qRed(color))/3;//Converting in an average gray level
                    if(gray>saveColor){saveColor=gray;}//IF/ENDIF1

                }//ENDFOR3
            }//ENDFOR4

            imageFirst.setPixel(i, j, qRgb(saveColor, saveColor, saveColor));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1

    //Double Loop for Pixels ignoring contour First Dilation Then Erosion //EROSION HERE
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            saveColor=255;


            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= imageFirst.pixel(i+l, j+m);//Getting the parameters of pixel
                    gray = (qGreen(color)+qBlue(color)+qRed(color))/3;//Converting in an average gray level
                    if(gray<saveColor){saveColor=gray;}//IF/ENDIF1

                }//ENDFOR3
            }//ENDFOR4

            imageSecond.setPixel(i, j, qRgb(saveColor, saveColor, saveColor));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1



    //Double Loop for calculating the approximation
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {

            if(i == 0 || j == 0 || i == width-1 || j == height-1){//IF1
                imageClosing.setPixel(i,j,qRgb(255, 255, 255));//White contour
            }else{

                color=imageSecond.pixel(i,j);
                imageClosing.setPixel(i,j,qRgb(color,color,color));
            }//ENDIF1


        }//ENDFOR2
    }//ENDFOR1




    DisplayedImage=DisplayedImage.fromImage(imageClosing);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label

}

void FilteredImage::Opening_Morph(QLabel * label, QString path) const //In this Function we will use Image Which will create image, then use crated image to create the final one //So it's a cascade image function
{
    QPixmap DisplayedImage(path);//Pixmap to display

    QImage imageOpening = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height;
    QRgb color;
    width=imageOpening.width();
    height=imageOpening.height();

    QPixmap imageFirstPix(path);
    QImage imageFirst = imageFirstPix.toImage();//Converting to Image

    QPixmap imageSecondPix(path);
    QImage imageSecond = imageSecondPix.toImage();//Converting to Image

    int saveColor,gray;


    //Double Loop for Pixels ignoring contour First Dilation Then Erosion //EROSION HERE
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            saveColor=255;


            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= imageOpening.pixel(i+l, j+m);//Getting the parameters of pixel
                    gray = (qGreen(color)+qBlue(color)+qRed(color))/3;//Converting in an average gray level
                    if(gray<saveColor){saveColor=gray;}//IF/ENDIF1

                }//ENDFOR3
            }//ENDFOR4

            imageFirst.setPixel(i, j, qRgb(saveColor, saveColor, saveColor));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1

    //Double Loop for Pixels ignoring contour First Dilation Then Erosion //DILATION HERE
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            saveColor=0;


            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= imageFirst.pixel(i+l, j+m);//Getting the parameters of pixel
                    gray = (qGreen(color)+qBlue(color)+qRed(color))/3;//Converting in an average gray level
                    if(gray>saveColor){saveColor=gray;}//IF/ENDIF1

                }//ENDFOR3
            }//ENDFOR4

            imageSecond.setPixel(i, j, qRgb(saveColor, saveColor, saveColor));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1



    //Double Loop for calculating the approximation
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {

            if(i == 0 || j == 0 || i == width-1 || j == height-1){//IF1
                imageOpening.setPixel(i,j,qRgb(255, 255, 255));//White contour
            }else{

                color=imageSecond.pixel(i,j);
                imageOpening.setPixel(i,j,qRgb(color,color,color));
            }//ENDIF1


        }//ENDFOR2
    }//ENDFOR1




    DisplayedImage=DisplayedImage.fromImage(imageOpening);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label

}

void FilteredImage::WhiteTopHat_Morph(QLabel * label, QString path) const //In this Function we will use Image Which will create image, then deduce it from the base grayscaled image //So it's a cascade image function
{
    QPixmap DisplayedImage(path);//Pixmap to display

    QImage imageWTH = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height;
    QRgb color;
    width=imageWTH.width();
    height=imageWTH.height();

    QPixmap imageFirstPix(path);
    QImage imageFirst = imageFirstPix.toImage();//Converting to Image

    QPixmap imageSecondPix(path);
    QImage imageSecond = imageSecondPix.toImage();//Converting to Image

    int saveColor,gray;


    //Double Loop for Pixels ignoring contour First Dilation Then Erosion //EROSION HERE
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            saveColor=255;


            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= imageWTH.pixel(i+l, j+m);//Getting the parameters of pixel
                    gray = (qGreen(color)+qBlue(color)+qRed(color))/3;//Converting in an average gray level
                    if(gray<saveColor){saveColor=gray;}//IF/ENDIF1

                }//ENDFOR3
            }//ENDFOR4

            imageFirst.setPixel(i, j, qRgb(saveColor, saveColor, saveColor));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1

    //Double Loop for Pixels ignoring contour First Dilation Then Erosion //DILATION HERE
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            saveColor=0;


            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= imageFirst.pixel(i+l, j+m);//Getting the parameters of pixel
                    gray = (qGreen(color)+qBlue(color)+qRed(color))/3;//Converting in an average gray level
                    if(gray>saveColor){saveColor=gray;}//IF/ENDIF1

                }//ENDFOR3
            }//ENDFOR4

            imageSecond.setPixel(i, j, qRgb(saveColor, saveColor, saveColor));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1


    //CREATING THE GRAYSCALE TO DO THYE LAST CALCULATION imageGS

    QPixmap imageGSPix(path);
    QImage imageGS = imageGSPix.toImage();//Converting to Image

    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {
        color= imageWTH.pixel(i, j);//Getting the parameters of pixel
        gray = (qGreen(color)+qBlue(color)+qRed(color))/3;//Converting in an average gray level
        imageGS.setPixel(i, j, qRgb(gray, gray, gray));//Setting the Image Pixel
        }//ENDFOR2
    }//ENDFOR1



    //Double Loop for calculating the approximation
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {

            if(i == 0 || j == 0 || i == width-1 || j == height-1){//IF1
                imageWTH.setPixel(i,j,qRgb(255, 255, 255));//White contour
            }else{

                color=imageGS.pixel(i,j)-imageSecond.pixel(i,j);
                imageWTH.setPixel(i,j,qRgb(color,color,color));
            }//ENDIF1


        }//ENDFOR2
    }//ENDFOR1




    DisplayedImage=DisplayedImage.fromImage(imageWTH);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label

}


void FilteredImage::BlackTopHat_Morph(QLabel * label, QString path) const //In this Function we will use Image Which will create image, then deduce it from the base grayscaled image //So it's a cascade image function
{
    QPixmap DisplayedImage(path);//Pixmap to display

    QImage imageBTH = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height;
    QRgb color;
    width=imageBTH.width();
    height=imageBTH.height();

    QPixmap imageFirstPix(path);
    QImage imageFirst = imageFirstPix.toImage();//Converting to Image

    QPixmap imageSecondPix(path);
    QImage imageSecond = imageSecondPix.toImage();//Converting to Image

    int saveColor,gray;


    //Double Loop for Pixels ignoring contour First Dilation Then Erosion //DILATION HERE
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            saveColor=0;


            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= imageBTH.pixel(i+l, j+m);//Getting the parameters of pixel
                    gray = (qGreen(color)+qBlue(color)+qRed(color))/3;//Converting in an average gray level
                    if(gray>saveColor){saveColor=gray;}//IF/ENDIF1

                }//ENDFOR3
            }//ENDFOR4

            imageFirst.setPixel(i, j, qRgb(saveColor, saveColor, saveColor));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1

    //Double Loop for Pixels ignoring contour First Dilation Then Erosion //EROSION HERE
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            saveColor=255;


            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= imageFirst.pixel(i+l, j+m);//Getting the parameters of pixel
                    gray = (qGreen(color)+qBlue(color)+qRed(color))/3;//Converting in an average gray level
                    if(gray<saveColor){saveColor=gray;}//IF/ENDIF1

                }//ENDFOR3
            }//ENDFOR4

            imageSecond.setPixel(i, j, qRgb(saveColor, saveColor, saveColor));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1



    //CREATING THE GRAYSCALE TO DO THYE LAST CALCULATION imageGS

    QPixmap imageGSPix(path);
    QImage imageGS = imageGSPix.toImage();//Converting to Image

    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {
        color= imageBTH.pixel(i, j);//Getting the parameters of pixel
        gray = (qGreen(color)+qBlue(color)+qRed(color))/3;//Converting in an average gray level
        imageGS.setPixel(i, j, qRgb(gray, gray, gray));//Setting the Image Pixel
        }//ENDFOR2
    }//ENDFOR1



    //Double Loop for calculating the approximation
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {

            if(i == 0 || j == 0 || i == width-1 || j == height-1){//IF1
                imageBTH.setPixel(i,j,qRgb(255, 255, 255));//White contour
            }else{

                color=imageSecond.pixel(i,j)-imageGS.pixel(i,j);
                imageBTH.setPixel(i,j,qRgb(color,color,color));
            }//ENDIF1


        }//ENDFOR2
    }//ENDFOR1




    DisplayedImage=DisplayedImage.fromImage(imageBTH);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label

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

void FilteredImage::KirschTop_Filter(QLabel * label, QString path) const
{

    int ValueTab1[9] = {5,5,5,-3,0,-3,-3,-3,-3};//Declare the filter [5 5 5 ; -3 0 -3 ; -3 -3 -3]

    this->ApplyOneFilter(label,path,ValueTab1);// Call generic function to apply filter on image

}


void FilteredImage::KirschTopLeft_Filter(QLabel * label, QString path) const
{

    int ValueTab1[9] = {5,5,-3,5,0,-3,-3,-3,-3};//Declare the filter [5 5 -3 ; 5 0 -3 ; -3 -3 -3]

    this->ApplyOneFilter(label,path,ValueTab1);// Call generic function to apply filter on image

}


void FilteredImage::KirschLeft_Filter(QLabel * label, QString path) const
{

    int ValueTab1[9] = {5,-3,-3,5,0,-3,5,-3,-3};//Declare the filter [5 -3 -3 ; 5 0 -3 ; 5 -3 -3]

    this->ApplyOneFilter(label,path,ValueTab1);// Call generic function to apply filter on image

}


void FilteredImage::KirschBottomLeft_Filter(QLabel * label, QString path) const
{

    int ValueTab1[9] = {-3,-3,-3,5,0,-3,5,5,-3};//Declare the filter [-3 -3 -3 ; 5 0 -3 ; 5 5 -3]

    this->ApplyOneFilter(label,path,ValueTab1);// Call generic function to apply filter on image

}

void FilteredImage::KirschBottom_Filter(QLabel * label, QString path) const
{

    int ValueTab1[9] = {-3,-3,-3,-3,0,-3,5,5,5};//Declare the filter [-3 -3 -3 ;-3 0 -3 ; 5 5 5]

    this->ApplyOneFilter(label,path,ValueTab1);// Call generic function to apply filter on image

}

void FilteredImage::KirschBottomRight_Filter(QLabel * label, QString path) const
{

    int ValueTab1[9] = {-3,-3,-3,-3,0,5,-3,5,5};//Declare the filter [-3 -3 -3 ; -3 0 5 ; -3 5 5]

    this->ApplyOneFilter(label,path,ValueTab1);// Call generic function to apply filter on image

}

void FilteredImage::KirschRight_Filter(QLabel * label, QString path) const
{

    int ValueTab1[9] = {-3,-3,5,-3,0,5,-3,-3,5};//Declare the filter [-3 -3 5 ; -3 0 5 ; -3 -3 5]

    this->ApplyOneFilter(label,path,ValueTab1);// Call generic function to apply filter on image

}

void FilteredImage::KirschTopRight_Filter(QLabel * label, QString path) const
{

    int ValueTab1[9] = {-3,5,5,-3,0,5,-3,-3,-3};//Declare the filter [-3 5 5 ; -3 0 5 ; -3 -3 -3]

    this->ApplyOneFilter(label,path,ValueTab1); // Call generic function to apply filter on image

}

void FilteredImage::KirschCenter_Filter(QLabel * label, QString path) const
{

    QPixmap DisplayedImage(path);//Pixmap to display

    QImage image = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height;
    QRgb color;
    width=image.width();
    height=image.height();

    QPixmap imageTopPix(path);
    QImage imageTop = imageTopPix.toImage();//Converting to Image
    int ValueTab1[9] = {5,5,5,-3,0,-3,-3,-3,-3};//Declare the filter [5 5 5 ; -3 0 -3 ; -3 -3 -3]

    QPixmap imageBotPix(path);
    QImage imageBot = imageBotPix.toImage();//Converting to Image
    int ValueTab2[9] = {-3,-3,-3,-3,0,-3,5,5,5};//Declare the filter [-3 -3 -3 ;-3 0 -3 ; 5 5 5]

    QPixmap imageLeftPix(path);
    QImage imageLeft = imageLeftPix.toImage();//Converting to Image
    int ValueTab3[9] = {5,-3,-3,5,0,-3,5,-3,-3};//Declare the filter [5 -3 -3 ; 5 0 -3 ; 5 -3 -3]

    QPixmap imageRightPix(path);
    QImage imageRight = imageRightPix.toImage();//Converting to Image
    int ValueTab4[9] = {-3,-3,5,-3,0,5,-3,-3,5};//Declare the filter [-3 -3 5 ; -3 0 5 ; -3 -3 5]


    QPixmap image1Pix(path);
    QImage image1 = image1Pix.toImage();//Converting to Image
    int ValueTab5[9] = {5,5,-3,5,0,-3,-3,-3,-3};//Declare the filter [5 5 -3 ; 5 0 -3 ; -3 -3 -3]

    QPixmap image2Pix(path);
    QImage image2 = image2Pix.toImage();//Converting to Image
    int ValueTab6[9] = {-3,-3,-3,5,0,-3,5,5,-3};//Declare the filter [-3 -3 -3 ; 5 0 -3 ; 5 5 -3]

    QPixmap image3Pix(path);
    QImage image3 = image3Pix.toImage();//Converting to Image
    int ValueTab7[9] = {-3,-3,-3,-3,0,5,-3,5,5};//Declare the filter [-3 -3 -3 ; -3 0 5 ; -3 5 5]

    QPixmap image4Pix(path);
    QImage image4 = image4Pix.toImage();//Converting to Image
    int ValueTab8[9] = {-3,5,5,-3,0,5,-3,-3,-3};//Declare the filter [-3 5 5 ; -3 0 5 ; -3 -3 -3]



    int sumPixel,count=0;

    //CROSS

    //Double Loop for Pixels ignoring contour
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            sumPixel=0;
            count=0;

            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= image.pixel(i+l, j+m);//Getting the parameters of pixel

                    sumPixel+= ValueTab1[count]*color;//Setting the values in function of counter and matrix vector
                    count++;

                }//ENDFOR3
            }//ENDFOR4

            imageTop.setPixel(i, j, qRgb(sumPixel, sumPixel, sumPixel));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1


    count=0;
    //Double Loop for Pixels ignoring contour::2nd matrix
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {
            sumPixel=0;
            count=0;

            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= image.pixel(i+l, j+m);//Getting the parameters of pixel

                    sumPixel+= ValueTab2[count]*color;//Setting the values in function of counter and matrix vector
                    count++;

                }//ENDFOR3
            }//ENDFOR4

            imageBot.setPixel(i, j, qRgb(sumPixel, sumPixel, sumPixel));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1



    //Double Loop for Pixels ignoring contour
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            sumPixel=0;
            count=0;

            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= image.pixel(i+l, j+m);//Getting the parameters of pixel

                    sumPixel+= ValueTab3[count]*color;//Setting the values in function of counter and matrix vector
                    count++;

                }//ENDFOR3
            }//ENDFOR4

            imageLeft.setPixel(i, j, qRgb(sumPixel, sumPixel, sumPixel));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1


    count=0;
    //Double Loop for Pixels ignoring contour::2nd matrix
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {
            sumPixel=0;
            count=0;

            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= image.pixel(i+l, j+m);//Getting the parameters of pixel

                    sumPixel+= ValueTab4[count]*color;//Setting the values in function of counter and matrix vector
                    count++;

                }//ENDFOR3
            }//ENDFOR4

            imageRight.setPixel(i, j, qRgb(sumPixel, sumPixel, sumPixel));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1

    //ENDCROSS

    //4 OTHERS DIRECTIONS

    //Double Loop for Pixels ignoring contour
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            sumPixel=0;
            count=0;

            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= image.pixel(i+l, j+m);//Getting the parameters of pixel

                    sumPixel+= ValueTab5[count]*color;//Setting the values in function of counter and matrix vector
                    count++;

                }//ENDFOR3
            }//ENDFOR4

            image1.setPixel(i, j, qRgb(sumPixel, sumPixel, sumPixel));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1


    count=0;
    //Double Loop for Pixels ignoring contour::2nd matrix
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {
            sumPixel=0;
            count=0;

            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= image.pixel(i+l, j+m);//Getting the parameters of pixel

                    sumPixel+= ValueTab6[count]*color;//Setting the values in function of counter and matrix vector
                    count++;

                }//ENDFOR3
            }//ENDFOR4

            image2.setPixel(i, j, qRgb(sumPixel, sumPixel, sumPixel));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1



    //Double Loop for Pixels ignoring contour
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            sumPixel=0;
            count=0;

            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= image.pixel(i+l, j+m);//Getting the parameters of pixel

                    sumPixel+= ValueTab7[count]*color;//Setting the values in function of counter and matrix vector
                    count++;

                }//ENDFOR3
            }//ENDFOR4

            image3.setPixel(i, j, qRgb(sumPixel, sumPixel, sumPixel));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1


    count=0;
    //Double Loop for Pixels ignoring contour::2nd matrix
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {
            sumPixel=0;
            count=0;

            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= image.pixel(i+l, j+m);//Getting the parameters of pixel

                    sumPixel+= ValueTab8[count]*color;//Setting the values in function of counter and matrix vector
                    count++;

                }//ENDFOR3
            }//ENDFOR4

            image4.setPixel(i, j, qRgb(sumPixel, sumPixel, sumPixel));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1


    //END 4 OTHERS DIRECTIONS



    //Double Loop for calculating the approximation
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {

            if(i == 0 || j == 0 || i == width-1 || j == height-1){//IF1
                image.setPixel(i,j,qRgb(255, 255, 255));//White contour
            }else{

                color=std::abs(imageTop.pixel(i,j))+std::abs(imageBot.pixel(i,j))+std::abs(imageLeft.pixel(i,j))+std::abs(imageRight.pixel(i,j))+std::abs(image1.pixel(i,j))+std::abs(image2.pixel(i,j))+std::abs(image3.pixel(i,j))+std::abs(image4.pixel(i,j));//Getting color, approximate from both calculated images, using cast double to convert instantly during calculus
                image.setPixel(i,j,qRgb(color,color,color));
            }//ENDIF1


        }//ENDFOR2
    }//ENDFOR1




    DisplayedImage=DisplayedImage.fromImage(image);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label

}



void FilteredImage::KirschCross_Filter(QLabel * label, QString path) const
{

    QPixmap DisplayedImage(path);//Pixmap to display

    QImage image = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height;
    QRgb color;
    width=image.width();
    height=image.height();

    QPixmap imageTopPix(path);
    QImage imageTop = imageTopPix.toImage();//Converting to Image
    int ValueTab1[9] = {5,5,5,-3,0,-3,-3,-3,-3};//Declare the filter [5 5 5 ; -3 0 -3 ; -3 -3 -3]

    QPixmap imageBotPix(path);
    QImage imageBot = imageBotPix.toImage();//Converting to Image
    int ValueTab2[9] = {-3,-3,-3,-3,0,-3,5,5,5};//Declare the filter [-3 -3 -3 ;-3 0 -3 ; 5 5 5]

    QPixmap imageLeftPix(path);
    QImage imageLeft = imageLeftPix.toImage();//Converting to Image
    int ValueTab3[9] = {5,-3,-3,5,0,-3,5,-3,-3};//Declare the filter [5 -3 -3 ; 5 0 -3 ; 5 -3 -3]

    QPixmap imageRightPix(path);
    QImage imageRight = imageRightPix.toImage();//Converting to Image
    int ValueTab4[9] = {-3,-3,5,-3,0,5,-3,-3,5};//Declare the filter [-3 -3 5 ; -3 0 5 ; -3 -3 5]



    int sumPixel,count=0;

    //Double Loop for Pixels ignoring contour
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            sumPixel=0;
            count=0;

            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= image.pixel(i+l, j+m);//Getting the parameters of pixel

                    sumPixel+= ValueTab1[count]*color;//Setting the values in function of counter and matrix vector
                    count++;

                }//ENDFOR3
            }//ENDFOR4

            imageTop.setPixel(i, j, qRgb(sumPixel, sumPixel, sumPixel));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1


    count=0;
    //Double Loop for Pixels ignoring contour::2nd matrix
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {
            sumPixel=0;
            count=0;

            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= image.pixel(i+l, j+m);//Getting the parameters of pixel

                    sumPixel+= ValueTab2[count]*color;//Setting the values in function of counter and matrix vector
                    count++;

                }//ENDFOR3
            }//ENDFOR4

            imageBot.setPixel(i, j, qRgb(sumPixel, sumPixel, sumPixel));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1



    //Double Loop for Pixels ignoring contour
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {

            sumPixel=0;
            count=0;

            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= image.pixel(i+l, j+m);//Getting the parameters of pixel

                    sumPixel+= ValueTab3[count]*color;//Setting the values in function of counter and matrix vector
                    count++;

                }//ENDFOR3
            }//ENDFOR4

            imageLeft.setPixel(i, j, qRgb(sumPixel, sumPixel, sumPixel));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1


    count=0;
    //Double Loop for Pixels ignoring contour::2nd matrix
    for (int i = 1; i < width-1; i++)//FOR1
    {
        for (int j = 1; j < height-1; j++)//FOR2
        {
            sumPixel=0;
            count=0;

            for (int l = -1; l <= 1; l++)//FOR3 //Double loop intermadiate to see all neighbour
            {
                for (int m = -1; m <= 1; m++)//FOR4
                {

                    color= image.pixel(i+l, j+m);//Getting the parameters of pixel

                    sumPixel+= ValueTab4[count]*color;//Setting the values in function of counter and matrix vector
                    count++;

                }//ENDFOR3
            }//ENDFOR4

            imageRight.setPixel(i, j, qRgb(sumPixel, sumPixel, sumPixel));//Setting the Image Pixel

        }//ENDFOR2
    }//ENDFOR1


    //Double Loop for calculating the approximation
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {

            if(i == 0 || j == 0 || i == width-1 || j == height-1){//IF1
                image.setPixel(i,j,qRgb(255, 255, 255));//White contour
            }else{

                color=std::abs(imageTop.pixel(i,j))+std::abs(imageBot.pixel(i,j))+std::abs(imageLeft.pixel(i,j))+std::abs(imageRight.pixel(i,j));//Getting color, approximate from both calculated images, using cast double to convert instantly during calculus
                image.setPixel(i,j,qRgb(color,color,color));
            }//ENDIF1


        }//ENDFOR2
    }//ENDFOR1




    DisplayedImage=DisplayedImage.fromImage(image);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label

}


void FilteredImage::KirschNS_Filter(QLabel * label, QString path) const
{

    int ValueTab1[9] = {5,5,5,-3,0,-3,-3,-3,-3};//Declare the filter [5 5 5 ; -3 0 -3 ; -3 -3 -3]
    int ValueTab2[9] = {-3,-3,-3,-3,0,-3,5,5,5};//Declare the filter [-3 -3 -3 ;-3 0 -3 ; 5 5 5]
    this->ApplyTwoFilters(label,path,ValueTab1,ValueTab2);// Call generic function to apply filters on image

}

void FilteredImage::KirschEW_Filter(QLabel * label, QString path) const
{

    int ValueTab1[9] = {5,-3,-3,5,0,-3,5,-3,-3};//Declare the filter [5 -3 -3 ; 5 0 -3 ; 5 -3 -3]
    int ValueTab2[9] = {-3,-3,5,-3,0,5,-3,-3,5};//Declare the filter [-3 -3 5 ; -3 0 5 ; -3 -3 5]
    this->ApplyTwoFilters(label,path,ValueTab1,ValueTab2);// Call generic function to apply filters on image

}


/*
*
*END BIG SECTION / SECOND TAB KIRSCH/ORIENTED
*
*/


/*
*
*START BIG SECTION / THIRD TAB AESTHETICS
*
*/

void FilteredImage::BrightnessChange(QLabel* label, QString path, int toAdd)const // BRIGHTNESS CHANGE
{

    QPixmap DisplayedImage(path);//Pixmap to display

    QImage image = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height;
    QRgb color;
    width=image.width();
    height=image.height();

    //Double Loop for Pixels
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {
        color= image.pixel(i, j);//Getting the parameters of pixel
        int newColor[3]={qRed(color)+toAdd , qGreen(color)+toAdd , qBlue(color)+toAdd};//Create table to verify parameters properly

        for(int l = 0 ; l <= 3 ; l++){//Verify parameters //FOR3

            if(newColor[l]>255){
                newColor[l]=255;
            }else if(newColor[l]<0){
                newColor[l]=0;
            }

        }//ENDFOR3



        image.setPixel(i, j, qRgb(newColor[0], newColor[1], newColor[2]));//Setting the Image Pixel
        }//ENDFOR2
    }//ENDFOR1

    DisplayedImage=DisplayedImage.fromImage(image);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label
}

void FilteredImage::ContrastChange(QLabel* label, QString path, double factor)const // CONTRAST CHANGE
{

    QPixmap DisplayedImage(path);//Pixmap to display

    QImage image = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height;
    QRgb color;
    width=image.width();
    height=image.height();

    //Double Loop for Pixels
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {
        color= image.pixel(i, j);//Getting the parameters of pixel
        int newColor[3]={int(factor*(qRed(color)-127)+127) , int(factor*(qGreen(color)-127)+127) , int(factor*(qBlue(color)-127)+127)};//Create table to verify parameters properly

        for(int l = 0 ; l <= 3 ; l++){//Verify parameters //FOR3

            if(newColor[l]>255){
                newColor[l]=255;
            }else if(newColor[l]<0){
                newColor[l]=0;
            }

        }//ENDFOR3



        image.setPixel(i, j, qRgb(newColor[0], newColor[1], newColor[2]));//Setting the Image Pixel
        }//ENDFOR2
    }//ENDFOR1

    DisplayedImage=DisplayedImage.fromImage(image);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label
}


void FilteredImage::GammaChange(QLabel* label, QString path, double gamma)const //GAMMA CHANGE
{

    QPixmap DisplayedImage(path);//Pixmap to display

    QImage image = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height;
    QRgb color;
    width=image.width();
    height=image.height();


    //Double Loop for Pixels
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {
        color= image.pixel(i, j);//Getting the parameters of pixel
        int newColor[3]={int(std::pow(qRed(color)/255.0,100.0/gamma)*255) , int(std::pow(qGreen(color)/255.0,100.0/gamma)*255) , int(std::pow(qBlue(color)/255.0,100.0/gamma)*255)};//Create table to verify parameters properly

        for(int l = 0 ; l <= 3 ; l++){//Verify parameters //FOR3

            if(newColor[l]>255){
                newColor[l]=255;
            }else if(newColor[l]<0){
                newColor[l]=0;
            }

        }//ENDFOR3



        image.setPixel(i, j, qRgb(newColor[0], newColor[1], newColor[2]));//Setting the Image Pixel
        }//ENDFOR2
    }//ENDFOR1

    DisplayedImage=DisplayedImage.fromImage(image);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label
}


void FilteredImage::LOAChange(QLabel* label, QString path,int toAdd, double factor, double gamma)const //CHANGE CONTRAST GAMMA AND BRIGHT TOGETHER
{

    QPixmap DisplayedImage(path);//Pixmap to display

    QImage image = DisplayedImage.toImage();//Converting to Image

    //Variable Init
    int width,height;
    QRgb color;
    width=image.width();
    height=image.height();


    //Double Loop for Pixels
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {
        color= image.pixel(i, j);//Getting the parameters of pixel
        int newColor[3]={qRed(color)+toAdd , qGreen(color)+toAdd , qBlue(color)+toAdd};//Create table to verify parameters properly

        for(int l = 0 ; l <= 3 ; l++){//Verify parameters //FOR3

            if(newColor[l]>255){
                newColor[l]=255;
            }else if(newColor[l]<0){
                newColor[l]=0;
            }

        }//ENDFOR3



        image.setPixel(i, j, qRgb(newColor[0], newColor[1], newColor[2]));//Setting the Image Pixel
        }//ENDFOR2
    }//ENDFOR1

    //Double Loop for Pixels
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {
        color= image.pixel(i, j);//Getting the parameters of pixel
        int newColor[3]={int(factor*(qRed(color)-127)+127) , int(factor*(qGreen(color)-127)+127) , int(factor*(qBlue(color)-127)+127)};//Create table to verify parameters properly

        for(int l = 0 ; l <= 3 ; l++){//Verify parameters //FOR3

            if(newColor[l]>255){
                newColor[l]=255;
            }else if(newColor[l]<0){
                newColor[l]=0;
            }

        }//ENDFOR3



        image.setPixel(i, j, qRgb(newColor[0], newColor[1], newColor[2]));//Setting the Image Pixel
        }//ENDFOR2
    }//ENDFOR1



    //Double Loop for Pixels
    for (int i = 0; i < width; i++)//FOR1
    {
        for (int j = 0; j < height; j++)//FOR2
        {
        color= image.pixel(i, j);//Getting the parameters of pixel
        int newColor[3]={int(std::pow(qRed(color)/255.0,100.0/gamma)*255) , int(std::pow(qGreen(color)/255.0,100.0/gamma)*255) , int(std::pow(qBlue(color)/255.0,100.0/gamma)*255)};//Create table to verify parameters properly

        for(int l = 0 ; l <= 3 ; l++){//Verify parameters //FOR3

            if(newColor[l]>255){
                newColor[l]=255;
            }else if(newColor[l]<0){
                newColor[l]=0;
            }

        }//ENDFOR3



        image.setPixel(i, j, qRgb(newColor[0], newColor[1], newColor[2]));//Setting the Image Pixel
        }//ENDFOR2
    }//ENDFOR1

    DisplayedImage=DisplayedImage.fromImage(image);//Converting to a pixmap
    label->setPixmap(DisplayedImage);//Displaying on label
}


/*
*
*END BIG SECTION / THIRD TAB AESTHETICS
*
*/


/*
*
*START BIG SECTION / FOURTH TAB OVERVIEW
*
*/

/*
*
*END BIG SECTION / FOURTH TAB OVERVIEW
*
*/


