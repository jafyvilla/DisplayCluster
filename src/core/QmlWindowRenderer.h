/*********************************************************************/
/* Copyright (c) 2015, EPFL/Blue Brain Project                       */
/*                     Raphael Dumusc <raphael.dumusc@epfl.ch>       */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/*   1. Redistributions of source code must retain the above         */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer.                                                  */
/*                                                                   */
/*   2. Redistributions in binary form must reproduce the above      */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer in the documentation and/or other materials       */
/*      provided with the distribution.                              */
/*                                                                   */
/*    THIS  SOFTWARE IS PROVIDED  BY THE  UNIVERSITY OF  TEXAS AT    */
/*    AUSTIN  ``AS IS''  AND ANY  EXPRESS OR  IMPLIED WARRANTIES,    */
/*    INCLUDING, BUT  NOT LIMITED  TO, THE IMPLIED  WARRANTIES OF    */
/*    MERCHANTABILITY  AND FITNESS FOR  A PARTICULAR  PURPOSE ARE    */
/*    DISCLAIMED.  IN  NO EVENT SHALL THE UNIVERSITY  OF TEXAS AT    */
/*    AUSTIN OR CONTRIBUTORS BE  LIABLE FOR ANY DIRECT, INDIRECT,    */
/*    INCIDENTAL,  SPECIAL, EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES    */
/*    (INCLUDING, BUT  NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE    */
/*    GOODS  OR  SERVICES; LOSS  OF  USE,  DATA,  OR PROFITS;  OR    */
/*    BUSINESS INTERRUPTION) HOWEVER CAUSED  AND ON ANY THEORY OF    */
/*    LIABILITY, WHETHER  IN CONTRACT, STRICT  LIABILITY, OR TORT    */
/*    (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY WAY OUT    */
/*    OF  THE  USE OF  THIS  SOFTWARE,  EVEN  IF ADVISED  OF  THE    */
/*    POSSIBILITY OF SUCH DAMAGE.                                    */
/*                                                                   */
/* The views and conclusions contained in the software and           */
/* documentation are those of the authors and should not be          */
/* interpreted as representing official policies, either expressed   */
/* or implied, of The University of Texas at Austin.                 */
/*********************************************************************/

#ifndef QMLWINDOWRENDERER_H
#define QMLWINDOWRENDERER_H

#include "types.h"

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <QtGui/QGraphicsObject>

#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeItem>

/**
 * Provide a Qml representation of a ContentWindow on the Wall.
 */
class QmlWindowRenderer : public boost::noncopyable
{
public:
    /** Constructor. */
    QmlWindowRenderer( QDeclarativeEngine& engine,
                       QDeclarativeItem& parentItem,
                       ContentWindowPtr contentWindow );
    /** Destructor. */
    ~QmlWindowRenderer();

    /** Update the qml object with a new data model. */
    void update( ContentWindowPtr contentWindow );

    void setStackingOrder( int value );

    void preRenderUpdate( WallToWallChannel& wallChannel,
                          const QRect& visibleWallArea );
    void postRenderUpdate( WallToWallChannel& wallChannel );

    /** Get the WallContent. */
    WallContentPtr getWallContent();

    /** Get the ContentWindow. */
    ContentWindowPtr getContentWindow();

private:
    ContentWindowPtr contentWindow_;
    boost::scoped_ptr<QDeclarativeContext> windowContext_;
    QDeclarativeItem* windowItem_;
    WallContentPtr wallContent_;

    void setupPixelStreamItem();
    QDeclarativeItem* createQmlItem( const QUrl& url );
};

#endif // QMLWINDOWRENDERER_H
