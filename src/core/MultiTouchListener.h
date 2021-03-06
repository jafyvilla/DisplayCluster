/*********************************************************************/
/* Copyright (c) 2013, EPFL/Blue Brain Project                       */
/*                     Daniel Nachbaur <daniel.nachbaur@epfl.ch>     */
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

#ifndef MULTI_TOUCH_LISTENER_H
#define MULTI_TOUCH_LISTENER_H

#include "types.h"

#include <TUIO/TuioListener.h>
#include <TUIO/TuioClient.h>

#include <QObject>
#include <QtGui/QtEvents>
#include <QtGui/QGraphicsView>

/**
 * Listen to TUIO events and transmit the touch points to a target QGraphicsView.
 */
class MultiTouchListener : public QObject, public TUIO::TuioListener
{
    Q_OBJECT

public:
    MultiTouchListener( QGraphicsView* graphicsView );
    ~MultiTouchListener();

    void addTuioObject( TUIO::TuioObject* tobj );
    void updateTuioObject( TUIO::TuioObject* tobj );
    void removeTuioObject( TUIO::TuioObject* tobj );

    void addTuioCursor( TUIO::TuioCursor* tcur );
    void updateTuioCursor( TUIO::TuioCursor* tcur );
    void removeTuioCursor( TUIO::TuioCursor* tcur );

    void refresh( TUIO::TuioTime frameTime );

signals:
    void touchPointAdded( int id, QPointF position );
    void touchPointUpdated( int id, QPointF position );
    void touchPointRemoved( int id );

private:
    QPointF getScenePos( TUIO::TuioCursor* tcur ) const;
    void fillBegin( QTouchEvent::TouchPoint& touchPoint ) const;
    void fill( QTouchEvent::TouchPoint& touchPoint,
               const QTouchEvent::TouchPoint& prevPoint ) const;
    void handleEvent( TUIO::TuioCursor* tcur,
                      const QEvent::Type eventType );

    QMap< int, QTouchEvent::TouchPoint > touchPointMap_;

    QGraphicsView* graphicsView_;

    TUIO::TuioClient client_;
};

#endif
