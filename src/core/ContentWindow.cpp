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

#include "ContentWindow.h"
#include "Content.h"
#include "DisplayGroup.h"
#include "ContentInteractionDelegate.h"
#include "config.h"
#include "log.h"

// Specialized delegate implementations
#include "PixelStreamInteractionDelegate.h"
#include "ZoomInteractionDelegate.h"
#if ENABLE_PDF_SUPPORT
#  include "PDFInteractionDelegate.h"
#endif

IMPLEMENT_SERIALIZE_FOR_XML(ContentWindow)

ContentWindow::ContentWindow()
{
}

ContentWindow::ContentWindow(ContentPtr content)
{
    setContent(content);

    adjustSize( SIZE_1TO1 );
}

ContentWindow::~ContentWindow()
{
}

void ContentWindow::setContent(ContentPtr content)
{
    if(content_)
    {
        content_->disconnect(this, SLOT(setContentDimensions(int, int)));
        content_->disconnect(this, SIGNAL(contentModified()));
    }

    content_ = content;

    if(content_)
    {
        content_->getDimensions(contentWidth_, contentHeight_);

        connect(content.get(), SIGNAL(dimensionsChanged(int, int)),
                this, SLOT(setContentDimensions(int, int)));

        connect(content.get(), SIGNAL(modified()),
                this, SIGNAL(contentModified()));
    }
    else
        contentWidth_ = contentHeight_ = 0;

    createInteractionDelegate();
}

ContentPtr ContentWindow::getContent() const
{
    return content_;
}

void ContentWindow::createInteractionDelegate()
{
    if(!content_)
    {
        interactionDelegate_.reset();
        return;
    }

    switch (content_->getType())
    {
    case CONTENT_TYPE_PIXEL_STREAM:
        interactionDelegate_.reset(new PixelStreamInteractionDelegate(*this));
        break;
#if ENABLE_PDF_SUPPORT
    case CONTENT_TYPE_PDF:
        interactionDelegate_.reset(new PDFInteractionDelegate(*this));
        break;
#endif
    default:
        interactionDelegate_.reset(new ZoomInteractionDelegate(*this));
        break;
    }
}

DisplayGroupPtr ContentWindow::getDisplayGroup() const
{
    return displayGroup_.lock();
}

void ContentWindow::setDisplayGroup(DisplayGroupPtr displayGroup)
{
    displayGroup_ = displayGroup;
}

ContentInteractionDelegate& ContentWindow::getInteractionDelegate() const
{
    return *interactionDelegate_;
}

void ContentWindow::moveToFront(ContentWindowInterface * source)
{
    ContentWindowInterface::moveToFront(source);

    if(source != this)
    {
        DisplayGroupPtr displayGroup = getDisplayGroup();
        if (displayGroup)
            displayGroup->moveContentWindowToFront(shared_from_this());
        else
            put_flog(LOG_DEBUG, "The DisplayGroupMangerPtr is invalid");
    }
}

void ContentWindow::close(ContentWindowInterface * source)
{
    ContentWindowInterface::close(source);

    if(source != this)
    {
        getDisplayGroup()->removeContentWindow(shared_from_this());
    }
}

QPointF ContentWindow::getWindowCenterPosition() const
{
    return QPointF(coordinates_.x() + 0.5 * coordinates_.width(),
                   coordinates_.y() + 0.5 * coordinates_.height());
}

void ContentWindow::centerPositionAround(const QPointF& position,
                                                const bool constrainToWindowBorders)
{
    if(position.isNull())
        return;

    double newX = position.x() - 0.5 * coordinates_.width();
    double newY = position.y() - 0.5 * coordinates_.height();

    if (constrainToWindowBorders)
    {
        if (newX + coordinates_.width() > 1.0)
            newX = 1.0-coordinates_.width();
        if (newY + coordinates_.height() > 1.0)
            newY = 1.0-coordinates_.height();

        newX = std::max(0.0, newX);
        newY = std::max(0.0, newY);
    }

    setPosition(newX, newY);
}