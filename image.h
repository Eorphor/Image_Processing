#ifndef IMAGE_H
#define IMAGE_H

#include <QPixmap>
#include <QImage>
#include<QLabel>
#include "mainwindow.h"

/*
*Quoted IN DEV, are in developpement components, they aren't influencing results, can be uncommented but no actions or not as we wish
*/

class Image
{
public:
    //Constructors
    Image(){}
    Image(QString imagePath) : realPath(imagePath){}
    Image(const Image& I): realPath(I.get_path()){}

    //Destructor
    ~Image(){}

    //Accessors

    QString get_path() const{return realPath;}

    //Mutators

    void set_path(const QString& newPath){ realPath = newPath;}

protected:

    QString realPath; //Path to the image

public:
    //Functions
    virtual void Display(QLabel*); //Defined in the main class and can be used in the derived one


    virtual void TemporaryFolder(QString){} //Force to declare in the derived class //Create Temp Folder
    virtual void DeleteTemporaryFolder(QString){}//Same //Delete Temp Folder
    virtual void TemporarySave(QString, QLabel*, QString){} //Same //Save in temp folder



};

class FilteredImage : public Image
{
public:
    //Constructors
    FilteredImage() : Image(){}
    FilteredImage(QString imagePath) : Image(imagePath) , savingPath(Image::get_path()) {} //Path to the image saving folder imediatly declared
    FilteredImage(const FilteredImage& FI) : Image(FI.get_path()) , savingPath(FI.get_savingPath()) {}

    //Destructor
    ~FilteredImage(){}

    //Accessors

    QString get_savingPath() const{return savingPath;}

    //Mutators

    void set_savingPath(const QString& newPath){savingPath = newPath;}

protected:

    QString savingPath;

public:

    /*
    *
    *UTILITY FUNCTIONS
    *
    * Poly from Image Class, we can see const and not const
    * When the const keyword is not used, it permit us to manipulate variables and change values this is why there is no const
    * While const is here, we assume that there will be no variable changes
    *
    */

    void TemporaryFolder(QString);
    void DeleteTemporaryFolder(QString) const;
    void TemporarySave(QString, QLabel*, QString);

    /*
    *
    *FILTER FUNCTIONS
    *
    * The Names of functions are explicit and says exactly whatr is does
    *
    */

    void GrayScale(QLabel*,QString) const;
    void Sobel_Filter(QLabel*, QString) const;
    void Prewitt_Filter(QLabel*, QString) const;
    void Scharr_Filter(QLabel*, QString) const;
    void Roberts_Filter(QLabel*, QString) const;
    void Laplacian_Filter(QLabel*, QString) const;
    void BW_Filter(QLabel*, QString) const;//Black or White
    void HP_Filter(QLabel*, QString) const;//High Pass
    void LP_Filter(QLabel*, QString) const;//Low Pass

    /*
    *
    *MORPHOLOGY FUNCTIONS
    *
    * The Names of functions are explicit and says exactly whatr is does
    *
    */

      void Ero_Morph(QLabel*, QString) const;
      void Dila_Morph(QLabel*, QString) const;
      void Closing_Morph(QLabel*, QString) const;
      void Opening_Morph(QLabel*, QString) const;
      void WhiteTopHat_Morph(QLabel*, QString) const;
      void BlackTopHat_Morph(QLabel*, QString) const;


      /*
      *
      *KIRSCH FUNCTIONS
      *
      * The Names of functions are explicit and says exactly whatr is does
      *
      */

      void KirschTop_Filter(QLabel*, QString) const;
      void KirschTopLeft_Filter(QLabel*, QString) const;
      void KirschLeft_Filter(QLabel*, QString) const;
      void KirschBottomLeft_Filter(QLabel*, QString) const;
      void KirschBottom_Filter(QLabel*, QString) const;
      void KirschBottomRight_Filter(QLabel*, QString) const;
      void KirschRight_Filter(QLabel*, QString) const;
      void KirschTopRight_Filter(QLabel*, QString) const;

      void KirschCenter_Filter(QLabel*, QString) const;//8 Directions
      void KirschCross_Filter(QLabel*, QString) const;//North South East West
      void KirschNS_Filter(QLabel*, QString) const;//North South
      void KirschEW_Filter(QLabel*, QString) const;//East West

      /*
      *
      *AESTHETICS FUNCTIONS
      *
      * The Names of functions are explicit and says exactly whatr is does
      *
      */

      void BrightnessChange(QLabel*, QString, int) const;
      void ContrastChange(QLabel*, QString, double) const;
      void GammaChange(QLabel*, QString, double) const;

      void LOAChange(QLabel*, QString, int, double, double) const; // Load 3 sliders values at once

      /*
      *
      *USEFUL FUNCTIONS
      *
      *Functions used to Wrap the code using this kind of functions to hav less repetitions and more readability
      *Functions like this will be adaptable, that mean that, we can just copy paste these functions and this will compute filters on image
      *These functions have been developped to optimize, quality and adaptability of the program
      *Some Complex functions that needs more than two array or more cascade stay unchanged to avoid complex function impossible to debug for a future user
      *
      *Functions are templated inside a non template class which is called member templating
      *It is usefull to deal with multiple types of input, like int and double
      *
      */


      template <typename T> void ApplyOneFilter(QLabel* , QString , T[9] )const;


      template <typename Ta, typename Tb> void ApplyTwoFilters(QLabel* , QString ,Ta[9], Tb[9] )const;


};

#endif // IMAGE_H
