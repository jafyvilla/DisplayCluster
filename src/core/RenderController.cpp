/*********************************************************************/
/* Copyright (c) 2014, EPFL/Blue Brain Project                       */
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

#include "RenderController.h"

#include "RenderContext.h"
#include "DisplayGroupRenderer.h"
#include "MarkerRenderer.h"

#include "DisplayGroup.h"
#include "Options.h"

#include <boost/make_shared.hpp>
#include <boost/bind.hpp>

RenderController::RenderController( RenderContextPtr renderContext )
    : renderContext_( renderContext )
    , displayGroupRenderer_( new DisplayGroupRenderer( renderContext ))
    , markerRenderer_( new MarkerRenderer )
    , syncQuit_( false )
    , syncDisplayGroup_( boost::make_shared<DisplayGroup>( QSize( )))
    , syncOptions_( boost::make_shared<Options>( ))
{
    renderContext_->addRenderable(markerRenderer_);

    syncDisplayGroup_.setCallback(boost::bind(&DisplayGroupRenderer::setDisplayGroup,
                                               displayGroupRenderer_.get(), _1));

    syncMarkers_.setCallback(boost::bind(&MarkerRenderer::setMarkers,
                                          markerRenderer_.get(), _1));

    syncOptions_.setCallback(boost::bind(&RenderController::setRenderOptions,
                                         this, _1));
}

DisplayGroupPtr RenderController::getDisplayGroup() const
{
    return syncDisplayGroup_.get();
}

void RenderController::synchronizeObjects(const SyncFunction& versionCheckFunc)
{
    syncQuit_.sync(versionCheckFunc);
    syncDisplayGroup_.sync(versionCheckFunc);
    syncMarkers_.sync(versionCheckFunc);
    syncOptions_.sync(versionCheckFunc);
}

bool RenderController::quitRendering() const
{
    return syncQuit_.get();
}

void RenderController::preRenderUpdate( WallToWallChannel& wallChannel )
{
    displayGroupRenderer_->preRenderUpdate( wallChannel );
}

void RenderController::postRenderUpdate( WallToWallChannel& wallChannel )
{
    displayGroupRenderer_->postRenderUpdate( wallChannel );
}

void RenderController::updateQuit()
{
    syncQuit_.update( true );
}

void RenderController::updateDisplayGroup( DisplayGroupPtr displayGroup )
{
    syncDisplayGroup_.update( displayGroup );
}

void RenderController::updateOptions( OptionsPtr options )
{
    syncOptions_.update( options );
}

void RenderController::updateMarkers( MarkersPtr markers )
{
    syncMarkers_.update( markers );
}

void RenderController::updatePixelStream( deflect::PixelStreamFramePtr /*frame*/ )
{
    // TODO //
//    typedef boost::shared_ptr<PixelStream> PixelStreamPtr;
//    PixelStreamPtr pixelStream = pixelStreamFactory_.getObject(frame->uri);
//    pixelStream->setNewFrame(frame);
//    pixelStream->setFrameIndex(frameIndex_);
}

void RenderController::setRenderOptions( OptionsPtr options )
{
    renderContext_->setBackgroundColor( options->getBackgroundColor( ));
    renderContext_->displayTestPattern( options->getShowTestPattern( ));
    renderContext_->displayFps( options->getShowStatistics( ));

    markerRenderer_->setVisible( options->getShowTouchPoints( ));

    displayGroupRenderer_->setRenderingOptions( options );
}

