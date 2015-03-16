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

#ifndef SVG_H
#define SVG_H

#include "WallContent.h"

#include "types.h"
#include "GLQuad.h"

#include <QtSvg/QtSvg>
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFramebufferObject>

typedef boost::shared_ptr<QGLFramebufferObject> FBOPtr;

/**
 * Hold texture FBO and region rendered.
 */
struct SVGTextureData
{
    /** Frame Buffer Object */
    FBOPtr fbo;

    /** Normalized svg region */
    QRectF region;
};

class SVG : public WallContent
{
public:
    SVG( const QString& uri );

    bool isValid() const;
    QSize getSize() const;

    QSize getTextureSize() const;
    const QRectF& getTextureRegion() const;
    void updateTexture( const QSize& textureSize, const QRectF& svgRegion );
    void render() override;
    void renderPreview() override;

private:
    QRectF svgExtents_;
    QSvgRenderer svgRenderer_;

    SVGTextureData textureData_;
    FBOPtr previewFbo_;
    GLQuad quad_;

    void preRenderUpdate( ContentWindowPtr window,
                          const QRect& wallArea ) override;

    void generatePreviewTexture();
    bool setImageData( const QByteArray& imageData );
    void renderToTexture( const QRectF& svgRegion, FBOPtr targetFbo );
    QRectF getViewBox( const QRectF& svgRegion );
    FBOPtr createMultisampledFBO( const QSize& size );

    void saveGLState();
    void restoreGLState();
};

#endif
