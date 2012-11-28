#ifndef QSFMLCANVASDIALOG_HPP
#define QSFMLCANVASDIALOG_HPP

#include <SFML/Graphics.hpp>
#include <Qt/qdialog.h>
#include <Qt/qtimer.h>

class QSFMLCANVASDIALOG : public QDialog, public sf::RenderWindow
{
    public:
    ////////////////////////////////////////////////////////////
    /// Construct the QSFMLCANVASDIALOG
    ///
    /// \param Parent :    Parent of the widget
    /// \param Position :  Position of the widget
    /// \param Size :      Size of the widget
    /// \param FrameTime : Frame duration, in milliseconds (0 by default)
    ///
    ////////////////////////////////////////////////////////////
    QSFMLCANVASDIALOG(QWidget* Parent);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~QSFMLCANVASDIALOG();

    unsigned int GetWidth  	() const {return (unsigned int)this->size().width();}
    unsigned int GetHeight () const {return (unsigned int)  this->size().height();};
    bool Activate(bool Active) {  if (Active) return SetActive(); else return true;}


private :

    ////////////////////////////////////////////////////////////
    /// Notification for the derived class that moment is good
    /// for doing initializations
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnInit();

    ////////////////////////////////////////////////////////////
    /// Notification for the derived class that moment is good
    /// for doing its update and drawing stuff
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnUpdate();

    ////////////////////////////////////////////////////////////
    /// Return the paint engine used by the widget to draw itself
    ///
    ////////////////////////////////////////////////////////////
    virtual QPaintEngine* paintEngine() const;

    ////////////////////////////////////////////////////////////
    /// Called when the widget is shown ;
    /// we use it to initialize our SFML window
    ///
    ////////////////////////////////////////////////////////////
    virtual void showEvent(QShowEvent*);

    ////////////////////////////////////////////////////////////
    /// Called when the widget needs to be painted ;
    /// we use it to display a new frame
    ///
    ////////////////////////////////////////////////////////////
    virtual void paintEvent(QPaintEvent*);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    QTimer myTimer;       ///< Timer used to update the view
    bool   myInitialized; ///< Tell whether the SFML window has been initialized or not;
};


#endif
