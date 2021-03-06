/*********************************************************************/
/* Copyright (c) 2011 - 2012, The University of Texas at Austin.     */
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

#ifndef MOVIE_CONTENT_H
#define MOVIE_CONTENT_H

#include "Content.h"
#include <boost/serialization/base_object.hpp>

enum ControlState
{
    STATE_PAUSED = 1 << 0,
    STATE_LOOP   = 1 << 1
};

class MovieContent : public Content
{
    Q_OBJECT

public:
    /** Create a MovieContent from the given uri. */
    explicit MovieContent( const QString& uri );

    /** Get the content type **/
    CONTENT_TYPE getType() const override;

    /**
     * Read movie informations from the source URI.
     * @return true on success, false if the URI is invalid or an error occured.
    **/
    bool readMetadata() override;

    static const QStringList& getSupportedExtensions();

    ControlState getControlState() const;

private slots:
    void play();
    void pause();

private:
    void createActions() override;

    friend class boost::serialization::access;

    // Default constructor required for boost::serialization
    MovieContent();

    template< class Archive >
    void serialize( Archive & ar, const unsigned int version )
    {
        // serialize base class information (with NVP for xml archives)
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( Content );
        if ( version >= 2 )
            ar & boost::serialization::make_nvp( "controlState", controlState_ );
    }

    ControlState controlState_;
};

BOOST_CLASS_VERSION( MovieContent, 2 )

#endif
